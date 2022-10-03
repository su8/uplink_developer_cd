
#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>

#include <GL/glu.h> /*_glu_extention_library_*/


#include "eclipse.h"
#include "vanbakel.h"
#include "gucci.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/opengl_interface.h"
#include "app/serialise.h"

#include "game/game.h"
#include "game/data/data.h"

#include "interface/interface.h"
#include "interface/taskmanager/taskmanager.h"
#include "interface/taskmanager/filecopier.h"

#include "world/world.h"
#include "world/player.h"
#include "world/message.h"
#include "world/computer/databank.h"


FileCopier::FileCopier () : UplinkTask ()
{

	source = NULL;
	sourceindex = -1;
	downloading = FILECOPIER_NOTDOWNLOADING;
	
	numticksrequired = 0;
	progress = 0;
    remotefile = false;

}

FileCopier::~FileCopier ()
{
}

int FileCopier::GetState ()
{

    return downloading;

}

void FileCopier::MoveTo ( int x, int y, int time_ms )
{

	UplinkTask::MoveTo ( x, y, time_ms );

	int pid = SvbLookupPID ( this );

	char stitle    [128];
	char sborder   [128];
	char sprogress [128];
	char sclose    [128];

	sprintf ( stitle, "filecopier_title %d", pid );
	sprintf ( sborder, "filecopier_border %d", pid );
	sprintf ( sprogress, "filecopier_progress %d", pid );
	sprintf ( sclose, "filecopier_close %d", pid );	
		
	EclRegisterMovement ( stitle, x, y, time_ms);
	EclRegisterMovement ( sborder, x + 20, y, time_ms );
	EclRegisterMovement ( sprogress, x + 20, y + 1, time_ms );
	EclRegisterMovement ( sclose, x + 140, y + 1, time_ms );

	EclButtonBringToFront ( stitle );
	EclButtonBringToFront ( sborder );
	EclButtonBringToFront ( sprogress );
	EclButtonBringToFront ( sclose );

}

void FileCopier::SetTarget ( UplinkObject *uo, char *uos, int uoi )
{

	if ( downloading == FILECOPIER_NOTDOWNLOADING ) {

		if ( uo->GetOBJECTID () == OID_DATABANK ) {

			/*
				DataBank selected

				uo  : DataBank object containing Data to be downloaded
				uos : Name of button representing downloadable data
				uoi : memory index of data in databank

				*/

			if ( ((DataBank *) uo)->GetData (uoi) ) {

				source = (DataBank *) uo;
				sourceindex = uoi;

				Data *data = source->GetData (sourceindex);
				UplinkAssert (data);

				numticksrequired = TICKSREQUIRED_COPY * ((float) data->size / (float) game->GetWorld ()->GetPlayer ()->gateway.GetBandwidth ());
				progress = 0;

                remotefile = strstr ( uos, "fileserverscreen" ) ? true : false;

				Button *button = EclGetButton ( uos );
				UplinkAssert (button);

				MoveTo ( button->x, button->y, 1000 );

			}

		}

	}
	else if ( downloading == FILECOPIER_WAITINGFORTARGET ) {

		UplinkAssert ( source );

		// Copy the data
		Data *data = source->GetData (sourceindex);	
        if ( !data ) {
            SvbRemoveTask(this);
            return;
        }

		Data *datacopy = new Data ( data );

		if ( uo->GetOBJECTID () == OID_DATABANK ) {

			// Copying the file into a memory bank
			// Target memory area selected
			// If memory index is -1, look for a valid placement

			DataBank *db = (DataBank *) uo;
			int memoryindex = uoi;

			if ( memoryindex == -1 ) memoryindex = db->FindValidPlacement ( datacopy );

			if ( db->IsValidPlacement ( datacopy, memoryindex ) == 0 ) {
				
				db->PutData ( datacopy, memoryindex );
				downloading = FILECOPIER_FINISHED;

			}

		}
		else if ( uo->GetOBJECTID () == OID_MESSAGE ) {

			// Attaching the file to a message
			
			Message *m = (Message *) uo;
			UplinkAssert ( m );

			m->AttachData ( datacopy );
	
			downloading = FILECOPIER_FINISHED;

		}

	}

}

void FileCopier::BorderDraw ( Button *button, bool highlighted, bool clicked )
{

	glBegin ( GL_QUADS );

		if      ( clicked )		glColor4f ( 0.5, 0.5, 0.6, ALPHA );
		else if ( highlighted ) glColor4f ( 0.2, 0.2, 0.5, ALPHA );
		else					glColor4f ( 0.2, 0.2, 0.4, ALPHA );
		glVertex2i ( button->x, button->y );

		if		( clicked )		glColor4f ( 0.7, 0.7, 0.6, ALPHA );
		else if ( highlighted ) glColor4f ( 0.5, 0.5, 0.6, ALPHA );
		else					glColor4f ( 0.3, 0.3, 0.5, ALPHA );
		glVertex2i ( button->x + button->width, button->y );

		if		( clicked )		glColor4f ( 0.5, 0.5, 0.6, ALPHA );
		else if ( highlighted ) glColor4f ( 0.2, 0.2, 0.5, ALPHA );
		else					glColor4f ( 0.2, 0.2, 0.4, ALPHA );
		glVertex2i ( button->x + button->width, button->y + button->height );

		if		( clicked )		glColor4f ( 0.7, 0.7, 0.6, ALPHA );
		else if ( highlighted ) glColor4f ( 0.5, 0.5, 0.6, ALPHA );
		else					glColor4f ( 0.3, 0.3, 0.5, ALPHA );
		glVertex2i ( button->x, button->y + button->height );

	glEnd ();

	if ( highlighted || clicked ) {

		glColor4f ( 0.3, 0.3, 0.7, 1.0 );
		border_draw ( button );	

	}

}

void FileCopier::ProgressDraw ( Button *button, bool highlighted, bool clicked )
{

	UplinkAssert ( button );

	float scale = (float) button->width / 100.0;
	if ( highlighted ) scale *= 2;

	glBegin ( GL_QUADS );

		glColor4f ( 0.0, 1.5 - scale, scale, 0.6 );
		glVertex3i ( button->x, button->y, 0 );

		glColor4f ( 0.0, 1.5 - scale, scale, 0.6 );
		glVertex3i ( button->x + button->width, button->y, 0 );

		glColor4f ( 0.0, 1.5 - scale, scale, 0.6 );
		glVertex3i ( button->x + button->width, button->y + button->height, 0 );

		glColor4f ( 0.0, 1.5 - scale, scale, 0.6 );
		glVertex3i ( button->x, button->y + button->height, 0 );

	glEnd ();

	int xpos = button->x + 5;
	int ypos = (button->y + button->height / 2) + 3;
		
	glColor4f ( 1.0, 1.0, 1.0, 1.0 );    
    GciDrawText ( xpos, ypos, button->caption, HELVETICA_10 );

}

void FileCopier::CloseClick ( Button *button )
{

	int pid;
	char bname [64];
	sscanf ( button->name, "%s %d", bname, &pid );

	SvbRemoveTask ( pid );

}

void FileCopier::BorderClick ( Button *button )
{

	int pid;
	char bname [64];
	sscanf ( button->name, "%s %d", bname, &pid );

	game->GetInterface ()->GetTaskManager ()->SetTargetProgram ( pid );

}

void FileCopier::Initialise ()
{
}

void FileCopier::Tick ( int n )
{

	if ( IsInterfaceVisible () ) {

		int pid = SvbLookupPID ( this );
		char sprogress [128];
		sprintf ( sprogress, "filecopier_progress %d", pid );

		if ( downloading == FILECOPIER_NOTDOWNLOADING ) {

			// Not downloading - look for a new target

			if ( source ) {

				// A new source file has been specified - start downloading it
				downloading = FILECOPIER_INPROGRESS;

				EclRegisterCaptionChange ( sprogress, "Downloading...", 0 );

			}

		}
		else if ( downloading == FILECOPIER_INPROGRESS ) {

            // Check we are still connected
            if ( remotefile && !game->GetWorld ()->GetPlayer ()->IsConnected () ) {
                SvbRemoveTask (pid);
                return;
            }

			// Download in progress
			Data *data = source->GetData (sourceindex);	
            if ( !data ) {
                SvbRemoveTask(pid);
                return;
            }
            			
			for ( int count = 0; count < n; ++count ) {

				++progress;		
				UplinkAssert ( EclGetButton ( sprogress ) );
				EclGetButton ( sprogress )->width = 120 * ( (float) progress / (float) numticksrequired );
				EclDirtyButton ( sprogress );

				if ( progress >= numticksrequired ) {

					// Finished downloading now
					downloading = FILECOPIER_WAITINGFORTARGET;
					EclRegisterCaptionChange ( sprogress, "Select memory slot" );
					break;

				}

			}

		}
		else if ( downloading == FILECOPIER_WAITINGFORTARGET ) {

			// Finished downloading - waiting for a target memory slot
            // Check we are still connected if this was a remote file

            if ( remotefile && !game->GetWorld ()->GetPlayer ()->IsConnected () ) {
                SvbRemoveTask (pid);
                return;
            }

		}
		else if ( downloading == FILECOPIER_FINISHED ) {

			downloading = FILECOPIER_NOTDOWNLOADING;
			SvbRemoveTask ( this );

		}

	}

}

void FileCopier::CreateInterface ()
{

	if ( !IsInterfaceVisible () ) {

		int pid = SvbLookupPID ( this );

		char stitle    [128];
		char sborder   [128];
		char sprogress [128];
		char sclose    [128];

		sprintf ( stitle, "filecopier_title %d", pid );
		sprintf ( sborder, "filecopier_border %d", pid );
		sprintf ( sprogress, "filecopier_progress %d", pid );
		sprintf ( sclose, "filecopier_close %d", pid );	

		EclRegisterButton ( 265, 432, 20, 15, "", "File Copier", stitle );
		button_assignbitmap ( stitle, "software/cpy.tif" );

		EclRegisterButton ( 285, 432, 120, 15, "", "", sborder );
		EclRegisterButtonCallbacks ( sborder, BorderDraw, BorderClick, button_click, button_highlight );

		EclRegisterButton ( 285, 433, 120, 13, "Select source", "Shows the fraction of the download already completed", sprogress );
		EclRegisterButtonCallbacks ( sprogress, ProgressDraw, BorderClick, button_click, button_highlight );		

		EclRegisterButton ( 405, 433, 13, 13, "", "Close the FileCopier (and stop downloading)", sclose );		
		button_assignbitmaps ( sclose, "close.tif", "close_h.tif", "close_c.tif" );
		EclRegisterButtonCallback ( sclose, CloseClick );
				
	}

}

void FileCopier::RemoveInterface ()
{

	if ( IsInterfaceVisible () ) {

		int pid = SvbLookupPID ( this );

		char stitle    [128];
		char sborder   [128];
		char sprogress [128];
		char sclose    [128];

		sprintf ( stitle, "filecopier_title %d", pid );
		sprintf ( sborder, "filecopier_border %d", pid );
		sprintf ( sprogress, "filecopier_progress %d", pid );
		sprintf ( sclose, "filecopier_close %d", pid );	

		EclRemoveButton ( stitle );
		EclRemoveButton ( sborder );
		EclRemoveButton ( sprogress );
		EclRemoveButton ( sclose );

	}

}

void FileCopier::ShowInterface ()
{

	if ( !IsInterfaceVisible () ) CreateInterface ();

	int pid = SvbLookupPID ( this );

	char stitle    [128];
	char sborder   [128];
	char sprogress [128];
	char sclose    [128];

	sprintf ( stitle, "filecopier_title %d", pid );
	sprintf ( sborder, "filecopier_border %d", pid );
	sprintf ( sprogress, "filecopier_progress %d", pid );
	sprintf ( sclose, "filecopier_close %d", pid );	

	EclButtonBringToFront ( stitle );
	EclButtonBringToFront ( sborder );
	EclButtonBringToFront ( sprogress );
	EclButtonBringToFront ( sclose );

}

bool FileCopier::IsInterfaceVisible ()
{

	int pid = SvbLookupPID ( this );

	char stitle [128];
	sprintf ( stitle, "filecopier_border %d", pid );
	
	return ( EclGetButton (stitle) != NULL );

}
