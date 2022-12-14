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
 "@(#) $Id: SDL_ph_events.c,v 1.9 2002/03/28 16:20:10 slouken Exp $";
#endif

/* Handle the event stream, converting photon events into SDL events */

#define DISABLE_X11

#include <Ph.h>
#include <stdio.h>
#include <setjmp.h>
#include <photon/PkKeyDef.h>
#include <sys/time.h>

#include "SDL.h"
#include "SDL_syswm.h"
#include "SDL_sysevents.h"
#include "SDL_sysvideo.h"
#include "SDL_events_c.h"
#include "SDL_ph_video.h"
#include "SDL_ph_modes_c.h"
#include "SDL_ph_image_c.h"
#include "SDL_ph_events_c.h"


/* The translation tables from a photon keysym to a SDL keysym */
static SDLKey ODD_keymap[256];
static SDLKey MISC_keymap[0xFF + 1];
SDL_keysym *ph_TranslateKey(PhKeyEvent_t *key, SDL_keysym *keysym);

/* Check to see if this is a repeated key.
   (idea shamelessly lifted from GII -- thanks guys! :)
 */

#if 0
static int ph_KeyRepeat(_THIS, PhKeyEvent_t* keyevent)
{
//	PhEvent_t* peekevent;
	PhKeyEvent_t* keyEvent;
	int repeated;

	repeated = 0;
	switch (PhEventPeek( peekevent, EVENT_SIZE ))
	{
		case Ph_EVENT_MSG: {
			if(peekevent->type == Ph_EV_KEY)
			{
				keyEvent = PhGetData( peekevent );
				if ( !(Pk_KF_Key_Down & keyEvent->key_flags) &&
					(keyEvent->key_cap == keyevent->key_cap) &&
					(peekevent->timestamp == event->timestamp)	
				) {
					repeated = 1;
					 /* PhEventNext( peekevent, EVENT_SIZE ); */
				}				
			}
		}
		break;

		case -1: {
			perror( "PhEventPeek failed" );
		}
		break;

		default: /* no events pending */
	}
	return(repeated);
}
#endif

static int ph_WarpedMotion(_THIS, PhEvent_t *winEvent)
{
/*	PhPointerEvent_t *pointer = PhGetData( winEvent ); */
	PhRect_t *rect = PhGetRects( winEvent );

	int centre_x, centre_y;
	int dx, dy;
        short abs_x, abs_y;
	int posted;

	centre_x = SDL_VideoSurface->w / 2;
	centre_y = SDL_VideoSurface->h / 2;

	dx = rect->ul.x - centre_x;
	dy = rect->ul.y - centre_y;

	posted = SDL_PrivateMouseMotion( 0, 1, dx, dy );

	/* Move mouse cursor to middle of the window */
	PtGetAbsPosition( window, &abs_x, &abs_y );
	PhMoveCursorAbs( PhInputGroup(NULL),
			abs_x + centre_x,
			abs_y + centre_y );

	return(posted);
}

/* Control which motion flags the window has set, a flags value of -1 sets
 * MOTION_BUTTON and MOTION_NOBUTTON */

static void set_motion_sensitivity(_THIS, unsigned int flags)
{
	int rid, fields = Ph_EV_PTR_MOTION_BUTTON | Ph_EV_PTR_MOTION_NOBUTTON;
	PhRegion_t region;

	if( window )
	{
		rid = PtWidgetRid( window );
		if( rid != 0 && PhRegionQuery( rid, &region, NULL, NULL, 0 ) == 0 )
		{
			region.events_sense = ( region.events_sense & ~fields ) |
				( flags & fields );
			PhRegionChange( Ph_REGION_EV_SENSE, 0, &region,
					NULL, NULL );
		}
	}
}

/* Convert the photon button state value to an SDL value */
static Uint8 ph2sdl_mousebutton( unsigned short button_state )
{
	Uint8 mouse_button = 0;

	if( button_state & Ph_BUTTON_SELECT )
			mouse_button |= SDL_BUTTON_LEFT;
	if( button_state & Ph_BUTTON_MENU )
			mouse_button |= SDL_BUTTON_RIGHT;
	if( button_state & Ph_BUTTON_ADJUST )
			mouse_button |= SDL_BUTTON_MIDDLE;

	return( mouse_button );
}

static int ph_DispatchEvent(_THIS)
{
    int posted;
    PhRect_t* rect;
    PhPointerEvent_t* pointerEvent;
    PhKeyEvent_t* keyEvent;
    PhWindowEvent_t* winEvent;
    int i, buttons;
    SDL_Rect sdlrects[50]; 
	
    posted = 0;
	
    switch (event->type)
    {
        case Ph_EV_BOUNDARY:
        {
            if (event->subtype == Ph_EV_PTR_ENTER)
            {
                posted = SDL_PrivateAppActive(1, SDL_APPMOUSEFOCUS);
            }
            else if (event->subtype ==Ph_EV_PTR_LEAVE)
            {
                posted = SDL_PrivateAppActive(0, SDL_APPMOUSEFOCUS);	
            }
        }
        break;

        case Ph_EV_PTR_MOTION_BUTTON:
        case Ph_EV_PTR_MOTION_NOBUTTON:
        {
            if (SDL_VideoSurface)
            {
                pointerEvent = PhGetData(event);
                rect = PhGetRects(event);

                if (mouse_relative)
                {
                    posted = ph_WarpedMotion(this, event);
                }
                else
                {
                    posted = SDL_PrivateMouseMotion(0, 0, rect->ul.x, rect->ul.y);
                }
            }
        }
        break;

        case Ph_EV_BUT_PRESS:
        {
            pointerEvent = PhGetData( event );
            buttons = ph2sdl_mousebutton( pointerEvent->buttons );
            if (buttons != 0)
            {
                posted = SDL_PrivateMouseButton(SDL_PRESSED, buttons, 0, 0);
            }
        }
        break;

        case Ph_EV_BUT_RELEASE:
        {
            pointerEvent = PhGetData(event);
            buttons = ph2sdl_mousebutton(pointerEvent->buttons);
            if (event->subtype == Ph_EV_RELEASE_REAL && buttons != 0)
            {
                posted = SDL_PrivateMouseButton(SDL_RELEASED, buttons, 0, 0);
            }
            else if(event->subtype == Ph_EV_RELEASE_PHANTOM)
            {
                /* If the mouse is outside the window,
                 * only a phantom release event is sent, so
                 * check if the window doesn't have mouse focus.
                 * Not perfect, maybe checking the mouse button
                 * state for Ph_EV_BOUNDARY events would be
                 * better. */
                if ((SDL_GetAppState() & SDL_APPMOUSEFOCUS) == 0)
		{
                    posted = SDL_PrivateMouseButton(SDL_RELEASED, buttons, 0, 0);
                }
            }
        }
        break;

        case Ph_EV_WM:
        {
            winEvent = PhGetData(event);

            /* losing focus */
            if ((winEvent->event_f==Ph_WM_FOCUS) && (winEvent->event_state==Ph_WM_EVSTATE_FOCUSLOST))
            {
                set_motion_sensitivity(this, Ph_EV_PTR_MOTION_BUTTON);
                posted = SDL_PrivateAppActive(0, SDL_APPINPUTFOCUS);	

                /* Queue leaving fullscreen mode */
                switch_waiting = 0x01;
                switch_time = SDL_GetTicks() + 200;
            }
            /* gaining focus */
            else if ((winEvent->event_f==Ph_WM_FOCUS) && (winEvent->event_state==Ph_WM_EVSTATE_FOCUS))
            {
                set_motion_sensitivity(this, -1);
                posted = SDL_PrivateAppActive(1, SDL_APPINPUTFOCUS);
            }
            /* request to quit */
            else if (winEvent->event_f==Ph_WM_CLOSE)
            {
                posted = SDL_PrivateQuit();
            }
            /* request to resize */
            else if (winEvent->event_f==Ph_WM_RESIZE)
            {
                SDL_PrivateResize(winEvent->size.w, winEvent->size.h);
            }
            /* request to maximize */
            else if (winEvent->event_f==Ph_WM_MAX)
            {
                /* TODO: get screen resolution, set window pos to 0, 0 and resize it ! */
            }
        }
        break;

        /* window has been resized, moved or removed */
        case Ph_EV_EXPOSE:
        {
            if (SDL_VideoSurface)
            {
                rect = PhGetRects(event);

                for(i=0;i<event->num_rects;i++)
                {
                    sdlrects[i].x = rect[i].ul.x;
                    sdlrects[i].y = rect[i].ul.y;
                    sdlrects[i].w = rect[i].lr.x - rect[i].ul.x + 1;
                    sdlrects[i].h = rect[i].lr.y - rect[i].ul.y + 1;
                }

                this->UpdateRects(this, event->num_rects, sdlrects);
            }
        }
	break;

        case Ph_EV_KEY:
        {
            SDL_keysym keysym;

            posted = 0;

            keyEvent = PhGetData( event );

            if (Pk_KF_Key_Down & keyEvent->key_flags)
            {
                posted = SDL_PrivateKeyboard(SDL_PRESSED, ph_TranslateKey(keyEvent, &keysym));
            }
            else /* must be key release */
            {
                 /* Ignore repeated key release events */
                 /* if (! Pk_KF_Key_Repeat & keyEvent->key_flags ) */

                posted = SDL_PrivateKeyboard(SDL_RELEASED, ph_TranslateKey( keyEvent, &keysym));
            }
        }
        break;
    }

    return(posted);
}

/* perform a blocking read if no events available */
int ph_Pending(_THIS)
{
	/* Flush the display connection and look to see if events are queued */
	PgFlush();

     while( 1 )
      {   //note this is a non-blocking call
          switch( PhEventPeek( event, EVENT_SIZE ) )
           {
              case Ph_EVENT_MSG:
                 
                 return 1;
                 break;
              case -1:
                 perror( "PhEventNext failed" );
                 break;
              default:
             	  
                return 0;
       }
   }

	/* Oh well, nothing is ready .. */
	return(0);
}

/*
SAMPLE EVENT PUMP
=================
static void update( int block ){

    int ch,fl;
    PhKeyEvent_t *key;

    for( ;; ){

        if( block ){
            do{
                fl=PhEventNext( event,EVENT_SIZE );
            }while( fl!=Ph_EVENT_MSG );
            block=0;
        }else{
            do{
                fl=PhEventPeek( event,EVENT_SIZE );
                if( !fl ) return;
            }while( fl!=Ph_EVENT_MSG );
        }

        switch( event->type ){
        case Ph_EV_KEY:
            key=PhGetData( event );
            ch=key->key_cap;    // & 127;
            fl=key->key_flags;
            if( ch<32 || ch>127 ) break;
            if( fl & Pk_KF_Key_Down ){
                if( !(fl & Pk_KF_Key_Repeat) ){
                    if( queput-queget<QUE_SIZE ) keyque[ queput++ & QUE_MASK ]=ch;
                    keyMatrix[ch]=1;
                }
            }else{
                keyMatrix[ch]=0;
            }
            break;
        default:
            PtEventHandler( event );
        }
    }
}
*/

void ph_PumpEvents(_THIS)
{
	int pending;

	/* Keep processing pending events */
	pending = 0;
	while ( ph_Pending(this) ) {
		ph_DispatchEvent(this);
		++pending;
	}
	if ( switch_waiting ) {
		Uint32 now;

		now  = SDL_GetTicks();
#if 0
		if ( pending || !SDL_VideoSurface ) {
			/* Try again later... */
			if ( switch_waiting & SDL_FULLSCREEN ) {
				switch_time = now + 1500;
			} else {
				switch_time = now + 200;
			}
		} else if ( now >= switch_time ) {
			Uint32 go_fullscreen;

			go_fullscreen = switch_waiting & SDL_FULLSCREEN;
			switch_waiting = 0;
			if ( SDL_VideoSurface->flags & SDL_FULLSCREEN ) {
				if ( go_fullscreen ) {
					ph_EnterFullScreen(this);
				} else {
					ph_LeaveFullScreen(this);
				}
			}
			/* Handle focus in/out when grabbed */
/*
			if ( go_fullscreen ) {
				ph_GrabInputNoLock(this, this->input_grab);
			} else {
				ph_GrabInputNoLock(this, SDL_GRAB_OFF);
			}
*/
		}
#endif
	}
}

void ph_InitKeymap(void)
{
	int i;

	/* Odd keys used in international keyboards */
	for ( i=0; i<SDL_TABLESIZE(ODD_keymap); ++i )
		ODD_keymap[i] = SDLK_UNKNOWN;

	/* Map the miscellaneous keys */
	for ( i=0; i<SDL_TABLESIZE(MISC_keymap); ++i )
		MISC_keymap[i] = SDLK_UNKNOWN;

	MISC_keymap[Pk_BackSpace&0xFF] = SDLK_BACKSPACE;
	MISC_keymap[Pk_Tab&0xFF] = SDLK_TAB;
	MISC_keymap[Pk_Clear&0xFF] = SDLK_CLEAR;
	MISC_keymap[Pk_Return&0xFF] = SDLK_RETURN;
	MISC_keymap[Pk_Pause&0xFF] = SDLK_PAUSE;
	MISC_keymap[Pk_Escape&0xFF] = SDLK_ESCAPE;
	MISC_keymap[Pk_Delete&0xFF] = SDLK_DELETE;

	MISC_keymap[Pk_KP_0&0xFF] = SDLK_KP0;
	MISC_keymap[Pk_KP_1&0xFF] = SDLK_KP1;
	MISC_keymap[Pk_KP_2&0xFF] = SDLK_KP2;
	MISC_keymap[Pk_KP_3&0xFF] = SDLK_KP3;
	MISC_keymap[Pk_KP_4&0xFF] = SDLK_KP4;
	MISC_keymap[Pk_KP_5&0xFF] = SDLK_KP5;
	MISC_keymap[Pk_KP_6&0xFF] = SDLK_KP6;
	MISC_keymap[Pk_KP_7&0xFF] = SDLK_KP7;
	MISC_keymap[Pk_KP_8&0xFF] = SDLK_KP8;
	MISC_keymap[Pk_KP_9&0xFF] = SDLK_KP9;

	MISC_keymap[Pk_KP_Decimal&0xFF] = SDLK_KP_PERIOD;
	MISC_keymap[Pk_KP_Divide&0xFF] = SDLK_KP_DIVIDE;
	MISC_keymap[Pk_KP_Multiply&0xFF] = SDLK_KP_MULTIPLY;
	MISC_keymap[Pk_KP_Subtract&0xFF] = SDLK_KP_MINUS;
	MISC_keymap[Pk_KP_Add&0xFF] = SDLK_KP_PLUS;
	MISC_keymap[Pk_KP_Enter&0xFF] = SDLK_KP_ENTER;
	MISC_keymap[Pk_KP_Equal&0xFF] = SDLK_KP_EQUALS;

	MISC_keymap[Pk_Up&0xFF] = SDLK_UP;
	MISC_keymap[Pk_Down&0xFF] = SDLK_DOWN;
	MISC_keymap[Pk_Right&0xFF] = SDLK_RIGHT;
	MISC_keymap[Pk_Left&0xFF] = SDLK_LEFT;
	MISC_keymap[Pk_Insert&0xFF] = SDLK_INSERT;
	MISC_keymap[Pk_Home&0xFF] = SDLK_HOME;
	MISC_keymap[Pk_End&0xFF] = SDLK_END;
	MISC_keymap[Pk_Pg_Up&0xFF] = SDLK_PAGEUP;
	MISC_keymap[Pk_Pg_Down&0xFF] = SDLK_PAGEDOWN;

	MISC_keymap[Pk_F1&0xFF] = SDLK_F1;
	MISC_keymap[Pk_F2&0xFF] = SDLK_F2;
	MISC_keymap[Pk_F3&0xFF] = SDLK_F3;
	MISC_keymap[Pk_F4&0xFF] = SDLK_F4;
	MISC_keymap[Pk_F5&0xFF] = SDLK_F5;
	MISC_keymap[Pk_F6&0xFF] = SDLK_F6;
	MISC_keymap[Pk_F7&0xFF] = SDLK_F7;
	MISC_keymap[Pk_F8&0xFF] = SDLK_F8;
	MISC_keymap[Pk_F9&0xFF] = SDLK_F9;
	MISC_keymap[Pk_F10&0xFF] = SDLK_F10;
	MISC_keymap[Pk_F11&0xFF] = SDLK_F11;
	MISC_keymap[Pk_F12&0xFF] = SDLK_F12;
	MISC_keymap[Pk_F13&0xFF] = SDLK_F13;
	MISC_keymap[Pk_F14&0xFF] = SDLK_F14;
	MISC_keymap[Pk_F15&0xFF] = SDLK_F15;

	MISC_keymap[Pk_Num_Lock&0xFF] = SDLK_NUMLOCK;
	MISC_keymap[Pk_Caps_Lock&0xFF] = SDLK_CAPSLOCK;
	MISC_keymap[Pk_Scroll_Lock&0xFF] = SDLK_SCROLLOCK;
	MISC_keymap[Pk_Shift_R&0xFF] = SDLK_RSHIFT;
	MISC_keymap[Pk_Shift_L&0xFF] = SDLK_LSHIFT;
	MISC_keymap[Pk_Control_R&0xFF] = SDLK_RCTRL;
	MISC_keymap[Pk_Control_L&0xFF] = SDLK_LCTRL;
	MISC_keymap[Pk_Alt_R&0xFF] = SDLK_RALT;
	MISC_keymap[Pk_Alt_L&0xFF] = SDLK_LALT;
	MISC_keymap[Pk_Meta_R&0xFF] = SDLK_RMETA;
	MISC_keymap[Pk_Meta_L&0xFF] = SDLK_LMETA;
	MISC_keymap[Pk_Super_L&0xFF] = SDLK_LSUPER; /* Left "Windows" */
	MISC_keymap[Pk_Super_R&0xFF] = SDLK_RSUPER; /* Right "Windows */
	MISC_keymap[Pk_Mode_switch&0xFF] = SDLK_MODE; /* "Alt Gr" key */

	MISC_keymap[Pk_Help&0xFF] = SDLK_HELP;
	MISC_keymap[Pk_Print&0xFF] = SDLK_PRINT;
//	MISC_keymap[Pk_Sys_Req] = SDLK_SYSREQ;
	MISC_keymap[Pk_Break&0xFF] = SDLK_BREAK;
	MISC_keymap[Pk_Menu&0xFF] = SDLK_MENU;
	MISC_keymap[Pk_Hyper_R&0xFF] = SDLK_MENU;   /* Windows "Menu" key */
}

static unsigned long cap;

SDL_keysym *ph_TranslateKey(PhKeyEvent_t *key, SDL_keysym *keysym)
{
/*	
	'sym' is set to the value of the key with modifiers applied to it. 
	This member is valid only if Pk_KF_Sym_Valid is set in the key_flags.
	We will assume it is valid.
*/
	/* FIXME: This needs to check whether the cap & scancode is valid */
	cap = key->key_cap;
	switch (cap>>8) {
            case 0x00:  /* Latin 1 */
            case 0x01:  /* Latin 2 */
            case 0x02:  /* Latin 3 */
            case 0x03:  /* Latin 4 */
            case 0x04:  /* Katakana */
            case 0x05:  /* Arabic */
            case 0x06:  /* Cyrillic */
            case 0x07:  /* Greek */
            case 0x08:  /* Technical */
            case 0x0A:  /* Publishing */
            case 0x0C:  /* Hebrew */
            case 0x0D:  /* Thai */
                keysym->sym = (SDLKey)(cap&0xFF);
                /* Map capital letter syms to lowercase */
                if ((keysym->sym >= 'A')&&(keysym->sym <= 'Z'))
                    keysym->sym += ('a'-'A');
                break;
//            case 0xFE:
//                keysym->sym = ODD_keymap[sym&0xFF];
//                break;
            case 0xF0:
                keysym->sym = MISC_keymap[cap&0xFF];
                break;
            default:
/*                fprintf(stderr,"Photon: Unknown key_cap, cap = 0x%.4x\n", (unsigned int)cap); */
		keysym->sym = SDLK_UNKNOWN;                
                break;
	}
	keysym->scancode = key->key_scan;
	keysym->unicode = 0;
	if( SDL_TranslateUNICODE )
	{
		char utf8[MB_CUR_MAX];
		int utf8len;
		wchar_t unicode;

		utf8len = PhKeyToMb( utf8, key );
		if( utf8len > 0 )
		{
			utf8len = mbtowc( &unicode, utf8, utf8len );
			if( utf8len > 0)
				keysym->unicode = unicode;
		}
	}

	return (keysym);
}

void ph_InitOSKeymap(_THIS)
{
	ph_InitKeymap();
}

