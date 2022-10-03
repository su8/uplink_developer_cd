// Tosser implementation file
// Part of tosser
// By Christopher Delay

#include <stdlib.h>
#include <iostream.h>
#include <assert.h>

#include "tosser.h"

template <class T>
DArray <T> :: DArray ()
{

    stepsize = 1;
    arraysize = 0;
    array = NULL;
    shadow = NULL;
    
}

template <class T>
DArray <T> :: DArray ( int newstepsize )
{

    stepsize = newstepsize;
    arraysize = 0;
    array = NULL;
    shadow = NULL;
    
}

template <class T>
DArray <T> :: ~DArray ()
{

    Empty ();

}

template <class T>
void DArray <T> :: SetSize ( int newsize )
{

	if ( newsize > arraysize ) {

		int oldarraysize = arraysize;

		arraysize = newsize;
		T *temparray = new T [ arraysize ];
		char *tempshadow = new char [ arraysize ];
		
		int a;

		for ( a = 0; a < oldarraysize; ++a ) {
			
			temparray [a] = array [a];
			tempshadow [a] = shadow [a];
			
		}
		
		for ( a = oldarraysize; a < arraysize; ++a )
			tempshadow [a] = 0;
		
		if ( array ) delete [] array;
		if ( shadow ) delete [] shadow;
		
		array = temparray;
		shadow = tempshadow;

	}
	else if ( newsize < arraysize ) {

		arraysize = newsize;
		T *temparray = new T [arraysize];
		char *tempshadow = new char [arraysize];

		for ( int a = 0; a < arraysize; ++a ) {

			temparray [a] = array [a];
			tempshadow [a] = shadow [a];

		}

		if ( array ) delete [] array;
		if ( shadow ) delete [] shadow;
		
		array = temparray;
		shadow = tempshadow;

	}
	else if ( newsize == arraysize ) {

		// Do nothing

	}

}

template <class T>
void DArray <T> :: SetStepSize ( int newstepsize )
{

	stepsize = newstepsize;

}

template <class T>
int DArray <T> :: PutData ( const T &newdata )
{
    
    int freespace = -1;				 // Find a free space
    
    for ( int a = 0; a < arraysize; ++a ) {
	
		if ( shadow [a] == 0 ) {
			
			freespace = a;
			break;
			
		}
		
    }
    
    if ( freespace == -1 ) {			 // Must resize the array
			
		freespace = arraysize;		
		SetSize ( arraysize + stepsize );
   
    }
	    
    array [freespace] = newdata;
    shadow [freespace] = 1;
    
    return freespace;

}

template <class T>
void DArray <T> :: PutData ( const T &newdata, int index )
{

    assert ( index < arraysize && index >= 0 );       

    array [index] = newdata;
    shadow [index] = 1;

}

template <class T>
void DArray <T> :: Empty ()
{

    if ( array != NULL )
		delete [] array;
    
    if ( shadow != NULL )
		delete [] shadow;
    
    array = NULL;
    shadow = NULL;
    
    arraysize = 0;

}

template <class T>
T DArray <T> :: GetData ( int index )
{

    assert ( index < arraysize && index >= 0 );       

    if ( shadow [index] == 0 )
	   cout << "DArray::GetData called, referenced unused data.  (Index = " << index << ")\n";
    
    return array [index];

}

template <class T>
T& DArray <T> :: operator [] (int index)
{

    assert ( index < arraysize && index >= 0 );

    if ( shadow [index] == 0 )
		cout << "DArray error : DArray::[] called, referenced unused data.  (Index = " << index << ")\n";
    
    return array [index];    
}


template <class T>
void DArray <T> :: ChangeData ( const T &newdata, int index )
{
    
	assert ( index < arraysize && index >= 0 );
        
    if ( shadow [index] == 0 )
		cout << "Warning : DArray::ChangeData called, referenced unused data.  (Index = " << index << ")\n";
    
    PutData ( newdata, index );
    shadow [index] = 1;
    
}

template <class T>
void DArray <T> :: RemoveData ( int index )
{
    
    assert ( index < arraysize && index >= 0 );
    
    if ( shadow [index] == 0 )
		cout << "Warning : DArray::RemoveData called, referenced unused data.  (Index = " << index << ")\n";
    
    shadow [index] = 0;
    
}

template <class T>
int DArray <T> :: NumUsed ()
{

    int count = 0;
    
    for ( int a = 0; a < arraysize; ++a )
		if ( shadow [a] == 1 )
			++count;
		
    return count;
		
}

template <class T>
int DArray <T> :: Size ()
{
	       
    return arraysize;
		
}

template <class T>
bool DArray <T> :: ValidIndex ( int index )
{
    
    if (index >= arraysize || index < 0 )
		return false;
    
    if (!shadow [index])
		return false;
    
    return true;
    
}

template <class T>
int DArray <T> :: FindData ( const T &newdata )
{
    
    for ( int a = 0; a < arraysize; ++a )
		if ( shadow [a] )
		    if ( array [a] == newdata )
				return a;
    
    return -1;    
    
}
