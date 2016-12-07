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

/* $Id: basic_functions.c,v 1.2 2005/01/22 15:07:28 wiz Exp $ */

#include <zend.h>
#include <zend_API.h>
#include <zend_operators.h>
#include <zend_globals.h>
#include <zend_constants.h>

#include <stdarg.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

#include "php.h"
#include "basic_functions.h"
#include "php_uuencode.h"
#include "php_math.h"
#include "php_type.h"
#include "php_lcg.h"
#include "php_var.h"
#include "php_string.h"
#include "php_rand.h"
#include "php_array.h"
#include "datetime.h"
#include "scanf.h"
#include "uniqid.h"
#include "sha1.h"
#include "dns.h"
#include "base64.h"
#include "microtime.h"
#include "php_incomplete_class.h"
#include "win32/php_win32_globals.h"

#ifndef INADDR_NONE
#define INADDR_NONE ((unsigned long int) -1)
#endif


static
	ZEND_BEGIN_ARG_INFO(first_and_second__args_force_ref, 0)
		ZEND_ARG_PASS_INFO(1)
		ZEND_ARG_PASS_INFO(1)
	ZEND_END_ARG_INFO()

static
	ZEND_BEGIN_ARG_INFO(second_and_third_args_force_ref, 0)
		ZEND_ARG_PASS_INFO(0)
		ZEND_ARG_PASS_INFO(1)
		ZEND_ARG_PASS_INFO(1)
	ZEND_END_ARG_INFO()

static 
	ZEND_BEGIN_ARG_INFO(third_and_fourth_args_force_ref, 0)
		ZEND_ARG_PASS_INFO(0)
		ZEND_ARG_PASS_INFO(0)
		ZEND_ARG_PASS_INFO(1)
		ZEND_ARG_PASS_INFO(1)
	ZEND_END_ARG_INFO()

static
	ZEND_BEGIN_ARG_INFO(third_and_rest_force_ref, 1)
		ZEND_ARG_PASS_INFO(0)
		ZEND_ARG_PASS_INFO(0)
		ZEND_ARG_PASS_INFO(1)
	ZEND_END_ARG_INFO()

static
	ZEND_BEGIN_ARG_INFO(first_through_third_args_force_ref, 0)
		ZEND_ARG_PASS_INFO(1)
		ZEND_ARG_PASS_INFO(1)
		ZEND_ARG_PASS_INFO(1)
	ZEND_END_ARG_INFO()

#undef sprintf

ZEND_basic_globals basic_globals;

zend_function_entry basic_functions[] = 
{
	ZEND_FE(constant,														NULL)
	ZEND_FE(print_r,														NULL)
	ZEND_FE(ip2long,														NULL)
	ZEND_FE(long2ip,														NULL)
	ZEND_FE(getenv,															NULL)
	ZEND_FE(call_user_func,													NULL)
	ZEND_FE(call_user_func_array,											NULL)

	/* functions from uuencode.c */
	ZEND_FE(convert_uuencode,												NULL)
	ZEND_FE(convert_uudecode,												NULL)

	/* functions from type.c */
	ZEND_FE(intval,															NULL)
	ZEND_FE(floatval,														NULL)
	ZEND_FALIAS(doubleval,          floatval,				                NULL)
	ZEND_FE(strval,															NULL)
	ZEND_FE(gettype,														NULL)
	ZEND_FE(settype,				first_arg_force_ref)
	ZEND_FE(is_null,														NULL)
	ZEND_FE(is_resource,													NULL)
	ZEND_FE(is_bool,														NULL)
	ZEND_FE(is_long,														NULL)
	ZEND_FE(is_float,														NULL)
	ZEND_FALIAS(is_int,				is_long,								NULL)
	ZEND_FALIAS(is_integer,			is_long,								NULL)
	ZEND_FALIAS(is_double,			is_float,								NULL)
	ZEND_FALIAS(is_real,			is_float,								NULL)
	ZEND_FE(is_numeric,														NULL)
	ZEND_FE(is_string,														NULL)
	ZEND_FE(is_array,														NULL)
	ZEND_FE(is_object,														NULL)
	ZEND_FE(is_scalar,														NULL)
	ZEND_FE(is_callable,			third_arg_force_ref)

	/* functions from lcg.c */
	ZEND_FE(lcg_value,														NULL)
 
	/* functions from var.c */
	ZEND_FE(serialize,														NULL)															
	ZEND_FE(unserialize,													NULL)
	ZEND_FE(var_dump,														NULL)
	ZEND_FE(var_export,														NULL)
	ZEND_FE(debug_zval_dump,												NULL)

	/* functions from string.c */
	ZEND_FE(strnatcmp,														NULL)
	ZEND_FE(strnatcasecmp,													NULL)
	ZEND_FE(substr_count,													NULL)
	ZEND_FE(strspn,															NULL)
	ZEND_FE(strcspn,															NULL)
	ZEND_FE(strtok,															NULL)
	ZEND_FE(strtoupper,														NULL)
	ZEND_FE(strtolower,														NULL)
	ZEND_FE(strpos,															NULL)
	ZEND_FE(stripos,															NULL)
	ZEND_FE(strrpos,															NULL)
	ZEND_FE(strripos,														NULL)
	ZEND_FE(strrev,															NULL)
	ZEND_FE(basename,														NULL)
	ZEND_FE(dirname,															NULL)
	ZEND_FE(pathinfo,														NULL)
	ZEND_FE(stripslashes,													NULL)
	ZEND_FE(stripcslashes,													NULL)
	ZEND_FE(strstr,															NULL)
	ZEND_FE(stristr,															NULL)
	ZEND_FE(strrchr,															NULL)
	ZEND_FE(str_shuffle,															NULL)
	ZEND_FE(str_word_count,														NULL)
	ZEND_FE(str_split,														NULL)
	ZEND_FE(strpbrk,															NULL)
	ZEND_FE(substr_compare,														NULL)
	ZEND_FE(substr,															NULL)
	ZEND_FE(substr_replace,													NULL)
	ZEND_FE(quotemeta,														NULL)
	ZEND_FE(ucfirst,															NULL)
	ZEND_FE(ucwords,															NULL)
	ZEND_FE(strtr,															NULL)
	ZEND_FE(addslashes,														NULL)
	ZEND_FE(addcslashes,														NULL)
	ZEND_FE(rtrim,															NULL)
	ZEND_FE(str_replace,				fourth_arg_force_ref)
	ZEND_FE(str_ireplace,			fourth_arg_force_ref)
	ZEND_FE(str_repeat,														NULL)
	ZEND_FE(count_chars,														NULL)
	ZEND_FE(chunk_split,														NULL)
	ZEND_FE(trim,															NULL)
	ZEND_FE(ltrim,															NULL)
	ZEND_FE(similar_text,			third_arg_force_ref)
	ZEND_FE(explode,															NULL)
	ZEND_FE(implode,															NULL)
	ZEND_FE(chr,																NULL)
	ZEND_FE(ord,																NULL)
	ZEND_FE(str_pad,															NULL)
	ZEND_FALIAS(chop,				rtrim,									NULL)
	ZEND_FALIAS(strchr,				strstr,									NULL)
	ZEND_NAMED_FE(sprintf,			ZEND_FN(user_sprintf),					NULL)
	ZEND_NAMED_FE(printf,			ZEND_FN(user_printf),					NULL)
	ZEND_FE(vprintf,															NULL)
	ZEND_FE(vsprintf,														NULL)
	ZEND_FE(sscanf,					third_and_rest_force_ref)
	ZEND_FE(bin2hex,														NULL)

	/* functions from rand.c */
	ZEND_FE(rand,															NULL)
	ZEND_FE(srand,															NULL)
	ZEND_FE(getrandmax,														NULL)
	ZEND_FE(mt_rand,															NULL)
	ZEND_FE(mt_srand,														NULL)
	ZEND_FE(mt_getrandmax,													NULL)

	/* functions from datetime.c */
	ZEND_FE(time,															NULL)
	ZEND_FE(mktime,															NULL)
	ZEND_FE(gmmktime,														NULL)
	ZEND_FE(strtotime,														NULL)
	ZEND_FE(date,															NULL)
	ZEND_FE(idate,															NULL)
	ZEND_FE(gmdate,															NULL)
	ZEND_FE(getdate,															NULL)
	ZEND_FE(localtime,														NULL)
	ZEND_FE(checkdate,														NULL)
#ifdef HAVE_GETTIMEOFDAY
	ZEND_FE(microtime,														NULL)
	ZEND_FE(gettimeofday,													NULL)
#endif

	/* functions from uniqid.c */
	ZEND_FE(uniqid,															NULL)

	/* functions from array.c */
	ZEND_FE(ksort,					first_arg_force_ref)
	ZEND_FE(krsort,					first_arg_force_ref)
	ZEND_FE(natsort,					first_arg_force_ref)
	ZEND_FE(natcasesort,				first_arg_force_ref)
	ZEND_FE(asort,					first_arg_force_ref)
	ZEND_FE(arsort,					first_arg_force_ref)
	ZEND_FE(sort,					first_arg_force_ref)
	ZEND_FE(rsort,					first_arg_force_ref)
	ZEND_FE(usort,					first_arg_force_ref)
	ZEND_FE(uasort,					first_arg_force_ref)
	ZEND_FE(uksort,					first_arg_force_ref)
	ZEND_FE(shuffle,					first_arg_force_ref)
	ZEND_FE(array_walk,				first_arg_force_ref)
	ZEND_FE(array_walk_recursive,			first_arg_force_ref)
	ZEND_FE(count,															NULL)
	ZEND_FE(end,						first_arg_force_ref)
	ZEND_FE(prev,					first_arg_force_ref)
	ZEND_FE(next,					first_arg_force_ref)
	ZEND_FE(reset,					first_arg_force_ref)
	ZEND_FE(current,					first_arg_force_ref)
	ZEND_FE(key,						first_arg_force_ref)
	ZEND_FE(min,																NULL)
	ZEND_FE(max,																NULL)
	ZEND_FE(in_array,														NULL)
	ZEND_FE(array_search,													NULL)
	ZEND_FE(extract,															NULL)
	ZEND_FE(compact,															NULL)
	ZEND_FE(array_fill,														NULL)
	ZEND_FE(range,															NULL)
	ZEND_FE(array_multisort,													NULL)
	ZEND_FE(array_push,				first_arg_force_ref)
	ZEND_FE(array_pop,				first_arg_force_ref)
	ZEND_FE(array_shift,				first_arg_force_ref)
	ZEND_FE(array_unshift,			first_arg_force_ref)
	ZEND_FE(array_splice,			first_arg_force_ref)
	ZEND_FE(array_slice,														NULL)
	ZEND_FE(array_merge,														NULL)
	ZEND_FE(array_merge_recursive,											NULL)
	ZEND_FE(array_keys,														NULL)
	ZEND_FE(array_values,													NULL)
	ZEND_FE(array_count_values,												NULL)
	ZEND_FE(array_reverse,													NULL)
	ZEND_FE(array_reduce,													NULL)
	ZEND_FE(array_pad,														NULL)
	ZEND_FE(array_flip,														NULL)
	ZEND_FE(array_change_key_case,											NULL)
	ZEND_FE(array_rand,														NULL)
	ZEND_FE(array_unique,													NULL)
	ZEND_FE(array_intersect,													NULL)
	ZEND_FE(array_uintersect,												NULL)
	ZEND_FE(array_intersect_assoc,											NULL)
	ZEND_FE(array_uintersect_assoc,											NULL)
	ZEND_FE(array_intersect_uassoc,											NULL)
	ZEND_FE(array_uintersect_uassoc,											NULL)
	ZEND_FE(array_diff,														NULL)
	ZEND_FE(array_udiff,														NULL)
	ZEND_FE(array_diff_assoc,												NULL)
	ZEND_FE(array_udiff_assoc,												NULL)
	ZEND_FE(array_diff_uassoc,												NULL)
	ZEND_FE(array_udiff_uassoc,												NULL)
	ZEND_FE(array_sum,														NULL)
	ZEND_FE(array_filter,													NULL)
	ZEND_FE(array_map,														NULL)
	ZEND_FE(array_chunk,														NULL)
	ZEND_FE(array_combine,														NULL)
	ZEND_FE(array_key_exists,														NULL)

	/* aliases from array.c */
	ZEND_FALIAS(pos, 				current, 				 first_arg_force_ref)
	ZEND_FALIAS(sizeof, 				count, 									NULL)
	ZEND_FALIAS(key_exists,			array_key_exists,						NULL)

	/* functions from math.c */
	ZEND_FE(abs,																NULL)
	ZEND_FE(ceil,															NULL)
	ZEND_FE(floor,															NULL)
	ZEND_FE(round,															NULL)
	ZEND_FE(sin,																NULL)
	ZEND_FE(cos,																NULL)
	ZEND_FE(tan,																NULL)
	ZEND_FE(asin,															NULL)
	ZEND_FE(acos,															NULL)
	ZEND_FE(atan,															NULL)
	ZEND_FE(atan2,															NULL)
	ZEND_FE(sinh,															NULL)
	ZEND_FE(cosh,															NULL)
	ZEND_FE(tanh,															NULL)

#ifdef HAVE_ASINH 
	ZEND_FE(asinh,															NULL)
#endif
#ifdef HAVE_ACOSH
	ZEND_FE(acosh,															NULL)
#endif
#ifdef HAVE_ATANH
	ZEND_FE(atanh,															NULL)
#endif
#if !defined(PHP_WIN32) && !defined(NETWARE)
	ZEND_FE(expm1,															NULL)
	ZEND_FE(log1p,															NULL)
#endif

	ZEND_FE(pi,																NULL)
	ZEND_FE(is_finite,														NULL)
	ZEND_FE(is_nan,															NULL)
	ZEND_FE(is_infinite,														NULL)
	ZEND_FE(pow,																NULL)
	ZEND_FE(exp,																NULL)
	ZEND_FE(log,																NULL)
	ZEND_FE(log10,															NULL)
	ZEND_FE(sqrt,															NULL)
#ifdef HAVE_HYPOT
	ZEND_FE(hypot,															NULL)
#endif	
	ZEND_FE(deg2rad,															NULL)
	ZEND_FE(rad2deg,															NULL)
	ZEND_FE(bindec,															NULL)
	ZEND_FE(hexdec,															NULL)
	ZEND_FE(octdec,															NULL)
	ZEND_FE(decbin,															NULL)
	ZEND_FE(decoct,															NULL)
	ZEND_FE(dechex,															NULL)
	ZEND_FE(base_convert,													NULL)
	ZEND_FE(number_format,													NULL)
	ZEND_FE(fmod,															NULL)

	/* functions from sha1.c */
	ZEND_FE(sha1,															NULL)
	ZEND_FE(sha1_file,														NULL)

	/* functions from base64.c */
	ZEND_FE(base64_decode,													NULL)
	ZEND_FE(base64_encode,													NULL)

	/* functions from dns.c */
	ZEND_FE(gethostbyaddr,													NULL)
	ZEND_FE(gethostbyname,													NULL)
	ZEND_FE(gethostbynamel,													NULL)

#if DISABLED

	ZEND_NAMED_FE(md5,php_if_md5,											NULL)
	ZEND_NAMED_FE(md5_file,php_if_md5_file,									NULL)
	ZEND_NAMED_FE(crc32,php_if_crc32,										NULL)

	ZEND_FE(parse_url,														NULL)
	ZEND_FE(urlencode,														NULL)
	ZEND_FE(urldecode,														NULL)
	ZEND_FE(rawurlencode,													NULL)
	ZEND_FE(rawurldecode,													NULL)

	ZEND_FE(unlink,															NULL)
	ZEND_FE(exec,					second_and_third_args_force_ref)
	ZEND_FE(system,					second_arg_force_ref)
	ZEND_FE(escapeshellcmd,													NULL)
	ZEND_FE(escapeshellarg,													NULL)
	ZEND_FE(passthru,				second_arg_force_ref)
	ZEND_FE(shell_exec,														NULL)

	/* functions from reg.c */
	ZEND_FE(ereg,					third_arg_force_ref)
	ZEND_FE(ereg_replace,													NULL)
	ZEND_FE(eregi,					third_arg_force_ref)
	ZEND_FE(eregi_replace,													NULL)
	ZEND_FE(split,															NULL)
	ZEND_FE(spliti,															NULL)
	ZEND_FALIAS(join,				implode,								NULL)
	ZEND_FE(sql_regcase,														NULL)

	/* functions from dl.c */
	ZEND_FE(dl,																NULL)

	/* functions from file.c */
	ZEND_FE(pclose,															NULL)
	ZEND_FE(popen,															NULL)
	ZEND_FE(readfile,														NULL)
	ZEND_FE(rewind,															NULL)
	ZEND_FE(rmdir,															NULL)
	ZEND_FE(umask,															NULL)
	ZEND_FE(fclose,															NULL)
	ZEND_FE(feof,															NULL)
	ZEND_FE(fgetc,															NULL)
	ZEND_FE(fgets,															NULL)
	ZEND_FE(fgetss,															NULL)
	ZEND_FE(fread,															NULL)
	ZEND_NAMED_FE(fopen,				php_if_fopen,							NULL)
	ZEND_FE(fpassthru,														NULL)
	ZEND_NAMED_FE(ftruncate,			php_if_ftruncate,						NULL)
	ZEND_NAMED_FE(fstat,				php_if_fstat,							NULL)
	ZEND_FE(fseek,															NULL)
	ZEND_FE(ftell,															NULL)
	ZEND_FE(fflush,															NULL)
	ZEND_FE(fwrite,															NULL)
	ZEND_FALIAS(fputs,				fwrite,									NULL)
	ZEND_FE(mkdir,															NULL)
	ZEND_FE(rename,															NULL)
	ZEND_FE(copy,															NULL)
	ZEND_FE(tempnam,															NULL)
	ZEND_NAMED_FE(tmpfile,			php_if_tmpfile,							NULL)
	ZEND_FE(file,															NULL)
	ZEND_FE(file_get_contents,												NULL)
	ZEND_FE(file_put_contents,												NULL)
	ZEND_FE(fgetcsv,															NULL)
	ZEND_FE(flock,											 third_arg_force_ref)
	ZEND_FE(get_meta_tags,													NULL)
	ZEND_FE(fscanf,					third_and_rest_force_ref)

	/* functions from pack.c */
	ZEND_FE(pack,															NULL)
	ZEND_FE(unpack,															NULL)

	/* functions from dir.c */
	ZEND_FE(opendir,															NULL)
	ZEND_FE(closedir,														NULL)
	ZEND_FE(chdir,															NULL)

	ZEND_FE(getcwd,															NULL)
	ZEND_FE(rewinddir,														NULL)
	ZEND_NAMED_FE(readdir,			php_if_readdir,							NULL)
	ZEND_FALIAS(dir,					getdir,									NULL)
	ZEND_FE(scandir,															NULL)

	/* functions from filestat.c */
	ZEND_FE(fileatime,														NULL)
	ZEND_FE(filectime,														NULL)
	ZEND_FE(filegroup,														NULL)
	ZEND_FE(fileinode,														NULL)
	ZEND_FE(filemtime,														NULL)
	ZEND_FE(fileowner,														NULL)
	ZEND_FE(fileperms,														NULL)
	ZEND_FE(filesize,														NULL)
	ZEND_FE(filetype,														NULL)
	ZEND_FE(file_exists,														NULL)
	ZEND_FE(is_writable,														NULL)
	ZEND_FALIAS(is_writeable,		is_writable,							NULL)
	ZEND_FE(is_readable,														NULL)
	ZEND_FE(is_executable,													NULL)
	ZEND_FE(is_file,															NULL)
	ZEND_FE(is_dir,															NULL)
	ZEND_FE(is_link,															NULL)
	ZEND_NAMED_FE(stat,				php_if_stat,							NULL)
	ZEND_NAMED_FE(lstat,				php_if_lstat,							NULL)
	ZEND_FE(chown,															NULL)
	ZEND_FE(chgrp,															NULL)
	ZEND_FE(chmod,															NULL)
#if HAVE_UTIME
	ZEND_FE(touch,															NULL)
#endif	
	ZEND_FE(clearstatcache,													NULL)
	ZEND_FE(disk_total_space,												NULL)
	ZEND_FE(disk_free_space,													NULL)
	ZEND_FALIAS(diskfreespace,		disk_free_space,						NULL)

	/* functions from ftok.c*/
#if HAVE_FTOK
	ZEND_FE(ftok,	NULL)
#endif

#endif /* DISABLED */

	{NULL, NULL, NULL}
};

zend_module_entry basic_functions_module = {
    STANDARD_MODULE_HEADER,
	"base",						/* extension name */
	basic_functions,			/* function list */
	ZEND_MINIT(basic),			/* process startup */
	ZEND_MSHUTDOWN(basic),		/* process shutdown */
	ZEND_RINIT(basic),			/* request startup */
	ZEND_RSHUTDOWN(basic),		/* request shutdown */
	NULL,						/* extension info */
    "1.0",						/* extension version */
	STANDARD_MODULE_PROPERTIES
};

static void basic_globals_ctor(ZEND_basic_globals *basic_globals_p TSRMLS_DC)
{
	BG(rand_is_seeded) = 0;
	BG(mt_rand_is_seeded) = 0;
	
	BG(next) = NULL;
	BG(left) = -1;
	BG(user_compare_fci_cache) = empty_fcall_info_cache;

//	memset(&BG(url_adapt_state), 0, sizeof(BG(url_adapt_state)));
//	memset(&BG(url_adapt_state_ex), 0, sizeof(BG(url_adapt_state_ex)));

	BG(incomplete_class) = php_create_incomplete_class(TSRMLS_C);
}


static void basic_globals_dtor(ZEND_basic_globals *basic_globals_p TSRMLS_DC)
{
/*	if (BG(url_adapt_state_ex).tags) {
		zend_hash_destroy(BG(url_adapt_state_ex).tags);
		free(BG(url_adapt_state_ex).tags);
	}*/
}

ZEND_MINIT_FUNCTION(basic)
{
	basic_globals_ctor( &basic_globals TSRMLS_CC );
#ifdef PHP_WIN32
	php_win32_core_globals_ctor(&php_win32_core_globals_ TSRMLS_CC);
#endif

#define REGISTER_MATH_CONSTANT(x)  REGISTER_DOUBLE_CONSTANT(#x, x, CONST_CS | CONST_PERSISTENT)
	REGISTER_MATH_CONSTANT(M_E);
	REGISTER_MATH_CONSTANT(M_LOG2E);
	REGISTER_MATH_CONSTANT(M_LOG10E);
	REGISTER_MATH_CONSTANT(M_LN2);
	REGISTER_MATH_CONSTANT(M_LN10);
	REGISTER_MATH_CONSTANT(M_PI);
	REGISTER_MATH_CONSTANT(M_PI_2);
	REGISTER_MATH_CONSTANT(M_PI_4);
	REGISTER_MATH_CONSTANT(M_1_PI);
	REGISTER_MATH_CONSTANT(M_2_PI);
	REGISTER_MATH_CONSTANT(M_2_SQRTPI);
	REGISTER_MATH_CONSTANT(M_SQRT2);
	REGISTER_MATH_CONSTANT(M_SQRT1_2);
	REGISTER_DOUBLE_CONSTANT("INF", php_get_inf(), CONST_CS | CONST_PERSISTENT);
	REGISTER_DOUBLE_CONSTANT("NAN", php_get_nan(), CONST_CS | CONST_PERSISTENT);

//	register_html_constants(INIT_FUNC_ARGS_PASSTHRU);
//	register_string_constants(INIT_FUNC_ARGS_PASSTHRU);

	ZEND_MINIT(lcg)(INIT_FUNC_ARGS_PASSTHRU);
	ZEND_MINIT(array)(INIT_FUNC_ARGS_PASSTHRU);
 
/*
	ZEND_MINIT(regex)(INIT_FUNC_ARGS_PASSTHRU);
	ZEND_MINIT(file)(INIT_FUNC_ARGS_PASSTHRU);
	ZEND_MINIT(pack)(INIT_FUNC_ARGS_PASSTHRU);

	ZEND_MINIT(dir)(INIT_FUNC_ARGS_PASSTHRU);
	ZEND_MINIT(url_scanner_ex)(INIT_FUNC_ARGS_PASSTHRU);
*/

	return SUCCESS;
}


ZEND_MSHUTDOWN_FUNCTION(basic)
{
	basic_globals_dtor(&basic_globals TSRMLS_CC);

	ZEND_MSHUTDOWN(array)(SHUTDOWN_FUNC_ARGS_PASSTHRU);
/*
	ZEND_MSHUTDOWN(regex)(SHUTDOWN_FUNC_ARGS_PASSTHRU);
	ZEND_MSHUTDOWN(url_scanner_ex)(SHUTDOWN_FUNC_ARGS_PASSTHRU);
	ZEND_MSHUTDOWN(file)(SHUTDOWN_FUNC_ARGS_PASSTHRU);
*/

	return SUCCESS;
}


ZEND_RINIT_FUNCTION(basic)
{

	memset(BG(strtok_table), 0, 256);
	BG(strtok_string) = NULL;
	BG(strtok_zval) = NULL;
	BG(user_compare_func_name) = NULL;
	BG(array_walk_func_name) = NULL;
/*	BG(page_uid) = -1;
	BG(page_gid) = -1;
	BG(page_inode) = -1;
	BG(page_mtime) = -1;
*/
/*
	PHP_RINIT(filestat)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_RINIT(dir)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_RINIT(url_scanner_ex)(INIT_FUNC_ARGS_PASSTHRU);
*/
	PHP_RINIT(lcg)(INIT_FUNC_ARGS_PASSTHRU);
 
	/* Setup default context */
//	FG(default_context) = NULL;

	return SUCCESS;
}


ZEND_RSHUTDOWN_FUNCTION(basic)
{

	if (BG(strtok_zval)) {
		zval_ptr_dtor(&BG(strtok_zval));
	}
	BG(strtok_string) = NULL;
	BG(strtok_zval) = NULL;

/*
	PHP_RSHUTDOWN(filestat)(SHUTDOWN_FUNC_ARGS_PASSTHRU);
#ifdef HAVE_SYSLOG_H
	PHP_RSHUTDOWN(syslog)(SHUTDOWN_FUNC_ARGS_PASSTHRU);
#endif
	PHP_RSHUTDOWN(assert)(SHUTDOWN_FUNC_ARGS_PASSTHRU);
	PHP_RSHUTDOWN(url_scanner_ex)(SHUTDOWN_FUNC_ARGS_PASSTHRU);
	PHP_RSHUTDOWN(streams)(SHUTDOWN_FUNC_ARGS_PASSTHRU);
#ifdef PHP_WIN32
	PHP_RSHUTDOWN(win32_core_globals)(SHUTDOWN_FUNC_ARGS_PASSTHRU);
#endif

	if (BG(user_tick_functions)) {
		zend_llist_destroy(BG(user_tick_functions));
		efree(BG(user_tick_functions));
		BG(user_tick_functions) = NULL;
	}

	PHP_RSHUTDOWN(user_filters)(SHUTDOWN_FUNC_ARGS_PASSTHRU);
*/
	return SUCCESS;
}

#define PHP_DOUBLE_INFINITY_HIGH       0x7ff00000
#define PHP_DOUBLE_QUIET_NAN_HIGH      0xfff80000

double php_get_nan(void)
{
	double val = 0.0;
	((unsigned int *)&val)[1] = PHP_DOUBLE_QUIET_NAN_HIGH;
	((unsigned int *)&val)[0] = 0;
	return val;
}

double php_get_inf(void)
{
	double val = 0.0;
	((unsigned int *)&val)[1] = PHP_DOUBLE_INFINITY_HIGH;
	((unsigned int *)&val)[0] = 0;
	return val;
}

/* {{{ proto mixed constant(string const_name)
   Given the name of a constant this function will return the constants associated value */
ZEND_FUNCTION(constant)
{
	zval **const_name;

	if (ZEND_NUM_ARGS() != 1 ||
		zend_get_parameters_ex(1, &const_name) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	convert_to_string_ex(const_name);

	if (!zend_get_constant(Z_STRVAL_PP(const_name), Z_STRLEN_PP(const_name), return_value TSRMLS_CC)) {
		RETURN_NULL();
	}
}


/* {{{ proto mixed print_r(mixed var [, bool return])
   Prints out or returns information about the specified variable */
ZEND_FUNCTION(print_r)
{
	zval *var;
	zend_bool i = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|b", &var, &i) == FAILURE) {
		RETURN_FALSE;
	}
	
	zend_print_zval_r(var, 0 TSRMLS_CC);

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto int ip2long(string ip_address)
   Converts a string containing an (IPv4) Internet Protocol dotted address into a proper address */
ZEND_FUNCTION(ip2long)
{
	zval **str;
	unsigned long int ip;

	if (ZEND_NUM_ARGS() != 1 || zend_get_parameters_ex(1, &str) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	convert_to_string_ex(str);

	if (Z_STRLEN_PP(str) == 0 || (ip = inet_addr(Z_STRVAL_PP(str))) == INADDR_NONE) {
		/* the only special case when we should return -1 ourselves,
		 * because inet_addr() considers it wrong.
		 */
		if (!memcmp(Z_STRVAL_PP(str), "255.255.255.255", Z_STRLEN_PP(str))) {
			RETURN_LONG(-1);
		}
		
		RETURN_FALSE;
	}

	RETURN_LONG(ntohl(ip));
}
/* }}} */

/* {{{ proto string long2ip(int proper_address)
   Converts an (IPv4) Internet network address into a string in Internet standard dotted format */
ZEND_FUNCTION(long2ip)
{
	zval **num;
	unsigned long n;
	struct in_addr myaddr;

	if (ZEND_NUM_ARGS() != 1 || zend_get_parameters_ex(1, &num) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_string_ex(num);
	
	n = strtoul(Z_STRVAL_PP(num), NULL, 0);

	myaddr.s_addr = htonl(n);
	RETURN_STRING(inet_ntoa(myaddr), 1);
}
/* }}} */

/* {{{ proto string getenv(string varname)
   Get the value of an environment variable */
ZEND_FUNCTION(getenv)
{
	char *ptr, *str;
	int str_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &str, &str_len) == FAILURE) {
		RETURN_FALSE;
	}

	ptr = getenv(str);

	if (ptr) {
		RETURN_STRING(ptr, 1);
	}
	RETURN_FALSE;
}
/* }}} */


/* {{{ proto mixed call_user_func(string function_name [, mixed parmeter] [, mixed ...])
   Call a user function which is the first parameter */
ZEND_FUNCTION(call_user_func)
{
	zval ***params;
	zval *retval_ptr;
	char *name;
	int argc = ZEND_NUM_ARGS();

	if (argc < 1) {
		WRONG_PARAM_COUNT;
	}

	params = safe_emalloc(sizeof(zval **), argc, 0);

	if (zend_get_parameters_array_ex(argc, params) == FAILURE) {
		efree(params);
		RETURN_FALSE;
	}

	if (Z_TYPE_PP(params[0]) != IS_STRING && Z_TYPE_PP(params[0]) != IS_ARRAY) {
		SEPARATE_ZVAL(params[0]);
		convert_to_string_ex(params[0]);
	}

	if (!zend_is_callable(*params[0], 0, &name)) {
		zend_error( E_WARNING, "First argument is expected to be a valid callback");
		efree(name);
		efree(params);
		RETURN_NULL();
	}

	if (call_user_function_ex(EG(function_table), NULL, *params[0], &retval_ptr, argc-1, params+1, 0, NULL TSRMLS_CC) == SUCCESS) {
		if (retval_ptr) {
			COPY_PZVAL_TO_ZVAL(*return_value, retval_ptr);
		}
	} else {
		if (argc > 1) {
			SEPARATE_ZVAL(params[1]);
			convert_to_string_ex(params[1]);
			if (argc > 2) {
				SEPARATE_ZVAL(params[2]);
				convert_to_string_ex(params[2]);
				zend_error( E_WARNING, "Unable to call %s(%s,%s)", name, Z_STRVAL_PP(params[1]), Z_STRVAL_PP(params[2]));
			} else {
				zend_error(E_WARNING, "Unable to call %s(%s)", name, Z_STRVAL_PP(params[1]));
			}
		} else {
			zend_error(E_WARNING, "Unable to call %s()", name);
		}
	}

	efree(name);
	efree(params);
}
/* }}} */

/* {{{ proto mixed call_user_func_array(string function_name, array parameters)
   Call a user function which is the first parameter with the arguments contained in array */
ZEND_FUNCTION(call_user_func_array)
{
	zval ***func_params, **func, **params;
	zval *retval_ptr;
	HashTable *func_params_ht;
	char *name;
	int count;
	int current = 0;

	if (ZEND_NUM_ARGS() != 2 || zend_get_parameters_ex(2, &func, &params) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	SEPARATE_ZVAL(params);
	convert_to_array_ex(params);

	if (Z_TYPE_PP(func) != IS_STRING && Z_TYPE_PP(func) != IS_ARRAY) {
		SEPARATE_ZVAL(func);
		convert_to_string_ex(func);
	}

	if (!zend_is_callable(*func, 0, &name)) {
		zend_error(E_WARNING, "First argument is expected to be a valid callback, '%s' was given", name);
		efree(name);
		RETURN_NULL();
	}

	func_params_ht = Z_ARRVAL_PP(params);

	count = zend_hash_num_elements(func_params_ht);
	func_params = safe_emalloc(sizeof(zval **), count, 0);

	for (zend_hash_internal_pointer_reset(func_params_ht);
		 zend_hash_get_current_data(func_params_ht, (void **) &func_params[current]) == SUCCESS;
		 zend_hash_move_forward(func_params_ht)
		) {
		current++;
	}

	if (call_user_function_ex(EG(function_table), NULL, *func, &retval_ptr, count, func_params, 0, NULL TSRMLS_CC) == SUCCESS && retval_ptr) {
		COPY_PZVAL_TO_ZVAL(*return_value, retval_ptr);
	} else {
		zend_error(E_WARNING, "Unable to call %s()", name);
	}

	efree(name);
	efree(func_params);
}
/* }}} */
