/* $Id: ScriptBasic.cpp,v 1.2 2005/02/08 18:59:19 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "OrionScriptInterface.h"
#include "ScriptBasic.h"
#include "ScriptClasses.h"
#include "Commands.h"
#include "Version.h"
#include "Main.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

#if SCRIPTING

static ZEND_FUNCTION( cmd );
static ZEND_FUNCTION( DisplayOnActiveWin );
static ZEND_FUNCTION( GetCurrentChannel );
static ZEND_FUNCTION( GetVersion );
static ZEND_FUNCTION( ShowMessage );
static ZEND_FUNCTION( MessageDlg );
static ZEND_FUNCTION( RegisterEvent );
static ZEND_FUNCTION( RemoveEvent );

static zend_function_entry BasicFunctions[] =
{
	ZEND_FE( cmd, NULL )
	ZEND_FE( DisplayOnActiveWin, NULL )
	ZEND_FE( GetCurrentChannel, NULL )
	ZEND_FE( GetVersion, NULL )
	ZEND_FE( ShowMessage, NULL )
	ZEND_FE( MessageDlg, NULL )
	ZEND_FE( RegisterEvent, NULL )
	ZEND_FE( RemoveEvent, NULL )

	{NULL, NULL, NULL}	/* Must be the last line in ctype_functions[] */
};

zend_module_entry BasicModule =
{
	STANDARD_MODULE_HEADER,
	"Basic Orion functions",
	BasicFunctions,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
    NO_VERSION_YET,
	STANDARD_MODULE_PROPERTIES
};

//---------------------------------------------------------------------------
ZEND_FUNCTION( cmd )
{
	char   *cmd;
	int 	cmd_len;

	if( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s", &cmd, &cmd_len ) == SUCCESS )
		HandleCommand( MainWin->GetCurrentServerWin(), MainWin->GetCurrentChannelWin(), cmd );
}
//---------------------------------------------------------------------------
ZEND_FUNCTION( DisplayOnActiveWin )
{
	char   *str;
	int 	str_len;

	if( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s", &str, &str_len ) == SUCCESS )
		MainWin->DisplayOnActiveWin( NULL, str, IRC_SCRIPT );
}
//---------------------------------------------------------------------------
ZEND_FUNCTION( GetCurrentChannel )
{
	DEFAULT_0_PARAMS;

	TChanWin   *channel = MainWin->GetCurrentChannelWin();

    if( channel ) {
    	zval *ret = ScriptClasses::CreateChannel( channel TSRMLS_CC );

		RETVAL_ZVAL( ret, FALSE, FALSE );

    } else
    	RETVAL_NULL();
}
//---------------------------------------------------------------------------
ZEND_FUNCTION( GetVersion )
{
	DEFAULT_0_PARAMS;

    RETVAL_STRING( Version.GetVersionStr().c_str(), TRUE );
}
//---------------------------------------------------------------------------
ZEND_FUNCTION( ShowMessage )
{
	char   *str;
	int 	str_len;

	if( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s", &str, &str_len ) == SUCCESS )
		ShowMessage( str );
}
//---------------------------------------------------------------------------
ZEND_FUNCTION( MessageDlg )
{
	char   *str;
	int 	str_len, type = mtInformation, buttons = BIT( mbOK );

	if( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s|ll",
    						   &str, &str_len, &type, &buttons ) == SUCCESS ) {
		TMsgDlgButtons btn;

        if( buttons & BIT( mbYes ))	       	btn << mbYes;
        if( buttons & BIT( mbNo ))	       	btn << mbNo;
        if( buttons & BIT( mbOK ))	      	btn << mbOK;
        if( buttons & BIT( mbCancel ))     	btn << mbCancel;
        if( buttons & BIT( mbHelp ))     	btn << mbHelp;
        if( buttons & BIT( mbAbort ))      	btn << mbAbort;
        if( buttons & BIT( mbRetry ))      	btn << mbRetry;
        if( buttons & BIT( mbIgnore ))     	btn << mbIgnore;
        if( buttons & BIT( mbAll ))	      	btn << mbAll;

		RETVAL_LONG( MessageDlg( str, static_cast<TMsgDlgType>( type ), btn, 0 ));
    }
}
//---------------------------------------------------------------------------
ZEND_FUNCTION( RegisterEvent )
{
	zval   *callback;
    int		event;

	if( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "zl",
    						   &callback, &event ) == SUCCESS ) {
    	int	ret = -1;

    	if(( event < 0 ) || ( event >= SE_COUNT ))
          	zend_error( E_ERROR, "%d is not a valid event for %s",
            			event, get_active_function_name( TSRMLS_C ));
    	else if( zend_is_callable( callback, 0, NULL ))
        	ret = OrionScript::AddCallback( callback, static_cast<ScriptEvent>( event ));
		else
          	zend_error( E_ERROR, "Invalid callback passed to %s",
            			get_active_function_name( TSRMLS_C ));

       	RETVAL_LONG( ret );
    }
}
//---------------------------------------------------------------------------
ZEND_FUNCTION( RemoveEvent )
{
    int	callback, event;

	if( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "ll",
    						   &callback, &event ) == SUCCESS ) {

    	if(( event < 0 ) || ( event >= SE_COUNT ))
          	zend_error( E_ERROR, "%d is not a valid event for %s",
            			event, get_active_function_name( TSRMLS_C ));
        else
        	OrionScript::RemoveCallback( callback, static_cast<ScriptEvent>( event ));
	}
}
//---------------------------------------------------------------------------
#endif /* SCRIPTING */
//---------------------------------------------------------------------------

