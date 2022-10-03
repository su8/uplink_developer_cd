
/*

  InterfaceScreen class object

	Part of the Interface sub-system
	Base class for defining a screen that will be in the interface
	 for example the InRealLife screen, or a password screen etc

  */


#ifndef _included_interfacescreen_h
#define _included_interfacescreen_h



class InterfaceScreen  
{

public:

	InterfaceScreen();
	virtual ~InterfaceScreen();

	virtual void Create ();
	virtual void Remove ();
	virtual void Update ();
	virtual bool IsVisible ();

	virtual int  ScreenID ();			

};


#endif 
