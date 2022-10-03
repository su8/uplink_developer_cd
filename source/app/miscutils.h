

/*
	Misc string/file utils

  */


#ifndef _included_miscutils_h
#define _included_miscutils_h

#include "tosser.h"

char *LowerCaseString       ( const char *thestring );
char *StripCarriageReturns  ( const char *thestring );               // Replaces first cr with \x0

char *GetFilePath           ( const char *filename );
void MakeDirectory          ( const char *directory );
bool DoesFileExist          ( const char *filename );

DArray <char *> *ListDirectory  ( char *directory, char *filter );
DArray <char *> *ListSubdirs ( char *directory );

void SetColour              ( char *colourName );                    // calls glColour3f

void PrintStackTrace();

#endif