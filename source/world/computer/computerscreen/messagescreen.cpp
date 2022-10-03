// MessageScreen.cpp: implementation of the MessageScreen class.
//
//////////////////////////////////////////////////////////////////////

#include "app/globals.h"
#include "app/serialise.h"

#include "world/computer/computerscreen/messagescreen.h"

MessageScreen::MessageScreen()
{
	
	nextpage = -1;
	textmessage = NULL;
	buttonmessage = NULL;
	mailthistome = false;

}

MessageScreen::~MessageScreen()
{
	
	if ( textmessage ) delete [] textmessage;
	if ( buttonmessage ) delete [] buttonmessage;

}

void MessageScreen::SetNextPage ( int newnextpage )
{

	nextpage = newnextpage;

}

void MessageScreen::SetTextMessage ( char *newtextmessage )
{

	if ( textmessage ) delete [] textmessage;
	textmessage = new char [strlen(newtextmessage)+1];
	strcpy ( textmessage, newtextmessage );

}

void MessageScreen::SetButtonMessage ( char *newbuttonmessage )
{

	if ( buttonmessage ) delete [] buttonmessage;
	buttonmessage = new char [strlen(newbuttonmessage)+1];
	strcpy ( buttonmessage, newbuttonmessage );

}

void MessageScreen::SetMailThisToMe ( bool newvalue )
{

	mailthistome = newvalue;

}

void MessageScreen::Load  ( FILE *file )
{

	LoadID ( file );

	ComputerScreen::Load ( file );

	fread ( &nextpage, sizeof(nextpage), 1, file );	
	textmessage = LoadDynamicString ( file );
	buttonmessage = LoadDynamicString ( file );
	fread ( &mailthistome, sizeof(mailthistome), 1, file );

	LoadID_END ( file );

}

void MessageScreen::Save  ( FILE *file )
{

	SaveID ( file );

	ComputerScreen::Save ( file );

	fwrite ( &nextpage, sizeof(nextpage), 1, file );
	SaveDynamicString ( textmessage, file );
	SaveDynamicString ( buttonmessage, file );
	fwrite (  &mailthistome, sizeof(mailthistome), 1, file );

	SaveID_END ( file );

}

void MessageScreen::Print ()
{

	printf ( "MessageScreen : \n" );
	ComputerScreen::Print ();
	printf ( "NextPage = %d, TextMessage = %s, ButtonMessage = %s, MailThisToMe = %d\n", 
				nextpage, textmessage, buttonmessage, mailthistome );

}
	
char *MessageScreen::GetID ()
{

	return "SCR_MESS";

}

int MessageScreen::GetOBJECTID ()
{

	return OID_MESSAGESCREEN;

}
			

