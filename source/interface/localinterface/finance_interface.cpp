// FinanceInterface.cpp: implementation of the FinanceInterface class.
//
//////////////////////////////////////////////////////////////////////

#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>

#include <GL/glu.h>

#include "eclipse.h"
#include "soundgarden.h"
#include "redshirt.h"

#include "game/game.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/miscutils.h"
#include "app/opengl_interface.h"

#include "options/options.h"

#include "interface/interface.h"
#include "interface/localinterface/localinterface.h"
#include "interface/localinterface/finance_interface.h"
#include "interface/localinterface/phonedialler.h"
#include "interface/remoteinterface/remoteinterface.h"

#include "world/world.h"
#include "world/vlocation.h"
#include "world/player.h"
#include "world/computer/computer.h"
#include "world/computer/recordbank.h"
#include "world/computer/bankaccount.h"



FinanceInterface::FinanceInterface ()
{

	oldbalance = 0;
	previousnumaccounts = 0;
	lastupdate = 0;

}

FinanceInterface::~FinanceInterface ()
{

}


void FinanceInterface::TitleClick ( Button *button )
{

	game->GetInterface ()->GetLocalInterface ()->RunScreen ( SCREEN_NONE );

}

void FinanceInterface::DrawAccountsTitle ( Button *button, bool highlighted, bool clicked )
{

	text_draw ( button, highlighted, clicked );

	SetColour ( "TitleText" );

	glBegin ( GL_LINES );
		glVertex2i ( button->x, button->y + button->height );
		glVertex2i ( button->x + button->width, button->y + button->height );
	glEnd ();

}

void FinanceInterface::DrawAccountButton ( Button *button, bool highlighted, bool clicked )
{

	UplinkAssert ( button );

	int index;
	sscanf ( button->name, "finance_account %d", &index );
		
	// Draw a gray background if this is the player's current account

	if ( index == game->GetWorld ()->GetPlayer ()->currentaccount ) {
		
		glBegin ( GL_QUADS );
			glColor4f ( 0.6, 0.6, 0.6, ALPHA );		glVertex2i ( button->x, button->y );
			glColor4f ( 0.4, 0.4, 0.4, ALPHA );		glVertex2i ( button->x + button->width, button->y );
			glColor4f ( 0.6, 0.6, 0.6, ALPHA );		glVertex2i ( button->x + button->width, button->y + button->height );
			glColor4f ( 0.4, 0.4, 0.4, ALPHA );		glVertex2i ( button->x, button->y + button->height );
		glEnd ();		

	}
	else {

		clear_draw ( button->x, button->y, button->width, button->height );

	}

	text_draw ( button, highlighted, clicked );

	// Draw a box around the text if highlighted

	if ( highlighted || clicked ) {

		glColor4f ( 1.0, 1.0, 1.0, 1.0 );
		border_draw ( button );	

	}

}

void FinanceInterface::ClickAccountButton ( Button *button )
{


	UplinkAssert ( button );

	int index;
	sscanf ( button->name, "finance_account %d", &index );

	if ( index == game->GetWorld ()->GetPlayer ()->currentaccount ) {

		// Get the account details
	
		char ip [SIZE_VLOCATION_IP];
		char accno [12];
		sscanf ( game->GetWorld ()->GetPlayer ()->accounts.GetData (game->GetWorld ()->GetPlayer ()->currentaccount),
				 "%s %s", ip, accno );

		// Connect to this account computer

		game->GetWorld ()->GetPlayer ()->GetConnection ()->Disconnect ();
		game->GetWorld ()->GetPlayer ()->GetConnection ()->Reset ();

		PhoneDialler pd;
		pd.Create ( 100, 100 );
		pd.DialNumber ( ip );	
		pd.Remove ();
	
		game->GetWorld ()->GetPlayer ()->GetConnection ()->AddVLocation ( ip );
		game->GetWorld ()->GetPlayer ()->GetConnection ()->Connect ();

		game->GetInterface ()->GetRemoteInterface ()->RunNewLocation ();

		// Lookup the user name and security level

		Computer *bank = game->GetWorld ()->GetVLocation ( ip )->GetComputer ();
		UplinkAssert (bank);

		Record *rec = bank->recordbank.GetRecordFromAccountNumber ( accno );
		UplinkAssert (rec);
		
		char *name = rec->GetField ( RECORDBANK_NAME );
		UplinkAssert (name);
	
		char *security_string = rec->GetField ( RECORDBANK_SECURITY );
		UplinkAssert ( security_string );
		int security;
		sscanf ( security_string, "%d", &security );

		// Set security name and level
		// Run the accounts screen

		game->GetInterface ()->GetRemoteInterface ()->SetSecurity ( name, security );
		game->GetInterface ()->GetRemoteInterface ()->RunScreen ( 3 );

	}
	else {

		// Set this as the current account

		int oldaccount = game->GetWorld ()->GetPlayer ()->currentaccount;
		game->GetWorld ()->GetPlayer ()->SetCurrentAccount ( index );

		char oldaccount_buttonname [32];
		sprintf ( oldaccount_buttonname, "finance_account %d", oldaccount );
		EclDirtyButton ( oldaccount_buttonname );

	}

}

void FinanceInterface::Create ()
{

	if ( !IsVisible () ) {

		LocalInterfaceScreen::Create ();

		int screenw = app->GetOptions ()->GetOptionValue ("graphics_screenwidth");
		int paneltop = 100.0 * ( (screenw * PANELSIZE) / 188.0 ) + 30;
		int panelwidth = screenw * PANELSIZE;

		EclRegisterButton ( screenw - panelwidth, paneltop + 3, panelwidth - 7, 15, "FINANCIAL", "Remove the finance screen", "finance_title" );
		EclRegisterButtonCallback ( "finance_title", TitleClick );

		EclRegisterButton ( screenw - panelwidth, paneltop + 50, panelwidth - 7, 15, "", "", "finance_balance" );
		EclRegisterButtonCallbacks ( "finance_balance", text_draw, NULL, NULL, NULL );

		EclRegisterButton ( screenw - panelwidth, paneltop + 80, panelwidth - 7, 15, "AccNo     Balance", "", "finance_acctitle" );
		EclRegisterButtonCallbacks ( "finance_acctitle", DrawAccountsTitle, NULL, NULL, NULL );

		// Create a new button for each open account

		for ( int i = 0; i < game->GetWorld ()->GetPlayer ()->accounts.Size (); ++i ) {

			char *accountdetails = game->GetWorld ()->GetPlayer ()->accounts.GetData (i);
			char ip [SIZE_VLOCATION_IP];
			char accno [16];
			sscanf ( accountdetails, "%s %s", ip, accno );
			
			char accounttext [128];
			sprintf ( accounttext, "Logging on..." );

			char name [32];
			sprintf ( name, "finance_account %d", i );
			EclRegisterButton ( screenw - panelwidth, paneltop + 100 + i * 20, panelwidth - 7, 15, accounttext, "Set this to be your current account", name );
			EclRegisterButtonCallbacks ( name, DrawAccountButton, ClickAccountButton, button_click, button_highlight );

		}

		previousnumaccounts = game->GetWorld ()->GetPlayer ()->accounts.Size ();
		lastupdate = 0;

	}

}

void FinanceInterface::Remove ()
{

	if ( IsVisible () ) {

		LocalInterfaceScreen::Remove ();

		EclRemoveButton ( "finance_title" );
		EclRemoveButton ( "finance_balance" );
		EclRemoveButton ( "finance_acctitle" );

		// Remove each account button

		int i = 0;
		char name [32];
		sprintf ( name, "finance_account %d", i );

		while ( EclGetButton ( name ) ) {

			EclRemoveButton ( name );
			++i;
			sprintf ( name, "finance_account %d", i );

		}

	}

}

void FinanceInterface::Update ()
{

	if ( time(NULL) > lastupdate + 5 && IsVisible () ) {

		// Create/Destroy new account buttons if neccisary

		if ( previousnumaccounts < game->GetWorld ()->GetPlayer ()->accounts.Size () ) {

			int screenw = app->GetOptions ()->GetOptionValue ("graphics_screenwidth");
			int paneltop = 100.0 * ( (screenw * PANELSIZE) / 188.0 ) + 30;
			int panelwidth = screenw * PANELSIZE;

			// Create a new button for each new open account

			for ( int i = previousnumaccounts; i < game->GetWorld ()->GetPlayer ()->accounts.Size (); ++i ) {

				char *accountdetails = game->GetWorld ()->GetPlayer ()->accounts.GetData (i);
				char ip [SIZE_VLOCATION_IP];
				char accno [16];
				sscanf ( accountdetails, "%s %s", ip, accno );
				
				char accounttext [128];
				sprintf ( accounttext, "Logging on..." );

				char name [32];
				sprintf ( name, "finance_account %d", i );
				EclRegisterButton ( screenw - panelwidth, paneltop + 100 + i * 20, panelwidth - 7, 15, accounttext, "Open this account", name );
				EclRegisterButtonCallbacks ( name, DrawAccountButton, ClickAccountButton, button_click, button_highlight );

			}

			previousnumaccounts = game->GetWorld ()->GetPlayer ()->accounts.Size ();

		}
		else if ( previousnumaccounts > game->GetWorld ()->GetPlayer ()->accounts.Size () ) {

			for ( int i = game->GetWorld ()->GetPlayer ()->accounts.Size (); i < previousnumaccounts; ++i ) {

				char name [32];
				sprintf ( name, "finance_account %d", i );
				EclRemoveButton ( name );

			}

			previousnumaccounts = game->GetWorld ()->GetPlayer ()->accounts.Size ();

		}

		
		// Update each account button with the new balance
		// Also work out the new balance (quicker to do it here than to call player->GetBalance )

		int balance = 0;

		for ( int i = 0; i < game->GetWorld ()->GetPlayer ()->accounts.Size (); ++i ) {

			char name [32];
			sprintf ( name, "finance_account %d", i );
			UplinkAssert ( EclGetButton ( name ) );

			// Try to get the old balance

			int oldbalance = -1;

			if ( strcmp ( EclGetButton (name)->caption, "Logging on..." ) != 0 &&
				 strcmp ( EclGetButton (name)->caption, "Failed to log on" ) != 0 ) {

				char accno [16];
				sscanf ( EclGetButton (name)->caption, "%s    %d", accno, &oldbalance );

			}
			
			char ip [SIZE_VLOCATION_IP];
			char accno [16];
			sscanf ( game->GetWorld ()->GetPlayer ()->accounts.GetData (i), "%s %s", ip, accno );

			// Lookup the balance

			BankAccount *ba = BankAccount::GetAccount ( ip, accno );
			int newbalance = ba->balance;
			if ( oldbalance == -1 ) oldbalance = newbalance;
			balance += newbalance;

			char newcaption [128];
			if ( newbalance > oldbalance )
				sprintf ( newcaption, "%s    %dc     (+%dc)", accno, newbalance, newbalance - oldbalance );
			else if ( newbalance < oldbalance )
				sprintf ( newcaption, "%s    %dc     (%dc)", accno, newbalance, newbalance - oldbalance );
			else
				sprintf ( newcaption, "%s    %dc", accno, newbalance );

			EclGetButton ( name )->SetCaption ( newcaption );

			if ( newbalance != oldbalance ) EclDirtyButton ( "localint_background" );

		}


		// Update the overall balance
		
		char caption [128];
		sprintf ( caption, "Balance : %d credits", balance );
		EclGetButton ( "finance_balance" )->SetCaption ( caption );

		lastupdate = time(NULL);

	}

}

bool FinanceInterface::IsVisible ()
{

	return ( EclGetButton ( "finance_title" ) != NULL );

}

int FinanceInterface::ScreenID ()
{
	
	return SCREEN_FINANCE;

}
