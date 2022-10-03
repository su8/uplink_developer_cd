#include <list>

#include <SDL/SDL.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "gucci.h"
#include "gucci_internal.h"

using namespace std;

static SDL_Surface *screen = NULL;

static unsigned _GciGetAccurateTime ();

static int sdlKeyToGucci(int key)
{
  switch (key) {
  case SDLK_F1: return GCI_KEY_F1;
  case SDLK_F12: return GCI_KEY_F12;
  default: return 1000 + key;
  }
}

static int sdlButtonToGucci(int button)
{
  switch (button) {
  case SDL_BUTTON_LEFT: return GCI_LEFT_BUTTON;
  case SDL_BUTTON_MIDDLE: return GCI_MIDDLE_BUTTON;
  case SDL_BUTTON_RIGHT: return GCI_RIGHT_BUTTON;
  default: return GCI_UNKNOWN;
  }
}

static int sdlMouseEventToGucci(int eventType)
{
  switch (eventType) {
  case SDL_MOUSEBUTTONDOWN: return GCI_DOWN;
  case SDL_MOUSEBUTTONUP: return GCI_UP;
  default: return GCI_UNKNOWN;
  }
}

void GciInitGraphics( const char *caption, int graphics_flags, int screenWidth, int screenHeight, 
		      int screenDepth, int screenRefresh, int argc, char *argv[] )
{
  bool debugging = (graphics_flags & GCI_DEBUGSTART) != 0;
  bool runFullScreen = (graphics_flags & GCI_FULLSCREEN) != 0;

  if ( debugging ) printf ( "Initialising SDL..." );
  if ((SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER)==-1)) { 
    printf("Could not initialize SDL: %s.\n", SDL_GetError());
    exit(-1);
  }

  int sdlFlags = 0;
  if (graphics_flags & GCI_DOUBLE) {
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    sdlFlags |= SDL_DOUBLEBUF;
  }

  const SDL_VideoInfo* info = NULL;
  info=SDL_GetVideoInfo();

  if (screenDepth == -1)
    screenDepth = info->vfmt->BitsPerPixel;

  if (graphics_flags & GCI_RGB) {
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
  }
  if (graphics_flags & GCI_FULLSCREEN) sdlFlags |= SDL_FULLSCREEN;

  if ( debugging ) printf ( "SDL is now opening a %dx%d window...", screenWidth, screenHeight );
  screen = SDL_SetVideoMode(screenWidth, screenHeight, screenDepth, sdlFlags | SDL_OPENGL);
  if (screen == NULL) {
    printf("Could not initialize SDL Video: %s.\n", SDL_GetError());
    exit(-1);
  }
  SDL_WM_SetCaption(caption, 0);
  
  SDL_EnableUNICODE(1);
  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,SDL_DEFAULT_REPEAT_INTERVAL);

  if ( debugging ) printf ( "done\n ");  
}


extern int gci_defaultfont; // The default Gucci style

void GciFallbackDrawText ( int x, int y, char *text, int STYLE )
{
  static bool inside = false;

  if (!inside) {
    inside = true;
    GciDrawText(x, y, text, gci_defaultfont);
    inside = false;
  }
  else {
    // We recursed back here, not even the
    // default font is supported!
    abort();
  }
}

int GciFallbackTextWidth ( char *text, int STYLE )
{
  static bool inside = false;

  if (!inside) {
    inside = true;
    int result = GciTextWidth(text, gci_defaultfont);
    inside = false;
  }
  else {
    // We recursed back here, not even the 
    // default font is supported!
    abort();
    return 0;
  }
}

#define GUCCI_FUNC(Function) \
static Gci##Function##FuncT *gci##Function##HandlerP = 0; \
void Gci##Function##Func( Gci##Function##FuncT * f) \
{ \
    gci##Function##HandlerP = f; \
}

GUCCI_FUNC(Display);
GUCCI_FUNC(Motion);
GUCCI_FUNC(PassiveMotion);
GUCCI_FUNC(Keyboard);
GUCCI_FUNC(Idle);
GUCCI_FUNC(Reshape);
GUCCI_FUNC(Mouse);
GUCCI_FUNC(Special);

static bool gciRedisplay = true;
static bool displayDamaged = false;
static bool finished = false;

bool GciLayerDamaged()
{
  return displayDamaged;
}

void GciSwapBuffers()
{
  SDL_GL_SwapBuffers();
}

void GciPostRedisplay()
{
  gciRedisplay = displayDamaged = true;
}

class Callback {
public:
  Callback(unsigned duration, GciCallbackT *callback, int value)
    : callback(callback), value(value)
  {
    expiryTime = SDL_GetTicks() + duration;
  };

  bool expired() {
    return SDL_GetTicks() > expiryTime;
  };

  void invoke();

private:
  GciCallbackT *callback;
  int value;
  unsigned expiryTime;
};

typedef list<Callback *> TimerList;
static TimerList timerEvents;

static void GciProcessTimerEvents()
{
  for (TimerList::iterator i = timerEvents.begin(); i != timerEvents.end(); ) {
    Callback *c = *i;
    if (c->expired()) {
      i = timerEvents.erase(i);
      c->invoke();
      delete c;
    }
    else
      i++;
  }
}

void Callback::invoke()
{
  (*callback)(value);
};

void GciTimerFunc(unsigned int millis, GciCallbackT *callback, int value)
{
  timerEvents.push_back(new Callback(millis, callback, value));
}

void GciMainLoop()
{
  finished = false;

  unsigned lastFrameTime = _GciGetAccurateTime();
  unsigned frameRate = 35;
  unsigned framePeriod = 1000 / frameRate;

  setpriority(PRIO_PROCESS, 0, 20);

  while (!finished) {
//     unsigned now = _GciGetAccurateTime();
//     unsigned frameDuration = now - lastFrameTime;

//     if (frameDuration > framePeriod) {
//       lastFrameTime = now;
      if (gciDisplayHandlerP)
	(*gciDisplayHandlerP)();
//     }

    SDL_Event event;

    /* Check for events */
    while(SDL_PollEvent(&event)){  /* Loop until there are no events left on the queue */
      switch(event.type){  /* Process the appropiate event type */
      case SDL_KEYDOWN: {
	int x, y;
	SDL_GetMouseState(&x, &y);
	if (event.key.keysym.unicode == 0) {
	  if (gciSpecialHandlerP) 
	    (*gciSpecialHandlerP)(sdlKeyToGucci(event.key.keysym.sym), x, y);
	}
	else {
	  if (gciKeyboardHandlerP) 
	    (*gciKeyboardHandlerP)(event.key.keysym.unicode & 0x7F, x, y);
	}
      }
	break;
      case SDL_MOUSEMOTION: 
	if (gciMotionHandlerP) {
	  /* I have a feeling we should only call this if 
	     a mouse button is depressed */
	  (*gciMotionHandlerP)(event.motion.x, event.motion.y);
	}
	if (gciPassiveMotionHandlerP) {
	  (*gciPassiveMotionHandlerP)(event.motion.x, event.motion.y);
	}
	break;
      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP:
	if (gciMouseHandlerP) {
	  (*gciMouseHandlerP)(sdlButtonToGucci(event.button.button),
			      sdlMouseEventToGucci(event.button.type),
			      event.button.x,
			      event.button.y);
	}
	break;
      case SDL_QUIT:
	finished = true;
	break;
      case SDL_VIDEOEXPOSE:
      case SDL_VIDEORESIZE:
	displayDamaged = true;
	break;
      default: /* Report an unhandled event */
	break;
      }
    }

    /* Let other processes run */
    SDL_Delay(1);

    GciProcessTimerEvents();

    if (gciIdleHandlerP)
      (*gciIdleHandlerP)();
  }
}

void GciRestoreScreenSize()
{
  SDL_QuitSubSystem(SDL_INIT_VIDEO);
  finished = true;
}

void GciStoreScreenSize ()
{
}

bool GciSetScreenSize ( int width, int height,
                        int bpp = -1, int refresh = -1 )
{
  return true;
}

void GciResizeGlut ( int width, int height )
{
}

void GciRestoreGlut()
{
}

static unsigned _GciGetAccurateTime ()
{

#ifdef WIN32

	//return 1000 * ( (float) clock () / (float) CLOCKS_PER_SEC );
	return GetTickCount ();

#else

	// Linux version

	static bool initted = false; 
	static struct timeval startTime; 
	struct timeval tv; 
  
	if (!initted) { 
		initted = true; 
		gettimeofday(&startTime, NULL); 
		return 0; 
	} 

	gettimeofday(&tv, NULL); 
  
	long diff_usec = tv.tv_usec - startTime.tv_usec; 
	if (diff_usec < 0) { 
		diff_usec += 1000000; 
		tv.tv_sec --; 
	} 
	long diff_sec = tv.tv_sec - startTime.tv_sec; 

	return 1000 * diff_sec + diff_usec / 1000; 

#endif

}
