
/*

  Gateway class object

	Represents the player's Gateway computer

  */


#ifndef _included_gateway_h
#define _included_gateway_h

// ============================================================================

#include "app/uplinkobject.h"

#include "world/computer/databank.h"
#include "world/computer/gatewaydef.h"


#define HUDUPGRADE_MAPSHOWSTRACE			1
#define HUDUPGRADE_CONNECTIONANALYSIS		2
#define HUDUPGRADE_IRCCLIENT                4
#define HUDUPGRADE_LANVIEW                  8

#define GATEWAYUPGRADETYPE_CPU				1
#define GATEWAYUPGRADETYPE_MODEM			2
#define GATEWAYUPGRADETYPE_COOLING			3
#define GATEWAYUPGRADETYPE_MEMORY			4
#define GATEWAYUPGRADETYPE_SECURITY			5
#define GATEWAYUPGRADETYPE_POWER			6

// ============================================================================


class Gateway : public UplinkObject
{

protected:

	char hudupgrades;						// Bitfield - see above

public:

	DataBank databank;

	int id;									// Unique ID number
	int type;								// Indexes into the list of Gateways
	int newtype;							// New type recently purchased, awaiting log out.  Or -1.
	bool nuked;								// Has the player blown it up?
	int proximity;							// Number of people nearby
	
    char name [SIZE_GATEWAY_NAME];          // Replaces type field (indexes are no longer fixed)

	char modemtype [64];

	int modemspeed;
	int memorysize;

	LList <char *> hardware;				// Names of all hardware items owned

public:

	Gateway ();
	~Gateway ();

	void GenerateNewID ();					// Call when gateway is changed
	void Nuke ();							// Call when gateway is nuked
	
	void IncrementProximity ( int n = 1 );				// Someone approaches
	void DecrementProximity ( int n = 1 );				// Someone leaves
	

	void SetType		  ( int newtype );
	void SetModemType     ( char *newmodem, int newmodemspeed );
	void SetMemorySize    ( int newsize );
	void ChangeMemorySize ( int amount );

	void ExchangeGateway ( int newgatewaytype );		// Call this when the gateway upgrade is ready
	void ExchangeGatewayComplete ();					// Does the actual upgrade

	void GiveHUDUpgrade ( char upgrade );
	bool HasHUDUpgrade  ( char upgrade );
	char GetHudUpgrades ();								// Returns the bitfield - be careful!
	bool HasAnyHUDUpgrade ();

	void GiveCPU ( char *CPUName );						
	int GetNumCPUs ();
	int GetCPUSpeed ();
	LList <char *> *GetCPUs ();

	int GetBandwidth ();								// Returns min ( modemspeed, gatewaydef.bandwidth )

	int GetNumSecurity ();
	LList <char *> *GetSecurity ();

	void GiveStartingHardware ();
	void GiveHardware  ( char *newhardware );
	bool IsHWInstalled ( char *name );

	void GiveStartingSoftware ();

	void GiveAllSoftware ();
	void GiveAllHardware ();

	// Common functions

	void Load  ( FILE *file );
	void Save  ( FILE *file );
	void Print ();
	void Update ();
	
	char *GetID ();

};



#endif