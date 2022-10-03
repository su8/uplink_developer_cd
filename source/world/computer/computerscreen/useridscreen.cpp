
#include "app/serialise.h"

#include "world/computer/computerscreen/useridscreen.h"


UserIDScreen::UserIDScreen ()
{

	nextpage = -1;
	difficulty = 1;
}

UserIDScreen::~UserIDScreen ()
{
}

void UserIDScreen::SetNextPage ( int newnextpage )
{
	
	nextpage = newnextpage;

}

void UserIDScreen::SetDifficulty ( int newdifficulty )
{

	difficulty = newdifficulty;

}

void UserIDScreen::Load  ( FILE *file )
{

	LoadID ( file );

	ComputerScreen::Load ( file );

	fread ( &nextpage, sizeof(nextpage), 1, file );
	fread ( &difficulty, sizeof(difficulty), 1, file );

	LoadID_END ( file );

}

void UserIDScreen::Save  ( FILE *file )
{

	SaveID ( file );

	ComputerScreen::Save ( file );

	fwrite ( &nextpage, sizeof(nextpage), 1, file );
	fwrite ( &difficulty, sizeof(difficulty), 1, file );

	SaveID_END ( file );

}

void UserIDScreen::Print ()
{
	
	printf ( "UserIDScreen : \n" );
	ComputerScreen::Print ();
	printf ( "\tNextPage:%d, Difficulty:%d\n", nextpage, difficulty );

}

char *UserIDScreen::GetID ()
{

	return "SCR_UID";

}

int UserIDScreen::GetOBJECTID ()
{

	return OID_USERIDSCREEN;

}
