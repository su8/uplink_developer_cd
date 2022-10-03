// LocalInterfaceScreen.cpp: implementation of the LocalInterfaceScreen class.
//
//////////////////////////////////////////////////////////////////////

#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>

#include <GL/glu.h>

#include "gucci.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/miscutils.h"
#include "app/opengl_interface.h"

#include "options/options.h"

#include "game/game.h"

#include "interface/interface.h"
#include "interface/localinterface/localinterface.h"
#include "interface/localinterface/localinterfacescreen.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void LocalInterfaceScreen::BackgroundDraw ( Button *button, bool highlighted, bool clicked )
{
   
	glBegin ( GL_QUADS );		
		SetColour ( "PanelBackgroundA" );		glVertex2i ( button->x, button->y + button->height );
		SetColour ( "PanelBackgroundB" );		glVertex2i ( button->x, button->y );
		SetColour ( "PanelBackgroundA" );		glVertex2i ( button->x + button->width, button->y );
		SetColour ( "PanelBackgroundB" );		glVertex2i ( button->x + button->width, button->y + button->height );
	glEnd ();

	SetColour ( "PanelBorder" );
	border_draw ( button );

}

LocalInterfaceScreen::LocalInterfaceScreen()
{

}

LocalInterfaceScreen::~LocalInterfaceScreen()
{

}

void LocalInterfaceScreen::Create ()
{	

	if ( !IsVisible () ) {

		int screenw = app->GetOptions ()->GetOptionValue ("graphics_screenwidth");
		int screenh = app->GetOptions ()->GetOptionValue ("graphics_screenheight");
		int paneltop = SY(100) + 30;
		int panelwidth = screenw * PANELSIZE;

		EclRegisterButton ( screenw - panelwidth - 3, paneltop, panelwidth, SY(300), "", "", "localint_background" );
		EclRegisterButtonCallbacks ( "localint_background", BackgroundDraw, NULL, NULL, NULL );		

	}

}

void LocalInterfaceScreen::Remove ()
{

	if ( IsVisible () ) {

		EclRemoveButton ( "localint_background" );

	}

}

void LocalInterfaceScreen::Update ()
{

}

bool LocalInterfaceScreen::IsVisible ()
{
	return false;
}

int LocalInterfaceScreen::ScreenID ()
{

	return 0;

}

LocalInterfaceScreen *LocalInterfaceScreen::GetInterfaceScreen ( int screenID )
{
	
	LocalInterfaceScreen *result = game->GetInterface ()->GetLocalInterface ()->GetInterfaceScreen ();
	UplinkAssert ( result );
	UplinkAssert ( result->ScreenID () == screenID );
	return result;
}