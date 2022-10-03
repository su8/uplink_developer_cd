

/*

  Change Gateway Event

	When this occurs, the player is told
	he must log out, then his gateway
	will upgrade to the new type

  */


#ifndef _included_changegatewayevent_h
#define _included_changegatewayevent_h


#include "world/scheduler/uplinkevent.h"



class ChangeGatewayEvent : public UplinkEvent
{

public:

	int newtype;

public:

	ChangeGatewayEvent ();
	~ChangeGatewayEvent ();

	void SetNewType ( int value );

	void Run ();
	void RunWarning ();

	char *GetShortString ();
	char *GetLongString ();

	// Common functions

	void Load  ( FILE *file );
	void Save  ( FILE *file );
	void Print ();
	
	char *GetID ();				
	int   GetOBJECTID ();		

};


#endif