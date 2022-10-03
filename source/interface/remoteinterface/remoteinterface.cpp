
#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>

#include <GL/glu.h>

#include "gucci.h"
#include "vanbakel.h"

#include "interface/interface.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/serialise.h"

#include "game/game.h"

#include "world/world.h"
#include "world/player.h"
#include "world/vlocation.h"
#include "world/computer/computer.h"
#include "world/computer/lancomputer.h"
#include "world/computer/lanmonitor.h"
#include "world/computer/computerscreen/computerscreen.h"
#include "world/computer/computerscreen/genericscreen.h"

#include "interface/remoteinterface/remoteinterface.h"


// === List all Remote Interface objects here =================================


#include "interface/remoteinterface/messagescreen_interface.h"
#include "interface/remoteinterface/passwordscreen_interface.h"
#include "interface/remoteinterface/menuscreen_interface.h"
#include "interface/remoteinterface/bbsscreen_interface.h"
#include "interface/remoteinterface/dialogscreen_interface.h"
#include "interface/remoteinterface/fileserverscreen_interface.h"
#include "interface/remoteinterface/linksscreen_interface.h"
#include "interface/remoteinterface/logscreen_interface.h"
#include "interface/remoteinterface/swsalesscreen_interface.h"
#include "interface/remoteinterface/hwsalesscreen_interface.h"
#include "interface/remoteinterface/recordscreen_interface.h"
#include "interface/remoteinterface/useridscreen_interface.h"
#include "interface/remoteinterface/accountscreen_interface.h"
#include "interface/remoteinterface/contactscreen_interface.h"
#include "interface/remoteinterface/newsscreen_interface.h"
#include "interface/remoteinterface/criminalscreen_interface.h"
#include "interface/remoteinterface/securityscreen_interface.h"
#include "interface/remoteinterface/academicscreen_interface.h"
#include "interface/remoteinterface/rankingscreen_interface.h"
#include "interface/remoteinterface/consolescreen_interface.h"
#include "interface/remoteinterface/socialsecurityscreen_interface.h"
#include "interface/remoteinterface/loansscreen_interface.h"
#include "interface/remoteinterface/shareslistscreen_interface.h"
#include "interface/remoteinterface/sharesviewscreen_interface.h"
#include "interface/remoteinterface/faithscreen_interface.h"
#include "interface/remoteinterface/cypherscreen_interface.h"
#include "interface/remoteinterface/voiceanalysisscreen_interface.h"
#include "interface/remoteinterface/companyinfoscreen_interface.h"
#include "interface/remoteinterface/voicephonescreen_interface.h"
#include "interface/remoteinterface/highsecurityscreen_interface.h"
#include "interface/remoteinterface/nearestgatewayscreen_interface.h"
#include "interface/remoteinterface/changegatewayscreen_interface.h"
#include "interface/remoteinterface/codecardscreen_interface.h"
#include "interface/remoteinterface/disconnectedscreen_interface.h"
#include "interface/remoteinterface/protovisionscreen_interface.h"
#include "interface/remoteinterface/nuclearwarscreen_interface.h"
#include "interface/remoteinterface/radiotransmitterscreen_interface.h"


// ============================================================================




RemoteInterface::RemoteInterface ()
{

	currentscreenindex = 0;
	previousscreenindex = 0;
	screen = NULL;
	security_level = 10;
	strcpy ( security_name, " " );

}

RemoteInterface::~RemoteInterface ()
{
	
	if ( screen ) delete screen;

}


void RemoteInterface::RunNewLocation ()
{

	if ( screen ) {
		screen->Remove ();
		delete screen;
		screen = NULL;
	}

	previousscreenindex = 0;
	currentscreenindex = 0;
	strcpy ( security_name, "Guest" );
	security_level = 10;

    if ( game->GetWorld ()->GetPlayer ()->GetRemoteHost ()->GetOBJECTID () == OID_VLOCATIONSPECIAL ) {

        // This is a special Virtual location
        // eg a modem or a phone

        currentscreenindex = ((VLocationSpecial *) game->GetWorld ()->GetPlayer ()->GetRemoteHost ())->screenIndex;

    }
    else 
    {

        // This could still be a LAN computer

        VLocation *vl = game->GetWorld ()->GetPlayer ()->GetRemoteHost ();
        if ( vl ) {
            Computer *comp = game->GetWorld ()->GetComputer ( vl->computer );
            if ( comp && comp->GetOBJECTID () == OID_LANCOMPUTER ) {

                LanComputer *lanComp = (LanComputer *) comp;

		        if ( lanComp->systems.ValidIndex(LanMonitor::currentSelected) ) {

			        LanComputerSystem *system = lanComp->systems.GetData(LanMonitor::currentSelected);
			        if ( system->screenIndex != -1 ) 
                        currentscreenindex = system->screenIndex;			

                }

            }

		}        

    }

	Create ();


	if ( !GetComputerScreen ()->GetComputer ()->isrunning ) {

		// This computer is fucked
		// So disconnect now

		game->GetWorld ()->GetPlayer ()->connection.Disconnect ();
		game->GetWorld ()->GetPlayer ()->connection.Reset ();
		
		game->GetInterface ()->GetRemoteInterface ()->RunNewLocation ();
		game->GetInterface ()->GetRemoteInterface ()->RunScreen ( 8 );

	}

	if ( !GetComputerScreen ()->GetComputer ()->isexternallyopen ) {

		// This computer cannot be connected to from an external location

		char *ipfrom = game->GetWorld ()->GetPlayer ()->GetConnection ()->GetGhost ();
		VLocation *vlfrom = game->GetWorld ()->GetVLocation ( ipfrom );
		UplinkAssert (vlfrom);
		Computer *compfrom = vlfrom->GetComputer ();
		UplinkAssert (compfrom);

		if ( strcmp ( compfrom->companyname, GetComputerScreen ()->GetComputer ()->companyname ) != 0 ) {

			game->GetWorld ()->GetPlayer ()->connection.Disconnect ();
			game->GetWorld ()->GetPlayer ()->connection.Reset ();
			
			game->GetInterface ()->GetRemoteInterface ()->RunNewLocation ();
			game->GetInterface ()->GetRemoteInterface ()->RunScreen ( 9 );

		}

	}

}

local void li_draw ( Button *button, bool highlighted, bool clicked ) 
{

	glColor4f ( 1.0, 1.0, 1.0, 0.8 );
	
	glBegin ( GL_LINE_LOOP );
		glVertex2f ( button->x, button->y );
		glVertex2f ( button->x + button->width, button->y );
		glVertex2f ( button->x + button->width, button->y + button->height );
		glVertex2f ( button->x, button->y + button->height );
	glEnd ();
 
}

void RemoteInterface::Create ()
{

	if ( !IsVisible () ) {

		// Run the interface
		RunScreen ( currentscreenindex );

	}

}

void RemoteInterface::Remove ()
{

	if ( IsVisible () )
		if ( screen ) 
			screen->Remove ();

}

void RemoteInterface::SetSecurity ( char *newname, int newvalue )
{
	
	UplinkAssert ( strlen (newname) < SIZE_PERSON_NAME );

	strcpy ( security_name, newname );
	security_level = newvalue;

}

void RemoteInterface::RunScreen ( int screenindex )
{
	
	if ( screen ) {
		screen->Remove ();
		delete screen;
		screen = NULL;
	}

	previousscreenindex = currentscreenindex;
	currentscreenindex = screenindex;

	VLocation *vl = game->GetWorld ()->GetPlayer ()->GetRemoteHost ();
	Computer *comp = vl->GetComputer ();

	UplinkAssert ( comp );

	// Interface with this computer

	ComputerScreen *cs = comp->GetComputerScreen ( currentscreenindex );
	
	if ( cs ) {

		switch ( cs->GetOBJECTID () ) {

			case OID_MESSAGESCREEN		:			screen = new MessageScreenInterface ();		break;
			case OID_PASSWORDSCREEN		:			screen = new PasswordScreenInterface ();	break;
			case OID_MENUSCREEN			:			screen = new MenuScreenInterface ();		break;
			case OID_DIALOGSCREEN		:			screen = new DialogScreenInterface ();		break;
			case OID_USERIDSCREEN		:			screen = new UserIDScreenInterface ();		break;
			case OID_LOGSCREEN			:			screen = new LogScreenInterface ();			break;
			case OID_BBSSCREEN			:			screen = new BBSScreenInterface ();			break;
			case OID_LINKSSCREEN		:			screen = new LinksScreenInterface ();		break;	
			case OID_SHARESLISTSCREEN	:			screen = new SharesListScreenInterface ();	break;
			case OID_CYPHERSCREEN		:			screen = new CypherScreenInterface ();		break;
			case OID_HIGHSECURITYSCREEN :			screen = new HighSecurityScreenInterface ();break;
            case OID_DISCONNECTEDSCREEN :           screen = new DisconnectedScreenInterface ();break;

			case OID_GENERICSCREEN		:

				switch ( ((GenericScreen *) cs)->SCREEN_TYPE ) {
					
					case SCREEN_FILESERVERSCREEN:	screen = new FileServerScreenInterface ();		break;
					case SCREEN_SWSALESSCREEN	:	screen = new SWSalesScreenInterface ();			break;
					case SCREEN_HWSALESSCREEN	:	screen = new HWSalesScreenInterface ();			break;			
					case SCREEN_RECORDSCREEN	:   screen = new RecordScreenInterface ();			break;
					case SCREEN_ACCOUNTSCREEN	:	screen = new AccountScreenInterface ();			break;
					case SCREEN_CONTACTSCREEN	:	screen = new ContactScreenInterface ();			break;
					case SCREEN_NEWSSCREEN		:	screen = new NewsScreenInterface ();			break;
					case SCREEN_CRIMINALSCREEN  :	screen = new CriminalScreenInterface ();		break;
					case SCREEN_SECURITYSCREEN	:	screen = new SecurityScreenInterface ();		break;
					case SCREEN_ACADEMICSCREEN	:	screen = new AcademicScreenInterface ();		break;
					case SCREEN_RANKINGSCREEN	:	screen = new RankingScreenInterface ();			break;
					case SCREEN_CONSOLESCREEN	:	screen = new ConsoleScreenInterface ();			break;
					case SCREEN_SOCSECSCREEN	:	screen = new SocialSecurityScreenInterface ();	break;
					case SCREEN_LOANSSCREEN		:	screen = new LoansScreenInterface ();			break;
					case SCREEN_SHARESVIEWSCREEN:	screen = new SharesViewScreenInterface ();		break;
					case SCREEN_FAITHSCREEN		:	screen = new FaithScreenInterface ();			break;
					case SCREEN_VOICEANALYSIS	:	screen = new VoiceAnalysisScreenInterface ();	break;
					case SCREEN_COMPANYINFO		:	screen = new CompanyInfoScreenInterface ();		break;
					case SCREEN_VOICEPHONE		:	screen = new VoicePhoneScreenInterface ();		break;
					case SCREEN_NEARESTGATEWAY  :   screen = new NearestGatewayScreenInterface ();	break;
					case SCREEN_CHANGEGATEWAY	:   screen = new ChangeGatewayScreenInterface ();	break;
                    case SCREEN_CODECARD        :   screen = new CodeCardScreenInterface ();        break;
                    case SCREEN_PROTOVISION     :   screen = new ProtovisionScreenInterface ();     break;                  
                    case SCREEN_NUCLEARWAR      :   screen = new NuclearWarScreenInterface ();      break;
                    case SCREEN_RADIOTRANSMITTER:   screen = new RadioTransmitterScreenInterface ();break;

					default:
						UplinkAbort ( "Unrecognised GenericScreen\n" );

				}
				break;

			case -1:
				printf ( "RemoteInterface warning : Unknown ComputerScreen \n" );
				break;

			default:
				UplinkAbort ("Unrecognised ComputerScreen");

		}

		UplinkAssert ( screen );
		screen->Create ( cs );

		// Bring all running tasks to the front
		SvbShowAllTasks ();

	}
	else {

		// There is no interface to this computer system
		UplinkAbort ( "Tried to run an invalid screenid" );

	}

}

bool RemoteInterface::IsVisible ()
{

	return ( screen && GetInterfaceScreen ()->IsVisible () );

}

RemoteInterfaceScreen *RemoteInterface::GetInterfaceScreen ()
{

	UplinkAssert ( screen );
	return screen;

}

ComputerScreen *RemoteInterface::GetComputerScreen ()
{

	UplinkAssert ( screen );
	return screen->GetComputerScreen ();

}

void RemoteInterface::Load ( FILE *file )
{

	LoadID ( file );

	fread ( &previousscreenindex, sizeof(previousscreenindex), 1, file );
	fread ( &currentscreenindex, sizeof(currentscreenindex), 1, file );
	fread ( security_name, sizeof(security_name), 1, file );
	fread ( &security_level, sizeof(security_level), 1, file );

	Create ();

	LoadID_END ( file );

}

void RemoteInterface::Save ( FILE *file )
{

	SaveID ( file );

	fwrite ( &previousscreenindex, sizeof(previousscreenindex), 1, file );
	fwrite ( &currentscreenindex, sizeof(currentscreenindex), 1, file );
	fwrite ( security_name, sizeof(security_name), 1, file );
	fwrite ( &security_level, sizeof(security_level), 1, file );

	SaveID_END ( file );

}

void RemoteInterface::Print ()
{

	printf ( "RemoteInterface : CurrentScreenIndex = %d\n, PreviousScreenIndex = %d\n, "
			 "Security_Name = %s, Security_Level = %d\n", 
			 currentscreenindex, previousscreenindex, security_name, security_level );

}

void RemoteInterface::Update ()
{

	if ( screen ) screen->Update ();

}

char *RemoteInterface::GetID ()
{

	return "RINT";

}


