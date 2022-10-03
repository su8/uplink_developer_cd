
#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>

#include <GL/glu.h>

#include "eclipse.h"
#include "gucci.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/opengl_interface.h"
#include "app/miscutils.h"
#include "app/serialise.h"

#include "options/options.h"

#include "game/game.h" 
#include "game/data/data.h"

#include "interface/interface.h"
#include "interface/scrollbox.h"
#include "interface/remoteinterface/remoteinterface.h"
#include "interface/remoteinterface/linksscreen_interface.h"
#include "interface/localinterface/phonedialler.h"

#include "world/world.h"
#include "world/message.h"
#include "world/player.h"
#include "world/computer/computer.h"
#include "world/computer/computerscreen/linksscreen.h"


int LinksScreenInterface::baseoffset = 0;


LinksScreenInterface::LinksScreenInterface ()
{
}

LinksScreenInterface::~LinksScreenInterface ()
{
    
    DeleteLListData ( &fulllist );
    DeleteLListData ( &filteredlist );

}

void LinksScreenInterface::LinkClick ( Button *button )
{

	UplinkAssert ( button );

	int fileindex;
	sscanf ( button->name, "linksscreen_link %d", &fileindex );
	fileindex += baseoffset;

	LList <char *> *filteredlist = &((LinksScreenInterface *) game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ())->filteredlist;

	if ( filteredlist->ValidIndex (fileindex) ) {

		char *ip = filteredlist->GetData (fileindex);

		game->GetWorld ()->GetPlayer ()->GetConnection ()->Disconnect ();
		game->GetWorld ()->GetPlayer ()->GetConnection ()->Reset ();

		PhoneDialler pd;
		pd.Create ( 100, 100 );
		pd.DialNumber ( ip );	
		pd.Remove ();
	
		game->GetWorld ()->GetPlayer ()->GetConnection ()->AddVLocation ( ip );
		game->GetWorld ()->GetPlayer ()->GetConnection ()->Connect ();

		game->GetInterface ()->GetRemoteInterface ()->RunNewLocation ();

	}

}

void LinksScreenInterface::LinkDraw ( Button *button, bool highlighted, bool clicked )
{

	UplinkAssert ( button );

	clear_draw ( button->x, button->y, button->width, button->height );

	int linkindex;
	sscanf ( button->name, "linksscreen_link %d", &linkindex );
	linkindex += baseoffset;

	LList <char *> *filteredlist = &((LinksScreenInterface *) game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ())->filteredlist;	
    char *link = filteredlist->ValidIndex(linkindex) ? filteredlist->GetData (linkindex) : NULL;

	/*
		Re above line

		calling getdata like that will be pretty slow, since GetData on a linked list 
		has to iterate accross the entire array.

		But this call of LinkDraw will come after another call to LinkDraw, for the link
		at linkindex-1.  This means that sequential access is occuring - which will be ok
		in terms of speed.

    */

	if ( link ) {

		if ( linkindex % 2 == 0 ) {

			glBegin ( GL_QUADS );
				SetColour ( "DarkPanelB" );   glVertex2i ( button->x, button->y );
				SetColour ( "DarkPanelA" );   glVertex2i ( button->x + button->width, button->y );
				SetColour ( "DarkPanelB" );   glVertex2i ( button->x + button->width, button->y + button->height );
				SetColour ( "DarkPanelA" );   glVertex2i ( button->x, button->y + button->height );
			glEnd ();

		}
		else {

			glBegin ( GL_QUADS );
				SetColour ( "DarkPanelA" );   glVertex2i ( button->x, button->y );
				SetColour ( "DarkPanelB" );   glVertex2i ( button->x + button->width, button->y );
				SetColour ( "DarkPanelA" );   glVertex2i ( button->x + button->width, button->y + button->height );
				SetColour ( "DarkPanelB" );   glVertex2i ( button->x, button->y + button->height );
			glEnd ();

		}

		SetColour ( "DefaultText" );

		// Draw the text

		GciDrawText ( button->x + 10, button->y + 10, link );

		char caption [SIZE_COMPUTER_NAME];
			
		if ( game->GetWorld ()->GetVLocation ( link ) )
			sprintf ( caption, "%s", game->GetWorld ()->GetVLocation ( link )->computer );
		else
			sprintf ( caption, "This link has expired" );

		GciDrawText ( button->x + 120, button->y + 10, caption );

		// Draw a bounding box
		if ( highlighted ) {

			SetColour ( "PanelHighlightBorder" );
			border_draw ( button );

		}

	}
	
}

void LinksScreenInterface::LinkMouseDown ( Button *button )
{

	UplinkAssert ( button );
	
	int linkindex;
	sscanf ( button->name, "linksscreen_link %d", &linkindex );
	linkindex += baseoffset;

	LList <char *> *filteredlist = &((LinksScreenInterface *) game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ())->filteredlist;

	if ( filteredlist->ValidIndex (linkindex) ) {

		button_click ( button );

	}

}

void LinksScreenInterface::LinkMouseMove ( Button *button )
{

	UplinkAssert ( button );
	
	int linkindex;
	sscanf ( button->name, "linksscreen_link %d", &linkindex );
	linkindex += baseoffset;

	LList <char *> *filteredlist = &((LinksScreenInterface *) game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ())->filteredlist;

	if ( filteredlist->ValidIndex (linkindex) ) {
 
		button_highlight ( button );

	}

}

void LinksScreenInterface::DeleteLinkDraw ( Button *button, bool highlighted, bool clicked )
{

	UplinkAssert ( button );
	
	int linkindex;
	sscanf ( button->name, "linksscreen_deletelink %d", &linkindex );
	linkindex += baseoffset;

	LList <char *> *filteredlist = &((LinksScreenInterface *) game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ())->filteredlist;
    char *link = filteredlist->ValidIndex(linkindex) ? filteredlist->GetData (linkindex) : NULL;

	if ( link ) 
		imagebutton_draw ( button, highlighted, clicked );

	else
		clear_draw ( button->x, button->y, button->width, button->height );

}

void LinksScreenInterface::DeleteLinkClick ( Button *button )
{

	/*
		Safe to delete this link
		(This button would not exist if these 
		were not player links)
	
		*/

	UplinkAssert ( button );
	
	int linkindex;
	sscanf ( button->name, "linksscreen_deletelink %d", &linkindex );
	linkindex += baseoffset;

	LList <char *> *filteredlist = &((LinksScreenInterface *) game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ())->filteredlist;
	char *link = filteredlist->ValidIndex (linkindex) ? filteredlist->GetData (linkindex) : NULL;

	if ( link && game->GetWorld ()->GetPlayer ()->HasLink (link) &&
         strcmp( link, IP_INTERNIC) != 0 ) {

		game->GetWorld ()->GetPlayer ()->RemoveLink (link);

		LinksScreenInterface *thisinterface = ((LinksScreenInterface *) game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ());
		UplinkAssert (thisinterface);
	
		int currentbaseoffset = baseoffset;
		thisinterface->SetFullList ( &(game->GetWorld ()->GetPlayer ()->links) );

		// Re-apply the filter
		
		Button *filterbutton = EclGetButton ( "linksscreen_filtertext" );
		if ( filterbutton ) {
			char *filter = filterbutton->caption;
			if ( filter [0] == '\x0' )  thisinterface->ApplyFilter ( NULL );
			else						thisinterface->ApplyFilter ( filter );
			baseoffset = currentbaseoffset;
		}
		else
			thisinterface->ApplyFilter ( NULL );

	}

}

void LinksScreenInterface::AddLinkDraw ( Button *button, bool highlighted, bool clicked )
{

	UplinkAssert ( button );
	
	int linkindex;
	sscanf ( button->name, "linksscreen_addlink %d", &linkindex );
	linkindex += baseoffset;

	LList <char *> *filteredlist = &((LinksScreenInterface *) game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ())->filteredlist;
    char *link = filteredlist->ValidIndex (linkindex) ? filteredlist->GetData (linkindex) : NULL;

	if ( link && !game->GetWorld ()->GetPlayer ()->HasLink (link) ) 
		imagebutton_draw ( button, highlighted, clicked );

	else
		clear_draw ( button->x, button->y, button->width, button->height );

}

void LinksScreenInterface::AddLinkClick ( Button *button )
{

	UplinkAssert ( button );
	
	int linkindex;
	sscanf ( button->name, "linksscreen_addlink %d", &linkindex );
	linkindex += baseoffset;

	LList <char *> *filteredlist = &((LinksScreenInterface *) game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ())->filteredlist;
    char *link = filteredlist->ValidIndex(linkindex) ? filteredlist->GetData (linkindex) : NULL;

	if ( link && !game->GetWorld ()->GetPlayer ()->HasLink (link) )
		game->GetWorld ()->GetPlayer ()->GiveLink ( link );

}

void LinksScreenInterface::ShowLinkDraw ( Button *button, bool highlighted, bool clicked )
{
    
	UplinkAssert ( button );
	
	int linkindex;
	sscanf ( button->name, "linksscreen_showlink %d", &linkindex );
	linkindex += baseoffset;

	LList <char *> *filteredlist = &((LinksScreenInterface *) game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ())->filteredlist;
    char *link = filteredlist->ValidIndex(linkindex) ? filteredlist->GetData (linkindex) : NULL;

    if ( link ) {

	    VLocation *vl = game->GetWorld ()->GetVLocation (link);

        if ( vl->displayed )
            imagebutton_draw ( button, highlighted, clicked );

        else
            clear_draw ( button->x, button->y, button->width, button->height );

    }
	else
		clear_draw ( button->x, button->y, button->width, button->height );

}

void LinksScreenInterface::ShowLinkClick ( Button *button )
{

	UplinkAssert ( button );
	
	int linkindex;
	sscanf ( button->name, "linksscreen_showlink %d", &linkindex );
	linkindex += baseoffset;

	LList <char *> *filteredlist = &((LinksScreenInterface *) game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ())->filteredlist;

	char *link = filteredlist->GetData (linkindex);

    if ( link ) {

	    VLocation *vl = game->GetWorld ()->GetVLocation (link);
        vl->SetDisplayed ( !vl->displayed );

    }

}

void LinksScreenInterface::FilterDraw ( Button *button, bool highlighted, bool clicked )
{

	UplinkAssert (button);

	textbutton_draw ( button, highlighted, clicked );

	glColor3f ( 1.0, 1.0, 1.0 );
	border_draw ( button );

}

void LinksScreenInterface::FilterClick ( Button *button )
{

	UplinkAssert (button);

	LinksScreenInterface *thisinterface = ((LinksScreenInterface *) game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ());
	UplinkAssert (thisinterface);

	char *filter = EclGetButton ( "linksscreen_filtertext" )->caption;

	if ( filter [0] == '\x0' )  thisinterface->ApplyFilter ( NULL );
	else						thisinterface->ApplyFilter ( filter );

}

void LinksScreenInterface::ScrollChange ( char *scrollname, int newValue )
{

    baseoffset = newValue;

	for ( int i = 0; i < NumLinksOnScreen(); ++i ) {

		char name [128];
		sprintf ( name, "linksscreen_link %d", i );
		EclDirtyButton ( name );

   		sprintf ( name, "linksscreen_deletelink %d", i );
    	EclDirtyButton ( name );

        sprintf ( name, "linksscreen_addlink %d", i );
		EclDirtyButton ( name );

        sprintf ( name, "linksscreen_showlink %d", i );
        EclDirtyButton ( name );

	}

}

void LinksScreenInterface::CloseClick ( Button *button )
{

	LinksScreenInterface *lsi = (LinksScreenInterface *) GetInterfaceScreen ( SCREEN_LINKSSCREEN );
	UplinkAssert (lsi);

	int nextpage = lsi->GetComputerScreen ()->nextpage;

	if ( nextpage != -1 ) 
		game->GetInterface ()->GetRemoteInterface ()->RunScreen ( nextpage );		
	
}

void LinksScreenInterface::SetFullList ( LList <char *> *newfulllist )
{

	UplinkAssert (newfulllist);

    DeleteLListData ( &fulllist );
    DeleteLListData ( &filteredlist );
    
	fulllist.Empty ();
	filteredlist.Empty ();

    for ( int i = 0; i < newfulllist->Size (); ++i ) {

        char *copydata = new char [SIZE_VLOCATION_IP];
        strcpy ( copydata, newfulllist->GetData (i) );
        fulllist.PutData ( copydata );

    }
	
	ApplyFilter ( NULL );

}

void LinksScreenInterface::SetFullList ()
{

	DeleteLListData ( &fulllist );
    fulllist.Empty ();

    for ( int i = 0; i < filteredlist.Size (); ++i ) {

        char *copydata = new char [SIZE_VLOCATION_IP];
        strcpy ( copydata, filteredlist.GetData (i) );
		fulllist.PutData ( copydata );

    }

	ApplyFilter ( NULL );

}

void LinksScreenInterface::ApplyFilter ( char *filter )
{

	//
	// Do the filtering
	//

    DeleteLListData ( &filteredlist );
    filteredlist.Empty ();

	if ( filter ) {

		char *lowercasefilter = LowerCaseString ( filter );

		for ( int i = 0; i < fulllist.Size (); ++i ) {

			char *computername = game->GetWorld ()->GetVLocation ( fulllist.GetData (i) )->computer;
			char *lowercasename = LowerCaseString ( computername );

            if ( strstr ( lowercasename, lowercasefilter ) != NULL) {

                char *datacopy = new char [SIZE_VLOCATION_IP];
                strcpy ( datacopy, fulllist.GetData (i) );
				filteredlist.PutData ( datacopy );

            }

			delete [] lowercasename;

		}

		delete [] lowercasefilter;

	}
	else {

        for ( int i = 0; i < fulllist.Size (); ++i ) {

            char *datacopy = new char [SIZE_VLOCATION_IP];
            strcpy ( datacopy, fulllist.GetData (i) );		     
            filteredlist.PutData ( datacopy );

        }

	}

	baseoffset = 0;

	//
	// Dirty the buttons
	//

	for ( int ib = 0; ib < NumLinksOnScreen(); ++ib ) {

		char name [128];
		sprintf ( name, "linksscreen_link %d", ib );
		EclDirtyButton ( name );

		if ( GetComputerScreen ()->SCREENTYPE == LINKSSCREENTYPE_PLAYERLINKS ) {

    		sprintf ( name, "linksscreen_deletelink %d", ib );
	    	EclDirtyButton ( name );

            char displaylink [128];
            sprintf ( displaylink, "linksscreen_showlink %d", ib );
            EclDirtyButton ( displaylink );

        }
        else {

		    sprintf ( name, "linksscreen_addlink %d", ib );
		    EclDirtyButton ( name );

        }

	}

    //
    // Update the scrollbar
    //

    ScrollBox *sb = ScrollBox::GetScrollBox("linksscreen_scroll");
    if ( sb ) sb->SetNumItems( filteredlist.Size() );

}

int LinksScreenInterface::NumLinksOnScreen ()
{

    int screenheight = app->GetOptions()->GetOptionValue( "graphics_screenheight" );
    int availablePixels = screenheight - 120 - 145;
    return availablePixels/15;

}

void LinksScreenInterface::Create ()
{

	if ( cs ) Create ( cs );
	else printf ( "LinksScreenInterface::Create, tried to create when GenericScreen==NULL\n" );

}

void LinksScreenInterface::Create ( ComputerScreen *newcs )
{

	UplinkAssert ( newcs );
	cs = newcs;

	if ( !IsVisible () ) {

		// Draw the screen titles

		EclRegisterButton ( 80, 60, 350, 25, GetComputerScreen ()->maintitle, "", "linksscreen_maintitle" );
		EclRegisterButtonCallbacks ( "linksscreen_maintitle", DrawMainTitle, NULL, NULL, NULL );
		EclRegisterButton ( 80, 80, 350, 20, GetComputerScreen ()->subtitle, "", "linksscreen_subtitle" );
		EclRegisterButtonCallbacks ( "linksscreen_subtitle", DrawSubTitle, NULL, NULL, NULL );

		// Create the links

        int numLinks = NumLinksOnScreen();

		for ( int li = 0; li < numLinks; ++li ) {

			char name [128];
			sprintf ( name, "linksscreen_link %d", li );
			EclRegisterButton ( 30, 145 + li * 15, SY(375), 14, "", "Connect to this computer", name );
			EclRegisterButtonCallbacks ( name, LinkDraw, LinkClick, LinkMouseDown, LinkMouseMove );

			if ( GetComputerScreen ()->SCREENTYPE == LINKSSCREENTYPE_PLAYERLINKS ) {

				// Give the player the option to delete any link

				sprintf ( name, "linksscreen_deletelink %d", li );
				EclRegisterButton ( 15, 145 + li * 15, 13, 13, "", "Delete this link", name );
				button_assignbitmaps ( name, "close.tif", "close_h.tif", "close_c.tif" );
				EclRegisterButtonCallbacks ( name, DeleteLinkDraw, DeleteLinkClick, button_click, button_highlight );

                // Give the player the option to display the link on the map

                char displaylink [128];
                sprintf ( displaylink, "linksscreen_showlink %d", li );
                EclRegisterButton ( 34 + SY(375), 145 + li * 15, 13, 13, "", "Toggle the visibility of this link on the map", displaylink );
                button_assignbitmaps ( displaylink, "link.tif", "link_h.tif", "link_c.tif" );
                EclRegisterButtonCallbacks ( displaylink, ShowLinkDraw, ShowLinkClick, button_click, button_highlight );
                
			}
			else {

				// Give the player the option to add this link to his collection

				sprintf ( name, "linksscreen_addlink %d", li );
				EclRegisterButton ( 15, 145 + li * 15, 13, 13, "", "Store this link", name );
				button_assignbitmaps ( name, "add.tif", "add_h.tif", "add_c.tif" );
				EclRegisterButtonCallbacks ( name, AddLinkDraw, AddLinkClick, button_click, button_highlight );

			}

		}
		
		//
		// Set the full list depending on the screen type
		//

		if ( GetComputerScreen ()->SCREENTYPE == LINKSSCREENTYPE_PLAYERLINKS ) {

			SetFullList ( &(game->GetWorld ()->GetPlayer ()->links) );

		}
		else if ( GetComputerScreen ()->SCREENTYPE == LINKSSCREENTYPE_ALLLINKS ) {

			LList <char *> alllinks;
			DArray <VLocation *> *locs = game->GetWorld ()->locations.ConvertToDArray ();

			for ( int i = 0; i < locs->Size (); ++i )
				if ( locs->ValidIndex (i) )
					if ( locs->GetData (i)->listed )
						alllinks.PutData ( locs->GetData (i)->ip );

			delete locs;

			SetFullList ( &alllinks );

		}
		else if ( GetComputerScreen ()->SCREENTYPE == LINKSSCREENTYPE_LOCALLINKS ) {

			char *companyname = GetComputerScreen ()->GetComputer ()->companyname;

			LList <char *> alllinks;
			DArray <VLocation *> *locs = game->GetWorld ()->locations.ConvertToDArray ();

			for ( int i = 0; i < locs->Size (); ++i )
				if ( locs->ValidIndex (i) )		
                    if ( locs->GetData (i)->GetOBJECTID () != OID_VLOCATIONSPECIAL )
					    alllinks.PutData ( locs->GetData (i)->ip );

			delete locs;

			SetFullList ( &alllinks );
			ApplyFilter ( companyname );
			SetFullList ();
	
		}

		if ( fulllist.Size () >= NumLinksOnScreen () ) {

			// Create the scrollbar
   
            ScrollBox::CreateScrollBox( "linksscreen_scroll", 50 + SY(375), 145, 15, 
                                        NumLinksOnScreen() * 15, fulllist.Size(), NumLinksOnScreen(), 0,  ScrollChange );

			EclRegisterButton ( 50 + SY(375), 125, 15, 15, "", "", "linksscreen_topright" );

			// Create the filter buttons

			EclRegisterButton ( 15, 150 + NumLinksOnScreen() * 15, 135, 15, "Filter", "Click here to apply the filter", "linksscreen_filter" );
			EclRegisterButtonCallback ( "linksscreen_filter", FilterClick );
			
			int width = (65 + SY(375)) - 155;
            if ( GetComputerScreen ()->nextpage != -1 ) width -= 50;

            EclRegisterButton ( 155, 150 + NumLinksOnScreen() * 15, width, 15, "", "Enter filter pattern here", "linksscreen_filtertext" );
			EclRegisterButtonCallbacks ( "linksscreen_filtertext", FilterDraw, NULL, button_click, button_highlight );
			EclMakeButtonEditable ( "linksscreen_filtertext" );

		}

		// Create the exit button

		if ( GetComputerScreen ()->nextpage != -1 ) {

            int width = (65 + SY(375)) - 205;
            EclRegisterButton ( 160 + width, 150 + NumLinksOnScreen() * 15, 45, 15, "Close", "Close this screen", "linksscreen_close" );
			EclRegisterButtonCallback ( "linksscreen_close", CloseClick );

		}

		// Create the title bars if this isn't the player links screen,
		// or if more than 13 links are on display

		if ( GetComputerScreen ()->SCREENTYPE != LINKSSCREENTYPE_PLAYERLINKS ||
			 fulllist.Size () >= NumLinksOnScreen() ) {

			int xpos = 15;
			int width = 150 - xpos;
			EclRegisterButton ( xpos, 125, width, 15, "IP Address", "Shows the IP address of the location", "linksscreen_iptitle" );
			EclRegisterButton ( 155, 125, (48 + SY(375)) - 155, 15, "Location name", "Shows the computer name at that location", "linksscreen_comptitle" );

		}


		ApplyFilter ( NULL );

	}

}

void LinksScreenInterface::Remove ()
{

	if ( IsVisible () ) {

		EclRemoveButton ( "linksscreen_maintitle" );
		EclRemoveButton ( "linksscreen_subtitle" );

		EclRemoveButton ( "linksscreen_iptitle" );
		EclRemoveButton ( "linksscreen_comptitle" );

		for ( int i = 0; i < NumLinksOnScreen(); ++i ) {

			char name [128];
			sprintf ( name, "linksscreen_link %d", i );
			EclRemoveButton ( name );

			if ( GetComputerScreen ()->SCREENTYPE == LINKSSCREENTYPE_PLAYERLINKS ) {

			    sprintf ( name, "linksscreen_deletelink %d", i );
			    EclRemoveButton ( name );

                char displaylink [128];
                sprintf ( displaylink, "linksscreen_showlink %d", i );
                EclRemoveButton ( displaylink );

            }
            else {

			    sprintf ( name, "linksscreen_addlink %d", i );
			    EclRemoveButton ( name );

            }

		}

        if ( ScrollBox::GetScrollBox( "linksscreen_scroll" ) != NULL ) {
		
            ScrollBox::RemoveScrollBox( "linksscreen_scroll" );

			EclRemoveButton ( "linksscreen_topright" );
			EclRemoveButton ( "linksscreen_filter" );
			EclRemoveButton ( "linksscreen_filtertext" );

		}

		EclRemoveButton ( "linksscreen_close" );

	}

}

bool LinksScreenInterface::IsVisible ()
{

	return ( EclGetButton ( "linksscreen_maintitle" ) != 0 );

}

bool LinksScreenInterface::ReturnKeyPressed ()
{

	Button *filterbutton = EclGetButton ( "linksscreen_filtertext" );
	
	if ( filterbutton ) {
		char *filter = filterbutton->caption;
		if ( filter [0] == '\x0' )  ApplyFilter ( NULL );
		else						ApplyFilter ( filter );
		return true;
	}

	return false;
}

void LinksScreenInterface::Update ()
{

}

LinksScreen *LinksScreenInterface::GetComputerScreen ()
{

	UplinkAssert (cs);
	return (LinksScreen *) cs;

}

int LinksScreenInterface::ScreenID ()
{

	return SCREEN_LINKSSCREEN;

}
