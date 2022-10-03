
#include <string.h>

#include "gucci.h"

#include "game/game.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/serialise.h"

#include "world/world.h"
#include "world/company/mission.h"
#include "world/vlocation.h"
#include "world/generator/missiongenerator.h"


Mission::Mission ()
{

	details = NULL;
	fulldetails = NULL;
	duedate = NULL;
    
	whysomuchmoney = NULL;
	howsecure = NULL;
	whoisthetarget = NULL;
	
	TYPE = MISSION_NONE;
	sprintf ( description, "" );
	sprintf ( employer, "" );
	sprintf ( contact, "" );

	sprintf ( completionA, "" );
	sprintf ( completionB, "" );
	sprintf ( completionC, "" );
	sprintf ( completionD, "" );
	sprintf ( completionE, "" );

	payment = 0;
	maxpayment = 0;
	paymentmethod = MISSIONPAYMENT_AFTERCOMPLETION;

    difficulty = 0;
    minuplinkrating = -1;
    acceptrating = 0;
	npcpriority = false;

}

Mission::~Mission ()
{

	if ( details )			delete [] details;
	if ( fulldetails )		delete [] fulldetails;
	if ( whysomuchmoney )	delete [] whysomuchmoney;
	if ( howsecure )		delete [] howsecure;
	if ( whoisthetarget )	delete [] whoisthetarget;

	if ( duedate ) delete duedate;

	DeleteLListData ( &links );
	DeleteBTreeData ( &codes );

}

void Mission::SetTYPE ( int newTYPE )
{

	TYPE = newTYPE;

}

void Mission::SetCompletion ( char *newA, char *newB, char *newC, char *newD, char *newE )
{

	// No need to set them if NULL is passed in - they will not be examined after all

	if ( newA ) {
		UplinkAssert ( strlen(newA) < SIZE_MISSION_COMPLETION );
		strcpy ( completionA, newA );
	}

	if ( newB ) {
		UplinkAssert ( strlen(newB) < SIZE_MISSION_COMPLETION );
		strcpy ( completionB, newB );
	}

	if ( newC ) {
		UplinkAssert ( strlen(newC) < SIZE_MISSION_COMPLETION );
		strcpy ( completionC, newC );
	}

	if ( newD ) {
		UplinkAssert ( strlen(newD) < SIZE_MISSION_COMPLETION );
		strcpy ( completionD, newD );
	}

	if ( newE ) {
		UplinkAssert ( strlen(newE) < SIZE_MISSION_COMPLETION );
		strcpy ( completionE, newE );
	}

}

void Mission::SetCreateDate ( Date *date )
{

	UplinkAssert ( date );
	createdate.SetDate ( date );

}

void Mission::SetNpcPriority ( bool newpriority )
{

	npcpriority = newpriority;

}

void Mission::SetDescription ( char *newdescription )
{

	UplinkAssert ( strlen ( newdescription ) < SIZE_MISSION_DESCRIPTION );
	strcpy ( description, newdescription );

}

void Mission::SetDetails ( char *newdetails )
{

	if ( details ) delete [] details;

	details = new char [strlen(newdetails)+1];
	strcpy ( details, newdetails );

}

void Mission::SetFullDetails ( char *newdetails )
{

	if ( fulldetails ) delete [] fulldetails;

	fulldetails = new char [strlen(newdetails)+1];
	strcpy ( fulldetails, newdetails );

}

void Mission::SetWhySoMuchMoney	( char *answer )
{

	if ( whysomuchmoney ) delete [] whysomuchmoney;
	whysomuchmoney = NULL;

	if ( answer ) {

		whysomuchmoney = new char [strlen(answer)+1];
		strcpy ( whysomuchmoney, answer );

	}

}

void Mission::SetHowSecure ( char *answer )
{

	if ( howsecure ) delete [] howsecure;
	howsecure = NULL;

	if ( answer ) {

		howsecure = new char [strlen(answer)+1];
		strcpy ( howsecure, answer );

	}

}

void Mission::SetWhoIsTheTarget ( char *answer )
{

	if ( whoisthetarget ) delete [] whoisthetarget;
	whoisthetarget = NULL;

	if ( answer ) {

		whoisthetarget = new char [strlen(answer)+1];
		strcpy ( whoisthetarget, answer );

	}

}

void Mission::SetEmployer ( char *newemployer )
{

	UplinkAssert ( strlen (newemployer) < 64 );
	UplinkAssert ( game->GetWorld ()->GetCompany (newemployer) );

	strcpy ( employer, newemployer );

}

void Mission::SetContact ( char *newcontact )
{

	UplinkAssert ( strlen (newcontact) < SIZE_PERSON_NAME );
	UplinkAssert ( game->GetWorld ()->GetPerson ( newcontact ) );

	strcpy ( contact, newcontact );

}

void Mission::SetPayment ( int newpayment, int newmaxpayment )
{

	payment = newpayment;

	if ( newmaxpayment != -1 )
		maxpayment = newmaxpayment;

	else
		maxpayment = payment;

}

void Mission::SetDifficulty ( int newdifficulty )
{

	difficulty = newdifficulty;

}

void Mission::SetMinRating ( int newrating )
{

	minuplinkrating = newrating;

}

void Mission::SetAcceptRating ( int newrating )
{

	acceptrating = newrating;

}

void Mission::GiveLink ( char *ip )
{

	UplinkAssert ( strlen (ip) < SIZE_VLOCATION_IP );
	char *theip = new char [SIZE_VLOCATION_IP];
	strcpy ( theip, ip );
	links.PutData ( theip );

}

void Mission::GiveCode ( char *ip, char *code )
{

	UplinkAssert ( strlen (ip) < SIZE_VLOCATION_IP );
	char theip [SIZE_VLOCATION_IP];
	strcpy ( theip, ip );

	char *thecode = new char [strlen(code)+1];
	strcpy ( thecode, code );

	codes.PutData ( theip, thecode );

}

void Mission::SetDueDate ( Date *newdate )
{

	duedate = newdate;

}

Date *Mission::GetDueDate ()
{

	return duedate;

}

char *Mission::GetDetails ()
{

	UplinkAssert ( details );
	return details;

}

char *Mission::GetFullDetails ()
{

	UplinkAssert ( fulldetails );
	return fulldetails;

}

char *Mission::GetWhySoMuchMoney ()
{

	return whysomuchmoney;

}

char *Mission::GetHowSecure	()
{

	return howsecure;

}

char *Mission::GetWhoIsTheTarget ()
{

	return whoisthetarget;

}

void Mission::Load  ( FILE *file )
{

	LoadID ( file );

	fread ( &TYPE, sizeof(TYPE), 1, file );
	
	LoadDynamicString ( completionA, SIZE_MISSION_COMPLETION, file );
	LoadDynamicString ( completionB, SIZE_MISSION_COMPLETION, file );
	LoadDynamicString ( completionC, SIZE_MISSION_COMPLETION, file );
	LoadDynamicString ( completionD, SIZE_MISSION_COMPLETION, file );
	LoadDynamicString ( completionE, SIZE_MISSION_COMPLETION, file );

	createdate.Load ( file );

	LoadDynamicString ( employer, SIZE_MISSION_EMPLOYER, file );
	LoadDynamicString ( contact, SIZE_PERSON_NAME, file );
	LoadDynamicString ( description, SIZE_MISSION_DESCRIPTION, file );

	fread ( &payment, sizeof(payment), 1, file );
	fread ( &difficulty, sizeof(difficulty), 1, file );
	fread ( &minuplinkrating, sizeof(minuplinkrating), 1, file );
	fread ( &acceptrating, sizeof(acceptrating), 1, file );
	fread ( &npcpriority, sizeof(npcpriority), 1, file);
	
	details			= LoadDynamicString ( file );
	fulldetails		= LoadDynamicString ( file );
	whysomuchmoney	= LoadDynamicString ( file );
	howsecure		= LoadDynamicString ( file );
	whoisthetarget	= LoadDynamicString ( file );
	
	LoadLList ( &links, file );
	LoadBTree ( &codes, file );

	bool hasduedate;
	fread ( &hasduedate, sizeof(hasduedate), 1, file );

	if ( hasduedate ) {
		duedate = new Date ();
		duedate->Load ( file );
	}
	else 
		duedate = NULL;

	LoadID_END ( file );

}

void Mission::Save  ( FILE *file )
{

	SaveID ( file );

	fwrite ( &TYPE, sizeof(TYPE), 1, file );
	
	SaveDynamicString ( completionA, file );
	SaveDynamicString ( completionB, file );
	SaveDynamicString ( completionC, file );
	SaveDynamicString ( completionD, file );
	SaveDynamicString ( completionE, file );

	createdate.Save ( file );

	SaveDynamicString ( employer, file );
	SaveDynamicString ( contact, file );
	SaveDynamicString ( description, file );

	fwrite ( &payment, sizeof(payment), 1, file );
	fwrite ( &difficulty, sizeof(difficulty), 1, file );
	fwrite ( &minuplinkrating, sizeof(minuplinkrating), 1, file );
	fwrite ( &acceptrating, sizeof(acceptrating), 1, file );
	fwrite ( &npcpriority, sizeof(npcpriority), 1, file );
	
	SaveDynamicString ( details, file );
	SaveDynamicString ( fulldetails, file );
	SaveDynamicString ( whysomuchmoney, file );
	SaveDynamicString ( howsecure, file );
	SaveDynamicString ( whoisthetarget, file );

	SaveLList ( &links, file );
	SaveBTree ( &codes, file );

	bool hasduedate = (duedate != NULL);
	fwrite ( &hasduedate, sizeof(hasduedate), 1, file );
	if ( hasduedate ) 	duedate->Save ( file );
	
	SaveID_END ( file );

}

void Mission::Print ()
{

	printf ( "Mission : TYPE=%d\n", TYPE );
	printf ( "Employer=%s, Payment=%d, Difficulty=%d, MinRating=%d, AcceptRating=%d, Description=%s\n",
			 employer, payment, difficulty, minuplinkrating, acceptrating, description );
	printf ( "NPC priority = %d\n", npcpriority );
	createdate.Print ();
	printf ( "\tCompletionA = %s\n", completionA );
	printf ( "\tCompletionB = %s\n", completionB );
	printf ( "\tCompletionC = %s\n", completionC );
	printf ( "\tCompletionD = %s\n", completionD );
	printf ( "\tCompletionE = %s\n", completionE );
	printf ( "\tContact = %s\n", contact );
	printf ( "\tDetails=%s\n", details );
	printf ( "\tFullDetails=%s\n", fulldetails );
	printf ( "\tWhySoMuchMoney=%s\n", whysomuchmoney );
	printf ( "\tHowSecure=%s\n", howsecure );

	PrintLList ( &links );
	PrintBTree ( &codes );

	if ( duedate ) {
		printf ( "Due date :\n" );
		duedate->Print ();
	}
	else 
		printf ( "No due date\n" );

}
	
char *Mission::GetID ()
{

	return "MISSION";

}

int Mission::GetOBJECTID ()
{

	return OID_MISSION;

}

