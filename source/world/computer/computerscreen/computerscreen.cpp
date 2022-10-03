// ComputerScreen.cpp: implementation of the ComputerScreen class.
//
//////////////////////////////////////////////////////////////////////

#include "gucci.h"

#include "game/game.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/serialise.h"

#include "world/world.h"
#include "world/computer/computerscreen/computerscreen.h"



ComputerScreen::ComputerScreen()
{

	strcpy ( maintitle, " " );
	strcpy ( subtitle, " " );
	strcpy ( computer, " " );

}

ComputerScreen::~ComputerScreen()
{

}

void ComputerScreen::SetMainTitle ( char *newmaintitle )
{

	UplinkAssert ( strlen(newmaintitle) < SIZE_COMPUTERSCREEN_MAINTITLE );
	strcpy ( maintitle, newmaintitle );

}

void ComputerScreen::SetSubTitle ( char *newsubtitle )
{

	UplinkAssert ( strlen(newsubtitle) < SIZE_COMPUTERSCREEN_SUBTITLE );
	strcpy ( subtitle, newsubtitle );

}

void ComputerScreen::SetComputer ( char *newcomputer )
{

	UplinkAssert ( strlen(newcomputer) < SIZE_COMPUTER_NAME );
	strcpy ( computer, newcomputer );

}

Computer *ComputerScreen::GetComputer ()
{

	return game->GetWorld ()->GetComputer ( computer );

}

void ComputerScreen::Load  ( FILE *file )
{

	LoadID ( file );

	LoadDynamicString ( maintitle, SIZE_COMPUTERSCREEN_MAINTITLE, file );
	LoadDynamicString ( subtitle, SIZE_COMPUTERSCREEN_SUBTITLE, file );
	LoadDynamicString ( computer, SIZE_COMPUTER_NAME, file );

	LoadID_END ( file );

}

void ComputerScreen::Save  ( FILE *file )
{

	SaveID ( file );

	SaveDynamicString ( maintitle, file );
	SaveDynamicString ( subtitle, file );
	SaveDynamicString ( computer, file );

	SaveID_END ( file );

}

void ComputerScreen::Print ()
{

	printf ( "MainTitle : %s\n", maintitle );
	printf ( "SubTitle  : %s\n", subtitle );

	printf ( "Computer  : %s\n", computer );

}

void ComputerScreen::Update ()
{
}

char *ComputerScreen::GetID ()
{

	return "COMPSCR";

}

int ComputerScreen::GetOBJECTID ()
{
	
	return OID_UNKNOWN;

}

