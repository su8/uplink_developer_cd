
/*

	ALL global objects / constants are defined here

  */

#ifndef _included_globals_h
#define _included_globals_h

//#ifdef _DEBUG
//#include "slasher.h"
//#endif


// ============================================================================
// Global defines


#define		VERSION_NUMBER		"1.31"
#define		VERSION_NAME		"RELEASE"
#define		SAVEFILE_VERSION	"SAV56"


#define		ALPHA				0.85							// Alpha value used for interface
#define		ALPHA_DISABLED		0.5								// Alpha value for disabled buttons

                                                                // SELECT ONE OF THESE THREE
//#define   FULLGAME										    // The finished, complete game for CD distribution
//#define   DEMOGAME											// Limit rating, stop story progression
#define     TESTGAME                                            // An internal build, with disabled start sequence

//#define	DOCLABRELEASE									    // This version designed for DOC labs
//#define	WAREZRELEASE                                        // Purity Control

#define	    CHEATMODES_ENABLED									// Eg all-links, all-software, password-bypass etc
//#define   CODECARD_ENABLED                                    // Do the code card lookup thingy at the start
//#define   DEBUGLOG_ENABLED                                    // Redirect std* to users/debug.log


#define     CODECARD_CHRIS                                      // Which code card to use
#define     CODECARD_TOM                                        // After V1.0 we use ALL THREE - to allow a unified
#define     CODECARD_MARK                                       // Patch that works on all copies


// ===========================================================================
// My very own, special assertion function
//

#ifdef TESTGAME

    #define  UplinkAssert(x)  {												\
	    if (!(x)) {															\
		    printf ( "\n"													\
				     "An Uplink Assertion Failure has occured\n"			\
				     "=======================================\n"			\
				     " Condition : %s\n"									\
				     " Location  : %s, line %d\n",							\
				     #x, __FILE__, __LINE__ );								\
		    GciRestoreScreenSize ();										\
		    abort (); 														\
	    }																	\
    }

#else

    #define  UplinkAssert(x)  {												\
	    if (!(x)) {															\
		    printf ( "\n"													\
				     "An Uplink Assertion Failure has occured\n"			\
				     "=======================================\n"			\
				     " Condition : %s\n"									\
				     " Location  : %s, line %d\n",							\
				     #x, __FILE__, __LINE__ );								\
            app->CoreDump ();                                               \
		    GciRestoreScreenSize ();										\
		    exit(255); 														\
	    }																	\
    }

#endif

//
// Abort - print message then bomb out
//

#ifdef DEBUGLOG_ENABLED

    #define  UplinkAbort(msg) {												\
	    printf ( "\n"														\
			     "Uplink has been forced to Abort\n"						\
			     "===============================\n"						\
			     " Message   : %s\n"										\
			     " Location  : %s, line %d\n",								\
			     msg, __FILE__, __LINE__ );									\
        app->CoreDump ();                                                   \
	    GciRestoreScreenSize ();											\
        exit(255);                                                          \
    }

#else

    #define  UplinkAbort(msg) {												\
	    printf ( "\n"														\
			     "Uplink has been forced to Abort\n"						\
			     "===============================\n"						\
			     " Message   : %s\n"										\
			     " Location  : %s, line %d\n",								\
			     msg, __FILE__, __LINE__ );									\
	    GciRestoreScreenSize ();											\
        abort ();                                                           \
    }

#endif

//
// Warning - print a message (when in DEBUG mode)
//

#ifdef _DEBUG
#define  UplinkWarning(msg) {											\
  printf ( "WARNING: %s ln %d :\n%s\n",									\
			__FILE__, __LINE__, msg );									\
}
#else
#define	 UplinkWarning(msg) 
#endif


#endif




