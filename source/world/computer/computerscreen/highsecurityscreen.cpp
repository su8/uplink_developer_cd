

#include "app/serialise.h"

#include "world/computer/computerscreen/menuscreen.h"
#include "world/computer/computerscreen/highsecurityscreen.h"



HighSecurityScreen::HighSecurityScreen ()
{
	
	nextpage = -1;

}

HighSecurityScreen::~HighSecurityScreen ()
{

    DeleteLListData ( (LList <UplinkObject *> *) &systems );

}

void HighSecurityScreen::AddSystem ( char *name, int page )
{

	MenuScreenOption *mso = new MenuScreenOption ();
	mso->SetCaption ( name );
	mso->SetNextPage ( page );
	mso->SetSecurity ( 10 );
	systems.PutData ( mso );

}

void HighSecurityScreen::SetNextPage ( int newnextpage )
{

	nextpage = newnextpage;

}

void HighSecurityScreen::ResetSecurity ()
{

	for ( int i = 0; i < systems.Size (); ++i )
		systems.GetData(i)->security = 10;

}

void HighSecurityScreen::Load  ( FILE *file )
{

	LoadID ( file );

	ComputerScreen::Load ( file );

	LoadLList ( (LList <UplinkObject *> *) &systems, file );
	fread ( &nextpage, sizeof(nextpage), 1, file );

	LoadID_END ( file );

}

void HighSecurityScreen::Save  ( FILE *file )
{

	SaveID ( file );

	ComputerScreen::Save ( file );

	SaveLList ( (LList <UplinkObject *> *) &systems, file );
	fwrite ( &nextpage, sizeof(nextpage), 1, file );

	SaveID_END ( file );

}

void HighSecurityScreen::Print ()
{

	printf ( "HighSecurityScreen : \n" );
	ComputerScreen::Print ();
	PrintLList ( (LList <UplinkObject *> *) &systems );
	printf ( "nextpage = %d\n", nextpage );
	
}
	
char *HighSecurityScreen::GetID ()
{

	return "SCR_HI";

}

int HighSecurityScreen::GetOBJECTID ()
{

	return OID_HIGHSECURITYSCREEN;

}
