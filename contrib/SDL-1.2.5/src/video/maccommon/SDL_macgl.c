/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997, 1998, 1999, 2000, 2001, 2002  Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: SDL_macgl.c,v 1.5 2002/08/19 17:58:08 slouken Exp $";
#endif

/* AGL implementation of SDL OpenGL support */

#include "SDL_error.h"
#include "SDL_lowvideo.h"
#include "SDL_macgl_c.h"


/* krat: adding OpenGL support */
int Mac_GL_Init(_THIS)
{
#ifdef HAVE_OPENGL
	AGLPixelFormat format;
   	int i = 0;
	GLint attributes [ 24 ]; /* 24 is max possible in this setup */
	GLboolean noerr;
   
	attributes[i++] = AGL_RGBA;

	if ( this->gl_config.double_buffer ) {
		attributes[i++] = AGL_DOUBLEBUFFER;
	}
	if ( this->gl_config.stereo ) {
		attributes[i++] = AGL_STEREO;
	}
	if ( this->gl_config.depth_size != 0 ) {
		attributes[i++] = AGL_DEPTH_SIZE;
		attributes[i++] = this->gl_config.depth_size;
	}	
	if ( this->gl_config.red_size   != 0 &&
	     this->gl_config.blue_size  != 0 &&
	     this->gl_config.green_size != 0 ) {

		attributes[i++] = AGL_RED_SIZE;
		attributes[i++] = this->gl_config.red_size;
		attributes[i++] = AGL_GREEN_SIZE;
		attributes[i++] = this->gl_config.green_size;
		attributes[i++] = AGL_BLUE_SIZE;
		attributes[i++] = this->gl_config.blue_size;
		attributes[i++] = AGL_ALPHA_SIZE;
		attributes[i++] = this->gl_config.alpha_size;
	}
	if ( this->gl_config.stencil_size != 0 ) {
		attributes[i++] = AGL_STENCIL_SIZE;
		attributes[i++] = this->gl_config.stencil_size;
	}
	if ( this->gl_config.accum_red_size   != 0 &&
	     this->gl_config.accum_blue_size  != 0 &&
	     this->gl_config.accum_green_size != 0 ) {
		
		attributes[i++] = AGL_ACCUM_RED_SIZE;
		attributes[i++] = this->gl_config.accum_red_size;
		attributes[i++] = AGL_ACCUM_GREEN_SIZE;
		attributes[i++] = this->gl_config.accum_green_size;
		attributes[i++] = AGL_ACCUM_BLUE_SIZE;
		attributes[i++] = this->gl_config.accum_blue_size;
		attributes[i++] = AGL_ACCUM_ALPHA_SIZE;
		attributes[i++] = this->gl_config.accum_alpha_size;
	}
	attributes[i++] = AGL_ALL_RENDERERS;
	attributes[i]	= AGL_NONE;

	format = aglChoosePixelFormat(NULL, 0, attributes);
	if ( format == NULL ) {
		SDL_SetError("Couldn't match OpenGL desired format");
		return(-1);
	}

	glContext = aglCreateContext(format, NULL);
	if ( glContext == NULL ) {
		SDL_SetError("Couldn't create OpenGL context");
		return(-1);
	}
	aglDestroyPixelFormat(format);

    #if  TARGET_API_MAC_CARBON
    noerr = aglSetDrawable(glContext, GetWindowPort(SDL_Window));
    #else
	noerr = aglSetDrawable(glContext, (AGLDrawable)SDL_Window);
    #endif
    
	if(!noerr) {
		SDL_SetError("Unable to bind GL context to window");
		return(-1);
	}
	return(0);
#else
	SDL_SetError("OpenGL support not configured");
	return(-1);
#endif
}

void Mac_GL_Quit(_THIS)
{
#ifdef HAVE_OPENGL
	if ( glContext != NULL ) {
		aglSetCurrentContext(NULL);
		aglSetDrawable(glContext, NULL);
		aglDestroyContext(glContext);		
		glContext = NULL;
	}
#endif
}

#ifdef HAVE_OPENGL

/* Make the current context active */
int Mac_GL_MakeCurrent(_THIS)
{
	int retval;

	retval = 0;
	if( ! aglSetCurrentContext(glContext) ) {
		SDL_SetError("Unable to make GL context current");
		retval = -1;
	}
	return(retval);
}

void Mac_GL_SwapBuffers(_THIS)
{
	aglSwapBuffers(glContext);
}

#endif /* HAVE_OPENGL */

