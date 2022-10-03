
#include <windows.h>

#include "crtdbg.h"

 
 

class FindMemoryLeaks
{

      _CrtMemState m_checkpoint;

public:

      FindMemoryLeaks()
      {      
      };

   
      ~FindMemoryLeaks()
      {
      };

      void Initialise () {

//        _CrtMemCheckpoint(&m_checkpoint);

      }

      void PrintAllMemoryLeaks ( char *filename ) {

        
//            _CrtMemState checkpoint;
//            _CrtMemCheckpoint(&checkpoint);
 
//            _CrtMemState diff;
//            _CrtMemDifference(&diff, &m_checkpoint, &checkpoint);
                 
//            _CrtMemDumpStatistics(&diff);
//            _CrtMemDumpAllObjectsSince(&diff);

          OFSTRUCT ofstruct;
          HFILE file = OpenFile ( filename,
                                  &ofstruct,
                                  OF_CREATE );
                             
          _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
          _CrtSetReportFile(_CRT_WARN, (void *) file);
          
          _CrtDumpMemoryLeaks ();

          _lclose ( file );

      }

};

