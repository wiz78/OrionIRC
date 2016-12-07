/* $Id: ScriptEvent.cpp,v 1.6 2005/02/08 18:59:19 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <stdarg>

#include "ScriptEvent.h"
#include "ScriptClasses.h"
#include "ChanWnd.h"
#include "ServerWnd.h"
#include "QueryWnd.h"

class TDCCTransfer;

//---------------------------------------------------------------------------
#pragma package(smart_init)

#if SCRIPTING

//---------------------------------------------------------------------------
TScriptEvent::TScriptEvent( zval *callback, ScriptEvent event )
{
	Callback = callback;
    Event    = event;

	zval_add_ref( &Callback );
}
//---------------------------------------------------------------------------
TScriptEvent::~TScriptEvent()
{
	zval_ptr_dtor( &Callback );
}
//---------------------------------------------------------------------------
int TScriptEvent::Fire( va_list va )
{
	int		ret = 1;
    int		status, num_args = 0;
    zval   	retval, **args;

    TSRMLS_FETCH();

    args   = GetArgs( va, &num_args );
   	status = call_user_function( OS_GetFunctionTable(), NULL, Callback,
    							 &retval, num_args, args TSRMLS_CC );

	for( int i = 0; i < num_args; i++ )
    	zval_ptr_dtor( &args[ i ] );

    if( status == SUCCESS ) {

    	convert_to_long( &retval );

        ret = Z_LVAL( retval );
    }

    delete[] args;

    return( ret );
}
//---------------------------------------------------------------------------
zval **TScriptEvent::GetArgs( va_list va, int *num_args )
{
	zval **ret = NULL;

    *num_args = 0;

	switch( Event ) {

        case SE_ACTION:         // args: win (channel | query), hostmask, text
        case SE_TEXT:        	// args: win (channel | query), hostmask, text
        case SE_TOPIC:          // args: channel, hostmask, topic
        case SE_MODE:			// args: channel, hostmask, mode
        case SE_INVITE:        	// args: channel, hostmask, channel name
        case SE_CTCP:			// args: server, hostmask, msg
        case SE_CTCPREPLY:		// args: server, hostmask, msg
        case SE_DCC:			// args: server, hostmask, msg
        case SE_WALLOPS:		// args: server, hostmask, msg
        case SE_NOTICE:			// args: server, hostmask, msg
	       	ret = GetWinHostmaskString( va, num_args );
        	break;

        case SE_ACTIVE:			// args: win (channel | server | query)
        case SE_CLOSE:			// args: win (channel | server | query)
        case SE_CONNECT:		// args: server
        case SE_LOGON:  		// args: server
        case SE_OPEN:			// args: win (channel | server | query)
        	ret = GetWin( va, num_args );
        	break;

        case SE_JOIN:			// args: channel, hostmask
        case SE_PART:           // args: channel, hostmask
        case SE_QUIT:       	// args: channel, hostmask
        	ret = GetWinHostmask( va, num_args );
        	break;

        case SE_CHAT:           // args: query, msg
        case SE_CONNECTFAIL:	// args: server, error
        case SE_DISCONNECT:		// args: server, msg
        case SE_ERROR:        	// args: server, msg
        case SE_INPUT:          // args: win ( channel | server | query ), string
        case SE_PING:			// args: server, msg
        case SE_PONG:			// args: server, msg
        case SE_RAW:			// args: server, msg
        case SE_SNOTICE:        // args: server, msg
        case SE_USERMODE:		// args: server, mode
        	ret = GetWinString( va, num_args );
        	break;

        case SE_APPACTIVE:      // no args
        case SE_APPDEACTIVE:    // no args
        case SE_EXIT:        	// no args
        	break;

        case SE_FILERCVD:       // args: DCCTransfer
        case SE_FILESENT:       // args: DCCTransfer
        case SE_GETFAIL:        // args: DCCTransfer
        case SE_SENDFAIL:		// args: DCCTransfer
        	ret = GetDCCTransfer( va, num_args );
        	break;

        case SE_BAN:			// args: channel, hostmask, string (user)
        case SE_UNBAN:        	// args: channel, hostmask, string (user)
        case SE_OP:     		// args: channel, opper (hostmask), opped (user)
        case SE_DEOP:           // args: channel, hostmask, string
        case SE_VOICE:          // args: channel, hostmask, string
        case SE_DEVOICE:        // args: channel, hostmask, string
        	ret = GetWinHostmaskString( va, num_args );
        	break;

        case SE_KICK: 			// args: channel, kicker (hostmask), kicked (string), reason
        	ret = GetWinHostmaskStringString( va, num_args );
        	break;

        case SE_NICK: 			// args: channel, old nick (user), new nick (user)
        case SE_NOTIFY:			// args: server, nick, on/off
        	ret = GetWinStringString( va, num_args );
        	break;

        case SE_SIGNAL:			// ***
        	break;
    }

    return( ret );
}
//---------------------------------------------------------------------------
zval **TScriptEvent::GetWinHostmaskString( va_list va, int *num_args )
{
	zval  **ret = new zval *[ 3 ];

    *num_args = 3;

    ret[ 0 ] = GetWin( va_arg( va, TForm * ));
    ret[ 1 ] = ScriptClasses::CreateHostMask( va_arg( va, HostMask * ));

    ALLOC_ZVAL( ret[ 2 ] );
    ZVAL_STRING( ret[ 2 ], va_arg( va, char * ), TRUE );

    return( ret );
}
//---------------------------------------------------------------------------
zval **TScriptEvent::GetWin( va_list va, int *num_args )
{
	zval  **ret = new zval *[ 1 ];

    *num_args = 1;

    ret[ 0 ] = GetWin( va_arg( va, TForm * ) );

    return( ret );
}
//---------------------------------------------------------------------------
zval **TScriptEvent::GetWinHostmask( va_list va, int *num_args )
{
	zval  **ret = new zval *[ 2 ];

    *num_args = 2;

    ret[ 0 ] = GetWin( va_arg( va, TForm * ));
    ret[ 1 ] = ScriptClasses::CreateHostMask( va_arg( va, HostMask * ));

    return( ret );
}
//---------------------------------------------------------------------------
zval **TScriptEvent::GetWinHostmaskStringString( va_list va, int *num_args )
{
	zval  **ret = new zval *[ 4 ];

    *num_args = 4;

    ret[ 0 ] = GetWin( va_arg( va, TForm * ));
    ret[ 1 ] = ScriptClasses::CreateHostMask( va_arg( va, HostMask * ));

    ALLOC_ZVAL( ret[ 2 ] );
    ZVAL_STRING( ret[ 2 ], va_arg( va, char * ), TRUE );

    ALLOC_ZVAL( ret[ 3 ] );
    ZVAL_STRING( ret[ 3 ], va_arg( va, char * ), TRUE );

    return( ret );
}
//---------------------------------------------------------------------------
zval **TScriptEvent::GetWinString( va_list va, int *num_args )
{
	zval  **ret = new zval *[ 2 ];

    *num_args = 2;

    ret[ 0 ] = GetWin( va_arg( va, TForm * ));

    ALLOC_ZVAL( ret[ 1 ] );
    ZVAL_STRING( ret[ 1 ], va_arg( va, char * ), TRUE );

    return( ret );
}
//---------------------------------------------------------------------------
zval **TScriptEvent::GetWinStringString( va_list va, int *num_args )
{
	zval  **ret = new zval *[ 3 ];

    *num_args = 3;

    ret[ 0 ] = GetWin( va_arg( va, TForm * ));

    ALLOC_ZVAL( ret[ 1 ] );
    ZVAL_STRING( ret[ 1 ], va_arg( va, char * ), TRUE );

    ALLOC_ZVAL( ret[ 2 ] );
    ZVAL_STRING( ret[ 2 ], va_arg( va, char * ), TRUE );

    return( ret );
}
//---------------------------------------------------------------------------
zval **TScriptEvent::GetDCCTransfer( va_list va, int *num_args )
{
	zval  **ret = new zval *[ 1 ];

    *num_args = 1;

    ret[ 0 ] = ScriptClasses::CreateDCCTransfer( va_arg( va, TDCCTransfer * ));

    return( ret );
}
//---------------------------------------------------------------------------
zval *TScriptEvent::GetWin( TForm *win )
{
	zval *ret;

    if( TChanWin *channel = dynamic_cast<TChanWin *>( win ))
    	ret = ScriptClasses::CreateChannel( channel );
    else if( TServerWin *server = dynamic_cast<TServerWin *>( win ))
    	ret = ScriptClasses::CreateServer( server );
    else if( TQueryWin *query = dynamic_cast<TQueryWin *>( win ))
    	ret = ScriptClasses::CreateQuery( query );
    else {
    	ALLOC_ZVAL( ret );
        ZVAL_NULL( ret );
    }

    return( ret );
}
//---------------------------------------------------------------------------
#endif /* SCRIPTING */
//---------------------------------------------------------------------------

