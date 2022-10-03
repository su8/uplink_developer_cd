
#ifdef WIN32
#include <windows.h>
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
#include "world/vlocation.h"
#include "world/player.h"
#include "world/computer/computer.h"
#include "world/computer/bankaccount.h"
#include "world/scheduler/bankrobberyevent.h"



BankRobberyEvent::BankRobberyEvent ()
{

    strcpy ( source_ip, " " );
    strcpy ( source_accno, " " );
    strcpy ( target_ip, " " );
    strcpy ( target_accno, " " );
    amount = 0;

}

BankRobberyEvent::~BankRobberyEvent ()
{
}

void BankRobberyEvent::Run ()
{

    char amount_s [64];
    sprintf ( amount_s, "%d", amount );

    bool rumbled = false;

    // 
    // Has the player left a log on the source bank system

    BankAccount *source = BankAccount::GetAccount ( source_ip, source_accno );
    
    if ( source ) {
        for ( int i = 0; i < source->log.internallogs.Size (); ++i ) {
        
			AccessLog *al = source->log.internallogs.GetData (i);
			
            if ( al && al->TYPE == LOG_TYPE_TRANSFERTO ) {

                if ( al->data1 && al->data2 ) {

				    if ( strstr ( al->data1, target_ip )    != 0 &&
				         strstr ( al->data1, target_accno ) != 0 &&
				         strcmp ( al->data2, amount_s )     == 0 ) {
				        
				        rumbled = true;
				        break;

                    }

                }

			}

        }   
    }

    //
    // Has the player left a log on the target bank system

    if ( !rumbled ) {

        BankAccount *target = BankAccount::GetAccount ( target_ip, target_accno );

        if ( target ) {

            for ( int i = 0; i < target->log.internallogs.Size (); ++i ) {

			    AccessLog *al = target->log.internallogs.GetData (i);
			    
                if ( al && al->TYPE == LOG_TYPE_TRANSFERFROM ) {

                    if ( al->data1 && al->data2 ) {

				        if ( strstr ( al->data1, source_ip )    != 0 &&
				             strstr ( al->data1, source_accno ) != 0 &&
				             strcmp ( al->data2, amount_s )     == 0 ) {
			        
				            rumbled = true;
				            break;

                        }

                    }

                }

			}
			
            

        }

    }


    //
    // To be or not to be

    if ( rumbled ) {

        VLocation *vl = game->GetWorld ()->GetVLocation ( source_ip );
        UplinkAssert (vl);        
        Computer *comp = vl->GetComputer ();
        UplinkAssert (comp);

		ostrstream deathmsg;
		deathmsg << "Disavowed by Uplink Corporation at " << game->GetWorld ()->date.GetLongString ()
				 << "\nFor attempting to steal money from "
				 << comp->name << "\n" 
                 << '\x0';

		game->GameOver ( deathmsg.str () );

    }
    else {

        game->GetWorld ()->GetPlayer ()->rating.ChangeUplinkScore ( UPLINKCHANGE_ROBBANK );
        game->GetWorld ()->GetPlayer ()->rating.ChangeNeuromancerScore ( NEUROMANCERCHANGE_ROBBANK );

    }

}

char *BankRobberyEvent::GetShortString ()
{
    char *string = new char [64];
    strcpy ( string, "Bank Robbery Event" );
    return string;
}

char *BankRobberyEvent::GetLongString ()
{
 
    ostrstream longstring;
    longstring << "Bank Robbery Event\n" 
               << "Source : " << source_ip << ", " << source_accno << "\n"
               << "Target : " << target_ip << ", " << target_accno << "\n"
               << "Amount : " << amount << "\n"
               << hackdate.GetLongString ()
               << '\x0';

    return longstring.str ();

}

void BankRobberyEvent::SetDetails ( char *newsourceip, char *newsourceaccno,
                                    char *newtargetip, char *newtargetaccno,
                                    int newamount, Date *newhackdate )
{

    strcpy ( source_ip, newsourceip );
    strcpy ( source_accno, newsourceaccno );
    strcpy ( target_ip, newtargetip );
    strcpy ( target_accno, newtargetaccno );
    
    amount = newamount;
    hackdate.SetDate ( newhackdate );

}

void BankRobberyEvent::Load  ( FILE *file )
{

    LoadID ( file );

    UplinkEvent::Load ( file );

    fread ( source_ip, sizeof(source_ip), 1, file );
    fread ( source_accno, sizeof(source_accno), 1, file );
    fread ( target_ip, sizeof(target_ip), 1, file );
    fread ( target_accno, sizeof(target_accno), 1, file );

    fread ( &amount, sizeof(amount), 1, file );
    hackdate.Load ( file );

    LoadID_END ( file );

}

void BankRobberyEvent::Save  ( FILE *file )
{

    SaveID ( file );

    UplinkEvent::Save ( file );

    fwrite ( source_ip, sizeof(source_ip), 1, file );
    fwrite ( source_accno, sizeof(source_accno), 1, file );
    fwrite ( target_ip, sizeof(target_ip), 1, file );
    fwrite ( target_accno, sizeof(target_accno), 1, file );

    fwrite ( &amount, sizeof(amount), 1, file );
    hackdate.Save ( file );

    SaveID_END ( file );

}

void BankRobberyEvent::Print ()
{

    printf ( "Bank Robbery Event\n" );
    UplinkEvent::Print ();
    printf ( "Source: %s %s\n", source_ip, source_accno );
    printf ( "Target: %s %s\n", target_ip, target_accno );
    printf ( "Amount: %d\n", amount );
    printf ( "Date:\n" );
    hackdate.Print ();

}
	
char *BankRobberyEvent::GetID ()
{

    return "EVT_BANK";

}

int BankRobberyEvent::GetOBJECTID ()
{

    return OID_BANKROBBERYEVENT;

}

