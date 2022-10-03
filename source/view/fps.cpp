
/*
  
  Frame rate checker utility class source file
  By Christopher Delay
  
  */

#include <stdio.h>

#include "eclipse.h"

#include "view/fps.h"


FrameRate :: FrameRate ()
{
    
    numframesremaining = NUMFRAMES;
    fps = 0.0;
    
}

FrameRate :: ~FrameRate ()
{
}
 
void FrameRate :: Start ()
{
	
	starttime = EclGetAccurateTime ();
	numframesremaining = NUMFRAMES;
	fps = 0.0;

}

void FrameRate :: Tick ()
{
    
    if ( numframesremaining == 0 ) {
	
		int endtime = EclGetAccurateTime ();
		int time = endtime - starttime;
		fps = (float) NUMFRAMES / (float) time;
		
		numframesremaining = NUMFRAMES;
		starttime = EclGetAccurateTime ();
	
    }
    else
	
	--numframesremaining;
    
}
    
float FrameRate :: FPS ()
{
    
    return fps;
    
}

