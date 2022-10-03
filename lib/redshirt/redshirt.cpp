// -*- tab-width:4 c-file-style:"cc-mode" -*-
#ifdef WIN32
#include <direct.h>
#include <io.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#endif

#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "bungle.h"
#include "tosser.h"
#include "redshirt.h"


const char marker [] = "REDSHIRT\x0";
const int SIZE_MARKER = 9;

const int SIZE_RSFILENAME = 256;

static char tempfilename [SIZE_RSFILENAME] = "";                         // Returned by RsArchiveFileOpen
static char rsapppath [SIZE_RSFILENAME] = "";
static char tempdir[SIZE_RSFILENAME] = "";
static bool rsInitialised = false;

#define BUFFER_SIZE 16384

typedef void filterFunc(unsigned char *, unsigned);
typedef bool headerFunc(FILE *);

// filterStream:
// transfers all the data from input to output via filterFunc(buffer, length)
bool filterStream(FILE *input, FILE *output, filterFunc *filterFunc)
{
	unsigned char buffer[BUFFER_SIZE];

	do {
		size_t bytesread = fread(buffer, 1, BUFFER_SIZE, input);
		if (bytesread <= 0)
			break;

		filterFunc(buffer, bytesread);

		if (fwrite(buffer, 1, bytesread, output) < bytesread) 
			return false;

	} while (true);

	return true;
};

void encryptBuffer(unsigned char *buffer, unsigned length)
{
	// Decrypt each byte in the buffer.
	
	for (unsigned i = 0; i < length; i++)
		buffer[i] += 128;
}

void decryptBuffer(unsigned char *buffer, unsigned int length)
{
	// Decrypt each byte in the buffer.
	
	for (unsigned i = 0; i < length; i++)
		buffer[i] -= 128;
}

bool RsFileExists  ( char *filename )
{

	FILE *file = fopen ( filename, "r" );

	bool success = file ? true : false;

	if ( success ) fclose ( file );

	return success;

}

bool readRsEncryptedHeader( FILE *input )
{
	char newmarker [SIZE_MARKER];
	fread ( newmarker, SIZE_MARKER, 1, input );
	return ( strcmp ( newmarker, marker ) == 0 );
}

bool writeRsEncryptedHeader(FILE *output)
{
	return fwrite ( marker, SIZE_MARKER, 1, output ) == 1;
}

bool noHeader( FILE * )
{
	// Useful for a no-operation
	return true;
}


bool RsFileEncrypted ( char *filename )
{
	
	FILE *input = fopen ( filename, "rb" );
	if ( !input ) return false;

	bool result = readRsEncryptedHeader(input);
	fclose ( input );

	return result;
}

// filterFile: 
//   takes input file (infile) and produces outfile
//
//   readHeader is a function which attempts to read the header
//   and returns true on success.
//
//   writeHeader is a function which attemps to write the header
//   and returns true on success.
//
//   filterFunc is a function which is used to transform the
//   bytes in the file.
//
//   returns true on success. Deletes outfile on failure.

bool filterFile( char *infile, char *outfile, 
				 headerFunc *readHeader, 
				 headerFunc *writeHeader, 
				 filterFunc *filter)
{
	FILE *input = fopen ( infile, "rb" );
	if ( !input ) 
		return false;

	// Read header from input file
	if (!readHeader(input)) {
		printf("redshirt: failed to read header!");
		fclose(input);
		return false;
	}

	FILE *output = fopen ( outfile, "wb" );
    if ( !output ) {
        fclose ( input );
        return false;
    }

	// Set completely unbuffered (we do our own)
	setvbuf(input, NULL, _IOFBF, 0);
	setvbuf(output, NULL, _IOFBF, 0);

	// Write header into output file
	if (!writeHeader(output)) {
		printf("redshirt: failed to write header!");
		fclose(input);
		fclose(output);
		remove(outfile);
		return false;
	}
		
	bool result = filterStream(input, output, filter);

	fclose ( input );
	fclose ( output );

	if (!result)
		remove(outfile);

	return result;
}

// filterFileInPlace
// process a file in place, temporary file = filename++ext
// see filterFile for description of readHeader, writeHeader and filterFunc

bool filterFileInPlace( char *filename, char *ext, 				 
						headerFunc *readHeader, 
						headerFunc *writeHeader, 
						filterFunc *filterFunc)
{
	char tempfilename [SIZE_RSFILENAME];
	sprintf ( tempfilename, "%s%s", filename, ext );

	if (filterFile(filename, tempfilename, readHeader, writeHeader, filterFunc)) {
		remove(filename);
		rename(tempfilename, filename);
		return true;
	}
	else {
		printf( "Redshirt ERROR : Failed to write output file\n" );
		return false;
	}
}

// RsEncryptFile: encrypts a file in-place
bool RsEncryptFile ( char *filename )
{
	return filterFileInPlace(filename, ".e", noHeader, writeRsEncryptedHeader, encryptBuffer);
}

// RsDecryptFile: decrypt a file in-place
bool RsDecryptFile ( char *filename )
{
	if ( !RsFileEncrypted ( filename ) ) {
		// Not encrypted, so nothing to do
		return true;
	}

	return filterFileInPlace(filename, ".d", readRsEncryptedHeader, noHeader, decryptBuffer);
};

const char *basename(const char *filename)
{
	// Return the basename of the file (minus any directory prefixes)
	const char *p = filename;
	do {
		// Search for the next forward- or backslash
		const char *slash = strchr(p, '/');
		if (slash == NULL)
			slash = strchr(p, '\\');

		// Didn't find one, quit out
		if (slash == NULL)
			break;
		
		p = slash + 1;
	} while (true);

	return p;
}

FILE *RsFileOpen ( char *filename, char *mode )
{

	if ( !RsFileExists ( filename ) ) return NULL;

	if ( !RsFileEncrypted ( filename ) ) {

		// Not encrypted, so just open it
		FILE *file = fopen ( filename, mode );
		return file;

	}
    else {

	    char dfilename [SIZE_RSFILENAME];
	    sprintf ( dfilename, "%s%s.d", tempdir, basename(filename) );
		
		if (filterFile(filename, dfilename, readRsEncryptedHeader, noHeader, decryptBuffer)) {
			// Open the result and return it
			FILE *result = fopen ( dfilename, mode );
			return result;
		}
		else {
			printf ( "Redshirt ERROR : Failed to write to output file\n" );
			return NULL;
		}
    }
}

void RsFileClose ( char *filename, FILE *file )
{

	fclose ( file );

	// Delete any decrypted versions

	char dfilename [SIZE_RSFILENAME];
	sprintf ( dfilename, "%s.d", filename );

	int result = remove ( dfilename );

}


bool RsLoadArchive ( char *filename )
{

	char fullfilename [SIZE_RSFILENAME];
	sprintf ( fullfilename, "%s%s", rsapppath, filename );

	FILE *archive = RsFileOpen ( fullfilename, "rb" );
	if ( !archive ) return false;

	bool result = BglOpenZipFile ( archive, rsapppath, filename );						// use the short filename as the id

	RsFileClose ( filename, archive );

	if ( result ) printf ( "Successfully loaded data archive %s\n", filename );
	else		  printf ( "Failed to load data archive %s\n", filename );

	return result;
}

FILE *RsArchiveFileOpen	( char *filename, char *mode )
{

	FILE *file = fopen ( RsArchiveFileOpen ( filename ), mode );

	return file;

}

char *RsArchiveFileOpen ( char *filename )
{

    //
    // WARNING
    // This function returns tempfilename - a static string
    // belonging to this library, which changes at every call
    //

	char fullfilename [SIZE_RSFILENAME];
	sprintf ( fullfilename, "%s%s", rsapppath, filename );

	//
	// Look to see if the file exists
	// If it does, just return its filename for use
	//

	if ( RsFileExists ( fullfilename ) ) {
    
        strcpy ( tempfilename, fullfilename );
        return tempfilename;
	
    }

	//
	// Now look in our data files for the file
	// Extract it to a temporary file in the same direcory if we find it
	//

	if ( BglFileLoaded ( fullfilename ) ) {

		char *extension = strrchr ( fullfilename, '.' );
		assert (extension);
		
		int attempt = 0;
		bool success = false;
		char targetfilename [SIZE_RSFILENAME];

		while ( !success && attempt < 3 ) {
			sprintf ( targetfilename, "%stemp%d%s", tempdir, attempt, extension );
			success = BglExtractFile ( fullfilename, targetfilename );
			++attempt;
		}
		
		if ( success ) {
			strcpy ( tempfilename, targetfilename );
			return tempfilename;
		}

	}

	//
	// Ooops - the file is nowhere to be found
	//

	printf ( "REDSHIRT : Failed to load file : %s\n", fullfilename );
	return NULL;

}

bool RsArchiveFileLoaded ( char *filename )
{

	char fullfilename [SIZE_RSFILENAME];
	sprintf ( fullfilename, "%s%s", rsapppath, filename );

	if ( RsFileExists ( fullfilename ) ) return true;
	if ( BglFileLoaded ( fullfilename ) ) return true;

	return false;

}

void RsArchiveFileClose	( char *filename, FILE *file )
{

	if ( file ) fclose ( file );

	char *extension = strrchr ( filename, '.' );
	assert (extension);
		
	char targetfilename [SIZE_RSFILENAME];
	int attempt = 0;

	while ( attempt < 3 ) {
		sprintf ( targetfilename, "%stemp%d%s", tempdir, attempt, extension );
		int result = remove ( targetfilename );
		++attempt;
	}

}

void RsCloseArchive	( char *filename )
{

	BglCloseZipFile ( filename );

}

bool RsMakeDirectory ( const char *dirname )
{
#ifdef WIN32
	return _mkdir ( dirname ) == 0;
#else
	return mkdir ( dirname, 0700 ) == 0;
#endif
}

void RsDeleteDirectory ( const char *dirname )
{

#ifdef WIN32
    _rmdir ( dirname );
#else
    rmdir( dirname );
#endif

}


void RsInitialise ( char *newapppath )
{
    sprintf ( rsapppath, "%s", newapppath );

    // create a temp directory

#ifdef WIN32
    sprintf ( tempdir, "%stemp/", rsapppath );
    RsMakeDirectory ( tempdir );
#else
	// Try in the current directory

    sprintf ( tempdir, "%stemp/", rsapppath );
    if (!RsMakeDirectory ( tempdir )) {
		// Try in /tmp

		strcpy(tempdir, "/tmp/uplink-XXXXXX");

		if (mkdtemp(tempdir) == NULL) {
			printf( "Failed to make temporary directory\n");
			abort();
		}

		strcat(tempdir, "/");
	}
#endif

	rsInitialised = true;

	// clean up, even on aborts
	atexit(RsCleanUp); 
}

void RsCleanUp ()
{
	if (!rsInitialised) 
		return;

	rsInitialised = false;

    // Delete all files in the temp directory

#ifdef WIN32

	char searchstring [SIZE_RSFILENAME];
	sprintf ( searchstring, "%s*.*", tempdir );

	_finddata_t thisfile;
	long fileindex = _findfirst ( searchstring, &thisfile );

	int exitmeplease = 0;

	while ( fileindex != -1 && !exitmeplease ) {

        if ( thisfile.attrib & _A_NORMAL || thisfile.attrib & _A_ARCH ) {

            char fullfilename [SIZE_RSFILENAME];
            sprintf ( fullfilename, "%s%s", tempdir, thisfile.name );
            int result = remove ( fullfilename );

        }

        exitmeplease = _findnext ( fileindex, &thisfile );

	}

#else
    {
	DIR *dir = opendir( tempdir );
	if (dir != NULL) {
	    struct dirent *entry = readdir ( dir );

	    while (entry != NULL) {
                
			char fullfilename [SIZE_RSFILENAME];
			sprintf ( fullfilename, "%s%s", tempdir, entry->d_name );
                
			remove ( fullfilename );

    		entry = readdir ( dir );
	    
	    }
	  
	    closedir( dir );
	}
    }
#endif

    // Delete the temp directory

    RsDeleteDirectory ( tempdir );     

}


DArray <char *> *RsListArchive ( char *path, char *filter )
{

    DArray <char *> *result = BglListFiles ( rsapppath, path, filter );

    // Strip rsapppath from every result

    for ( int i = 0; i < result->Size(); ++i ) {
        if ( result->ValidIndex(i) ) {

            char *thisResult = result->GetData(i);
            char *newResult = new char [strlen(thisResult) - strlen(rsapppath) + 1];
            strcpy ( newResult, thisResult + strlen(rsapppath) );
            result->PutData( newResult, i );

        }
    }

    return result;

}

