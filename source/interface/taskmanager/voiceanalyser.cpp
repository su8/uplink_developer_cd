

#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>

#include <GL/glu.h>

#include "vanbakel.h"
#include "soundgarden.h"
#include "redshirt.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/opengl_interface.h"

#include "game/game.h"

#include "world/world.h"
#include "world/computer/computer.h"
#include "world/computer/computerscreen/genericscreen.h"
#include "world/generator/numbergenerator.h"

#include "interface/interface.h"
#include "interface/localinterface/localinterface.h"
#include "interface/remoteinterface/remoteinterface.h"
#include "interface/remoteinterface/voicephonescreen_interface.h"
#include "interface/remoteinterface/voiceanalysisscreen_interface.h"
#include "interface/taskmanager/taskmanager.h"
#include "interface/taskmanager/voiceanalyser.h"


VoiceAnalyser::VoiceAnalyser ()
{

	for ( int i = 0; i < VOICEANALYSER_NUMSAMPLES; ++i )
		sample [i] = 0;

	STATUS = VOICEANALYSER_STATUS_NONE;
	strcpy ( personname, " " );
	timesync = 0;
	animsync = 0;

}

VoiceAnalyser::~VoiceAnalyser ()
{
}

void VoiceAnalyser::CloseClick ( Button *button )
{

	UplinkAssert (button);

	char name [64];
	int pid;
	sscanf ( button->name, "%s %d", name, &pid );

	SvbRemoveTask ( pid );

}

void VoiceAnalyser::TitleClick ( Button *button )
{

	UplinkAssert (button);

	char name [64];
	int pid;
	sscanf ( button->name, "%s %d", name, &pid );

	VoiceAnalyser *thistask = (VoiceAnalyser *) SvbGetTask ( pid );

	thistask->followmouse = true;
	game->GetInterface ()->GetTaskManager ()->SetTargetProgram ( pid );

}

void VoiceAnalyser::PlayDraw ( Button *button, bool highlighted, bool clicked )
{

	UplinkAssert (button);

	char name [64];
	int pid;
	sscanf ( button->name, "%s %d", name, &pid );

	VoiceAnalyser *thistask = (VoiceAnalyser *) SvbGetTask ( pid );

	if ( thistask->STATUS == VOICEANALYSER_STATUS_READY )
		imagebutton_draw ( button, highlighted, clicked );

}

void VoiceAnalyser::PlayClick ( Button *button )
{

	UplinkAssert (button);

	char name [64];
	int pid;
	sscanf ( button->name, "%s %d", name, &pid );

	VoiceAnalyser *thistask = (VoiceAnalyser *) SvbGetTask ( pid );

	if ( thistask->STATUS == VOICEANALYSER_STATUS_READY ) {

#ifndef DEMOGAME

        Person *person = game->GetWorld ()->GetPerson ( thistask->personname );
        UplinkAssert (person);
        char filename [256];
        sprintf ( filename, "sounds/analyser/verifyme%d.wav", person->voiceindex );
		SgPlaySound ( RsArchiveFileOpen ( filename ), filename, true );

#endif
        
        thistask->STATUS = VOICEANALYSER_STATUS_REPRODUCING;
		thistask->timesync = EclGetAccurateTime () + 8000;

		char textbutton [64];
		sprintf ( textbutton, "analyser_text %d", pid );
		EclRegisterCaptionChange ( textbutton, "Reproducing voice sample..." );

		// IF we're looking at a voice analysis screen,
		// input this voice

		if ( game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ()->ScreenID () == SCREEN_VOICEANALYSIS ) {
			
			VoiceAnalysisScreenInterface *vas = (VoiceAnalysisScreenInterface *) game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ();
			UplinkAssert (vas);

			vas->StartVoicePlayback ();

		}

	}

}

void VoiceAnalyser::PlayMouseMove ( Button *button )
{

	UplinkAssert (button);

	char name [64];
	int pid;
	sscanf ( button->name, "%s %d", name, &pid );

	VoiceAnalyser *thistask = (VoiceAnalyser *) SvbGetTask ( pid );

	if ( thistask->STATUS == VOICEANALYSER_STATUS_READY )
		button_highlight ( button );

}

void VoiceAnalyser::PlayMouseDown ( Button *button )
{

	UplinkAssert (button);

	char name [64];
	int pid;
	sscanf ( button->name, "%s %d", name, &pid );

	VoiceAnalyser *thistask = (VoiceAnalyser *) SvbGetTask ( pid );

	if ( thistask->STATUS == VOICEANALYSER_STATUS_READY )
		button_click ( button );

}

void VoiceAnalyser::DrawAnalysis ( Button *button, bool highlighted, bool clicked )
{

	UplinkAssert (button);

	char name [64];
	int pid;
	sscanf ( button->name, "%s %d", name, &pid );

	VoiceAnalyser *thistask = (VoiceAnalyser *) SvbGetTask ( pid );

	clear_draw ( button->x, button->y, button->width, button->height );

	glBegin ( GL_LINE_STRIP );

	for ( int i = 0; i < VOICEANALYSER_NUMSAMPLES; ++i ) {

		int x = button->x + i * (button->width/VOICEANALYSER_NUMSAMPLES);
		int y = (button->y + button->height) - thistask->sample [i] - 1;
		float r = 0.7 - ((float) thistask->sample[i] / 40.0);		
		float b = (float) thistask->sample[i] / 40.0;

		glColor4f ( r, 0.1, b, 1.0 );
		glVertex2i ( x, y );

	}

	glEnd ();

	glColor4f ( 1.0, 1.0, 1.0, 1.0 );
	border_draw ( button );

}

void VoiceAnalyser::MainTextDraw ( Button *button, bool highlighted, bool clicked )
{

	glBegin ( GL_QUADS );		
		glColor3ub ( 8, 20, 0 );		glVertex2i ( button->x, button->y + button->height );
		glColor3ub ( 8, 20, 124 );		glVertex2i ( button->x, button->y );
		glColor3ub ( 8, 20, 0 );		glVertex2i ( button->x + button->width, button->y );
		glColor3ub ( 8, 20, 124 );		glVertex2i ( button->x + button->width, button->y + button->height );
	glEnd ();

	glColor3ub ( 81, 138, 215 );
	border_draw ( button );

	text_draw ( button, highlighted, clicked );

}


void VoiceAnalyser::Initialise ()
{
}

void VoiceAnalyser::Tick ( int n )
{

	int maxsample = 0;
	
	int pid = SvbLookupPID ( this );
	char textbutton [64];
	sprintf ( textbutton, "analyser_text %d", pid );


	switch ( STATUS ) {

		case VOICEANALYSER_STATUS_WAITING:
		{

			maxsample = 10;

			// Are we looking at a voice phone screen?
			// If so, start recording the voice

			if ( game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ()->ScreenID () == SCREEN_VOICEPHONE ) {

				VoicePhoneScreenInterface *vps = (VoicePhoneScreenInterface *) (game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ());
				UplinkAssert (vps);
				
				if ( vps->STATUS == VOICESTATUS_TALKING ) {
					EclRegisterCaptionChange ( textbutton, "Recording voice pattern..." );
					STATUS = VOICEANALYSER_STATUS_RECORDING;
					strcpy ( personname, vps->GetComputerScreen ()->GetComputer ()->companyname );
				}

			}

		}
		break;
	
		case VOICEANALYSER_STATUS_RECORDING:
		{

			maxsample = 40;

			// We are now recording a voice
			// Until he finishes talking

			if ( game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ()->ScreenID () != SCREEN_VOICEPHONE ) {
			
				EclRegisterCaptionChange ( textbutton, "Awaiting voice input..." );
				STATUS = VOICEANALYSER_STATUS_WAITING;

			}

			VoicePhoneScreenInterface *vps = (VoicePhoneScreenInterface *) (game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ());
			UplinkAssert (vps);

			if ( vps->STATUS == VOICESTATUS_HUNGUP ) {
				EclRegisterCaptionChange ( textbutton, "Analysing voice recording..." );
				STATUS = VOICEANALYSER_STATUS_ANALYSING;
				timesync = EclGetAccurateTime () + 5000;
			}
			
		}
		break;

		case VOICEANALYSER_STATUS_ANALYSING:
		{

			maxsample = 10;

			if ( timesync > 0 && EclGetAccurateTime () > timesync ) {

				// Finished analysing
				EclRegisterCaptionChange ( textbutton, "Voice analysis complete.\nPlayback ready." );
				STATUS = VOICEANALYSER_STATUS_READY;
				timesync = 0;

				char play [64];
				sprintf ( play, "analyser_play %d", pid );				
				EclDirtyButton ( play );

			}

		}
		break;

		case VOICEANALYSER_STATUS_READY:
		{

			maxsample = 10;

		}
		break;

		case VOICEANALYSER_STATUS_REPRODUCING:
		{

			// We are currently playing back
			maxsample = 40;

			if ( timesync > 0 && EclGetAccurateTime () > timesync ) {

				// Finished playing back

				EclRegisterCaptionChange ( textbutton, "Finished playing back.\nPress play to repeat." );
				STATUS = VOICEANALYSER_STATUS_READY;
				timesync = 0;

				char play [64];
				sprintf ( play, "analyser_play %d", pid );				
				EclDirtyButton ( play );

				// IF we're looking at a voice analyser screen,
				// Tell it that we've finished playing our sample

				if ( game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ()->ScreenID () == SCREEN_VOICEANALYSIS ) {
					
					VoiceAnalysisScreenInterface *vas = (VoiceAnalysisScreenInterface *) game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ();
					UplinkAssert (vas);

					vas->FinishVoicePlayback ( personname );

				}

			}

		}

	}

	// ==================================================================================
	// Update the voice analyser image
	//

	if ( EclGetAccurateTime () > animsync + 75 ) {

		for ( int i = 0; i < VOICEANALYSER_NUMSAMPLES; ++i ) {

			sample [i] += NumberGenerator::RandomNumber ( maxsample / 2 ) - ( maxsample / 4 );
			if ( sample [i] < 0 ) sample [i] = 0;
			if ( sample [i] > maxsample ) sample [i] = maxsample;

		}
		
		char analyser [64];
		sprintf ( analyser, "analyser_analyser %d", pid );
		EclDirtyButton ( analyser );

		animsync = EclGetAccurateTime ();

	}

	//	
	// ==================================================================================

}

void VoiceAnalyser::MoveTo ( int x, int y, int time_ms )
{

	if ( IsInterfaceVisible () ) {

		int pid = SvbLookupPID ( this );

		char title [64];
		char close [64];
		char text [64];
		char analyser [64];
		char play [64];

		sprintf ( title, "analyser_title %d", pid );
		sprintf ( close, "analyser_close %d", pid );
		sprintf ( text, "analyser_text %d", pid );
		sprintf ( analyser, "analyser_analyser %d", pid );
		sprintf ( play, "analyser_play %d", pid );

		EclRegisterMovement ( title, x + 1, y + 1, time_ms );
		EclRegisterMovement ( close, x + 276, y + 1, time_ms );
		EclRegisterMovement ( text, x + 1, y + 13, time_ms );
		EclRegisterMovement ( analyser, x + 188, y + 13, time_ms );
		EclRegisterMovement ( play, x + 170, y + 45, time_ms );

		EclButtonBringToFront ( title );
		EclButtonBringToFront ( close );
		EclButtonBringToFront ( text );
		EclButtonBringToFront ( analyser );
		EclButtonBringToFront ( play );

	}

}

void VoiceAnalyser::CreateInterface ()
{

	if ( !IsInterfaceVisible () ) {

		int pid = SvbLookupPID ( this );

		char title [64];
		char close [64];
		char text [64];
		char analyser [64];
		char play [64];

		sprintf ( title, "analyser_title %d", pid );
		sprintf ( close, "analyser_close %d", pid );
		sprintf ( text, "analyser_text %d", pid );
		sprintf ( analyser, "analyser_analyser %d", pid );
		sprintf ( play, "analyser_play %d", pid );

		EclRegisterButton ( 200, 400, 275, 13, "Voice Analyser", "Click to move this application", title );
		EclRegisterButtonCallback ( title, TitleClick );
		
		EclRegisterButton ( 475, 400, 13, 13, "Close", "Close the analyser software", close );
		button_assignbitmaps ( close, "close.tif", "close_h.tif", "close_c.tif" );
		EclRegisterButtonCallback ( close, CloseClick );
		
		EclRegisterButton ( 200, 413, 188, 50, "Awaiting voice input...", "", text );
		EclRegisterButtonCallbacks ( text, MainTextDraw, NULL, NULL, NULL );

		EclRegisterButton ( 388, 413, 100, 50, " ", " ", analyser );
		EclRegisterButtonCallbacks ( analyser, DrawAnalysis, NULL, NULL, NULL );

		EclRegisterButton ( 370, 445, 15, 15, " ", "Play the analysed voice recording", play );
		button_assignbitmaps ( play, "software/play.tif", "software/play_h.tif", "software/play_c.tif" );
		EclRegisterButtonCallbacks ( play, PlayDraw, PlayClick, PlayMouseDown, PlayMouseMove );

		STATUS = VOICEANALYSER_STATUS_WAITING;

	}

}

void VoiceAnalyser::RemoveInterface ()
{
	
	if ( IsInterfaceVisible () ) {

		int pid = SvbLookupPID ( this );

		char title [64];
		char close [64];
		char text [64];
		char analyser [64];
		char play [64];

		sprintf ( title, "analyser_title %d", pid );
		sprintf ( close, "analyser_close %d", pid );
		sprintf ( text, "analyser_text %d", pid );
		sprintf ( analyser, "analyser_analyser %d", pid );
		sprintf ( play, "analyser_play %d", pid );

		EclRemoveButton ( title );
		EclRemoveButton ( close );
		EclRemoveButton ( text );
		EclRemoveButton ( analyser );
		EclRemoveButton ( play );

	}

}

void VoiceAnalyser::ShowInterface ()
{

	if ( IsInterfaceVisible () ) {

		int pid = SvbLookupPID ( this );

		char title [64];
		char close [64];
		char text [64];
		char analyser [64];
		char play [64];

		sprintf ( title, "analyser_title %d", pid );
		sprintf ( close, "analyser_close %d", pid );
		sprintf ( text, "analyser_text %d", pid );
		sprintf ( analyser, "analyser_analyser %d", pid );
		sprintf ( play, "analyser_play %d", pid );

		EclButtonBringToFront ( title );
		EclButtonBringToFront ( close );
		EclButtonBringToFront ( text );
		EclButtonBringToFront ( analyser );
		EclButtonBringToFront ( play );

	}

}

bool VoiceAnalyser::IsInterfaceVisible ()
{

	int pid = SvbLookupPID ( this );
	char title [64];
	sprintf ( title, "analyser_title %d", pid );

	return ( EclGetButton ( title ) != NULL );

}

