/* $Id: ScriptClasses.h,v 1.4 2005/01/30 21:31:30 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef ScriptClassesH
#define ScriptClassesH
//---------------------------------------------------------------------------

#include "OrionScriptInterface.h"

class TServerWin;
class TChanWin;
class TQueryWin;
class TUser;
class HostMask;
class TDCCTransfer;

class ScriptClasses
{
public:
	static void 	Register( void );

    static zval	   *CreateServer( TServerWin *server TSRMLS_DC );
    static zval	   *CreateChannel( TChanWin *channel TSRMLS_DC );
    static zval	   *CreateQuery( TQueryWin *query TSRMLS_DC );
	static zval    *CreateUser( TUser *user TSRMLS_DC );
	static zval    *CreateHostMask( HostMask *mask TSRMLS_DC );
	static zval    *CreateDCCTransfer( TDCCTransfer *dcc TSRMLS_DC );

private:
	static void		InitUser( TSRMLS_D );
	static void		InitHostMask( TSRMLS_D );
	static void		InitServer( TSRMLS_D );
	static void		InitChannel( TSRMLS_D );
	static void		InitQuery( TSRMLS_D );
	static void		InitDCCTransfer( TSRMLS_D );
};

#define SET_STRING_PROP( class, obj, name, val ) \
        zend_update_property_string( class, obj, name, sizeof( name ) - 1, val TSRMLS_CC );

#define SET_LONG_PROP( class, obj, name, val ) \
        zend_update_property_long( class, obj, name, sizeof( name ) - 1, val TSRMLS_CC );

#define MAKE_OBJ( obj, class ) \
		ALLOC_ZVAL( obj ); \
		object_init_ex( obj, class );

#endif
