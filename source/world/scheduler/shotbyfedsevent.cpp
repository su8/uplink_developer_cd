
#ifdef WIN32
#include <strstrea.h>
#else
#include <strstream.h>
#endif

#include "gucci.h"

#include "app/app.h"
#include "app/serialise.h"
#include "app/globals.h"

#include "game/game.h"

#include "world/world.h"
#include "world/scheduler/shotbyfedsevent.h"
#include "world/generator/consequencegenerator.h"

#include "options/options.h"


ShotByFedsEvent::ShotByFedsEvent ()
{

	reason = NULL;

}

ShotByFedsEvent::~ShotByFedsEvent ()
{

	if ( reason ) delete [] reason;

}

void ShotByFedsEvent::SetName ( char *newname )
{

	UplinkAssert ( strlen (newname) < SIZE_PERSON_NAME );
	strcpy ( name, newname );
		
}

void ShotByFedsEvent::SetReason ( char *newreason )
{

	reason = new char [strlen(newreason)+1];
	strcpy ( reason, newreason );

}

void ShotByFedsEvent::Run ()
{

	Person *person = game->GetWorld ()->GetPerson ( name );
	UplinkAssert (person);

	person->SetStatus ( PERSON_STATUS_DEAD );

	ConsequenceGenerator::ShotByFeds ( person, reason );

}

char *ShotByFedsEvent::GetShortString ()
{

	char *shortstring = new char [strlen(name) + 32];
	sprintf ( shortstring, "ShotByFeds for %s", name );
	return shortstring;

}

char *ShotByFedsEvent::GetLongString ()
{

	ostrstream longstring;
	longstring << "Shot By Feds Event\n"
			   << "For : " << name << "\n"
			   << "Reason : " << reason 
			   << '\x0';
	return longstring.str ();

}

void ShotByFedsEvent::Load  ( FILE *file )
{

	LoadID ( file );

	UplinkEvent::Load ( file );

	fread ( name, sizeof(name), 1, file );
	reason = LoadDynamicString ( file );

	LoadID_END ( file );

}

void ShotByFedsEvent::Save  ( FILE *file )
{

	SaveID ( file );

	UplinkEvent::Save ( file );

	fwrite ( name, sizeof(name), 1, file );
	SaveDynamicString ( reason, file );

	SaveID_END ( file );
		
}

void ShotByFedsEvent::Print ()
{

	printf ( "ShotByFedsEvent : for %s\n", name );
	UplinkEvent::Print ();
	printf ( "Reason : %s\n", reason );
	
}
	
char *ShotByFedsEvent::GetID ()
{

	return "EVT_SHOT";

}

int   ShotByFedsEvent::GetOBJECTID ()
{

	return OID_SHOTBYFEDSEVENT;
		
}
