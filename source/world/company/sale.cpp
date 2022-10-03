
#include "gucci.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/serialise.h"

#include "world/computer/databank.h"
#include "world/company/sale.h"


Sale::Sale ()
{

	strcpy ( title, "" );
	saleTYPE = SALETYPE_NONE;
	swhwTYPE = SOFTWARETYPE_NONE;

}

Sale::~Sale ()
{

	DeleteDArrayData ( (DArray <UplinkObject *> *) &versions );

}

void Sale::SetTitle ( char *newtitle )
{
	
	UplinkAssert ( strlen (newtitle) < SIZE_SALE_TITLE );
	strcpy ( title, newtitle );

}

void Sale::SetSaleTYPE ( int newTYPE )
{

	saleTYPE = newTYPE;

}

void Sale::SetSwhwTYPE ( int newSwhwTYPE ) 
{

	swhwTYPE = newSwhwTYPE;

}

void Sale::AddVersion ( char *details, int cost, int size, int data )
{

	SaleVersion *sv = new SaleVersion ();
	sv->Set ( details, cost, size, data );

	versions.PutData ( sv );

}

SaleVersion *Sale::GetVersion ( int index )
{

	if ( versions.ValidIndex (index) )
		return versions.GetData (index);

	else
		return NULL;

}

void Sale::Load ( FILE *file )
{
	
	LoadID ( file );

	LoadDynamicString ( title, SIZE_SALE_TITLE, file );

	fread ( &saleTYPE, sizeof(saleTYPE), 1, file );
	fread ( &swhwTYPE, sizeof(swhwTYPE), 1, file );

	LoadDArray ( (DArray <UplinkObject *> *) &versions, file );

	LoadID_END ( file );

}

void Sale::Save ( FILE *file )
{
	
	SaveID ( file );

	SaveDynamicString ( title, file );
	
	fwrite ( &saleTYPE, sizeof(saleTYPE), 1, file );
	fwrite ( &swhwTYPE, sizeof(swhwTYPE), 1, file );
	
	SaveDArray ( (DArray <UplinkObject *> *) &versions, file );

	SaveID_END ( file );

}

void Sale::Print ()
{

	printf ( "Sale\n" );
	printf ( "\tTitle:%s, saleTYPE:%d, swhwTYPE:\n", 
				title, saleTYPE, swhwTYPE );

	PrintDArray ( (DArray <UplinkObject *> *) &versions );

}

void Sale::Update ()
{
}
	
char *Sale::GetID ()
{
	
	return "SALE";

}

int Sale::GetOBJECTID ()
{

	return OID_SALE;

}


// ============================================================================



SaleVersion::SaleVersion ()
{
	
	details = NULL;
	cost = -1;
	size = -1;
	data = -1;

}

SaleVersion::~SaleVersion ()
{

	if ( details ) delete [] details;

}

void SaleVersion::Set ( char *newdetails, int newcost, int newsize, int newdata )
{

	if ( details ) delete [] details;
	details = new char [strlen(newdetails)+1];
	strcpy ( details, newdetails );

	cost = newcost;
	size = newsize;
	data = newdata;

}

char *SaleVersion::GetDetails ()
{

	return details;

}

void SaleVersion::Load ( FILE *file )
{

	LoadID ( file );

	details = LoadDynamicString ( file );
	fread ( &cost, sizeof(cost), 1, file );
	fread ( &size, sizeof(size), 1, file );
	fread ( &data, sizeof(data), 1, file );
		
	LoadID_END ( file );

}

void SaleVersion::Save  ( FILE *file )
{

	SaveID ( file );

	SaveDynamicString ( details, file );
	fwrite ( &cost, sizeof(cost), 1, file );
	fwrite ( &size, sizeof(size), 1, file );
	fwrite ( &data, sizeof(data), 1, file );
		
	SaveID_END ( file );

}

void SaleVersion::Print ()
{

	printf ( "SaleVersion: %s\n", details );
	printf ( "Cost: %d, Size: %d, Data: %d\n", cost, size, data );

}

void SaleVersion::Update ()
{
}
	
char *SaleVersion::GetID ()
{

	return "SALEVER";

}

int SaleVersion::GetOBJECTID ()
{
	
	return OID_SALEVERSION;

}

