// -*- tab-width:4 c-file-style:"cc-mode" -*-

#ifdef WIN32
#include <strstrea.h>
#else
#include <strstream.h>
#endif

#include "gucci.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/serialise.h"

#include "game/game.h"
#include "game/data/data.h"

#include "world/world.h"
#include "world/message.h"
#include "world/computer/gateway.h"
#include "world/computer/gatewaydef.h"


Gateway::Gateway ()
{

	id = 0;
	type = 0;
	newtype = -1;
	nuked = false;
	proximity = 0;

    sprintf ( name, " "  );
	sprintf ( modemtype, " " );
	memorysize = 0;

	hudupgrades = 0;

}

Gateway::~Gateway ()
{

    DeleteLListData ( &hardware );

}

void Gateway::GenerateNewID ()
{
	
	id++;

}

void Gateway::Nuke ()
{

	//
	// Reset software
	//

	databank.Format ();
	databank.SetSize ( PLAYER_START_MEMORYSIZE );

	GiveStartingSoftware ();

	hudupgrades = 0;

	//
	// Reset all of the hardware items
	//

	SetType ( PLAYER_START_GATEWAYTYPE );

	DeleteLListData ( &hardware );					
	hardware.Empty ();

	GiveStartingHardware ();

	//
	// Change the Gateway ID  (so any SeizeGatewayEvents will now fail)
	//

	GenerateNewID ();

	// 
	// Set it to be nuked, to remind us next time we log in
	//

	proximity = 0;
	nuked = true;

}

void Gateway::IncrementProximity ( int n )
{

	proximity += n;

}

void Gateway::DecrementProximity ( int n )
{
	
	proximity -= n;
    if ( proximity < 0 ) proximity = 0;	

}

void Gateway::SetType ( int newtype )
{

    GatewayDef *newgateway = game->GetWorld ()->GetGatewayDef (newtype, NULL);
    UplinkAssert (newgateway);
    strcpy ( name, newgateway->name );

	type = -1;

}

void Gateway::ExchangeGateway ( int newgatewaytype )
{

	newtype = newgatewaytype;

}

void Gateway::ExchangeGatewayComplete ()
{

	int oldtype = type;
    char oldName[256];
    sprintf ( oldName, name );

	SetType ( newtype );
	newtype = -1;
	nuked = false;
	GenerateNewID ();

	
	// 
	// Handle loss of hardware or software
	//

	GatewayDef *oldgateway = game->GetWorld ()->GetGatewayDef (oldtype, oldName);
	GatewayDef *newgateway = game->GetWorld ()->GetGatewayDef (type, name);
	UplinkAssert (oldgateway);
	UplinkAssert (newgateway);

	LList <char *> removedItems;


	//
	// NumCPUS - remove the slowest until we have the right number
	//

	while ( newgateway->maxcpus < GetNumCPUs () ) {
		
		int indexSlowest = -1;
		const ComputerUpgrade *upgradeSlowest = NULL;

		for ( int i = 0; i < hardware.Size (); ++i ) {
			
			const ComputerUpgrade *cu = GetHardwareUpgrade ( hardware.GetData (i) );
			UplinkAssert (cu);
			
			if ( cu->TYPE == GATEWAYUPGRADETYPE_CPU ) {
				if ( indexSlowest == -1 || upgradeSlowest == NULL ) {
					indexSlowest = i;
					upgradeSlowest = cu;
				}
				else if ( cu->data < upgradeSlowest->data ) {
					indexSlowest = i;
					upgradeSlowest = cu;
				}
			}

		}
	
		if ( indexSlowest != -1 ) {
			UplinkAssert (upgradeSlowest != NULL);
			removedItems.PutData ( upgradeSlowest->name );
			hardware.RemoveData (indexSlowest);
		}
		else {
			UplinkAbort ( "Gateway::ExchangeGatewayComplete, there should be a slowest CPU" );
		}				

	}
	

	//
	// Memory - remove the last N GQs until the right amount is reached
	//

	if ( newgateway->maxmemory * 8 < databank.GetSize () ) {

		for ( int i = newgateway->maxmemory * 8; i < memorysize; ++i ) 		
			databank.RemoveData (i);
		
		char *removedMemory = new char [64];
		sprintf ( removedMemory, "%dGq of memory", memorysize - (newgateway->maxmemory*8) );
		removedItems.PutData (removedMemory);

        SetMemorySize ( newgateway->maxmemory * 8 );

	}

    
	// 
	// Security - Remove N security items, first come first serve ;)
	//

    int numSecurity = GetNumSecurity ();

	while ( newgateway->maxsecurity < numSecurity ) {

		for ( int i = 0; i < hardware.Size (); ++i ) {

			const ComputerUpgrade *cu = GetHardwareUpgrade ( hardware.GetData (i) );
			UplinkAssert (cu);
			
			if ( cu->TYPE == GATEWAYUPGRADETYPE_SECURITY ) {
				removedItems.PutData ( cu->name );
				hardware.RemoveData (i);
			}

		}
		
        numSecurity = GetNumSecurity ();
	}
	

	//
	// Tell the player what we removed
	//

	if ( removedItems.Size () > 0 ) {

		ostrstream removedText;
		removedText << "Unfortunately, your new gateway does not have enough space to contain "
					   "all of your old hardware.  As such, we have been forced to remove the "
					   "following items from your new system :\n\n";

		for ( int i = 0; i < removedItems.Size (); ++i )
			removedText << removedItems.GetData(i) << "\n";

		removedText << '\x0';

		Message *msg = new Message ();
		msg->SetTo ( "PLAYER" );
		msg->SetFrom ( "Uplink Corporation" );
		msg->SetSubject ( "Removed hardware items" );
		msg->SetBody ( removedText.str () );
		msg->Send ();

		delete [] removedText.str ();

	}

}

void Gateway::SetModemType ( char *newmodem, int newmodemspeed )
{

	UplinkAssert ( strlen(newmodem) < 64 );
	strcpy ( modemtype, newmodem );

	modemspeed = newmodemspeed;

}

void Gateway::SetMemorySize ( int newsize )
{

	memorysize = newsize;
	databank.SetSize ( memorysize );

}

void Gateway::ChangeMemorySize ( int amount )
{

	SetMemorySize ( memorysize + amount );	

}

void Gateway::GiveHUDUpgrade ( char upgrade )
{

	hudupgrades = hudupgrades | upgrade;

}

bool Gateway::HasHUDUpgrade ( char upgrade )
{

	return ( hudupgrades & upgrade );
		
}

char Gateway::GetHudUpgrades ()
{

	return hudupgrades;

}

bool Gateway::HasAnyHUDUpgrade ()
{

	return ( hudupgrades != 0 );

}

void Gateway::GiveCPU ( char *CPUName )
{

	GatewayDef *gatewaydef = game->GetWorld ()->GetGatewayDef ( type, name );
	UplinkAssert (gatewaydef);

    char *cpucopy = new char [strlen(CPUName)+1];
    strcpy ( cpucopy, CPUName );
    
	if ( GetNumCPUs () < gatewaydef->maxcpus ) {

		hardware.PutData ( cpucopy );

	}
	else {

		// The player already has a full set of CPUs 
		// So replace his slowest

		int indexSlowest = -1;
		const ComputerUpgrade *upgradeSlowest = NULL;

		for ( int i = 0; i < hardware.Size (); ++i ) {
			
			const ComputerUpgrade *cu = GetHardwareUpgrade ( hardware.GetData (i) );
			UplinkAssert (cu);
			
			if ( cu->TYPE == GATEWAYUPGRADETYPE_CPU ) {
				if ( indexSlowest == -1 || upgradeSlowest == NULL ) {
					indexSlowest = i;
					upgradeSlowest = cu;
				}
				else if ( cu->data < upgradeSlowest->data ) {
					indexSlowest = i;
					upgradeSlowest = cu;
				}
			}

		}
	
		if ( indexSlowest != -1 ) {
			char *existing = hardware.GetData (indexSlowest);
            hardware.RemoveData (indexSlowest);
            delete [] existing;
			hardware.PutDataAtIndex ( cpucopy, indexSlowest);
		}
		else {
			UplinkAbort ( "Gateway::GiveCPU, there should be a slowest CPU" );
		}

	}

}

int Gateway::GetNumCPUs ()
{

	int numCPUs = 0;

	for ( int i = 0; i < hardware.Size (); ++i ) {

		const ComputerUpgrade *cu = GetHardwareUpgrade ( hardware.GetData (i) );
		UplinkAssert (cu);
	
		if ( cu->TYPE == GATEWAYUPGRADETYPE_CPU )
			++numCPUs;

	}

	return numCPUs;

}

int Gateway::GetCPUSpeed ()
{

	int speed = 0;

	for ( int i = 0; i < hardware.Size (); ++i ) {
		const ComputerUpgrade *cu = GetHardwareUpgrade ( hardware.GetData (i) );
		UplinkAssert (cu);
		if ( cu->TYPE == GATEWAYUPGRADETYPE_CPU )
			speed += cu->data;
	}

	return speed;

}

LList <char *> *Gateway::GetCPUs ()
{

	LList <char *> *list = new LList <char *> ();

	for ( int i = 0; i < hardware.Size (); ++i ) {

		const ComputerUpgrade *cu = GetHardwareUpgrade ( hardware.GetData (i) );
		UplinkAssert (cu);
	
		if ( cu->TYPE == GATEWAYUPGRADETYPE_CPU )
			list->PutData ( cu->name );

	}

	return list;

}

int Gateway::GetBandwidth ()
{

	GatewayDef *gatewaydef = game->GetWorld ()->GetGatewayDef ( type, name );
	UplinkAssert (gatewaydef);

	if ( modemspeed < gatewaydef->bandwidth )
		return modemspeed;

	else
		return gatewaydef->bandwidth;

}

int Gateway::GetNumSecurity ()
{

	int numsec = 0;

	for ( int i = 0; i < hardware.Size (); ++i ) {

		const ComputerUpgrade *cu = GetHardwareUpgrade ( hardware.GetData (i) );
		UplinkAssert (cu);
	
		if ( cu->TYPE == GATEWAYUPGRADETYPE_SECURITY )
			++numsec;

	}

	return numsec;

}

LList <char *> *Gateway::GetSecurity ()
{

	LList <char *> *list = new LList <char *> ();

	for ( int i = 0; i < hardware.Size (); ++i ) {

		const ComputerUpgrade *cu = GetHardwareUpgrade ( hardware.GetData (i) );
		UplinkAssert (cu);
	
		if ( cu->TYPE == GATEWAYUPGRADETYPE_SECURITY )
			list->PutData ( cu->name );

	}

	return list;

}

void Gateway::GiveStartingHardware ()
{

	SetType ( PLAYER_START_GATEWAYTYPE );

	// CPU

	GiveCPU ( PLAYER_START_CPUTYPE );
		
	// Modem

	char modemname [64];
	sprintf ( modemname, "Modem (%d Ghz)", PLAYER_START_MODEMSPEED );
	SetModemType  ( modemname, PLAYER_START_MODEMSPEED );
	
	// Memory

	SetMemorySize ( PLAYER_START_MEMORYSIZE );

}

void Gateway::GiveHardware ( char *newhardware )
{

	char *hwtitle = new char [strlen(newhardware)+1];
	strcpy ( hwtitle, newhardware );

	hardware.PutData ( hwtitle );

}

bool Gateway::IsHWInstalled ( char *name )
{

	for ( int i = 0; i < hardware.Size (); ++i ) 
		if ( hardware.GetData (i) )
			if ( strcmp ( hardware.GetData (i), name ) == 0 ) 
				return true;

	return false;

}

void Gateway::GiveStartingSoftware ()
{

	Data *copier = new Data ();
	copier->SetTitle ( "File_Copier" );
	copier->SetDetails ( DATATYPE_PROGRAM, 1, 0, 0, 1.0, SOFTWARETYPE_FILEUTIL );
	databank.PutData ( copier, 14 );

	Data *deleter = new Data ();
	deleter->SetTitle ( "File_Deleter" );
	deleter->SetDetails ( DATATYPE_PROGRAM, 1, 0, 0, 1.0, SOFTWARETYPE_FILEUTIL );
	databank.PutData ( deleter, 15 );

	Data *tutorial = new Data ();
	tutorial->SetTitle ( "Tutorial" );
	tutorial->SetDetails ( DATATYPE_PROGRAM, 4, 0, 0, 1.0, SOFTWARETYPE_OTHER );
	databank.PutData ( tutorial, 17 );

#ifdef TESTGAME
    Data *defrag = new Data ();
    defrag->SetTitle ( "Defrag" );
    defrag->SetDetails ( DATATYPE_PROGRAM, 2, 0, 0, 1.0, SOFTWARETYPE_FILEUTIL );
    databank.PutData ( defrag, 4 );
#endif

}

void Gateway::GiveAllHardware ()
{

	// Best gateway

	int bestGatewayIndex = game->GetWorld ()->gatewaydefs.Size () - 1;
	const GatewayDef *gatewayDef = game->GetWorld ()->GetGatewayDef (bestGatewayIndex, NULL);
	UplinkAssert (gatewayDef);

	SetType ( bestGatewayIndex );


	// Some big CPUs

	for ( int i = 0; i < 4; ++i )
		GiveCPU ( "CPU ( 150 Ghz )" );


	// Max all stats

	SetMemorySize ( gatewayDef->maxmemory * 8 );
	SetModemType ( "Fast fucker", gatewayDef->bandwidth );

}

void Gateway::GiveAllSoftware ()
{

	for ( int i = 0; i < NUM_STARTINGSOFTWAREUPGRADES; ++i ) {
		
		Data *prog = new Data ();
		prog->SetTitle ( SOFTWARE_UPGRADES [i].name );
		prog->SetDetails ( DATATYPE_PROGRAM, 
						   SOFTWARE_UPGRADES [i].size,
						   0, 0,
						   SOFTWARE_UPGRADES [i].data,
						   SOFTWARE_UPGRADES [i].TYPE );
		databank.PutData ( prog );
		
	}

	// Give all upgrades

	hudupgrades = 255;

}


void Gateway::Load  ( FILE *file )
{

	LoadID ( file );

	databank.Load ( file );

	LoadLList ( &hardware, file );
	
	fread ( &type,		 sizeof(type),		 1, file );
	fread ( &newtype,	 sizeof(newtype),	 1, file );
	fread ( &id,		 sizeof(id),		 1, file );
	fread ( &nuked,		 sizeof(nuked),		 1, file );
	fread ( &proximity,  sizeof(proximity),	 1, file );

	fread ( &modemspeed, sizeof(modemspeed), 1, file );
	fread ( &memorysize, sizeof(memorysize), 1, file );
	fread ( &modemtype,  sizeof(modemtype),  1, file );

	fread ( &hudupgrades, sizeof(hudupgrades), 1, file );

    if ( type == -1 )    fread ( name, sizeof(name), 1, file );
    else                 SetType ( type );

	LoadID_END ( file );

}

void Gateway::Save  ( FILE *file )
{

	SaveID ( file );

	databank.Save ( file );

	SaveLList ( &hardware, file );

	fwrite ( &type,		  sizeof(type),		  1, file );
	fwrite ( &newtype,	  sizeof(newtype),	  1, file );
	fwrite ( &id,		  sizeof(id),         1, file );
	fwrite ( &nuked,	  sizeof(nuked),      1, file );
	fwrite ( &proximity,  sizeof(proximity),  1, file );

	fwrite ( &modemspeed, sizeof(modemspeed), 1, file );
	fwrite ( &memorysize, sizeof(memorysize), 1, file );
	fwrite ( &modemtype,  sizeof(modemtype),  1, file );

	fwrite ( &hudupgrades, sizeof(hudupgrades), 1, file );

    if ( type == -1 )      fwrite ( name, sizeof(name), 1, file );

	SaveID_END ( file );

}

void Gateway::Print ()
{
	
	printf ( "Gateway : type=%d, newtype=%d, id=%d\n"
			 "nuked=%d, proximity=%d\n", type, id, nuked, proximity );
	printf ( "Upgrades : %c\n", hudupgrades );
    printf ( "Name : %s\n", name );

	databank.Print ();

	PrintLList ( &hardware );

	printf ( "Modem Speed = %d, MemorySize = %d\n", 
			 modemspeed, memorysize );

}


void Gateway::Update ()
{

	databank.Update ();

}


char *Gateway::GetID ()
{

	return "GATEWAY";

}
