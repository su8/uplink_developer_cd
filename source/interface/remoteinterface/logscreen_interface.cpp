
#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>

#include <GL/glu.h>

#include "eclipse.h"
#include "gucci.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/opengl_interface.h"

#include "options/options.h"

#include "game/game.h"

#include "interface/interface.h"
#include "interface/remoteinterface/remoteinterface.h"
#include "interface/remoteinterface/logscreen_interface.h"
#include "interface/taskmanager/taskmanager.h"

#include "world/world.h"
#include "world/message.h"
#include "world/player.h"
#include "world/computer/computer.h"
#include "world/computer/logbank.h"
#include "world/computer/computerscreen/logscreen.h"


int LogScreenInterface::baseoffset = 0;
int LogScreenInterface::previousnumlogs = 0;


void LogScreenInterface::CloseClick ( Button *button )
{

    LogScreen *ls = (LogScreen *) game->GetInterface ()->GetRemoteInterface ()->GetComputerScreen ();
    UplinkAssert (ls);
    
	if ( ls->nextpage != -1 ) 
        game->GetInterface ()->GetRemoteInterface ()->RunScreen ( ls->nextpage );

}

bool LogScreenInterface::EscapeKeyPressed ()
{

    CloseClick ( NULL );
    return true;
    
}

void LogScreenInterface::LogClick ( Button *button )
{

	UplinkAssert ( button );

	int logindex;
	sscanf ( button->name, "logscreen_log %d", &logindex );
	logindex = baseoffset - logindex;

	LogScreen *ls = (LogScreen *) game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ()->GetComputerScreen ();
	LogBank *logbank = ls->GetTargetLogBank ();

	Computer *comp = game->GetInterface ()->GetRemoteInterface ()->GetComputerScreen ()->GetComputer ();
	UplinkAssert (comp);

	if ( comp->security.IsRunning_Proxy () ) {
		create_msgbox ( "Error", "Denied access by Proxy Server" );
		return;
	}

    if ( logbank->logs.ValidIndex (logindex) )
	    game->GetInterface ()->GetTaskManager ()->SetProgramTarget ( logbank, button->name, logindex );

}

void LogScreenInterface::ScrollUpClick ( Button *button ) 
{

	++baseoffset;	

	LogScreen *ls = (LogScreen *) game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ()->GetComputerScreen ();
	int numlogs = ls->GetTargetLogBank ()->logs.Size ();
		
	if ( baseoffset >= numlogs ) baseoffset = numlogs - 1;

	for ( int i = 0; i < 15; ++i ) {

		char name [128];
		sprintf ( name, "logscreen_log %d", i );
		EclDirtyButton ( name );

	}

}

void LogScreenInterface::ScrollDownClick ( Button *button )
{

	--baseoffset;
	if ( baseoffset < 0 ) baseoffset = 0;

	for ( int i = 0; i < 15; ++i ) {

		char name [128];
		sprintf ( name, "logscreen_log %d", i );
		EclDirtyButton ( name );

	}

}

void LogScreenInterface::LogDraw ( Button *button, bool highlighted, bool clicked )
{

	UplinkAssert ( button );

	int screenheight = app->GetOptions ()->GetOptionValue ( "graphics_screenheight" );
	glScissor ( button->x, screenheight - (button->y + button->height), button->width, button->height );	
	glEnable ( GL_SCISSOR_TEST );
	
	int logindex;
	sscanf ( button->name, "logscreen_log %d", &logindex );
	logindex = baseoffset - logindex;

	LogScreen *ls = (LogScreen *) game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ()->GetComputerScreen ();
	LogBank *logbank = ls->GetTargetLogBank ();

	if ( logbank->logs.ValidIndex (logindex) ) {

		AccessLog *log = logbank->logs.GetData (logindex);

		if ( logindex % 2 == 0 ) {

			glBegin ( GL_QUADS );
				glColor3ub ( 8, 20, 80 );		glVertex2i ( button->x, button->y );
				glColor3ub ( 8, 20, 0 );		glVertex2i ( button->x + button->width, button->y );
				glColor3ub ( 8, 20, 80 );		glVertex2i ( button->x + button->width, button->y + button->height );
				glColor3ub ( 8, 20, 0 );		glVertex2i ( button->x, button->y + button->height );
			glEnd ();

		}
		else {

			glBegin ( GL_QUADS );
				glColor3ub ( 8, 20, 0 );		glVertex2i ( button->x, button->y );
				glColor3ub ( 8, 20, 80 );		glVertex2i ( button->x + button->width, button->y );
				glColor3ub ( 8, 20, 0 );		glVertex2i ( button->x + button->width, button->y + button->height );
				glColor3ub ( 8, 20, 80 );		glVertex2i ( button->x, button->y + button->height );
			glEnd ();

		}

		glColor4f ( 1.0, 1.0, 1.0, 1.0 );
		GciDrawText ( button->x + 10, button->y + 10, log->date.GetShortString () );
		
		char *description = log->GetDescription ();
		GciDrawText ( button->x + 100, button->y + 10, description );
		delete [] description;

		// Draw a bounding box
		if ( highlighted ) {

			glColor4f ( 1.0, 1.0, 1.0, 1.0 );
			border_draw ( button );

		}

	}
	else {

		clear_draw ( button->x, button->y, button->width, button->height );

	}
	
	glDisable ( GL_SCISSOR_TEST );

}

void LogScreenInterface::Create ()
{

	if ( cs ) Create ( cs );
	else printf ( "LogScreenInterface::Create, tried to create when LogScreen==NULL\n" );

}

void LogScreenInterface::Create ( ComputerScreen *newcs )
{

	UplinkAssert ( newcs );
	cs = newcs;

	if ( !IsVisible () ) {

		LogBank *logbank = GetComputerScreen ()->GetTargetLogBank ();		
		UplinkAssert (logbank);

		EclRegisterButton ( 80, 60, 350, 25, GetComputerScreen ()->maintitle, "", "logscreen_maintitle" );
		EclRegisterButtonCallbacks ( "logscreen_maintitle", DrawMainTitle, NULL, NULL, NULL );
		EclRegisterButton ( 80, 80, 350, 20, GetComputerScreen ()->subtitle, "", "logscreen_subtitle" );
		EclRegisterButtonCallbacks ( "logscreen_subtitle", DrawSubTitle, NULL, NULL, NULL );

		EclRegisterButton ( 15, 120, 96, 15, "Date", "", "logscreen_datetitle" );
		EclRegisterButton ( 115, 120, 300, 15, "Action", "", "logscreen_actiontitle" );
		
		// Create the log entries

		for ( int i = 0; i < 15; ++i ) {

			char name [128];
			sprintf ( name, "logscreen_log %d", i );
			EclRegisterButton ( 15, 140 + i * 15, 400, 14, "", "Select this log", name );
			EclRegisterButtonCallbacks ( name, LogDraw, LogClick, button_click, button_highlight );

		}

		if ( logbank->logs.Size () >= 15 ) {

			// Create the scroll bar

			EclRegisterButton ( 420, 140, 15, 15, "^", "Scroll up", "logscreen_scrollup" );
			button_assignbitmaps ( "logscreen_scrollup", "up.tif", "up_h.tif", "up_c.tif" );
			EclRegisterButtonCallback ( "logscreen_scrollup", ScrollUpClick );

			EclRegisterButton ( 420, 157, 15, 13 * 15 - 4, "", "logscreen_scrollbar" );

			EclRegisterButton ( 420, 140 + 14 * 15, 15, 15, "v", "Scroll down", "logscreen_scrolldown" );
			button_assignbitmaps ( "logscreen_scrolldown", "down.tif", "down_h.tif", "down_c.tif" );
			EclRegisterButtonCallback ( "logscreen_scrolldown", ScrollDownClick );

		}

		// Create the close button

		EclRegisterButton ( 421, 121, 13, 13, "", "Close the Log Screen", "logscreen_close" );
		button_assignbitmaps ( "logscreen_close", "close.tif", "close_h.tif", "close_c.tif" );
		EclRegisterButtonCallback ( "logscreen_close", CloseClick );

		// Set the baseoffset to be the last entry in the logs (newest)

		baseoffset = logbank->logs.Size () - 1;

	}

}

void LogScreenInterface::Remove ()
{

	if ( IsVisible () ) {

		EclRemoveButton ( "logscreen_maintitle" );
		EclRemoveButton ( "logscreen_subtitle" );

		EclRemoveButton ( "logscreen_datetitle" );
		EclRemoveButton ( "logscreen_actiontitle" );

		for ( int i = 0; i < 15; ++i ) {

			char name [128];
			sprintf ( name, "logscreen_log %d", i );
			EclRemoveButton ( name );

		}

		EclRemoveButton ( "logscreen_scrollup" );
		EclRemoveButton ( "logscreen_scrollbar" );
		EclRemoveButton ( "logscreen_scrolldown" );

        EclRemoveButton ( "logscreen_close" );

	}

}

bool LogScreenInterface::IsVisible ()
{

	return ( EclGetButton ( "logscreen_maintitle" ) != NULL );

}

void LogScreenInterface::Update ()
{

	LogScreen *ls = (LogScreen *) game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ()->GetComputerScreen ();
	LogBank *logbank = ls->GetTargetLogBank ();

	int newnumlogs = logbank->logs.NumUsed ();

	if ( newnumlogs != previousnumlogs ) {

		for ( int i = 0; i < 15; ++i ) {

			char name [128];
			sprintf ( name, "logscreen_log %d", i );
			EclDirtyButton ( name );

		}

		previousnumlogs = newnumlogs;

	}

}

LogScreen *LogScreenInterface::GetComputerScreen ()
{

	UplinkAssert (cs);
	return (LogScreen *) cs;

}

int LogScreenInterface::ScreenID ()
{

	return SCREEN_LOGSCREEN;

}
