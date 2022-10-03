// Message.cpp: implementation of the Message class.
//
//////////////////////////////////////////////////////////////////////

#include "gucci.h"

#include "game/game.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/serialise.h"

#include "world/world.h"
#include "world/message.h"
#include "world/computer/databank.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Message::Message()
{
	
	subject = NULL;
	body = NULL;

	SetTo ( " " );
	SetFrom ( " " );
	SetSubject ( " " );
	SetBody ( " " );

	data = NULL;

}
 
Message::~Message()
{

	if ( subject ) delete [] subject;
	if ( body ) delete [] body;
	if ( data ) delete data;

    DeleteLListData ( &links );
    DeleteBTreeData ( &codes );

}

void Message::SetTo	( char *newto )
{

	UplinkAssert ( strlen (newto) < SIZE_PERSON_NAME );
	strcpy ( to, newto );

}

void Message::SetFrom ( char *newfrom )
{

	UplinkAssert ( strlen (newfrom) < SIZE_PERSON_NAME );
	strcpy ( from, newfrom );

}

void Message::SetSubject ( char *newsubject )
{

	if ( subject ) delete [] subject;
	subject = new char [ strlen(newsubject) + 1 ];
	strcpy ( subject, newsubject );

}

void Message::SetBody ( char *newbody )
{

	if ( body ) delete [] body;
	body = new char [ strlen (newbody) + 1 ];
	strcpy ( body, newbody );

}

void Message::SetDate ( Date *newdate )
{

	date.SetDate ( newdate );

}

void Message::GiveLink ( char *ip )
{

	char *theip = new char [SIZE_VLOCATION_IP];
	UplinkAssert (strlen(ip) < SIZE_VLOCATION_IP );
	strcpy ( theip, ip );
	links.PutData (theip);

}

void Message::GiveCode ( char *ip, char *code )
{

	char *thecode = new char [strlen(code)+1];
	strcpy ( thecode, code );
    codes.PutData ( ip, thecode );

}

void Message::AttachData ( Data *newdata )
{

	// Copy newdata
	data = new Data ( newdata );	

}

Data *Message::GetData ()
{

	return data;

}

char *Message::GetSubject ()
{

	UplinkAssert ( subject );
	return subject;

}

char *Message::GetBody ()
{

	UplinkAssert ( body );
	return body;

}

void Message::Send ()
{

	SetDate ( &(game->GetWorld ()->date) );

	Person *person = game->GetWorld ()->GetPerson ( to );

	if ( person ) {

		person->GiveMessage ( this );

	}
	else {

		// Send an error message to the sender
		Message *msg = new Message ();
		msg->SetTo ( from );
		msg->SetFrom ( "MAIL SERVER" );
		msg->SetSubject ( "Failed to deliver this email - unknown recipient" );
		msg->SetBody ( body );
		msg->Send ();

	}

}

void Message::Load ( FILE *file )
{

	LoadID ( file );

	LoadDynamicString ( to, SIZE_PERSON_NAME, file );
	LoadDynamicString ( from, SIZE_PERSON_NAME, file );

	subject = LoadDynamicString ( file );
	body = LoadDynamicString ( file );

	date.Load ( file );

	LoadLList ( &links, file );
	LoadBTree ( &codes, file );

	LoadID_END ( file );

}

void Message::Save ( FILE *file )
{

	SaveID ( file );

	SaveDynamicString ( to, file );
	SaveDynamicString ( from, file );

	SaveDynamicString ( subject, file );
	SaveDynamicString ( body, file );

	date.Save ( file );

	SaveLList ( &links, file );
	SaveBTree ( &codes, file );

	SaveID_END ( file );

}

void Message::Print ()
{

	printf ( "Message : \n" );
	printf ( "\tTo   = %s\n", to );
	printf ( "\tFrom = %s\n", from );
	printf ( "\tSubject = %s\n", subject );
	printf ( "\tbody = %s\n", body );

	PrintLList ( &links );
	PrintBTree ( &codes );

	date.Print ();

}
	
char *Message::GetID ()
{

	return "MESSAGE";

}

int Message::GetOBJECTID ()
{

	return OID_MESSAGE;

}
