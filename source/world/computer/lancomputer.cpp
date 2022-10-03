
#include "app/uplinkobject.h"
#include "app/serialise.h"

#include "game/data/data.h"

#include "world/computer/lancomputer.h"
#include "world/generator/numbergenerator.h"



LanComputer::LanComputer () : Computer ()
{
}

LanComputer::~LanComputer ()
{

    DeleteDArrayData ( (DArray <UplinkObject *> *) &systems );
    DeleteDArrayData ( (DArray <UplinkObject *> *) &links );
    
}

int LanComputer::AddLanSystem ( LanComputerSystem *system )
{

    return systems.PutData ( system );

}

int LanComputer::AddLanSystem ( int TYPE, int x, int y, int security, int screenIndex, int dataScreenIndex )
{

    LanComputerSystem *system = new LanComputerSystem ();
    system->TYPE = TYPE;
    system->x = x;
    system->y = y;
	system->subnet = NumberGenerator::RandomNumber ( LAN_SUBNETRANGE );
	system->security = security;
	system->screenIndex = screenIndex;
    system->dataScreenIndex = dataScreenIndex;
    if ( TYPE == LANSYSTEM_ROUTER ) system->visible = LANSYSTEMVISIBLE_TYPE;
    return AddLanSystem ( system );

}

int LanComputer::AddLanLink ( int from, float fromX, float fromY,  
                               int to, float toX, float toY, int security )
{

    LanComputerLink *link = new LanComputerLink ();
    link->from = from;
    link->to = to;
    link->fromX = fromX;
    link->fromY = fromY;
    link->toX = toX;
    link->toY = toY;
	link->port = NumberGenerator::RandomNumber ( LAN_LINKPORTRANGE );
	link->security = security;
    return links.PutData ( link );

}

void LanComputer::Load  ( FILE *file )
{

    LoadID( file );

    Computer::Load ( file );

    LoadDArray ( (DArray <UplinkObject *> *) &systems, file );
    LoadDArray ( (DArray <UplinkObject *> *) &links, file );

    LoadID_END ( file );

}

void LanComputer::Save  ( FILE *file )
{

    SaveID ( file );

    Computer::Save ( file );

    SaveDArray ( (DArray <UplinkObject *> *) &systems, file );
    SaveDArray ( (DArray <UplinkObject *> *) &links, file );

    SaveID_END ( file );

}

void LanComputer::Print ()
{

    printf ( "LAN Computer\n" );

    PrintDArray ( (DArray <UplinkObject *> *) &systems );
    PrintDArray ( (DArray <UplinkObject *> *) &links );

    Computer::Print ();

}

void LanComputer::Update ()
{

    Computer::Update ();

}

char *LanComputer::GetID ()
{
    return "LANCOMP";
}

int LanComputer::GetOBJECTID ()
{
    return OID_LANCOMPUTER;
}

// ============================================================================


LanComputerSystem::LanComputerSystem ()
{
    TYPE = LANSYSTEM_NONE;
    x = y = 0;
    visible = LANSYSTEMVISIBLE_NONE;
	subnet = 0;
	security = 0;
	screenIndex = -1;
    dataScreenIndex = -1;
	data1 = data2 = data3 = -1;
}

LanComputerSystem::~LanComputerSystem ()
{
}

void LanComputerSystem::IncreaseVisibility( int newValue )
{

    if ( newValue > visible )
        visible = newValue;

}

void LanComputerSystem::Load ( FILE *file )
{

    LoadID ( file );

    fread ( &TYPE, sizeof(TYPE), 1, file );
    fread ( &x, sizeof(x), 1, file );
    fread ( &y, sizeof(y), 1, file );
    fread ( &visible, sizeof(visible), 1, file );
	fread ( &subnet, sizeof(subnet), 1, file );
	fread ( &security, sizeof(security), 1, file );
	fread ( &screenIndex, sizeof(screenIndex), 1, file );
    fread ( &dataScreenIndex, sizeof(dataScreenIndex), 1, file );
	
	fread ( &data1, sizeof(data1), 1, file );
	fread ( &data2, sizeof(data2), 1, file );
	fread ( &data3, sizeof(data3), 1, file );

	LoadDArray ( &validSubnets, file );

    LoadID_END ( file );

}

void LanComputerSystem::Save ( FILE *file )
{

    SaveID ( file );

    fwrite ( &TYPE, sizeof(TYPE), 1, file );
    fwrite ( &x, sizeof(x), 1, file );
    fwrite ( &y, sizeof(y), 1, file );
    fwrite ( &visible, sizeof(visible), 1, file );
	fwrite ( &subnet, sizeof(subnet), 1, file );
	fwrite ( &security, sizeof(security), 1, file );
	fwrite ( &screenIndex, sizeof(screenIndex), 1, file );
    fwrite ( &dataScreenIndex, sizeof(dataScreenIndex), 1, file );

	fwrite ( &data1, sizeof(data1), 1, file );
	fwrite ( &data2, sizeof(data2), 1, file );
	fwrite ( &data3, sizeof(data3), 1, file );

	SaveDArray ( &validSubnets, file );

    SaveID_END ( file );

}

void LanComputerSystem::Print ()
{

    printf ( "LanComputerSystem\n" );
    printf ( "TYPE = %d\n", TYPE );
    printf ( "Position = %d, %d\n", x, y );
    printf ( "Visible = %d\n", visible );
	printf ( "Subnet = %d\n", subnet );
	printf ( "Security = %d\n", security );
	printf ( "ScreenIndex = %d, DataScreenIndex = %d\n", screenIndex, dataScreenIndex );
	printf ( "Data1 = %d, Data2 = %d, Data3 = %d\n", data1, data2, data3 );
	
	PrintDArray ( &validSubnets );

}

char *LanComputerSystem::GetID ()
{
    return "LANSYST";
}

int LanComputerSystem::GetOBJECTID ()
{
    return OID_LANCOMPUTERSYSTEM;
}


// ============================================================================


LanComputerLink::LanComputerLink ()
{

    visible = LANLINKVISIBLE_NONE;
    from = -1;
    to = -1;
	port = 1000;
	security = 1;
    fromX = fromY = toX = toY = 0.0f;

}

LanComputerLink::~LanComputerLink ()
{
}

void LanComputerLink::IncreaseVisibility( int newValue )
{

    if ( newValue > visible )
        visible = newValue;

}

void LanComputerLink::Load  ( FILE *file )
{
    LoadID ( file );

    fread ( &visible, sizeof(visible), 1, file );

    fread ( &from, sizeof(from), 1, file );
    fread ( &to, sizeof(to), 1, file );
	fread ( &port, sizeof(port), 1, file );
	fread ( &security, sizeof(security), 1, file );

    fread ( &fromX, sizeof(fromX), 1, file );
    fread ( &fromY, sizeof(fromY), 1, file );
    fread ( &toX, sizeof(toX), 1, file );
    fread ( &toY, sizeof(toY), 1, file );

    LoadID_END ( file );

}

void LanComputerLink::Save  ( FILE *file )
{
    SaveID ( file );

    fwrite ( &visible, sizeof(visible), 1, file );

    fwrite ( &from, sizeof(from), 1, file );
    fwrite ( &to, sizeof(to), 1, file );
	fwrite ( &port, sizeof(port), 1, file );
	fwrite ( &security, sizeof(security), 1, file );

    fwrite ( &fromX, sizeof(fromX), 1, file );
    fwrite ( &fromY, sizeof(fromY), 1, file );
    fwrite ( &toX, sizeof(toX), 1, file );
    fwrite ( &toY, sizeof(toY), 1, file );

    SaveID_END ( file );

}

void LanComputerLink::Print ()
{

    printf ( "LanComputerLink\n" );

    printf ( "visible = %d\n", visible );
    printf ( "From %d to %d\n", from, to );
    printf ( "From XY = %f, %f\n", fromX, fromY );
    printf ( "to XY = %f, %f\n", toX, toY );
	printf ( "Port %d\n", port );
	printf ( "Security level %d\n", security );

}
	
char *LanComputerLink::GetID ()
{
    return "LANLINK";
}

int LanComputerLink::GetOBJECTID ()
{
    return OID_LANCOMPUTERLINK;
}

