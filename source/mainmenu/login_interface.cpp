// LoginInterface.cpp: implementation of the LoginInterface class.
//
//////////////////////////////////////////////////////////////////////

#ifdef WIN32
#include <direct.h>
#include <windows.h>
#include <io.h>
#else
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#endif

#include <GL/gl.h>

#include <GL/glu.h> /* glu extention library */

#include "eclipse.h"
#include "soundgarden.h"
#include "redshirt.h"

#include "app/app.h"
#include "app/opengl_interface.h"
#include "app/globals.h"
#include "app/opengl.h"
#include "app/miscutils.h"

#include "game/game.h"
#include "game/scriptlibrary.h"

#include "interface/interface.h"
#include "interface/localinterface/localinterface.h"
#include "interface/remoteinterface/remoteinterface.h"

#include "network/network.h"
#include "network/networkclient.h"

#include "options/options.h"

#include "mainmenu/mainmenu.h"
#include "mainmenu/login_interface.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LoginInterface::LoginInterface()
{

}

LoginInterface::~LoginInterface()
{

}

DArray <char *> *LoginInterface::ListExistingGames ()
{

	DArray <char *> *existing = new DArray <char *> ();

#ifdef WIN32

	char searchstring [128];
	sprintf ( searchstring, "%s*.usr", app->userpath );

	_finddata_t thisfile;
	long fileindex = _findfirst ( searchstring, &thisfile );

	int exitmeplease = 0;

	while ( fileindex != -1 && !exitmeplease ) {

		char *newname = new char [128];
		strcpy ( newname, thisfile.name );
		char *p = strchr ( newname, '.' );
		if ( p ) *p = '\x0';

		existing->PutData ( newname );
		exitmeplease = _findnext ( fileindex, &thisfile );

	}

#else
	char userdir [128];
	sprintf ( userdir, "%s", app->userpath );
	DIR *dir = opendir( userdir );
	if (dir != NULL) {
	    struct dirent *entry = readdir ( dir );

	    while (entry != NULL) {
	    
		char *p = strstr(entry->d_name, ".usr");
		if ( p ) {
		    *p = '\x0';
	      
		    char *newname = new char [128];
		    strcpy ( newname, entry->d_name );
		    existing->PutData ( newname );
		}
	    
		entry = readdir ( dir );
	    
	    }
	  
	    closedir( dir );
	}
#endif

	return existing;

}

void LoginInterface::NewGameClick ( Button *button )
{

#ifndef TESTGAME

	// Do the start sequence

	app->GetOptions ()->SetOptionValue ( "game_firsttime", 1 ) ;
	
	app->GetMainMenu ()->RunScreen ( MAINMENU_FIRSTLOAD );
	GciTimerFunc ( 2000, ScriptLibrary::RunScript, 30 );

#else

	// Skip it

	app->GetMainMenu ()->RunScreen ( MAINMENU_LOADING );
	display ();

	game->NewGame ();
	app->GetMainMenu ()->Remove ();

#endif

    if ( app->GetOptions ()->IsOptionEqualTo ( "sound_musicenabled", 1 ) ) {			
        //SgPlaylist_Play ("ambient");
        SgPlaylist_Play ("main");
	}

}

void LoginInterface::ProceedClick ( Button *button )
{

	UplinkAssert ( EclGetButton ( "userid_name" ) );	
	char username [256];
	strcpy ( username, EclGetButton ( "userid_name" )->caption );

	app->GetMainMenu ()->RunScreen ( MAINMENU_LOADING );
	display ();

	app->LoadGame ( username );

    if ( app->GetOptions ()->IsOptionEqualTo ( "sound_musicenabled", 1 ) ) {		
        //SgPlaylist_Play ("ambient");
        SgPlaylist_Play ("main");
	}

}

void LoginInterface::OptionsClick ( Button *button )
{

	app->GetMainMenu ()->RunScreen ( MAINMENU_OPTIONS );
//    SgPlaylist_NextSong();

}

void LoginInterface::ExitGameClick ( Button *button )
{

#ifdef DEMOGAME

    app->GetMainMenu ()->RunScreen ( MAINMENU_CLOSING );

#else

	app->Close ();

#endif

}

void LoginInterface::LargeTextBoxDraw ( Button *button, bool highlighted, bool clicked )
{

	SetColour ( "TitleText" );
	int ypos = (button->y + button->height / 2) + 5;

	GciDrawText ( button->x, ypos, button->caption, HELVETICA_18 );

}

void LoginInterface::UserIDButtonDraw ( Button *button, bool highlighted, bool clicked )
{

	UplinkAssert ( button );

	int screenheight = app->GetOptions ()->GetOptionValue ( "graphics_screenheight" );
	glScissor ( button->x, screenheight - (button->y + button->height), button->width, button->height );	
	glEnable ( GL_SCISSOR_TEST );

	// Draw a background colour

	SetColour ( "PasswordBoxBackground" );
	
	glBegin ( GL_QUADS );

		glVertex2i ( button->x, button->y );
		glVertex2i ( button->x + button->width - 1, button->y );
		glVertex2i ( button->x + button->width - 1, button->y + button->height );
		glVertex2i ( button->x, button->y + button->height );

	glEnd ();

	// Draw the text

	SetColour ( "DefaultText" );

	text_draw ( button, highlighted, clicked );

	// Draw a border

	if ( highlighted || clicked ) border_draw ( button );

	glDisable ( GL_SCISSOR_TEST );

}

void LoginInterface::CodeButtonDraw ( Button *button, bool highlighted, bool clicked )
{

	UplinkAssert (button);

	int screenheight = app->GetOptions ()->GetOptionValue ( "graphics_screenheight" );
	glScissor ( button->x, screenheight - (button->y + button->height), button->width, button->height );	
	glEnable ( GL_SCISSOR_TEST );

	// Draw a background colour

	SetColour ( "PasswordBoxBackground" );
	
	glBegin ( GL_QUADS );

		glVertex2i ( button->x, button->y );
		glVertex2i ( button->x + button->width - 1, button->y );
		glVertex2i ( button->x + button->width - 1, button->y + button->height );
		glVertex2i ( button->x, button->y + button->height );

	glEnd ();

	// Print the text

	SetColour ( "DefaultText" );

	char *caption = new char [strlen(button->caption) + 1];
	for ( int i = 0; i < strlen(button->caption); ++i )
		caption [i] = '*';

	caption [strlen(button->caption)] = '\x0';
	GciDrawText ( button->x + 10, button->y + 10, caption, BITMAP_15 );

	delete [] caption;

	// Draw a box around the text if highlighted

	if ( highlighted || clicked )
		border_draw ( button );

	glDisable ( GL_SCISSOR_TEST );

}

void LoginInterface::UserNameClick ( Button *button )
{

	EclRegisterCaptionChange ( "userid_name", button->caption );
	EclRegisterCaptionChange ( "userid_code", "PASSWORD" );

}

void LoginInterface::CommsClick ( Button *button )
{

	UplinkAssert ( app->GetNetwork ()->STATUS == NETWORK_CLIENT );

    EclReset ( app->GetOptions ()->GetOptionValue ("graphics_screenwidth"),
			   app->GetOptions ()->GetOptionValue ("graphics_screenheight") );

	app->GetNetwork ()->GetClient ()->SetClientType ( CLIENT_COMMS );
	
}

void LoginInterface::StatusClick	( Button *button )
{

	UplinkAssert ( app->GetNetwork ()->STATUS == NETWORK_CLIENT );

    EclReset ( app->GetOptions ()->GetOptionValue ("graphics_screenwidth"),
			   app->GetOptions ()->GetOptionValue ("graphics_screenheight") );

	app->GetNetwork ()->GetClient ()->SetClientType ( CLIENT_STATUS );

}

bool LoginInterface::ReturnKeyPressed ()
{

	ProceedClick ( NULL );
	return true;

}

void LoginInterface::Create ()
{	

	if ( !IsVisible () ) {
		
		// Play some music

		if ( app->GetOptions ()->IsOptionEqualTo ( "sound_musicenabled", 1 ) ) {
			
            //SgPlaylist_Play ("action");
            SgPlaylist_Play ("main");

		}

		MainMenuScreen::Create ();

		int screenw = app->GetOptions ()->GetOptionValue ("graphics_screenwidth");
		int screenh = app->GetOptions ()->GetOptionValue ("graphics_screenheight");

		if ( app->GetNetwork ()->STATUS == NETWORK_CLIENT ) {

			EclRegisterButton ( 20, 20, 110, 20, "COMMS", "Run as a dumb client, showing the communications status", "client_comms" );
			EclRegisterButtonCallback ( "client_comms", CommsClick );
			EclRegisterButton ( 20, 50, 110, 20, "STATUS", "Run as a dumb client, showing your characters status", "client_status" );
			EclRegisterButtonCallback ( "client_status", StatusClick );

		}

		// New game

        int buttonsX = SX(600);
        int buttonsY = SY(270);

		EclRegisterButton ( buttonsX, buttonsY, 32, 32, "", "Create a new user's account", "newgame" );		
		button_assignbitmaps ( "newgame", "mainmenu/newgame.tif", "mainmenu/newgame_h.tif", "mainmenu/newgame_c.tif" );	
		EclRegisterButton ( buttonsX - 100, buttonsY + 5, 100, 20, "New User", "Create a new user's account", "newgame_text" );
		EclRegisterButtonCallbacks ( "newgame_text", LargeTextBoxDraw, NewGameClick, button_click, button_highlight );

		// TODO : Look at this alpha stuff - it don't work no more
		if ( app->GetNetwork ()->STATUS == NETWORK_CLIENT ) {
			EclRegisterButtonCallbacks ( "newgame", imagebutton_draw, NULL, NULL, NULL );
			EclGetButton ( "newgame" )->image_standard->SetAlpha ( ALPHA_DISABLED );
		}
		else {
			EclRegisterButtonCallbacks ( "newgame", imagebutton_draw, NewGameClick, button_click, button_highlight );			
		}
		
		// Options

		EclRegisterButton ( buttonsX, buttonsY + 40, 32, 32, "", "Advanced options", "options" );		
		button_assignbitmaps ( "options", "mainmenu/options.tif", "mainmenu/options_h.tif", "mainmenu/options_c.tif" );
		EclRegisterButtonCallbacks ( "options", imagebutton_draw, OptionsClick, button_click, button_highlight ) ;

		EclRegisterButton ( buttonsX - 80, buttonsY + 45, 80, 20, "Options", "Advanced options", "options_text" );
		EclRegisterButtonCallbacks ( "options_text", LargeTextBoxDraw, OptionsClick, button_click, button_highlight );

		// Exit

		EclRegisterButton ( buttonsX, buttonsY + 80, 32, 32, "", "Exit Uplink", "exitgame" );		
		button_assignbitmaps ( "exitgame", "mainmenu/exitgame.tif", "mainmenu/exitgame_h.tif", "mainmenu/exitgame_c.tif" );
		EclRegisterButtonCallbacks ( "exitgame", imagebutton_draw, ExitGameClick, button_click, button_highlight );

		EclRegisterButton ( buttonsX - 45, buttonsY + 85, 50, 20, "Exit", "Exit Uplink", "exitgame_text" );
		EclRegisterButtonCallbacks ( "exitgame_text", LargeTextBoxDraw, ExitGameClick, button_click, button_highlight );

		// UserID box

        int useridX = SX(320) - 110;
        int useridY = SY(265);

		EclRegisterButton ( useridX, useridY, 220, 120, "", "", "userid_image" );
		button_assignbitmap ( "userid_image", "userid.tif" );		

		EclRegisterButton ( useridX + 59, useridY + 55, 147, 15, "", "Enter your userID here", "userid_name" );
		EclRegisterButtonCallbacks ( "userid_name", UserIDButtonDraw, NULL, button_click, button_highlight );

		EclRegisterButton ( useridX + 59, useridY + 81, 147, 15, "", "Enter your access code here", "userid_code" );
		EclRegisterButtonCallbacks ( "userid_code", CodeButtonDraw, NULL, button_click, button_highlight );
		
		EclRegisterButton ( useridX, useridY + 125, 120, 15, "", "", "userid_message" );
		EclRegisterButtonCallbacks ( "userid_message", textbutton_draw, NULL, NULL, NULL );

		EclRegisterButton ( useridX + 140, useridY + 125, 80, 15, "Proceed", "Click here when done", "userid_proceed" );
		button_assignbitmaps ( "userid_proceed", "proceed.tif", "proceed_h.tif", "proceed_c.tif" );
		EclRegisterButtonCallback ( "userid_proceed", ProceedClick );
		
		// Text help box

		EclRegisterButton ( useridX, SY(200), 240, SY(50), "", "", "texthelp" );
		EclRegisterButtonCallbacks ( "texthelp", textbutton_draw, NULL, NULL, NULL );
		EclRegisterCaptionChange ( "texthelp", "If you are registered as an Uplink Agent,\n"
											   "enter your username and password below.\n"
											   "Otherwise, click on the New User button." );

		// List of all existing games

		EclRegisterButton ( SX(35), SY(270), SX(170), SY(20), "Valid User Names", "", "usernames_title" );
		EclRegisterButtonCallbacks ( "usernames_title", LargeTextBoxDraw, NULL, NULL, NULL );

		DArray <char *> *existing = ListExistingGames ();
		
		for ( int i = 0; i < existing->Size (); ++i ) {
			if ( existing->ValidIndex (i) ) {

				char name [32];
				sprintf ( name, "username %d", i );
				EclRegisterButton ( SX(35), SY(300) + i * 18, SX(170), 15, existing->GetData (i), "Log in as this Agent", name );
				EclRegisterButtonCallbacks ( name, textbutton_draw, UserNameClick, button_click, button_highlight );

				delete [] existing->GetData (i);

			}
		}
				
		delete existing;

		EclMakeButtonEditable ( "userid_name" );
		EclMakeButtonEditable ( "userid_code" );
		
	}

}

void LoginInterface::Remove ()
{

	if ( IsVisible () ) {

		MainMenuScreen::Remove ();

		if ( app->GetNetwork ()->STATUS == NETWORK_CLIENT ) {
			EclRemoveButton ( "client_comms" );
			EclRemoveButton ( "client_status" );
		}
		
		EclRemoveButton ( "newgame" );
		EclRemoveButton ( "newgame_text" );
		
		EclRemoveButton ( "options" );
		EclRemoveButton ( "options_text" );

		EclRemoveButton ( "exitgame" );
		EclRemoveButton ( "exitgame_text" );

		EclRemoveButton ( "userid_image" );
		EclRemoveButton ( "userid_name" );
		EclRemoveButton ( "userid_code" );
		EclRemoveButton ( "userid_message" );
		EclRemoveButton ( "userid_proceed" );

		EclRemoveButton ( "texthelp" );

		EclRemoveButton ( "usernames_title" );

		// Remove all username buttons
		int usernameindex = 0;
		char name [32];
		sprintf ( name, "username %d", usernameindex );
		while ( EclGetButton ( name ) ) {
			EclRemoveButton ( name );
			++usernameindex;
			sprintf ( name, "username %d", usernameindex );			
		} 

	}

}

void LoginInterface::Update ()
{
}

bool LoginInterface::IsVisible ()
{

	return ( EclGetButton ( "newgame" ) != NULL );

}

int LoginInterface::ScreenID ()
{

	return MAINMENU_LOGIN;

}
