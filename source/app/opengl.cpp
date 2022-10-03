// -*- tab-width:4; c-file-style:"cc-mode" -*- 
/*

	All generic OpenGL code goes in here
	i.e mouse handler, keyboard handler, idle function etc

  */

#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>

#include <GL/glu.h>

#include "tosser.h"
#include "eclipse.h"
#include "vanbakel.h"
#include "gucci.h"
#include "soundgarden.h"

#include "options/options.h"

#include "app/app.h"
#include "app/opengl.h"
#include "app/globals.h"
#include "app/opengl_interface.h"

#include "mainmenu/mainmenu.h"
#include "mainmenu/mainmenuscreen.h"

#include "view/view.h"

#include "game/game.h"

#include "interface/interface.h"
#include "interface/scrollbox.h"
#include "interface/localinterface/localinterface.h"
#include "interface/localinterface/hud_interface.h"
#include "interface/localinterface/irc_interface.h"
#include "interface/remoteinterface/remoteinterface.h"
#include "interface/taskmanager/taskmanager.h"
#include "interface/taskmanager/uplinktask.h"

#include "world/world.h"
#include "world/player.h"

//#define  DRAWMOUSE

// ============================================================================
// Local function definitions


local void init(void);
local void keyboard(unsigned char, int, int);
local void specialkeyboard(int, int, int);
local void mouse(int,int,int,int);
local void mousemove(int,int);
local void passivemouse(int,int);
local void resize(int, int);
local void drawcube(int, int, int);
local void idle(void);

local int lastidleupdate = 0;
local int mouseX = 0;
local int mouseY = 0;

// ============================================================================


void opengl_initialise (int argc, char **argv)
{

	bool debugging = false;
	if ( app->GetOptions ()->IsOptionEqualTo ( "game_debugstart", 1 ) )
		debugging = true;

	int screenWidth = app->GetOptions ()->GetOptionValue ( "graphics_screenwidth" );
	int screenHeight = app->GetOptions ()->GetOptionValue ( "graphics_screenheight" );
	bool runFullScreen = app->GetOptions ()->IsOptionEqualTo ( "graphics_fullscreen", 1 ) &&
						!app->GetOptions ()->IsOptionEqualTo ( "graphics_safemode", 1 );
	int screenDepth = app->GetOptions ()->GetOptionValue ( "graphics_screendepth" );
    int screenRefresh = app->GetOptions ()->GetOptionValue ( "graphics_screenrefresh" );

	GciInitGraphics( "uplink",
			 GCI_DOUBLE | 
			 GCI_RGB | 
			 (runFullScreen ? GCI_FULLSCREEN : 0) |
			 (debugging ? GCI_DEBUGSTART : 0), 
			 screenWidth, screenHeight,
			 screenDepth, screenRefresh,
			 argc, argv );

	if ( debugging ) printf ( "Initialising OpenGL...\n" );
	init();
	if ( debugging ) printf ( "Finished initialising OpenGL.\n" );
	

	if ( debugging ) printf ( "Now registering callback functions..." );
    setcallbacks ();
    if ( debugging ) printf ( "done\n ");

}

void setcallbacks ()
{
	GciDisplayFunc(display);	
	GciMouseFunc(mouse);
	GciMotionFunc(mousemove);
	GciPassiveMotionFunc(passivemouse);
	GciKeyboardFunc(keyboard);
	GciSpecialFunc(specialkeyboard);
	GciIdleFunc (idle);
	GciReshapeFunc(resize);  
}

void opengl_run ()
{

	GciMainLoop ();

}

void opengl_close ()
{

	GciRestoreScreenSize ();

}

local void init(void)
{

/*
	// First things first - show the OpenGL version in use
	printf ( "\n" );
	printf ( "OpenGL Driver Information\n" );
	printf ( "Vendor   : %s\n", glGetString ( GL_VENDOR ) );
	printf ( "Renderer : %s\n", glGetString ( GL_RENDERER ) );
	printf ( "Version  : %s\n", glGetString ( GL_VERSION ) );
	printf ( "GL Utils : %s\n", gluGetString ( (GLenum) GLU_VERSION ) );
	printf ( "\n" );
*/

	glClearColor(0.0, 0.0, 0.0, 0.0);

	// ====================================================================== 
	// Fix for Riva TNT cards (these don't automatically clear the background
	// UPDATE : Should now 	be covered by the GLUT_NORMAL_DAMAGED code in display()
	// int screenwidth = app->GetOptions ()->GetOptionValue ( "graphics_screenwidth" );
	// int screenheight = app->GetOptions ()->GetOptionValue ( "graphics_screenheight" );
	// clear_draw ( 0, 0, screenwidth, screenheight );
	// ======================================================================

//	glEnable(GL_DEPTH_TEST);
	glDisable ( GL_DEPTH_TEST );
		
	glMatrixMode(GL_MODELVIEW);

	glTexEnvf ( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );	

        glDisable(GL_ALPHA_TEST);        
        glDisable(GL_FOG);        
		glDisable(GL_LIGHTING);
        glDisable(GL_LOGIC_OP);        
		glDisable(GL_STENCIL_TEST);
        glDisable(GL_TEXTURE_1D);      
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		
        glPixelTransferi(GL_MAP_COLOR, GL_FALSE);
        glPixelTransferi(GL_RED_SCALE, 1);
        glPixelTransferi(GL_RED_BIAS, 0);
        glPixelTransferi(GL_GREEN_SCALE, 1);
        glPixelTransferi(GL_GREEN_BIAS, 0);
        glPixelTransferi(GL_BLUE_SCALE, 1);
        glPixelTransferi(GL_BLUE_BIAS, 0);
        glPixelTransferi(GL_ALPHA_SCALE, 1);
        glPixelTransferi(GL_ALPHA_BIAS, 0);

	GLuint texName;
	glGenTextures( 1, &texName );
	glBindTexture ( GL_TEXTURE_2D, texName );	

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glHint ( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
	glHint ( GL_LINE_SMOOTH_HINT,	 GL_NICEST );
	glHint ( GL_POINT_SMOOTH_HINT,	 GL_NICEST );

    EclReset ( app->GetOptions ()->GetOptionValue ("graphics_screenwidth"),
			   app->GetOptions ()->GetOptionValue ("graphics_screenheight") );
	EclRegisterClearDrawFunction      ( clear_draw );
	EclRegisterDefaultButtonCallbacks ( button_draw, NULL, button_click, button_highlight );
	EclRegisterSuperHighlightFunction ( 3, superhighlight_draw );

	if ( app->GetOptions ()->GetOption ("graphics_buttonanimations") &&
		 app->GetOptions ()->GetOption ("graphics_buttonanimations")->value == 0 ) {

		EclDisableAnimations ();

	}

}

void display(void)
{

    if ( app->Closed () ) return;

	// 
	// If the display was damaged (eg by other window activity)
	// Then we need to blank and both buffers now
	//

        if (GciLayerDamaged())
		EclDirtyRectangle ( 0, 0, app->GetOptions ()->GetOptionValue ( "graphics_screenwidth" ), 
								  app->GetOptions ()->GetOptionValue ( "graphics_screenheight" ) );

	//
	// If safe mode redraw EVERYTHING
	//

	if ( app->GetOptions ()->IsOptionEqualTo ( "graphics_safemode", 1 ) )
		EclDirtyRectangle ( 0, 0, app->GetOptions ()->GetOptionValue ( "graphics_screenwidth" ), 
								  app->GetOptions ()->GetOptionValue ( "graphics_screenheight" ) );

	//
	// Draw the world into both buffers
	//

	for ( int i = 0; i < 2; ++i ) {

		//  Draw the Eclipse buttons

		glPushMatrix ();
		glLoadIdentity ();
		glMatrixMode ( GL_PROJECTION );
		glPushMatrix ();
		glLoadIdentity ();
		glPushAttrib ( GL_ALL_ATTRIB_BITS );
        
		glOrtho ( 0.0, app->GetOptions ()->GetOptionValue ( "graphics_screenwidth" ), 
					   app->GetOptions ()->GetOptionValue ( "graphics_screenheight" ), 0.0, -1.0, 1.0 );

		glTranslatef ( 0.375, 0.375, 0.0 );
        
		EclDrawAllButtons ();

		glPopAttrib ();
		glPopMatrix ();
		glMatrixMode ( GL_MODELVIEW );
		glPopMatrix ();

		//  Swap the buffers, do it all again to the new back-buffer
		GciSwapBuffers();

	}

	//
	// Finished drawing to the two buffers - clear all dirty areas
	//

	EclDirtyClear ();

}

void keyboard(unsigned char key, int x, int y)
{

	if ( key == 13 ) {									// ======== Return key

      	bool returned = false;

        if ( game->IsRunning () ) 
        {
            if ( game->GetInterface ()->GetLocalInterface ()->InScreen() != SCREEN_NONE &&
                 game->GetInterface ()->GetLocalInterface ()->GetInterfaceScreen ()->ScreenID () == SCREEN_IRC )
                returned = ((IRCInterface *) game->GetInterface ()->GetLocalInterface ()->GetInterfaceScreen ())->ReturnKeyPressed ();
            else
			    returned = game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ()->ReturnKeyPressed ();
        }
		else if ( app->GetMainMenu ()->InScreen () != MAINMENU_UNKNOWN )
			returned = app->GetMainMenu ()->GetMenuScreen ()->ReturnKeyPressed ();

	    if ( !returned ) {

		    // Pass the key press on to the box under the mouse
		    char *name = EclGetHighlightedButton ();
		    if ( EclIsButtonEditable (name) ) 
			    textbutton_keypress ( EclGetButton (name), key );

	    }

	}
    else if ( key == 27 ) {                             // ======== Esc key

		char *name = EclGetHighlightedButton ();

        if ( EclIsButtonEditable (name) ) {

            textbutton_keypress ( EclGetButton (name), key );

        }
        else {
        
            if ( game->IsRunning () )
                bool escaped = game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ()->EscapeKeyPressed ();

        }

    }
	else if ( key == 9 ) {								// ========= Tab key
		
        EclHighlightNextEditableButton ();

	}
    else {

		char *name = EclGetHighlightedButton ();
		if ( EclIsButtonEditable (name) ) 
			textbutton_keypress ( EclGetButton (name), key );

    }

	GciPostRedisplay();

}

void specialkeyboard (int key, int x, int y)
{

	switch ( key ) {

		case GCI_KEY_F1:							// Cheat menu

#ifdef CHEATMODES_ENABLED
            game->GetInterface ()->GetLocalInterface ()->RunScreen ( SCREEN_CHEATS );
#else
	#ifndef DEMOGAME
		#ifndef WAREZRELEASE								// Prevent cheats from working in the warez release
//            if ( game->IsRunning () && strcmp ( game->GetWorld ()->GetPlayer ()->handle, "TooManySecrets" ) == 0 )
//                game->GetInterface ()->GetLocalInterface ()->RunScreen ( SCREEN_CHEATS );
		#endif
	#endif
#endif  
			break;



#ifndef DEMOGAME

		case GCI_KEY_F12:							// Exit
			if ( game->IsRunning () ) app->SaveGame ( game->GetWorld ()->GetPlayer ()->handle );
			app->Close ();
			break;

#endif
            
	}

	GciPostRedisplay ();

}

void idle ()
{

    if ( app->Closed () ) return;

	int timesincelastupdate = EclGetAccurateTime () - lastidleupdate;

	EclUpdateAllAnimations ();		
    SgUpdate ();
	app->Update ();
	GciPostRedisplay ();

	lastidleupdate = EclGetAccurateTime ();

}

void mouse ( int button, int state, int x, int y )
{

	if ( button == GCI_LEFT_BUTTON && state == GCI_DOWN ) {

		char *name = EclGetButtonAtCoord ( x, y );

		if ( name ) {
			Button *button = EclGetButton ( name );
			if ( button ) button->MouseDown ();			
		}

	}
	else if ( button == GCI_LEFT_BUTTON && state == GCI_UP ) {

		EclUnClickButton ();
        ScrollBox::UnGrabScrollBar();
		GciPostRedisplay ();

		char *name = EclGetButtonAtCoord ( x, y );
		
		if ( name ) {			
			Button *button = EclGetButton ( name );
			if ( button ) button->MouseUp ();
		}
		else {

			// Clicked on nothing - remove the software menu
			if ( game->IsRunning () && 
				game->GetInterface ()->GetLocalInterface ()->GetHUD ()->si.IsVisibleSoftwareMenu () ) {

				game->GetInterface ()->GetLocalInterface ()->GetHUD ()->si.ToggleSoftwareMenu ();

			}

		}

	}
	else if ( button == GCI_RIGHT_BUTTON && state == GCI_UP ) {

		// Put down any programs that are linked to the cursur

		if ( game->IsRunning () ) 
			game->GetInterface ()->GetTaskManager ()->SetTargetProgram ( -1 );

	}

}

local void mousedraw ( Button *button, bool highlighted, bool clicked )
{

	UplinkAssert (button);

	glColor4f ( 1.0, 1.0, 1.0, 1.0 );

	glBegin ( GL_LINES );

		glLineWidth ( 1.0 );
		glVertex2i ( button->x, button->y );
		glVertex2i ( button->x + button->width - 1, button->y + button->height - 1 );
		
		glVertex2i ( button->x, button->y );
		glVertex2i ( button->x + button->width/1.5, button->y );

		glVertex2i ( button->x, button->y );
		glVertex2i ( button->x, button->y + button->height/1.5 );
	glEnd ();

	glLineWidth ( 1.0 );

}

void passivemouse ( int x, int y )
{

    if ( app->Closed () ) return;

	mouseX = x;
	mouseY = y;

    bool showSWMouse = app->GetOptions ()->IsOptionEqualTo ( "graphics_softwaremouse", 1 );

	if ( showSWMouse ) {

		// Draw the mouse

		if ( !EclGetButton ( "mouse" ) ) {
			EclRegisterButton ( 0, 0, 8, 8, "", "mouse" );
			EclRegisterButtonCallbacks ( "mouse", mousedraw, NULL, NULL, NULL );
		}

		EclButtonBringToFront ( "mouse" );
		Button *mouse = EclGetButton ( "mouse" );
		UplinkAssert (mouse);
		EclDirtyRectangle ( mouse->x, mouse->y, mouse->width, mouse->height );
		EclGetButton ( "mouse" )->x = x + 1;
		EclGetButton ( "mouse" )->y = y + 1;
		EclDirtyRectangle ( mouse->x, mouse->y, mouse->width, mouse->height );

	}

	// Look for any buttons under the mouse

	char *name = EclGetButtonAtCoord ( x, y );

	if ( name ) {
		Button *button = EclGetButton ( name );
		if ( button ) button->MouseMove ();			
	}
	else {
		EclUnHighlightButton ();
		tooltip_update ( " " );
		GciPostRedisplay ();
	}

}

void mousemove(int x, int y)
{

    if ( app->Closed () ) return;

	mouseX = x;
	mouseY = y;

    // Update scroll bars

    if ( ScrollBox::IsGrabInProgress() ) ScrollBox::UpdateGrabScroll();


    bool showSWMouse = app->GetOptions ()->IsOptionEqualTo ( "graphics_softwaremouse", 1 );

	if ( showSWMouse ) {

		// Draw the mouse

		if ( !EclGetButton ( "mouse" ) ) {
			EclRegisterButton ( 0, 0, 8, 8, "", "mouse" );
			EclRegisterButtonCallbacks ( "mouse", mousedraw, NULL, NULL, NULL );
		}

		EclButtonBringToFront ( "mouse" );
		Button *mouse = EclGetButton ( "mouse" );
		UplinkAssert (mouse);
		EclDirtyRectangle ( mouse->x, mouse->y, mouse->width, mouse->height );
		EclGetButton ( "mouse" )->x = x + 1;
		EclGetButton ( "mouse" )->y = y + 1;
		EclDirtyRectangle ( mouse->x, mouse->y, mouse->width, mouse->height );

	}

}

int get_mouseX ()
{

	return mouseX;

}

int get_mouseY ()
{

	return mouseY;

}

void resize(int width, int height)
{

	if (height == 0) height = 1;
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/* note we divide our width by our height to get the aspect ratio */
	gluPerspective(45.0, width / height, 1.0, 400.0);

	/* set initial position */
	glTranslatef(0.0, -5.0, -150.0);

	glMatrixMode(GL_MODELVIEW);

}

