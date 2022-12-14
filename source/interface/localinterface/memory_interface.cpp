// MemoryInterface.cpp: implementation of the MemoryInterface class.
//
//////////////////////////////////////////////////////////////////////

#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>

#include <GL/glu.h>

#include <stdio.h>

#include "eclipse.h"
#include "soundgarden.h"
#include "redshirt.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/opengl_interface.h"

#include "game/game.h"

#include "options/options.h"

#include "interface/interface.h"
#include "interface/scrollbox.h"
#include "interface/localinterface/localinterface.h"
#include "interface/localinterface/memory_interface.h"
#include "interface/taskmanager/taskmanager.h"

#include "world/world.h"
#include "world/player.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int MemoryInterface::previousnumfiles = 0;
int MemoryInterface::baseoffset = 0;
int MemoryInterface::currentprogramindex = -1;
int MemoryInterface::specialHighlight = -1;


void MemoryInterface::TitleClick ( Button *button )
{

	game->GetInterface ()->GetLocalInterface ()->RunScreen ( SCREEN_NONE );

}

void MemoryInterface::MemoryBlockDraw ( Button *button, bool highlighted, bool clicked )
{

	UplinkAssert ( button );

	int index;
	sscanf ( button->name, "memory_block %d", &index );
	index += baseoffset;

	if ( index < game->GetWorld ()->GetPlayer ()->gateway.databank.GetSize () ) {

		Data *data = game->GetWorld ()->GetPlayer ()->gateway.databank.GetData (index);

		// Write the memory address
		// (With a black background)

		clear_draw ( button->x, button->y, 30, button->height );

		char caption [64];
		sprintf ( caption, "%03d", index );
		glColor4f ( 1.0, 1.0, 1.0, 1.0 );
		GciDrawText ( button->x, button->y + button->height - 1, caption );	

		// Draw a box, colour coded on the data type
		// Set the colour

		if ( data ) {
				
			if ( data->TYPE == DATATYPE_DATA )
				glColor4f ( 0.2, 0.8, 0.2, ALPHA );

			else if ( data->TYPE == DATATYPE_PROGRAM )
				glColor4f ( 0.8, 0.2, 0.2, ALPHA );
				
			else
				glColor4f ( 0.4, 0.4, 0.4, ALPHA );

		}
		else {

			glColor4f ( 0.3, 0.3, 0.5, ALPHA );

		}

		// Draw the background colour of the box
		glBegin ( GL_QUADS );
			glVertex2i ( button->x + 30, button->y );
			glVertex2i ( button->x + 30, button->y + button->height );

			if ( highlighted || ( index == specialHighlight ) )
				glColor4f ( 0.6, 0.6, 0.8, ALPHA );

			else
				glColor4f ( 0.3, 0.3, 0.8, ALPHA );

			glVertex2i ( button->x + button->width, button->y + button->height );
			glVertex2i ( button->x + button->width, button->y );			
		glEnd ();


		// Draw a box if this program is highlighted

		glColor4f ( 1.0, 1.0, 1.0, ALPHA );

		if ( currentprogramindex != -1 && 
			 game->GetWorld ()->GetPlayer ()->gateway.databank.GetDataIndex (index) == currentprogramindex ) {

			glBegin ( GL_LINES );

				glVertex2i ( button->x + 30, button->y );
				glVertex2i ( button->x + 30, button->y + button->height );
				glVertex2i ( button->x + button->width - 1, button->y );			
				glVertex2i ( button->x + button->width - 1, button->y + button->height );
							
				if ( game->GetWorld ()->GetPlayer ()->gateway.databank.GetDataIndex (index-1) != currentprogramindex ) {
					glVertex2i ( button->x + 30, button->y );
					glVertex2i ( button->x + button->width - 1, button->y );
				}

				if ( game->GetWorld ()->GetPlayer ()->gateway.databank.GetDataIndex (index+1) != currentprogramindex ) {
					glVertex2i ( button->x + 30, button->y + button->height - 1 );
					glVertex2i ( button->x + button->width - 1, button->y + button->height - 1 );
				}

			glEnd ();

		}

		// Write the data title and version if there is one

		if ( data && 
			 game->GetWorld ()->GetPlayer ()->gateway.databank.GetDataIndex (index)	!= 
			 game->GetWorld ()->GetPlayer ()->gateway.databank.GetDataIndex (index-1) ) {
								
				GciDrawText ( button->x + 30, button->y + 8, data->title );

				if ( data->TYPE == DATATYPE_PROGRAM ) {
					char version [5];
					sprintf ( version, "v%1.1f", data->version );
					GciDrawText ( button->x + button->width - 23, button->y + 8, version );
				}

		}

	}
	else {				// Index outside memory banks

		clear_draw ( button->x, button->y, button->width, button->height );

		if ( index == game->GetWorld ()->GetPlayer ()->gateway.databank.GetSize () * 5 ) {
			glColor4f ( 1.0, 1.0, 1.0, 1.0 );
			GciDrawText ( button->x, button->y + 8, "Mouse-Button now fucked" );
		}

	}


}

void MemoryInterface::MemoryBlockHighlight ( Button *button )
{

	// Dirty all blocks that shared the old currentprogramindex;

	int screenw = app->GetOptions ()->GetOptionValue ("graphics_screenwidth");
	int screenh = app->GetOptions ()->GetOptionValue ("graphics_screenheight");
	int paneltop = 100.0 * ( (screenw * PANELSIZE) / 188.0 ) + 30;
	int numrows = ((screenh - 50) - (paneltop + 50)) / 10;

	if ( currentprogramindex != -1 ) {
		for ( int i = baseoffset; i < baseoffset + numrows; ++i ) {
			if ( game->GetWorld ()->GetPlayer ()->gateway.databank.GetDataIndex (i) == currentprogramindex ) {
				
				char name [32];
				sprintf ( name, "memory_block %d", i - baseoffset );
				EclDirtyButton ( name );

			}
		}
	}

	// Work out the new current program index

	int index;
	sscanf ( button->name, "memory_block %d", &index );
	index += baseoffset;

	if ( index < game->GetWorld ()->GetPlayer ()->gateway.databank.GetSize () ) {

		currentprogramindex = game->GetWorld ()->GetPlayer ()->gateway.databank.GetDataIndex (index);

	}
	else {
		
		currentprogramindex = -1;

	}

	EclHighlightButton ( button->name );

	// Dirty all other blocks that share this currentprogramindex

	if ( currentprogramindex != -1 ) {
		for ( int i = baseoffset; i < baseoffset + numrows; ++i ) {
			if ( game->GetWorld ()->GetPlayer ()->gateway.databank.GetDataIndex (i) == currentprogramindex ) {
				
				char name [32];
				sprintf ( name, "memory_block %d", i - baseoffset );
				EclDirtyButton ( name );

			}
		}
	}

}

void MemoryInterface::MemoryBlockClick ( Button *button )
{

	DataBank *db = &(game->GetWorld ()->GetPlayer ()->gateway.databank);

	int index;
	sscanf ( button->name, "memory_block %d", &index );
	index += baseoffset;

	game->GetInterface ()->GetTaskManager ()->SetProgramTarget ( db, button->name, index );

}

void MemoryInterface::ScrollChange ( char *scrollname, int newValue )
{

    baseoffset = newValue;

	int screenw = app->GetOptions ()->GetOptionValue ("graphics_screenwidth");
	int screenh = app->GetOptions ()->GetOptionValue ("graphics_screenheight");
	int paneltop = 100.0 * ( (screenw * PANELSIZE) / 188.0 ) + 30;
	int numrows = ((screenh - 50) - (paneltop + 50)) / 10;

	for ( int i = 0; i < numrows; ++i ) {
	
		char name [64];
		sprintf ( name, "memory_block %d", i );
		EclDirtyButton ( name );

	}

}

void MemoryInterface::Create ()
{

	if ( !IsVisible () ) {

		int screenw = app->GetOptions ()->GetOptionValue ("graphics_screenwidth");
		int screenh = app->GetOptions ()->GetOptionValue ("graphics_screenheight");
		int paneltop = 100.0 * ( (screenw * PANELSIZE) / 188.0 ) + 30;
		int panelwidth = screenw * PANELSIZE;

		EclRegisterButton ( screenw - panelwidth - 3, paneltop, panelwidth, 15, "MEMORY BANKS", "Remove the memory screen", "memory_title" );
		EclRegisterButtonCallback ( "memory_title", TitleClick );

		EclRegisterButton ( screenw - panelwidth - 3, paneltop + 20, panelwidth, 15, "", "memory_capacity" );
		EclRegisterButtonCallbacks ( "memory_capacity", textbutton_draw, NULL, NULL, NULL );

		int numrows = ((screenh - 50) - (paneltop + 50)) / 10;

		for ( int i = 0; i < numrows; ++i ) {

			char name [64];
			sprintf ( name, "memory_block %d", i );
			EclRegisterButton ( screenw - panelwidth - 3 + 5, paneltop + 50 + i * 10, panelwidth - 25, 10, "", name );
			EclRegisterButtonCallbacks ( name, MemoryBlockDraw, MemoryBlockClick, button_click, MemoryBlockHighlight );

		}

        int totalmemory = game->GetWorld ()->GetPlayer ()->gateway.databank.GetSize ();
        ScrollBox::CreateScrollBox ( "memory_scroll", screenw - 20, paneltop + 50, 15, numrows * 10, totalmemory, numrows, baseoffset, ScrollChange );

	}

}

void MemoryInterface::Remove ()
{

	if ( IsVisible () ) {

		EclRemoveButton ( "memory_title" );
		EclRemoveButton ( "memory_capacity" );
        ScrollBox::RemoveScrollBox( "memory_scroll" );

		int screenw = app->GetOptions ()->GetOptionValue ("graphics_screenwidth");
		int screenh = app->GetOptions ()->GetOptionValue ("graphics_screenheight");
		int paneltop = 100.0 * ( (screenw * PANELSIZE) / 188.0 ) + 30;
		int numrows = ((screenh - 50) - (paneltop + 50)) / 10;

		for ( int i = 0; i < numrows; ++i ) {

			char name [64];
			sprintf ( name, "memory_block %d", i );

			EclRemoveButton ( name );

		}

	}

}

void MemoryInterface::ForceUpdateAll ()
{

    DataBank *db = &(game->GetWorld ()->GetPlayer ()->gateway.databank);
    previousnumfiles = db->NumDataFiles() + 1;

}

void MemoryInterface::SpecialHighlight ( int memoryIndex )
{

    specialHighlight = memoryIndex;

}

void MemoryInterface::Update ()
{

	if ( IsVisible () ) {

		char memorycapacity [64];
		sprintf ( memorycapacity, "Memory Capacity : %d GigaQuads", game->GetWorld ()->GetPlayer ()->gateway.databank.GetSize () );

		EclGetButton ( "memory_capacity" )->SetCaption ( memorycapacity );

		DataBank *db = &(game->GetWorld ()->GetPlayer ()->gateway.databank);

		if ( previousnumfiles != db->NumDataFiles () ) {

			// Something has changed
			// Invalidate all the buttons

			int screenw = app->GetOptions ()->GetOptionValue ("graphics_screenwidth");
			int screenh = app->GetOptions ()->GetOptionValue ("graphics_screenheight");
			int paneltop = 100.0 * ( (screenw * PANELSIZE) / 188.0 ) + 30;
			int numrows = ((screenh - 50) - (paneltop + 50)) / 10;

            ScrollBox::GetScrollBox( "memory_scroll" )->SetNumItems( db->GetSize() );

			for ( int i = 0; i < numrows; ++i ) {
			
				char name [64];
				sprintf ( name, "memory_block %d", i );
				EclDirtyButton ( name );

			}

			previousnumfiles = db->NumDataFiles ();

		}

	}

}

bool MemoryInterface::IsVisible ()
{

	return ( EclGetButton ( "memory_title" ) != NULL );

}

int MemoryInterface::ScreenID ()
{
	
	return SCREEN_MEMORY;

}
