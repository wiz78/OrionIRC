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
   | Authors: Rasmus Lerdorf <rasmus@php.net>                             |
   |          Stig Sæther Bakken <ssb@php.net>                            |
   +----------------------------------------------------------------------+
*/

/* $Id: php_string.h,v 1.1 2005/01/22 00:27:37 wiz Exp $ */

/* Synced with php 3.0 revision 1.43 1999-06-16 [ssb] */

#ifndef PHP_STRING_H
#define PHP_STRING_H

ZEND_FUNCTION(strspn);
ZEND_FUNCTION(strcspn);
ZEND_FUNCTION(str_replace);
ZEND_FUNCTION(str_ireplace);
ZEND_FUNCTION(rtrim);
ZEND_FUNCTION(trim);
ZEND_FUNCTION(ltrim);
ZEND_FUNCTION(soundex);
ZEND_FUNCTION(levenshtein);

ZEND_FUNCTION(count_chars);
ZEND_FUNCTION(wordwrap);
ZEND_FUNCTION(explode);
ZEND_FUNCTION(implode);
ZEND_FUNCTION(strtok);
ZEND_FUNCTION(strtoupper);
ZEND_FUNCTION(strtolower);
ZEND_FUNCTION(basename);
ZEND_FUNCTION(dirname);
ZEND_FUNCTION(pathinfo);
ZEND_FUNCTION(strstr);
ZEND_FUNCTION(strpos);
ZEND_FUNCTION(stripos);
ZEND_FUNCTION(strrpos);
ZEND_FUNCTION(strripos);
ZEND_FUNCTION(strrchr);
ZEND_FUNCTION(substr);
ZEND_FUNCTION(quotemeta);
ZEND_FUNCTION(ucfirst);
ZEND_FUNCTION(ucwords);
ZEND_FUNCTION(strtr);
ZEND_FUNCTION(strrev);
ZEND_FUNCTION(hebrev);
ZEND_FUNCTION(hebrevc);
ZEND_FUNCTION(user_sprintf);
ZEND_FUNCTION(user_printf);
ZEND_FUNCTION(vprintf);
ZEND_FUNCTION(vsprintf);
ZEND_FUNCTION(addcslashes);
ZEND_FUNCTION(addslashes);
ZEND_FUNCTION(stripcslashes);
ZEND_FUNCTION(stripslashes);
ZEND_FUNCTION(chr);
ZEND_FUNCTION(ord);
ZEND_FUNCTION(nl2br);
ZEND_FUNCTION(setlocale);
ZEND_FUNCTION(localeconv);
ZEND_FUNCTION(nl_langinfo);
ZEND_FUNCTION(stristr);
ZEND_FUNCTION(chunk_split);
ZEND_FUNCTION(parse_str);
ZEND_FUNCTION(bin2hex);
ZEND_FUNCTION(similar_text);
ZEND_FUNCTION(strip_tags);
ZEND_FUNCTION(str_repeat);
ZEND_FUNCTION(substr_replace);
ZEND_FUNCTION(strnatcmp);
ZEND_FUNCTION(strnatcasecmp);
ZEND_FUNCTION(substr_count);
ZEND_FUNCTION(str_pad);
ZEND_FUNCTION(sscanf);
ZEND_FUNCTION(str_shuffle);
ZEND_FUNCTION(str_word_count);
ZEND_FUNCTION(str_split);
ZEND_FUNCTION(strpbrk);
ZEND_FUNCTION(substr_compare);
#ifdef HAVE_STRCOLL
ZEND_FUNCTION(strcoll);
#endif
#if HAVE_STRFMON
ZEND_FUNCTION(money_format);
#endif

#if defined(HAVE_LOCALECONV) && defined(ZTS)
PHP_MINIT_FUNCTION(localeconv);
PHP_MSHUTDOWN_FUNCTION(localeconv);
#endif
#if HAVE_NL_LANGINFO
PHP_MINIT_FUNCTION(nl_langinfo);
#endif

#define strnatcmp(a, b) \
	strnatcmp_ex(a, strlen(a), b, strlen(b), 0)
#define strnatcasecmp(a, b) \
	strnatcmp_ex(a, strlen(a), b, strlen(b), 1)
PHPAPI int strnatcmp_ex(char const *a, size_t a_len, char const *b, size_t b_len, int fold_case);

#ifdef HAVE_LOCALECONV
PHPAPI struct lconv *localeconv_r(struct lconv *out);
#endif

PHPAPI char *php_strtoupper(char *s, size_t len);
PHPAPI char *php_strtolower(char *s, size_t len);
PHPAPI char *php_strtr(char *str, int len, char *str_from, char *str_to, int trlen);
PHPAPI char *php_addslashes(char *str, int length, int *new_length, int freeit TSRMLS_DC);
PHPAPI char *php_addcslashes(char *str, int length, int *new_length, int freeit, char *what, int wlength TSRMLS_DC);
PHPAPI void php_stripslashes(char *str, int *len TSRMLS_DC);
PHPAPI void php_stripcslashes(char *str, int *len);
PHPAPI void php_basename(char *s, size_t len, char *suffix, size_t sufflen, char **p_ret, size_t *p_len TSRMLS_DC);
PHPAPI size_t php_dirname(char *str, size_t len);
PHPAPI char *php_stristr(unsigned char *s, unsigned char *t, size_t s_len, size_t t_len);
PHPAPI char *php_str_to_str_ex(char *haystack, int length, char *needle,
		int needle_len, char *str, int str_len, int *_new_length, int case_sensitivity, int *replace_count);
PHPAPI char *php_str_to_str(char *haystack, int length, char *needle,
		int needle_len, char *str, int str_len, int *_new_length);
PHPAPI char *php_trim(char *c, int len, char *what, int what_len, zval *return_value, int mode TSRMLS_DC);
PHPAPI size_t php_strip_tags(char *rbuf, int len, int *state, char *allow, int allow_len);
PHPAPI int php_char_to_str_ex(char *str, uint len, char from, char *to, int to_len, zval *result, int case_sensitivity, int *replace_count);
PHPAPI int php_char_to_str(char *str, uint len, char from, char *to, int to_len, zval *result);
PHPAPI void php_implode(zval *delim, zval *arr, zval *return_value);
PHPAPI void php_explode(zval *delim, zval *str, zval *return_value, int limit);

PHPAPI size_t php_strspn(char *s1, char *s2, char *s1_end, char *s2_end); 
PHPAPI size_t php_strcspn(char *s1, char *s2, char *s1_end, char *s2_end); 

#ifndef HAVE_STRERROR
PHPAPI char *php_strerror(int errnum);
#define strerror php_strerror
#endif

#ifndef HAVE_MBLEN
# define php_mblen(ptr, len) 1
#else
# if defined(_REENTRANT) && defined(HAVE_MBRLEN) && defined(HAVE_MBSTATE_T)
#  define php_mblen(ptr, len) ((ptr) == NULL ? mbsinit(&BG(mblen_state)): (int)mbrlen(ptr, len, &BG(mblen_state)))
# else
#  define php_mblen(ptr, len) mblen(ptr, len)
# endif
#endif

void register_string_constants(INIT_FUNC_ARGS);

#endif /* PHP_STRING_H */
