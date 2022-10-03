/*
	Sound garden sound library
	SDL_mixer support
  */

#include "tosser.h"
#include "soundgarden.h"

#include <SDL_mixer.h>
#include <SDL.h>

#include <stdio.h>
#include <stdarg.h>

#ifndef WIN32
#include <unistd.h>
#endif // WIN32

static Mix_Music *currentmod = NULL;
static BTree <Mix_Chunk *> cache;					// Stores sounds already loaded

static pthread_t mikmod_update_thread;
static bool sg_thread_termination_requested = false;

static void SgDebugPrintf( const char *fmt, ... );
static void * SgUpdateMikModThread( void *arg );

static void SgPrintActualSoundSettings();
static void SgPrintSDLMixerVersionInfo();
static void SgSetProcessPriority();

static int playerVolume = SDL_MIX_MAXVOLUME / 2;

static bool SgInitialised = false;

// #define _DEBUG

void SgInitialise ()
{
  /* Initialise the SoundGarden library */

  SgPrintSDLMixerVersionInfo();

  if (SDL_Init(SDL_INIT_AUDIO) == -1) {
    printf("SDL_Init: %s\n", SDL_GetError());
    return;
  }

  int fragmentSize = 8192;

  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, fragmentSize)==-1) {
    printf("Mix_OpenAudio: %s\n", Mix_GetError());
    return;
  }

  /* Allocate an appropriate number of mixing channels */
  Mix_AllocateChannels(16);
  
  /* Reserve the first channel for sound effects */
  Mix_ReserveChannels(1);

  SgPrintActualSoundSettings();  
  
  SgSetProcessPriority();

  SgInitialised = true;
};

static void SgSetProcessPriority()
{
  /* Set uplink to be nice (so that sound daemons can process the sound) */
#ifndef WIN32
  nice(20);
#endif
}

static void SgPrintSDLMixerVersionInfo()
{
  SDL_version compile_version;
  const SDL_version *link_version;

  MIX_VERSION(&compile_version);
  printf("Compiled with SDL_mixer version: %d.%d.%d\n", 
	 compile_version.major,
	 compile_version.minor,
	 compile_version.patch);
  link_version=Mix_Linked_Version();
  printf("Running with SDL_mixer version: %d.%d.%d\n", 
	 link_version->major,
	 link_version->minor,
	 link_version->patch);
}

static void SgPrintActualSoundSettings()
{
  // get and print the audio format in use
  int numtimesopened, frequency, channels;
  Uint16 format;
  numtimesopened=Mix_QuerySpec(&frequency, &format, &channels);
  if(!numtimesopened) {
    printf("Mix_QuerySpec: %s\n",Mix_GetError());
  }
  else {
    char *format_str="Unknown";
    switch(format) {
    case AUDIO_U8: format_str="U8"; break;
    case AUDIO_S8: format_str="S8"; break;
    case AUDIO_U16LSB: format_str="U16LSB"; break;
    case AUDIO_S16LSB: format_str="S16LSB"; break;
    case AUDIO_U16MSB: format_str="U16MSB"; break;
    case AUDIO_S16MSB: format_str="S16MSB"; break;
    }
    printf("SDL_Mixer audio configuration: opened %d times, %dHz (%s) %d channels\n", 
	   numtimesopened, frequency, format_str, channels);
  }
}


void SgShutdown()
{
  if (!SgInitialised)
    return;

  SgStopMod();

  Mix_HaltChannel(-1); /* Stop playback on all channels */
  Mix_CloseAudio();
}

void SgPlaySound ( char *fullfilename, char *id, bool synchronised )
{
  if (!SgInitialised)
    return;

  Mix_Chunk *sample = NULL;
  char *sampleid = id ? id : fullfilename;

  if ( cache.LookupTree ( sampleid ) ) {

    // Sound sample already loaded into memory - start it playing
		
    sample = cache.GetData ( sampleid );
		
    if ( !sample ) {

      SgDebugPrintf ( "SoundGarden WARNING : Failed to load sound from cache : %s\n", fullfilename );
      cache.RemoveData ( sampleid );
      return;

    }

  }
  else {

    // Load sample and place into cache

    sample = Mix_LoadWAV( fullfilename );
    if ( !sample ) {
      SgDebugPrintf ( "SoundGarden WARNING : failed to load sound %s\n (%s)", fullfilename, Mix_GetError() );
      return;
    }

    cache.PutData ( sampleid, sample );

  }

  Mix_VolumeChunk(sample, MIX_MAX_VOLUME / 4 );
  Mix_PlayChannel(-1 /* First channel */, sample, 0 /* number of loops */);
}

void SgPlayMod ( char *fullfilename )
{
  if (!SgInitialised)
    return;

  SgStopMod ();
    
  if ( !currentmod ) {

    currentmod=Mix_LoadMUS(fullfilename);

    /* didn't work -> exit with errormsg. */

    if(currentmod==NULL){
      // SgDebugPrintf("SoundGarden WARNING : Failed to load music file %s\n (%s)\n", 
      //	  fullfilename, Mix_GetError());
      return;
    }

    /*  start playing the module: */
        
    Mix_PlayMusic(currentmod, 0 /* loops */);
    Mix_VolumeMusic(playerVolume);
        
    SgDebugPrintf ( "SoundGarden Playing Music : %s\n", fullfilename );

  }
}

void SgSetModVolume ( int newvolume )
{
  if (!SgInitialised)
    return;

  SgDebugPrintf ( "SoundGarden Music Volume: %d\n", newvolume );
    
  playerVolume = newvolume;
  Mix_VolumeMusic( newvolume );
}

void SgStopMod ()
{
  if (!SgInitialised)
    return;

  if (currentmod) {
    Mix_HaltMusic();
    Mix_FreeMusic(currentmod);  /* and free the module */
    currentmod = NULL;
  }
}

bool SgModFinished ()
{
  if (!SgInitialised)
    return false;

  return !currentmod || !Mix_PlayingMusic ();
}

static void SgDebugPrintf( const char *fmt, ... )
{
#ifdef _DEBUG
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt,ap);
    va_end(ap);
#endif
}

