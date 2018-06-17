#ifndef core_h
#define core_h

/* define a macro to make abbreviation */
#define cclass(class, member) class##member

/* indicate this API's implemention is System dependent */
#define PLATFORM_API

/* indicate this API's implemention is Architecture dependent */
#define ARCH_API

#endif