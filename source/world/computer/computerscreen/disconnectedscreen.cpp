
#include "app/globals.h"
#include "app/serialise.h"

#include "world/vlocation.h"
#include "world/computer/computerscreen/disconnectedscreen.h"

DArray <char *> DisconnectedScreen::loginslost;


DisconnectedScreen::DisconnectedScreen()
{
	
	nextpage = -1;
	textmessage = NULL;

}

DisconnectedScreen::~DisconnectedScreen()
{
	
	if ( textmessage ) delete [] textmessage;

}

void DisconnectedScreen::SetNextPage ( int newnextpage )
{

	nextpage = newnextpage;

}

void DisconnectedScreen::SetTextMessage ( char *newtextmessage )
{

	if ( textmessage ) delete [] textmessage;
	textmessage = new char [strlen(newtextmessage)+1];
	strcpy ( textmessage, newtextmessage );

}

void DisconnectedScreen::AddLoginLost ( char *ip )
{

    char *ipcopy = new char [SIZE_VLOCATION_IP];
    strcpy ( ipcopy, ip );
    loginslost.PutData ( ipcopy );

}

void DisconnectedScreen::ClearLoginsLost ()
{

    DeleteDArrayData ( &loginslost );
    loginslost.Empty ();

}

void DisconnectedScreen::Load  ( FILE *file )
{

	LoadID ( file );

	ComputerScreen::Load ( file );

	fread ( &nextpage, sizeof(nextpage), 1, file );	
	textmessage = LoadDynamicString ( file );

	LoadID_END ( file );

}

void DisconnectedScreen::Save  ( FILE *file )
{

	SaveID ( file );

	ComputerScreen::Save ( file );

	fwrite ( &nextpage, sizeof(nextpage), 1, file );
	SaveDynamicString ( textmessage, file );

	SaveID_END ( file );

}

void DisconnectedScreen::Print ()
{

	printf ( "DisconnectedScreen : \n" );
	ComputerScreen::Print ();
	printf ( "NextPage = %d, TextMessage = %s\n", 
				nextpage, textmessage );

}
	
char *DisconnectedScreen::GetID ()
{

	return "SCR_DISC";

}

int DisconnectedScreen::GetOBJECTID ()
{

	return OID_DISCONNECTEDSCREEN;

}
			

