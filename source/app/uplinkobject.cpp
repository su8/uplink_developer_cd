// UplinkObject.cpp: implementation of the UplinkObject class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>

#include "gucci.h"

#include "app/uplinkobject.h"
#include "app/globals.h"
#include "app/serialise.h"

#ifdef _DEBUG
//#define  SAVEFILE_VERBOSE
//#define  SAVEFILE_PRINTPROGRESS
#endif

static int indent;							// Used for formatting Load/Save
static void PrintIndent ();					//


UplinkObject::UplinkObject()
{
}

UplinkObject::~UplinkObject()
{
}

void UplinkObject::Load ( FILE *file )
{
}

void UplinkObject::Save ( FILE *file )
{
}

void UplinkObject::Print ()
{
}

void UplinkObject::Update ()
{
}

char *UplinkObject::GetID ()
{
	return "UOBJ";
}

char *UplinkObject::GetID_END ()
{

	char *id_end = new char [SIZE_SAVEID_END];
	sprintf ( id_end, "%s_END", GetID () );
	return id_end;

}

int UplinkObject::GetOBJECTID ()
{

	return OID_UNKNOWN;

}

static void PrintIndent ()
{

	for ( int i = 0; i < indent-1; ++i )
		printf ( "\t" );

}

void UplinkObject::LoadID ( FILE *file )
{
	
#ifdef SAVEFILE_PRINTPROGRESS
	++indent;
	PrintIndent ();
	printf ( "Started  loading  %s\n", GetID () );
#endif

#ifdef SAVEFILE_VERBOSE
	char id [SIZE_SAVEID];
	fread ( id, SIZE_SAVEID, 1, file );
	UplinkAssert ( id );	
	UplinkAssert ( strcmp ( id, GetID () ) == 0 );
#endif

}

void UplinkObject::SaveID ( FILE *file )
{

#ifdef SAVEFILE_PRINTPROGRESS
	++indent;
	PrintIndent ();
	printf ( "Started  saving  %s\n", GetID () );
#endif

#ifdef SAVEFILE_VERBOSE
	fwrite ( GetID (), SIZE_SAVEID, 1, file );
#endif

}

void UplinkObject::LoadID_END ( FILE *file )
{

#ifdef SAVEFILE_PRINTPROGRESS
	PrintIndent ();
	printf ( "Finished loading  %s\n", GetID () );
	--indent;
#endif

#ifdef SAVEFILE_VERBOSE
	char id_end [SIZE_SAVEID_END];
	fread ( id_end, SIZE_SAVEID_END, 1, file );
	UplinkAssert ( id_end );
	UplinkAssert ( strcmp ( id_end, GetID_END () ) == 0 );
#endif

}

void UplinkObject::SaveID_END ( FILE *file )
{

#ifdef SAVEFILE_PRINTPROGRESS
	PrintIndent ();
	printf ( "Finished saving  %s\n", GetID () );
	--indent;
#endif
	
#ifdef SAVEFILE_VERBOSE
	fwrite ( GetID_END (), SIZE_SAVEID_END, 1, file );
#endif

}




