
#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>

#include <GL/glu.h> /* glu extention library */


#include "eclipse.h"
#include "gucci.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/opengl_interface.h"

#include "game/game.h"

#include "interface/interface.h"
#include "interface/remoteinterface/remoteinterface.h"
#include "interface/remoteinterface/protovisionscreen_interface.h"

#include "world/message.h"
#include "world/computer/computer.h"
#include "world/computer/computerscreen/genericscreen.h"


void ProtovisionScreenInterface::Close ( Button *button )
{

	int nextpage;
	sscanf ( button->name, "protovisionscreen_close %d", &nextpage );

	if ( nextpage != -1 ) game->GetInterface ()->GetRemoteInterface ()->RunScreen ( nextpage );

}

bool ProtovisionScreenInterface::ReturnKeyPressed ()
{

	if ( GetComputerScreen ()->nextpage != -1 ) 
		game->GetInterface ()->GetRemoteInterface ()->RunScreen ( GetComputerScreen ()->nextpage );

	return true;

}

void ProtovisionScreenInterface::NuclearWarDraw ( Button *button, bool highlighted, bool clicked )
{

    textbutton_draw ( button, highlighted, clicked );

}

void ProtovisionScreenInterface::NuclearWar ( Button *button )
{

#ifdef DEMOGAME
    
    EclRegisterCaptionChange ( button->name, "What, in the demo? ARE YOU SERIOUS?", 1000 );

#else

    game->GetInterface ()->GetRemoteInterface ()->RunScreen ( 4 );
    
#endif

}

void ProtovisionScreenInterface::Create ()
{

	if ( cs ) Create ( cs );
	else printf ( "ProtovisionScreenInterface::Create, tried to create when GenericScreen==NULL\n" );

}

static void NuclearWarStart ()
{

    EclRegisterCaptionChange ( "protovisionscreen_nuclearwar", "Global Thermonuclear War", 1000 );    

}

void ProtovisionScreenInterface::Create ( ComputerScreen *newcs )
{

	UplinkAssert ( newcs );
	cs = newcs;

	if ( !IsVisible () ) {

		EclRegisterButton ( 80, 60, 350, 25, GetComputerScreen ()->maintitle, "", "protovisionscreen_maintitle" );
		EclRegisterButtonCallbacks ( "protovisionscreen_maintitle", DrawMainTitle, NULL, NULL, NULL );
		EclRegisterButton ( 80, 80, 350, 20, GetComputerScreen ()->subtitle, "", "protovisionscreen_subtitle" );
		EclRegisterButtonCallbacks ( "protovisionscreen_subtitle", DrawSubTitle, NULL, NULL, NULL );

        char *textmessage = "Falken's maze\n"
							"Black jack\n"
							"Gin rummy\n"
							"hearts\n"
							"bridge\n"
							"checkers\n"
							"chess\n"
							"poker\n"
							"fighter combat\n"
							"guerrilla engagement\n"
							"desert warefare\n"
							"air-to-ground actions\n"
							"theatrewide tactical warefare\n"
							"theatrewide biotoxic and chemical warefare";

		EclRegisterButton ( 50, 120, 300, 245, "", "", "protovisionscreen_message" );
		EclRegisterButtonCallbacks ( "protovisionscreen_message", textbutton_draw, NULL, NULL, NULL );
		EclRegisterCaptionChange   ( "protovisionscreen_message", textmessage, 5000, NuclearWarStart );		

        EclRegisterButton ( 50, 340, 300, 20, "", "", "protovisionscreen_nuclearwar" );
        EclRegisterButtonCallbacks ( "protovisionscreen_nuclearwar", NuclearWarDraw, NuclearWar, button_click, button_highlight );

		int width = 80;
		char name [128];
		sprintf ( name, "protovisionscreen_close %d", GetComputerScreen ()->nextpage );
		EclRegisterButton ( 320 - width/2, 370, width, 20, "Close", "Close this screen", name );
		EclRegisterButtonCallback ( name, Close );

	}

}

void ProtovisionScreenInterface::Remove ()
{

	if ( IsVisible () ) {

  		EclRemoveButton ( "protovisionscreen_maintitle" );
		EclRemoveButton ( "protovisionscreen_subtitle" );

		EclRemoveButton ( "protovisionscreen_message" );
        EclRemoveButton ( "protovisionscreen_nuclearwar" );

		char name [128];
		sprintf ( name, "protovisionscreen_close %d", GetComputerScreen ()->nextpage );
		EclRemoveButton ( name );

	}

}

void ProtovisionScreenInterface::Update () 
{

}

bool ProtovisionScreenInterface::IsVisible ()
{

	return ( EclGetButton ( "protovisionscreen_message" ) != NULL );

}

GenericScreen *ProtovisionScreenInterface::GetComputerScreen ()
{

	UplinkAssert (cs);
	return (GenericScreen *) cs;

}

int ProtovisionScreenInterface::ScreenID ()
{

	return SCREEN_PROTOVISION;

}
