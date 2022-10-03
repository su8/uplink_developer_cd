
#include "gucci.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/serialise.h"

#include "world/company/news.h"


News::News ()
{

	details = NULL;
	NEWSTYPE = NEWS_TYPE_NONE;

    strcpy ( headline, " " );
    strcpy ( data1, " " );
    strcpy ( data2, " " );

}

News::~News ()
{

	if ( details ) delete [] details;
		
}

void News::SetDate ( Date *newdate )
{

	date.SetDate ( newdate );

}

void News::SetHeadline ( char *newheadline )
{

	UplinkAssert ( strlen ( newheadline ) < SIZE_NEWS_HEADLINE );
	strcpy ( headline, newheadline );

}

void News::SetDetails ( char *newdetails )
{

	details = new char [strlen(newdetails)+1];
	strcpy ( details, newdetails );

}

char *News::GetDetails ()
{

	return details;

}

void News::SetData ( int newNEWSTYPE, char *newdata1, char *newdata2 )
{

	NEWSTYPE = newNEWSTYPE;

	if ( newdata1 ) {
		UplinkAssert (strlen(newdata1) < SIZE_NEWS_DATA);
		strcpy ( data1, newdata1 );
	}
	else
		strcpy ( data1, " " );

	if ( newdata2 ) {
		UplinkAssert (strlen(newdata2) < SIZE_NEWS_DATA);
		strcpy ( data2, newdata2 );
	}
	else
		strcpy ( data2, " " );

}

void News::Load  ( FILE *file )
{
	
	LoadID ( file );

	date.Load ( file );
	fread ( &NEWSTYPE,	sizeof(NEWSTYPE), 1, file );

	LoadDynamicString ( headline, SIZE_NEWS_HEADLINE, file );
	LoadDynamicString ( data1, SIZE_NEWS_DATA, file );
	LoadDynamicString ( data2, SIZE_NEWS_DATA, file );

	details = LoadDynamicString ( file );

	LoadID_END ( file );

}

void News::Save  ( FILE *file )
{
	
	SaveID ( file );

	date.Save ( file );
	fwrite ( &NEWSTYPE, sizeof(NEWSTYPE), 1, file );

	SaveDynamicString ( headline, file );
	SaveDynamicString ( data1, file );
	SaveDynamicString ( data2, file );

	SaveDynamicString ( details, file );

	SaveID_END ( file );

}

void News::Print ()
{

	printf ( "News: %s\n", headline );
	date.Print ();
	printf ( "%s\n", details );
	
	printf ( "NEWSTYPE:%d\n", NEWSTYPE );
	printf ( "data1 = %s\n", data1 );
	printf ( "data2 = %s\n", data2 );
		
}
	
char *News::GetID ()
{

	return "NEWS";

}

int News::GetOBJECTID ()
{

	return OID_NEWS;

}
