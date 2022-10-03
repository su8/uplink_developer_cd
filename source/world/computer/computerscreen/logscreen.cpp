// LogScreen.cpp: implementation of the LogScreen class.
//
//////////////////////////////////////////////////////////////////////

#include "gucci.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/serialise.h"

#include "game/game.h"

#include "interface/interface.h"
#include "interface/remoteinterface/remoteinterface.h"

#include "world/world.h"
#include "world/vlocation.h"
#include "world/player.h"
#include "world/computer/computer.h"
#include "world/computer/bankcomputer.h"
#include "world/computer/computerscreen/logscreen.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LogScreen::LogScreen()
{

	nextpage = -1;
	TARGET = LOGSCREEN_TARGET_UNKNOWN;

}

LogScreen::~LogScreen()
{

}

void LogScreen::SetNextPage ( int newnextpage )
{

	nextpage = newnextpage;

}

void LogScreen::SetTARGET ( int newTARGET )
{

	TARGET = newTARGET;

}

LogBank *LogScreen::GetTargetLogBank ()
{

	LogBank *logbank = NULL;


	switch ( TARGET ) {

		case LOGSCREEN_TARGET_ACCESSLOGS:
		{
			VLocation *vl = game->GetWorld ()->GetVLocation ( game->GetWorld ()->GetPlayer ()->remotehost );
			UplinkAssert ( vl );
			Computer *comp = game->GetWorld ()->GetComputer ( vl->computer );
			UplinkAssert ( comp );
			logbank = &(comp->logbank);
			break;
		}			

		case LOGSCREEN_TARGET_ACCOUNTLOGS:
		{
			
			// Get the bank computer

			VLocation *vl = game->GetWorld ()->GetVLocation ( game->GetWorld ()->GetPlayer ()->remotehost );
			UplinkAssert ( vl );
			Computer *comp = game->GetWorld ()->GetComputer ( vl->computer );
			UplinkAssert ( comp );
			UplinkAssert ( comp->GetOBJECTID () == OID_BANKCOMPUTER );			
			BankComputer *bank = (BankComputer *) comp;

			// Get the account number

			Record *rec = bank->recordbank.GetRecordFromName ( game->GetInterface ()->GetRemoteInterface ()->security_name );
			UplinkAssert (rec);
			char *accno = rec->GetField ( RECORDBANK_ACCNO );
			UplinkAssert (accno);

			// Now look up the account based on that account number

			BankAccount *account = bank->accounts.GetData ( accno );
			UplinkAssert (account);
			
			logbank = &(account->log);			
			break;

		}			

		case LOGSCREEN_TARGET_UNKNOWN:
			
			UplinkAbort ( "Unknown LogScreen Target" );

		default:

			char errormsg [64];
			sprintf ( errormsg, "Unrecognised LogScreen Target:%d", TARGET );
			UplinkAbort ( errormsg );

	}

	UplinkAssert (logbank);
	return logbank;

}

void LogScreen::Load ( FILE *file )
{

	LoadID ( file );

	ComputerScreen::Load ( file );

	fread ( &nextpage, sizeof(nextpage), 1, file );
	fread ( &TARGET, sizeof(TARGET), 1, file );

	LoadID_END ( file );

}

void LogScreen::Save ( FILE *file )
{

	SaveID ( file );

	ComputerScreen::Save ( file );

	fwrite ( &nextpage, sizeof(nextpage), 1, file );
	fwrite ( &TARGET, sizeof(TARGET), 1, file );

	SaveID_END ( file );

}

void LogScreen::Print ()
{

	printf ( "LogScreen : \n" );
	
	ComputerScreen::Print ();
	
	printf ( "\tNextPage = %d, TARGET = %d\n", nextpage, TARGET );

}

char *LogScreen::GetID ()
{

	return "SCR_LOGS";

}

int LogScreen::GetOBJECTID ()
{

	return OID_LOGSCREEN;

}


