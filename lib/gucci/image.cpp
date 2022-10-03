// Image.cpp: implementation of the Image class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#include "tiff.h"
#include "tiffio.h"

#include "image.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Image::Image()
{

	pixels = NULL;
	rgb_pixels = NULL;
}

Image::~Image()
{

	delete [] pixels;
	delete [] rgb_pixels;
}

void Image::LoadRAW ( char *filename, int sizex, int sizey )
{

	width = sizex;
	height = sizey;

	pixels = new unsigned char [sizex * sizey * 4];

	FILE *file = fopen ( filename, "rb" );

	if ( !file ) {
		printf ( "GCI Error : Failed to load image %s\n", filename );
		exit(255);
	}

	for ( int y = 0; y < sizey; ++y ) {
		for ( int x = 0; x < sizex; ++x ) {

			for ( int i = 0; i < 3; ++i ) {
				
				int thechar = fgetc ( file );
				pixels [((height-y-1) * width + x) * 4 + i] = (unsigned char) thechar;

			}

			pixels [((height-y-1) * width + x) * 4 + 3] = alpha;

		}
	}

	fclose ( file );

}

void Image::LoadTIF ( char *filename )
{

    char emsg[1024];		
	TIFFRGBAImage img;

    TIFF *tif = TIFFOpen(filename, "r");
    if ( !tif ) {
        printf ( "GUCCI Error - failed to load TIF %s\n", filename );
		CreateErrorBitmap ();
		return;
	}

    TIFFRGBAImageBegin (&img, tif, 0, emsg);
    int npixels = img.width * img.height;    
	uint32 *raster = new uint32 [npixels * sizeof(uint32)];
    
	TIFFRGBAImageGet(&img, raster, img.width, img.height);

	// Now convert the TIFF data into RAW data

	width = img.width;
	height = img.height;	
	pixels = (unsigned char *) raster;

	// Close down all those horrible TIF structures

    TIFFRGBAImageEnd(&img);
	TIFFClose ( tif );

}

void Image::SetAlpha ( float newalpha )
{

	alpha = (unsigned char) ( newalpha * 256.0 );
	
	if ( pixels ) {

		for ( int x = 0; x < width; ++x ) 
			for ( int y = 0; y < height; ++y )
				pixels [( y * width + x) * 4 + 3] = alpha;

	}

}

float Image::GetAlpha ()
{

	return float ( alpha / 256.0 );

}

int Image::Width ()
{
	
	return width;

}

int Image::Height ()
{

	return height;

}



void Image::FlipAroundH ()
{

	unsigned char *newpixels = new unsigned char [width * height * 4];
	
	for ( int y = 0; y < height; ++y ) {

		unsigned char *source = pixels + y * width * 4;
		unsigned char *dest = newpixels + (width * (height-1) * 4) - (y * width * 4);

		memcpy ( (void *) dest, (void *) source, width * 4 );

	}

	delete [] pixels;
	pixels = newpixels;

}

void Image::Scale ( int newwidth, int newheight )
{

	unsigned char *newpixels = new unsigned char [newwidth * newheight * 4];

/*
	for ( int x = 0; x < newwidth; ++x ) {
		for ( int y = 0; y < newheight; ++y ) {

			int scaleX = ((float) x / (float) newwidth) * width;
			int scaleY = ((float) y / (float) newheight) * height;

			for ( int i = 0; i < 4; ++i )
				newpixels [ (y * newwidth + x) * 4 + i ] = pixels [ (scaleY * width + scaleX) * 4 + i ];

		}
	}
*/
	int result = gluScaleImage ( GL_RGBA, width, height, GL_UNSIGNED_BYTE, pixels, newwidth, newheight, GL_UNSIGNED_BYTE, newpixels );
	char *resultc = (char *) gluErrorString ( (GLenum) result );

	delete [] pixels;
	delete [] rgb_pixels;

	rgb_pixels = NULL;
	pixels = newpixels;
	width = newwidth;
	height = newheight;
		
}

void Image::ScaleToOpenGL ()
{

	int twidth, theight;

	if		( width <= 32 )  twidth = 64;
	else if ( width <= 128 ) twidth = 128;
	else					 twidth = 256;

	if		( height <= 32 )  theight = 64;
	else if ( height <= 128 ) theight = 128;
	else					  theight = 256;

	Scale ( twidth, theight );

}

void Image::Draw ( int x, int y )
{

	glPushAttrib ( GL_ALL_ATTRIB_BITS );
	glDisable ( GL_BLEND );

	glRasterPos2i ( x, y + height );
	glDrawPixels ( width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels );

	glPopAttrib ();

}

unsigned char *Image::GetRGBPixels()
{
  if (rgb_pixels == NULL) {
    rgb_pixels = new unsigned char [ width * height * 3 ];
  
    for ( int x = 0; x < width; ++x ) 
      for ( int y = 0; y < height; ++y ) 
	for ( int i = 0; i < 3; ++i)
	  rgb_pixels[3 * (y * width + x) + i] = pixels[4 * (y * width + x) + i];
  }

  return rgb_pixels;
}

void Image::DrawBlend ( int x, int y )
{

	glPushAttrib ( GL_ALL_ATTRIB_BITS );

	glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable ( GL_BLEND );

	glRasterPos2i ( x, y + height );
	glDrawPixels ( width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels );

	glPopAttrib ();

}

void Image::CreateErrorBitmap ()
{

	width = 32;
	height = 32;
	uint32 *newimage = new uint32 [ width * height * sizeof(uint32) ];

	uint32 WHITE = 0xFFFFFFFF;
	uint32 BLACK = 0xFF000000;

	for ( int x = 0; x < width; ++x ) {
		for ( int y = 0; y < height; ++y ) {

			if ( x == 0 || y == 0 || x == width - 1 || y == height - 1 || x == y || x + y == width )
				newimage [y * width + x] = WHITE;

			else
				newimage [y * width + x] = BLACK;

		}
	}
	
	pixels = (unsigned char *) newimage;

}

char Image::GetPixelR ( int x, int y )
{

	if ( x < 0 || x >= width ||
	  	 y < 0 || y >= height )
		
		return -1;

	else

		return ( pixels [(y * width + x) * 4] );
		
}

char Image::GetPixelG ( int x, int y )
{

	if ( x < 0 || x >= width ||
	  	 y < 0 || y >= height )
		
		return -1;

	else

		return ( pixels [(y * width + x) * 4 + 1] );
		
		
}

char Image::GetPixelB ( int x, int y )
{

	if ( x < 0 || x >= width ||
	  	 y < 0 || y >= height )
		
		return -1;

	else

		return ( pixels [(y * width + x) * 4 + 2] );
		
		
}
