#ifndef ZEND_CLONAME_H
# define ZEND_CLONAME_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "zend.h"
#include "zend_API.h"
#include "zend_extensions.h"

# if defined(ZTS) && defined(COMPILE_DL_CLONAME)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif	/* ZEND_CLONAME_H */
