
/*

   The new Slasher library
   for detecting memory leaks

    Include this file
    and #define SLASHER

  */

#ifndef _included_slasher_h
#define _included_slasher_h


#include "crtdbg.h"

#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)



void SlInitialise ();
void SlPrintMemoryLeaks ( char *filename );



#endif