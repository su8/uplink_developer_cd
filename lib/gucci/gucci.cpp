#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <stdio.h>

//#define USE_FTGL
#define USE_GLTT

#include <map>

#include "gucci.h"
#include "gucci_internal.h"
#include "tosser.h"

#ifdef USE_FTGL
#include <FTFace.h>
#include <FTGLBitmapFont.h>

static map<int, FTGLBitmapFont *> fonts;
#endif // USE_FTGL

#ifdef USE_GLTT
#include <FTFace.h>
#include <GLTTBitmapFont.h>

static std::map<int, GLTTBitmapFont *> fonts;
#endif // USE_GLTT

#include <math.h>

using namespace std;

static BTree<FTFace *> faces;

int  gci_defaultfont = HELVETICA_12;
bool gci_truetypeenabled = false;

void GciSetDefaultFont ( int STYLE )
{

	gci_defaultfont = STYLE;

}

void GciDrawText ( int x, int y, char *text )
{

	GciDrawText ( x, y, text, gci_defaultfont );

}

int GciTextWidth ( char *text )
{
	
	return GciTextWidth ( text, gci_defaultfont );

}

void GciEnableTrueTypeSupport ()
{

	gci_truetypeenabled = true;

}

void GciDisableTrueTypeSupport ()
{

	gci_truetypeenabled = false;

}

bool GciRegisterTrueTypeFont( const char *filename )
{
    return true;
}

bool GciUnregisterTrueTypeFont( const char *filename )
{
    return true;
}

static FTFace * RegisterFace( const char *familyname, const char *filename )
{
    if (faces.GetData(familyname))
        return faces.GetData(familyname);
    
    FTFace *face = new FTFace;
#ifdef USE_GLTT
    if (!face->open(filename)) {
        delete face;
        return NULL;
    }
#endif // USE_GLTT
#ifdef USE_FTGL
    if (!face->Open(filename)) {
        delete face;
        return NULL;
    }
#endif // USE_GLTT
    faces.PutData(familyname, face);
    return face;
}

static void UnregisterFace( const char *familyname )
{
    FTFace *face = faces.GetData(familyname);
    if (face) {
        faces.RemoveData(familyname);
        delete face;
    }
}


void GciDrawText ( int x, int y, char *text, int STYLE )
{

    if ( gci_truetypeenabled && fonts[STYLE] ) {
            
        // Use true type fonts
#ifdef USE_GLTT        
        GLTTBitmapFont *font = fonts[STYLE];
        font->output( x, y, text );
#endif // USE_GLTT
#ifdef USE_FTGL
	FTGLBitmapFont *font = fonts[STYLE];
        glRasterPos2f(x, y);
	font->render(text);
#endif // USE_FTGL

        }
    else {
      GciFallbackDrawText( x, y, text, STYLE );
    }
}

int GciTextWidth ( char *text, int STYLE )
{
  if (fonts[STYLE]) {
#ifdef USE_GLTT
        return fonts[STYLE]->getWidth(text);
#endif
#ifdef USE_FTGL
    float llx, lly, llz, urx, ury, urz;
    fonts[STYLE]->BBox( text, llx, lly, llz, urx, ury, urz );
    return (int)(fabs(llx - urx) + 0.5);
#endif
  }
    else 
    return GciFallbackTextWidth( text, STYLE );
}

bool GciLoadTrueTypeFont ( int index, char *fontname, char *filename, int size )
{
    
    if (gci_truetypeenabled) {
        FTFace *face = RegisterFace( fontname, filename );
        
        if (!face)
            return false;
        
        int pointSize = int (size * 72.0 / 96.0 + 0.5);
        
#ifdef USE_GLTT
        GLTTBitmapFont *font = new GLTTBitmapFont(face);

        if (!font->create(pointSize)) {
            delete font;
            return false;
        }
#endif // USE_GLTT
#ifdef USE_FTGL
        FTGLBitmapFont *font = new FTGLBitmapFont();
	font->Open(filename);
        if (!font->FaceSize(pointSize, 96)) {
            delete font;
            return false;
        }
#endif // USE_FTGL
            
        GciDeleteTrueTypeFont(index);
        fonts[index] = font;

        return true;
    }
    else {
        printf ( "GciLoadTrueTypeFont called, but truetypes are not enabled\n" );
        return false;
    }
}

void GciDeleteTrueTypeFont ( int index )
{
#ifdef USE_FTGL
  if (fonts[index]) 
    fonts[index]->Close();
#endif

#ifdef USE_GLTT
  // FTGL's destructor hangs, for some reason!
    delete fonts[index];
#endif
    fonts[index] = NULL;
}

void GciDeleteAllTrueTypeFonts ()
{
    // Delete all the frickin' fonts
#ifdef USE_GLTT    
    for (map<int, GLTTBitmapFont *>::iterator x = fonts.begin(); x != fonts.end(); x++)
        GciDeleteTrueTypeFont(x->first);
#else
    for (map<int, FTGLBitmapFont *>::iterator x = fonts.begin(); x != fonts.end(); x++)
        GciDeleteTrueTypeFont(x->first);
#endif
    // Delete all the frickin' faces

    DArray <FTFace *> *thefaces = faces.ConvertToDArray ();
    
    for ( int id = 0; id < thefaces->Size (); ++id ) {
        if ( thefaces->ValidIndex (id) ) {

            FTFace *theface = thefaces->GetData (id);
            delete theface;

        }
    }

    // Delete the structures

    delete thefaces;
    faces.Empty ();
    fonts.clear ();

}
