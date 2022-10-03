// -*- tab-width:4 c-file-style:"cc-mode" -*-

/*
	App object

	Stores data about the app - eg path, version, etc

  */

#ifndef _included_app_h
#define _included_app_h

// ============================================================================

#include "app/uplinkobject.h"


class Options;
class Network;
class MainMenu;


#define SIZE_APP_PATH    256
#define SIZE_APP_VERSION 32
#define SIZE_APP_TYPE    32
#define SIZE_APP_DATE    32
#define SIZE_APP_TITLE   64
#define SIZE_APP_RELEASE 256

// ============================================================================


class App : public UplinkObject
{

public :

    char path    [SIZE_APP_PATH];
    char userpath [SIZE_APP_PATH];
    char version [SIZE_APP_VERSION];
    char type    [SIZE_APP_TYPE];
    char date    [SIZE_APP_DATE];
    char title   [SIZE_APP_TITLE];
    char release [SIZE_APP_RELEASE];          

    int starttime;
    bool closed;

    Options *options;
	Network *network;
	MainMenu *mainmenu;

public:

    App ();
    ~App ();
    
    void Set ( char *newpath, char *newversion, char *newtype,
			   char *newdate, char *newtitle );
    
	void Initialise ();	
	void Close ();			// Shuts down the app	
    bool Closed ();         // True if in the process of shutting down

	void LoadGame ( char *username );
	void SaveGame ( char *username );

	Options *GetOptions ();
	Network *GetNetwork ();
	MainMenu *GetMainMenu ();

    void CoreDump ();             

	// Common functions

	void Load   ( FILE *file );
	void Save   ( FILE *file );
	void Print  ();
	void Update ();
	char *GetID ();

};

extern App *app;

#endif


