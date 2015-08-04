/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2015 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_arrayex.h"

/* If you declare any globals in php_arrayex.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(arrayex)
*/

/* True global resources - no need for thread safety here */
static int le_arrayex;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("arrayex.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_arrayex_globals, arrayex_globals)
    STD_PHP_INI_ENTRY("arrayex.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_arrayex_globals, arrayex_globals)
PHP_INI_END()
*/
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_arrayex_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_arrayex_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "arrayex", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/


/* {{{ php_arrayex_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_arrayex_init_globals(zend_arrayex_globals *arrayex_globals)
{
	arrayex_globals->global_value = 0;
	arrayex_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(arrayex)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(arrayex)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(arrayex)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(arrayex)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(arrayex)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "arrayex support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

ZEND_FUNCTION(arrayex_fetch_field)
{
    //定义参数 s 源数组 field 字段名
    zval *s;
    char *f;
    int f_len;

    //取得参数的值
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a|s", &s, &f, &f_len) == FAILURE)
    {
        RETURN_FALSE;
    }

    //定义结果数组
    array_init(return_value);

    //遍历 源数组 查找 相关字段 并生成新数组
    zval **ppzval;
    zval **fzval;
    for (zend_hash_internal_pointer_reset((*s).value.ht);
        zend_hash_has_more_elements((*s).value.ht) == SUCCESS;
        zend_hash_move_forward((*s).value.ht))
    {
        //取得当前元素，并判断是否是数组
        if (zend_hash_get_current_data((*s).value.ht, (void **) & ppzval) == FAILURE || Z_TYPE_PP(ppzval) != IS_ARRAY)
        {
            continue;
        }
        if (f_len)
        {
            if (zend_hash_find((**ppzval).value.ht, f, f_len + 1, (void **) & fzval) == FAILURE)
            {
                continue;
            }
            else
            {
                add_next_index_zval(return_value, *fzval);
            }
        }
        else
        {
            if (zend_hash_get_current_data((**ppzval).value.ht, (void **) & fzval) == FAILURE)
            {
                continue;
            }
            else
            {
                add_next_index_zval(return_value, *fzval);
            }
        }
    }
}

/* {{{ arrayex_functions[]
 *
 * Every user visible function must have an entry in arrayex_functions[].
 */
const zend_function_entry arrayex_functions[] = {
	PHP_FE(confirm_arrayex_compiled,	NULL)		/* For testing, remove later. */
        PHP_FE(arrayex_fetch_field, NULL)
	PHP_FE_END	/* Must be the last line in arrayex_functions[] */
};
/* }}} */

/* {{{ arrayex_module_entry
 */
zend_module_entry arrayex_module_entry = {
	STANDARD_MODULE_HEADER,
	"arrayex",
	arrayex_functions,
	PHP_MINIT(arrayex),
	PHP_MSHUTDOWN(arrayex),
	PHP_RINIT(arrayex),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(arrayex),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(arrayex),
	PHP_ARRAYEX_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_ARRAYEX
ZEND_GET_MODULE(arrayex)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
