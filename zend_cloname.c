/* closure naming demo */
#ifndef ZEND_CLONAME
# define ZEND_CLONAME

#define ZEND_CLONAME_EXTNAME   "Closure Name"
#define ZEND_CLONAME_VERSION   "0.0.0-dev"
#define ZEND_CLONAME_AUTHOR    "krakjoe"
#define ZEND_CLONAME_URL       "https://github.com/krakjoe/cloname"
#define ZEND_CLONAME_COPYRIGHT "Copyright (c) 2021"

#if defined(__GNUC__) && __GNUC__ >= 4
# define ZEND_CLONAME_EXTENSION_API __attribute__ ((visibility("default")))
#else
# define ZEND_CLONAME_EXTENSION_API
#endif

#include "zend_cloname.h"

static int  zend_cloname_startup(zend_extension*);
static void zend_cloname_activate(void);
static void zend_cloname_setup(zend_op_array*);
static void zend_cloname_link(zend_op_array*);

static int zend_cloname_resource;

ZEND_CLONAME_EXTENSION_API zend_extension_version_info extension_version_info = {
    ZEND_EXTENSION_API_NO,
    ZEND_EXTENSION_BUILD_ID
};

ZEND_CLONAME_EXTENSION_API zend_extension zend_extension_entry = {
    ZEND_CLONAME_EXTNAME,
    ZEND_CLONAME_VERSION,
    ZEND_CLONAME_AUTHOR,
    ZEND_CLONAME_URL,
    ZEND_CLONAME_COPYRIGHT,
    zend_cloname_startup,
    NULL,
    zend_cloname_activate,
    NULL,
    NULL,
    zend_cloname_link,
    NULL,
    NULL,
    NULL,
    zend_cloname_setup,
    NULL,
    STANDARD_ZEND_EXTENSION_PROPERTIES
};

static int zend_cloname_startup(zend_extension *ze) {
    /* reserve a resource */
#if ZEND_EXTENSION_API_NO > 400000000
    zend_cloname_resource = zend_get_resource_handle(ZEND_CLONAME_EXTNAME);
#else
    zend_cloname_resource = zend_get_resource_handle(ze);
#endif

    return SUCCESS;
}

static void zend_cloname_activate(void) {
#if defined(ZEND_TSRMLS_CACHE_UPDATE)
# if defined(ZTS) && defined(COMPILE_DL_CLONAME)
    ZEND_TSRMLS_CACHE_UPDATE();
# endif
#endif
}

static void zend_cloname_setup(zend_op_array *ops) {
    /*
     We are in init_op_array - no names/flags etc are set
    */
    ops->reserved[zend_cloname_resource] = CG(active_op_array);
}

static void zend_cloname_link(zend_op_array *ops) {
    /*
     We are in pass_two, everything is set ...
    */
    zend_op_array *parent = ops->reserved[zend_cloname_resource];
    
    if (!(ops->fn_flags & ZEND_ACC_CLOSURE)) {
        /* don't use the slot */
        ops->reserved[zend_cloname_resource] = NULL;
        return;
    }
    
    zend_string *closureName;
    
    if (parent->scope) {
        /* Closure@Class::method:file#begin-end */
        closureName = zend_strpprintf(0, 
            "Closure@%s::%s:%s#%d-%d",
            ZSTR_VAL(parent->scope->name),
            ZSTR_VAL(parent->function_name),
            ZSTR_VAL(ops->filename),
            ops->line_start,
            ops->line_end);
    } else {
        /* Closure@function:file#begin-end */
        closureName = zend_strpprintf(0,
            "Closure@%s:%s#%d-%d",
            parent->function_name ? 
                ZSTR_VAL(parent->function_name) : "{main}",
            ZSTR_VAL(ops->filename),
            ops->line_start,
            ops->line_end);
    }

    /* don't have to care about freeing interned strings */
    ops->reserved[zend_cloname_resource] = 
        zend_new_interned_string(closureName);
        
    zend_printf("%s\n", ZSTR_VAL(closureName));
    
    /* free the non-interned string */
    zend_string_release(closureName);
}

#if defined(ZEND_TSRMLS_CACHE_DEFINE)
# if defined(ZTS) && defined(COMPILE_DL_CLONAME)
    ZEND_TSRMLS_CACHE_DEFINE();
# endif
#endif

#endif /* ZEND_CLONAME */
