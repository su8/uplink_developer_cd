
#ifndef _included_dialler_h
#define _included_dialler_h


#include "interface/localinterface/localinterfacescreen.h"


class PhoneDialler : public LocalInterfaceScreen
{

protected:

	static char buttonnames [][9];

public:

	void Create ( int x, int y );
	void Remove ();
	bool IsVisible ();

	void DialNumber ( char *number );

};

#endif