// VLocation.cpp: implementation of the VLocation class.
//
//////////////////////////////////////////////////////////////////////

#include "gucci.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/serialise.h"

#include "game/game.h"

#include "world/world.h"
#include "world/vlocation.h"
#include "world/computer/computer.h"
 

VLocation::VLocation()
{
	
	sprintf ( ip, "0.0.0.0" );
	sprintf ( computer, "" );

	x = y = 0;
	listed = true;
    displayed = true;

}

VLocation::~VLocation()
{
}

void VLocation::SetIP ( char *newip )
{

	UplinkAssert ( strlen (newip) < SIZE_VLOCATION_IP );
	strcpy ( ip, newip );

}

void VLocation::SetComputer ( char *newcomputer )
{

	UplinkAssert ( strlen (newcomputer) < SIZE_COMPUTER_NAME );
	strcpy ( computer, newcomputer );

//	UplinkAssert ( game->GetWorld ()->GetComputer ( computer ) );

}

void VLocation::SetPLocation ( int newx, int newy )
{

	UplinkAssert ( newx >= 0 && newx < VIRTUAL_WIDTH );
	UplinkAssert ( newy >= 0 && newy < VIRTUAL_HEIGHT );

	x = newx;
	y = newy;

}

void VLocation::SetListed ( bool newlisted )
{

	listed = newlisted;

}

void VLocation::SetDisplayed ( bool newdisplayed )
{

    displayed = newdisplayed;

}

Computer *VLocation::GetComputer ()
{
/*
	Computer *comp = game->GetWorld ()->GetComputer ( computer );
	
	if ( comp )
		return comp;

	else
		return NULL;
*/


	BTree <Computer *> *tree = &(game->GetWorld()->computers);
    
    /*  Remember there is no rule that states computer names have to be unique.
     *  eg "CompanyName Access Terminal" - there can be many.
     *  We must now continue searching until we find a computer that matches
     *  both the name and the IP of this VLocation.
     *
     *  Also remember that this might be a Dialup Number - in which case its IP
     * (telephone number) will definately NOT match the IP of the computer.
     *
     */
     
    while(tree)
    {
        tree = tree->LookupTree( computer );

        if( tree )
        {
            Computer *comp = tree->data;
            if( comp && strcmp( comp->ip, ip ) == 0 ) 
            {
                return comp;
            }
            else if( comp && 
                     comp->TYPE == COMPUTER_TYPE_LAN && 
                     GetOBJECTID() == OID_VLOCATIONSPECIAL )
            {
                return comp;
            }
            else 
            {
                // Copies with identical names will always be on the left branch
                tree = tree->Left();
            }
        }
    }

    return NULL;

}

void VLocation::Load ( FILE *file )
{

	LoadID ( file );

	LoadDynamicString ( ip, SIZE_VLOCATION_IP, file );
	LoadDynamicString ( computer, SIZE_COMPUTER_NAME, file );

	fread ( &x, sizeof (x), 1, file );
	fread ( &y, sizeof (y), 1, file );
	fread ( &listed, sizeof(listed), 1, file );
	fread ( &displayed, sizeof(displayed), 1, file );
    
	LoadID_END ( file );

}

void VLocation::Save ( FILE *file )
{

	SaveID ( file );

	SaveDynamicString ( ip, file );
	SaveDynamicString ( computer, file );

	fwrite ( &x, sizeof (x), 1, file );
	fwrite ( &y, sizeof (y), 1, file );
	fwrite ( &listed, sizeof(listed), 1, file );
    fwrite ( &displayed, sizeof(displayed), 1, file );

	SaveID_END ( file );

}

void VLocation::Print ()
{

	printf ( "VLocation : " );
	printf ( "\tIP = %s, Computer = %s, x = %d, y = %d\n",ip, computer, x, y );
    printf ( "\tlisted = %d, displayed = %d\n", listed, displayed );

}

char *VLocation::GetID ()
{

	return "VIRTLOC";

}

int VLocation::GetOBJECTID ()
{

	return OID_VLOCATION;

}


VLocationSpecial::VLocationSpecial () : VLocation ()
{
    screenIndex = 0;
    securitySystemIndex = -1;
}


VLocationSpecial::~VLocationSpecial ()
{
}

void VLocationSpecial::SetScreenIndex ( int newScreenIndex )
{
    screenIndex = newScreenIndex;
}

void VLocationSpecial::SetSecuritySystemIndex ( int newIndex )
{
    securitySystemIndex = newIndex;
}

void VLocationSpecial::Load  ( FILE *file )
{
    LoadID ( file );

    VLocation::Load ( file );

    fread ( &screenIndex, sizeof(screenIndex), 1, file );
    fread ( &securitySystemIndex, sizeof(securitySystemIndex), 1, file );

    LoadID_END ( file );
}

void VLocationSpecial::Save  ( FILE *file )
{
    SaveID ( file );

    VLocation::Save ( file );

    fwrite ( &screenIndex, sizeof(screenIndex), 1, file );
    fwrite ( &securitySystemIndex, sizeof(securitySystemIndex), 1, file );

    SaveID_END ( file );
}

void VLocationSpecial::Print ()
{
    printf ( "VLocationSpecial:\n" );
    printf ( "screenIndex = %d\n", screenIndex );
    printf ( "securitySystemIndex = %d\n", securitySystemIndex );

    VLocation::Print ();
}
	
char *VLocationSpecial::GetID ()
{
    return "VLOCSPEC";
}

int VLocationSpecial::GetOBJECTID ()
{
    return OID_VLOCATIONSPECIAL;
}

