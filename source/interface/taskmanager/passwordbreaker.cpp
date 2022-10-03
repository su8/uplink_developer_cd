
#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>

#include <GL/glu.h> /*_glu_extention_library_*/

#include <string.h>
#include <stdio.h>

#include "soundgarden.h"
#include "gucci.h"
#include "redshirt.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/serialise.h"
#include "app/opengl_interface.h"

#include "game/game.h"
#include "game/data/data.h"

#include "interface/interface.h"
#include "interface/taskmanager/taskmanager.h"
#include "interface/taskmanager/passwordbreaker.h"

#include "world/world.h"
#include "world/date.h"
#include "world/player.h"
#include "world/computer/computer.h"
#include "world/computer/recordbank.h"
#include "world/computer/computerscreen/passwordscreen.h"
#include "world/computer/computerscreen/useridscreen.h"
#include "world/generator/numbergenerator.h"


static char alphabet [27] = "abcdefghijklmnopqrstuvwxyz";


PasswordBreaker::PasswordBreaker () : UplinkTask ()
{

	length = difficulty = currentchar = 0;
	caption = password = username = NULL;
	found = NULL;

}

PasswordBreaker::~PasswordBreaker()
{

	if ( caption )  delete [] caption;
	if ( username ) delete [] username;
	if ( password ) delete [] password;
	if ( found )    delete [] found;

}

void PasswordBreaker::Initialise ()
{

	caption = new char [32];
	sprintf ( caption, "Select target" );

}

void PasswordBreaker::SetTarget ( UplinkObject *uo, char *uos, int uoi )
{

	/*

		uo  : Target PasswordScreen or UserIDScreen object
		uos : name of button with password in
		uoi : unused

		*/

	targetobject = uo;
	targetstring = uos;
	targetint    = uoi;

    Computer *comp = NULL;

	if ( uo->GetOBJECTID () == OID_PASSWORDSCREEN ) {

		PasswordScreen *ps = (PasswordScreen *) uo;
	    comp = ps->GetComputer ();
	    UplinkAssert (comp);

		length = strlen ( ps->password );
		difficulty = ps->difficulty;
		
		password = new char [length+1];
		sprintf ( password, "%s", ps->password );

		username = NULL;

	}
	else if ( uo->GetOBJECTID () == OID_USERIDSCREEN ) {

	    UserIDScreen *uid = (UserIDScreen *) uo;
	    comp = uid->GetComputer ();
	    UplinkAssert (comp);

		// If the player has not entered a UserID then do this for admin
		UplinkAssert ( EclGetButton ( "useridscreen_name" ) );
		if ( strcmp ( EclGetButton ( "useridscreen_name" )->caption, "" ) == 0 ) 
			EclGetButton ( "useridscreen_name" )->SetCaption ( "admin" );
				
		char *name = EclGetButton ( "useridscreen_name" )->caption;
		username = new char [strlen(name)+1];
		strcpy ( username, name );

		// Look up the user name in this computer's records
				
		Record *rec = comp->recordbank.GetRecordFromName ( name );

		if ( rec ) {

			UplinkAssert ( rec->GetField ( RECORDBANK_PASSWORD ) );
			UplinkAssert ( rec->GetField ( RECORDBANK_SECURITY ) );

			char *passwd = rec->GetField ( RECORDBANK_PASSWORD );				
			int security;
			sscanf ( rec->GetField ( RECORDBANK_SECURITY ), "%d", &security );

			difficulty = HACKDIFFICULTY_SECURITYMODIFIER [security] * uid->difficulty;

			length = strlen ( passwd );
								
			password = new char [length+1];
			sprintf ( password, "%s", passwd );
						
		}
		else {

			// The user name is not recognised
			SvbRemoveTask ( this );
			return;

		}

	}
	else {

		// Unrecognised object
		return;

	}

	// Set up the password breaker to work with this new data

	caption = new char [length+1];
	found = new bool [length];

	sprintf ( caption, "" );
	for ( int i = 0; i < length; ++i ) {
		strcat ( caption, "x" );
		found [i] = false;
	}

    int pid = SvbLookupPID (this);

	char buttonname [64];
	sprintf ( buttonname, "passwordbreaker %d", pid );
	EclGetButton ( buttonname )->width = length * 26;

	char closename [64];
	sprintf ( closename, "passwordbreaker_close %d", pid );
	EclGetButton ( closename )->x = EclGetButton(buttonname)->x + (length * 26) - 13;
    EclDirtyButton ( closename );

    char fillname [64];
    sprintf ( fillname, "passwordbreaker_fill %d", pid );
    EclGetButton ( fillname )->width = (length * 26) - 14;
    EclDirtyButton ( fillname );

    if ( difficulty == -1 )     numticksrequired = -1;
    else                	    numticksrequired = NumberGenerator::ApplyVariance ( difficulty, HACKDIFFICULTY_VARIANCE * 100 );

	progress = 0;

    UplinkAssert (comp);

	if ( comp->security.IsRunning_Monitor () )
		game->GetWorld ()->GetPlayer ()->GetConnection ()->BeginTrace ();

}

void PasswordBreaker::Tick ( int n )
{

	if ( IsInterfaceVisible () ) {

		if ( length > 0 ) {

			for ( int count = 0; count < n; ++count ) {


				// First check to make sure we are still looking at this password screen
				// i.e the player has not logged out / moved / got past it already

				if ( !EclGetButton ( targetstring ) ) {

					// Target password box has been removed; Terminate this task
					SvbRemoveTask ( this );			

					// WARNING - This instance has been deleted - do not access any more data
					return;

				}
				
				if ( currentchar < length ) {

					// Some of the word has not been calculated
					// Do it now

					// First see if we have calculated the current character

					++progress;

					if ( numticksrequired != -1 && progress >= numticksrequired ) {

						// This char has been discovered
						caption [currentchar] = password [currentchar];
						found [currentchar] = true;
						currentchar++;

						SgPlaySound ( RsArchiveFileOpen ( "sounds/done.wav" ), "sounds/done.wav" );

						// Work out when the next one will be discovered
						//int timetonextletter = NumberGenerator::ApplyVariance ( difficulty, HACKDIFFICULTY_VARIANCE * 100 );
						//nextletter.SetDate ( &(game->GetWorld ()->date) );
						//nextletter.AdvanceSecond ( timetonextletter );

						numticksrequired = NumberGenerator::ApplyVariance ( difficulty, HACKDIFFICULTY_VARIANCE * 100 );
						progress = 0;

					}
					

					// Now refresh the caption with random chars where appropriate
					for ( int i = currentchar; i < length; ++i )
						caption [i] = alphabet [ int ( 26 * ( (float) rand () / (float) RAND_MAX ) ) ];
					
					if ( IsInterfaceVisible () ) {
						char buttonname [64];
						sprintf ( buttonname, "passwordbreaker %d", SvbLookupPID (this) );
						EclGetButton ( buttonname )->SetCaption ( caption );
						GciPostRedisplay ();
					}


				}
				else {

					// The full word has been discovered - enter it into the password screen

					EclRegisterCaptionChange ( targetstring, password );

					// Give the code to the player

					char newcode [64];
                    if ( username ) Computer::GenerateAccessCode( username, password, newcode );
                    else			Computer::GenerateAccessCode( password, newcode );
					game->GetWorld ()->GetPlayer ()->GiveCode ( game->GetWorld ()->GetPlayer ()->remotehost, newcode );				

					// Remove this task

					SvbRemoveTask ( this );
				
					// WARNING - This instance has been deleted - do not access any more data
					return;

				}

			}

		}

	}

}

void PasswordBreaker::PasswordDraw ( Button *button, bool highlighted, bool clicked )
{

    /*

	glBegin ( GL_QUADS );

		if      ( clicked )		glColor4f ( 0.5, 0.5, 1.0, ALPHA );
		else if ( highlighted ) glColor4f ( 0.2, 0.2, 0.9, ALPHA );
		else					glColor4f ( 0.0, 0.0, 0.5, ALPHA );
		glVertex2i ( button->x, button->y );

		if		( clicked )		glColor4f ( 0.7, 0.7, 1.0, ALPHA );
		else if ( highlighted ) glColor4f ( 0.5, 0.5, 1.0, ALPHA );
		else					glColor4f ( 0.0, 0.0, 0.7, ALPHA );
		glVertex2i ( button->x + button->width, button->y );

		if		( clicked )		glColor4f ( 0.5, 0.5, 1.0, ALPHA );
		else if ( highlighted ) glColor4f ( 0.2, 0.2, 0.9, ALPHA );
		else					glColor4f ( 0.0, 0.0, 0.5, ALPHA );
		glVertex2i ( button->x + button->width, button->y + button->height );

		if		( clicked )		glColor4f ( 0.7, 0.7, 1.0, ALPHA );
		else if ( highlighted ) glColor4f ( 0.5, 0.5, 1.0, ALPHA );
		else					glColor4f ( 0.0, 0.0, 0.7, ALPHA );
		glVertex2i ( button->x, button->y + button->height );

	glEnd ();

    */

    BackgroundDraw ( button, highlighted, clicked );

	int xpos = button->x + 10;
	int ypos = button->y + 18;
		
	glColor4f ( 1.0, 1.0, 1.0, 0.8 );    
    
	if ( strcmp ( button->caption, "Select target" ) != 0 ) {

		for ( int i = 0; i < strlen (button->caption); ++i ) {
			char caption [2];
			sprintf ( caption, "%c", button->caption [i] );
			GciDrawText ( xpos + i * 25, ypos, caption, BITMAP_15 );
		}

	}
	else {
		
		GciDrawText ( xpos, ypos, button->caption, HELVETICA_12 );

	}

}

void PasswordBreaker::PasswordClick ( Button *button )
{

	int pid;
	char bname [64];
	sscanf ( button->name, "%s %d", bname, &pid );

	game->GetInterface ()->GetTaskManager ()->SetTargetProgram ( pid );

}

void PasswordBreaker::CloseClick ( Button *button )
{

	int pid;
	char bname [64];
	sscanf ( button->name, "%s %d", bname, &pid );

	SvbRemoveTask ( pid );

}

void PasswordBreaker::BackgroundDraw ( Button *button, bool highlighted, bool clicked )
{

	glBegin ( GL_QUADS );		
		glColor3ub ( 8, 20, 0 );		glVertex2i ( button->x, button->y + button->height );
		glColor3ub ( 8, 20, 124 );		glVertex2i ( button->x, button->y );
		glColor3ub ( 8, 20, 0 );		glVertex2i ( button->x + button->width, button->y );
		glColor3ub ( 8, 20, 124 );		glVertex2i ( button->x + button->width, button->y + button->height );
	glEnd ();

	glColor3ub ( 81, 138, 215 );
	border_draw ( button );
	
}

void PasswordBreaker::CreateInterface ()
{

	if ( !IsInterfaceVisible () ) {

		int pid = SvbLookupPID (this);

        char buttonname [64];
		sprintf ( buttonname, "passwordbreaker %d", pid );
		EclRegisterButton ( 245, 375, 130, 30, caption, "Password Breaker", buttonname );
		EclRegisterButtonCallbacks ( buttonname, PasswordDraw, PasswordClick, button_click, button_highlight );

        char fillname [64];
        sprintf ( fillname, "passwordbreaker_fill %d", pid );
        EclRegisterButton ( 245, 360, 116, 15, "Password Breaker", "", fillname );
        EclRegisterButtonCallbacks ( fillname, button_draw, NULL, NULL, NULL );

		char closename [64];
		sprintf ( closename, "passwordbreaker_close %d", pid );
    	EclRegisterButton ( 362, 361, 13, 13, "X", "Close the password breaker", closename );
		button_assignbitmaps ( closename, "close.tif", "close_h.tif", "close_c.tif" );
		EclRegisterButtonCallback ( closename, CloseClick );

	}

}

void PasswordBreaker::RemoveInterface ()
{

	if ( IsInterfaceVisible () ) {

		int pid = SvbLookupPID ( this );

		char buttonname [64];
		sprintf ( buttonname, "passwordbreaker %d", pid );
		EclRemoveButton ( buttonname );

		char closename [64];
		sprintf ( closename, "passwordbreaker_close %d", pid );
		EclRemoveButton ( closename );

        char fillname [64];
        sprintf ( fillname, "passwordbreaker_fill %d", pid );
        EclRemoveButton ( fillname );

	}

}

void PasswordBreaker::ShowInterface ()
{

	if ( !IsInterfaceVisible () ) CreateInterface ();

	int pid = SvbLookupPID (this);

	char buttonname [64];
    sprintf ( buttonname, "passwordbreaker %d", pid );
	EclButtonBringToFront ( buttonname );

	char closename [64];
	sprintf ( closename, "passwordbreaker_close %d", pid );
	EclButtonBringToFront ( closename );

    char fillname [64];
    sprintf ( fillname, "passwordbreaker_fill %d", pid );
    EclButtonBringToFront ( fillname );

}

bool PasswordBreaker::IsInterfaceVisible ()
{
	
	char buttonname [64];
	sprintf ( buttonname, "passwordbreaker %d", SvbLookupPID (this) );

	return ( EclGetButton ( buttonname ) != NULL );

}

