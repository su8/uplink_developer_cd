// -*- tab-width:4 c-file-style:"cc-mode" -*-

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gucci.h"

#include "app/app.h"
#include "app/globals.h"

#include "world/computer/gatewaydef.h"



GatewayDef::GatewayDef ()
{

	strcpy ( name, "UnNamed" );
	strcpy ( description, "UnDescribed ;)" );
    strcpy ( filename, "None" );
    strcpy ( thumbnail, "None" );
    index = 0;
	cost = 0;
	maxcpus = 0;
	maxmemory = 0;
	maxupgrades = 0;
	maxsecurity = 0;
	bandwidth = 0;

	width = 0;
	height = 0;
	modemX = modemY = 0;
	powerX = powerY = 0;

}

GatewayDef::~GatewayDef ()
{

    for ( int c = 0; c < cpus.Size (); ++c ) 
        if ( cpus.ValidIndex (c) )
            if ( cpus.GetData (c) )
                delete cpus.GetData (c);


    for ( int m = 0; m < memory.Size (); ++m ) 
        if ( memory.ValidIndex (m) )
            if ( memory.GetData (m) )
                delete memory.GetData (m);


    for ( int s = 0; s < security.Size (); ++s ) 
        if ( security.ValidIndex (s) )
            if ( security.GetData (s) )
                delete security.GetData (s);
        
}

void GatewayDef::LoadGatewayDefinition ( istream &thefile )
{

	// Basics

    thefile.getline ( name, SIZE_GATEWAY_NAME );

	thefile >> cost >> maxcpus >> maxmemory >> maxupgrades 
			>> maxsecurity >> bandwidth >> width >> height >> ws;

	thefile.getline ( description, SIZE_GATEWAY_DESCRIPTION );

	// CPU positions

	cpus.SetSize ( maxcpus );

	if ( maxcpus > 0 ) {

		char unused [8];
		thefile >> unused >> ws;

		for ( int ic = 0; ic < maxcpus; ++ic ) {
		
			GatewayDefLocation *gdl = new GatewayDefLocation ();
			thefile >> gdl->x >> gdl->y >> ws;
			cpus.PutData ( gdl, ic );
			
		}

	}

	// Memory positions

	if ( maxmemory > 0 ) {

		memory.SetSize ( maxmemory );
		
		char unused [8];
		thefile >> unused >> ws;

		for ( int i = 0; i < maxmemory; ++i ) {

			GatewayDefLocation *gdl = new GatewayDefLocation ();
			thefile >> gdl->x >> gdl->y >> ws;
			memory.PutData ( gdl, i );

		}

	}

	// Security positions

	if ( maxsecurity > 0 ) {

		security.SetSize ( maxsecurity );
		
		char unused [8];
		thefile >> unused >> ws;

		for ( int i = 0; i < maxsecurity; ++i ) {

			GatewayDefLocation *gdl = new GatewayDefLocation ();
			thefile >> gdl->x >> gdl->y >> ws;
			security.PutData ( gdl, i );

		}

	}

	// Modem position

	char unused [8];
	thefile >> unused >> modemX >> modemY >> ws;

	// Power position

	thefile >> unused >> powerX >> powerY >> ws;

}

void GatewayDef::SetName ( char *newname )
{

	UplinkAssert ( strlen (newname) < SIZE_GATEWAY_NAME );
	strcpy ( name, newname );
		
}

void GatewayDef::SetDescription ( char *newdescription )
{

	UplinkAssert ( strlen (newdescription) < SIZE_GATEWAY_DESCRIPTION );
	strcpy ( description, newdescription );

}

void GatewayDef::SetIndex ( int newindex )
{

	index = newindex;

}

void GatewayDef::SetFilename ( char *newfilename )
{

    UplinkAssert ( strlen ( newfilename) < SIZE_GATEWAY_FILENAME );
    strcpy ( filename, newfilename );

}

void GatewayDef::SetThumbnail ( char *newthumbnail )
{

    UplinkAssert ( strlen ( newthumbnail) < SIZE_GATEWAY_FILENAME );
    strcpy ( thumbnail, newthumbnail );

}

void GatewayDef::SetCost ( int newcost )
{
	
	cost = newcost;

}

void GatewayDef::SetMaxMemory ( int newmaxmemory )
{

	maxmemory = newmaxmemory;
		
}

void GatewayDef::SetMaxUpgrades ( int newmaxupgrades )
{
	
	maxupgrades = newmaxupgrades;

}

void GatewayDef::SetMaxSecurity ( int newmaxsecurity )
{
	
	maxsecurity = newmaxsecurity;

}

void GatewayDef::SetBandwidth ( int newbandwidth )
{

	bandwidth = newbandwidth;

}

void GatewayDef::SetSize ( int newW, int newH )
{

	width = newW;
	height = newH;

}

void GatewayDef::SetModemPosition ( int newX, int newY )
{

	modemX = newX;
	modemY = newY;

}

void GatewayDef::SetPowerPosition ( int newX, int newY )
{

	powerX = newX;
	powerY = newY;

}
