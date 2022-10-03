
#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>

#include <GL/glu.h>


#include "app/globals.h"
#include "app/app.h"
#include "app/opengl_interface.h"

#include "options/options.h"

#include "game/game.h"
#include "game/data/data.h"

#include "world/world.h"
#include "world/player.h"
#include "world/vlocation.h"
#include "world/computer/computerscreen/genericscreen.h"

#include "interface/interface.h"
#include "interface/remoteinterface/remoteinterface.h"
#include "interface/remoteinterface/nearestgatewayscreen_interface.h"


NearestGatewayScreenInterface::NearestGatewayScreenInterface ()
{
}

NearestGatewayScreenInterface::~NearestGatewayScreenInterface ()
{
}

void NearestGatewayScreenInterface::DrawBlack ( Button *button, bool highlighted, bool clicked )
{

	UplinkAssert (button);
	clear_draw ( button->x, button->y, button->width, button->height );

}

void NearestGatewayScreenInterface::DrawLocation ( Button *button, bool highlighted, bool clicked )
{

	UplinkAssert (button);

    glColor4f ( 1.0, 1.0, 1.0, 1.0 );

    glBegin ( GL_QUADS );
        glVertex2i ( button->x, button->y );
        glVertex2i ( button->x + 7, button->y );
        glVertex2i ( button->x + 7, button->y + 7 );
        glVertex2i ( button->x, button->y +7 );
    glEnd ();

	// Write some text

	char unused [64];
	int index;
	sscanf ( button->name, "%s %d", unused, &index );

	UplinkAssert (index < NUM_PHYSICALGATEWAYLOCATIONS);
	const PhysicalGatewayLocation *pgl = &(PHYSICALGATEWAYLOCATIONS[index]);
	UplinkAssert (pgl);

	GciDrawText ( button->x - 10, button->y + 20, pgl->city );
//	GciDrawText ( button->x - 10, button->y + 30, pgl->country );

}

void NearestGatewayScreenInterface::ClickLocation ( Button *button )
{

	// Set the players physical location 

	char unused [64];
	int index;
	sscanf ( button->name, "%s %d", unused, &index );

	UplinkAssert (index < NUM_PHYSICALGATEWAYLOCATIONS);
	const PhysicalGatewayLocation *pgl = &(PHYSICALGATEWAYLOCATIONS[index]);
	UplinkAssert (pgl);

	game->GetWorld ()->GetPlayer ()->GetLocalHost ()->SetPLocation ( pgl->x, pgl->y );

	
	// Go on to the next screen

	NearestGatewayScreenInterface *thisint = (NearestGatewayScreenInterface *) game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ();
	UplinkAssert (thisint);
	GenericScreen *gs = thisint->GetComputerScreen ();
	UplinkAssert(gs);

	game->GetInterface ()->GetRemoteInterface ()->RunScreen (gs->nextpage);

}

bool NearestGatewayScreenInterface::ReturnKeyPressed ()
{
	
	return false;

}

void NearestGatewayScreenInterface::Create ( ComputerScreen *newcs )
{

	cs = newcs;
 
	if ( !IsVisible () ) {

		EclRegisterButton ( 40, 10, 350, 25, GetComputerScreen ()->subtitle, "", "nearestgateway_maintitle" );
		EclRegisterButtonCallbacks ( "nearestgateway_maintitle", DrawMainTitle, NULL, NULL, NULL );

		// Black out the computer picture

		EclRegisterButton ( 470, 280, 140, 140, " ", " ", "nearestgateway_black" );
		EclRegisterButtonCallbacks ( "nearestgateway_black", DrawBlack, NULL, NULL, NULL );

		// Work out the size/ratios of the map

		int screenw = app->GetOptions ()->GetOptionValue ( "graphics_screenwidth" );
	    int screenh = app->GetOptions ()->GetOptionValue ( "graphics_screenheight" );

		int x1 = 23;
		int y1 = 50;
		int fullsizeX = screenw - x1 * 2;
		int fullsizeY = 316.0 * ( fullsizeX / 595.0 );

		// Create the large map graphic

		EclRegisterButton ( x1, y1, fullsizeX, fullsizeY, "", "", "nearestgateway_largemap" );                                                
		button_assignbitmap ( "nearestgateway_largemap", "worldmaplarge.tif" );
		EclRegisterButtonCallbacks ( "nearestgateway_largemap", imagebutton_drawtextured, NULL, NULL, NULL );
    
		// Create a button for each gateway

		for ( int i = 0; i < NUM_PHYSICALGATEWAYLOCATIONS; ++i ) {

			const PhysicalGatewayLocation *pgl = &(PHYSICALGATEWAYLOCATIONS[i]);
			UplinkAssert (pgl);

			char bname [64];
			sprintf ( bname, "nearestgateway_location %d", i );

			char tooltip [128];
			sprintf ( tooltip, "Select %s as your local Gateway", pgl->city );

			EclRegisterButton ( x1 + SX(pgl->x), y1 + SY(pgl->y), 7, 7, pgl->city, tooltip, bname );
			EclRegisterButtonCallbacks ( bname, DrawLocation, ClickLocation, button_click, button_highlight );

		}

		// Create some helpfull text

		EclRegisterButton ( 50, y1 + fullsizeY + 20, 400, 70, " ", " ", "nearestgateway_helptext" );
		EclRegisterButtonCallbacks ( "nearestgateway_helptext", textbutton_draw, NULL, NULL, NULL );
		EclRegisterCaptionChange ( "nearestgateway_helptext", "Uplink Corporation has offices all around the world.\n"
															  "Please select a nearby location for your Gateway.\n"
															  "You will dial into this location when you use Uplink.", 2000 );

	}

}

void NearestGatewayScreenInterface::Remove ()
{

	if ( IsVisible () ) {
		
		EclRemoveButton ( "nearestgateway_maintitle" );
		EclRemoveButton ( "nearestgateway_black" );
		EclRemoveButton ( "nearestgateway_largemap" );

		for ( int i = 0; i < NUM_PHYSICALGATEWAYLOCATIONS; ++i ) {

			char bname [64];
			sprintf ( bname, "nearestgateway_location %d", i );
			EclRemoveButton ( bname );

		}

		EclRemoveButton ( "nearestgateway_helptext" );

	}

}

bool NearestGatewayScreenInterface::IsVisible ()
{
	
	return ( EclGetButton ( "nearestgateway_largemap" ) != NULL );

}

int NearestGatewayScreenInterface::ScreenID ()
{

	return SCREEN_NEARESTGATEWAY;

}

GenericScreen *NearestGatewayScreenInterface::GetComputerScreen ()
{
	
	UplinkAssert (cs);
	return (GenericScreen *) cs;

}
