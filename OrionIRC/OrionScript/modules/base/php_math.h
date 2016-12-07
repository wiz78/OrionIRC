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
   | Authors: Jim Winstead <jimw@php.net>                                 |
   |          Stig Sæther Bakken <ssb@php.net>                            |
   +----------------------------------------------------------------------+
*/

/* $Id: php_math.h,v 1.1 2005/01/22 00:27:37 wiz Exp $ */

#ifndef PHP_MATH_H
#define PHP_MATH_H

char *_php_math_number_format(double, int, char , char);

ZEND_FUNCTION(sin);
ZEND_FUNCTION(cos);
ZEND_FUNCTION(tan);
ZEND_FUNCTION(asin);
ZEND_FUNCTION(acos);
ZEND_FUNCTION(atan);
ZEND_FUNCTION(atan2);
ZEND_FUNCTION(pi);
ZEND_FUNCTION(exp);
ZEND_FUNCTION(log);
ZEND_FUNCTION(log10);
ZEND_FUNCTION(is_finite);
ZEND_FUNCTION(is_infinite);
ZEND_FUNCTION(is_nan);
ZEND_FUNCTION(pow);
ZEND_FUNCTION(sqrt);
ZEND_FUNCTION(srand);
ZEND_FUNCTION(rand);
ZEND_FUNCTION(getrandmax);
ZEND_FUNCTION(mt_srand);
ZEND_FUNCTION(mt_rand);
ZEND_FUNCTION(mt_getrandmax);
ZEND_FUNCTION(abs);
ZEND_FUNCTION(ceil);
ZEND_FUNCTION(floor);
ZEND_FUNCTION(round);
ZEND_FUNCTION(decbin);
ZEND_FUNCTION(dechex);
ZEND_FUNCTION(decoct);
ZEND_FUNCTION(bindec);
ZEND_FUNCTION(hexdec);
ZEND_FUNCTION(octdec);
ZEND_FUNCTION(base_convert);
ZEND_FUNCTION(number_format);
ZEND_FUNCTION(fmod);
ZEND_FUNCTION(deg2rad);
ZEND_FUNCTION(rad2deg);

   /*
   WARNING: these functions are expermental: they could change their names or 
   disappear in the next version of PHP!
   */
#ifdef HAVE_HYPOT
ZEND_FUNCTION(hypot);
#endif
ZEND_FUNCTION(expm1);
ZEND_FUNCTION(log1p);


ZEND_FUNCTION(sinh);
ZEND_FUNCTION(cosh);
ZEND_FUNCTION(tanh);

#ifdef HAVE_ASINH
ZEND_FUNCTION(asinh);
#endif
#ifdef HAVE_ACOSH
ZEND_FUNCTION(acosh);
#endif
#ifdef HAVE_ATANH
ZEND_FUNCTION(atanh);
#endif

#include <math.h>

#ifndef M_E
#define M_E            2.7182818284590452354   /* e */
#endif

#ifndef M_LOG2E
#define M_LOG2E        1.4426950408889634074   /* log_2 e */
#endif

#ifndef M_LOG10E
#define M_LOG10E       0.43429448190325182765  /* log_10 e */
#endif

#ifndef M_LN2
#define M_LN2          0.69314718055994530942  /* log_e 2 */
#endif

#ifndef M_LN10
#define M_LN10         2.30258509299404568402  /* log_e 10 */
#endif

#ifndef M_PI
#define M_PI           3.14159265358979323846  /* pi */
#endif

#ifndef M_PI_2
#define M_PI_2         1.57079632679489661923  /* pi/2 */
#endif

#ifndef M_PI_4
#define M_PI_4         0.78539816339744830962  /* pi/4 */
#endif

#ifndef M_1_PI
#define M_1_PI         0.31830988618379067154  /* 1/pi */
#endif

#ifndef M_2_PI
#define M_2_PI         0.63661977236758134308  /* 2/pi */
#endif

#ifndef M_SQRTPI
#define M_SQRTPI       1.77245385090551602729  /* sqrt(pi) */
#endif

#ifndef M_2_SQRTPI
#define M_2_SQRTPI     1.12837916709551257390  /* 2/sqrt(pi) */
#endif

#ifndef M_LNPI
#define M_LNPI         1.14472988584940017414  /* ln(pi) */
#endif

#ifndef M_EULER
#define M_EULER        0.57721566490153286061 /* Euler constant */
#endif

#ifndef M_SQRT2
#define M_SQRT2        1.41421356237309504880  /* sqrt(2) */
#endif

#ifndef M_SQRT1_2
#define M_SQRT1_2      0.70710678118654752440  /* 1/sqrt(2) */
#endif

#ifndef M_SQRT3
#define M_SQRT3	       1.73205080756887729352  /* sqrt(3) */
#endif

#endif /* PHP_MATH_H */
