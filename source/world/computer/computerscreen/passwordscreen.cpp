// PasswordScreen.cpp: implementation of the PasswordScreen class.
//
//////////////////////////////////////////////////////////////////////

#include <string.h>
#include <stdio.h>

#include "gucci.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/serialise.h"

#include "world/computer/computerscreen/passwordscreen.h"


PasswordScreen::PasswordScreen()
{

	nextpage = -1;
	difficulty = 1;

}

PasswordScreen::~PasswordScreen()
{

}

void PasswordScreen::SetNextPage ( int newnextpage )
{

	nextpage = newnextpage;

}

void PasswordScreen::SetPassword ( char *newpassword )
{

	UplinkAssert ( strlen (newpassword) < SIZE_PASSWORDSCREEN_PASSWORD );
	strcpy ( password, newpassword );

}

void PasswordScreen::SetDifficulty ( int newdifficulty )
{

	difficulty = newdifficulty;

}

void PasswordScreen::Load  ( FILE *file )
{

	LoadID ( file );

	ComputerScreen::Load ( file );

	fread ( &nextpage, sizeof(nextpage), 1, file );
	fread ( password, sizeof(password), 1, file );
	fread ( &difficulty, sizeof(difficulty), 1, file );

	LoadID_END ( file );

}

void PasswordScreen::Save  ( FILE *file )
{

	SaveID ( file );

	ComputerScreen::Save ( file );

	fwrite ( &nextpage, sizeof(nextpage), 1, file );
	fwrite ( password, sizeof(password), 1, file );
	fwrite ( &difficulty, sizeof(difficulty), 1, file );

	SaveID_END ( file );

}

void PasswordScreen::Print ()
{

	printf ( "PasswordScreen : \n" );
	ComputerScreen::Print ();
	printf ( "Nextpage = %d, Password = %s, Difficulty = %d\n", nextpage, password, difficulty );

}
	
char *PasswordScreen::GetID ()
{

	return "SCR_PASS";

}

int PasswordScreen::GetOBJECTID ()
{

	return OID_PASSWORDSCREEN;

}

