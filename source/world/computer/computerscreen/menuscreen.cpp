// MenuScreen.cpp: implementation of the MenuScreen class.
//
//////////////////////////////////////////////////////////////////////

#include "gucci.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/serialise.h"

#include "world/computer/computerscreen/menuscreen.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MenuScreen::MenuScreen()
{

}

MenuScreen::~MenuScreen()
{
    
    DeleteLListData ( (LList <UplinkObject *> *) &options );

}

void MenuScreen::AddOption ( char *caption, char *tooltip, int nextpage, int security, int index )
{

	MenuScreenOption *mso = new MenuScreenOption ();
	mso->SetCaption ( caption );
	mso->SetTooltip ( tooltip );
	mso->SetNextPage ( nextpage );
	mso->SetSecurity ( security );

	if ( index == -1 )
		options.PutData ( mso );

	else
		options.PutDataAtIndex ( mso, index );

}

int MenuScreen::NumOptions  ()
{

	return options.Size ();

}

char *MenuScreen::GetCaption ( int index )
{

	UplinkAssert ( options.ValidIndex ( index ) );

	return options.GetData (index)->caption;

}

char *MenuScreen::GetTooltip ( int index )
{

	UplinkAssert ( options.ValidIndex ( index ) );

	return options.GetData (index)->tooltip;
	
}

int MenuScreen::GetNextPage ( int index )
{

	UplinkAssert ( options.ValidIndex ( index ) );

	return options.GetData (index)->nextpage;

}

int MenuScreen::GetSecurity ( int index )
{

	UplinkAssert ( options.ValidIndex ( index ) );

	return options.GetData (index)->security;

}

void MenuScreen::Load ( FILE *file )
{

	LoadID ( file );

	ComputerScreen::Load ( file );

	LoadLList ( (LList <UplinkObject *> *) &options, file );

	LoadID_END ( file );

}

void MenuScreen::Save ( FILE *file )
{

	SaveID ( file );

	ComputerScreen::Save ( file );

	SaveLList ( (LList <UplinkObject *> *) &options, file );

	SaveID_END ( file );

}

void MenuScreen::Print ()
{

	printf ( "MenuScreen : \n" );
	
	ComputerScreen::Print ();
	
	PrintLList ( (LList <UplinkObject *> *) &options );

}

char *MenuScreen::GetID ()
{

	return "SCR_MENU";

}

int MenuScreen::GetOBJECTID ()
{

	return OID_MENUSCREEN;

}



// ============================================================================



MenuScreenOption::MenuScreenOption ()
{

	sprintf ( caption, "" );
	sprintf ( tooltip, "" );
	nextpage = -1;
	security = 10;

}

MenuScreenOption::~MenuScreenOption ()
{
}

void MenuScreenOption::SetCaption ( char *newcaption )
{

	UplinkAssert ( strlen(newcaption) < SIZE_MENUSCREENOPTION_CAPTION );
	strcpy ( caption, newcaption );

}

void MenuScreenOption::SetTooltip ( char *newtooltip )
{

	UplinkAssert ( strlen(newtooltip) < SIZE_MENUSCREENOPTION_TOOLTIP );
	strcpy ( tooltip, newtooltip );

}

void MenuScreenOption::SetNextPage ( int newnextpage )
{

	nextpage = newnextpage;

}

void MenuScreenOption::SetSecurity ( int newsecurity )
{

	security = newsecurity;

}

void MenuScreenOption::Load ( FILE *file )
{

	LoadID ( file );

	LoadDynamicString ( caption, SIZE_MENUSCREENOPTION_CAPTION, file );
	LoadDynamicString ( tooltip, SIZE_MENUSCREENOPTION_TOOLTIP, file );

	fread ( &nextpage, sizeof(nextpage), 1, file );
	fread ( &security, sizeof(security), 1, file );

	LoadID_END ( file );

}

void MenuScreenOption::Save ( FILE *file )
{

	SaveID ( file );

	SaveDynamicString ( caption, file );
	SaveDynamicString ( tooltip, file );

	fwrite ( &nextpage, sizeof(nextpage), 1, file );
	fwrite ( &security, sizeof(security), 1, file );

	SaveID_END ( file );

}

void MenuScreenOption::Print ()
{

	printf ( "MenuScreenOption : \n" );
	printf ( "\tNextPage = %d, Caption = %s\n", nextpage, caption );
	printf ( "\tTooltip = %s, security = %d\n", tooltip, security );

}

void MenuScreenOption::Update ()
{
}

char *MenuScreenOption::GetID ()
{

	return "MNUOPT";

}

int MenuScreenOption::GetOBJECTID ()
{

	return OID_MENUSCREENOPTION;

}

