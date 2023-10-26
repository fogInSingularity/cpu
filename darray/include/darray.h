#ifndef DARRAY_H
#define DARRAY_H

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <memory.h>
#include <stdio.h>

static const size_t DArrayStandartAllocSize = 8;
static const size_t DArrayMultiplyConst = 2;

struct DArray {
  void* array;

  size_t size;
  size_t cap;
  size_t elemSize;
};

void DArrayCtor(DArray* dArr, size_t elemSize, size_t initCap = DArrayStandartAllocSize);
void DArrayDtor(DArray* dArr);

void DArrayPushBack(DArray* dArr, void* elem);
void DArrayPopBack(DArray* dArr, void* elem);

void DArraySetAt(DArray* dArr, void* elem, size_t index);
void DArrayGetAt(DArray* dArr, void* elem, size_t index);

void DArrayAt(DArray* dArr, size_t index, void** retPtr);

typedef void Dump_t(void* elem);

#define DARRAY_DUMP(dArr, funcRef) DArrayDump(dArr,funcRef, __FILE__, __LINE__, __func__);
void DArrayDump(DArray* dArr, Dump_t* DumpElemFunc, const char* file, size_t line, const char* func);

#endif
