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
   | Author: Wez Furlong <wez@php.net>                                    |
   +----------------------------------------------------------------------+
*/

/* $Id: globals.c,v 1.1 2005/01/22 15:07:28 wiz Exp $ */

#include "php.h"
#include "php_win32_globals.h"

#ifdef ZTS
PHPAPI int php_win32_core_globals_id;
#else
php_win32_core_globals php_win32_core_globals_;
#endif

void php_win32_core_globals_ctor(void *vg TSRMLS_DC)
{
	php_win32_core_globals *wg = (php_win32_core_globals*)vg;
	memset(wg, 0, sizeof(*wg));
}

PHP_RSHUTDOWN_FUNCTION(win32_core_globals)
{
	php_win32_core_globals *wg =
#ifdef ZTS
		ts_resource(php_win32_core_globals_id)
#else
		&php_win32_core_globals_
#endif
		;

	STR_FREE(wg->login_name);
	
	memset(wg, 0, sizeof(*wg));
	return SUCCESS;
}

