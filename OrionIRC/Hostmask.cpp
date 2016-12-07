/* $Id: Hostmask.cpp,v 1.5 2005/04/27 20:12:21 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <winsock2.h>

#include "Hostmask.h"
#include "MatchStr.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
HostMask::HostMask()
{
}
//---------------------------------------------------------------------------
HostMask::HostMask( AnsiString mask )
{
	Set( mask );
}
//---------------------------------------------------------------------------
HostMask::HostMask( AnsiString nick, AnsiString user, AnsiString host )
{
	Nick = nick;
    User = user;
    Host = host;
}
//---------------------------------------------------------------------------
AnsiString HostMask::Get( void )
{
	AnsiString	mask = Nick + "!" + User + "@" + Host;

    return(( mask.Length() > 2 ) ? mask : AnsiString( "" ));
}
//---------------------------------------------------------------------------
AnsiString HostMask::GetUserHost( void )
{
	AnsiString	mask = User + "@" + Host;

    return(( mask.Length() > 1 ) ? mask : AnsiString( "" ));
}
//---------------------------------------------------------------------------
void HostMask::Set( AnsiString mask )
{
	int pos;

    Clear();

	Nick = mask;
    User = "";
    Host = "";

    if( pos = Nick.Pos( "!" )) {

    	User = Nick.SubString( pos + 1, Nick.Length() );

    	Nick.Delete( pos, Nick.Length() );

        if( pos = User.Pos( "@" )) {

        	Host = User.SubString( pos + 1, User.Length() );

            User.Delete( pos, User.Length() );
        }
    }
}
//---------------------------------------------------------------------------
void HostMask::Clear( void )
{
	Nick = "";
    User = "";
    Host = "";
}
//---------------------------------------------------------------------------
AnsiString HostMask::GetMask( MaskType type )
{
	AnsiString ret, user = User;

	switch( user[1] ) {

    	case '~':
        case '+':
		case '=':
		case '^':
        case '-':
        	user.Delete( 1, 1 );
        	break;
    }

	switch( type ) {

    	case MASK_NICK_USER_HOST:
        	ret = Nick + "!" + user + "@" + Host;
        	break;

    	case MASK_NICK_USER_TLD:
        	ret = Nick + "!" + user + "@" + GetTldMask();
        	break;

    	case MASK_USER_HOST:
        	ret = "*!" + user + "@" + Host;
        	break;

        case MASK_USER_TLD:
        	ret = "*!" + user + "@" + GetTldMask();
        	break;

        case MASK_HOST:
        	ret = "*!*@" + Host;
        	break;

    	case MASK_TLD:
        	ret = "*!*@" + GetTldMask();
        	break;
    }

    return( ret );
}
//---------------------------------------------------------------------------
AnsiString HostMask::GetTldMask( void )
{
	AnsiString ret = Host;

    if( inet_addr( ret.c_str() ) != INADDR_NONE ) {

		ret.Delete( ret.LastDelimiter( "." ) + 1, ret.Length() );

        ret += "*";

    } else {
		int pos = ret.Pos( "." );

        if( pos ) {
        	AnsiString domain = ret.SubString( pos + 1, ret.Length() );

            if( domain.Pos( "." ))
	        	ret = "*." + domain;
        }
    }

    return( ret );
}
//---------------------------------------------------------------------------
bool HostMask::Match( AnsiString pattern )
{
	AnsiString	patt = pattern.LowerCase();
    AnsiString  user = Get().LowerCase();

    return( MatchStr( user.c_str(), patt.c_str(), false ));
}
//---------------------------------------------------------------------------

