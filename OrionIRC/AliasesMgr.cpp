/* $Id: AliasesMgr.cpp,v 1.1 2005/01/23 19:16:43 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "AliasesMgr.h"
#include "Alias.h"
#include "ServerWnd.h"
#include "irc.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

#define MAX_RECURSION	100

//---------------------------------------------------------------------------
AliasesMgr::AliasesMgr() : Aliases( 127 )
{
	Records   = new TList();
    Recursion = 0;
}
//---------------------------------------------------------------------------
AliasesMgr::~AliasesMgr()
{
	Clear();

	delete Records;
}
//---------------------------------------------------------------------------
AliasesMgr& AliasesMgr::operator =( const AliasesMgr& rhs )
{
	Clear();

    for( int i = 0; i < rhs.GetCount(); i++ )
    	Add( new TAlias( rhs.Get( i )));

    return( *this );
}
//---------------------------------------------------------------------------
int AliasesMgr::GetCount( void ) const
{
	return( Records->Count );
}
//---------------------------------------------------------------------------
TAlias *AliasesMgr::Get( int i ) const
{
	TAlias *ret = NULL;

    if( i < GetCount() )
		ret = reinterpret_cast<TAlias *>( Records->Items[ i ]);

    return( ret );
}
//---------------------------------------------------------------------------
void AliasesMgr::Clear( void )
{
    for( int i = 0; i < GetCount(); i++ )
		delete Get( i );

    Records->Clear();
}
//---------------------------------------------------------------------------
void AliasesMgr::Add( TAlias *rec )
{
	Records->Add( rec );
}
//---------------------------------------------------------------------------
void AliasesMgr::Load( TRegistry *reg, AnsiString key )
{
    if( reg->OpenKey( key, false )) {
    	int num = 0;

        try{ num = reg->ReadInteger( "Count" ); } catch(...) {}

        reg->CloseKey();

        for( int i = 0; i < num; i++ ) {
			TAlias	*rec = new TAlias();

            rec->Load( reg, key + "\\Record" + IntToStr( i ));

            Add( rec );
        }

        ReHash();
    }
}
//---------------------------------------------------------------------------
void AliasesMgr::Save( TRegistry *reg, AnsiString key )
{
    if( reg->OpenKey( key, true )) {

        try{
        	reg->WriteInteger( "Count", GetCount() );
        }
        catch(...) {
        }

        reg->CloseKey();

        for( int i = 0; i < GetCount(); i++ )
            Get( i )->Save( reg, key + "\\Record" + IntToStr( i ));
    }
}
//---------------------------------------------------------------------------
void AliasesMgr::ReHash( void )
{
	Aliases.Clear();

    for( int i = 0; i < GetCount(); i++ ) {
    	TAlias *rec = Get( i );

        Aliases.Add( rec->GetName().LowerCase().c_str(), rec );
    }
}
//---------------------------------------------------------------------------
bool AliasesMgr::Invoke( TServerWin *server, TChanWin *channel,
						 AnsiString cmd, AnsiString args, TStringList *words )
{
	bool   	ret = false;

    if( ++Recursion > MAX_RECURSION ) {

    	server->DisplayOnActiveWin( NULL, "Recursion too deep while invoking an alias!", IRC_ERROR );

    } else {
		TAlias *rec = reinterpret_cast<TAlias *>( Aliases.FindData( cmd.LowerCase().c_str() ));

    	if( rec ) {

	    	rec->Invoke( server, channel, args, words );

    		ret = true;
	    }
    }

    Recursion--;

    return( ret );
}
//---------------------------------------------------------------------------
