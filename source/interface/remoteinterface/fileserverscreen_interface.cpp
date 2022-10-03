
#ifdef WIN32
#include <windows.h>
#endif

#include <string.h>

#include <GL/gl.h>

#include <GL/glu.h>

#include "eclipse.h"
#include "gucci.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/opengl_interface.h"

#include "game/game.h"

#include "interface/interface.h"
#include "interface/remoteinterface/remoteinterface.h"
#include "interface/remoteinterface/fileserverscreen_interface.h"
#include "interface/taskmanager/taskmanager.h"

#include "world/world.h"
#include "world/message.h"
#include "world/player.h"
#include "world/computer/computer.h"
#include "world/computer/logbank.h"
#include "world/computer/computerscreen/genericscreen.h"


int FileServerScreenInterface::baseoffset = 0;
int FileServerScreenInterface::previousnumfiles = 0;


void FileServerScreenInterface::CloseClick ( Button *button )
{

	UplinkAssert ( button );

	int nextpage;
	sscanf ( button->name, "fileserverscreen_click %d", &nextpage );

	if ( nextpage != -1 ) game->GetInterface ()->GetRemoteInterface ()->RunScreen ( nextpage );

}

bool FileServerScreenInterface::EscapeKeyPressed ()
{

	char name [128];
	sprintf ( name, "fileserverscreen_click %d", GetComputerScreen ()->nextpage );
    Button *button = EclGetButton (name);
    UplinkAssert (button);

    CloseClick (button);
    return true;

}

void FileServerScreenInterface::FileClick ( Button *button )
{

	UplinkAssert ( button );

	int fileindex;
	sscanf ( button->name, "fileserverscreen_file %d", &fileindex );
	fileindex += baseoffset;

	VLocation *vl = game->GetWorld ()->GetVLocation ( game->GetWorld ()->GetPlayer ()->remotehost );
	UplinkAssert ( vl );
	Computer *comp = game->GetWorld ()->GetComputer ( vl->computer );
	UplinkAssert ( comp );

	if ( comp->security.IsRunning_Firewall () ) {
		create_msgbox ( "Error", "Denied access by Firewall" );
		return;
	}

	Data *data = comp->databank.GetDataFile (fileindex);
	int memoryindex = comp->databank.GetMemoryIndex ( fileindex );

	if ( data && memoryindex != -1 ) {

		// Log this access
		char action [64];

		sprintf ( action, "Accessed file %s", data->title );
		AccessLog *log = new AccessLog ();
		log->SetProperties ( &(game->GetWorld ()->date), 
							 game->GetWorld ()->GetPlayer ()->GetConnection ()->GetGhost (), "PLAYER",
							 LOG_NOTSUSPICIOUS, LOG_TYPE_TEXT );
		log->SetData1 ( action );
		
		comp->logbank.AddLog ( log );

		// Target this databank
		game->GetInterface ()->GetTaskManager ()->SetProgramTarget ( &(comp->databank), button->name, memoryindex );

	}
	else {

		// There is nothing here, so target an empty spot in memory
		game->GetInterface ()->GetTaskManager ()->SetProgramTarget ( &(comp->databank), button->name, -1 );

		// Log this access
		char action [64];

		sprintf ( action, "Accessed memory file index %d", fileindex );
		AccessLog *log = new AccessLog ();
		log->SetProperties ( &(game->GetWorld ()->date), 
							 game->GetWorld ()->GetPlayer ()->GetConnection ()->GetGhost (), "PLAYER",
							 LOG_NOTSUSPICIOUS, LOG_TYPE_TEXT );
		log->SetData1 ( action );
		
		comp->logbank.AddLog ( log );

	}

}

void FileServerScreenInterface::ScrollUpClick ( Button *button ) 
{

	--baseoffset;
	if ( baseoffset < 0 ) baseoffset = 0;

	for ( int i = 0; i < 15; ++i ) {

		char name [128];
		sprintf ( name, "fileserverscreen_file %d", i );
		EclDirtyButton ( name );

	}

}

void FileServerScreenInterface::ScrollDownClick ( Button *button )
{

	++baseoffset;

	for ( int i = 0; i < 15; ++i ) {

		char name [128];
		sprintf ( name, "fileserverscreen_file %d", i );
		EclDirtyButton ( name );

	}

}

void FileServerScreenInterface::FileDraw ( Button *button, bool highlighted, bool clicked )
{

	UplinkAssert ( button );

	int fileindex;
	sscanf ( button->name, "fileserverscreen_file %d", &fileindex );
	fileindex += baseoffset;

	VLocation *vl = game->GetWorld ()->GetVLocation ( game->GetWorld ()->GetPlayer ()->remotehost );
	UplinkAssert ( vl );
	Computer *comp = game->GetWorld ()->GetComputer ( vl->computer );
	UplinkAssert ( comp );

	Data *data = comp->databank.GetDataFile (fileindex);

	if ( data ) {

		if ( fileindex % 2 == 0 ) {

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

		GciDrawText ( button->x, button->y + 10, data->title );

		char size [64];
		sprintf ( size, "%d GigaQuads", data->size );
		GciDrawText ( button->x + 150, button->y + 10, size );

		if ( data->encrypted > 0 ) {
			char encrypttext [32];
			sprintf ( encrypttext, "Level %d", data->encrypted );
			GciDrawText ( button->x + 250, button->y + 10, encrypttext );
		}

		if ( data->compressed > 0 ) {
			char compressedtext [32];
			sprintf ( compressedtext, "Level %d", data->compressed );
			GciDrawText ( button->x + 330, button->y + 10, compressedtext );
		}

	}
	else {

		clear_draw ( button->x, button->y, button->width, button->height );

	}

	// Draw a bounding box

	if ( highlighted && fileindex <= comp->databank.GetSize () ) {

		glColor4f ( 1.0, 1.0, 1.0, 1.0 );
		border_draw ( button );

	}
	
}

void FileServerScreenInterface::Create ()
{

	if ( cs ) Create ( cs );
	else printf ( "FileServerScreenInterface::Create, tried to create when GenericScreen==NULL\n" );

}

void FileServerScreenInterface::Create ( ComputerScreen *newcs )
{

	UplinkAssert ( newcs );
	cs = newcs;

	if ( !IsVisible () ) {

		DataBank *db = &(GetComputerScreen ()->GetComputer ()->databank);

		AccessLog *log = new AccessLog ();
		log->SetProperties ( &(game->GetWorld ()->date), 
							 game->GetWorld ()->GetPlayer ()->GetConnection ()->GetGhost (), "PLAYER" );
		log->SetData1 ( "Accessed fileserver" );
		GetComputerScreen ()->GetComputer ()->logbank.AddLog ( log );

		EclRegisterButton ( 80, 60, 350, 25, GetComputerScreen ()->maintitle, "", "fileserverscreen_maintitle" );
		EclRegisterButtonCallbacks ( "fileserverscreen_maintitle", DrawMainTitle, NULL, NULL, NULL );
		EclRegisterButton ( 80, 80, 350, 20, GetComputerScreen ()->subtitle, "", "fileserverscreen_subtitle" );
		EclRegisterButtonCallbacks ( "fileserverscreen_subtitle", DrawSubTitle, NULL, NULL, NULL );

		EclRegisterButton ( 15, 120, 144, 15, "Filename", "", "fileserverscreen_filenametitle" );
		EclRegisterButton ( 162, 120, 96, 15, "Size", "", "fileserverscreen_sizetitle" );
		EclRegisterButton ( 261, 120, 76, 15, "Encryption", "", "fileserverscreen_encryption" );
		EclRegisterButton ( 340, 120, 75, 15, "Compression", "", "fileserverscreen_compression" );


		// Create the file entries

		for ( int i = 0; i < 15; ++i ) {

			char name [128];
			sprintf ( name, "fileserverscreen_file %d", i );
			EclRegisterButton ( 15, 140 + i * 15, 400, 14, "", "Select this file", name );
			EclRegisterButtonCallbacks ( name, FileDraw, FileClick, button_click, button_highlight );

		}

		if ( db->NumDataFiles () >= 14 ) {

			// Create the scroll bar

			EclRegisterButton ( 420, 140, 15, 15, "^", "Scroll up", "fileserverscreen_scrollup" );
			button_assignbitmaps ( "fileserverscreen_scrollup", "up.tif", "up_h.tif", "up_c.tif" );
			EclRegisterButtonCallback ( "fileserverscreen_scrollup", ScrollUpClick );

			EclRegisterButton ( 420, 157, 15, 13 * 15 - 4, "", "fileserverscreen_scrollbar" );

			EclRegisterButton ( 420, 140 + 14 * 15, 15, 15, "v", "Scroll down", "fileserverscreen_scrolldown" );
			button_assignbitmaps ( "fileserverscreen_scrolldown", "down.tif", "down_h.tif", "down_c.tif" );
			EclRegisterButtonCallback ( "fileserverscreen_scrolldown", ScrollDownClick );

		}

		// Create the close button

		char name [128];
		sprintf ( name, "fileserverscreen_click %d", GetComputerScreen ()->nextpage );
		EclRegisterButton ( 421, 121, 13, 13, "", "Close the File Server Screen", name );
		button_assignbitmaps ( name, "close.tif", "close_h.tif", "close_c.tif" );
		EclRegisterButtonCallback ( name, CloseClick );
		

        // Move to the top

        baseoffset = 0;

	}

}

void FileServerScreenInterface::Remove ()
{

	if ( IsVisible () ) {

		EclRemoveButton ( "fileserverscreen_maintitle" );
		EclRemoveButton ( "fileserverscreen_subtitle" );

		EclRemoveButton ( "fileserverscreen_filenametitle" );
		EclRemoveButton ( "fileserverscreen_sizetitle" );
		EclRemoveButton ( "fileserverscreen_encryption" );
		EclRemoveButton ( "fileserverscreen_compression" );

		for ( int i = 0; i < 15; ++i ) {

			char name [128];
			sprintf ( name, "fileserverscreen_file %d", i );
			EclRemoveButton ( name );

		}

		EclRemoveButton ( "fileserverscreen_scrollup" );
		EclRemoveButton ( "fileserverscreen_scrollbar" );
		EclRemoveButton ( "fileserverscreen_scrolldown" );

		char name [128];
		sprintf ( name, "fileserverscreen_click %d", GetComputerScreen ()->nextpage );
		EclRemoveButton ( name );

	}

}

bool FileServerScreenInterface::IsVisible ()
{

	return ( EclGetButton ( "fileserverscreen_maintitle" ) != NULL );

}

void FileServerScreenInterface::Update ()
{

	VLocation *vl = game->GetWorld ()->GetVLocation ( game->GetWorld ()->GetPlayer ()->remotehost );
	UplinkAssert ( vl );
	Computer *comp = game->GetWorld ()->GetComputer ( vl->computer );
	UplinkAssert ( comp );

	int newnumfiles = comp->databank.NumDataFiles ();

	if ( newnumfiles != previousnumfiles ) {

		for ( int i = 0; i < 15; ++i ) {

			char name [128];
			sprintf ( name, "fileserverscreen_file %d", i );
			EclDirtyButton ( name );

		}
		
		previousnumfiles = newnumfiles;

	}

}

GenericScreen *FileServerScreenInterface::GetComputerScreen ()
{

	UplinkAssert (cs);
	return (GenericScreen *) cs;

}

int FileServerScreenInterface::ScreenID ()
{

	return SCREEN_FILESERVERSCREEN;

}
