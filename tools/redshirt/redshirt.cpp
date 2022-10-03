
/*

  Redshirt command line application
  For encrypting/decrypting data files

  */

#include <stdlib.h>

#include "redshirt.h"


void incorrectusage ()
{

	printf ( "Usage :\n" );
	printf ( "to Encrypt : redshirt -e filename\n" );
	printf ( "to Decrypt : redshirt -d filename\n" );
	printf ( "\n" );

	exit (1);

}

void encrypt ( char *filename )
{

	printf ( "Encrypting file : %s ... ", filename );

	bool success = RsEncryptFile ( filename );

	if ( success )  printf ( "done\n" );
	else			printf ( "failed\n" );

}

void decrypt ( char *filename )
{

	printf ( "Decrypting file : %s ... ", filename );

	bool success = RsDecryptFile ( filename );

	if ( success )  printf ( "done\n" );
	else			printf ( "failed\n" );

}

void main ( int argc, char *argv [] )
{

	//
	// Introduction
	//
		
	printf ( "RedShirt command line utility\n" );

	if ( argc < 3 ) incorrectusage ();

	//
	// Parse command line options
	//

	char option;
	char filename [128];

	sscanf ( argv [1], "-%c", &option );
	sscanf ( argv [2], "%s", filename );

	if ( option != 'e' && option != 'd' ) incorrectusage ();

	//
	// Act on options
	//

	bool success = false;

	if		( option == 'e' ) encrypt ( filename );
	else if ( option == 'd' ) decrypt ( filename );

}
