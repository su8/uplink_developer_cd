
#include <stdio.h>

#include "eclipse.h"
#include "gucci.h"
#include "soundgarden.h"
#include "redshirt.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/opengl.h"
#include "app/serialise.h"
#include "app/opengl_interface.h"

#include "game/game.h"
#include "game/gameobituary.h"
#include "game/data/data.h"

#include "mainmenu/mainmenu.h"

#include "view/view.h"

#include "interface/interface.h"

#include "world/world.h"
#include "world/player.h"
#include "world/message.h"
#include "world/generator/worldgenerator.h"
#include "world/generator/numbergenerator.h"
#include "world/generator/plotgenerator.h"
#include "world/scheduler/notificationevent.h"


Game::Game ()
{

	ui      = NULL;
	view    = NULL;
	world   = NULL;

	gamespeed = GAMESPEED_PAUSED;
	gob = NULL;

}

Game::~Game () 
{

	if ( ui )      delete ui;
	if ( view )    delete view;
	if ( world )   delete world;

	if ( gob )	   delete gob;

}

void Game::NewGame ()
{
 
	if ( ui )      delete ui;
	if ( view )    delete view;
	if ( world )   delete world;
	if ( gob )	   delete gob;

	gob = NULL;

	// Initialise the world
	world = new World ();
	NotificationEvent::ScheduleStartingEvents ();
	WorldGenerator::GenerateAll ();
	world->plotgenerator.Initialise ();
    world->demoplotgenerator.Initialise ();

    SgPlaySound ( RsArchiveFileOpen ( "sounds/ringout.wav" ), "sounds/ringout.wav", false );

	// Initialise the view		
	view = new View ();
	view->Initialise ();

	// Initialise the interface
	ui = new Interface ();
	GetInterface ()->Create ();	
	
	// Start the game running
	gamespeed = GAMESPEED_NORMAL;

	// Run until (at least) the game start date
	// Generate a few extra "easy" missions for the player to get started with

	Date gamestart;
	gamestart.SetDate ( GAME_START_DATE );
    
	do {

		GetWorld ()->Update ();
		GetWorld ()->date.AdvanceDay (1);
		GetWorld ()->date.AdvanceHour ( NumberGenerator::RandomNumber ( 12 ) - 6 );
		GetWorld ()->date.AdvanceMinute ( NumberGenerator::RandomNumber ( 60 ) );

		if ( NumberGenerator::RandomNumber (5) == 0 ) 	WorldGenerator::GenerateSimpleStartingMissionA ();	
		if ( NumberGenerator::RandomNumber (5) == 0 ) 	WorldGenerator::GenerateSimpleStartingMissionB ();

	} while ( GetWorld ()->date.Before ( &gamestart ) );

}

void Game::ExitGame ()
{

	opengl_close ();

}

void Game::SetGameSpeed ( int newspeed )
{

	gamespeed = newspeed;

}

int Game::GameSpeed ()
{

	return gamespeed;

}

bool Game::IsRunning ()
{

	return ( gamespeed > GAMESPEED_PAUSED );

}

void Game::GameOver ( char *reason )
{

	gamespeed = GAMESPEED_GAMEOVER;

	if ( !gob ) gob = new GameObituary ();
	gob->SetGameOverReason ( reason );
	gob->Evaluate ();
	
}

void Game::DemoGameOver ()
{

  	gamespeed = GAMESPEED_GAMEOVER;

	if ( !gob ) gob = new GameObituary ();
	gob->SetGameOverReason (  "You reached the maximum permissable Agent rating for a Demo user.\n"
                              "You can visit www.introversion.co.uk to buy the full product." );
	gob->SetDemoGameOver ( true );
    gob->Evaluate ();

}

void Game::WarezGameOver ()
{

    gamespeed = GAMESPEED_GAMEOVER;

    if ( !gob ) gob = new GameObituary ();
    gob->SetGameOverReason (  "You are playing an illegal copy of Uplink.\n"
                              "You can visit www.introversion.co.uk to buy the full product." );
    gob->SetWarezGameOver ( true );
    gob->Evaluate ();

}

Interface *Game::GetInterface ()
{

	UplinkAssert ( ui );
	return ui;

}

View *Game::GetView ()
{

	UplinkAssert ( view );
	return view;

}

World *Game::GetWorld ()
{

	UplinkAssert ( world );
	return world;

}

GameObituary *Game::GetObituary ()
{

	UplinkAssert (gob);
	return gob;

}

bool Game::LoadGame ( FILE *file )
{

	// Initialise the random number generator

	srand( (unsigned) time( NULL ) );  

	LoadID ( file );

	// Check the file version is correct

	char version [32];
	fread ( version, sizeof (SAVEFILE_VERSION), 1, file );
    if ( strcmp ( version, SAVEFILE_VERSION ) != 0 ) {

        //UplinkAbort ( "This save game file is from an older version of Uplink" );        

        app->GetMainMenu ()->RunScreen ( MAINMENU_LOGIN );

        char message [256];
        sprintf ( message, "Failed to load user profile\n"
                           "The save file is not compatable\n\n"
                           "Save file is Version [%s]\n"
                           "Required Version is  [%s]", version, SAVEFILE_VERSION );
        create_msgbox ( "Error", message );

        return false;
    
    }

    Load ( file );

	LoadID_END ( file );

    return true;

}


void Game::Load ( FILE *file )
{

	// Reset currently running game

	if ( ui )      delete ui;
	if ( view )    delete view;
	if ( world )   delete world;

	ui	    = new Interface ();
	view    = new View ();
	world   = new World ();

	// Is this a "GameOver" game?

	fread ( &gamespeed, sizeof(gamespeed), 1, file );

	if ( gamespeed != GAMESPEED_GAMEOVER ) {

		// Load each of the modules

		GetWorld     ()->Load ( file );
		GetInterface ()->Load ( file );		
		GetView      ()->Load ( file );
		
	}
	else {

		// This is a "game over" game
		gob = new GameObituary ();
		gob->Load ( file );
		
	}

}

void Game::Save ( FILE *file )
{

	if ( gamespeed != GAMESPEED_PAUSED ) {

		SaveID ( file );

		UplinkAssert ( ui );
		UplinkAssert ( view );
		UplinkAssert ( world );

		fwrite ( SAVEFILE_VERSION, sizeof (SAVEFILE_VERSION), 1, file );

		fwrite ( &gamespeed, sizeof(gamespeed), 1, file );

		if ( !(gamespeed == GAMESPEED_GAMEOVER) ) {

			GetWorld     ()->Save ( file );
			GetInterface ()->Save ( file );		
			GetView      ()->Save ( file );

		}
		else {
	
			UplinkAssert ( gob );
			gob->Save ( file );

		}

		SaveID_END ( file );

	}

}

void Game::Print ()
{

	printf ( "============== G A M E =====================================\n" );

	printf ( "Game speed = %d\n", gamespeed );

	if ( ui )      ui     ->Print ();		else printf ( "UI is NULL\n" );
	if ( view )    view   ->Print ();		else printf ( "View is NULL\n" );
	if ( world )   world  ->Print ();		else printf ( "World is NULL\n" );
	
	printf ( "============== E N D  O F  G A M E =========================\n" );

}

char *Game::GetID ()
{

	return "GAME";

}

void Game::Update ()
{

	//
	// Update the modules
	//

	if ( gamespeed > GAMESPEED_PAUSED ) {

		GetWorld     ()->Update ();
		GetView      ()->Update ();
		GetInterface ()->Update ();

	}

	//
	// Autosave every minute
	//

	if ( time(NULL) > lastsave + 1 * 60 ) {

		app->SaveGame ( GetWorld ()->GetPlayer ()->handle );
		lastsave = time(NULL);

	}

}


Game *game;