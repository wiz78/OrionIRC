/* $Id: ScriptClasses.cpp,v 1.6 2005/02/08 18:59:19 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ScriptClasses.h"
#include "Main.h"
#include "ServerWnd.h"
#include "ChanWnd.h"
#include "QueryWnd.h"
#include "User.h"
#include "HostMask.h"
#include "DCCTransfer.h"
#include "Commands.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

#if SCRIPTING

static zend_class_entry    *UserCE;
static zend_class_entry    *HostMaskCE;
static zend_class_entry    *ChannelCE;
static zend_class_entry    *ServerCE;
static zend_class_entry    *QueryCE;
static zend_class_entry    *DCCTransferCE;

//---------------------------------------------------------------------------
static void GetUserProp( zval *object, char *name, int name_len, zval *return_value TSRMLS_DC )
{
	*return_value = *zend_read_property( UserCE, object, name, name_len, 0 TSRMLS_CC );

	zval_copy_ctor( return_value );
}
//---------------------------------------------------------------------------
static ZEND_METHOD( User, getNick )
{
	METHOD_NOTSTATIC_NUMPARAMS( 0 );

    GetUserProp( getThis(), "nick", sizeof( "nick" ) - 1, return_value TSRMLS_CC );
}
//---------------------------------------------------------------------------
static ZEND_METHOD( User, getHost )
{
	METHOD_NOTSTATIC_NUMPARAMS( 0 );

    GetUserProp( getThis(), "host", sizeof( "host" ) - 1, return_value TSRMLS_CC );
}
//---------------------------------------------------------------------------
static ZEND_METHOD( User, getAwayReason )
{
	METHOD_NOTSTATIC_NUMPARAMS( 0 );

    GetUserProp( getThis(), "awayReason", sizeof( "awayReason" ) - 1, return_value TSRMLS_CC );
}
//---------------------------------------------------------------------------
static ZEND_METHOD( User, isOp )
{
	METHOD_NOTSTATIC_NUMPARAMS( 0 );

    GetUserProp( getThis(), "op", sizeof( "op" ) - 1, return_value TSRMLS_CC );
}
//---------------------------------------------------------------------------
static ZEND_METHOD( User, isOwner )
{
	METHOD_NOTSTATIC_NUMPARAMS( 0 );

    GetUserProp( getThis(), "owner", sizeof( "owner" ) - 1, return_value TSRMLS_CC );
}
//---------------------------------------------------------------------------
static ZEND_METHOD( User, isChanAdmin )
{
	METHOD_NOTSTATIC_NUMPARAMS( 0 );

    GetUserProp( getThis(), "chanAdmin", sizeof( "chanAdmin" ) - 1, return_value TSRMLS_CC );
}
//---------------------------------------------------------------------------
static ZEND_METHOD( User, isAway )
{
	METHOD_NOTSTATIC_NUMPARAMS( 0 );

    GetUserProp( getThis(), "away", sizeof( "away" ) - 1, return_value TSRMLS_CC );
}
//---------------------------------------------------------------------------
static ZEND_METHOD( User, hasVoice )
{
	METHOD_NOTSTATIC_NUMPARAMS( 0 );

    GetUserProp( getThis(), "voice", sizeof( "voice" ) - 1, return_value TSRMLS_CC );
}
//---------------------------------------------------------------------------
static void GetHostMaskProp( zval *object, char *name, int name_len, zval *return_value TSRMLS_DC )
{
	*return_value = *zend_read_property( HostMaskCE, object, name, name_len, 0 TSRMLS_CC );

	zval_copy_ctor( return_value );
}
//---------------------------------------------------------------------------
static HostMask GetHostMask( zval *object TSRMLS_DC )
{
    zval *nick, *user, *host;

	nick = zend_read_property( HostMaskCE, object, "nick", sizeof( "nick" ) - 1, 0 TSRMLS_CC );
	user = zend_read_property( HostMaskCE, object, "user", sizeof( "user" ) - 1, 0 TSRMLS_CC );
	host = zend_read_property( HostMaskCE, object, "host", sizeof( "host" ) - 1, 0 TSRMLS_CC );

    HostMask mask( Z_STRVAL_P( nick ), Z_STRVAL_P( user ), Z_STRVAL_P( host ));

    return( mask );
}
//---------------------------------------------------------------------------
static ZEND_METHOD( HostMask, get )
{
	METHOD_NOTSTATIC_NUMPARAMS( 0 );
    RETVAL_STRING( GetHostMask( getThis() TSRMLS_CC ).Get().c_str(), TRUE );
}
//---------------------------------------------------------------------------
static ZEND_METHOD( HostMask, Match )
{
	char   *str;
    int		str_len;

	METHOD_NOTSTATIC_NUMPARAMS( 1 );

	if( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s", &str, &str_len ) == SUCCESS )
	    RETVAL_BOOL( GetHostMask( getThis() TSRMLS_CC ).Match( str ));
}
//---------------------------------------------------------------------------
static ZEND_METHOD( HostMask, getMask )
{
    int	type;

	METHOD_NOTSTATIC_NUMPARAMS( 1 );

	if( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "l", &type ) == SUCCESS )
	    RETVAL_STRING( GetHostMask( getThis() TSRMLS_CC ).GetMask( static_cast<MaskType>( type )).c_str(), TRUE );
}
//---------------------------------------------------------------------------
static ZEND_METHOD( HostMask, getNick )
{
	METHOD_NOTSTATIC_NUMPARAMS( 0 );

    GetHostMaskProp( getThis(), "nick", sizeof( "nick" ) - 1, return_value TSRMLS_CC );
}
//---------------------------------------------------------------------------
static ZEND_METHOD( HostMask, getHost )
{
	METHOD_NOTSTATIC_NUMPARAMS( 0 );

    GetHostMaskProp( getThis(), "host", sizeof( "host" ) - 1, return_value TSRMLS_CC );
}
//---------------------------------------------------------------------------
static ZEND_METHOD( HostMask, getUser )
{
	METHOD_NOTSTATIC_NUMPARAMS( 0 );

    GetHostMaskProp( getThis(), "user", sizeof( "user" ) - 1, return_value TSRMLS_CC );
}
//---------------------------------------------------------------------------
static ZEND_METHOD( HostMask, set )
{
	char   *str;
    int		str_len;

	METHOD_NOTSTATIC_NUMPARAMS( 1 );

	if( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s", &str, &str_len ) == SUCCESS ) {
    	HostMask mask( str );

	    SET_STRING_PROP( HostMaskCE, getThis(), "nick", mask.GetNick().c_str() );
	    SET_STRING_PROP( HostMaskCE, getThis(), "host", mask.GetHost().c_str() );
	    SET_STRING_PROP( HostMaskCE, getThis(), "user", mask.GetUser().c_str() );
    }
}
//---------------------------------------------------------------------------
static ZEND_METHOD( HostMask, setNick )
{
	char   *str;
    int		str_len;

	METHOD_NOTSTATIC_NUMPARAMS( 1 );

	if( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s", &str, &str_len ) == SUCCESS )
	    SET_STRING_PROP( HostMaskCE, getThis(), "nick", str );
}
//---------------------------------------------------------------------------
static ZEND_METHOD( HostMask, setHost )
{
	char   *str;
    int		str_len;

	METHOD_NOTSTATIC_NUMPARAMS( 1 );

	if( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s", &str, &str_len ) == SUCCESS )
	    SET_STRING_PROP( HostMaskCE, getThis(), "host", str );
}
//---------------------------------------------------------------------------
static ZEND_METHOD( HostMask, setUser )
{
	char   *str;
    int		str_len;

	METHOD_NOTSTATIC_NUMPARAMS( 1 );

	if( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s", &str, &str_len ) == SUCCESS )
	    SET_STRING_PROP( HostMaskCE, getThis(), "user", str );
}
//---------------------------------------------------------------------------
static void GetServerProp( zval *object, char *name, int name_len, zval *return_value TSRMLS_DC )
{
	*return_value = *zend_read_property( ServerCE, object, name, name_len, 0 TSRMLS_CC );

	zval_copy_ctor( return_value );
}
//---------------------------------------------------------------------------
static ZEND_METHOD( Server, getHost )
{
	METHOD_NOTSTATIC_NUMPARAMS( 0 );

    GetServerProp( getThis(), "host", sizeof( "host" ) - 1, return_value TSRMLS_CC );
}
//---------------------------------------------------------------------------
static ZEND_METHOD( Server, cmd )
{
	zval   	   *server;
    TServerWin *serverWin;
    bool		ok = false;
    char	   *str;
    int			str_len;

	METHOD_NOTSTATIC_NUMPARAMS( 1 );

    server    = zend_read_property( ServerCE, getThis(), "host", sizeof( "host" ) - 1, 0 TSRMLS_CC );
    serverWin = MainWin->FindServerWin( Z_STRVAL_P( server ));

    if( !serverWin )
    	zend_error( E_ERROR, "The %s server window has been closed!", Z_STRVAL_P( server ));
    else if( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s", &str, &str_len ) == SUCCESS ) {

    	HandleCommand( serverWin, NULL, str );

    	ok = true;
    }

    RETVAL_BOOL( ok );
}
//---------------------------------------------------------------------------
static void GetChannelProp( zval *object, char *name, int name_len, zval *return_value TSRMLS_DC )
{
	*return_value = *zend_read_property( ChannelCE, object, name, name_len, 0 TSRMLS_CC );

	zval_copy_ctor( return_value );
}
//---------------------------------------------------------------------------
static TChanWin *GetChannelWin( zval *obj TSRMLS_DC )
{
	zval   	   *name, *server;
    TServerWin *serverWin;
    TChanWin   *chanWin = NULL;

    name   = zend_read_property( ChannelCE, obj, "name", sizeof( "name" ) - 1, 0 TSRMLS_CC );
    server = zend_read_property( ChannelCE, obj, "server", sizeof( "server" ) - 1, 0 TSRMLS_CC );

    if( name && ( serverWin = MainWin->FindServerWin( Z_STRVAL_P( server ))))
        chanWin = serverWin->FindChannelWin( Z_STRVAL_P( name ));

    if( !chanWin )
    	zend_error( E_ERROR,
        			"The window for channel %s on %s has been closed",
        			Z_STRVAL_P( name ), Z_STRVAL_P( server ));

    return( chanWin );
}
//---------------------------------------------------------------------------
static ZEND_METHOD( Channel, getName )
{
	METHOD_NOTSTATIC_NUMPARAMS( 0 );

    GetChannelProp( getThis(), "name", sizeof( "name" ) - 1, return_value TSRMLS_CC );
}
//---------------------------------------------------------------------------
static ZEND_METHOD( Channel, getServer )
{
	METHOD_NOTSTATIC_NUMPARAMS( 0 );

	zval   	   *server;
    TServerWin *serverWin;

    server    = zend_read_property( ChannelCE, getThis(), "server", sizeof( "server" ) - 1, 0 TSRMLS_CC );
    serverWin = MainWin->FindServerWin( Z_STRVAL_P( server ));

    if( serverWin ) {

    	server = ScriptClasses::CreateServer( serverWin );

    	RETVAL_ZVAL( server, FALSE, FALSE );

    } else
    	RETVAL_NULL();
}
//---------------------------------------------------------------------------
static ZEND_METHOD( Channel, getUsers )
{
    TChanWin   *chanWin;

	METHOD_NOTSTATIC_NUMPARAMS( 0 );

    chanWin = GetChannelWin( getThis() TSRMLS_CC );

    if( chanWin ) {
        TUser  *user;
        int		i = 0;
        zval   *ret;

        ALLOC_ZVAL( ret );
        array_init( ret );

        while( user = chanWin->GetUser( i++ ))
            add_next_index_zval( ret, ScriptClasses::CreateUser( user TSRMLS_CC ));

        RETVAL_ZVAL( ret, FALSE, FALSE );

    } else
    	RETVAL_NULL();
}
//---------------------------------------------------------------------------
static ZEND_METHOD( Channel, say )
{
	char   *str;
    int		str_len;

	METHOD_NOTSTATIC_NUMPARAMS( 1 );

	if( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s", &str, &str_len ) == SUCCESS ) {
		TChanWin *win = GetChannelWin( getThis() TSRMLS_CC );

        if( win )
        	win->Say( str );
	}
}
//---------------------------------------------------------------------------
static ZEND_METHOD( Channel, getTopic )
{
	METHOD_NOTSTATIC_NUMPARAMS( 0 );

	TChanWin *win = GetChannelWin( getThis() TSRMLS_CC );

    if( win ) {
    	RETVAL_STRING( win->GetTopic().c_str(), TRUE );
    } else {
    	RETVAL_NULL();
    }
}
//---------------------------------------------------------------------------
static ZEND_METHOD( Channel, setTopic )
{
	char   *str;
    int		str_len;

	METHOD_NOTSTATIC_NUMPARAMS( 1 );

	if( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s", &str, &str_len ) == SUCCESS ) {
		TChanWin *win = GetChannelWin( getThis() TSRMLS_CC );

        if( win )
	       	HandleCommand( win->GetServerWin(), win, "TOPIC " + AnsiString( str ));
	}
}
//---------------------------------------------------------------------------
static ZEND_METHOD( Channel, part )
{
	METHOD_NOTSTATIC_NUMPARAMS( 0 );

	TChanWin *win = GetChannelWin( getThis() TSRMLS_CC );

    if( win )
        HandleCommand( win->GetServerWin(), win, "PART " + win->GetChannel() );
}
//---------------------------------------------------------------------------
static ZEND_METHOD( Channel, getModes )
{
	METHOD_NOTSTATIC_NUMPARAMS( 0 );

	TChanWin *win = GetChannelWin( getThis() TSRMLS_CC );

    if( win ) {
    	AnsiString modes = win->ChanData.GetModesStr();

    	RETVAL_STRING( modes.c_str(), TRUE );
    } else {
    	RETVAL_NULL();
    }
}
//---------------------------------------------------------------------------
static ZEND_METHOD( Channel, getKeyword )
{
	METHOD_NOTSTATIC_NUMPARAMS( 0 );

	TChanWin *win = GetChannelWin( getThis() TSRMLS_CC );

    if( win ) {
    	RETVAL_STRING( win->ChanData.Keyword.c_str(), TRUE );
    } else {
    	RETVAL_NULL();
    }
}
//---------------------------------------------------------------------------
static ZEND_METHOD( Channel, getLimit )
{
	METHOD_NOTSTATIC_NUMPARAMS( 0 );

	TChanWin *win = GetChannelWin( getThis() TSRMLS_CC );

    if( win ) {
    	RETVAL_LONG( win->ChanData.Limit );
    } else {
    	RETVAL_NULL();
    }
}
//---------------------------------------------------------------------------
static void GetQueryProp( zval *object, char *name, int name_len, zval *return_value TSRMLS_DC )
{
	*return_value = *zend_read_property( QueryCE, object, name, name_len, 0 TSRMLS_CC );

	zval_copy_ctor( return_value );
}
//---------------------------------------------------------------------------
static TQueryWin *GetQueryWin( zval *obj TSRMLS_DC )
{
	zval   	   *name, *server;
    TServerWin *serverWin;
    TQueryWin  *win = NULL;

    name   = zend_read_property( QueryCE, obj, "nick", sizeof( "nick" ) - 1, 0 TSRMLS_CC );
    server = zend_read_property( QueryCE, obj, "server", sizeof( "server" ) - 1, 0 TSRMLS_CC );

    if( name && ( serverWin = MainWin->FindServerWin( Z_STRVAL_P( server ))))
        win = serverWin->FindQueryWin( Z_STRVAL_P( name ), false, false );

    if( !win )
    	zend_error( E_ERROR,
        			"The query with %s on %s has been closed",
        			Z_STRVAL_P( name ), Z_STRVAL_P( server ));

    return( win );
}
//---------------------------------------------------------------------------
static ZEND_METHOD( Query, getNick )
{
	METHOD_NOTSTATIC_NUMPARAMS( 0 );

    GetQueryProp( getThis(), "nick", sizeof( "nick" ) - 1, return_value TSRMLS_CC );
}
//---------------------------------------------------------------------------
static ZEND_METHOD( Query, getServer )
{
	METHOD_NOTSTATIC_NUMPARAMS( 0 );

	zval   	   *server;
    TServerWin *serverWin;

    server    = zend_read_property( QueryCE, getThis(), "server", sizeof( "server" ) - 1, 0 TSRMLS_CC );
    serverWin = MainWin->FindServerWin( Z_STRVAL_P( server ));

    if( serverWin ) {

    	server = ScriptClasses::CreateServer( serverWin );

    	RETVAL_ZVAL( server, FALSE, FALSE );

    } else
    	RETVAL_NULL();
}
//---------------------------------------------------------------------------
static ZEND_METHOD( Query, say )
{
	char   *str;
    int		str_len;

	METHOD_NOTSTATIC_NUMPARAMS( 1 );

	if( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s", &str, &str_len ) == SUCCESS ) {
		TQueryWin *win = GetQueryWin( getThis() TSRMLS_CC );

        if( win )
        	win->Say( str );
	}
}
//---------------------------------------------------------------------------
static ZEND_METHOD( Query, close )
{
	METHOD_NOTSTATIC_NUMPARAMS( 0 );

    TQueryWin *win = GetQueryWin( getThis() TSRMLS_CC );

    if( win )
        win->Close();
}
//---------------------------------------------------------------------------
static ZEND_METHOD( Query, isDCC )
{
	METHOD_NOTSTATIC_NUMPARAMS( 0 );

	TQueryWin  *win = GetQueryWin( getThis() TSRMLS_CC );
    bool		dcc = false;

    if( win )
    	dcc = win->DCCInProgress();

    RETVAL_BOOL( dcc );
}
//---------------------------------------------------------------------------
static void GetDCCTransferProp( zval *object, char *name, int name_len, zval *return_value TSRMLS_DC )
{
	*return_value = *zend_read_property( DCCTransferCE, object, name, name_len, 0 TSRMLS_CC );

	zval_copy_ctor( return_value );
}
//---------------------------------------------------------------------------
static ZEND_METHOD( DCCTransfer, getNick )
{
	METHOD_NOTSTATIC_NUMPARAMS( 0 );

    GetQueryProp( getThis(), "nick", sizeof( "nick" ) - 1, return_value TSRMLS_CC );
}
//---------------------------------------------------------------------------
static ZEND_METHOD( DCCTransfer, getServer )
{
	METHOD_NOTSTATIC_NUMPARAMS( 0 );

    GetQueryProp( getThis(), "server", sizeof( "server" ) - 1, return_value TSRMLS_CC );
}
//---------------------------------------------------------------------------
static ZEND_METHOD( DCCTransfer, getFile )
{
	METHOD_NOTSTATIC_NUMPARAMS( 0 );

    GetQueryProp( getThis(), "file", sizeof( "file" ) - 1, return_value TSRMLS_CC );
}
//---------------------------------------------------------------------------
static ZEND_METHOD( DCCTransfer, getSize )
{
	METHOD_NOTSTATIC_NUMPARAMS( 0 );

    GetQueryProp( getThis(), "size", sizeof( "size" ) - 1, return_value TSRMLS_CC );
}
//---------------------------------------------------------------------------
static zend_function_entry 	UserMethods[] =
{
	ZEND_ME( User, getNick, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME( User, getHost, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME( User, getAwayReason, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME( User, isOp, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME( User, isOwner, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME( User, isChanAdmin, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME( User, isAway, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME( User, hasVoice, NULL, ZEND_ACC_PUBLIC )
	{NULL, NULL, NULL}
};
//---------------------------------------------------------------------------
static zend_function_entry 	HostMaskMethods[] =
{
	ZEND_ME( HostMask, get, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME( HostMask, Match, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME( HostMask, getMask, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME( HostMask, getNick, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME( HostMask, getHost, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME( HostMask, getUser, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME( HostMask, setNick, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME( HostMask, setHost, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME( HostMask, setUser, NULL, ZEND_ACC_PUBLIC )
	{NULL, NULL, NULL}
};
//---------------------------------------------------------------------------
static zend_function_entry 	ServerMethods[] =
{
	ZEND_ME( Server, getHost, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME( Server, cmd, NULL, ZEND_ACC_PUBLIC )
	{NULL, NULL, NULL}
};
//---------------------------------------------------------------------------
static zend_function_entry 	ChannelMethods[] =
{
	ZEND_ME( Channel, getName, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME( Channel, getUsers, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME( Channel, getServer, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME( Channel, say, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME( Channel, getTopic, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME( Channel, setTopic, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME( Channel, part, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME( Channel, getModes, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME( Channel, getKeyword, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME( Channel, getLimit, NULL, ZEND_ACC_PUBLIC )
	{NULL, NULL, NULL}
};
//---------------------------------------------------------------------------
static zend_function_entry 	QueryMethods[] =
{
	ZEND_ME( Query, getNick, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME( Query, getServer, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME( Query, say, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME( Query, close, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME( Query, isDCC, NULL, ZEND_ACC_PUBLIC )
	{NULL, NULL, NULL}
};
//---------------------------------------------------------------------------
static zend_function_entry 	DCCTransferMethods[] =
{
	ZEND_ME( DCCTransfer, getNick, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME( DCCTransfer, getServer, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME( DCCTransfer, getFile, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME( DCCTransfer, getSize, NULL, ZEND_ACC_PUBLIC )
	{NULL, NULL, NULL}
};
//---------------------------------------------------------------------------
void ScriptClasses::Register( void )
{
    TSRMLS_FETCH();

	InitUser( TSRMLS_C );
    InitHostMask( TSRMLS_C );
    InitServer( TSRMLS_C );
    InitChannel( TSRMLS_C );
    InitQuery( TSRMLS_C );
    InitDCCTransfer( TSRMLS_C );
}
//---------------------------------------------------------------------------
void ScriptClasses::InitUser( TSRMLS_D )
{
	zend_class_entry entry;

	INIT_CLASS_ENTRY( entry, "User", UserMethods );

	UserCE = zend_register_internal_class( &entry TSRMLS_CC );

	zend_declare_property_string( UserCE, "nick", sizeof( "nick" ) - 1, "", ZEND_ACC_PRIVATE TSRMLS_CC );
	zend_declare_property_string( UserCE, "host", sizeof( "host" ) - 1, "", ZEND_ACC_PRIVATE TSRMLS_CC );
	zend_declare_property_string( UserCE, "awayReason", sizeof( "awayReason" ) - 1, "", ZEND_ACC_PRIVATE TSRMLS_CC );
	zend_declare_property_long( UserCE, "op", sizeof( "op" ) - 1, 0, ZEND_ACC_PRIVATE TSRMLS_CC );
	zend_declare_property_long( UserCE, "owner", sizeof( "owner" ) - 1, 0, ZEND_ACC_PRIVATE TSRMLS_CC );
	zend_declare_property_long( UserCE, "chanAdmin", sizeof( "chanAdmin" ) - 1, 0, ZEND_ACC_PRIVATE TSRMLS_CC );
	zend_declare_property_long( UserCE, "away", sizeof( "away" ) - 1, 0, ZEND_ACC_PRIVATE TSRMLS_CC );
	zend_declare_property_long( UserCE, "voice", sizeof( "voice" ) - 1, 0, ZEND_ACC_PRIVATE TSRMLS_CC );
}
//---------------------------------------------------------------------------
void ScriptClasses::InitHostMask( TSRMLS_D )
{
	zend_class_entry entry;

	INIT_CLASS_ENTRY( entry, "HostMask", HostMaskMethods );

	HostMaskCE = zend_register_internal_class( &entry TSRMLS_CC );

	zend_declare_property_string( HostMaskCE, "nick", sizeof( "nick" ) - 1, "", ZEND_ACC_PRIVATE TSRMLS_CC );
	zend_declare_property_string( HostMaskCE, "host", sizeof( "host" ) - 1, "", ZEND_ACC_PRIVATE TSRMLS_CC );
	zend_declare_property_string( HostMaskCE, "user", sizeof( "user" ) - 1, "", ZEND_ACC_PRIVATE TSRMLS_CC );
}
//---------------------------------------------------------------------------
void ScriptClasses::InitServer( TSRMLS_D )
{
	zend_class_entry entry;

	INIT_CLASS_ENTRY( entry, "Server", ServerMethods );

	ServerCE = zend_register_internal_class( &entry TSRMLS_CC );

	zend_declare_property_string( ServerCE, "host", sizeof( "host" ) - 1, "", ZEND_ACC_PRIVATE TSRMLS_CC );
}
//---------------------------------------------------------------------------
void ScriptClasses::InitChannel( TSRMLS_D )
{
	zend_class_entry entry;

	INIT_CLASS_ENTRY( entry, "Channel", ChannelMethods );

	ChannelCE = zend_register_internal_class( &entry TSRMLS_CC );

	zend_declare_property_string( ChannelCE, "name", sizeof( "name" ) - 1, "", ZEND_ACC_PRIVATE TSRMLS_CC );
	zend_declare_property_string( ChannelCE, "server", sizeof( "server" ) - 1, "", ZEND_ACC_PRIVATE TSRMLS_CC );
}
//---------------------------------------------------------------------------
void ScriptClasses::InitQuery( TSRMLS_D )
{
	zend_class_entry entry;

	INIT_CLASS_ENTRY( entry, "Query", QueryMethods );

	QueryCE = zend_register_internal_class( &entry TSRMLS_CC );

	zend_declare_property_string( QueryCE, "nick", sizeof( "nick" ) - 1, "", ZEND_ACC_PRIVATE TSRMLS_CC );
	zend_declare_property_string( QueryCE, "server", sizeof( "server" ) - 1, "", ZEND_ACC_PRIVATE TSRMLS_CC );
}
//---------------------------------------------------------------------------
void ScriptClasses::InitDCCTransfer( TSRMLS_D )
{
	zend_class_entry entry;

	INIT_CLASS_ENTRY( entry, "DCCTransfer", DCCTransferMethods );

	DCCTransferCE = zend_register_internal_class( &entry TSRMLS_CC );

	zend_declare_property_string( DCCTransferCE, "nick", sizeof( "nick" ) - 1, "", ZEND_ACC_PRIVATE TSRMLS_CC );
	zend_declare_property_string( DCCTransferCE, "server", sizeof( "server" ) - 1, "", ZEND_ACC_PRIVATE TSRMLS_CC );
	zend_declare_property_string( DCCTransferCE, "file", sizeof( "file" ) - 1, "", ZEND_ACC_PRIVATE TSRMLS_CC );
	zend_declare_property_long( DCCTransferCE, "size", sizeof( "size" ) - 1, 0, ZEND_ACC_PRIVATE TSRMLS_CC );
}
//---------------------------------------------------------------------------
zval *ScriptClasses::CreateUser( TUser *user TSRMLS_DC )
{
    zval *ret;

    MAKE_OBJ( ret, UserCE );

    SET_STRING_PROP( UserCE, ret, "nick", user->GetNick().c_str() );
    SET_STRING_PROP( UserCE, ret, "host", user->GetHost().c_str() );
    SET_STRING_PROP( UserCE, ret, "awayReason", user->GetAwayReason().c_str() );
    SET_LONG_PROP( UserCE, ret, "op", user->IsOp() );
    SET_LONG_PROP( UserCE, ret, "owner", user->IsOwner() );
    SET_LONG_PROP( UserCE, ret, "chanAdmin", user->IsChanAdmin() );
    SET_LONG_PROP( UserCE, ret, "away", user->IsAway() );
    SET_LONG_PROP( UserCE, ret, "voice", user->HasVoice() );

    return( ret );
}
//---------------------------------------------------------------------------
zval *ScriptClasses::CreateHostMask( HostMask *mask TSRMLS_DC )
{
    zval *ret;

    MAKE_OBJ( ret, HostMaskCE );

    SET_STRING_PROP( HostMaskCE, ret, "nick", mask->GetNick().c_str() );
    SET_STRING_PROP( HostMaskCE, ret, "user", mask->GetUser().c_str() );
    SET_STRING_PROP( HostMaskCE, ret, "host", mask->GetHost().c_str() );

    return( ret );
}
//---------------------------------------------------------------------------
zval *ScriptClasses::CreateServer( TServerWin *server TSRMLS_DC )
{
    zval *ret;

    MAKE_OBJ( ret, ServerCE );

    SET_STRING_PROP( ServerCE, ret, "host", server->Server.Host.c_str() );

    return( ret );
}
//---------------------------------------------------------------------------
zval *ScriptClasses::CreateChannel( TChanWin *channel TSRMLS_DC )
{
    zval *ret;

    MAKE_OBJ( ret, ChannelCE );

    SET_STRING_PROP( ChannelCE, ret, "name", channel->GetChannel().c_str() );
    SET_STRING_PROP( ChannelCE, ret, "server", channel->GetServerWin()->Server.Host.c_str() );

    return( ret );
}
//---------------------------------------------------------------------------
zval *ScriptClasses::CreateQuery( TQueryWin *query TSRMLS_DC )
{
    zval *ret;

    MAKE_OBJ( ret, QueryCE );

    SET_STRING_PROP( QueryCE, ret, "nick", query->GetTargetNick().c_str() );
    SET_STRING_PROP( QueryCE, ret, "server", query->GetServerWin()->Server.Host.c_str() );

    return( ret );
}
//---------------------------------------------------------------------------
zval *ScriptClasses::CreateDCCTransfer( TDCCTransfer *dcc TSRMLS_DC )
{
    zval *ret;

    MAKE_OBJ( ret, DCCTransferCE );

    SET_STRING_PROP( DCCTransferCE, ret, "nick", dcc->GetTarget().c_str() );
    SET_STRING_PROP( DCCTransferCE, ret, "server", dcc->GetServer().c_str() );
    SET_STRING_PROP( DCCTransferCE, ret, "file", dcc->GetFile().c_str() );
    SET_LONG_PROP( DCCTransferCE, ret, "size", dcc->GetSize() );

    return( ret );
}
//---------------------------------------------------------------------------
#endif /* SCRIPTING */
//---------------------------------------------------------------------------

