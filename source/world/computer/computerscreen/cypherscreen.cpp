
#include "app/serialise.h"

#include "world/computer/computerscreen/cypherscreen.h"


CypherScreen::CypherScreen ()
{

	nextpage = -1;
	difficulty = 1;

}

CypherScreen::~CypherScreen ()
{
}

void CypherScreen::SetNextPage ( int newnextpage )
{
	
	nextpage = newnextpage;

}

void CypherScreen::SetDifficulty ( int newdifficulty )
{

	difficulty = newdifficulty;

}

void CypherScreen::Load  ( FILE *file )
{

	LoadID ( file );

	ComputerScreen::Load ( file );

	fread ( &nextpage, sizeof(nextpage), 1, file );
	fread ( &difficulty, sizeof(difficulty), 1, file );

	LoadID_END ( file );

}

void CypherScreen::Save  ( FILE *file )
{

	SaveID ( file );

	ComputerScreen::Save ( file );

	fwrite ( &nextpage, sizeof(nextpage), 1, file );
	fwrite ( &difficulty, sizeof(difficulty), 1, file );

	SaveID_END ( file );

}

void CypherScreen::Print ()
{
	
	printf ( "CypherScreen : \n" );
	ComputerScreen::Print ();
	printf ( "\tNextPage:%d, Difficulty:%d\n", nextpage, difficulty );

}

char *CypherScreen::GetID ()
{

	return "SCR_CYPH";

}

int CypherScreen::GetOBJECTID ()
{

	return OID_CYPHERSCREEN;

}
