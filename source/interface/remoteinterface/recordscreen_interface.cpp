
#include <stdio.h>

#include "app/app.h"
#include "app/globals.h"
#include "app/opengl_interface.h"

#include "game/game.h"

#include "world/world.h"
#include "world/player.h"
#include "world/computer/computer.h"
#include "world/computer/recordbank.h"
#include "world/computer/computerscreen/genericscreen.h"

#include "interface/interface.h"
#include "interface/remoteinterface/remoteinterface.h"
#include "interface/remoteinterface/recordscreen_interface.h"


int RecordScreenInterface::recordindex = 0;


void RecordScreenInterface::CloseClick ( Button *button )
{

	UplinkAssert ( button );

	int nextpage;
	sscanf ( button->name, "recordscreen_click %d", &nextpage );

	if ( nextpage != -1 ) game->GetInterface ()->GetRemoteInterface ()->RunScreen ( nextpage );

}

void RecordScreenInterface::CommitClick ( Button *button )
{

	// Go through each button-pair on screen, updating each
	// (Get the title from the left box and the value from the right)

	ComputerScreen *cs = game->GetInterface ()->GetRemoteInterface ()->GetComputerScreen ();
	UplinkAssert (cs);

	Computer *comp = cs->GetComputer ();
	UplinkAssert (comp);

	if ( comp->security.IsRunning_Proxy () ) {
		create_msgbox ( "Error", "Denied access by Proxy Server" );
		return;
	}

	Record *rec = comp->recordbank.GetRecord (recordindex);	
	UplinkAssert (rec);

	int i = 0;

	while ( true ) {
		
		char bname_title [64];
		char bname_value [128];

		sprintf ( bname_title, "recordscreen_title %d", i );
		sprintf ( bname_value, "recordscreen_value %d", i );

		Button *title = EclGetButton ( bname_title );
		Button *value = EclGetButton ( bname_value );

		if ( title && value )						
			rec->ChangeField ( title->caption, value->caption );

		else
			break;

		i++;

	}

	// Log this change

	char logcaption [32];
	sprintf ( logcaption, "Changed Record #%d", recordindex );

	AccessLog *log = new AccessLog ();
	log->SetProperties ( &(game->GetWorld ()->date), 
						 game->GetWorld ()->GetPlayer ()->GetConnection ()->GetGhost (), "PLAYER" );
	log->SetData1 ( logcaption );
	comp->logbank.AddLog ( log );

}

void RecordScreenInterface::ScrollLeftClick ( Button *button )
{

	if ( recordindex > 0 )	CreateRecord ( recordindex - 1 );
		
}

void RecordScreenInterface::ScrollRightClick ( Button *button )
{

	ComputerScreen *cs = game->GetInterface ()->GetRemoteInterface ()->GetComputerScreen ();
	UplinkAssert (cs);
	Record *rec = cs->GetComputer ()->recordbank.GetRecord (recordindex+1);	

	if ( rec ) CreateRecord ( recordindex + 1 );

}

void RecordScreenInterface::Create ( ComputerScreen *newcs )
{

	UplinkAssert ( newcs );
	cs = newcs;

	if ( !IsVisible () ) {

		// Create the titles

		EclRegisterButton ( 80, 60, 350, 25, GetComputerScreen ()->maintitle, "", "recordscreen_maintitle" );
		EclRegisterButtonCallbacks ( "recordscreen_maintitle", DrawMainTitle, NULL, NULL, NULL );
		EclRegisterButton ( 80, 80, 350, 20, GetComputerScreen ()->subtitle, "", "recordscreen_subtitle" );
		EclRegisterButtonCallbacks ( "recordscreen_subtitle", DrawSubTitle, NULL, NULL, NULL );

		// Create record 0

		CreateRecord ( 0 );

		// Create the record scroll buttons

		EclRegisterButton ( 20, 370, 50, 20, "Prev", "Scroll to the previous record", "recordscreen_scrollleft" );		
		EclRegisterButton ( 75, 370, 50, 20, "Next", "Scroll to the next record", "recordscreen_scrollright" );

		EclRegisterButtonCallback ( "recordscreen_scrollleft", ScrollLeftClick );
		EclRegisterButtonCallback ( "recordscreen_scrollright", ScrollRightClick );

		// Create the control buttons

		EclRegisterButton ( 230, 370, 80, 20, "Commit", "Commit the changes you have made", "recordscreen_commit" );
		EclRegisterButtonCallback ( "recordscreen_commit", CommitClick );

		char name [128];
		sprintf ( name, "recordscreen_click %d", GetComputerScreen ()->nextpage );
		EclRegisterButton ( 320, 370, 80, 20, "Close", "Close the records screen", name );
		EclRegisterButtonCallback ( name, CloseClick );

		// Log this access

		AccessLog *log = new AccessLog ();
		log->SetProperties ( &(game->GetWorld ()->date), 
							 game->GetWorld ()->GetPlayer ()->GetConnection ()->GetGhost (), "PLAYER" );
		log->SetData1 ( "Accessed logs" );
		GetComputerScreen ()->GetComputer ()->logbank.AddLog ( log );

	}

}

void RecordScreenInterface::CreateRecord ( int index )
{

	// First remove all the old boxes

	int i = 0;

	while ( true ) {

		char bname_title [64];
		char bname_value [128];

		sprintf ( bname_title, "recordscreen_title %d", i );
		sprintf ( bname_value, "recordscreen_value %d", i );

		if ( EclGetButton ( bname_title ) && EclGetButton ( bname_value ) ) {

			EclRemoveButton ( bname_title );
			EclRemoveButton ( bname_value );

			++i;

		}
		else 

			break;

	}

	// Create the new record

	recordindex = index;

	ComputerScreen *cs = game->GetInterface ()->GetRemoteInterface ()->GetComputerScreen ();
	UplinkAssert (cs);
	Record *rec = cs->GetComputer ()->recordbank.GetRecord (recordindex);		

	if ( rec ) {

		DArray <char *> *field_titles = rec->fields.ConvertIndexToDArray ();
		DArray <char *> *field_values = rec->fields.ConvertToDArray ();

		int currentY = 150;

		for ( int j = 0; j < field_titles->Size (); ++j ) {

			UplinkAssert ( field_titles->ValidIndex (j) );
			UplinkAssert ( field_values->ValidIndex (j) );
		
			char bname_title [64];
			char bname_value [128];

			sprintf ( bname_title, "recordscreen_title %d", j );
			sprintf ( bname_value, "recordscreen_value %d", j );

			// Count the number of newlines
			int numnewlines = 1;			// (the last one)
			for ( char *p = field_values->GetData (j); *p != 0; ++p )
				if ( *p == '\n' )
					++numnewlines;

			int buttonheight = 15 * numnewlines;

			EclRegisterButton ( 20, currentY, 120, buttonheight, field_titles->GetData (j), "", bname_title );
			EclRegisterButton ( 140, currentY, 300, buttonheight, field_values->GetData (j), "", bname_value );

			EclRegisterButtonCallbacks ( bname_title, textbutton_draw, NULL, NULL, NULL );
			EclRegisterButtonCallbacks ( bname_value, textbutton_draw, NULL, button_click, button_highlight );

			currentY += buttonheight + 5;

		}

		delete field_titles;
		delete field_values;

	}

}

void RecordScreenInterface::Remove ()
{

	if ( IsVisible () ) {

		EclRemoveButton ( "recordscreen_maintitle" );
		EclRemoveButton ( "recordscreen_subtitle" );

		CreateRecord ( -1 );

		EclRemoveButton ( "recordscreen_scrollleft" );
		EclRemoveButton ( "recordscreen_scrollright" );

		EclRemoveButton ( "recordscreen_commit" );

		char name [128];
		sprintf ( name, "recordscreen_click %d", GetComputerScreen ()->nextpage );
		EclRemoveButton ( name );

	}

}

void RecordScreenInterface::Update ()
{
}

bool RecordScreenInterface::IsVisible ()
{

	return ( EclGetButton ( "recordscreen_maintitle" ) != 0 );

}

int  RecordScreenInterface::ScreenID ()
{

	return SCREEN_RECORDSCREEN;

}

GenericScreen *RecordScreenInterface::GetComputerScreen ()
{

	UplinkAssert (cs);
	return (GenericScreen *) cs;

}
