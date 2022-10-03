// -*- tab-width:4; c-file-mode:"cc-mode" -*-
// Options.cpp: implementation of the Options class.
//
//////////////////////////////////////////////////////////////////////

#ifdef WIN32
#include <strstrea.h>
#else
#include <strstream.h>
#endif

#include <fstream.h>
#include "app/dos2unix.h"

#include "gucci.h"
#include "redshirt.h"

#include "app/globals.h"
#include "app/app.h"
#include "app/miscutils.h"
#include "app/serialise.h"

#include "options/options.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static ColourOption getColourDefault ( 0.0, 0.0, 0.0 );


Options::Options()
{
    strcpy ( themeName, "graphics" );
}

Options::~Options()
{

	DeleteBTreeData ( (BTree <UplinkObject *> *) &options );


	DArray <ColourOption *> *cols = colours.ConvertToDArray ();

	for ( int i = 0; i < cols->Size (); ++i )
		if ( cols->ValidIndex (i) )
			if ( cols->GetData (i) )				
				delete cols->GetData (i);

	delete cols;	

}

Option *Options::GetOption ( char *name )
{
	
	BTree <Option *> *btree = options.LookupTree ( name );

	if ( btree )
		return btree->data;

	else
		return NULL;

}

int Options::GetOptionValue ( char *name )
{

	Option *option = GetOption ( name );

	if ( !option ) {
		char msg [256];
		sprintf ( msg, "Option %s not found", name );
		UplinkAbort(msg);
	}
	
	return option->value;

}

bool Options::IsOptionEqualTo ( char *name, int value )
{

	Option *option = GetOption ( name );

	if ( option && option->value == value )
		return true;

	else
		return false;

}

void Options::SetOptionValue ( char *name, int newvalue )
{

	BTree <Option *> *btree = options.LookupTree ( name );

	if ( btree ) {

		UplinkAssert ( btree->data );
		Option *option = btree->data;
		option->SetValue ( newvalue );

	}
	else {

		printf ( "Tried to set unrecognised option: %s\n", name );

	}

}

void Options::SetOptionValue ( char *name, int newvalue, char *tooltip, bool yesorno, bool visible )
{

	BTree <Option *> *btree = options.LookupTree ( name );

	if ( btree ) {

		UplinkAssert ( btree->data );
		Option *option = btree->data;
		option->SetValue ( newvalue );
		option->SetTooltip ( tooltip );
		option->SetYesOrNo ( yesorno );
		option->SetVisible ( visible );
		
	}
	else {

		// Create a new entry
		Option *option = new Option ();
		option->SetName ( name );
		option->SetValue ( newvalue );
		option->SetTooltip ( tooltip );
		option->SetYesOrNo ( yesorno );
		option->SetVisible ( visible );

		options.PutData ( name, option );

	}
		

}

LList <Option *> *Options::GetAllOptions ( char *searchstring, bool returnhidden )
{

	DArray <Option *> *alloptions = options.ConvertToDArray ();
	LList <Option *> *result = new LList <Option *> ();

	for ( int i = 0; i < alloptions->Size (); ++i ) {
		if ( alloptions->ValidIndex (i) ) {

			Option *option = alloptions->GetData (i);
			UplinkAssert (option);

			if ( option->visible || returnhidden )
				if ( !searchstring || strstr ( option->name, searchstring ) != NULL )
					result->PutData ( option );

		}
	}

	delete alloptions;
	return result;

}

void Options::CreateDefaultOptions ()
{

	// Game

	if ( !GetOption ( "game_debugstart" ) )				SetOptionValue ( "game_debugstart", 0, "z", true, false );

#ifndef TESTGAME
	if ( !GetOption ( "game_firsttime" ) )				SetOptionValue ( "game_firsttime", 1, "z", true, false );
#else
	if ( !GetOption ( "game_firsttime" ) )				SetOptionValue ( "game_firsttime", 0, "z", true, false );
#endif
	
    if ( !GetOption ( "game_version" ) )                SetOptionValue ( "game_version", 100, "z", false, false );

	// Graphics

	if ( !GetOption ( "graphics_screenwidth" ) )		SetOptionValue ( "graphics_screenwidth", 640, "Sets the width of the screen", false, false );
	if ( !GetOption ( "graphics_screenheight" ) )		SetOptionValue ( "graphics_screenheight", 480, "Sets the height of the screen", false, false );
    if ( !GetOption ( "graphics_screendepth" ) )        SetOptionValue ( "graphics_screendepth", -1, "Sets the colour depth. -1 Means use desktop colour depth.", false, false );
    if ( !GetOption ( "graphics_screenrefresh" ) )      SetOptionValue ( "graphics_screenrefresh", -1, "Sets the refresh rate. -1 Means use desktop refresh.", false, false );
	if ( !GetOption ( "graphics_fullscreen" ) )			SetOptionValue ( "graphics_fullscreen", 1, "Sets the game to run fullscreen or in a window", true, true );
	if ( !GetOption ( "graphics_buttonanimations" ) )	SetOptionValue ( "graphics_buttonanimations", 1, "Enables or disables button animations", true, true );
	if ( !GetOption ( "graphics_safemode" ) )			SetOptionValue ( "graphics_safemode", 0, "Enables graphical safemode for troubleshooting", true, true );
	if ( !GetOption ( "graphics_softwaremouse" ) )		SetOptionValue ( "graphics_softwaremouse", 0, "Render a software mouse.  Use to correct mouse problems.", true, true );

	// Sound

	if ( !GetOption ( "sound_musicenabled" ) )			SetOptionValue ( "sound_musicenabled", 1, "Enables or disables music", true, true );

	// Network

    // Theme

    // SetThemeName ( "graphics" );


    // Repair old options files

    app->GetOptions ()->GetOption ( "graphics_screenwidth" )->SetVisible ( false );
    app->GetOptions ()->GetOption ( "graphics_screenheight" )->SetVisible ( false );
    app->GetOptions ()->GetOption ( "graphics_screendepth" )->SetVisible ( false );
    app->GetOptions ()->GetOption ( "graphics_screenrefresh" )->SetVisible ( false );

}

void Options::SetThemeName ( char *newThemeName )
{
    strcpy ( themeName, newThemeName );

    //
    // Parse the theme.txt file

    char *themeTextFilename = ThemeFilename ( "theme.txt" );
    char *rsFilename = RsArchiveFileOpen ( themeTextFilename );
    
    if ( rsFilename ) {
        idos2unixstream thefile ( rsFilename );
	    
        // Header

        char unused [64];
	    thefile >> unused >> ws;
        thefile.getline ( themeTitle, 128 );

        thefile >> unused >> ws;
        thefile.getline ( themeAuthor, 128 );
        
        thefile >> unused >> ws;
        thefile.getline ( themeDescription, 1024 );

        // Colours

        while ( !thefile.eof() ) {
    
            char lineBuffer [256];
            thefile.getline ( lineBuffer, 256 );

            if ( strlen(lineBuffer) < 2 )     continue;
		    if ( lineBuffer[0] == ';' )       continue;

            char colourName[64];
            float r, g, b;
    		istrstream thisLine ( lineBuffer );
            thisLine >> colourName >> ws >> r >> g >> b >> ws;

            BTree <ColourOption *> *exists = colours.LookupTree( colourName );
            if ( !exists ) {
                colours.PutData( colourName, new ColourOption ( r, g, b ) );
            }
            else {
                delete exists->data;
                exists->data = new ColourOption ( r, g, b );
            }

        }

        thefile.close();
    }


    RsArchiveFileClose ( themeTextFilename, NULL );
    delete [] themeTextFilename;

}

char *Options::GetThemeName ()
{
    return themeName;
}

char *Options::GetThemeTitle ()
{
    return themeTitle;
}

char *Options::GetThemeDescription ()
{
    return themeDescription;
}

ColourOption *Options::GetColour ( char *colourName )
{

    ColourOption *result = colours.GetData (colourName);

    if ( result ) {
        return result;
    }
    else {

        printf ( "Options::GetColour WARNING : Couldn't find colour %s\n", colourName );
        return &getColourDefault;

    }

}

char *Options::ThemeFilename ( char *filename )
{

    char *result = new char [256];

    if ( strcmp ( themeName, "graphics" ) == 0 ) {
    
        sprintf ( result, "graphics/%s", filename );

    }
    else {

        char fullFilename[256];
        sprintf ( fullFilename, "%s%s/%s", app->path, themeName, filename );
        if ( DoesFileExist ( fullFilename ) )         
            sprintf ( result, "%s/%s", themeName, filename );

        else
            sprintf ( result, "graphics/%s", filename );

    }

    return result;

}

void Options::RequestShutdownChange ( char *optionName, int newValue )
{

    OptionChange *oc = new OptionChange ();
    strcpy ( oc->name, optionName );
    oc->value = newValue;

    shutdownChanges.PutData( oc );

}

void Options::ApplyShutdownChanges ()
{

    while ( shutdownChanges.GetData(0) ) {

        OptionChange *oc = shutdownChanges.GetData(0);
        shutdownChanges.RemoveData(0);

        SetOptionValue ( oc->name, oc->value );

        delete oc;

    }

}

void Options::Load ( FILE *file )
{

	// Read from our own options file

	char filename [256];
	sprintf ( filename, "%soptions", app->userpath );
	printf ( "Loading uplink options from %s...", filename );

	FILE *optionsfile = fopen ( filename, "rb" );
	
	if ( optionsfile ) {

	    char version [32];
	    fread ( version, sizeof (SAVEFILE_VERSION), 1, optionsfile );
        if ( strcmp ( version, SAVEFILE_VERSION ) != 0 ) {
            printf ( "\nERROR : Could not load options due to old version format\n" );
            fclose ( optionsfile );
            return;
        }

		printf ( "success\n" );

		LoadID ( optionsfile );
		LoadBTree ( (BTree <UplinkObject *> *) &options, optionsfile );
		LoadID_END ( optionsfile );

		// 't' in the optionsfile at this point signals
		// theme name comes next

		char newThemeName[sizeof(themeName)];
		unsigned short newThemeLen;

		if (fgetc(optionsfile) == 't' &&
			fread(&newThemeLen, sizeof(newThemeLen), 1, optionsfile) == 1 &&
			newThemeLen + 1 < sizeof(themeName) &&
			fread(newThemeName, newThemeLen, 1, optionsfile) == 1) {
		  newThemeName[newThemeLen] = '\0';
		  strcpy(themeName, newThemeName);
		}

		fclose ( optionsfile );

	}
	else {

//        float currentVersion;
//        sscanf ( VERSION_NUMBER, "%f", &currentVersion );
//        currentVersion *= 100;
//        SetOptionValue ( "game_version", currentVersion, "z", false, false );

		printf ( "failed\n" );

	}

}

void Options::Save ( FILE *file )
{

	// Write to our own options file

	MakeDirectory ( app->userpath );

	char filename [256];
	sprintf ( filename, "%soptions", app->userpath );

	printf ( "Saving uplink options to %s...", filename );

	FILE *optionsfile = fopen ( filename, "wb" );
	
	if ( optionsfile ) {

		printf ( "success\n" );

		fwrite ( SAVEFILE_VERSION, sizeof (SAVEFILE_VERSION), 1, optionsfile );

		SaveID ( optionsfile );
		SaveBTree ( (BTree <UplinkObject *> *) &options, optionsfile );
		SaveID_END ( optionsfile );

		fputc('t', optionsfile);
		unsigned short themeLen = strlen(themeName);
		fwrite(&themeLen, sizeof(themeLen), 1, optionsfile);
		fwrite(themeName, themeLen, 1, optionsfile);

		fclose ( optionsfile );

	}
	else {

		printf ( "failed\n" );

	}

}

void Options::Print ()
{

	printf ( "============== O P T I O N S ===============================\n" );

	PrintBTree ( (BTree <UplinkObject *> *) &options );

	printf ( "============================================================\n" );

}

void Options::Update ()
{
}

char *Options::GetID ()
{

	return "OPTIONS";

}

Options *options;


// ============================================================================
// Option class


Option::Option()
{

	sprintf ( name, "" );
	sprintf ( tooltip, "" );
	yesorno = false;
	visible = true;
	value = 0;

}

Option::~Option()
{

}

void Option::SetName ( char *newname )
{

	UplinkAssert ( strlen(newname) < SIZE_OPTION_NAME );
	strcpy ( name, newname );

}

void Option::SetTooltip ( char *newtooltip )
{

	UplinkAssert ( strlen(newtooltip) < SIZE_OPTION_TOOLTIP );
	strcpy ( tooltip, newtooltip );

}

void Option::SetYesOrNo	( bool newyesorno )
{

	yesorno = newyesorno;

}

void Option::SetVisible ( bool newvisible )
{

	visible = newvisible;

}

void Option::SetValue ( int newvalue )
{

	value = newvalue;

}

void Option::Load ( FILE *file )
{

	LoadID ( file );

	fread ( name, sizeof(name), 1, file );
	fread ( tooltip, sizeof(tooltip), 1, file );
	fread ( &yesorno, sizeof(yesorno), 1, file );
	fread ( &visible, sizeof(visible), 1, file );
	fread ( &value, sizeof(value), 1, file );

	LoadID_END ( file );

}

void Option::Save ( FILE *file )
{
	
	SaveID ( file );

	fwrite ( name, sizeof(name), 1, file );
	fwrite ( tooltip, sizeof(tooltip), 1, file );
	fwrite ( &yesorno, sizeof(yesorno), 1, file );
	fwrite ( &visible, sizeof(visible), 1, file );
	fwrite ( &value, sizeof(value), 1, file );

	SaveID_END ( file );

}

void Option::Print ()
{

	printf ( "Option : name=%s, value=%d\n", name, value );
	printf ( "\tYesOrNo=%d, Visible=%d\n", yesorno, visible );

}

void Option::Update ()
{
}

char *Option::GetID ()
{

	return "OPTION";

}

int Option::GetOBJECTID ()
{

	return OID_OPTION;

}
