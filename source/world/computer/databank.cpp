// DataBank.cpp: implementation of the DataBank class.
//
//////////////////////////////////////////////////////////////////////

#include "gucci.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/serialise.h"

#include "world/computer/databank.h"
#include "world/generator/numbergenerator.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DataBank::DataBank()
{
	
	formatted = false;

}

DataBank::~DataBank()
{

    DeleteDArrayData ( (DArray <UplinkObject *> *) &data );
    
}

void DataBank::SetSize ( int newsize )
{

	memory.SetSize ( newsize );

}

int DataBank::GetSize ()
{

	return memory.Size ();

}

int DataBank::NumDataFiles ()
{

	return data.NumUsed ();

}

int DataBank::GetDataSize ()
{

	return	data.Size ();

}

void DataBank::PutData ( Data *newdata )
{

	UplinkAssert (newdata);

	int index = FindValidPlacement ( newdata );

	if ( index != -1 ) PutData ( newdata, index );

}

void DataBank::InsertData ( Data *newdata )
{

    UplinkAssert (newdata);

    if ( data.Size () == 0 )
        PutData ( newdata );

    else {

        int insertindex = NumberGenerator::RandomNumber ( data.Size () );

        // Move the old data item from this location to the end

        if ( data.ValidIndex (insertindex) ) {
            data.SetSize ( data.Size () + 1 );
            Data *olddata = data.GetData (insertindex);
            UplinkAssert (olddata);
            int newindex = data.Size () - 1;
            data.PutData (olddata, newindex );

            for ( int i = 0; i < memory.Size (); ++i )
                if ( memory.ValidIndex (i) )
                    if ( memory.GetData (i) == insertindex )
                        memory.PutData ( newindex, i );
        }
        
        // Insert the new data item

        data.PutData ( newdata, insertindex );

        int index = FindValidPlacement (newdata);

        if ( index == -1 ) {
            memory.SetSize ( memory.Size () + newdata->size );
            index = FindValidPlacement (newdata);
        }
    
        UplinkAssert (index != -1);

        for ( int i = 0; i < newdata->size; ++i )
		    memory.PutData ( insertindex, index + i );

    }

}

void DataBank::PutData ( Data *newdata, int memoryindex )
{

	int pos = data.PutData ( newdata );

	UplinkAssert ( newdata );

	for ( int i = 0; i < newdata->size; ++i )
		memory.PutData ( pos, memoryindex + i );

}

void DataBank::RemoveData ( int memoryindex )
{

	if ( memory.ValidIndex (memoryindex) ) {

		// Delete the file

		int dataindex = GetDataIndex (memoryindex);
		
		RemoveDataFile ( dataindex );

	}

}

void DataBank::RemoveDataFile ( int dataindex )
{

	if ( data.ValidIndex (dataindex) ) {

		// Delete the file

        Data *thefile = data.GetData (dataindex);               // TODO is it safe to delete the data here?
        if ( thefile ) delete thefile;

		data.RemoveData (dataindex);

		// Delete any indexes to the file
	
		for ( int i = 0; i < memory.Size (); ++i )
			if ( memory.ValidIndex (i) )
				if ( memory.GetData (i) == dataindex )
					memory.RemoveData ( i );

		// If that was the last file, then this databank has been formatted

		if ( NumDataFiles () == 0 )
			formatted = true;

	}

}

Data *DataBank::GetData ( int memoryindex )
{

	if ( !memory.ValidIndex (memoryindex) )
		return NULL;

	else if ( !data.ValidIndex ( memory [memoryindex] ) )
		return NULL;

	else
		return data [ memory [memoryindex] ];

}

Data *DataBank::GetData ( char *title )
{

	for ( int i = 0; i < data.Size (); ++i )
		if ( data.ValidIndex (i) )
			if ( strcmp ( data.GetData (i)->title, title ) == 0 )
				return data.GetData (i);

	return NULL;

}

Data *DataBank::GetDataFile ( int dataindex )
{

	if ( !data.ValidIndex (dataindex) )
		return NULL;

	else
		return data.GetData (dataindex);

}

int DataBank::GetDataIndex ( int memoryindex )
{

	if ( !memory.ValidIndex (memoryindex) )
		return -1;

	else if ( !data.ValidIndex ( memory [memoryindex] ) )
		return -1;

	else
		return memory [memoryindex];
	

}

int DataBank::GetMemoryIndex ( int dataindex )
{

	for ( int i = 0; i < memory.Size (); ++i ) {
		if ( memory.ValidIndex (i) ) {
	
			if ( memory.GetData (i) == dataindex )
				return i;

		}
	}

	return -1;

}

int DataBank::IsValidPlacement ( Data *newdata, int memoryindex )
{

	UplinkAssert (newdata);
	
	if ( memoryindex < 0 ) return 2;

	for ( int i = memoryindex; i < memoryindex + newdata->size; ++i ) {
	
		if ( i >= memory.Size () || i < 0 ) 
			return 2;									// Not valid

		else if ( memory.ValidIndex (i) )
			return 1;									// Will overwrite

	}

	return 0;											// No conflicts

}

int DataBank::FindValidPlacement ( Data *newdata )
{

	UplinkAssert (newdata);

	for ( int i = 0; i < memory.Size (); ++i ) {

		if ( IsValidPlacement ( newdata, i ) == 0 ) {

			return i;
		
		}

	}

	return -1;

}

void DataBank::Format ()
{

    int oldmemsize = memory.Size ();

	DeleteDArrayData ( (DArray <UplinkObject *> *) &data );
	
	data.Empty ();
	memory.Empty ();
    memory.SetSize ( oldmemsize );

	formatted = true;

}

void DataBank::Load ( FILE *file )
{
	
	LoadID ( file );

	LoadDArray ( (DArray <UplinkObject *> *) &data, file );
	LoadDArray ( &memory, file );

	fread ( &formatted, sizeof(formatted), 1, file );

	LoadID_END ( file );

}

void DataBank::Save ( FILE *file )
{

	SaveID ( file );

	SaveDArray ( (DArray <UplinkObject *> *) &data, file );
	SaveDArray ( &memory, file );

	fwrite ( &formatted, sizeof(formatted), 1, file );

	SaveID_END ( file );

}

void DataBank::Print ()
{

	printf ( "DataBank\n" );
	PrintDArray ( (DArray <UplinkObject *> *) &data );
	PrintDArray ( &memory );
	printf ( "Formatted: %d\n" );

}
	
char *DataBank::GetID ()
{

	return "DATABANK";

}

int DataBank::GetOBJECTID ()
{

	return OID_DATABANK;

}

void DataBank::Update ()
{

//	UpdateDArray ( (DArray <UplinkObject *> *) &data );

}


// Data class =================================================================


Data::Data ()
{

	sprintf ( title, "Empty" );
	TYPE = DATATYPE_NONE;
	size = 0;
	encrypted = 0;
	compressed = 0;
	version = 1.0;
	softwareTYPE = SOFTWARETYPE_NONE;

}

Data::Data ( Data *copyme )
{

	UplinkAssert (copyme);

	SetTitle ( copyme->title );
	TYPE = copyme->TYPE;
	size = copyme->size;
	encrypted = copyme->encrypted;
	compressed = copyme->compressed;
	version = copyme->version;
	softwareTYPE = copyme->softwareTYPE;

}

Data::~Data ()
{
}

void Data::SetTitle ( char *newtitle )
{
	
	UplinkAssert ( strlen (newtitle) < SIZE_DATA_TITLE );
	strcpy ( title, newtitle );

}

void Data::SetDetails ( int newTYPE, int newsize, int newencrypted, int newcompressed, 
					    float newversion, int newsoftwareTYPE )
{

	TYPE = newTYPE;
	size = newsize;
	encrypted = newencrypted;
	compressed = newcompressed;
	version = newversion;
	softwareTYPE = newsoftwareTYPE;

}

void Data::Load ( FILE *file )
{

	LoadID ( file );

	LoadDynamicString ( title, SIZE_DATA_TITLE, file );

	fread ( &TYPE, sizeof(TYPE), 1, file );
	fread ( &size, sizeof(size), 1, file );
	fread ( &encrypted, sizeof(encrypted), 1, file );
	fread ( &compressed, sizeof(compressed), 1, file );
	fread ( &version, sizeof(version), 1, file );
	fread ( &softwareTYPE, sizeof(softwareTYPE), 1, file );

	LoadID_END ( file );

}

void Data::Save ( FILE *file )
{

	SaveID ( file );

	SaveDynamicString ( title, file );

	fwrite ( &TYPE, sizeof(TYPE), 1, file );
	fwrite ( &size, sizeof(size), 1, file );
	fwrite ( &encrypted, sizeof(encrypted), 1, file );
	fwrite ( &compressed, sizeof(compressed), 1, file );
	fwrite ( &version, sizeof(version), 1, file );
	fwrite ( &softwareTYPE, sizeof(softwareTYPE), 1, file );

	SaveID_END ( file );

}

void Data::Print ()
{
	
	printf ( "Data: %s\n", title );
	printf ( "type = %d, size = %d, encrypted = %d, compressed = %d\nversion = %f, softwareTYPE = %d\n", 
				TYPE, size, encrypted, compressed, version, softwareTYPE );

}

void Data::Update ()
{
}

char *Data::GetID ()
{
	
	return "DATA";

}

int Data::GetOBJECTID ()
{

	return OID_DATA;

}


