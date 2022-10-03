
#ifdef WIN32
#include <strstrea.h>
#else
#include <strstream.h>
#endif

#include "soundgarden.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/serialise.h"

#include "game/game.h"

#include "world/world.h"
#include "world/vlocation.h"
#include "world/generator/consequencegenerator.h"
#include "world/scheduler/arrestevent.h"

#include "options/options.h"



ArrestEvent::ArrestEvent ()
{

	reason = NULL;
	ip = NULL;

}

ArrestEvent::~ArrestEvent ()
{

	if ( reason ) delete [] reason;
	if ( ip ) delete [] ip;

}

void ArrestEvent::SetName ( char *newname )
{

	UplinkAssert ( strlen(newname) < SIZE_PERSON_NAME );
	strcpy ( name, newname );

}

void ArrestEvent::SetReason ( char *newreason )
{

	reason = new char [strlen(newreason)+1];
	strcpy ( reason, newreason );

}

void ArrestEvent::SetIP ( char *newip ) 
{

	ip = new char [strlen(newip)+1];
	strcpy ( ip, newip );

}

void ArrestEvent::Run ()
{
	
	Person *person = game->GetWorld ()->GetPerson ( name );
	UplinkAssert (person);

	person->SetStatus ( PERSON_STATUS_INJAIL );

	// Look up the computer they messed with

	Computer *comp = NULL;

	if ( ip ) {
		VLocation *vl = game->GetWorld ()->GetVLocation ( ip );
		UplinkAssert (vl);
		comp = vl->GetComputer ();
		UplinkAssert (comp);
	}

	ConsequenceGenerator::Arrested ( person, comp, reason );

}

char *ArrestEvent::GetShortString ()
{

	char *shortstring = new char [strlen(name) + 16];
	sprintf ( shortstring, "Arrest for %s", name );
	return shortstring;
}

char *ArrestEvent::GetLongString ()
{

	ostrstream longstring;
	longstring << "ArrestEvent:\n"
			   << "For : " << name << "\n"
			   << "Reason : " << reason
			   << '\x0';

	return longstring.str ();

}


void ArrestEvent::Load  ( FILE *file )
{

	LoadID ( file );

	UplinkEvent::Load ( file );

	fread ( name, sizeof(name), 1, file );
	
	reason = LoadDynamicString (file);
	ip     = LoadDynamicString (file);

	LoadID_END ( file );

}

void ArrestEvent::Save  ( FILE *file )
{

	SaveID ( file );

	UplinkEvent::Save ( file );

	fwrite ( name, sizeof(name), 1, file );
	
	SaveDynamicString ( reason, file );
	SaveDynamicString ( ip, file );

	SaveID_END ( file );

}

void ArrestEvent::Print ()
{

	printf ( "Arrest Event for %s:\n", name );
	UplinkEvent::Print ();
	
	printf ( "%s\n", reason );
	if ( ip ) printf ( "IP:%s\n", ip );


}
	
char *ArrestEvent::GetID ()
{

	return "EVT_ARR";

}

int   ArrestEvent::GetOBJECTID ()
{

	return OID_ARRESTEVENT;

}

