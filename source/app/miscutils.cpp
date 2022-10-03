// -*- tab-width:4 c-file-style:"cc-mode" -*-

#ifdef WIN32
#include <windows.h>
#include <direct.h>
#include <io.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#endif

#include <GL/gl.h>

#include <GL/glu.h> /* glu extention library */

#include "redshirt.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gucci.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/miscutils.h"

#include "options/options.h"


char *GetFilePath ( const char *filename )
{

	UplinkAssert (filename);

    char *newstring = new char [strlen (filename) + 1];
    strcpy ( newstring, filename );
/*    
    char *p = newstring;
    
    while ( strchr ( p, '/' ) )	
		p = strchr ( p, '/' ) + 1;
	    
    if (p) *p = '\x0';
  */
    
    char     *p = strrchr ( newstring, '/' );
    if ( !p ) p = strrchr ( newstring, '\\' );

    if ( p ) *(p+1) = '\x0';
    else     strcpy ( newstring, "./" );

    return newstring;    
    
}

char *LowerCaseString ( const char *thestring )
{

	UplinkAssert (thestring);
	
	char *thecopy = new char [strlen(thestring)+1];
	strcpy ( thecopy, thestring );

	for ( char *p = thecopy; *p != '\x0'; ++p )
		if ( *p >= 'A' && *p <= 'Z' )
			*p += 'a' - 'A';

	return thecopy;

}

char *StripCarriageReturns ( const char *thestring )
{

    UplinkAssert (thestring);

    // Is there a cr?

    char *firstcr = strchr ( thestring, '\n' );

    if ( !firstcr ) {

        // No cr found - copy string and return it
     
        char *result = new char [strlen(thestring)+1];
        strcpy ( result, thestring );
        return result;

    }
    else {

        // Found a cr - so shorten string to that point

        int newlength = firstcr - thestring;
        char *result = new char [newlength+1];
        strncpy ( result, thestring, newlength );
        result [newlength] = '\x0';
        return result;

    }

}

void MakeDirectory ( const char *dirname )
{

#ifdef WIN32
  _mkdir ( dirname );
#else
  mkdir ( dirname, 0700 );
#endif

}


bool DoesFileExist ( const char *filename )
{

    return access(filename, 0) == 0;

}

DArray <char *> *ListDirectory  ( char *directory, char *filter )
{

    //
    // Start with a listing from Redshirt archives

    DArray <char *> *result = RsListArchive ( directory, filter );

    //
    // Now add on all files found locally

#ifdef WIN32

	char searchstring [128];
	sprintf ( searchstring, "%s*%s", directory, filter );

	_finddata_t thisfile;
	long fileindex = _findfirst ( searchstring, &thisfile );

	int exitmeplease = 0;

	while ( fileindex != -1 && !exitmeplease ) {

		char *newname = new char [256];
		sprintf ( newname, "%s%s", directory, thisfile.name );      
        result->PutData ( newname );
		exitmeplease = _findnext ( fileindex, &thisfile );

	}

#else

	char userdir [128];
	sprintf ( userdir, "%s", directory );
	DIR *dir = opendir( userdir );
	if (dir != NULL) {
	    struct dirent *entry = readdir ( dir );

	    while (entry != NULL) {
	    
		    char *p = strstr(entry->d_name, filter);
		    if ( p ) {
		        char *newname = new char [256];
		        sprintf ( newname, "%s%s", directory, entry->d_name );
		        result->PutData ( newname );
		    }
	        
		    entry = readdir ( dir );
	    
	    }
	  
	    closedir( dir );
	}
#endif

    //
    // Now make absolutely sure there are no duplicates

    for ( int i = 0; i < result->Size(); ++i ) {
        if ( result->ValidIndex(i) ) {

            for ( int j = i + 1; j < result->Size(); ++j ) {
                if ( result->ValidIndex(j) ) {

                    char *resultI = result->GetData(i);
                    char *resultJ = result->GetData(j);

                    if ( strcmp ( resultI, resultJ ) == 0 ) 
                        result->RemoveData( j );

                }
            }

        }
    }

    //
    // All done

    return result;

}

DArray <char *> *ListSubdirs ( char *directory )
{

    DArray <char *> *result = new DArray <char *> ();

    //
    // Now add on all files found locally

#ifdef WIN32

	_finddata_t thisfile;
	char filter[256];
	sprintf ( filter, "%s*.*", directory );
	long fileindex = _findfirst ( filter, &thisfile );

	int exitmeplease = 0;

	while ( fileindex != -1 && !exitmeplease ) {
		if ( thisfile.attrib & _A_SUBDIR &&
			 strcmp ( thisfile.name, "." ) != 0 &&
			 strcmp ( thisfile.name, ".." ) != 0 ) {		

			char *newname = new char [256];
			sprintf ( newname, "%s", thisfile.name );
			result->PutData ( newname );

		}
		exitmeplease = _findnext ( fileindex, &thisfile );
	}

#else

	DIR *dir = opendir(directory);
	if (dir != NULL) {
		for (struct dirent *d; (d = readdir(dir)) != NULL;) {
			char fullfilename[256];
			struct stat info;

			sprintf(fullfilename, "%s/%s", directory, d->d_name);
			
			if (stat(fullfilename, &info) == 0 
				&& S_ISDIR(info.st_mode) 
				&& d->d_name[0] != '.') { 
					char *newname = new char [strlen(d->d_name) + 1];
					strcpy(newname, d->d_name);
					result->PutData( newname );
			}
		}
		closedir(dir);
	}
#endif

	return result;

}

void SetColour ( char *colourName )
{

    if ( !app || 
         !app->GetOptions () || 
         !app->GetOptions()->GetColour( colourName ) ) {

        printf ( "SetColour WARNING : Failed to find colour %s\n", colourName );
        glColor3f ( 0.0, 0.0, 0.0 );
        return;

    }

    ColourOption *col = app->GetOptions ()->GetColour ( colourName );
    UplinkAssert (col);
    glColor3f ( col->r, col->g, col->b );

}


unsigned *getRetAddress(unsigned *mBP)
{
#ifdef WIN32
	unsigned *retAddr;

	__asm {
		mov eax, [mBP]
		mov eax, ss:[eax+4];
		mov [retAddr], eax
	}

	return retAddr;
#else
	unsigned **p = (unsigned **) mBP;
	return p[1];
#endif
}

void PrintStackTrace()
{

	// Get our frame pointer, chain upwards
	unsigned *framePtr;
    unsigned *previousFramePtr = NULL;

    
#ifdef WIN32
	__asm { 
		mov [framePtr], ebp
	}
#else
	asm (
	    "movl %%ebp, %0;"
	    :"=r"(framePtr)
	    );
#endif
	while(framePtr) {
		                
		printf("retAddress = %p\n", getRetAddress(framePtr));
		framePtr = *(unsigned **)framePtr;

	    // Frame pointer must be aligned on a
	    // DWORD boundary.  Bail if not so.
	    if ( (unsigned long) framePtr & 3 )   
		break;                    

        if ( framePtr <= previousFramePtr )
            break;

        // Can two DWORDs be read from the supposed frame address?          
#ifdef WIN32
        if ( IsBadWritePtr(framePtr, sizeof(PVOID)*2) ||
             IsBadReadPtr(framePtr, sizeof(PVOID)*2) )
            break;
#endif

        previousFramePtr = framePtr;
    
    }
	

}

