/* $Id: OrionScript.h,v 1.6 2005/01/26 21:10:05 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef ORIONSCRIPT_H
#define ORIONSCRIPT_H
//---------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

#include <zend.h>
#include <zend_extensions.h>
#include <zend_modules.h>
#include <zend_constants.h>
#include <zend_list.h>
#include <zend_API.h>
#include <zend_exceptions.h>
#include <zend_builtin_functions.h>
#include <zend_ini.h>

#define ORIONSCRIPTCC		__cdecl

#ifdef ORIONSCRIPT_EXPORTS
#define ORIONSCRIPTAPI(x)	__declspec(dllexport) x ORIONSCRIPTCC
#else
#define ORIONSCRIPTAPI(x)	__declspec(dllimport) x ORIONSCRIPTCC
#endif

typedef struct _OrionCallbacks
{
	int						( ORIONSCRIPTCC *Write )( const char *str, uint str_length );
	int						( ORIONSCRIPTCC *GetCfgDirective )( char *name, uint name_length, zval *contents );
	void					( ORIONSCRIPTCC *Ticks )( int ticks );
} OrionCallbacks;


ORIONSCRIPTAPI( void )			OS_Setup( OrionCallbacks *cbs, zend_module_entry **modules );
ORIONSCRIPTAPI( void )			OS_Cleanup( void );
ORIONSCRIPTAPI( void )			OS_ResetCode( void );
ORIONSCRIPTAPI( void )			OS_SaveState( void );
ORIONSCRIPTAPI( void )			OS_RestoreState( void );
ORIONSCRIPTAPI( void )			OS_Compile( const char *script );
ORIONSCRIPTAPI( void )			OS_CompileString( const char *code, const char *filename );
ORIONSCRIPTAPI( void )			OS_CallFunction( const char *func );
ORIONSCRIPTAPI( void )			OS_Eval( const char *code );
ORIONSCRIPTAPI( HashTable * )	OS_GetFunctionTable( void );

#ifdef __cplusplus
};
#endif

//---------------------------------------------------------------------------
#endif /* ORIONSCRIPT_H */
//---------------------------------------------------------------------------
