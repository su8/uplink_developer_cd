/*
 * gltt graphics library
 * Copyright (C) 1998-1999 Stephane Rehel
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __BottomText_h
#define __BottomText_h

#ifndef __GLTTboolean_h
#include "GLTTboolean.h"
#endif

class GLTTPixmapFont;
class FTFace;

/////////////////////////////////////////////////////////////////////////////

class BottomText
{
protected:
  int point_size;
  GLTTPixmapFont* font;
  const char* msg;
  float r, g, b;
  int height;
  int screen_width, screen_height;

public:
  BottomText( int _screen_width, int _screen_height );

  ~BottomText();

  GLTTboolean init( FTFace* face );

  void set( const char* _msg );
  void set( float _r, float _g, float _b );

  void draw( GLTTboolean center = GLTT_FALSE );

  int getHeight() const
    {
    return height;
    }
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __BottomText_h
