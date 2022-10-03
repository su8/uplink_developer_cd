// MissionInterface.cpp: implementation of the MissionInterface class.
//
//////////////////////////////////////////////////////////////////////

#ifdef WIN32
#include <strstrea.h>
#else
#include <strstream.h>
#endif

#include <stdio.h>

#include "eclipse.h"
#include "gucci.h"
#include "soundgarden.h"
#include "redshirt.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/opengl_interface.h"

#include "game/game.h"

#include "options/options.h"

#include "interface/interface.h"
#include "interface/localinterface/localinterface.h"
#include "interface/localinterface/memory_interface.h"

#include "world/world.h"
#include "world/player.h"
#include "world/company/mission.h"
#include "world/computer/computer.h"
#include "world/generator/missiongenerator.h"

#include "interface/localinterface/mission_interface.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
	
MissionInterface::MissionInterface ()
{
	
	index = -1;
	timesync = 0;
	mission = NULL;

}

MissionInterface::~MissionInterface ()
{
}

void MissionInterface::TitleClick ( Button *button )
{

	game->GetInterface ()->GetLocalInterface ()->RunScreen ( SCREEN_NONE );

}

void MissionInterface::AbandonClick ( Button *button )
{

	UplinkAssert (button);

	if ( strcmp(button->caption, "Abandon") == 0 ) {
		button->SetCaption( "! ABANDON !");

	}
	else {

		// Get the mission that this interface is looking at

		MissionInterface *mi = (MissionInterface *) game->GetInterface ()->GetLocalInterface ()->GetInterfaceScreen ();
		UplinkAssert (mi);
		UplinkAssert (mi->mission);

		// Get the mission at the supposed indexd

		Mission *realmission = game->GetWorld ()->GetPlayer ()->missions.GetData ( mi->index );

		// Ensure they are the same
		// ie Player has not lost this mission

		if ( mi->mission == realmission) {

			MissionGenerator::MissionFailed ( mi->mission, game->GetWorld ()->GetPlayer (), "You abandoned the mission." );

			// Remove the mission
			if ( mi->mission ) delete mi->mission;
			game->GetWorld ()->GetPlayer ()->missions.RemoveData ( mi->index );
			
		}

		// Get rid of this screen
		game->GetInterface ()->GetLocalInterface ()->RunScreen ( SCREEN_NONE );

	}

}

void MissionInterface::ReplyClick ( Button *button )
{

	// Get the mission that this interface is looking at

	MissionInterface *mi = (MissionInterface *) game->GetInterface ()->GetLocalInterface ()->GetInterfaceScreen ();
	UplinkAssert (mi);
	UplinkAssert (mi->mission);

	// Get the mission at the supposed indexd

	Mission *realmission = game->GetWorld ()->GetPlayer ()->missions.GetData ( mi->index );

	// Ensure they are the same
	// ie Player has not lost this mission

	if ( mi->mission != realmission) 
		game->GetInterface ()->GetLocalInterface ()->RunScreen ( SCREEN_NONE );


	char contact [SIZE_PERSON_NAME];
	strcpy ( contact, mi->mission->contact );

	ostrstream body;
	body << "I have completed the following mission:\n"
		 << mi->mission->description << "\n\n"
		 << "Please credit my account with the remaining payment ASAP.\n"
		 << "Additional requested data:\n" 
		 << '\x0';

	game->GetInterface ()->GetLocalInterface ()->RunScreen ( SCREEN_SENDMAIL );
	game->GetInterface ()->GetLocalInterface ()->Update ();

	EclRegisterCaptionChange ( "sendmail_to", contact, 1 );						// Should occur instantly
	EclRegisterCaptionChange ( "sendmail_subject", "Mission completed" );
	EclRegisterCaptionChange ( "sendmail_body box", body.str (), 1000 );

	delete [] body.str ();

}

void MissionInterface::SetMission ( int newindex )
{

	index = newindex;
	mission = game->GetWorld ()->GetPlayer ()->missions.GetData ( index );
	UplinkAssert (mission);

}

void MissionInterface::Create ()
{

	UplinkAssert ( mission );

	if ( !IsVisible () ) {

		int screenw = app->GetOptions ()->GetOptionValue ("graphics_screenwidth");
		int screenh = app->GetOptions ()->GetOptionValue ("graphics_screenheight");
		int paneltop = 100.0 * ( (screenw * PANELSIZE) / 188.0 ) + 30;
		int panelwidth = screenw * PANELSIZE;
		
		EclRegisterButton ( screenw - panelwidth - 2, paneltop, panelwidth, 15, "MISSION", "Remove the mission screen", "mission_title" );
		EclRegisterButtonCallback ( "mission_title", TitleClick );

		char employer [SIZE_MISSION_EMPLOYER + 8];
		char payment [32];
		char description [SIZE_MISSION_DESCRIPTION + 16];		
		ostrstream details;

		sprintf ( employer, "Employer : %s", mission->employer );
		sprintf ( payment, "Payment : %d credits", mission->payment );
		sprintf ( description, "%s", mission->description );
		details << mission->GetFullDetails ();

		// Concatenate any links onto the end of the mission

		if ( mission->links.Size () > 0 ) {
			
			details << "\n\nLinks included : \n";

			for ( int i = 0; i < mission->links.Size (); ++i ) {
				
				VLocation *vl = game->GetWorld ()->GetVLocation ( mission->links.GetData (i) );				

				if ( !vl ) {

					details << "- " << mission->links.GetData (i) << "(Invalid)\n";

				}
				else {

					Computer *computer = game->GetWorld ()->GetComputer ( vl->computer );

					if ( !computer )
						details << "- " << mission->links.GetData (i) << "(Invalid)\n";

					else 
						details << "- " << computer->name << "\n";					

				}

			}

		}

		// Concatenate any codes onto the end of the mission

		if ( mission->codes.Size () > 0 ) {
			
			DArray <char *> *darray = mission->codes.ConvertToDArray ();
			DArray <char *> *darray_index = mission->codes.ConvertIndexToDArray ();

			details << "\n\nCodes included : \n";

			for ( int i = 0; i < darray->Size (); ++i ) {
				
				UplinkAssert ( darray->ValidIndex ( i ) );

				VLocation *vl = game->GetWorld ()->GetVLocation ( darray_index->GetData (i) );				

				char thislink [64];

				if ( !vl ) {
					
					details << "[" << mission->links.GetData (i) << "(Invalid)]\n";					

				}
				else {

					Computer *computer = game->GetWorld ()->GetComputer ( vl->computer );

					if ( !computer ) 
						details << "[" << mission->links.GetData (i) << "(Invalid)]\n";					
					
					else 
						details << "[" << computer->name << "]\n";
											
				}

				details << "- " << darray->GetData (i) << "\n";

			}

            delete darray;
            delete darray_index;

		}

		details << '\x0';

		EclRegisterButton ( screenw - panelwidth - 2, paneltop + 20, panelwidth, 15, "", "mission_employer" );
		EclRegisterButton ( screenw - panelwidth - 2, paneltop + 40, panelwidth, 15, "", "mission_payment" );
		EclRegisterButton ( screenw - panelwidth - 2, paneltop + 60, panelwidth, 30, "", "mission_description" );		

		EclRegisterButtonCallbacks ( "mission_employer",    textbox_draw, NULL, NULL, NULL );
		EclRegisterButtonCallbacks ( "mission_payment",     textbox_draw, NULL, NULL, NULL );
		EclRegisterButtonCallbacks ( "mission_description", textbox_draw, NULL, NULL, NULL );		

		EclRegisterCaptionChange ( "mission_employer", employer, 100 );
		EclRegisterCaptionChange ( "mission_payment", payment, 100 );
		EclRegisterCaptionChange ( "mission_description", description, 200 );

		int boxheight = screenh - (paneltop + 95) - 70;
		create_stextbox ( screenw - panelwidth - 2, paneltop + 95, panelwidth, boxheight, details.str (), "mission_details" );

		int x1 = screenw - panelwidth - 2;
		int x2 = x1 + panelwidth / 3 + 2;
		int x3 = x2 + panelwidth / 3 + 2;

		EclRegisterButton ( x1, paneltop + 95 + boxheight + 5, panelwidth / 3 - 2, 15, "Close", "Close this screen", "mission_close" );
		EclRegisterButtonCallback ( "mission_close", TitleClick );

		EclRegisterButton ( x2, paneltop + 95 + boxheight + 5, panelwidth / 3 - 2, 15, "Reply", "Send a mail to the employer", "mission_reply" );
		EclRegisterButtonCallback ( "mission_reply", ReplyClick );

		EclRegisterButton ( x3, paneltop + 95 + boxheight + 5, panelwidth / 3 - 2, 15, "Abandon", "Abandon this mission", "mission_abandon" );
		EclRegisterButtonCallback ( "mission_abandon", AbandonClick );

		delete [] details.str ();

	}

}

void MissionInterface::Remove ()
{

	if ( IsVisible () ) {

		EclRemoveButton ( "mission_title" );
		EclRemoveButton ( "mission_employer" );
		EclRemoveButton ( "mission_payment" );
		EclRemoveButton ( "mission_description" );

		remove_stextbox ( "mission_details" );

		EclRemoveButton ( "mission_close" );
		EclRemoveButton ( "mission_reply" );
		EclRemoveButton ( "mission_abandon" );

	}

}

void MissionInterface::Update ()
{

	if ( time(NULL) > timesync + 1 ) {

		// Get the mission at the supposed indexd

		Mission *realmission = game->GetWorld ()->GetPlayer ()->missions.GetData ( index );

		// Ensure they are the same
		// ie Player has not lost this mission

		if ( mission != realmission) 
			game->GetInterface ()->GetLocalInterface ()->RunScreen ( SCREEN_NONE );

		timesync = time(NULL);

	}

}

bool MissionInterface::IsVisible ()
{

	return ( EclGetButton ( "mission_title" ) != NULL );

}

int MissionInterface::ScreenID ()
{
	
	return SCREEN_MISSION;

}
