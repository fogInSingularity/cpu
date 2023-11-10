#ifndef DARRAY_H
#define DARRAY_H

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <memory.h>
#include <stdio.h>

#include "debug.h"

typedef void Dump_t(void* elem);

static const size_t DArrayStandartAllocSize = 8;
static const size_t DArrayMultiplyConst = 2;

enum DArrayError {
  SUCCESS,
  CTOR_CANT_ALLOC,
  RECLC_CANT_ALLOC,
  EMPTY_DARR_POP,
  SETAT_OUT_OF_BOUNDS,
  GETAT_OUT_OF_BOUNDS,
  AT_OUT_OF_BOUNDS,
};

struct DArray {
 public:
  void* array;
  size_t size;
  size_t cap;
  size_t elemSize;

  DArrayError Ctor(size_t elemSize, size_t initCap = DArrayStandartAllocSize);
  DArrayError Dtor();

  DArrayError PushBack(void* elem);
  DArrayError PopBack(void* elem);
  DArrayError SetAt(void* elem, size_t index);
  DArrayError GetAt(void* elem, size_t index);
  DArrayError At(void** retPtr, size_t index);
  void Dump(Dump_t* DumpElemFunc, const char* file, size_t line, const char* func);
 private:
  DArrayError ResizeUp_();
  DArrayError ResizeDown_();
  DArrayError Recalloc_();
};

#define DARRAY_DUMP(dArr, funcRef) dArr->Dump(funcRef, __FILE__, __LINE__, __func__);

#endif
