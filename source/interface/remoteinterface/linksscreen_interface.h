

#ifndef _included_linksscreeninterface_h
#define _included_linksscreeninterface_h


// ============================================================================

#include "interface/remoteinterface/remoteinterfacescreen.h"

class ComputerScreen;
class LinksScreen; 

// ============================================================================


class LinksScreenInterface : public RemoteInterfaceScreen
{

protected:

	static int baseoffset;

	LList <char *> fulllist;
	LList <char *> filteredlist;

protected:

	static void LinkDraw ( Button *button, bool highlighted, bool clicked );
	static void LinkClick  ( Button *button );
	static void LinkMouseDown ( Button *button );
	static void LinkMouseMove ( Button *button );

	static void FilterDraw ( Button *button, bool highlighted, bool clicked );
	static void FilterClick ( Button *button );

	static void DeleteLinkDraw ( Button *button, bool highlighted, bool clicked );
	static void DeleteLinkClick ( Button *button );
	
	static void AddLinkDraw ( Button *button, bool highlighted, bool clicked );
	static void AddLinkClick ( Button *button );

    static void ShowLinkDraw ( Button *button, bool highlighted, bool clicked );
    static void ShowLinkClick ( Button *button );

	static void ScrollUpClick ( Button *button );
	static void ScrollDownClick ( Button *button );

	static void CloseClick ( Button *button );

    static void ScrollChange ( char *scrollname, int newValue );

    static int NumLinksOnScreen ();

public:

    LinksScreenInterface ();
    ~LinksScreenInterface ();

	void SetFullList ( LList <char *> *newfulllist );						            // The list is copied
	void SetFullList ();										            			// Uses current filtered list
	void ApplyFilter ( char *filter );

	void Create ();
	void Create ( ComputerScreen *newcs );
	void Remove ();
	void Update ();
	bool IsVisible ();

	int ScreenID ();

	bool ReturnKeyPressed ();

	LinksScreen *GetComputerScreen ();

};


#endif
