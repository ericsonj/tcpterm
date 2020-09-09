/*
 * Exceptions.h
 *
 *  Created on: Aug 28, 2020
 *      Author: Ericson Joseph
 */

#ifndef CORE_APPLICATION_EXCEPTIONS_H_
#define CORE_APPLICATION_EXCEPTIONS_H_

#include <stdlib.h>
#include <stdint.h>
#include <errno.h>

struct _Exception {
    int      line;
    int 	 error;
    uint32_t e;
};

typedef struct _Exception Exception_t;

#define TRY                      \
    do {                         \
    	__label__ __excep_label; \
        Exception_t __excep;     \
        __excep.line = 0;        \
        __excep.e    = 0;        \
        if (1)

#define THROW(ex)					\
    __excep.line  = __LINE__;		\
    __excep.error = errno;			\
    __excep.e     = __excep.e | ex; \
    goto __excep_label

#define CATCH(n, ...) \
    Exception_t n;    \
    __excep_label:    \
    if ((n = __excep).e & (__VA_ARGS__))

#define FINALLY\
    }          \
    while (0)  \
        ;      \
    if (1)

#define EXCEPTIONS_MAP(XX) \
    XX(NullPointerException, 		(1 << 0), 								"NullPointerException") \
    XX(ParseException, 				(1 << 1), 								"ParseException")       \
	XX(FileNotFoundException, 		(1 << 2), 								"FileNotFoundException")\
    XX(Exception, 					(uint32_t)0xFFFFFFFF, 					"Exception")			\

enum {
#define XX(name, value, str) name = value,
    EXCEPTIONS_MAP(XX)
#undef XX
};

#define THROW_IF_FAIL(expr, ex) \
    if (!(expr)) {              \
        THROW(ex);              \
    }


#endif /* CORE_APPLICATION_EXCEPTIONS_H_ */
