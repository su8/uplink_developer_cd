
#ifdef WIN32
#include <strstrea.h>
#else
#include <strstream.h>
#endif

#include "app/serialise.h"

#include "world/scheduler/warningevent.h"


WarningEvent::WarningEvent ()
{

	event = NULL;

}

WarningEvent::~WarningEvent ()
{
}


void WarningEvent::SetEvent ( UplinkEvent *newevent )
{

	event = newevent;

}


void WarningEvent::Run ()
{

	if ( event )		event->RunWarning ();

}

char *WarningEvent::GetShortString ()
{

	char *shortstring = new char [32];
	sprintf ( shortstring, "Warning Event" );
	return shortstring;

}

char *WarningEvent::GetLongString ()
{
	
	char *eventstring = event->GetLongString ();

	ostrstream longstring;
	longstring << "Warning Event for :\n"
			   << eventstring
			   << '\x0';

	delete [] eventstring;

	return longstring.str ();

}

void WarningEvent::Load  ( FILE *file )
{

	LoadID ( file );

	UplinkEvent::Load ( file );

	bool eventexists;
	fread ( &eventexists, sizeof(eventexists), 1, file );

	if ( eventexists ) {
		int objectID;
		fread ( &objectID, sizeof(objectID), 1, file );
		event = (UplinkEvent *) CreateUplinkObject ( objectID );
		event->Load ( file );
	}
	else 
		event = NULL;

	LoadID_END ( file );

}

void WarningEvent::Save  ( FILE *file )
{

	SaveID ( file );

	UplinkEvent::Save ( file );

	bool eventexists = ( event != NULL );
	fwrite ( &eventexists, sizeof(eventexists), 1, file );
	
	if ( eventexists ) {
		int objectID = event->GetOBJECTID ();
		fwrite ( &objectID, sizeof(objectID), 1, file );
		event->Save ( file );
	}

	SaveID_END ( file );

}

void WarningEvent::Print ()
{

	printf ( "Warning Event : \n" );
	
	UplinkEvent::Print ();	

	if ( event ) event->Print ();
	
}
	
char *WarningEvent::GetID ()
{

	return "EVT_WARN";

}


int   WarningEvent::GetOBJECTID ()
{

	return OID_WARNINGEVENT;
		
}
