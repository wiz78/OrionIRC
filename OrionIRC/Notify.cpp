/* $Id: Notify.cpp,v 1.2 2004/12/21 21:42:28 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <registry.hpp>

#include "Notify.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TNotify::TNotify()
{
	On = false;
}
//---------------------------------------------------------------------------
TNotify::TNotify( AnsiString nick )
{
	Nick = nick;
    On   = false;
}
//---------------------------------------------------------------------------
TNotify::TNotify( TNotify *copy )
{
	*this = *copy;
}
//---------------------------------------------------------------------------
TNotify& TNotify::operator =( const TNotify& rhs )
{
	Nick = rhs.Nick;
    On   = rhs.On;

	return( *this );
}
//---------------------------------------------------------------------------
void TNotify::Load( TRegistry *reg, AnsiString key )
{
	if( reg->OpenKey( key, false )) {

    	Nick = reg->ReadString( "Nick" );

    	reg->CloseKey();
    }
}
//---------------------------------------------------------------------------
void TNotify::Save( TRegistry *reg, AnsiString key )
{
	if( reg->OpenKey( key, true )) {

    	try {
            reg->WriteString( "Nick", Nick );
        }
        catch(...) {
        }

    	reg->CloseKey();
    }
}
//---------------------------------------------------------------------------
AnsiString TNotify::GetNick( void ) const
{
	return( Nick );
}
//---------------------------------------------------------------------------
bool TNotify::IsOn( void ) const
{
	return( On );
}
//---------------------------------------------------------------------------
void TNotify::SetNick( AnsiString str )
{
	Nick = str;
}
//---------------------------------------------------------------------------
void TNotify::SetIsOn( bool set )
{
	On = set;
}
//---------------------------------------------------------------------------

