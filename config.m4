dnl config.m4 for extension cloname

PHP_ARG_ENABLE([cloname],
  [whether to enable cloname support],
  [AS_HELP_STRING([--enable-cloname],
    [Enable cloname support])],
  [no])

if test "$PHP_CLONAME" != "no"; then
  AC_DEFINE(HAVE_CLONAME, 1, [ Have cloname support ])

  PHP_NEW_EXTENSION(cloname, 
        zend_cloname.c,
        $ext_shared,,-DZEND_ENABLE_STATIC_TSRMLS_CACHE=1,,yes)

  PHP_SUBST(CLONAME_SHARED_LIBADD)
fi
