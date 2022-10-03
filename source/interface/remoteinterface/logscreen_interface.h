

#ifndef _included_logscreeninterface_h
#define _included_logscreeninterface_h


// ============================================================================

#include "interface/remoteinterface/remoteinterfacescreen.h"

class ComputerScreen;
class LogScreen;

// ============================================================================


class LogScreenInterface : public RemoteInterfaceScreen
{

protected:

	static int baseoffset;
	static int previousnumlogs;

protected:

	static void LogDraw			( Button *button, bool highlighted, bool clicked );

	static void ScrollUpClick	( Button *button );
	static void ScrollDownClick ( Button *button );

	static void LogClick		( Button *button );
	static void CloseClick		( Button *button );

public:

    bool EscapeKeyPressed ();

	void Create ();
	void Create ( ComputerScreen *newcs );
	void Remove ();
	void Update ();
	bool IsVisible ();

	int ScreenID ();

	LogScreen *GetComputerScreen ();

};


#endif
