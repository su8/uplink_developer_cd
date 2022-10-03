// -*- tab-width:4 c-file-style:"cc-mode" -*-
/*

  =============================================================================
  =																		      =
  =							 U	 P	 L	 I	 N	 K							  =
  =																			  =
  =							 Started 22nd June '99							  =
  =																			  =
  =							  By Christopher Delay							  =
  =																			  =
  =																			  =
  =============================================================================

  */


#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#ifdef WIN32
#include <windows.h>
#include <conio.h>
#include <io.h>
#else
#include <unistd.h>
#endif

#include <GL/gl.h>

#include <GL/glu.h>


#include "eclipse.h"
#include "vanbakel.h"
#include "soundgarden.h"
#include "gucci.h"
#include "redshirt.h"
#include "bungle.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/opengl.h"
#include "app/opengl_interface.h"
#include "app/miscutils.h"
#include "app/serialise.h"

#include "options/options.h"

#include "mainmenu/mainmenu.h"

#include "game/scriptlibrary.h"
#include "game/game.h"

// ============================================================================
// Initialisation functions

void RunUplink ( int argc, char **argv );

void Init_App      ( char *apppath );
void Init_Options  ( int argc, char **argv );

void Load_Data	   ();
void Init_Game     ();
void Init_Sound    ();
void Init_Music    ();
void Init_Graphics ();
void Init_OpenGL   ( int argc, char **argv );
void Init_Fonts	   ();

void Run_MainMenu  ();
void Run_Game      ();

// ============================================================================
// Program entry points


void main ( int argc, char **argv )
{

	// Standard C++ entry point
	
	RunUplink ( argc, argv );

}


#ifdef _WINDOWS

/*
    Remember to set subsystem:windows
    instead of subsystem:console
    in the link options
    */

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	
    // Work out the exe app path
    // THANK YOU VERY MUCH WIN32
    // WHATEVER WAS WRONG WITH ARGV[0] YOU FUCKING IDIOTS

	char *apppath = new char [256];
    strcpy ( apppath, GetCommandLine () );
	// Strip inverted commas from the app path
	// note that it's not guaranteed the path is surrounded
	// by inverted commas (eg. if path does not have a space in it,
	// or you are running under Wine)
	if (apppath[0] == '\"') {
		apppath = (apppath+1);
		char *lastdit = strchr ( apppath, '\"' ); 
		UplinkAssert (lastdit);
		*lastdit = '\x0';
	}

    // Break up lpCmdLine into argc and argv

    char **argv = NULL;
    int argc = 1;

    if ( *lpCmdLine != '\x0' ) {

	    char *cmdLineCopy = new char [strlen(lpCmdLine)+1];
	    strcpy ( cmdLineCopy, lpCmdLine );

	    char *c = cmdLineCopy;
	    while ( c ) {
		    ++argc;
		    c = strchr ( (c+1), ' ' );
	    }

        argv = new char *[argc];
        argv[0] = apppath;
        
	    if ( argc > 1 ) {
		    
		    argv [1] = cmdLineCopy;
		    char *c = strchr ( cmdLineCopy, ' ');
		    int n = 2;
		    while ( c ) {
			    *c = '\x0';
			    argv [n] = (c+1);
			    ++n;
			    c = strchr ( (c+1), ' ' );
		    }
	    }

    }
    else {

        argv = new char *[1];
        argv[0] = apppath;

    }

    // Run the game

/*
	printf ( "App path : %s\n", apppath );
	printf ( "Command line : %s\n", lpCmdLine );
	printf ( "Num arguments : %d\n", argc );
	for ( int i = 0; i < argc; ++i )
		printf ( "Arg %d : '%s'\n", i, argv [i] );
*/

	RunUplink ( argc, argv );

	return 0;

}

#endif

// ============================================================================


void RunUplink ( int argc, char **argv ) 
{

	try {

		// Initialise each of the modules

		Init_App      ( argv [0] );
		Init_Options  ( argc, argv );

		Load_Data	  ();

		Init_Game     ();
		Init_Graphics ();
		Init_OpenGL   ( argc, argv );
		Init_Fonts	  ();
		Init_Sound    ();
		Init_Music    ();

		// Run everything

		Run_MainMenu  ();
		Run_Game      ();

	} 
	catch ( ... ) {

		// Catch any errors that occured
		// Set the screen size back to normal,
		// then continue with the error - permitting debugging

		printf ( "\n" );
		printf ( "An (unrecognised) Uplink Internal Error has occured\n" );
		printf ( "===================================================\n" );

        app->CoreDump ();

        GciRestoreScreenSize ();

#ifdef TESTGAME
        throw;
#else
        exit(255);
#endif

	}

}

// ============================================================================


void Init_App ( char *argv0 )
{

	// Set up App object

    char *apppath = GetFilePath ( argv0 );
	app = new App ();
	char currenttime [SIZE_APP_DATE];
	sprintf ( currenttime, "%s at %s", __DATE__, __TIME__ );
	app->Set ( apppath, VERSION_NUMBER, VERSION_NAME, currenttime, "Uplink" );
    delete [] apppath;

#ifdef CODECARD_ENABLED
    #ifdef CODECARD_CHRIS
        strcat ( app->release, "(code-C)" );
    #endif
    #ifdef CODECARD_MARK
        strcat ( app->release, "(code-M)" );
    #endif
    #ifdef CODECARD_TOM
        strcat ( app->release, "(code-T)" );
    #endif
#endif

	printf ( "=============================\n" );
	printf ( "=                           =\n" );
	printf ( "=        U P L I N K        =\n" );
	printf ( "=                           =\n" );		
	printf ( "=        Version %-10s =\n", app->version );

#ifdef FULLGAME
    printf ( "=     - R E L E A S E -     =\n" );
#endif

#ifdef DEMOGAME	
	printf ( "=        - D E M O -        =\n" );
#endif
    
#ifdef TESTGAME
    printf ( "=    - I N T E R N A L -    =\n" );
#endif

	printf ( "=                           =\n" );
	printf ( "=============================\n" );
	printf ( "\n" );
	printf ( "%s\n", app->release );

#ifdef DEBUGLOG_ENABLED

	MakeDirectory ( app->userpath );

    char debugpath [256];
    sprintf ( debugpath, "%sdebug.log", app->userpath );

    FILE *stdoutfile = freopen ( debugpath, "a", stdout );
    if ( !stdoutfile ) printf ( "WARNING : Failed to open users/debug.log for writing stdout\n" );

    FILE *stderrfile = freopen ( debugpath, "a", stderr );
    if ( !stderrfile ) printf ( "WARNING : Failed to open users/debug.log for writing stderr\n" );

    // Print some basic info to the file

    time_t timet = time(NULL);
    tm *thetime = localtime(&timet);

    printf ( "\n\n" );
    printf ( "===============================================\n" );
    printf ( "NEW GAME     %d:%d, %d/%d/%d\n", thetime->tm_hour, thetime->tm_min, thetime->tm_mday, thetime->tm_mon+1, thetime->tm_year+1900 );
    printf ( "===============================================\n" );
        
    printf ( "Version : %s\n", app->version );

#ifdef FULLGAME
    printf ( "RELEASE\n" );
#endif
#ifdef DEMOGAME	
	printf ( "DEMO\n" );
#endif
#ifdef TESTGAME
    printf ( "INTERNAL\n" );
#endif
#ifdef WIN32
    printf ( "Win32 Build\n" );
#else
    printf ( "Linux Build\n" );
#endif
    printf ( "%s\n", app->release );

#endif

    RsInitialise ( app->path );

	app->Initialise ();

}

void Init_Options ( int argc, char **argv )
{

	// Parse all command line options

	for ( int i = 1; i < argc; ++i ) {
		
		char type;
		char optionname [SIZE_OPTION_NAME];

		sscanf ( argv [i], "%c%s", &type, optionname );

		if ( type == '+' ) {													// Turn option ON  (1)
			app->GetOptions ()->SetOptionValue ( optionname, true );
#ifdef _DEBUG
			printf ( "Parsed command line option : %s\n", argv [i] );
#endif
		}
		else if ( type == '-' ) {												// Turn option OFF (0)
			app->GetOptions ()->SetOptionValue ( optionname, false );
#ifdef _DEBUG
			printf ( "Parsed command line option : %s\n", argv [i] );
#endif
		}
		else if ( type == '!' ) {												// Set option value
			++i;
			int newvalue;
			sscanf ( argv [i], "%d", &newvalue );
			app->GetOptions ()->SetOptionValue ( optionname, newvalue );
#ifdef _DEBUG
			printf ( "Parsed command line option : %s %d\n", optionname, newvalue ); 
#endif
		}
		else {
			char msg [256];
			sprintf ( msg, "Error parsing command line option : %d", argv [i] );
			UplinkAbort ( msg );
		}

	}


    //
    // Safe mode?
    //

    if ( app->GetOptions ()->GetOptionValue ( "graphics_safemode" ) == 1 ) {
        app->GetOptions ()->SetOptionValue ( "graphics_fullscreen", false );
        app->GetOptions ()->SetOptionValue ( "graphics_screenrefresh", -1 );
        app->GetOptions ()->SetOptionValue ( "graphics_screendepth", -1 );
        app->GetOptions ()->SetOptionValue ( "graphics_softwaremouse", true );
    }

	printf ( "\n" );
	
    SetWindowScaleFactor ( app->GetOptions ()->GetOptionValue("graphics_screenwidth") / 640.0,
                           app->GetOptions ()->GetOptionValue("graphics_screenheight") / 480.0 );

	if ( app->GetOptions ()->IsOptionEqualTo ( "game_debugstart", 1 ) ) 	
		printf ( "=====DEBUGGING INFORMATION ENABLED=====\n" );

}

void Load_Data ()
{

	bool debugging = false;
	if ( app->GetOptions ()->IsOptionEqualTo ( "game_debugstart", 1 ) )
		debugging = true;

	if ( debugging ) printf ( "Loading application data\n" );

	RsLoadArchive ( "data.dat" );
	RsLoadArchive ( "graphics.dat" );
    RsLoadArchive ( "loading.dat" );
	RsLoadArchive ( "sounds.dat" );
	RsLoadArchive ( "music.dat" );
	RsLoadArchive ( "fonts.dat" );
    RsLoadArchive ( "patch.dat" );

#ifdef FULLGAME

    // Generate the file CRC

    char realbuffer [9];
    strcpy ( realbuffer, "\xe7\x6b\x7e\x6b\x4c\x4f\x57\x7d" );
    
    // Ensure the world.dat file is in place and has the right CRC

    char worlddatfilename [256];
    sprintf ( worlddatfilename, "%s/world.dat", app->path );
    FILE *file = fopen ( worlddatfilename, "rb" );
    UplinkAssert (file);

    fseek ( file, 128, SEEK_SET	);
    char readbuffer [9];
    fread ( readbuffer, 8, 1, file );
    readbuffer [8] = '\x0';
    int match = strcmp ( readbuffer, realbuffer );
    UplinkAssert ( match == 0 );

    fclose ( file );

    // Ensure the filesize is correct

#ifdef WIN32
    int filehandle = _open ( worlddatfilename, _O_RDONLY );
    UplinkAssert ( filehandle != -1 );
    struct _stat filestats;
    _fstat ( filehandle, &filestats );
    bool match2 = ( filestats.st_size == 14400792 );
    UplinkAssert ( match2 );
	_close ( filehandle );
#else
    int filehandle = open ( worlddatfilename, O_RDONLY );
    UplinkAssert ( filehandle != -1 );
    struct stat filestats;
    fstat ( filehandle, &filestats );
    bool match2 = ( filestats.st_size == 14400792 );
    UplinkAssert ( match2 );
    close ( filehandle );
#endif

#endif // FULLGAME

	if ( debugging ) printf ( "Finished loading application data\n" );

}

void Init_Game () 
{

	bool debugging = false;
	if ( app->GetOptions ()->IsOptionEqualTo ( "game_debugstart", 1 ) )
		debugging = true;

	if ( debugging ) printf ( "Init_Game called...creating game object\n" );

	// Initialise the random number generator
	srand( time( NULL ) );  		

	// Set up Game object
	game = new Game ();	
	
	if ( debugging ) printf ( "Finished with Init_Game\n" );

}

void Init_Sound ()
{

	bool debugging = false;
	if ( app->GetOptions ()->IsOptionEqualTo ( "game_debugstart", 1 ) )
		debugging = true;

	if ( debugging ) printf ( "Init_Sound called...setting up sound system\n" );

	SgInitialise ();

	if ( debugging ) printf ( "Finished with Init_Sound\n" );

}

void Init_Music ()
{

	bool debugging = false;
	if ( app->GetOptions ()->IsOptionEqualTo ( "game_debugstart", 1 ) )
		debugging = true;

    if ( debugging ) printf ( "Init_Music called...loading modules\n" );

    SgPlaylist_Initialise ();

	SgSetModVolume ( 20 );

    SgPlaylist_Create ( "main" );

#ifdef DEMOGAME
    SgPlaylist_AddSong ("main", "music/bluevalley.uni" );
    SgPlaylist_AddSong ("main", "music/serenity.uni" );
    SgPlaylist_AddSong ("main", "music/mystique.uni" );
#else
    SgPlaylist_AddSong ("main", "music/bluevalley.uni" );
    SgPlaylist_AddSong ("main", "music/serenity.uni" );
    SgPlaylist_AddSong ("main", "music/mystique.uni" );
    SgPlaylist_AddSong ("main", "music/a94final.uni" );
    SgPlaylist_AddSong ("main", "music/symphonic.uni" );
    SgPlaylist_AddSong ("main", "music/myst2.uni" );
#endif

/*
    SgPlaylist_Create ("action");
    SgPlaylist_Create ("ambient");
    SgPlaylist_Create ("sad");

#ifdef DEMOGAME
    SgPlaylist_AddSong ("action", "music/bluevalley.uni" );
    SgPlaylist_AddSong ("ambient", "music/bluevalley.uni" );
    SgPlaylist_AddSong ("ambient", "music/serenity.uni" );
    SgPlaylist_AddSong ("ambient", "music/mystique.uni" );
    SgPlaylist_AddSong ("sad", "music/serenity.uni" );
#else
    SgPlaylist_AddSong ("action", "music/bluevalley.uni" );
    SgPlaylist_AddSong ("ambient", "music/bluevalley.uni" );
    SgPlaylist_AddSong ("ambient", "music/serenity.uni" );
    SgPlaylist_AddSong ("ambient", "music/mystique.uni" );
    SgPlaylist_AddSong ("ambient", "music/a94final.uni" );
    SgPlaylist_AddSong ("sad", "music/symphonic.uni" );
#endif
*/

    if ( debugging ) printf ( "Finished with Init_Music\n" );

}


void Init_Graphics ()
{
	Options *o = app->GetOptions ();
	o->SetThemeName ( o->GetThemeName() );
}

void Init_Fonts	()
{

	bool debugging = false;
	if ( app->GetOptions ()->IsOptionEqualTo ( "game_debugstart", 1 ) )
		debugging = true;

	if ( debugging ) printf ( "Init_Fonts called...setting up system fonts\n" );

	// Set up Gucci graphics library	

	GciEnableTrueTypeSupport ();

	if ( debugging ) printf ( "Registering fonts..." );
        
    char *dungeonFontFilename = RsArchiveFileOpen ( "fonts/dungeon.ttf" );
    bool dungeonFontAdded = GciRegisterTrueTypeFont( dungeonFontFilename );
        
	if ( debugging ) printf ( "done\n ");
	if ( debugging ) printf ( "Loading system fonts into memory...\n" );

	bool success =
	  (GciLoadTrueTypeFont ( HELVETICA_10, "Dungeon", dungeonFontFilename, 9  ) &
	   GciLoadTrueTypeFont ( HELVETICA_12, "Dungeon", dungeonFontFilename, 11 ) &
	   GciLoadTrueTypeFont ( HELVETICA_18, "Dungeon", dungeonFontFilename, 16 ));

	if ( debugging ) printf ( "done\n" );

	if ( debugging ) printf ( "Unregistering fonts..." );
	if ( dungeonFontAdded ) GciUnregisterTrueTypeFont( dungeonFontFilename );
	if ( debugging ) printf ( "done\n ");

	if ( !success ) {
		GciDisableTrueTypeSupport ();
		printf ( "True type font support is DISABLED\n" );
	}
	
	GciSetDefaultFont ( HELVETICA_10 );

	if ( debugging ) printf ( "Finished with Init_Fonts\n ");

}


void Init_OpenGL ( int argc, char **argv )
{

	// Set up OpenGL

	opengl_initialise ( argc, argv );						

}


void Run_MainMenu ()
{

	if ( app->GetOptions ()->IsOptionEqualTo ( "game_debugstart", 1 ) ) {
		printf ( "Creating main menu.\n" );
		printf ( "====== END OF DEBUGGING INFORMATION ====\n" );
	}
	

	// Set up the game (mainmenu if not first time, or straight into the game );
    
    float currentVersion;
    sscanf ( VERSION_NUMBER, "%f", &currentVersion );
    currentVersion *= 100;

    int oldVersion = app->GetOptions ()->GetOptionValue( "game_version" );

    if ( (int) currentVersion != oldVersion ) {

        // A new patch has been installed
        printf ( "New patch Detected!\n" );
        printf ( "Old version = %d\n", oldVersion );
        printf ( "New version = %d\n\n", (int) currentVersion );

        app->GetOptions()->SetOptionValue ( "game_version", (int) currentVersion, "z", false, false );

        if ( oldVersion < 120 )
        {
            // Ugrading the game from a version before Nakatomi
            app->GetMainMenu ()->RunScreen ( MAINMENU_FIRSTLOAD );
            ScriptLibrary::RunScript( 45 );
        }
        else
        {
    		app->GetMainMenu ()->RunScreen ( MAINMENU_LOGIN );
        }

    }
    else if ( app->GetOptions ()->IsOptionEqualTo ( "game_firsttime", 1 ) ) {

        // This is a brand new installation

		app->GetMainMenu ()->RunScreen ( MAINMENU_FIRSTLOAD );
		GciTimerFunc ( 2000, ScriptLibrary::RunScript, 30 );

	}
	else {
	
        // Normal startup

		app->GetMainMenu ()->RunScreen ( MAINMENU_LOGIN );
	
	}

}

void Run_Game ()
{

	// Run the game!

	opengl_run ();

}

