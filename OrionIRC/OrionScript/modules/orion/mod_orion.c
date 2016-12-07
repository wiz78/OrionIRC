/* $Id: mod_orion.c,v 1.2 2005/01/23 20:48:49 wiz Exp $ */
//---------------------------------------------------------------------------
#include "php.h"
#include "mod_orion.h"

//---------------------------------------------------------------------------

static zend_function_entry functions[] = 
{
	ZEND_FE( RelativePath,	NULL )

	{ NULL, NULL, NULL }	/* Must be the last line in ctype_functions[] */
};

zend_module_entry orion_module_entry = {
	STANDARD_MODULE_HEADER,
	"Orion utility functions",
	functions,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
    NO_VERSION_YET,
	STANDARD_MODULE_PROPERTIES
};

//---------------------------------------------------------------------------
ZEND_FUNCTION( RelativePath )
{
	char   *str;
	int 	str_len;

	TSRMLS_FETCH();

	if( ZEND_NUM_ARGS() != 1 ) {
		WRONG_PARAM_COUNT;
	}

	if( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s", &str, &str_len ) == SUCCESS ) {
		char *file = zend_get_executed_filename( TSRMLS_C );

		if( file ) {
			char *tmp = strrchr( file, '\\' );

			if( !tmp )
				tmp = strrchr( file, '/' );
	
			if( tmp ) {
				int len = tmp - file + 1;
		
				tmp = emalloc( str_len + len + 1 );

				strncpy( tmp, file, len );
				strcpy( &tmp[ len ], str );

				file = tmp;
			}

			RETVAL_STRING( file, 1 );

			efree( tmp );

		} else {

			RETVAL_STRINGL( str, str_len, 1 );
		}
	}
}
//---------------------------------------------------------------------------
