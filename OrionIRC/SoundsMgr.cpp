/* $Id: SoundsMgr.cpp,v 1.1 2005/04/27 17:34:05 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <mmsystem.h>

#include "SoundsMgr.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
SoundsMgr::SoundsMgr()
{
	Clear();
}
//---------------------------------------------------------------------------
SoundsMgr& SoundsMgr::operator =( const SoundsMgr& rhs )
{
	for( int i = 0; i < COUNT; i++ )
    	Files[ i ] = rhs.Files[ i ];

    return( *this );
}
//---------------------------------------------------------------------------
void SoundsMgr::Clear( void )
{
	for( int i = 0; i < COUNT; i++ ) {
    	Files[ i ]   = "";
        Enabled[ i ] = false;
    }
}
//---------------------------------------------------------------------------
void SoundsMgr::Play( EventType ev ) const
{
	if( IsSoundEnabled( ev ))
		PlaySound( Files[ ev ].c_str(), NULL, SND_ASYNC | SND_FILENAME );
}
//---------------------------------------------------------------------------
AnsiString SoundsMgr::GetSound( EventType ev ) const
{
	return( Files[ ev ] );
}
//---------------------------------------------------------------------------
bool SoundsMgr::IsSoundEnabled( EventType ev ) const
{
	return( Enabled[ ev ] && !Files[ ev ].IsEmpty() );
}
//---------------------------------------------------------------------------
void SoundsMgr::SetSound( EventType ev, AnsiString file, bool enabled )
{
	Files[ ev ]   = file;
    Enabled[ ev ] = enabled;
}
//---------------------------------------------------------------------------
void SoundsMgr::Save( TRegistry *reg, AnsiString key ) const
{
	if( reg->OpenKey( key, true )) {

    	try {
			for( int i = 0; i < COUNT; i++ ) {

            	reg->WriteString( "File" + IntToStr( i ), Files[ i ] );
                reg->WriteBool( "Enabled" + IntToStr( i ), Enabled[ i ] );
            }
        }
        catch(...) {
        }


    	reg->CloseKey();
    }
}
//---------------------------------------------------------------------------
void SoundsMgr::Load( TRegistry *reg, AnsiString key )
{
	if( reg->OpenKey( key, false )) {

		for( int i = 0; i < COUNT; i++ ) {

           	Files[ i ] = reg->ReadString( "File" + IntToStr( i ));

            try { Enabled[ i ] = reg->ReadBool( "Enabled" + IntToStr( i )); } catch(...) {}
        }

    	reg->CloseKey();
    }
}
//---------------------------------------------------------------------------
