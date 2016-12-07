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

/* $Id: php.h,v 1.2 2005/01/22 15:07:28 wiz Exp $ */

#ifndef PHP_H
#define PHP_H

#include <zend.h>
#include <zend_qsort.h>
#include <zend_API.h>

#undef sprintf
#define sprintf php_sprintf

/* PHP's DEBUG value must match Zend's ZEND_DEBUG value */
#undef PHP_DEBUG
#define PHP_DEBUG ZEND_DEBUG

#define PHP_WIN32
#define WINDOWS
#define HAVE_GETTIMEOFDAY

typedef unsigned int	php_uint32;
typedef int				php_int32;

typedef zval			pval;

//#include "php_regex.h"


/*
 * This is a fast version of strlcpy which should be used, if you
 * know the size of the destination buffer and if you know
 * the length of the source string.
 *
 * size is the allocated number of bytes of dst
 * src_size is the number of bytes excluding the NUL of src
 */

#define PHP_STRLCPY(dst, src, size, src_size)	\
	{											\
		size_t php_str_len;						\
												\
		if (src_size >= size)					\
			php_str_len = size - 1;				\
		else									\
			php_str_len = src_size;				\
		memcpy(dst, src, php_str_len);			\
		dst[php_str_len] = '\0';				\
	}

#ifndef HAVE_STRLCPY
BEGIN_EXTERN_C()
PHPAPI size_t php_strlcpy(char *dst, const char *src, size_t siz);
END_EXTERN_C()
#undef strlcpy
#define strlcpy php_strlcpy
#endif

#ifndef HAVE_STRLCAT
BEGIN_EXTERN_C()
PHPAPI size_t php_strlcat(char *dst, const char *src, size_t siz);
END_EXTERN_C()
#undef strlcat
#define strlcat php_strlcat
#endif

#ifndef HAVE_STRTOK_R
BEGIN_EXTERN_C()
char *strtok_r(char *s, const char *delim, char **last);
END_EXTERN_C()
#endif

#ifndef HAVE_SOCKLEN_T
typedef unsigned int socklen_t;
#endif

#define CREATE_MUTEX(a, b)
#define SET_MUTEX(a)
#define FREE_MUTEX(a)

/*
 * Then the ODBC support can use both iodbc and Solid,
 * uncomment this.
 * #define HAVE_ODBC (HAVE_IODBC|HAVE_SOLID)
 */

#include <stdlib.h>
#include <ctype.h>
#if HAVE_UNISTD_H
#include <unistd.h>
#endif
#if HAVE_STDARG_H
#include <stdarg.h>
#else
# if HAVE_SYS_VARARGS_H
# include <sys/varargs.h>
# endif
#endif


#include <zend_hash.h>
#include <zend_alloc.h>
#include <zend_stack.h>

#include <string.h>

#ifndef LONG_MAX
#define LONG_MAX 2147483647L
#endif

#ifndef LONG_MIN
#define LONG_MIN (- LONG_MAX - 1)
#endif

#ifndef INT_MAX
#define INT_MAX 2147483647
#endif

#ifndef INT_MIN
#define INT_MIN (- INT_MAX - 1)
#endif

#define PHP_GCC_VERSION ZEND_GCC_VERSION
#define PHP_ATTRIBUTE_MALLOC ZEND_ATTRIBUTE_MALLOC
#define PHP_ATTRIBUTE_FORMAT ZEND_ATTRIBUTE_FORMAT

BEGIN_EXTERN_C()
#include "snprintf.h"
END_EXTERN_C()
#include "spprintf.h"

/* macros */
#define STR_PRINT(str)	((str)?(str):"")

#ifndef MAXPATHLEN
# ifdef PATH_MAX
#  define MAXPATHLEN PATH_MAX
# else
#  define MAXPATHLEN 256    /* Should be safe for any weird systems that do not define it */
# endif
#endif


/* global variables */
#ifdef PHP_PWRITE_64
ssize_t pwrite(int, void *, size_t, off64_t);
#endif

#ifdef PHP_PREAD_64
ssize_t pread(int, void *, size_t, off64_t);
#endif

extern int _printf( const char *format, ... );

#define php_write	zend_write
#define php_printf	_printf
#define php_error	zend_error

typedef enum {
	EH_NORMAL = 0,
	EH_SUPPRESS,
	EH_THROW
} error_handling_t;


#define phpparse zendparse
#define phprestart zendrestart
#define phpin zendin

#define php_memnstr zend_memnstr

/* PHP-named Zend macro wrappers */
#define PHP_FN					ZEND_FN
#define PHP_NAMED_FUNCTION		ZEND_NAMED_FUNCTION
#define PHP_FUNCTION			ZEND_FUNCTION
#define PHP_METHOD  			ZEND_METHOD

#define PHP_NAMED_FE	ZEND_NAMED_FE
#define PHP_FE			ZEND_FE
#define PHP_FALIAS		ZEND_FALIAS
#define PHP_ME          ZEND_ME
#define PHP_ME_MAPPING  ZEND_ME_MAPPING

#define PHP_MODULE_STARTUP_N	ZEND_MODULE_STARTUP_N
#define PHP_MODULE_SHUTDOWN_N	ZEND_MODULE_SHUTDOWN_N
#define PHP_MODULE_ACTIVATE_N	ZEND_MODULE_ACTIVATE_N
#define PHP_MODULE_DEACTIVATE_N	ZEND_MODULE_DEACTIVATE_N
#define PHP_MODULE_INFO_N		ZEND_MODULE_INFO_N

#define PHP_MODULE_STARTUP_D	ZEND_MODULE_STARTUP_D
#define PHP_MODULE_SHUTDOWN_D	ZEND_MODULE_SHUTDOWN_D
#define PHP_MODULE_ACTIVATE_D	ZEND_MODULE_ACTIVATE_D
#define PHP_MODULE_DEACTIVATE_D	ZEND_MODULE_DEACTIVATE_D
#define PHP_MODULE_INFO_D		ZEND_MODULE_INFO_D

/* Compatibility macros */
#define PHP_MINIT		ZEND_MODULE_STARTUP_N
#define PHP_MSHUTDOWN	ZEND_MODULE_SHUTDOWN_N
#define PHP_RINIT		ZEND_MODULE_ACTIVATE_N
#define PHP_RSHUTDOWN	ZEND_MODULE_DEACTIVATE_N
#define PHP_MINFO		ZEND_MODULE_INFO_N

#define PHP_MINIT_FUNCTION		ZEND_MODULE_STARTUP_D
#define PHP_MSHUTDOWN_FUNCTION	ZEND_MODULE_SHUTDOWN_D
#define PHP_RINIT_FUNCTION		ZEND_MODULE_ACTIVATE_D
#define PHP_RSHUTDOWN_FUNCTION	ZEND_MODULE_DEACTIVATE_D
#define PHP_MINFO_FUNCTION		ZEND_MODULE_INFO_D


/* Output support */
#define PHPWRITE(str, str_len)		zend_write((str), (str_len) TSRMLS_CC)
#define PUTS(str)					{		\
	const char *__str = (str);						\
	zend_write(__str, strlen(__str) TSRMLS_CC);		\
}

#define PUTC(c)						(zend_write(&(c), 1 TSRMLS_CC), (c))

#include <zend_constants.h>

/* Finding offsets of elements within structures.
 * Taken from the Apache code, which in turn, was taken from X code...
 */

#ifndef XtOffset
#if defined(CRAY) || (defined(__arm) && !defined(LINUX))
#ifdef __STDC__
#define XtOffset(p_type, field) _Offsetof(p_type, field)
#else
#ifdef CRAY2
#define XtOffset(p_type, field) \
    (sizeof(int)*((unsigned int)&(((p_type)NULL)->field)))

#else /* !CRAY2 */

#define XtOffset(p_type, field) ((unsigned int)&(((p_type)NULL)->field))

#endif /* !CRAY2 */
#endif /* __STDC__ */
#else /* ! (CRAY || __arm) */

#define XtOffset(p_type, field) \
    ((long) (((char *) (&(((p_type)NULL)->field))) - ((char *) NULL)))

#endif /* !CRAY */
#endif /* ! XtOffset */

#ifndef XtOffsetOf
#ifdef offsetof
#define XtOffsetOf(s_type, field) offsetof(s_type, field)
#else
#define XtOffsetOf(s_type, field) XtOffset(s_type*, field)
#endif
#endif /* !XtOffsetOf */

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
