
#include <stdio.h>

#include "slasher.h"
#include "findmemleaks.h"


static FindMemoryLeaks findmemoryleaks;


void SlInitialise ()
{

    findmemoryleaks.Initialise ();

}

void SlPrintMemoryLeaks ( char *filename )
{

    findmemoryleaks.PrintAllMemoryLeaks ( filename );

}
