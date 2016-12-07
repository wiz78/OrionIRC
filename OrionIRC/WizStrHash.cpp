/* $Id: WizStrHash.cpp,v 1.3 2005/04/17 10:54:25 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "WizStrHash.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
WizStrHash::WizStrHash( DWORD size )
{
    Size = size;

    HashTable = new TList *[ size ];

    if( !HashTable )
        throw Exception( "Out of mem!" );

    for( DWORD i = 0; i < Size; i++ )
        HashTable[ i ] = new TList();
}
//---------------------------------------------------------------------------
WizStrHash::~WizStrHash()
{
    if( HashTable ) {

        Clear();

        for( DWORD i = 0; i < Size; i++ )
        	delete HashTable[ i ];

        delete[] HashTable;
    }
}
//---------------------------------------------------------------------------
void WizStrHash::Clear( void )
{
    for( DWORD i = 0; i < Size; i++ ) {
    	TList *list = HashTable[ i ];

        for( int j = 0; j < list->Count; j++ )
            delete reinterpret_cast<WizStrHashNode *>( list->Items[ j ] );

        list->Clear();
    }
}
//---------------------------------------------------------------------------
DWORD WizStrHash::ComputeHash( char *str )
{
    DWORD   code = 0;

    if( str && *str ) {

	    code = tolower( *str++ );

    	while( *str )
        	code = ( code * 32 ) + tolower( *str++ );

        code %= Size;
    }

    return( code );
}
//---------------------------------------------------------------------------
WizStrHashNode *WizStrHash::Find( char *str )
{
    DWORD   hash = ComputeHash( str );
    TList  *list = HashTable[ hash ];

    for( int j = 0; j < list->Count; j++ ) {
        WizStrHashNode *node = reinterpret_cast<WizStrHashNode *>( list->Items[ j ] );

        if( !node->String.AnsiCompareIC( str ))
            return( node );
    }

    return( NULL );
}
//---------------------------------------------------------------------------
void WizStrHash::Add( char *str, void *data )
{
    if( !Find( str )) {
        DWORD          	hash = ComputeHash( str );
        WizStrHashNode *item = new WizStrHashNode();

        item->String = str;
        item->Data   = data;

        HashTable[ hash ]->Add( item );
    }
}
//---------------------------------------------------------------------------
void WizStrHash::Remove( char *str )
{
    WizStrHashNode *item = Find( str );

    if( item ) {
        DWORD   hash = ComputeHash( str );
        TList  *list = HashTable[ hash ];

        list->Remove( item );

        delete item;
    }
}
//---------------------------------------------------------------------------
void *WizStrHash::FindData( char *str )
{
    WizStrHashNode *item = Find( str );
    void           *ret = NULL;

    if( item )
        ret = item->Data;

    return( ret );
}
//---------------------------------------------------------------------------
