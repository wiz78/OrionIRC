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
   | Author: Rasmus Lerdorf <rasmus@lerdorf.on.ca>                        |
   +----------------------------------------------------------------------+
*/

/* $Id: php_type.h,v 1.1 2005/01/22 00:27:37 wiz Exp $ */

#ifndef PHP_TYPE_H
#define PHP_TYPE_H

ZEND_FUNCTION(intval);
ZEND_FUNCTION(floatval);
ZEND_FUNCTION(strval);
ZEND_FUNCTION(gettype);
ZEND_FUNCTION(settype);
ZEND_FUNCTION(is_null);
ZEND_FUNCTION(is_resource);
ZEND_FUNCTION(is_bool);
ZEND_FUNCTION(is_long);
ZEND_FUNCTION(is_float);
ZEND_FUNCTION(is_numeric);
ZEND_FUNCTION(is_string);
ZEND_FUNCTION(is_array);
ZEND_FUNCTION(is_object);
ZEND_FUNCTION(is_scalar);
ZEND_FUNCTION(is_callable);

#endif
