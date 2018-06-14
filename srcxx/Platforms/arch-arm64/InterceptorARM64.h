//
// Created by jmpews on 2018/6/14.
//

#ifndef HOOKZZ_INTERCEPTORARM64_H
#define HOOKZZ_INTERCEPTORARM64_H

#include "MemoryManager.h"
#include "ARM64Writer.h"
#include "ARM64Reader.h"

typedef struct _InterceptorBackend {
  MemoryManager *mm;
  ARM64Relocator relocatorARM64;
  ARM64AssemblerWriter writerARM64;
  ARM64AssemblyReader readerARM64;
} InterceptorBackend;

#endif //HOOKZZ_INTERCEPTORARM64_H
