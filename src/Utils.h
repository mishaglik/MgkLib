#ifndef MGK_UTILS_H
#define MGK_UTILS_H
#include <stdlib.h>
#include "MGK/Logger.h"


#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

struct DBG_Location
{
    const char *filename = NULL;
    const char *function = NULL;
    const char *var_name = NULL;

    int line = 0;
};

#undef  LOCATION
#define LOCATION(x) {__FILE__, __func__, #x, __LINE__}

#ifndef NDEBUG
#define MGK_OBJECT DBG_Location _dbg_location = {};

#define MGK_CTOR_DEC DBG_Location dbg_location
#define MGK_CTOR_DEF(obj) obj->_dbg_location = dbg_location;
#define MGK_CTOR_CALL(x) LOCATION(x)

#ifdef LOG_DEBUG
#define LOG_OBJ_LOCATION(obj) if(obj != NULL) {LOG_DEBUG("Object \"%s\"[%p] born in file \"%s\" in %s(%d) with name %s:\n",\
    #obj, obj ,obj->_dbg_location.filename, obj->_dbg_location.function, obj->_dbg_location.line, obj->_dbg_location.var_name)} else{LOG_DEBUG("Object %s [%p]", #obj, obj);} 

#define LOG_CUR_LOCATION    LOG_DEBUG("From file \"%s\" function %s(%d)", __FILE__, __func__, __LINE__)

#define LOG_ASSERT(x) (x) ? void(0) : ({LOG_FATAL("%s:%d Assertion \"%s\" failed.", __FILE__, __LINE__, #x); abort();})
#endif
#else

#define MGK_OBJECT
#ifdef LOG_DEBUG
#define LOG_OBJ_LOCATION(obj) 
#define LOG_CUR_LOCATION
#endif
#define MGK_CTOR_DEC
#define MGK_CTOR_DEF
#define MGK_CTOR_CALL(x)
#define LOG_ASSERT(x)

#endif

#ifdef __GNUC__
#define mgk_calloc(n, sz)  ({void* _ptr =  calloc((n), (sz)); LOG_ASSERT(_ptr != NULL && "Bad calloc on line:" && __LINE__ && "in file:" && __FILE__); _ptr;})
#define mgk_realloc(old_ptr, n, sz) ({void* _ptr = realloc(old_ptr, (n) * (sz)); LOG_ASSERT(_ptr != NULL && "Bad calloc on line:" && __LINE__ && "in file:" && __FILE__); _ptr;})
#else
#error "You must use gcc compliler"
#endif
#endif