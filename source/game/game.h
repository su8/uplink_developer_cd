
/* 

  Game object

	Top level glue object of Uplink
	Owns all of the other major components - 
	World, View, Interface and Player

  */


#ifndef _included_game_h
#define _included_game_h

#include <stdio.h>
#include <time.h>

// ============================================================================

#include "app/uplinkobject.h"

class Interface;
class View;
class World;
class GameObituary;

#define GAMESPEED_GAMEOVER		-1
#define GAMESPEED_PAUSED		0
#define GAMESPEED_NORMAL		1
#define GAMESPEED_FAST			2
#define GAMESPEED_MEGAFAST		3
#define GAMESPEED_OHMYGOD		4						// Only in DEBUG mode


// ============================================================================


class Game : public UplinkObject
{

protected:

	Interface *ui;
	View      *view;
	World     *world;

	int gamespeed;

	GameObituary *gob;					   // Only used if this is a dead person
	time_t lastsave;					   // Used in autosaving

public:

	Game ();
	~Game ();

	void NewGame ();
	void ExitGame ();

	void SetGameSpeed ( int newspeed );
	int  GameSpeed ();
	bool IsRunning ();
	
	void GameOver ( char *reason );			//  Ends the current game
    void DemoGameOver ();                   //  This demo game has come to an end
    void WarezGameOver ();                  //  This WAREZ copy has come to an end

											//  These functions will 
	Interface *GetInterface ();				//  stop the program if their
	View      *GetView ();					//  values are NULL, so check
	World     *GetWorld ();					//  Game::IsRunning before using

	GameObituary *GetObituary ();			//  Asserts it exists

    bool LoadGame ( FILE *file );           //  Use this rather than Load

	// Common functions

	void Load   ( FILE *file );
	void Save   ( FILE *file );
	void Print  ();
	void Update ();
	char *GetID ();

};


extern Game *game;


#endif