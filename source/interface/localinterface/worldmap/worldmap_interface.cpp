
#ifdef WIN32
#include <windows.h> 
#endif

#include <GL/gl.h>

#include <GL/glu.h>

#include <math.h>

#include "gucci.h"
#include "vanbakel.h"
#include "soundgarden.h"
#include "redshirt.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/serialise.h"
#include "app/opengl.h"
#include "app/opengl_interface.h"

#include "game/game.h"
#include "game/data/data.h"

#include "world/world.h"

#include "interface/interface.h"
#include "interface/localinterface/localinterface.h"
#include "interface/localinterface/hud_interface.h"
#include "interface/localinterface/worldmap/worldmap_layout.h"
#include "interface/localinterface/worldmap/worldmap_interface.h"
#include "interface/localinterface/worldmap/rectangle.h"
#include "interface/localinterface/phonedialler.h"
#include "interface/remoteinterface/remoteinterface.h"

#include "world/player.h"
#include "world/computer/computer.h"

#include "options/options.h"


static int stippleindex = 0;
static int STIPPLE []  = { 0x1111,        //        0001000100010001
                           0x1111,        //        0001000100010001
                           0x2222,        //        0010001000100010
                           0x2222,        //        0010001000100010
                           0x4444,        //        0100010001000100
                           0x4444,        //        0100010001000100
                           0x8888,        //        1000100010001000
                           0x8888 };      //        1000100010001000

static int lastupdate = 0;
static int scrollupdate = 0;

static float revelationColour = 1.0;

int WorldMapInterface::stipplepattern = STIPPLE [0];

float WorldMapInterface::scrollX = 0.0;
float WorldMapInterface::scrollY = 0.0;
float WorldMapInterface::zoom = 1.0;

#define MAXZOOM 4.0


void WorldMapInterface::CycleStipplePattern ()
{

    ++stippleindex;
    if ( stippleindex > 7 ) stippleindex = 0;

    stipplepattern = STIPPLE [stippleindex];

}

void WorldMapInterface::FullScreenClick ( Button *button )
{

	game->GetInterface ()->GetLocalInterface ()->RunScreen ( SCREEN_NONE );
	CreateWorldMapInterface ( WORLDMAP_LARGE );

}

void WorldMapInterface::LocationClick ( Button *button )
{

    if ( !game->GetWorld ()->GetPlayer ()->GetConnection ()->Connected () ) {
        
        char ip [SIZE_VLOCATION_IP];
        int index;

        sscanf ( button->name, "worldmap %s %d", ip, &index );
            
        game->GetWorld ()->GetPlayer ()->GetConnection ()->AddVLocation ( ip );

        SgPlaySound ( RsArchiveFileOpen ("sounds/addlink.wav"), "sounds/addlink.wav", false );

    }
    else {

        EclRegisterCaptionChange ( "worldmap_texthelp", "You cannot modify an existing connection. Disconnect first." );

    }

}

void WorldMapInterface::ConnectClick ( Button *button )
{

    if ( game->GetWorld ()->GetPlayer ()->IsConnected () ) {

        // Disconnect

		bool mapmaximised = (IsVisibleWorldMapInterface () == WORLDMAP_LARGE);

        game->GetWorld ()->GetPlayer ()->GetConnection ()->Disconnect ();
        game->GetWorld ()->GetPlayer ()->GetConnection ()->Reset ();

        game->GetInterface ()->GetRemoteInterface ()->RunNewLocation ();
        game->GetInterface ()->GetRemoteInterface ()->RunScreen ( 1 );        

        if ( mapmaximised ) {
			// Re-build the large map 
			// as it is now covered by the new remote interface
			RemoveWorldMapInterface ();
			CreateWorldMapInterface ( WORLDMAP_LARGE );
		}

    }
    else {

        if ( IsVisibleWorldMapInterface () == WORLDMAP_LARGE ) {

            // Connect

            CreateWorldMapInterface ( WORLDMAP_SMALL );

            PhoneDialler pd;
            pd.Create ( 100, 100 );
            pd.DialNumber ( game->GetWorld ()->GetPlayer ()->GetConnection ()->GetTarget () );    
            pd.Remove ();

            game->GetWorld ()->GetPlayer ()->GetConnection ()->Connect ();
            game->GetInterface ()->GetRemoteInterface ()->RunNewLocation ();

        }

    }

}

void WorldMapInterface::ConnectMouseMove ( Button *button )
{

    if ( game->GetWorld ()->GetPlayer ()->IsConnected () || 
         IsVisibleWorldMapInterface () == WORLDMAP_LARGE ) {

        if ( game->GetWorld ()->GetPlayer ()->IsConnected () )
            button->SetTooltip ( "Break the connection to your current remote host" );

        else
            button->SetTooltip ( "Establish the connection to your current remote host" );    

        button_highlight ( button );

    }

}

void WorldMapInterface::ConnectMouseDown ( Button *button )
{

    if ( game->GetWorld ()->GetPlayer ()->IsConnected () ||
         IsVisibleWorldMapInterface () == WORLDMAP_LARGE ) {

        button_click ( button );

    }

}

void WorldMapInterface::ConnectDraw ( Button *button, bool highlighted, bool clicked )
{

    if ( game->GetWorld ()->GetPlayer ()->IsConnected () || 
         IsVisibleWorldMapInterface () == WORLDMAP_LARGE ) {
                
        if ( game->GetWorld ()->GetPlayer ()->IsConnected () )
            button->SetCaption ( "Disconnect" );
    
        else
            button->SetCaption ( "Connect" );
        
        button_draw ( button, highlighted, clicked );

    }
    else {

        clear_draw ( button->x, button->y, button->width, button->height );

    }

}

void WorldMapInterface::CancelClick ( Button *button )
{

	// Close the screen if a connection is set up to somewhere else
	//

    if ( game->GetWorld ()->GetPlayer ()->IsConnected () )
		
		CloseClick ( NULL );


	//
	// Close the screen if we are at home without any half set up connection

	if ( strcmp ( game->GetWorld ()->GetPlayer ()->GetRemoteHost ()->ip, IP_LOCALHOST ) == 0 &&
		 strcmp ( game->GetWorld ()->GetPlayer ()->GetConnection ()->GetTarget (), IP_LOCALHOST ) == 0 ) 
	
		 CloseClick ( NULL );


	//
    // Cancel any "half set up" connections

	if ( ! game->GetWorld ()->GetPlayer ()->IsConnected () )
		game->GetWorld ()->GetPlayer ()->GetConnection ()->Reset ();
 
}

void WorldMapInterface::CloseClick ( Button *button )
{

    CreateWorldMapInterface ( WORLDMAP_SMALL );
    SgPlaySound ( RsArchiveFileOpen ("sounds/close.wav"), "sounds/close.wav", false );

}

void WorldMapInterface::SaveConnectionClick ( Button *button )
{
    
    WorldMapInterface *wmi = &(game->GetInterface ()->GetLocalInterface ()->GetHUD ()->wmi);
    UplinkAssert (wmi);
    wmi->SaveCurrentConnection ();
    EclRegisterCaptionChange ( "worldmap_texthelp", "Connection saved" );

}

void WorldMapInterface::LoadConnectionClick ( Button *button )
{

    WorldMapInterface *wmi = &(game->GetInterface ()->GetLocalInterface ()->GetHUD ()->wmi);
    UplinkAssert (wmi);
    wmi->LoadConnection ();

}

void WorldMapInterface::DrawAllObjects ()
{
    LList <WorldMapInterfaceObject *> &locations = layout->GetLocations();
    LList <WorldMapInterfaceLabel *> &labels = layout->GetLabels();

    // Draw all labels

    for ( int i = 0; i < labels.Size (); ++i ) {

        WorldMapInterfaceLabel *object = labels.GetData (i);
        UplinkAssert (object);
        object->Draw ( scrollX * GetLargeMapWidth(),
                       scrollY * GetLargeMapHeight(),
                       zoom );

    }

    // Draw all location Dots

    for ( int il = 0; il < locations.Size (); ++il ) {

        WorldMapInterfaceObject *object = locations.GetData (il);
        UplinkAssert (object);
        object->Draw ( scrollX * GetLargeMapWidth(),
                       scrollY * GetLargeMapHeight(),
                       zoom );

    }

}

void WorldMapInterface::DrawWorldMapSmall ( Button *button, bool highlighted, bool clicked )
{

    // Draw the background bitmap

    imagebutton_draw ( button, highlighted, clicked );

    // Draw the players connection

    if ( !game->GetWorld ()->GetPlayer ()->IsConnected () ) {

        // Draw a dot to represent the player

        glColor4f ( 1.0, 1.0, 1.0, 1.0 );
        
        int playerX = game->GetWorld ()->GetPlayer ()->GetLocalHost ()->x;
        int playerY = game->GetWorld ()->GetPlayer ()->GetLocalHost ()->y;
        int scaledX = button->x + GetScaledX ( playerX, WORLDMAP_SMALL );
        int scaledY = button->y + GetScaledY ( playerY, WORLDMAP_SMALL );
        
        glBegin ( GL_QUADS );
            glVertex2f ( scaledX - 1, scaledY - 1 );    
            glVertex2f ( scaledX + 2, scaledY - 1 );
            glVertex2f ( scaledX + 2, scaledY + 2 );
            glVertex2f ( scaledX - 1, scaledY + 2 );
        glEnd ();

    }
    else {

        Connection *connection = game->GetWorld ()->GetPlayer ()->GetConnection ();

        // Draw the lines

        glColor4f ( 1.0, 1.0, 1.0, 1.0 );
        glLineWidth ( 2.0 );        
        glLineStipple ( 1, stipplepattern );    
        glEnable ( GL_LINE_STIPPLE );

        glBegin ( GL_LINE_STRIP );

            for ( int li = 0; li < connection->vlocations.Size (); ++li ) {

                VLocation *vl = game->GetWorld ()->GetVLocation ( connection->vlocations.GetData (li) );
                UplinkAssert ( vl );
                glVertex2f ( button->x + GetScaledX ( vl->x, WORLDMAP_SMALL ), button->y + GetScaledY ( vl->y, WORLDMAP_SMALL ) );

                if ( connection->TraceInProgress () &&
                     connection->traceprogress == (connection->vlocations.Size () - li - 1) &&
                     game->GetWorld ()->GetPlayer ()->gateway.HasHUDUpgrade (HUDUPGRADE_MAPSHOWSTRACE) ) {
                    glColor4f ( 1.0, 0.0, 0.0, 1.0 );
                    glVertex2f ( button->x + GetScaledX ( vl->x, WORLDMAP_SMALL ), button->y + GetScaledY ( vl->y, WORLDMAP_SMALL ) );                        
                }

            }

        glEnd ();

        glLineWidth ( 1.0 );
        glDisable ( GL_LINE_STIPPLE );
        glColor4f ( 1.0, 1.0, 1.0, 1.0 );

        // Draw the dots

        glBegin ( GL_QUADS );

            for ( int di = 0; di < connection->vlocations.Size (); ++di ) {

                VLocation *vl = game->GetWorld ()->GetVLocation ( connection->vlocations.GetData (di) );
                UplinkAssert ( vl );

                int x = button->x + GetScaledX ( vl->x, WORLDMAP_SMALL );
                int y = button->y + GetScaledY ( vl->y, WORLDMAP_SMALL );

                glVertex2f ( x - 1, y - 1 );
                glVertex2f ( x + 2, y - 1 );
                glVertex2f ( x + 2, y + 2 );
                glVertex2f ( x - 1, y + 2 );

                if ( connection->traceprogress == (connection->vlocations.Size () - di - 1) &&
                     game->GetWorld ()->GetPlayer ()->gateway.HasHUDUpgrade (HUDUPGRADE_MAPSHOWSTRACE) )
                    glColor4f ( 1.0, 0.0, 0.0, 1.0 );

            }

        glEnd ();

    }

    //
    // Draw red circles over computers infected with Revelation
    //

    glBegin ( GL_QUADS );

        for ( int i = 0; i < game->GetWorld ()->plotgenerator.infected.Size (); ++i ) {

            char *ip = game->GetWorld ()->plotgenerator.infected.GetData (i);
            UplinkAssert (ip);

            VLocation *vl = game->GetWorld ()->GetVLocation (ip);
            UplinkAssert (vl);
        
            int x = button->x + GetScaledX ( vl->x, WORLDMAP_SMALL );
            int y = button->y + GetScaledX ( vl->y, WORLDMAP_SMALL );

            glColor4f ( revelationColour, 0.0, 0.0, 1.0 );
            glVertex2f ( x - 3, y - 3 );
            glVertex2f ( x + 4, y - 3 );
            glVertex2f ( x + 4, y + 4 );
            glVertex2f ( x - 3, y + 4 );

            revelationColour -= 0.09;
            if ( revelationColour < 0.0 ) revelationColour = 1.0;

        }

    glEnd ();

    if ( highlighted || clicked ) {

        glColor4f ( 0.4, 0.4, 0.8, 1.0 );
        border_draw ( button );

    }

}

class FrameRate {
public:
  FrameRate(unsigned desiredFrameRate /* Frames per second */)
    : frameDuration( 1000 / desiredFrameRate ),
      lastFrame(0)
  {
  }

  void drawingNow() {
    lastFrame = EclGetAccurateTime();
  };

  bool shouldDrawNow() {
    return EclGetAccurateTime() - lastFrame  > frameDuration;
  }

private:
  unsigned lastFrame;
  unsigned frameDuration;
};

void WorldMapInterface::DrawWorldMapLarge ( Button *button, bool highlighted, bool clicked )
{
#ifdef LIMIT_FRAMERATE
  static FrameRate frameRate(10); /* 1 times a second */

  if (!frameRate.shouldDrawNow()) 
    return;

  frameRate.drawingNow();
#endif 

    UplinkAssert ( button );

    //
    // Clipping

	glPushAttrib ( GL_ALL_ATTRIB_BITS );

	int screenheight = app->GetOptions ()->GetOptionValue ( "graphics_screenheight" );
	glScissor ( button->x, screenheight - (button->y + button->height), button->width, button->height );	
	glEnable ( GL_SCISSOR_TEST );

    //
    // Draw the background image
    //

    /* Draw the image */
	glColor4f ( 1.0, 1.0, 1.0, 1.0 );
	glEnable ( GL_TEXTURE_2D );      
	Image *image = button->image_standard;	    
    UplinkAssert (image);

	glBindTexture ( GL_TEXTURE_2D, 1 );
	glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGB, image->Width(), image->Height(), 0, GL_RGB, GL_UNSIGNED_BYTE, image->GetRGBPixels() );

	// ** used to be here **

    float windowW = 1.0 / zoom;
    float windowH = 1.0 / zoom;

	glBegin(GL_QUADS);
		glTexCoord2f(scrollX, 1.0 - scrollY);                       glVertex2f(button->x, button->y);
		glTexCoord2f(scrollX + windowW, 1.0 - scrollY); 	        glVertex2f(button->x + button->width, button->y);
		glTexCoord2f(scrollX + windowW, 1.0 - (scrollY + windowH)); glVertex2f(button->x + button->width, button->y + button->height);
		glTexCoord2f(scrollX, 1.0 - (scrollY + windowH));           glVertex2f(button->x, button->y + button->height);
	glEnd ();

    glDisable ( GL_TEXTURE_2D );

    //
    // Gimme a border
    //

	glColor3ub ( 81, 138, 215 );
	border_draw ( button );

    //
    // Draw the text labels, dots etc
    //

    WorldMapInterface *wmi = (WorldMapInterface *) &(game->GetInterface ()->GetLocalInterface ()->GetHUD ()->wmi);
    UplinkAssert (wmi);

    wmi->DrawAllObjects ();

    //
    // Draw red circles over computers infected with Revelation
    //

    glBegin ( GL_QUADS );

        for ( int j = 0; j < game->GetWorld ()->plotgenerator.infected.Size (); ++j ) {

            char *ip = game->GetWorld ()->plotgenerator.infected.GetData (j);
            UplinkAssert (ip);

            VLocation *vl = game->GetWorld ()->GetVLocation (ip);
            UplinkAssert (vl);
        
            int x = button->x + GetScaledX ( vl->x, WORLDMAP_LARGE );
            int y = button->y + GetScaledX ( vl->y, WORLDMAP_LARGE );

            glColor4f ( revelationColour, 0.0, 0.0, 1.0 );
            glVertex2f ( x - 6, y - 6 );
            glVertex2f ( x + 7, y - 7 );
            glVertex2f ( x + 7, y + 7 );
            glVertex2f ( x - 6, y + 7 );

            revelationColour -= 0.09;
            if ( revelationColour < 0.0 ) 
                revelationColour = 1.0;

        }

    glEnd ();

    //
    // Draw connecting lines over the players connection
    //

    Connection *connection = game->GetWorld ()->GetPlayer ()->GetConnection ();

    glLineWidth ( 2.0 );
    glColor4f ( 1.0, 1.0, 1.0, 1.0 );

    glLineStipple ( 2, stipplepattern );
    glEnable ( GL_LINE_STIPPLE );

    glBegin ( GL_LINE_STRIP );

        for ( int i = 0; i < connection->vlocations.Size (); ++i ) {

            VLocation *vl = game->GetWorld ()->GetVLocation ( connection->vlocations.GetData (i) );
            UplinkAssert ( vl );

            int xpos = button->x + GetScaledX ( vl->x, WORLDMAP_LARGE );
            int ypos = button->y + GetScaledY ( vl->y, WORLDMAP_LARGE );

            glVertex2f ( xpos, ypos );

            if ( connection->TraceInProgress () &&
                 connection->traceprogress == (connection->vlocations.Size () - i - 1) &&
                 game->GetWorld ()->GetPlayer ()->gateway.HasHUDUpgrade (HUDUPGRADE_MAPSHOWSTRACE) ) {
                glColor4f ( 1.0, 0.0, 0.0, 1.0 );
                glVertex2f ( xpos, ypos );                
            }

        }

    glEnd ();


	glLineWidth ( 1.0 );
    glDisable ( GL_LINE_STIPPLE );
    glDisable ( GL_SCISSOR_TEST );

  	glPopAttrib ();

}

void WorldMapInterface::DrawLocation ( Button *button, bool highlighted, bool clicked )
{

    //
    // Am I visible?

    MapRectangle mapRect = GetLargeMapRect();
    MapRectangle buttonRect ( button->x - 2, button->y - 2, button->width + 4, button->height + 4 );
    if ( !mapRect.contains ( buttonRect ) )
        return;

    // Get some useful information about this location

    char ip [ SIZE_VLOCATION_IP ];
    int index;
    sscanf ( button->name, "worldmap %s %d", ip, &index );

    // If the player has an account, draw a surrounding box
    
    int accesslevel = game->GetWorld ()->GetPlayer ()->HasAccount (ip);
    if ( accesslevel != -1 ) {
        
        int x = button->x - 2;
        int y = button->y - 2;
        int w = button->width + 3;
        int h = button->height + 3;
        
        if ( accesslevel > 1 ) {
            glLineStipple ( 1, stipplepattern );        
            glEnable ( GL_LINE_STIPPLE );
            x -= 1;
            y -= 1;
            w += 2;
            h += 2;
        }
                        
        glColor3f ( 1.0, 1.0, 1.0 );        

		glBegin ( GL_LINE_LOOP );
			glVertex2i ( x, y + h );
			glVertex2i ( x, y );
			glVertex2i ( x + w, y );
			glVertex2i ( x + w, y + h );
		glEnd ();

        glDisable ( GL_LINE_STIPPLE );

    }

    // If the player is highlighting this button, print some text in a box

    if ( clicked || highlighted ) {

        VLocation *vl = game->GetWorld ()->GetVLocation ( ip );
        UplinkAssert ( vl );
        Computer *comp = vl->GetComputer ();        

        Button *largemap = EclGetButton ( "worldmap_largemap" );
        UplinkAssert (largemap);

        int screenheight = app->GetOptions ()->GetOptionValue ( "graphics_screenheight" );
		int scissorX = largemap->x;
		int scissorY = screenheight - (largemap->y + largemap->height);
		int scissorW = largemap->width;
		int scissorH = largemap->height;
        glScissor ( scissorX, scissorY, scissorW, scissorH );    
        glEnable ( GL_SCISSOR_TEST );

		// Draw a box behind the text

		int x = button->x - 15;
		int y = button->y + 20;
		
		char msg[256];
		
		sprintf(msg, "Owner: %s", comp->companyname);
		int w1 = GciTextWidth(msg) + 10;

		sprintf(msg, "IP: %s", ip);
		int w2 = GciTextWidth(msg) + 10;

		int w = w1 > w2 ? w1 : w2;
		int h = 26;

		// Make sure the info box doesn't get clipped

		if ( x < 0 ) x = 0;
		if ( y < 0 ) y = 0;
		if ( x + w + 2 > largemap->x + largemap->width )  x = largemap->x + largemap->width - w - 2;
		if ( y + h + 2 > largemap->y + largemap->height ) y = largemap->y + largemap->height - h - 2;

		// Draw the box

		glBegin ( GL_QUADS );		
			glColor3ub ( 8, 20, 0 );		glVertex2i ( x, y + h );
			glColor3ub ( 8, 20, 124 );		glVertex2i ( x, y );
			glColor3ub ( 8, 20, 0 );		glVertex2i ( x + w, y );
			glColor3ub ( 8, 20, 124 );		glVertex2i ( x + w, y + h );
		glEnd ();

		glColor3ub ( 81, 138, 215 );

		glBegin ( GL_LINE_LOOP );
			glVertex2i ( x, y + h );
			glVertex2i ( x, y );
			glVertex2i ( x + w, y );
			glVertex2i ( x + w, y + h );
		glEnd ();
		

		// Draw the text

        char line1 [64], line2 [128];
        sprintf ( line1, "IP: %s", ip );
        sprintf ( line2, "Owner: %s", comp->companyname );
        glColor3f ( 1.0, 1.0, 1.0 );
		GciDrawText ( x + 5, y + 10, line1 );
        GciDrawText ( x + 5, y + 20, line2 );

        glDisable ( GL_SCISSOR_TEST );

    }

}

void WorldMapInterface::CreateWorldMapInterface ( int SIZE ) 
{

    if ( IsVisibleWorldMapInterface () != SIZE ) {
        
        RemoveWorldMapInterface ();

        if      ( SIZE == WORLDMAP_SMALL )    CreateWorldMapInterface_Small ();
        else if ( SIZE == WORLDMAP_LARGE )    CreateWorldMapInterface_Large ();
        else    UplinkWarning ( "Unrecognised Map size" );

    }

}

void WorldMapInterface::CreateWorldMapInterface_Small ()
{

    int screenw = app->GetOptions ()->GetOptionValue ( "graphics_screenwidth" );
    int screenh = app->GetOptions ()->GetOptionValue ( "graphics_screenheight" );

    int fullsizeX = screenw * PANELSIZE;
    int fullsizeY = 100.0 * ( fullsizeX / 188.0 );

    EclRegisterButton ( screenw - fullsizeX - 3, 3, fullsizeX, fullsizeY, "", "Global Communications", "worldmap_smallmap" );                
    button_assignbitmap ( "worldmap_smallmap", "worldmapsmall.tif" );                                
    EclGetButton ( "worldmap_smallmap" )->image_standard->Scale ( fullsizeX, fullsizeY );                
    //EclGetButton ( "worldmap_smallmap" )->image_standard->ScaleToOpenGL ();                
    EclRegisterButtonCallbacks ( "worldmap_smallmap", DrawWorldMapSmall, FullScreenClick, button_click, button_highlight );

    EclRegisterButton ( screenw - fullsizeX - 3, fullsizeY + 4, fullsizeX, 15, "", "", "worldmap_connect" );
    EclRegisterButtonCallbacks ( "worldmap_connect", ConnectDraw, ConnectClick, ConnectMouseDown, ConnectMouseMove );

    EclRegisterButton ( screenw - 3, 0, 3, 3, "", "Global Communications", "worldmap_toprightclick" );
    EclRegisterButtonCallbacks ( "worldmap_toprightclick", NULL, FullScreenClick, button_click, button_highlight );

}

void WorldMapInterface::ZoomInClick ( Button *button )
{

    WorldMapInterface *thisint = (WorldMapInterface *) &(game->GetInterface ()->GetLocalInterface ()->GetHUD ()->wmi);
    UplinkAssert (thisint);
    thisint->ChangeZoom ( 0.1 );
}

void WorldMapInterface::ZoomOutClick ( Button *button )
{
    WorldMapInterface *thisint = (WorldMapInterface *) &(game->GetInterface ()->GetLocalInterface ()->GetHUD ()->wmi);
    UplinkAssert (thisint);
    thisint->ChangeZoom ( -0.1 );
}

void WorldMapInterface::ZoomButtonClick ( Button *button )
{
    WorldMapInterface *thisint = (WorldMapInterface *) &(game->GetInterface ()->GetLocalInterface ()->GetHUD ()->wmi);
    UplinkAssert (thisint);

    float fractionX = (float) (get_mouseX() - button->x) / (float) button->width;

    thisint->SetZoom( 1.0 + fractionX * (MAXZOOM - 1.0) );
}

void WorldMapInterface::ZoomButtonDraw ( Button *button, bool highlighted, bool clicked )
{

    clear_draw ( button->x, button->y, button->width, button->height );

    // 
    // Zoom bar

    glColor3ub ( 81, 138, 215 );

    glBegin ( GL_TRIANGLES );
        glVertex2f ( button->x, button->y + 5 );    
        glVertex2f ( button->x + button->width, button->y + 5 );
        glVertex2f ( button->x + button->width, button->y + 10 );
    glEnd ();
    
    WorldMapInterface *thisint = (WorldMapInterface *) &(game->GetInterface ()->GetLocalInterface ()->GetHUD ()->wmi);
    UplinkAssert (thisint);

    //
    // Slider
    
    glColor3f ( 0.0, 0.0, 0.7 );
    float sliderX = 2 + (button->width-4) * (thisint->zoom - 1.0) / (MAXZOOM - 1.0);
    
    glBegin ( GL_QUADS );
        glVertex2f ( button->x + sliderX - 1, button->y );    
        glVertex2f ( button->x + sliderX + 1, button->y );
        glVertex2f ( button->x + sliderX + 1, button->y + button->height );
        glVertex2f ( button->x + sliderX - 1, button->y + button->height );
    glEnd ();

}

int WorldMapInterface::GetLargeMapX1()
{
    return 23;
}

int WorldMapInterface::GetLargeMapY1()
{
    return 50;
}

int WorldMapInterface::GetLargeMapWidth()
{
    int screenw = app->GetOptions ()->GetOptionValue ( "graphics_screenwidth" );
    return screenw - GetLargeMapX1() * 2;
}

int WorldMapInterface::GetLargeMapHeight()
{
    int screenh = app->GetOptions ()->GetOptionValue ( "graphics_screenheight" );
    return 316.0 * ( GetLargeMapWidth() / 595.0 );
}

MapRectangle WorldMapInterface::GetLargeMapRect() 
{
    return MapRectangle(GetLargeMapX1(), GetLargeMapY1(), GetLargeMapWidth(), GetLargeMapHeight());
}


void WorldMapInterface::ProgramLayoutEngine()
{
    // Program the Map Layout Engine with the positions of the feature points
    // and the labels
    
    layout->Reset();
    
    // Create all known links
    
    Player *player = game->GetWorld ()->GetPlayer ();
    LList <char *> &conn = player->connection.vlocations;
    LList <char *> &links = player->links;

    // Add all links in the connection
    
    for ( int i = 0; i < conn.Size(); ++i) {

        VLocation *vl = game->GetWorld ()->GetVLocation ( conn.GetData (i) );
        UplinkAssert ( vl );
	
        if (vl->displayed) {

            float oldZoom = zoom;
            float oldScrollX = scrollX;
            float oldScrollY = scrollY;

            zoom = 1.0;
            scrollX = 0.0;
            scrollY = 0.0;

            layout->AddLocation ( GetScaledX ( vl->x, WORLDMAP_LARGE ) + GetLargeMapX1() - 3, 
		            		      GetScaledY ( vl->y, WORLDMAP_LARGE ) + GetLargeMapY1() - 3,
        			    	      vl->computer );

            zoom = oldZoom;
            scrollX = oldScrollX;
            scrollY = oldScrollY;
            
        }

    }
    
    // Add all the other known links

    for ( int j = 0; j < links.Size (); ++j ) {
                            
        VLocation *vl = game->GetWorld ()->GetVLocation ( links.GetData (j) );
        UplinkAssert ( vl );

        if ( !player->connection.LocationIncluded(vl->ip) && vl->displayed ) {

            float oldZoom = zoom;
            float oldScrollX = scrollX;
            float oldScrollY = scrollY;

            zoom = 1.0;
            scrollX = 0.0;
            scrollY = 0.0;

            layout->AddLocation ( GetScaledX ( vl->x, WORLDMAP_LARGE ) + GetLargeMapX1() - 3, 
                                  GetScaledY ( vl->y, WORLDMAP_LARGE ) + GetLargeMapY1() - 3,
                                  vl->computer );

            zoom = oldZoom;
            scrollX = oldScrollX;
            scrollY = oldScrollY;
            
        }

    }
}

void WorldMapInterface::SaveCurrentConnection ()
{

    DeleteLListData ( &savedconnection );
    savedconnection.Empty ();

    Connection *connection = &(game->GetWorld ()->GetPlayer ()->connection);

    for ( int i = 0; i < connection->GetSize (); ++i ) {

        char *thisip = connection->vlocations.GetData (i);
        UplinkAssert (thisip);

        char *ipcopy = new char [SIZE_VLOCATION_IP];
        strcpy ( ipcopy, thisip );

        savedconnection.PutDataAtEnd (ipcopy);

    }

}

void WorldMapInterface::LoadConnection ()
{

    if ( game->GetWorld ()->GetPlayer ()->IsConnected () ) {

        EclRegisterCaptionChange ( "worldmap_texthelp", "You cannot modify an existing connection. Disconnect first." );

    }
    else {

        for ( int i = 0; i < savedconnection.Size (); ++i ) {

            char *ip = savedconnection.GetData (i);
            UplinkAssert (ip);

            game->GetWorld ()->GetPlayer ()->connection.AddVLocation ( ip );

        }

    }

}

void WorldMapInterface::CreateWorldMapInterface_Large ()
{

    SgPlaySound ( RsArchiveFileOpen ("sounds/openworldmap.wav"), "sounds/openworldmap.wav", false );
        
    MapRectangle mapRect = GetLargeMapRect();
    
    int x1 = mapRect.x1;
    int y1 = mapRect.y1;
    int fullsizeX = mapRect.width;
    int fullsizeY = mapRect.height;
    
    WorldMapInterface *thisint = (WorldMapInterface *) &(game->GetInterface ()->GetLocalInterface ()->GetHUD ()->wmi);
    UplinkAssert (thisint);

    // Create the background bitmap
    
    int start = EclGetAccurateTime();

    EclRegisterButton ( x1, y1, fullsizeX, fullsizeY, "", "", "worldmap_largemap" );                                                
    button_assignbitmap ( "worldmap_largemap", "worldmaplarge.tif" );
           
//    cout << "Large world map graphic created, " << EclGetAccurateTime() - start << " ms\n";
    start = EclGetAccurateTime();
    
    //EclGetButton ( "worldmap_largemap" )->image_standard->SetAlpha ( 0.99 );
    EclRegisterButtonCallbacks ( "worldmap_largemap", DrawWorldMapLarge, NULL, NULL, button_highlight);                
    
    VLocation *vl = game->GetWorld ()->GetVLocation ( game->GetWorld ()->GetPlayer ()->localhost );
    UplinkAssert ( vl );
    EclRegisterButton ( GetScaledX ( vl->x, WORLDMAP_LARGE ) + x1 - 3, GetScaledY ( vl->y, WORLDMAP_LARGE ) + y1 - 3, 7, 7, "Localhost", "Your physical location", "worldmap 127.0.0.1 0" );
    EclRegisterButtonCallbacks ( "worldmap 127.0.0.1 0", DrawLocation, NULL, NULL, button_highlight );

    // Create a text box for help messages

    EclRegisterButton ( 120, y1 + fullsizeY + 3, 340, 25, "", "", "worldmap_texthelp" );
    EclRegisterButtonCallbacks ( "worldmap_texthelp", textbutton_draw, NULL, NULL, NULL );

    // Create the control buttons

    EclRegisterButton ( 20, y1 + fullsizeY + 5, 50, 15, "Save", "Store the current connection settings", "worldmap_saveconnection" );
    EclRegisterButtonCallback ( "worldmap_saveconnection", SaveConnectionClick );

    EclRegisterButton ( 75, y1 + fullsizeY + 5, 50, 15, "Load", "Load previously saved connection settings", "worldmap_loadconnection" );
    EclRegisterButtonCallback ( "worldmap_loadconnection", LoadConnectionClick );

    EclRegisterButton ( 150, y1 + fullsizeY + 5, 15, 15, "-", "Zoom out", "worldmap_zoomout" );
    EclRegisterButtonCallback ( "worldmap_zoomout", ZoomOutClick );

    EclRegisterButton ( 170, y1 + fullsizeY + 5, 50, 15, "---", "Click to zoom", "worldmap_zoom" );
    EclRegisterButtonCallbacks ( "worldmap_zoom", ZoomButtonDraw, ZoomButtonClick, button_click, button_highlight );

    EclRegisterButton ( 225, y1 + fullsizeY + 5, 15, 15, "+", "Zoom in", "worldmap_zoomin" );
    EclRegisterButtonCallback ( "worldmap_zoomin", ZoomInClick );

    EclRegisterButton ( 455, y1 + fullsizeY + 5, 70, 15, "Cancel", "Cancels changes made to your connection", "worldmap_cancel" );
    EclRegisterButtonCallback ( "worldmap_cancel", CancelClick );

    EclRegisterButton ( 530, y1 + fullsizeY + 5, 70, 15, "", "", "worldmap_connect" );
    EclRegisterButtonCallbacks ( "worldmap_connect", ConnectDraw, ConnectClick, ConnectMouseDown, ConnectMouseMove );
    
    EclRegisterButton ( 605, y1 + fullsizeY + 6, 13, 13, "", "Close the World Map screen", "worldmap_close" );
    button_assignbitmaps ( "worldmap_close", "close.tif", "close_h.tif", "close_c.tif" );
    EclRegisterButtonCallback ( "worldmap_close", CloseClick );
    
    // Create all known links
    
    LList <char *> *links = &(game->GetWorld ()->GetPlayer ()->links);                

    for ( int i = 0; i < links->Size (); ++i ) {
                            
        VLocation *vl = game->GetWorld ()->GetVLocation ( links->GetData (i) );
        UplinkAssert ( vl );

        if ( vl->displayed || 
             game->GetWorld ()->GetPlayer ()->connection.LocationIncluded (vl->ip) ) {

            char name [128];
            char caption [128];
            char tooltip [128];
            sprintf ( name, "worldmap %s %d", vl->ip, i );
            sprintf ( caption, "%s", vl->ip );
            sprintf ( tooltip, "Connect to IP address %s", vl->ip );
            EclRegisterButton ( GetScaledX ( vl->x, WORLDMAP_LARGE ) + x1 - 3, GetScaledY ( vl->y, WORLDMAP_LARGE ) + y1 - 3, 7, 7, caption, tooltip, name );
            EclRegisterButtonCallbacks ( name, DrawLocation, LocationClick, button_click, button_highlight );

        }

    }
    
//    cout << "Large world map controls added, " << EclGetAccurateTime() - start << " ms\n";
    start = EclGetAccurateTime();
    
    thisint->layout->FullLayoutLabels();
    
//    cout << "Large would map labels layed-out, " << EclGetAccurateTime() - start << " ms\n";

	// Bring all running tasks to the front

	SvbShowAllTasks ();
	
}

void WorldMapInterface::RemoveWorldMapInterface ()
{

    int mapsize = IsVisibleWorldMapInterface ();

    if ( mapsize != WORLDMAP_NONE ) {

        if ( mapsize == WORLDMAP_SMALL ) {

            EclRemoveButton ( "worldmap_smallmap" );
            EclRemoveButton ( "worldmap_connect" );
            EclRemoveButton ( "worldmap_toprightclick" );

        }
        else if ( mapsize == WORLDMAP_LARGE ) {

            EclRemoveButton ( "worldmap_largemap" );

            EclRemoveButton ( "worldmap_saveconnection" );
            EclRemoveButton ( "worldmap_loadconnection" );

            EclRemoveButton ( "worldmap_zoomout" );
            EclRemoveButton ( "worldmap_zoom" );
            EclRemoveButton ( "worldmap_zoomin" );

            EclRemoveButton ( "worldmap_connect" );
            EclRemoveButton ( "worldmap_cancel" );
            EclRemoveButton ( "worldmap_close" );
 
            EclRemoveButton ( "worldmap_scrollleft" );
            EclRemoveButton ( "worldmap_scrollright" );
            EclRemoveButton ( "worldmap_scrollup" );
            EclRemoveButton ( "worldmap_scrolldown" );

            LList <char *> *links = &(game->GetWorld ()->GetPlayer ()->links);                

            for ( int i = 0; i < links->Size (); ++i ) {

                VLocation *vl = game->GetWorld ()->GetVLocation ( links->GetData (i) );
                    
                char name [128];
                sprintf ( name, "worldmap %s %d", vl->ip, i );
                EclRemoveButton ( name );

            }

            EclRemoveButton ( "worldmap_texthelp" );
            EclRemoveButton ( "worldmap 127.0.0.1 0" );

        }

    }
    
}

int WorldMapInterface::IsVisibleWorldMapInterface ()
{

    if ( EclGetButton ( "worldmap_smallmap" ) != 0 )
        return WORLDMAP_SMALL;

    else if ( EclGetButton ( "worldmap_largemap" ) != 0 )
        return WORLDMAP_LARGE;

    else
        return WORLDMAP_NONE;

}


int WorldMapInterface::GetScaledX ( int x, int SIZE )
{

    UplinkAssert ( x >= 0 && x < VIRTUAL_WIDTH );

    switch ( SIZE ) {

        case WORLDMAP_SMALL:
            return ( (float) x / (float) VIRTUAL_WIDTH ) * EclGetButton ( "worldmap_smallmap" )->width;

        case WORLDMAP_LARGE:
            float fullSize = ( (float) x / (float) VIRTUAL_WIDTH ) * GetLargeMapWidth();
            return ( fullSize * zoom ) - ( scrollX * GetLargeMapWidth() * zoom );
            break;

    }

    return -1;

}


int WorldMapInterface::GetScaledY ( int y, int SIZE )
{

    UplinkAssert ( y >= 0 && y < VIRTUAL_HEIGHT );

    switch ( SIZE ) {

        case WORLDMAP_SMALL:
            return ( (float) y / (float) VIRTUAL_HEIGHT ) * EclGetButton ( "worldmap_smallmap" )->height;

        case WORLDMAP_LARGE:
            float fullsize = ( (float) y / (float) VIRTUAL_HEIGHT ) * GetLargeMapHeight();
            return ( fullsize * zoom ) - ( scrollY * GetLargeMapHeight() * zoom );
            break;

    }

    return -1;

}

WorldMapInterface::WorldMapInterface ()
    : layout (NULL)
{
}

WorldMapInterface::~WorldMapInterface ()
{

    if ( layout ) delete layout;

}

void WorldMapInterface::Create ()
{

    Create ( WORLDMAP_LARGE );

}

void WorldMapInterface::Create ( int SIZE )
{
//    cout << "Creating Worldmap Interface\n";
    
    layout = new WorldMapLayout(GetLargeMapRect());
    ProgramLayoutEngine();
    
    CreateWorldMapInterface ( SIZE );

}

void WorldMapInterface::ScrollX ( float x )
{

    // background

    float windowW = 1.0 / zoom;
    float windowH = 1.0 / zoom;

    scrollX += x;
    if ( scrollX < 0.0 ) scrollX = 0.0;
    if ( scrollX + windowW > 1.0 ) scrollX = 1.0 - windowW;
    EclDirtyButton ( "worldmap_largemap" );

    MapRectangle mapRect = GetLargeMapRect();
    
    int x1 = mapRect.x1;
    int y1 = mapRect.y1;

    // Buttons

    LList <char *> *links = &(game->GetWorld ()->GetPlayer ()->links);                

    for ( int i = 0; i < links->Size (); ++i ) {
                            
        VLocation *vl = game->GetWorld ()->GetVLocation ( links->GetData (i) );
        UplinkAssert ( vl );
        char name [128];
        sprintf ( name, "worldmap %s %d", vl->ip, i );

        Button *button = EclGetButton ( name );
        if ( button ) {
            button->x = GetScaledX ( vl->x, WORLDMAP_LARGE ) + x1 - 3;
            button->y = GetScaledY ( vl->y, WORLDMAP_LARGE ) + y1 - 3;            
        }
        
    }

}

void WorldMapInterface::ScrollY ( float y )
{

    // background

    float windowW = 1.0 / zoom;
    float windowH = 1.0 / zoom;

    scrollY += y;
    if ( scrollY < 0.0 ) scrollY = 0.0;
    if ( scrollY + windowH > 1.0 ) scrollY = 1.0 - windowH;
    EclDirtyButton ( "worldmap_largemap" );

    MapRectangle mapRect = GetLargeMapRect();
    
    int x1 = mapRect.x1;
    int y1 = mapRect.y1;

    // Buttons

    LList <char *> *links = &(game->GetWorld ()->GetPlayer ()->links);                

    for ( int i = 0; i < links->Size (); ++i ) {
                            
        VLocation *vl = game->GetWorld ()->GetVLocation ( links->GetData (i) );
        UplinkAssert ( vl );
        char name [128];
        sprintf ( name, "worldmap %s %d", vl->ip, i );

        Button *button = EclGetButton ( name );
        if ( button ) {
            button->x = GetScaledX ( vl->x, WORLDMAP_LARGE ) + x1 - 3;
            button->y = GetScaledY ( vl->y, WORLDMAP_LARGE ) + y1 - 3;
        }
        
    }

}

void WorldMapInterface::SetZoom ( float z )
{

    float oldWindowW = 1.0 / zoom;
    float oldWindowH = 1.0 / zoom;

    zoom = z;
    if ( zoom < 1.0 ) zoom = 1.0;
    if ( zoom > MAXZOOM ) zoom = MAXZOOM;
    
    float windowW = 1.0 / zoom;
    float windowH = 1.0 / zoom;
    
    scrollX += (float) ( oldWindowW - windowW ) / 2.0;
    scrollY += (float) ( oldWindowH - windowH ) / 2.0;

    if ( scrollX < 0.0 ) scrollX = 0.0;
    if ( scrollX + windowW > 1.0 ) scrollX = 1.0 - windowW;
    if ( scrollY < 0.0 ) scrollY = 0.0;
    if ( scrollY + windowH > 1.0 ) scrollY = 1.0 - windowH;
    
    EclDirtyButton ( "worldmap_largemap" );

    MapRectangle mapRect = GetLargeMapRect();
    
    int x1 = mapRect.x1;
    int y1 = mapRect.y1;

    // Buttons

    LList <char *> *links = &(game->GetWorld ()->GetPlayer ()->links);                

    for ( int i = 0; i < links->Size (); ++i ) {
                            
        VLocation *vl = game->GetWorld ()->GetVLocation ( links->GetData (i) );
        UplinkAssert ( vl );
        char name [128];
        sprintf ( name, "worldmap %s %d", vl->ip, i );

        Button *button = EclGetButton ( name );
        if ( button ) {
            button->x = GetScaledX ( vl->x, WORLDMAP_LARGE ) + x1 - 3;
            button->y = GetScaledY ( vl->y, WORLDMAP_LARGE ) + y1 - 3;            
        }
        
    }

    EclDirtyButton ( "worldmap_zoom" );
}

void WorldMapInterface::ChangeZoom ( float z )
{

    SetZoom ( zoom + z );

}

void WorldMapInterface::CheckLinksChanged()
{
    LList <char *> &links = game->GetWorld ()->GetPlayer ()->links;
    
    // Count the number of links that should be displayed
    
    int count = game->GetWorld ()->GetPlayer ()->connection.GetSize ();

    for ( int i = 0; i < links.Size (); ++i ) {
        char *link = links.GetData (i);
        UplinkAssert (link);
        if ( !game->GetWorld ()->GetPlayer ()->connection.LocationIncluded (link) ) {
            VLocation *vl = game->GetWorld ()->GetVLocation (link);
            UplinkAssert (vl);
	    
            if ( vl->displayed ) 
		        ++count;
        }
    }

    // Program Layout Engine if the num links has changed

    if (layout->GetLocations().Size() != count) 
        ProgramLayoutEngine();

}

void WorldMapInterface::Update ()
{
    CheckLinksChanged();
    layout->PartialLayoutLabels();

    if ( IsVisible () ) {

        int timesincelastupdate = EclGetAccurateTime () - lastupdate;

        // Update the small map

        if ( timesincelastupdate > 100 ) {
            
            if ( EclGetButton ( "worldmap_smallmap" ) ) {
                EclDirtyButton ( "worldmap_smallmap" );    
                EclDirtyButton ( "worldmap_connect" );
				CycleStipplePattern ();
                lastupdate = EclGetAccurateTime ();
            }

		}

		// Update the large map

		if ( timesincelastupdate > 200 ) {

            if ( EclGetButton ( "worldmap_largemap" ) ) {
                EclDirtyButton ( "worldmap_largemap" );
				CycleStipplePattern ();
                lastupdate = EclGetAccurateTime ();
            }

        }

        // Update scrolling

        int timesincelastscroll = EclGetAccurateTime() - scrollupdate;

        if ( timesincelastscroll > 20 ) {

            Button *worldmap = EclGetButton ( "worldmap_largemap" );
            if ( !worldmap ) return;

            int mouseX = get_mouseX() - worldmap->x;
            int mouseY = get_mouseY() - worldmap->y;
            
            if ( mouseX < 0 || mouseY < 0 || mouseX > worldmap->width || mouseY > worldmap->height )
                return;

            int dX = mouseX - ( worldmap->width / 2.0 );
            int dY = mouseY - ( worldmap->height / 2.0 );

	    int scrollThresholdX = worldmap->width * 0.42;
	    int scrollThresholdY = worldmap->height * 0.40;

            if ( dX > scrollThresholdX ) ScrollX ( (dX - scrollThresholdX) / 3500.0 );
            if ( dX < -scrollThresholdX ) ScrollX ( (dX + scrollThresholdX) / 3500.0 );

            if ( dY > scrollThresholdY ) ScrollY ( (dY - scrollThresholdY) / 3500.0 );
            if ( dY < -scrollThresholdY ) ScrollY ( (dY + scrollThresholdY) / 3500.0 );

            scrollupdate = EclGetAccurateTime ();

        }

    }

}

void WorldMapInterface::Remove ()
{

    RemoveWorldMapInterface ();

}

bool WorldMapInterface::IsVisible ()
{

    return ( IsVisibleWorldMapInterface () != 0 );

}

int WorldMapInterface::ScreenID ()
{

    return SCREEN_MAP;

}
