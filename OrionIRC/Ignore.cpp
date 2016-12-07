/* $Id: Ignore.cpp,v 1.3 2005/05/04 13:27:41 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <registry.hpp>

#include "Ignore.h"
#include "IgnoreMgr.h"
#include "Hostmask.h"
#include "IRCUtils.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TIgnore::TIgnore()
{
	Exclude = false;
}
//---------------------------------------------------------------------------
TIgnore::TIgnore( TIgnore *copy )
{
	*this = *copy;
}
//---------------------------------------------------------------------------
TIgnore& TIgnore::operator =( const TIgnore& rhs )
{
	Mask    = rhs.Mask;
	Events  = rhs.Events;
    Exclude = rhs.Exclude;

    return( *this );
}
//---------------------------------------------------------------------------
bool TIgnore::Match( HostMask& mask )
{
	return( mask.Match( Mask ));
}
//---------------------------------------------------------------------------
bool TIgnore::Ignore( AnsiString& msg, int event )
{
	bool ignore = !Exclude && Events.IsSet( event & ~IGN_CONTROL_CODE );

	if( !ignore && Events.IsSet( IGN_CONTROL_CODE ))
    	msg = IRCUtils::StripCodes( msg );

    return( ignore );
}
//---------------------------------------------------------------------------
void TIgnore::Load( TRegistry *reg, AnsiString key )
{
	if( reg->OpenKey( key, false )) {

    	Mask = reg->ReadString( "Mask" );

        try{ Events.Set( reg->ReadInteger( "Events" )); } catch(...) {}
        try{ Exclude = reg->ReadBool( "Exclude" ); } catch(...) { Exclude = false; }

    	reg->CloseKey();
    }
}
//---------------------------------------------------------------------------
void TIgnore::Save( TRegistry *reg, AnsiString key )
{
	if( reg->OpenKey( key, true )) {

    	try {

    		reg->WriteString( "Mask", Mask );

        	reg->WriteInteger( "Events", Events.Get() );
            reg->WriteBool( "Exclude", Exclude );
        }
        catch(...) {
        }

    	reg->CloseKey();
    }
}
//---------------------------------------------------------------------------
