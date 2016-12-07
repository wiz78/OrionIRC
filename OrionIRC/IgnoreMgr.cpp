/* $Id: IgnoreMgr.cpp,v 1.3 2003/12/31 16:16:04 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <registry.hpp>

#include "IgnoreMgr.h"
#include "IgnoreWnd.h"
#include "Ignore.h"
#include "Hostmask.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
IgnoreMgr::IgnoreMgr()
{
    Records = new TList();
    Enabled = true;
}
//---------------------------------------------------------------------------
IgnoreMgr::~IgnoreMgr()
{
	Clear();

    delete Records;
}
//---------------------------------------------------------------------------
IgnoreMgr& IgnoreMgr::operator =( const IgnoreMgr& rhs )
{
	Clear();

	Enabled = rhs.Enabled;

    for( int i = 0; i < rhs.GetCount(); i++ )
    	Add( new TIgnore( rhs.Get( i )));

    return( *this );
}
//---------------------------------------------------------------------------
int IgnoreMgr::GetCount( void ) const
{
	return( Records->Count );
}
//---------------------------------------------------------------------------
TIgnore *IgnoreMgr::Get( int i ) const
{
	TIgnore *ret = NULL;

    if( i < GetCount() )
		ret = reinterpret_cast<TIgnore *>( Records->Items[ i ]);

    return( ret );
}
//---------------------------------------------------------------------------
void IgnoreMgr::Clear( void )
{
    for( int i = 0; i < GetCount(); i++ )
		delete Get( i );

    Records->Clear();
}
//---------------------------------------------------------------------------
void IgnoreMgr::Add( TIgnore *ign )
{
	Records->Add( ign );

    if( IgnoreWin )
    	IgnoreWin->AddItem( new TIgnore( ign ));
}
//---------------------------------------------------------------------------
void IgnoreMgr::Remove( TIgnore *ign )
{
	Records->Remove( ign );
}
//---------------------------------------------------------------------------
void IgnoreMgr::Load( TRegistry *reg, AnsiString key )
{
    if( reg->OpenKey( key, false )) {
    	int num = 0;

        try{ num = reg->ReadInteger( "Count" ); } catch(...) {}
        try{ Enabled = reg->ReadBool( "Enabled" ); } catch(...) {}

        reg->CloseKey();

        for( int i = 0; i < num; i++ ) {
			TIgnore	*rec = new TIgnore();

            rec->Load( reg, key + "\\Record" + IntToStr( i ));

            Add( rec );
        }
    }
}
//---------------------------------------------------------------------------
void IgnoreMgr::Save( TRegistry *reg, AnsiString key )
{
    if( reg->OpenKey( key, true )) {

        try{
        	reg->WriteInteger( "Count", GetCount() );
            reg->WriteBool( "Enabled", Enabled );
        }
        catch(...) {
        }

        reg->CloseKey();

        for( int i = 0; i < GetCount(); i++ )
            Get( i )->Save( reg, key + "\\Record" + IntToStr( i ));
    }
}
//---------------------------------------------------------------------------
bool IgnoreMgr::Ignore( HostMask& mask, AnsiString& msg, int event )
{
	bool	ret = false;

    if( IsEnabled() )
        for( int i = 0; i < GetCount(); i++ ) {
            TIgnore *ign = Get( i );

            if( ign->Match( mask )) {

                ret = ign->Ignore( msg, event );

                break;
            }
        }

    return( ret );
}
//---------------------------------------------------------------------------