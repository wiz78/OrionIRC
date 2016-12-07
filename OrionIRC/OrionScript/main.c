/* $Id: main.c,v 1.8 2005/01/26 21:10:05 wiz Exp $ */
//---------------------------------------------------------------------------
#include <stdarg.h>

#include "OrionScript.h"
#include "modules/base/basic_functions.h"
#include "modules/pcre/php_pcre.h"
#include "modules/ctype/php_ctype.h"
#include "modules/orion/mod_orion.h"

//---------------------------------------------------------------------------
static HashTable GlobalVars;

//---------------------------------------------------------------------------
static void _error_cb( int type, const char *error_filename, const uint error_lineno, 
					   const char *format, va_list args )
{
	char	buffer[ 10 * 1024 ];
	char	*errType;

	switch( type ) {

		case E_WARNING:
		case E_CORE_WARNING:
		case E_USER_WARNING:
			errType = "Warning";
			break;

		case E_PARSE:
			errType = "Parse error";
			break;

		case E_NOTICE:
		case E_USER_NOTICE:
			errType = "Notice";
			break;

		case E_COMPILE_ERROR:
			errType = "Compile error";
			break;

		case E_COMPILE_WARNING:
			errType = "Compile warning";
			break;

		default:
			errType = "Error";
			break;
	}

	sprintf( buffer, "%s on line %d of %s: ", errType, error_lineno, error_filename );
	vsprintf( strchr( buffer, '\0' ), format, args );

	zend_write( buffer, strlen( buffer ));

	switch( type ) {

		case E_ERROR:
		case E_CORE_ERROR:
		case E_USER_ERROR:
		case E_STRICT:
		case E_PARSE:
		case E_COMPILE_ERROR:
			_zend_bailout((char *)error_filename, error_lineno );
			break;
	}
}
//---------------------------------------------------------------------------
int _printf( const char *format, ... )
{
	va_list	va;
	char	buffer[ 1024 * 10 ];

	va_start( va, format );
	vsprintf( buffer, format, va );
	va_end( va );

	zend_write( buffer, strlen( buffer ));

	return( strlen( buffer ));
}
//---------------------------------------------------------------------------
static void _message_handler( long message, void *data )
{
	TSRMLS_FETCH();

	switch( message ) {
		
		case ZMSG_FAILED_INCLUDE_FOPEN:
			zend_error( E_WARNING, "Failed opening '%s' for inclusion", data );
			break;

		case ZMSG_FAILED_REQUIRE_FOPEN:
			zend_error( E_COMPILE_ERROR, "Failed opening required '%s'", data );
			break;
	}
}
//---------------------------------------------------------------------------
static void _on_timeout( int seconds TSRMLS_DC )
{
}
//---------------------------------------------------------------------------
static int vspprintf( char **pbuf, size_t max_len, const char *format, va_list ap )
{
	return( 0 );
}
//---------------------------------------------------------------------------
void ORIONSCRIPTCC OS_Setup( OrionCallbacks *cbs, zend_module_entry **modules )
{
	TSRMLS_FETCH();

	zend_try {
		zend_utility_functions	zuf;
		zend_utility_values		zuv;

		EG( bailout_set ) = 0;
		EG( error_reporting ) = E_ALL & ~E_NOTICE;

		memset( &zuf, 0, sizeof( zuf ));
		memset( &zuv, 0, sizeof( zuv ));

		zuf.error_function              = _error_cb;
		zuf.printf_function             = _printf;
		zuf.write_function              = cbs->Write;
		zuf.message_handler             = _message_handler;
		zuf.get_configuration_directive = cbs->GetCfgDirective;
		zuf.ticks_function              = cbs->Ticks;
		zuf.on_timeout                  = _on_timeout;
		zuf.vspprintf_function          = vspprintf;

		zend_startup( &zuf, NULL, 0 );

		zuv.html_errors          = 1;
		zuv.import_use_extension = ".ophp";

		zend_register_standard_ini_entries( TSRMLS_C );
		zend_set_utility_values( &zuv );
		zend_startup_builtin_functions( TSRMLS_C );
		zend_register_default_classes( TSRMLS_C );
		zend_startup_extensions();

		zend_startup_module( &basic_functions_module );
		zend_startup_module( &pcre_module_entry );
		zend_startup_module( &ctype_module_entry );
		zend_startup_module( &orion_module_entry );

		if( modules )
			while( *modules )
				zend_startup_module( *modules++ );

		zend_activate( TSRMLS_C );
		zend_activate_modules( TSRMLS_C );

		zend_hash_init( &GlobalVars, 0, NULL, ZVAL_PTR_DTOR, 0 ); 

	} zend_end_try();
}
//---------------------------------------------------------------------------
void ORIONSCRIPTCC OS_Cleanup( void )
{
	TSRMLS_FETCH();

	zend_hash_destroy( &GlobalVars );
	zend_deactivate_modules( TSRMLS_C );

	zend_try {
		zend_post_deactivate_modules( TSRMLS_C );
	} zend_end_try();

	zend_try {

		if( EG( active_op_array )) {
			destroy_op_array( EG( active_op_array ) TSRMLS_CC );
			efree( EG( active_op_array ));
		}

		zend_deactivate( TSRMLS_C );
		zend_shutdown( TSRMLS_C );

		zend_ini_shutdown( TSRMLS_C );
		shutdown_memory_manager( CG( unclean_shutdown ), 1 TSRMLS_CC );

	} zend_end_try();

	zend_try { 
		zend_unset_timeout( TSRMLS_C );
	} zend_end_try();
}
//---------------------------------------------------------------------------
static int copy_var( zval **orig, int num_args, va_list args, zend_hash_key *hash_key )
{
	if(( hash_key->nKeyLength < 8 ) || strcmp( hash_key->arKey, "GLOBALS" )) {
		HashTable	*dest = va_arg( args, HashTable * );
		zval		*copy;

		ALLOC_ZVAL( copy );
		ZVAL_ZVAL( copy, *orig, TRUE, FALSE );

		zend_hash_update( dest, hash_key->arKey, hash_key->nKeyLength, &copy, sizeof( zval * ), NULL );
	}

	return( ZEND_HASH_APPLY_KEEP );
} 
//---------------------------------------------------------------------------
void ORIONSCRIPTCC OS_ResetCode( void )
{
	TSRMLS_FETCH();

	zend_deactivate( TSRMLS_C );
	zend_activate( TSRMLS_C );
}
//---------------------------------------------------------------------------
void ORIONSCRIPTCC OS_SaveState( void )
{
	TSRMLS_FETCH();

	zend_hash_clean( &GlobalVars );
	zend_hash_apply_with_arguments( &EG( symbol_table ), (apply_func_args_t) copy_var, 1, &GlobalVars );
}
//---------------------------------------------------------------------------
void ORIONSCRIPTCC OS_RestoreState( void )
{
	TSRMLS_FETCH();

	zend_hash_apply_with_arguments( &GlobalVars, (apply_func_args_t) copy_var, 1, &EG( symbol_table ));
	zend_hash_clean( &GlobalVars );
}
//---------------------------------------------------------------------------
void ORIONSCRIPTCC OS_Compile( const char *script )
{
	TSRMLS_FETCH();

	zend_try {
		zend_file_handle file;

		memset( &file, 0, sizeof( file ));

		file.filename = (char *)script;
		file.type     = ZEND_HANDLE_FILENAME;

		if( EG( active_op_array )) {
			destroy_op_array( EG( active_op_array ) TSRMLS_CC );
			efree( EG( active_op_array ));
		}

		EG( active_op_array ) = zend_compile_file( &file, ZEND_INCLUDE TSRMLS_CC );

		zend_destroy_file_handle( &file TSRMLS_CC );

		if( EG( active_op_array )) {
			zval *retval;

			EG( return_value_ptr_ptr ) = &retval; 

			zend_unset_timeout( TSRMLS_C );
			zend_execute( EG( active_op_array ) TSRMLS_CC );

			zval_ptr_dtor( EG( return_value_ptr_ptr ));
		}

	} zend_end_try();
}
//---------------------------------------------------------------------------
void ORIONSCRIPTCC OS_CompileString( const char *code, const char *filename )
{
	TSRMLS_FETCH();

	zend_try {
		zval *source;

		if( EG( active_op_array )) {
			destroy_op_array( EG( active_op_array ) TSRMLS_CC );
			efree( EG( active_op_array ));
		}

		MAKE_STD_ZVAL( source );
		ZVAL_STRING( source, (char *)code, 1 );

		EG( active_op_array ) = compile_string( source, (char *)filename TSRMLS_CC );

		zval_ptr_dtor( &source );

		if( EG( active_op_array )) {
			zval *retval;

			EG( return_value_ptr_ptr ) = &retval; 

			zend_unset_timeout( TSRMLS_C );
			zend_execute( EG( active_op_array ) TSRMLS_CC );

			zval_ptr_dtor( EG( return_value_ptr_ptr ));
		}

	} zend_end_try();
}
//---------------------------------------------------------------------------
void ORIONSCRIPTCC OS_CallFunction( const char *func )
{
	TSRMLS_FETCH();

	zend_try {

		if( CG( function_table )) {
			zval *mt, *ret = NULL;
			
			MAKE_STD_ZVAL( mt );
			ZVAL_STRING( mt, (char *)func, 1 );

			call_user_function_ex( CG( function_table ),
								   NULL, mt,
								   &ret,
								   0, NULL, 1, NULL TSRMLS_CC );
			
			zval_ptr_dtor( &mt );

			if( ret )
				zval_ptr_dtor( &ret );
		}

	} zend_end_try();
}
//---------------------------------------------------------------------------
void ORIONSCRIPTCC OS_Eval( const char *code )
{
	TSRMLS_FETCH();

	zend_try {
		zval *ret;

		MAKE_STD_ZVAL( ret );	

		zend_eval_string( (char *)code, ret, "Eval() code" TSRMLS_CC );
	
		zval_ptr_dtor( &ret );

	} zend_end_try();
}
//---------------------------------------------------------------------------
HashTable *ORIONSCRIPTCC OS_GetFunctionTable( void )
{
	return( EG( function_table ));
}
//---------------------------------------------------------------------------
