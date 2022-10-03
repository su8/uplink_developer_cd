
#include "app/serialise.h"

#include "interface/remoteinterface/remoteinterface.h"

#include "world/computer/computerscreen/linksscreen.h"



LinksScreen::LinksScreen ()
{
	
	nextpage = -1;
	SCREENTYPE = LINKSSCREENTYPE_NONE;

}

LinksScreen::~LinksScreen ()
{
}

void LinksScreen::SetNextPage ( int newnextpage )
{
	
	nextpage = newnextpage;

}

void LinksScreen::SetScreenType ( int newSCREEN_TYPE )
{
	
	SCREENTYPE = newSCREEN_TYPE;

}

void LinksScreen::Load ( FILE *file )
{

	LoadID ( file );

	ComputerScreen::Load ( file );

	fread ( &nextpage, sizeof(nextpage), 1, file );
	fread ( &SCREENTYPE, sizeof(SCREENTYPE), 1, file );

	LoadID_END ( file );

}

void LinksScreen::Save ( FILE *file )
{

	SaveID ( file );

	ComputerScreen::Save ( file );

	fwrite ( &nextpage, sizeof(nextpage), 1, file );
	fwrite ( &SCREENTYPE, sizeof(SCREENTYPE), 1, file );

	SaveID_END ( file );

}

void LinksScreen::Print ()
{
	
	printf ( "LinksScreen :\n" );
	ComputerScreen::Print ();
	printf ( "TYPE=%d, nextpage=%d\n", SCREENTYPE, nextpage );

}
	
char *LinksScreen::GetID ()
{
	
	return "SCR_LINK";

}

int LinksScreen::GetOBJECTID ()
{

	return OID_LINKSSCREEN;

}

