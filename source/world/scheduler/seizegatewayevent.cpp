
#ifdef WIN32
#include <strstrea.h>
#else
#include <strstream.h>
#endif

#include "soundgarden.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/serialise.h"
#include "app/opengl.h"
#include "app/opengl_interface.h"

#include "mainmenu/mainmenu.h"

#include "game/game.h"
#include "game/data/data.h"

#include "world/world.h"
#include "world/player.h"
#include "world/message.h"
#include "world/generator/consequencegenerator.h"
#include "world/scheduler/seizegatewayevent.h"

#include "options/options.h"



SeizeGatewayEvent::SeizeGatewayEvent ()
{

	reason = NULL;
	gateway_id = 0;

}

SeizeGatewayEvent::~SeizeGatewayEvent ()
{

	if ( reason ) delete [] reason;

}

void SeizeGatewayEvent::SetName ( char *newname )
{

	UplinkAssert ( strlen(newname) < SIZE_PERSON_NAME );
	strcpy ( name, newname );

}

void SeizeGatewayEvent::SetReason ( char *newreason )
{

	reason = new char [strlen(newreason)+1];
	strcpy ( reason, newreason );

}

void SeizeGatewayEvent::SetGatewayID ( int newid )
{

	gateway_id = newid;

}

void SeizeGatewayEvent::Run ()
{
	
	Person *person = game->GetWorld ()->GetPerson ( name );
	UplinkAssert (person);

	ConsequenceGenerator::SeizeGateway ( person, reason );

	//
	// If this is the player, then it's GAME OVER
	//

	if ( strcmp ( name, "PLAYER" ) == 0 ) {

		if ( gateway_id == game->GetWorld ()->GetPlayer ()->gateway.id ) {

			ostrstream deathmsg;
			
			deathmsg << "Disavowed by Uplink Corporation at " << game->GetWorld ()->date.GetLongString ()
					 << "\nFor " << reason
					 << "\n" << '\x0';

			game->GameOver ( deathmsg.str () );

			delete [] deathmsg.str ();

		}
		else {

			// The player's old gateway was seized
			// Not quite as severe - not game over

			Message *m = new Message ();
			m->SetTo ( "PLAYER" );
			m->SetFrom ( "Uplink Corporation" );
			m->SetSubject ( "Gateway seizure" );
			m->SetBody ( "Your old gateway was recently seized by federal agents, shortly after you blew it up.\n"
						 "It seems that no evidence remains of the crime that you didn't commit.\n\n" 
						 "A very lucky escape, if you ask us.\n" );
			m->Send ();

			// Uplink Corporation won't like you for this (it's reckless after all)
			// Other hackers love this - everybody knows you did it but 
			// you got away with it scott free!

			game->GetWorld ()->GetPlayer ()->rating.ChangeNeuromancerScore ( NEUROMANCERCHANGE_GATEWAYNEARMISS );
			game->GetWorld ()->GetPlayer ()->rating.ChangeUplinkScore ( UPLINKCHANGE_GATEWAYNEARMISS );

		}

	}

}

void SeizeGatewayEvent::RunWarning ()
{

	//
	// Agents are near (2 of em!)
	//

	game->GetWorld ()->GetPlayer ()->gateway.IncrementProximity ( 2 );
	
}

char *SeizeGatewayEvent::GetShortString ()
{

	char *shortstring = new char [strlen(name) + 32];
	sprintf ( shortstring, "Seize Gateway of %s", name );
	return shortstring;

}

char *SeizeGatewayEvent::GetLongString ()
{

	ostrstream longstring;
	longstring << "Seize Gateway Event\n"
			   << "For : " << name << "\n"
			   << "GatewayID : " << gateway_id << "\n"
			   << "Reason : " << reason 
			   << '\x0';
	return longstring.str ();

}


void SeizeGatewayEvent::Load  ( FILE *file )
{

	LoadID ( file );

	UplinkEvent::Load ( file );

	fread ( name, sizeof(name), 1, file );
	reason = LoadDynamicString (file);
	fread ( &gateway_id, sizeof(gateway_id), 1, file );

	LoadID_END ( file );

}

void SeizeGatewayEvent::Save  ( FILE *file )
{

	SaveID ( file );

	UplinkEvent::Save ( file );

	fwrite ( name, sizeof(name), 1, file );
	SaveDynamicString ( reason, file );
	fwrite ( &gateway_id, sizeof(gateway_id), 1, file );

	SaveID_END ( file );

}

void SeizeGatewayEvent::Print ()
{

	printf ( "Seize Gateway Event for %s, Gateway no. %d:\n", name, gateway_id );
	UplinkEvent::Print ();
	printf ( "%s\n", reason );


}
	
char *SeizeGatewayEvent::GetID ()
{

	return "EVT_SEZ";

}

int   SeizeGatewayEvent::GetOBJECTID ()
{

	return OID_SEIZEGATEWAYEVENT;

}

