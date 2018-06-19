#ifndef core_h
#define core_h

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "logging.h"
#include "std_kit/std_kit.h"


/* define a macro to make abbreviation */
#define cclass(class, member) class##_##member

/* indicate this API's implemention is System dependent */
#define PLATFORM_API

/* indicate this API's implemention is Architecture dependent */
#define ARCH_API

#endif