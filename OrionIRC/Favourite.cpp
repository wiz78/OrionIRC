/* $Id: Favourite.cpp,v 1.2 2003/05/25 10:48:29 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <registry.hpp>

#include "Favourite.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TFavourite::TFavourite( TFavourite *copy )
{
	*this = *copy;
}
//---------------------------------------------------------------------------
TFavourite& TFavourite::operator =( const TFavourite &rhs )
{
	Channel		= rhs.Channel;
	Description = rhs.Description;
    Network     = rhs.Network;
    Password	= rhs.Password;

    return( *this );
}
//---------------------------------------------------------------------------
void TFavourite::Save( TRegistry *reg, AnsiString key )
{
	if( reg->OpenKey( key, true )) {

    	try {

            reg->WriteString( "Channel",     Channel     );
            reg->WriteString( "Description", Description );
            reg->WriteString( "Network",     Network     );
            reg->WriteString( "Password",    Password    );
        }
        catch(...) {
        }

    	reg->CloseKey();
    }
}
//---------------------------------------------------------------------------
void TFavourite::Load( TRegistry *reg, AnsiString key )
{
	if( reg->OpenKey( key, false )) {

    	Channel     = reg->ReadString( "Channel"     );
    	Description = reg->ReadString( "Description" );
    	Network     = reg->ReadString( "Network"     );
    	Password    = reg->ReadString( "Password"    );

    	reg->CloseKey();
    }
}
//---------------------------------------------------------------------------

