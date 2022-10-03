
#include "gucci.h"

#include "app/app.h"
#include "app/globals.h"

#include "world/scheduler/uplinkevent.h"


UplinkEvent::UplinkEvent ()
{
}

UplinkEvent::~UplinkEvent ()
{
}
	
void UplinkEvent::SetRunDate ( Date *newrundate )
{

	UplinkAssert (newrundate);
	rundate.SetDate ( newrundate );

}
	
void UplinkEvent::Run ()
{

	UplinkAbort ( "This MUST be overridden" );

}

void UplinkEvent::RunWarning ()
{
}

char *UplinkEvent::GetShortString ()
{

	char *shortstring = new char [32];
	sprintf ( shortstring, "Uplink Event" );
	return shortstring;
		
}

char *UplinkEvent::GetLongString ()
{

	char *longstring = new char [32];
	sprintf ( longstring, "Uplink Event" );
	return longstring;

}

void UplinkEvent::Load  ( FILE *file )
{

	LoadID ( file );
	
	rundate.Load ( file );

	LoadID_END ( file );
		
}

void UplinkEvent::Save  ( FILE *file )
{

	SaveID ( file );

	rundate.Save ( file );

	SaveID_END ( file );

}

void UplinkEvent::Print ()
{

	printf ( "UplinkEvent:\n" );
	rundate.Print ();

}
	
char *UplinkEvent::GetID ()
{

	UplinkAbort ( "This MUST be overridden" );
	return NULL;

}

int UplinkEvent::GetOBJECTID ()
{

	UplinkAbort ( "This MUST be overridden" );
	return -1;

}
