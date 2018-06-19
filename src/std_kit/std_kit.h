#ifndef std_kit_std_kit_h
#define std_kit_std_kit_h

#include <stdio.h>
#include <stdlib.h>

/* malloc with 0 and NULL check */
void *safe_malloc(size_t size);

/* malloc macro */
#define SAFE_MALLOC_TYPE(TYPE) (TYPE *)safe_malloc(sizeof(TYPE));

/* safe free*/
#define SAFE_FREE(obj)                                                                                                 \
    do {                                                                                                               \
        free(obj);                                                                                                     \
        obj = NULL;                                                                                                    \
    } while (0);

#endif

#define CHECK(repr)

#define ERROR_LOG_STR(str)

#define ERROR_LOG(fmt, ...)