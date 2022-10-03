
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
#include "interface/taskmanager/logdeleter.h"
#include "interface/remoteinterface/remoteinterface.h"

#include "world/world.h"
#include "world/player.h"
#include "world/vlocation.h"
#include "world/computer/computer.h"
#include "world/computer/logbank.h"
#include "world/computer/computerscreen/computerscreen.h"
#include "world/generator/worldgenerator.h"


LogDeleter::LogDeleter () : UplinkTask ()
{

	source = NULL;
	sourceindex = -1;
	status = LOGDELETER_OFF;
	numticksrequired = 0;
	progress = 0;

}

LogDeleter::~LogDeleter ()
{
}

void LogDeleter::MoveTo ( int x, int y, int time_ms )
{

	UplinkTask::MoveTo ( x, y, time_ms );

	int pid = SvbLookupPID ( this );

	char stitle    [128];
	char sborder   [128];
	char sprogress [128];
	char sclose    [128];

	sprintf ( stitle, "logdeleter_title %d", pid );
	sprintf ( sborder, "logdeleter_border %d", pid );
	sprintf ( sprogress, "logdeleter_progress %d", pid );
	sprintf ( sclose, "logdeleter_close %d", pid );	

	EclRegisterMovement ( stitle, x, y, time_ms);
	EclRegisterMovement ( sborder, x + 20, y, time_ms );
	EclRegisterMovement ( sprogress, x + 20, y + 1, time_ms );
	EclRegisterMovement ( sclose, x + 140, y + 1, time_ms );

	EclButtonBringToFront ( stitle );
	EclButtonBringToFront ( sborder );
	EclButtonBringToFront ( sprogress );
	EclButtonBringToFront ( sclose );

}

void LogDeleter::SetTarget ( UplinkObject *uo, char *uos, int uoi )
{

	/*

		uo  : LogBank object containing AccessLog to be deleted
		uos : Name of button representing data
		uoi : index of log in logbank

		*/

	if ( status == LOGDELETER_OFF ) {

		if ( uo->GetOBJECTID () == OID_LOGBANK ) {

			// Logbank selected

			if ( ((LogBank *) uo)->logs.GetData (uoi) ) {

				source = (LogBank *) uo;
				sourceindex = uoi;

				numticksrequired = TICKSREQUIRED_LOGDELETER;
				progress = 0;

				Button *button = EclGetButton ( uos );
				UplinkAssert (button);

				MoveTo ( button->x, button->y, 1000 );

				Computer *comp = game->GetInterface ()->GetRemoteInterface ()->GetComputerScreen ()->GetComputer ();
				UplinkAssert (comp);

				if ( comp->security.IsRunning_Monitor () ) 	
					game->GetWorld ()->GetPlayer ()->GetConnection ()->BeginTrace ();

			}

		}

	}

}

void LogDeleter::BorderDraw ( Button *button, bool highlighted, bool clicked )
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

void LogDeleter::ProgressDraw ( Button *button, bool highlighted, bool clicked )
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

void LogDeleter::CloseClick ( Button *button )
{

	int pid;
	char bname [64];
	sscanf ( button->name, "%s %d", bname, &pid );

	SvbRemoveTask ( pid );

}

void LogDeleter::BorderClick ( Button *button )
{

	int pid;
	char bname [64];
	sscanf ( button->name, "%s %d", bname, &pid );

	game->GetInterface ()->GetTaskManager ()->SetTargetProgram ( pid );

}

void LogDeleter::Initialise ()
{
}

void LogDeleter::Tick ( int n )
{

	if ( IsInterfaceVisible () ) {

		int pid = SvbLookupPID ( this );
		char sprogress [128];
		sprintf ( sprogress, "logdeleter_progress %d", pid );

		for ( int tick = 0; tick < n; ++tick ) {

			if ( status == LOGDELETER_OFF ) {

				// Not downloading - look for a new target

				if ( source ) {

					// A new source file has been specified - start downloading it
					status = LOGDELETER_INPROGRESS;

					EclRegisterCaptionChange ( sprogress, "Removing log...", 0 );

				}

			}
			else if ( status == LOGDELETER_INPROGRESS ) {

				UplinkAssert (source);
				
				// Check log is still there
				if ( !source->logs.ValidIndex (sourceindex) ) {
					status = LOGDELETER_FINISHED;
					break;
				}

                // Check we are still connected
                if ( !game->GetWorld ()->GetPlayer ()->IsConnected () ) {
                    SvbRemoveTask (pid);
                    return;
                }

				// Delete in progress
				++progress;		
				UplinkAssert ( EclGetButton ( sprogress ) );
				EclGetButton ( sprogress )->width = 120 * ( (float) progress / (float) numticksrequired );
				EclDirtyButton ( sprogress );

				if ( progress >= numticksrequired ) {

					// We must keep the date the same (logs are in date order)
					Date logdate;
					logdate.SetDate ( &(source->logs.GetData (sourceindex)->date) );

					if ( version == 1.0 ) {

						// Delete the log	
						delete source->logs.GetData (sourceindex);
						source->logs.RemoveData (sourceindex);

						// Replace it with a "Deleted" marker
						AccessLog *al = new AccessLog ();
						al->SetProperties ( &logdate, "Unknown", " ",
											LOG_NOTSUSPICIOUS, LOG_TYPE_DELETED );
						source->logs.PutData ( al, sourceindex );

						// Finished deleting now
						status = LOGDELETER_FINISHED;
						EclRegisterCaptionChange ( sprogress, "Finished" );

					}
					else if ( version == 2.0 ) {

						// Delete the log	
						delete source->logs.GetData (sourceindex);
						source->logs.RemoveData (sourceindex);

						// Replace it with a "Deleted" marker
						AccessLog *al = new AccessLog ();
						al->SetProperties ( &logdate, " ", " ",
											LOG_NOTSUSPICIOUS, LOG_TYPE_DELETED);
						source->logs.PutData ( al, sourceindex );

						// Finished deleting now
						status = LOGDELETER_FINISHED;
						EclRegisterCaptionChange ( sprogress, "Finished" );

					}
					else if ( version == 3.0 ) {

						// Delete the log
						delete source->logs.GetData (sourceindex);
						source->logs.RemoveData (sourceindex);
						bool overwritten = false;

						// Look for a valid log to put in the blank place
						for ( int i = 0; i < source->logs.Size (); ++i ) {
							if ( source->logs.ValidIndex (i) ) {
								if ( strcmp ( source->logs.GetData (i)->fromip, IP_LOCALHOST ) != 0 ) {
									
									AccessLog *copyme = source->logs.GetData (i);
									// This log was made by someone else
									AccessLog *al = new AccessLog ();
									al->SetProperties ( copyme );
									al->date.SetDate ( &logdate );
									al->SetSuspicious ( LOG_NOTSUSPICIOUS );
									source->logs.PutData ( al, sourceindex );
									overwritten = true;
									break;

								}	
							}
						}

						if ( !overwritten ) {

							// Could not find log to overwrite with - 
							// So make up a new one

							AccessLog *al = new AccessLog ();
							al->SetProperties ( &logdate, WorldGenerator::GetRandomLocation ()->ip, " ",
												LOG_NOTSUSPICIOUS, LOG_TYPE_TEXT );
							al->SetData1 ( "Accessed File" );
							source->logs.PutData ( al, sourceindex );

						}


						// Finished deleting now
						status = LOGDELETER_FINISHED;					

					}
					else if ( version == 4.0 ) {

						// Delete the log
						delete source->logs.GetData (sourceindex);
						source->logs.RemoveData ( sourceindex );

						// Start moving all logs below this one upwards

						status = LOGDELETER_REPLACING;
						EclRegisterCaptionChange ( sprogress, "Shifting logs (0%)" );
						currentreplaceindex = sourceindex;
						currentreplaceprogress = 0;

					}
					else {

						printf ( "LogDeleter WARNING : Unrecognised version number\n" );

					}

					// Dirty the log-screen buttons (hack!)
					for ( int i = 0; i < 15; ++i ) {

						char name [128];
						sprintf ( name, "logscreen_log %d", i );
						EclDirtyButton ( name );

					}

				}
				

			}
			else if ( status == LOGDELETER_REPLACING ) {

                // Check we are still connected
                if ( !game->GetWorld ()->GetPlayer ()->IsConnected () ) {
                    SvbRemoveTask (pid);
                    return;
                }

				// Check to see if an update occurs
				++currentreplaceprogress;

				if ( currentreplaceprogress > 20 ) {

					// Check to see if we have finished

					if ( currentreplaceindex >= source->logs.Size () - 1 ) {

						// Remove the gap from the end permenantly
						source->logs.SetSize ( source->logs.Size () - 1 );
						source->internallogs.SetSize ( source->logs.Size () );

						status = LOGDELETER_FINISHED;											

					}
					else {

						// Shift the next one up into this position

						if ( source->logs.ValidIndex ( currentreplaceindex + 1 ) ) {
							
							AccessLog *al = source->logs.GetData ( currentreplaceindex + 1 );
							source->logs.PutData ( al, currentreplaceindex );
							source->logs.RemoveData ( currentreplaceindex + 1 );

							// Move the backups up as well

							if ( source->internallogs.ValidIndex ( currentreplaceindex + 1 ) ) {

								AccessLog *al2 = source->internallogs.GetData ( currentreplaceindex + 1 );
								source->internallogs.PutData ( al2, currentreplaceindex );
								source->internallogs.RemoveData ( currentreplaceindex + 1 );

							}

							// Dirty the log-screen buttons (hack!)
							for ( int i = 0; i < 15; ++i ) {

								char name [128];
								sprintf ( name, "logscreen_log %d", i );
								EclDirtyButton ( name );

							}

						}

						++currentreplaceindex;
						int percentagedone = 100 * (currentreplaceindex - sourceindex) / (source->logs.Size () - sourceindex);
						char caption [128];
						sprintf ( caption, "Shifting logs...(%d%%)", percentagedone ); 
						EclRegisterCaptionChange ( sprogress, caption, 0 );

					}

					currentreplaceprogress = 0;

				}

			}
			else if ( status == LOGDELETER_FINISHED ) {
				
				source = NULL;
				sourceindex = -1;
				numticksrequired = 0;
				progress = 0;
				EclRegisterCaptionChange ( sprogress, "Finished" );
				status = LOGDELETER_OFF;		

			}

		}

	}

}

void LogDeleter::CreateInterface ()
{

	if ( !IsInterfaceVisible () ) {

		int pid = SvbLookupPID ( this );

		char stitle    [128];
		char sborder   [128];
		char sprogress [128];
		char sclose    [128];
		char tooltip   [128];

		sprintf ( stitle, "logdeleter_title %d", pid );
		sprintf ( sborder, "logdeleter_border %d", pid );
		sprintf ( sprogress, "logdeleter_progress %d", pid );
		sprintf ( sclose, "logdeleter_close %d", pid );	
		sprintf ( tooltip, "Log Deleter v%1.1f", version );

		EclRegisterButton ( 265, 450, 20, 15, "", tooltip, stitle );
		button_assignbitmap ( stitle, "software/log.tif" );

		EclRegisterButton ( 285, 450, 120, 15, "", "", sborder );
		EclRegisterButtonCallbacks ( sborder, BorderDraw, BorderClick, button_click, button_highlight );

		EclRegisterButton ( 285, 450, 120, 13, "Select target log", "Shows the progress of the log delete", sprogress );
		EclRegisterButtonCallbacks ( sprogress, ProgressDraw, BorderClick, button_click, button_highlight );		

		EclRegisterButton ( 405, 450, 13, 13, "", "Close the LogDeleter (and stop deleting)", sclose );		
		button_assignbitmaps ( sclose, "close.tif", "close_h.tif", "close_c.tif" );
		EclRegisterButtonCallback ( sclose, CloseClick );
		
	}

}

void LogDeleter::RemoveInterface ()
{

	if ( IsInterfaceVisible () ) {

		int pid = SvbLookupPID ( this );

		char stitle    [128];
		char sborder   [128];
		char sprogress [128];
		char sclose    [128];

		sprintf ( stitle, "logdeleter_title %d", pid );
		sprintf ( sborder, "logdeleter_border %d", pid );
		sprintf ( sprogress, "logdeleter_progress %d", pid );
		sprintf ( sclose, "logdeleter_close %d", pid );	

		EclRemoveButton ( stitle );
		EclRemoveButton ( sborder );
		EclRemoveButton ( sprogress );
		EclRemoveButton ( sclose );

	}

}

void LogDeleter::ShowInterface ()
{

	if ( !IsInterfaceVisible () ) CreateInterface ();

	int pid = SvbLookupPID ( this );

	char stitle    [128];
	char sborder   [128];
	char sprogress [128];
	char sclose    [128];

	sprintf ( stitle, "logdeleter_title %d", pid );
	sprintf ( sborder, "logdeleter_border %d", pid );
	sprintf ( sprogress, "logdeleter_progress %d", pid );
	sprintf ( sclose, "logdeleter_close %d", pid );	

	EclButtonBringToFront ( stitle );
	EclButtonBringToFront ( sborder );
	EclButtonBringToFront ( sprogress );
	EclButtonBringToFront ( sclose );

}

bool LogDeleter::IsInterfaceVisible ()
{

	int pid = SvbLookupPID ( this );

	char stitle [128];
	sprintf ( stitle, "logdeleter_border %d", pid );
	
	return ( EclGetButton (stitle) != NULL );

}
