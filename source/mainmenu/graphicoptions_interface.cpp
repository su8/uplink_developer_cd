
#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>

#include <GL/glu.h>

#include "soundgarden.h"
#include "redshirt.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/miscutils.h"
#include "app/opengl.h"
#include "app/opengl_interface.h"

#include "options/options.h"

#include "mainmenu/mainmenu.h"
#include "mainmenu/graphicoptions_interface.h"
#include "mainmenu/login_interface.h"


int GraphicOptionsInterface::newScreenWidth = 0;
int GraphicOptionsInterface::newScreenHeight = 0;
int GraphicOptionsInterface::newColourDepth = 0;
int GraphicOptionsInterface::newRefresh = 0;
bool GraphicOptionsInterface::screenSettingsChanged = false;


void GraphicOptionsInterface::ReturnToMainMenuClick ( Button *button )
{

	app->GetMainMenu ()->RunScreen ( MAINMENU_OPTIONS );

}

void GraphicOptionsInterface::ApplyClick ( Button *button )
{

	GraphicOptionsInterface *thisint = (GraphicOptionsInterface *) app->GetMainMenu ()->GetMenuScreen ();
	UplinkAssert (thisint);

	int index = 0;
	char name1 [64];
	sprintf ( name1, "graphic_option %d", index );

    bool shutdownRequired = false;

	while ( EclGetButton ( name1 ) != NULL ) {

		// Look up the next option button

		char name2 [64];
		sprintf ( name2, "graphic_value %d", index );

		Button *namebutton = EclGetButton ( name1 );
		Button *valuebutton = EclGetButton ( name2 );

		UplinkAssert (namebutton);
		UplinkAssert (valuebutton);

		// Extract the values

		char optionname [64];
		int newvalue;
		sprintf ( optionname, "%s_%s", thisint->optionTYPE, namebutton->caption );
		sscanf ( valuebutton->caption, "%d", &newvalue );
		
		// If they've changed, update them

		Option *thisoption = app->GetOptions ()->GetOption (optionname);

        if ( thisoption && thisoption->value != newvalue ) {
			bool thisShutdown = thisint->ChangeOptionValue ( optionname, newvalue );
            if ( thisShutdown )
                shutdownRequired = true;
        }

		// Next button

		++index;
		sprintf ( name1, "graphic_option %d", index );

	}

    if ( screenSettingsChanged ) {

        app->GetOptions ()->RequestShutdownChange ( "graphics_screenwidth", newScreenWidth );
        app->GetOptions ()->RequestShutdownChange ( "graphics_screenheight", newScreenHeight );
        app->GetOptions ()->RequestShutdownChange ( "graphics_screendepth", newColourDepth );
        app->GetOptions ()->RequestShutdownChange ( "graphics_screenrefresh", newRefresh );
        shutdownRequired = true;

    }

    if ( shutdownRequired ) {
    
        create_msgbox ( "Shutdown", "Uplink must be restarted\nbefore this change can occur" );		
        EclRegisterButtonCallback ( "msgbox_close", ExitGameClick );
        EclRegisterButtonCallback ( "msgbox_title", ExitGameClick );

    }

}

void GraphicOptionsInterface::ToggleBoxDraw ( Button *button, bool highlighted, bool clicked )
{

	UplinkAssert (button);

	if ( button->caption [0] == '0' )
		imagebutton_draw ( button, false, false );

	else
		imagebutton_draw ( button, true, true );

}

void GraphicOptionsInterface::ToggleBoxClick ( Button *button )
{

	UplinkAssert (button);

	// Get the button index

	char unused [64];
	int buttonindex;
	sscanf ( button->name, "%s %d", unused, &buttonindex );

	// Grab the appropriate button

	char name2 [64];
	sprintf ( name2, "graphic_value %d", buttonindex );
	Button *button2 = EclGetButton ( name2 );
	UplinkAssert (button2);

	// Read the value from the button
	
	if ( button2->caption [0] == '0' ) 
		button2->SetCaption ( "1" );

	else
		button2->SetCaption ( "0" );

}


GraphicOptionsInterface::GraphicOptionsInterface ()
{
}

GraphicOptionsInterface::~GraphicOptionsInterface ()
{
}

bool GraphicOptionsInterface::ChangeOptionValue ( char *option, int newvalue )
{

	//
	// Deal with specific options
	//

	if ( strcmp ( option, "graphics_buttonanimations" ) == 0 ) {

    	app->GetOptions ()->SetOptionValue ( option, newvalue );

		if ( newvalue == 0 ) EclDisableAnimations ();
		else				 EclEnableAnimations ();

        return false;

	}
	else if ( strcmp ( option, "graphics_fullscreen" ) == 0 ||
         strcmp ( option, "graphics_screendepth" ) == 0 ||
         strcmp ( option, "graphics_screenrefresh" ) == 0 ||
         strcmp ( option, "graphics_safemode" ) == 0 || 
         strcmp ( option, "graphics_screenwidth" ) == 0 ||
         strcmp ( option, "graphics_screenheight" ) == 0 ) {

        app->GetOptions ()->RequestShutdownChange ( option, newvalue );

        return true;

	}
    else {

        app->GetOptions ()->SetOptionValue ( option, newvalue );
        return false;

    }

}

void GraphicOptionsInterface::ScreenOptionDraw ( Button *button, bool highlighted, bool clicked )
{

    UplinkAssert (button);
   
    char unused [128];
    int type;
    int valueA;
    int valueB;
    sscanf ( button->name, "%s %d %d %d", unused, &type, &valueA, &valueB );

    bool currentValue = false;
    if ( type == 1 && 
         valueA == newScreenWidth && 
         valueB == newScreenHeight ) {
        currentValue = true;
    }
    else if ( type == 2 && valueA == newColourDepth ) {
        currentValue = true;

    }
    else if ( type == 3 && valueA == newRefresh ) {
        currentValue = true;

    }


    if ( highlighted || clicked || currentValue ) {

	    glBegin ( GL_QUADS );		
		    SetColour ( "PanelHighlightA" );	glVertex2i ( button->x, button->y + button->height );
		    SetColour ( "PanelHighlightB" );	glVertex2i ( button->x, button->y );
		    SetColour ( "PanelHighlightA" );	glVertex2i ( button->x + button->width, button->y );
		    SetColour ( "PanelHighlightB" );	glVertex2i ( button->x + button->width, button->y + button->height );
	    glEnd ();

    }
    else {

	    glBegin ( GL_QUADS );		
		    SetColour ( "PanelBackgroundA" );   glVertex2i ( button->x, button->y + button->height );
		    SetColour ( "PanelBackgroundB" );   glVertex2i ( button->x, button->y );
		    SetColour ( "PanelBackgroundA" );   glVertex2i ( button->x + button->width, button->y );
		    SetColour ( "PanelBackgroundB" );   glVertex2i ( button->x + button->width, button->y + button->height );
	    glEnd ();

    }

    if ( clicked || currentValue ) {

        SetColour ( "PanelHighlightBorder" );
        border_draw ( button );

    }
    else {

	    SetColour ( "PanelBorder" );
	    border_draw ( button );

    }

	int xpos = (button->x + button->width  / 2) - ( GciTextWidth ( button->caption ) / 2 );
	int ypos = (button->y + button->height / 2) + 2;

    SetColour ( "DefaultText" );
    GciDrawText ( xpos, ypos, button->caption );

}

void GraphicOptionsInterface::ScreenOptionClick ( Button *button )
{

    // 
    // Change new button

    UplinkAssert (button);

    char unused [128];
    int type;
    int valueA;
    int valueB;
    sscanf ( button->name, "%s %d %d %d", unused, &type, &valueA, &valueB );

    if ( type == 1 ) {

        newScreenWidth = valueA;
        newScreenHeight = valueB;

    }
    else if ( type == 2 ) {
        
        newColourDepth = valueA;

    }
    else if ( type == 3 ) {

        newRefresh = valueA;

    }

    screenSettingsChanged = true;

    int screenW = app->GetOptions ()->GetOptionValue ( "graphics_screenwidth" );
    int screenH = app->GetOptions ()->GetOptionValue ( "graphics_screenheight" );
    int startY = screenH - 170;
    EclDirtyRectangle ( 0, startY, screenW, screenH - startY );

}

void GraphicOptionsInterface::ExitGameClick ( Button *button )
{

	app->Close ();

}

void GraphicOptionsInterface::SetOptionTYPE ( char *newtype )
{

	sprintf ( optionTYPE, newtype );

	//
	// Build a list of all options of this type
	//

	LList <Option *> *options = app->GetOptions ()->GetAllOptions ( newtype, false );
	
	//
	// Create Title
	//

	int screenw = app->GetOptions ()->GetOptionValue ("graphics_screenwidth");
	int screenh = app->GetOptions ()->GetOptionValue ("graphics_screenheight");

	char title [64];
	sprintf ( title, "%s options", optionTYPE );
	EclRegisterButton ( screenw - 210, screenh - 40, 200, 15, title, "Close this options screen", "graphic_title" );
	EclRegisterButtonCallback ( "graphic_title", ReturnToMainMenuClick );
	EclRegisterMovement ( "graphic_title", screenw - 210, screenh - 100 - options->Size () * 20, 500 );

	//
	// Create Each option name and text edit box
	//

	for ( int i = 0; i < options->Size (); ++i ) {
	
		Option *option = options->GetData (i);
		UplinkAssert (option);

		char name1 [64];
		char name2 [64];

		sprintf ( name1, "graphic_option %d", i );
		sprintf ( name2, "graphic_value %d", i );

		char *optionname = strchr ( option->name, '_' ) + 1;
		
		char value [8];
		sprintf ( value, "%d", option->value );

		EclRegisterButton ( screenw - 210, screenh - 40, 150, 15, optionname, option->tooltip, name1 );
		EclRegisterButtonCallbacks ( name1, button_draw, ToggleBoxClick, button_click, button_highlight );
        
		if ( option->yesorno ) {

			EclRegisterButton ( screenw - 60, screenh - 40, 15, 15, value, "click to Enable or Disable this option", name2 );
			button_assignbitmaps ( name2, "mainmenu/optionno.tif", "mainmenu/optionyes.tif", "mainmenu/optionyes.tif" );
			EclRegisterButtonCallbacks ( name2, ToggleBoxDraw, ToggleBoxClick, button_click, button_highlight );

		}
		else {

			EclRegisterButton ( screenw - 60, screenh - 40, 50, 15, value, "enter the new value here", name2 );
			EclRegisterButtonCallbacks ( name2, textbutton_draw, NULL, button_click, button_highlight );
            EclMakeButtonEditable ( name2 );

		}

		int timems = 500 * ((float) (i + 1) / (float) options->Size ());
		EclRegisterMovement ( name1, screenw - 210, screenh - 90 - i * 20, timems );
		EclRegisterMovement ( name2, screenw - 60, screenh - 90 - i * 20, timems );

	}

	delete options;

	// 
	// Create "Apply" button
	//

	EclRegisterButton ( screenw - 210, screenh - 40, 145, 15, "Apply Changes", "Apply all changes", "graphic_apply" );
	EclRegisterButtonCallback ( "graphic_apply", ApplyClick );

}

void GraphicOptionsInterface::Create ()
{
	
	if ( !IsVisible () ) {

		MainMenuScreen::Create ();

		int screenw = app->GetOptions ()->GetOptionValue ("graphics_screenwidth");
		int screenh = app->GetOptions ()->GetOptionValue ("graphics_screenheight");

		//
		// Close button

		EclRegisterButton ( screenw - 60, screenh - 40, 50, 15, "Close", "Return to Options menu", "graphic_return" );
		EclRegisterButtonCallback ( "graphic_return", ReturnToMainMenuClick );
		
		//
		// Set me up baby

		SetOptionTYPE ( "graphics" );

        //
        // Additional resolution change buttons

        int startY = screenh - 170;

        EclRegisterButton ( 50, startY, 100, 15, "RESOLUTION", "Select your screen resolution", "graphic_screenrestitle" );        
        EclRegisterButton ( 50, startY + 30, 100, 15, "640x480", "Choose this resolution", "graphic 1 640 480" );
        EclRegisterButton ( 50, startY + 50, 100, 15, "800x600", "Choose this resolution", "graphic 1 800 600" );
        EclRegisterButton ( 50, startY + 70, 100, 15, "1024x768", "Choose this resolution", "graphic 1 1024 768" );
        EclRegisterButton ( 50, startY + 90, 100, 15, "1280x1024", "Choose this resolution", "graphic 1 1280 1024" );
        EclRegisterButton ( 50, startY + 110, 100, 15, "1600x1200", "Choose this resolution", "graphic 1 1600 1200" );
        EclRegisterButtonCallbacks ( "graphic 1 640 480", ScreenOptionDraw, ScreenOptionClick, button_click, button_highlight );
        EclRegisterButtonCallbacks ( "graphic 1 800 600", ScreenOptionDraw, ScreenOptionClick, button_click, button_highlight );
        EclRegisterButtonCallbacks ( "graphic 1 1024 768", ScreenOptionDraw, ScreenOptionClick, button_click, button_highlight );
        EclRegisterButtonCallbacks ( "graphic 1 1280 1024", ScreenOptionDraw, ScreenOptionClick, button_click, button_highlight );
        EclRegisterButtonCallbacks ( "graphic 1 1600 1200", ScreenOptionDraw, ScreenOptionClick, button_click, button_highlight );

        EclRegisterButton ( 170, startY, 100, 15, "COLOUR DEPTH", "Select your colour depth", "graphic_colourdepthtitle" );
        EclRegisterButton ( 170, startY + 30, 100, 15, "8 Bit", "Choose this colour depth", "graphic 2 8 0" );
        EclRegisterButton ( 170, startY + 50, 100, 15, "16 Bit", "Choose this colour depth", "graphic 2 16 0" );
        EclRegisterButton ( 170, startY + 70, 100, 15, "32 Bit", "Choose this colour depth", "graphic 2 32 0" );
        EclRegisterButtonCallbacks ( "graphic 2 8 0", ScreenOptionDraw, ScreenOptionClick, button_click, button_highlight );
        EclRegisterButtonCallbacks ( "graphic 2 16 0", ScreenOptionDraw, ScreenOptionClick, button_click, button_highlight );
        EclRegisterButtonCallbacks ( "graphic 2 32 0", ScreenOptionDraw, ScreenOptionClick, button_click, button_highlight );

        EclRegisterButton ( 290, startY, 100, 15, "REFRESH", "Select your refresh rate", "graphic_refreshtitle" );
        EclRegisterButton ( 290, startY + 30, 100, 15, "60 Hz", "Choose this refresh rate", "graphic 3 60 0" );
        EclRegisterButton ( 290, startY + 50, 100, 15, "70 Hz", "Choose this refresh rate", "graphic 3 70 0" );
        EclRegisterButton ( 290, startY + 70, 100, 15, "75 Hz", "Choose this refresh rate", "graphic 3 75 0" );
        EclRegisterButton ( 290, startY + 90, 100, 15, "85 Hz", "Choose this refresh rate", "graphic 3 85 0" );
        EclRegisterButton ( 290, startY + 110, 100, 15, "100 Hz", "Choose this refresh rate", "graphic 3 100 0" );
        EclRegisterButtonCallbacks ( "graphic 3 60 0", ScreenOptionDraw, ScreenOptionClick, button_click, button_highlight );
        EclRegisterButtonCallbacks ( "graphic 3 70 0", ScreenOptionDraw, ScreenOptionClick, button_click, button_highlight );
        EclRegisterButtonCallbacks ( "graphic 3 75 0", ScreenOptionDraw, ScreenOptionClick, button_click, button_highlight );
        EclRegisterButtonCallbacks ( "graphic 3 85 0", ScreenOptionDraw, ScreenOptionClick, button_click, button_highlight );
        EclRegisterButtonCallbacks ( "graphic 3 100 0", ScreenOptionDraw, ScreenOptionClick, button_click, button_highlight );


        newScreenWidth = app->GetOptions ()->GetOptionValue ("graphics_screenwidth");
        newScreenHeight = app->GetOptions ()->GetOptionValue ("graphics_screenheight");
        newColourDepth = app->GetOptions ()->GetOptionValue ("graphics_screendepth");
        newRefresh = app->GetOptions ()->GetOptionValue ("graphics_screenrefresh" );
        screenSettingsChanged = false;

	}

}

void GraphicOptionsInterface::Remove ()
{

	if ( IsVisible () ) {

		EclRemoveButton ( "graphic_return" );
		EclRemoveButton ( "graphic_title" );

		int index = 0;
		char name1 [64];
		sprintf ( name1, "graphic_option %d", index );

		while ( EclGetButton ( name1 ) != NULL ) {

			char name2 [64];
			sprintf ( name2, "graphic_value %d", index );

			EclRemoveButton ( name1 );
			EclRemoveButton ( name2 );

			++index;
			sprintf ( name1, "graphic_option %d", index );

		}

		EclRemoveButton ( "graphic_apply" );

        EclRemoveButton ( "graphic_screenrestitle" );
        EclRemoveButton ( "graphic 1 640 480" );
        EclRemoveButton ( "graphic 1 800 600" );
        EclRemoveButton ( "graphic 1 1024 768" );
        EclRemoveButton ( "graphic 1 1280 1024" );
        EclRemoveButton ( "graphic 1 1600 1200" );

        EclRemoveButton ( "graphic_colourdepthtitle" );
        EclRemoveButton ( "graphic 2 8 0" );
        EclRemoveButton ( "graphic 2 16 0" );
        EclRemoveButton ( "graphic 2 32 0" );

        EclRemoveButton ( "graphic_refreshtitle" );
        EclRemoveButton ( "graphic 3 60 0" );
        EclRemoveButton ( "graphic 3 70 0" );
        EclRemoveButton ( "graphic 3 75 0" );
        EclRemoveButton ( "graphic 3 85 0" );
        EclRemoveButton ( "graphic 3 100 0" );

	}

}

void GraphicOptionsInterface::Update ()
{
}

bool GraphicOptionsInterface::IsVisible ()
{

	return ( EclGetButton ( "graphic_return" ) != NULL );

}


int  GraphicOptionsInterface::ScreenID ()
{

	return MAINMENU_GRAPHICOPTIONS;

}

