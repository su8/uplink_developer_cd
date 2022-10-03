// MainMenuScreen.cpp: implementation of the MainMenuScreen class.
//
//////////////////////////////////////////////////////////////////////

#include "eclipse.h"

#include "app/app.h"
#include "app/opengl_interface.h"

#include "options/options.h"

#include "mainmenu/mainmenuscreen.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MainMenuScreen::MainMenuScreen()
{

}

MainMenuScreen::~MainMenuScreen()
{

}

void MainMenuScreen::Create ()
{	
	
	// Background picture

	int screenw = app->GetOptions ()->GetOptionValue ("graphics_screenwidth");
	int screenh = app->GetOptions ()->GetOptionValue ("graphics_screenheight");

	EclRegisterButton ( SX(320) - 170, 75, 425, 60, "", "mainmenu_background" );
	button_assignbitmap ( "mainmenu_background", "mainmenu/uplinklogo.tif" );		
	EclRegisterButtonCallbacks ( "mainmenu_background", imagebutton_draw, NULL, NULL, NULL );

	EclRegisterButton ( SX(320) - 165, 75 + 60, 330, 15, "", "mainmenu_version" );
	button_assignbitmap ( "mainmenu_version", "mainmenu/versionnew.tif" );
	EclRegisterButtonCallbacks ( "mainmenu_version", imagebutton_draw, NULL, NULL, NULL );

}

void MainMenuScreen::Remove ()
{

	EclRemoveButton ( "mainmenu_background" );
	EclRemoveButton ( "mainmenu_version" );

}

bool MainMenuScreen::ReturnKeyPressed ()
{

	return false;

}

void MainMenuScreen::Update ()
{
}

bool MainMenuScreen::IsVisible ()
{
	return false;
}

int MainMenuScreen::ScreenID ()
{

	return 0;

}
