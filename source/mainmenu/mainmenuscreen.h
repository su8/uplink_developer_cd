
/*

	Main Menu Screen base class

	All screens accessable via the main menu should be derived from this

  */

#ifndef _included_mainmenuscreen_h
#define _included_mainmenuscreen_h


class MainMenuScreen
{

public:

	MainMenuScreen();
	virtual ~MainMenuScreen();

	virtual void Create ();
	virtual void Remove ();
	virtual void Update ();
	virtual bool IsVisible ();

	virtual int  ScreenID ();			

	virtual bool ReturnKeyPressed ();				// Return true to block further usage of this return

};

#endif
