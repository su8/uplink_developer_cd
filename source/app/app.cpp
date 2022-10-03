// -*- tab-width:4 c-file-style:"cc-mode" -*-
// App source file
// By Christopher Delay

#ifdef WIN32
#include <windows.h>
#endif


#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "gucci.h"
#include "eclipse.h"
#include "redshirt.h"
#include "soundgarden.h"
#include "vanbakel.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/miscutils.h"

#include "options/options.h"

#include "game/game.h"
#include "game/data/data.h"
#include "game/gameobituary.h"

#include "mainmenu/mainmenu.h"
#include "network/network.h"

#include "world/world.h"
#include "world/player.h"
#include "world/generator/worldgenerator.h"

#include "interface/interface.h"
#include "interface/localinterface/localinterface.h"
#include "interface/remoteinterface/remoteinterface.h"


App :: App ()
{

    strcpy ( path, "c:/" );
    strcpy ( userpath, path );
    strcpy ( version, "1.0" );
    strcpy ( type, "RELEASE" );
    strcpy ( date, "01/01/97" );
    strcpy ( title, "NewApp" );
    strcpy ( release, "Version 1.0 (RELEASE), Compiled on 01/01/97" );

    starttime = 0;

	options = NULL;
	network = NULL;
	mainmenu = NULL;

    closed = false;

#ifdef _DEBUG
//    SlInitialise ();
#endif

}

App :: ~App ()
{
	
	// All destructor information should go in App::Close ()

}

void App :: Initialise ()
{

	options = new Options ();	
	options->Load ( NULL );
	options->CreateDefaultOptions ();

    starttime = EclGetAccurateTime ();

	network = new Network ();
	mainmenu = new MainMenu ();

}

void App :: Set ( char *newpath, char *newversion, char *newtype, char *newdate, char *newtitle )
{
                
	UplinkAssert ( strlen ( newpath ) < SIZE_APP_PATH );
	UplinkAssert ( strlen ( newversion ) < SIZE_APP_VERSION );
	UplinkAssert ( strlen ( newtype ) < SIZE_APP_TYPE );
	UplinkAssert ( strlen ( newdate ) < SIZE_APP_DATE );
	UplinkAssert ( strlen ( newtitle ) < SIZE_APP_TITLE );

    strcpy ( path, newpath );
    strcpy ( version, newversion );
    strcpy ( type, newtype );
    strcpy ( date, newdate );
    strcpy ( title, newtitle );
    sprintf ( release, "Version %s (%s)\nCompiled on %s\n", version, type, date );    

#ifdef WIN32
	// Under Windows, the user-path is %app-path%/users
	sprintf( userpath, "%susers/", path );
#else
	// Under Linux, the user-path is ~/.uplink 
	// (or %app-path%/users if no HOME environment variable)
	char *homedir = getenv("HOME");
	if (homedir != NULL) 
		sprintf( userpath, "%s/.uplink/", homedir);
	else
		sprintf( userpath, "%susers/", path );
#endif // WIN32
}

Options *App::GetOptions ()
{

	UplinkAssert (options);
	return options;

}

Network *App::GetNetwork ()
{

	UplinkAssert (network);
	return network;

}

MainMenu *App::GetMainMenu ()
{

	UplinkAssert (mainmenu);
	return mainmenu;

}

void App::LoadGame ( char *username )
{

	// Try to load from the local dir

	char filename [128];
	sprintf ( filename, "%s%s.usr", app->userpath, username );

	printf ( "Loading profile from %s...", filename );
	
	FILE *file = RsFileOpen ( filename );			

	if ( file ) {

		GetMainMenu ()->Remove ();

		bool success = game->LoadGame ( file );
		RsFileClose ( filename, file );

        if ( !success ) {
            printf ( "failed\n" );
            printf ( "App::LoadGame, Failed to load user profile\n" );
            return;
        }

		printf ( "success\n" );

	}
	else {

		printf ( "failed\n" );
		printf ( "App::LoadGame, Failed to load user profile\n" );

		GetMainMenu ()->RunScreen ( MAINMENU_LOGIN );

		return;

	}
	
	if ( !(game->GameSpeed () == GAMESPEED_GAMEOVER) ) {

        WorldGenerator::LoadDynamics();

		game->GetWorld ()->GetPlayer ()->GetConnection ()->Disconnect ();
		game->GetWorld ()->GetPlayer ()->GetConnection ()->Reset ();

		if ( game->GetWorld ()->GetPlayer ()->gateway.newtype != -1 ) {

			// The player has upgraded his gateway and now wishes to 
			// log into the new system

			game->GetWorld ()->GetPlayer ()->gateway.ExchangeGatewayComplete ();
			
			// "Welcome to your new gateway"

			game->GetInterface ()->GetRemoteInterface ()->RunNewLocation ();
			game->GetInterface ()->GetRemoteInterface ()->RunScreen ( 10 );
			

		}
		else if ( game->GetWorld ()->GetPlayer ()->gateway.nuked ) {

			// The player has nuked his gateway, so set him up
			// with a new one           
            game->GetWorld ()->GetPlayer ()->gateway.nuked = false;

			game->GetWorld ()->GetPlayer ()->GetConnection ()->AddVLocation ( IP_UPLINKPUBLICACCESSSERVER );
			game->GetWorld ()->GetPlayer ()->GetConnection ()->Connect ();

            game->GetInterface ()->GetLocalInterface ()->Remove ();
            EclReset ( app->GetOptions ()->GetOptionValue ("graphics_screenwidth"),
				       app->GetOptions ()->GetOptionValue ("graphics_screenheight") );
			game->GetInterface ()->GetRemoteInterface ()->RunNewLocation ();
			game->GetInterface ()->GetRemoteInterface ()->RunScreen ( 10 );

		}
		else {
			
			// "Welcome back to your gateway"

			game->GetInterface ()->GetRemoteInterface ()->RunNewLocation ();
			game->GetInterface ()->GetRemoteInterface ()->RunScreen ( 3 );

		}

	}
	else {											// This is a Game Over game

		game->SetGameSpeed ( GAMESPEED_PAUSED );
        EclReset ( app->GetOptions ()->GetOptionValue ("graphics_screenwidth"),
				   app->GetOptions ()->GetOptionValue ("graphics_screenheight") );
		mainmenu->RunScreen ( MAINMENU_OBITUARY );

	}

}

void App::SaveGame ( char *username )
{

	if ( strcmp ( username, "NEWAGENT" ) == 0 ) return;

	// Try to save to the local dir

	MakeDirectory ( userpath );

	char filename [128];
	sprintf ( filename, "%s%s.usr", userpath, username );

	printf ( "Saving profile to %s...", filename );

	FILE *file = fopen ( filename, "wb" );

	if ( file ) {

	
		UplinkAssert ( file );

		game->Save ( file );
		fclose ( file );

#ifndef TESTGAME
		RsEncryptFile ( filename );
#endif

		printf ( "success\n" );

	}
	else {

		printf ( "failed\n" );
		printf ( "App::SaveGame, Failed to save user profile\n" );
	
	}

}

void App::Close ()
{

    closed = true;

    EclReset ( app->GetOptions ()->GetOptionValue ("graphics_screenwidth"),
			   app->GetOptions ()->GetOptionValue ("graphics_screenheight") );

	if ( game ) game->ExitGame ();

    options->ApplyShutdownChanges ();
    options->Save ( NULL );
    
	SgShutdown ();
    SvbReset ();
    GciDeleteAllTrueTypeFonts ();
    RsCleanUp ();

	delete mainmenu;
	delete options;	
	delete network;
	delete game;

#ifdef _DEBUG
    char filename [256];
    sprintf ( filename, "%smemtemp", app->userpath );
//    SlPrintMemoryLeaks ( filename );
#endif

	exit (1);

}

bool App::Closed ()
{

    return closed;

}

void App::Load ( FILE *file )
{
}

void App::Save ( FILE *file )
{
}

void App::CoreDump ()
{

#ifdef WIN32
    MessageBox ( NULL, "A Fatal Error occured in Uplink.\n\n"
                       "Please report this on the Uplink forums at\n"
					   "http://www.introversion.co.uk/\n\n"
                       "Uplink will now shut down.", 
                       "Uplink Error", MB_ICONEXCLAMATION | MB_OK );    

#endif
    
  	printf ( "============== B E G I N  C O R E  D U M P =================\n" );
    PrintStackTrace ();
   	printf ( "============== E N D  C O R E  D U M P =====================\n" );

}

void App::Print ()
{
    
  	printf ( "============== A P P =======================================\n" );

	if ( game )     game->Print ();     else printf ( "game == NULL\n" );
    if ( mainmenu ) mainmenu->Print (); else printf ( "mainmenu == NULL\n" );
	if ( options )  options->Print ();  else printf ( "options == NULL\n" );
	if ( network )  network->Print ();  else printf ( "network == NULL\n" );

    printf ( "============== E N D  O F  A P P ===========================\n" );

}

void App::Update ()
{

	if ( game->GameSpeed () == GAMESPEED_GAMEOVER || 
		( game->IsRunning () &&
		  game->GetWorld ()->GetPlayer ()->gateway.nuked ) ) {

		// The game has ended and now needs shutting down
		
		app->SaveGame ( game->GetWorld ()->GetPlayer ()->handle );				
		game->SetGameSpeed ( GAMESPEED_PAUSED );

        EclReset ( app->GetOptions ()->GetOptionValue ("graphics_screenwidth"),
				   app->GetOptions ()->GetOptionValue ("graphics_screenheight") );

		if ( game->GetWorld ()->GetPlayer ()->gateway.nuked )
			mainmenu->RunScreen ( MAINMENU_CONNECTIONLOST );

        else if ( game->GetObituary ()->demogameover ) 
            mainmenu->RunScreen ( MAINMENU_DEMOGAMEOVER );

        else if ( game->GetObituary ()->warezgameover )
            mainmenu->RunScreen ( MAINMENU_WAREZGAMEOVER );

        else if ( game->GetWorld ()->plotgenerator.revelation_releaseuncontrolled )
            mainmenu->RunScreen ( MAINMENU_REVELATIONWON );

        else if ( game->GetWorld ()->plotgenerator.revelation_releasefailed )
            mainmenu->RunScreen ( MAINMENU_REVELATIONLOST );

		else
			mainmenu->RunScreen ( MAINMENU_DISAVOWED );

	}

	if ( game->IsRunning () ) {	
		game->Update ();
	}

	if ( mainmenu->IsVisible () ) {
		mainmenu->Update ();
	}

	if ( network->IsActive () ) {
		network->Update ();
	}

}

char *App::GetID ()
{

	return "APP";

}


App *app;



