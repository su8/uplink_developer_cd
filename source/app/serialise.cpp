
#include <stdio.h>
#include <string.h>

#include "gucci.h"

#include "app/app.h"
#include "app/serialise.h"
#include "app/globals.h"
#include "options/options.h"

#include "world/vlocation.h"
#include "world/person.h"
#include "world/message.h"
#include "world/agent.h"
#include "world/player.h"

#include "world/computer/computer.h"
#include "world/computer/logbank.h"
#include "world/computer/databank.h"
#include "world/computer/recordbank.h"
#include "world/computer/bankcomputer.h"
#include "world/computer/bankaccount.h"
#include "world/computer/securitysystem.h"
#include "world/computer/lancomputer.h"

#include "world/computer/computerscreen/genericscreen.h"
#include "world/computer/computerscreen/messagescreen.h"
#include "world/computer/computerscreen/passwordscreen.h"
#include "world/computer/computerscreen/menuscreen.h"
#include "world/computer/computerscreen/dialogscreen.h"
#include "world/computer/computerscreen/useridscreen.h"
#include "world/computer/computerscreen/logscreen.h"
#include "world/computer/computerscreen/bbsscreen.h"
#include "world/computer/computerscreen/linksscreen.h"
#include "world/computer/computerscreen/shareslistscreen.h"
#include "world/computer/computerscreen/cypherscreen.h"
#include "world/computer/computerscreen/highsecurityscreen.h"
#include "world/computer/computerscreen/disconnectedscreen.h"

#include "world/company/company.h"
#include "world/company/companyuplink.h"
#include "world/company/mission.h"
#include "world/company/sale.h"
#include "world/company/news.h"

#include "world/scheduler/notificationevent.h"
#include "world/scheduler/arrestevent.h"
#include "world/scheduler/shotbyfedsevent.h"
#include "world/scheduler/seizegatewayevent.h"
#include "world/scheduler/installhardwareevent.h"
#include "world/scheduler/warningevent.h"
#include "world/scheduler/attemptmissionevent.h"
#include "world/scheduler/runplotsceneevent.h"
#include "world/scheduler/changegatewayevent.h"
#include "world/scheduler/bankrobberyevent.h"



void SaveBTree  ( BTree <UplinkObject *> *btree, FILE *file )
{

	UplinkAssert ( btree );

	DArray <UplinkObject *> *uo = btree->ConvertToDArray ();
	DArray <char *> *uo_id = btree->ConvertIndexToDArray ();

	int size = uo->Size ();
	fwrite ( &size, sizeof(size), 1, file );

	for ( int i = 0; i < size; ++i ) {
		if ( uo->ValidIndex (i) ) {

			UplinkAssert ( uo_id->ValidIndex (i) );
			UplinkAssert ( uo->GetData (i)->GetOBJECTID () != 0 );

			SaveDynamicString ( uo_id->GetData (i), file );

			int OBJECTID = uo->GetData (i)->GetOBJECTID ();
			fwrite ( &OBJECTID, sizeof(int), 1, file );			

			uo->GetData (i)->Save ( file );

		}
	}

	delete uo;
	delete uo_id;

}

void LoadBTree  ( BTree <UplinkObject *> *btree, FILE *file )
{

	UplinkAssert ( btree );

	int size;
	fread ( &size, sizeof(size), 1, file );

	for ( int i = 0; i < size; ++i ) {

		char *id = LoadDynamicString ( file );

		int OBJECTID;
		fread ( &OBJECTID, sizeof(int), 1, file );

		UplinkObject *uo = CreateUplinkObject ( OBJECTID );

		uo->Load ( file );
		btree->PutData ( id, uo );

		delete [] id;

	}

}

void PrintBTree ( BTree <UplinkObject *> *btree )
{

	UplinkAssert ( btree );

	DArray <UplinkObject *> *uo = btree->ConvertToDArray ();
	DArray <char *> *uo_id = btree->ConvertIndexToDArray ();

	for ( int i = 0; i < uo->Size (); ++i ) {
		if ( uo->ValidIndex (i) ) {
			
			UplinkAssert ( uo_id->ValidIndex (i) );
			printf ( "Index = %s\n", uo_id->GetData (i) );

			if ( uo->GetData (i) )
				uo->GetData (i)->Print ();

			else
				printf ( "NULL\n" );

		}
	}

	delete uo;
	delete uo_id;

}

void UpdateBTree ( BTree <UplinkObject *> *btree )
{

	UplinkAssert ( btree );

	DArray <UplinkObject *> *uo = btree->ConvertToDArray ();

	for ( int i = 0; i < uo->Size (); ++i )
		if ( uo->ValidIndex (i) )
			if ( uo->GetData (i) )
				uo->GetData (i)->Update ();

	delete uo;	


}

void DeleteBTreeData ( BTree <UplinkObject *> *btree )
{

	UplinkAssert ( btree );

	DArray <UplinkObject *> *uo = btree->ConvertToDArray ();

	for ( int i = 0; i < uo->Size (); ++i )
		if ( uo->ValidIndex (i) )
			if ( uo->GetData (i) )				
				delete uo->GetData (i);

	delete uo;	

}

void SaveBTree ( BTree <char *> *btree, FILE *file )
{

	UplinkAssert ( btree );

	DArray <char *> *uo = btree->ConvertToDArray ();
	DArray <char *> *uo_id = btree->ConvertIndexToDArray ();

	int size = uo->Size ();
	fwrite ( &size, sizeof(size), 1, file );

	for ( int i = 0; i < size; ++i ) {
		if ( uo->ValidIndex (i) ) {

			UplinkAssert ( uo_id->ValidIndex (i) );

			SaveDynamicString ( uo_id->GetData (i), file );
			SaveDynamicString ( uo->GetData (i), file );

		}
	}

	delete uo;
	delete uo_id;

}

void LoadBTree ( BTree <char *> *btree, FILE *file )
{

	UplinkAssert ( btree );

	int size;
	fread ( &size, sizeof(size), 1, file );

	for ( int i = 0; i < size; ++i ) {

		char *id = LoadDynamicString ( file );
		char *data = LoadDynamicString ( file );
		btree->PutData ( id, data );
		delete [] id;

	}

}

void PrintBTree	( BTree <char *> *btree )
{

	UplinkAssert ( btree );

	DArray <char *> *uo = btree->ConvertToDArray ();
	DArray <char *> *uo_id = btree->ConvertIndexToDArray ();

	for ( int i = 0; i < uo->Size (); ++i ) {
		if ( uo->ValidIndex (i) ) {
			
			UplinkAssert ( uo_id->ValidIndex (i) );
			printf ( "Index = %s\n", uo_id->GetData (i) );

			if ( uo->GetData (i) )
				printf ( "%s\n", uo->GetData (i) );

			else
				printf ( "NULL\n" );

		}
	}

	delete uo;
	delete uo_id;

}

void DeleteBTreeData ( BTree <char *> *btree )
{

	UplinkAssert ( btree );

	DArray <char *> *uo = btree->ConvertToDArray ();

	for ( int i = 0; i < uo->Size (); ++i )
		if ( uo->ValidIndex (i) )
			if ( uo->GetData (i) )
				if ( strcmp ( uo->GetData (i), "" ) != 0 )
					delete [] uo->GetData (i);

	delete uo;	

}


void SaveLList ( LList <UplinkObject *> *llist, FILE *file )
{

	UplinkAssert ( llist );

	int size = llist->Size ();
	fwrite ( &size, sizeof(size), 1, file );

	for ( int i = 0; i < size; ++i ) {

		int OBJECTID = llist->GetData (i)->GetOBJECTID ();
		fwrite ( &OBJECTID, sizeof(int), 1, file );
		llist->GetData (i)->Save ( file );

	}

}

void LoadLList ( LList <UplinkObject *> *llist, FILE *file )
{

	UplinkAssert ( llist );

	int size;
	fread ( &size, sizeof(size), 1, file );

	for ( int i = 0; i < size; ++i ) {
	
		int OBJECTID;
		fread ( &OBJECTID, sizeof(int), 1, file );
		UplinkObject *uo = CreateUplinkObject ( OBJECTID );

		uo->Load ( file );
		llist->PutData ( uo );

	}

}

void PrintLList ( LList <UplinkObject *> *llist )
{

	UplinkAssert ( llist );

	for ( int i = 0; i < llist->Size (); ++i ) {

		printf ( "Index = %d\n", i );

		if ( llist->GetData (i) )
			llist->GetData (i)->Print ();

		else
			printf ( "NULL\n" );

	}

}

void UpdateLList ( LList <UplinkObject *> *llist )
{

	UplinkAssert ( llist );

	for ( int i = 0; i < llist->Size (); ++i )
		if ( llist->GetData (i) )
			llist->GetData (i)->Update ();

}

void DeleteLListData ( LList <UplinkObject *> *llist )
{

	UplinkAssert ( llist );

	for ( int i = 0; i < llist->Size (); ++i )
		if ( llist->GetData (i) )
			delete llist->GetData (i);

}

void SaveLList ( LList <char *> *llist, FILE *file )
{

	UplinkAssert ( llist );

	int size = llist->Size ();
	fwrite ( &size, sizeof(size), 1, file );

	for ( int i = 0; i < llist->Size (); ++i ) 
		SaveDynamicString ( llist->GetData (i), file );

}

void LoadLList ( LList <char *> *llist, FILE *file )
{

	UplinkAssert ( llist );

	int size;
	fread ( &size, sizeof(size), 1, file );

	for ( int i = 0; i < size; ++i ) {

		char *stringdata = LoadDynamicString ( file );
		llist->PutData ( stringdata );

	}
			
}

void PrintLList	( LList <char *> *llist )
{
	
	UplinkAssert ( llist );

	for ( int i = 0; i < llist->Size (); ++i )
		printf ( "Index = %d : %s\n", i, llist->GetData (i) );

}

void DeleteLListData ( LList <char *> *llist )
{

	UplinkAssert ( llist );

	for ( int i = 0; i < llist->Size (); ++i ) 
		if ( llist->GetData (i) )
			if ( strlen(llist->GetData (i)) != 0 )
				if ( strcmp(llist->GetData (i), "") != 0 )
					delete [] llist->GetData (i);
					

}

void SaveDArray ( DArray <UplinkObject *> *darray, FILE *file )
{

	UplinkAssert ( darray );

	int size = darray->Size ();
	fwrite ( &size, sizeof(size), 1, file );

	for ( int i = 0; i < size; ++i ) {
		if ( darray->ValidIndex (i) ) {

			fwrite ( &i, sizeof(i), 1, file );

			int OBJECTID = darray->GetData (i)->GetOBJECTID ();
			if ( OBJECTID == OID_UNKNOWN ) printf ( "SaveDArray WARNING : OID_UNKNOWN as OBJECTID\n" );
			fwrite ( &OBJECTID, sizeof(int), 1, file );

			darray->GetData (i)->Save ( file );

		}
		else {

			int dummy = -1;
			fwrite ( &dummy, sizeof(dummy), 1, file );

		}
	}

}

void LoadDArray ( DArray <UplinkObject *> *darray, FILE *file )
{

	UplinkAssert ( darray );

	int size;
	fread ( &size, sizeof(size), 1, file );
	darray->SetSize ( size );

	for ( int i = 0; i < size; ++i ) {

		int index;
		fread ( &index, sizeof(index), 1, file );

		if ( index != -1 ) {

			int OBJECTID;
			fread ( &OBJECTID, sizeof(int), 1, file );
			if ( OBJECTID == OID_UNKNOWN ) printf ( "LoadDArray WARNING : OID_UNKNOWN as OBJECTID\n" );

			UplinkObject *uo = CreateUplinkObject ( OBJECTID );
			uo->Load ( file );
			darray->PutData ( uo, index );			// Insert into the correct index

		}

	}

}

void PrintDArray ( DArray <UplinkObject *> *darray )
{

	UplinkAssert ( darray );

	for ( int i = 0; i < darray->Size (); ++i ) {

		printf ( "Index = %d\n", i );

		if ( darray->ValidIndex (i) ) {

			if ( darray->GetData (i) )
				darray->GetData (i)->Print ();

			else
				printf ( "NULL\n" );

		}
		else {

			printf ( "Not a Valid Index\n" );

		}

	}
	
}

void UpdateDArray ( DArray <UplinkObject *> *darray )
{

	UplinkAssert ( darray );

	for ( int i = 0; i < darray->Size (); ++i )
		if ( darray->ValidIndex ( i ) )
			if ( darray->GetData (i) )
				darray->GetData (i)->Update ();

}

void DeleteDArrayData ( DArray <UplinkObject *> *darray )
{

	UplinkAssert ( darray );

	for ( int i = 0; i < darray->Size (); ++i )
		if ( darray->ValidIndex ( i ) )
			if ( darray->GetData (i) )
				delete darray->GetData (i);

}

void DeleteDArrayData ( DArray <char *> *darray )
{

	UplinkAssert ( darray );

	for ( int i = 0; i < darray->Size (); ++i ) 
		if ( darray->ValidIndex (i) )
            if ( darray->GetData (i) )
			    if ( strlen(darray->GetData (i)) != 0 )
				    if ( strcmp(darray->GetData (i), "") != 0 )
					    delete [] darray->GetData (i);

}

void SaveDArray ( DArray <int> *darray, FILE *file )
{

	UplinkAssert ( darray );

	int size = darray->Size ();
	fwrite ( &size, sizeof(size), 1, file );

	for ( int i = 0; i < size; ++i ) {
		if ( darray->ValidIndex (i) ) {

			int data = darray->GetData (i);

			fwrite ( &i, sizeof(i), 1, file );
			fwrite ( &data, sizeof(data), 1, file );

		}
		else {

			int dummy = -1;
			fwrite ( &dummy, sizeof(dummy), 1, file );

		}
	}

}

void LoadDArray ( DArray <int> *darray, FILE *file )
{

	UplinkAssert ( darray );

	int size;
	fread ( &size, sizeof(size), 1, file );
	darray->SetSize ( size );

	for ( int i = 0; i < size; ++i ) {

		int index;
		fread ( &index, sizeof(index), 1, file );

		if ( index != -1 ) {

			// -1 Means it was an Invalid Index
			int data;
			fread ( &data, sizeof(data), 1, file );

			darray->PutData ( data, index );			// Insert into the correct index

		}

	}

}

void PrintDArray ( DArray <int> *darray )
{

	UplinkAssert ( darray );

	for ( int i = 0; i < darray->Size (); ++i ) {

		printf ( "Index %d : ", i );

		if ( darray->ValidIndex (i) )
			printf ( "%d\n", darray->GetData (i) );

		else
			printf ( "Not a valid index\n" );

	}

}

UplinkObject *CreateUplinkObject ( int OBJECTID )
{

	UplinkObject *uo;

	switch ( OBJECTID ) {
		
		case OID_VLOCATION				: uo = new VLocation ();			break;	
		case OID_MESSAGE				: uo = new Message ();				break;
		case OID_PERSON					: uo = new Person ();				break;
		case OID_AGENT					: uo = new Agent ();				break;
		case OID_PLAYER					: uo = new Player ();				break;
		case OID_OPTION					: uo = new Option ();				break;
        case OID_VLOCATIONSPECIAL       : uo = new VLocationSpecial ();     break;

		case OID_COMPUTER				: uo = new Computer ();				break;
		case OID_DATA					: uo = new Data ();					break;
		case OID_ACCESSLOG				: uo = new AccessLog ();			break;
		case OID_RECORD					: uo = new Record ();				break;
		case OID_BANKCOMPUTER			: uo = new BankComputer ();			break;
		case OID_BANKACCOUNT			: uo = new BankAccount ();			break;
		case OID_SECURITYSYSTEM			: uo = new SecuritySystem ();		break;
        case OID_LANCOMPUTER            : uo = new LanComputer ();          break;
        case OID_LANCOMPUTERSYSTEM      : uo = new LanComputerSystem ();    break;
        case OID_LANCOMPUTERLINK        : uo = new LanComputerLink ();      break;

		case OID_GENERICSCREEN			: uo = new GenericScreen ();		break;
		case OID_MESSAGESCREEN			: uo = new MessageScreen ();		break;
		case OID_PASSWORDSCREEN			: uo = new PasswordScreen ();		break;
		case OID_MENUSCREEN				: uo = new MenuScreen ();			break;
		case OID_MENUSCREENOPTION		: uo = new MenuScreenOption ();		break;
		case OID_DIALOGSCREEN			: uo = new DialogScreen ();			break;
		case OID_DIALOGSCREENWIDGET		: uo = new DialogScreenWidget ();	break;
		case OID_USERIDSCREEN			: uo = new UserIDScreen ();			break;
		case OID_LOGSCREEN				: uo = new LogScreen ();			break;
		case OID_BBSSCREEN				: uo = new BBSScreen ();			break;
		case OID_LINKSSCREEN			: uo = new LinksScreen ();			break;
		case OID_SHARESLISTSCREEN		: uo = new SharesListScreen ();		break;
		case OID_CYPHERSCREEN			: uo = new CypherScreen ();			break;
		case OID_HIGHSECURITYSCREEN		: uo = new HighSecurityScreen ();	break;
        case OID_DISCONNECTEDSCREEN     : uo = new DisconnectedScreen ();   break;
			
		case OID_COMPANY				: uo = new Company ();				break;
		case OID_COMPANYUPLINK			: uo = new CompanyUplink ();		break;
		case OID_MISSION				: uo = new Mission ();				break;
		case OID_SALE					: uo = new Sale ();					break;
		case OID_SALEVERSION			: uo = new SaleVersion ();			break;
		case OID_NEWS					: uo = new News ();					break;

		case OID_NOTIFICATIONEVENT		: uo = new NotificationEvent ();	break;
		case OID_ARRESTEVENT			: uo = new ArrestEvent ();			break;
		case OID_SHOTBYFEDSEVENT		: uo = new ShotByFedsEvent ();		break;
		case OID_SEIZEGATEWAYEVENT		: uo = new SeizeGatewayEvent ();	break;
		case OID_INSTALLHWEVENT			: uo = new InstallHardwareEvent ();	break;
		case OID_WARNINGEVENT			: uo = new WarningEvent ();			break;
		case OID_ATTEMPTMISSIONEVENT	: uo = new AttemptMissionEvent ();	break;
		case OID_RUNPLOTSCENEEVENT		: uo = new RunPlotSceneEvent ();	break;
		case OID_CHANGEGATEWAYEVENT		: uo = new ChangeGatewayEvent ();	break;
        case OID_BANKROBBERYEVENT       : uo = new BankRobberyEvent ();     break;			

		case OID_UNKNOWN				: UplinkAbort ( "OBJECTID Not assigned" );
		default							: UplinkAbort ( "Unrecognised OBJECTID" );

	}

	return uo;

}

char *LoadDynamicString ( FILE *file )
{

	int size;
	fread ( &size, sizeof(size), 1, file );

	char *string;

	if ( size == -1 ) {

		string = NULL;

	}
    else if ( size < 0 ) {

#ifdef TESTGAME
        UplinkAbort ( "String size appears to be invalid" );
#else
        printf ( "WARNING: LoadDynamicString, size appears to be wrong\n" );
        string = new char [16];
        sprintf ( string, "ERROR" );
#endif

    }
	else {

		string = new char [size+1];
		fread ( string, size, 1, file );
	
	}

	return string;

}

void LoadDynamicString ( char *string, int maxsize, FILE *file )
{

	UplinkAssert (string);

	int size;
	fread ( &size, sizeof(size), 1, file );

	if ( size == -1 || size > maxsize ) {

		// Set up the string as an error
		for ( int i = 0; i < maxsize; ++i )
			string [i] = '#';

	}
    else if ( size < 0 ) {

#ifdef TESTGAME
        UplinkAbort ( "String size appears to be invalid" );
#else
        printf ( "WARNING: LoadDynamicString, size appears to be wrong\n" );
        sprintf ( string, "ERROR" );
#endif

    }
	else {
		
		fread ( string, size, 1, file );
	
	}

}

void SaveDynamicString ( char *string, FILE *file )
{

	if ( string ) {

		int size = strlen(string) + 1;
		fwrite ( &size, sizeof(size), 1, file );
		fwrite ( string, size, 1, file );

	}
	else {

		int size = -1;
		fwrite ( &size, sizeof(size), 1, file );

	}

}
