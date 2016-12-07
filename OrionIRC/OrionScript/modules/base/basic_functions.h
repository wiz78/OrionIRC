/*
   +----------------------------------------------------------------------+
   | PHP Version 5                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2004 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.0 of the PHP license,       |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_0.txt.                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Authors: Andi Gutmans <andi@zend.com>                                |
   |          Zeev Suraski <zeev@zend.com>                                |
   +----------------------------------------------------------------------+
*/

/* $Id: basic_functions.h,v 1.2 2005/01/22 15:07:28 wiz Exp $ */

#ifndef BASIC_FUNCTIONS_H
#define BASIC_FUNCTIONS_H

#ifdef HAVE_WCHAR_H
#include <wchar.h>
#endif

extern zend_module_entry basic_functions_module;

ZEND_MINIT_FUNCTION(basic);
ZEND_MSHUTDOWN_FUNCTION(basic);
ZEND_RINIT_FUNCTION(basic);
ZEND_RSHUTDOWN_FUNCTION(basic);

ZEND_FUNCTION(constant);
ZEND_FUNCTION(toggle_short_open_tag);
ZEND_FUNCTION(sleep);
ZEND_FUNCTION(usleep);
#if HAVE_NANOSLEEP
ZEND_FUNCTION(time_nanosleep);
#endif
ZEND_FUNCTION(flush);
ZEND_FUNCTION(ip2long);
ZEND_FUNCTION(long2ip);

/* system functions */
ZEND_FUNCTION(getenv);
ZEND_FUNCTION(putenv);

ZEND_FUNCTION(getopt);

ZEND_FUNCTION(get_cfg_var);

ZEND_FUNCTION(call_user_func);
ZEND_FUNCTION(call_user_func_array);
ZEND_FUNCTION(call_user_method);
ZEND_FUNCTION(call_user_method_array);

ZEND_FUNCTION(print_r);
ZEND_FUNCTION(fprintf);
ZEND_FUNCTION(vfprintf);

ZEND_FUNCTION(getservbyname);
ZEND_FUNCTION(getservbyport);
ZEND_FUNCTION(getprotobyname);
ZEND_FUNCTION(getprotobynumber);

ZEND_NAMED_FUNCTION(ZEND_if_crc32);

ZEND_MINIT_FUNCTION(user_filters);
ZEND_RSHUTDOWN_FUNCTION(user_filters);

typedef unsigned int ZEND_uint32;
typedef signed int ZEND_int32;

#define MT_N (624)

typedef struct _ZEND_basic_globals {
	zval *strtok_zval;
	char *strtok_string;

	char *strtok_last;
	char strtok_table[256];
	ulong strtok_len;
	char str_ebuf[40];
	zval **array_walk_func_name;
	zend_fcall_info_cache array_walk_fci_cache;
	zval **user_compare_func_name;
	zend_fcall_info_cache user_compare_fci_cache;

	/* var.c */
	zend_class_entry *incomplete_class;
 	
	/* rand.c */
	ZEND_uint32   state[MT_N+1];  /* state vector + 1 extra to not violate ANSI C */
	ZEND_uint32   *next;       /* next random value is computed from here */
	int      left;        /* can *next++ this many times before reloading */

	unsigned int rand_seed; /* Seed for rand(), in ts version */

	zend_bool rand_is_seeded; /* Whether rand() has been seeded */
	zend_bool mt_rand_is_seeded; /* Whether mt_rand() has been seeded */
    
	/* url_scanner.c */
//	url_adapt_state_t url_adapt_state;
	/* url_scanner_ex.re */
//	url_adapt_state_ex_t url_adapt_state_ex;

} ZEND_basic_globals;

#ifdef ZTS
#define BG(v) TSRMG(basic_globals_id, ZEND_basic_globals *, v)
extern int basic_globals_id;
#else
#define BG(v) (basic_globals.v)
extern ZEND_basic_globals basic_globals;
#endif

double php_get_nan();
double php_get_inf();

#endif /* BASIC_FUNCTIONS_H */
