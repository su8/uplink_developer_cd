
#include "app/serialise.h"

#include "game/game.h"

#include "world/world.h"
#include "world/player.h"
#include "world/message.h"
#include "world/scheduler/changegatewayevent.h"


ChangeGatewayEvent::ChangeGatewayEvent ()
{

	newtype = -1;

}

ChangeGatewayEvent::~ChangeGatewayEvent ()
{
}

void ChangeGatewayEvent::SetNewType ( int value )
{

	newtype = value;

}

void ChangeGatewayEvent::Run ()
{

	// Do the exchange

	if ( newtype != -1 )
		game->GetWorld ()->GetPlayer ()->gateway.ExchangeGateway ( newtype );


	// Send a mail to the player

	Message *msg = new Message ();
	msg->SetTo ( "PLAYER" );
	msg->SetFrom ( "Uplink Corporation" );
	msg->SetSubject ( "New Gateway Ready" );
	msg->SetBody ( "We have finished preparing your new gateway machine.  We have installed "
				   "identical hardware in this new gateway, and we have copied your memory "
				   "banks onto the new system.\n\n"
				   "In order to complete the upgrade, you must now log out from Uplink.  When "
				   "you next log back in to the system, you will be automatically redirected "
				   "to your new gateway system.\n\n"
				   "We will then immediately dismantle your old system and wipe the memory banks "
				   "of all data stored.\n\n"
				   "Regards\n"
				   "[END]" );
	msg->Send ();

}

void ChangeGatewayEvent::RunWarning ()
{

}

char *ChangeGatewayEvent::GetShortString ()
{

	char *shortstring = new char [128];
	sprintf ( shortstring, "Exchanging gateway for TYPE %d", newtype );
	return shortstring;
		
}

char *ChangeGatewayEvent::GetLongString ()
{

	return GetShortString ();
		
}

void ChangeGatewayEvent::Load  ( FILE *file )
{

	LoadID ( file );

	UplinkEvent::Load ( file );

	fread ( &newtype, sizeof(newtype), 1, file );

	LoadID_END ( file );
	
}

void ChangeGatewayEvent::Save  ( FILE *file )
{

	SaveID ( file );

	UplinkEvent::Save ( file );

	fwrite ( &newtype, sizeof(newtype), 1, file );

	SaveID_END ( file );
		
}

void ChangeGatewayEvent::Print ()
{

	printf ( "ChangeGateway Event\n" );
	UplinkEvent::Print ();
	printf ( "NewTYPE = %d\n", newtype );

}
	
char *ChangeGatewayEvent::GetID ()
{

	return "EVT_CGE";

}

int ChangeGatewayEvent::GetOBJECTID ()
{

	return OID_CHANGEGATEWAYEVENT;

}
