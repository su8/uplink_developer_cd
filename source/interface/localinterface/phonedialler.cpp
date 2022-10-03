
#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>

#include <GL/glu.h> /*_glu_extention_library_*/

#include <string.h>
#include <stdio.h>

#include "soundgarden.h"
#include "gucci.h"
#include "redshirt.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/opengl_interface.h"
#include "app/opengl.h"

#include "interface/localinterface/phonedialler.h"

// === Initialise static variables here =======================================


char PhoneDialler::buttonnames [][9] = { "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };


// ======================================================================================

void PhoneDialler::Create ( int x, int y )
{

	if ( !IsVisible () ) {

		EclRegisterButton ( x - 10, y - 10, 120, 140, "", "dialler_bg" );
		EclRegisterButtonCallbacks ( "dialler_bg", BackgroundDraw, NULL, NULL, NULL );

		EclRegisterButton (  x + 0,  y + 0,  29,  29, "1", "one" );
		EclRegisterButton (  x + 36,  y + 0,  29,  29, "2", "two" );
		EclRegisterButton (  x + 72,  y + 0,  29,  29, "3", "three" );
		EclRegisterButton (  x + 0,  y + 36,  29,  29, "4", "four" );
		EclRegisterButton (  x + 36,  y + 36,  29,  29, "5", "five" );
		EclRegisterButton (  x + 72,  y + 36,  29,  29, "6", "six" );
		EclRegisterButton (  x + 0,  y + 72,  29,  29, "7", "seven" );
		EclRegisterButton (  x + 36,  y + 72,  29,  29, "8", "eight" );
		EclRegisterButton (  x + 72,  y + 72,  29,  29, "9", "nine" );

		button_assignbitmaps ( "one", "dialler/one.tif", "dialler/one_h.tif", "dialler/one_h.tif" );
		button_assignbitmaps ( "two", "dialler/two.tif", "dialler/two_h.tif", "dialler/two_h.tif" );
		button_assignbitmaps ( "three", "dialler/three.tif", "dialler/three_h.tif", "dialler/three_h.tif" );
		button_assignbitmaps ( "four", "dialler/four.tif", "dialler/four_h.tif", "dialler/four_h.tif" );
		button_assignbitmaps ( "five", "dialler/five.tif", "dialler/five_h.tif", "dialler/five_h.tif" );
		button_assignbitmaps ( "six", "dialler/six.tif", "dialler/six_h.tif", "dialler/six_h.tif" );
		button_assignbitmaps ( "seven", "dialler/seven.tif", "dialler/seven_h.tif", "dialler/seven_h.tif" );
		button_assignbitmaps ( "eight", "dialler/eight.tif", "dialler/eight_h.tif", "dialler/eight_h.tif" );
		button_assignbitmaps ( "nine", "dialler/nine.tif", "dialler/nine_h.tif", "dialler/nine_h.tif" );

		EclRegisterButton ( x, y + 108, 101, 15, "", "dialler_number" );

	}

}

void PhoneDialler::Remove ()
{

	if ( IsVisible () ) {

		EclRemoveButton ( "dialler_bg" );

		EclRemoveButton ( "one" );
		EclRemoveButton ( "two" );
		EclRemoveButton ( "three" );
		EclRemoveButton ( "four" );
		EclRemoveButton ( "five" );
		EclRemoveButton ( "six" );
		EclRemoveButton ( "seven" );
		EclRemoveButton ( "eight" );
		EclRemoveButton ( "nine" );

		EclRemoveButton ( "dialler_number" );

	}

}

void PhoneDialler::DialNumber ( char *number )
{

	if ( IsVisible () ) {

		for ( int i = 0; i < strlen (number); ++i ) {

			char ch = number [i];

			if ( ch >= '1' && ch <= '9' ) {

				EclHighlightButton ( buttonnames [ ch - '1' ] );
				
				char caption [64];
				sprintf ( caption, "%s", number );
				caption [i+1] = 0;
				UplinkAssert ( EclGetButton ( "dialler_number" ) );
				EclGetButton ( "dialler_number" )->SetCaption ( caption );

				display ();

				char filename [128];
				sprintf ( filename, "sounds/%d.wav", ch - '0' );
				SgPlaySound ( RsArchiveFileOpen ( filename ), filename, true );

				// Pause for a small time
				 int starttime = EclGetAccurateTime ();
				 while ( EclGetAccurateTime () - starttime < 110 ) { }

			}

		}

	}

}

bool PhoneDialler::IsVisible () 
{

	return ( EclGetButton ( "dialler_number" ) != 0 );

}
