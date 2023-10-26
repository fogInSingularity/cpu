#include "../include/darray.h"

//-----------------------------------------------------------------------------

static void DArrayResizeUp(DArray* dArr);
static void DArrayResizeDown(DArray* dArr);

static void DArrayRecalloc(DArray* dArr);

//-----------------------------------------------------------------------------

void DArrayCtor(DArray* dArr, size_t elemSize, size_t initCap) {
  dArr->elemSize = elemSize;
  dArr->size = 0;
  dArr->cap = (initCap < DArrayStandartAllocSize) ? DArrayStandartAllocSize : initCap;

  dArr->array = calloc(dArr->cap, dArr->elemSize);
}

void DArrayDtor(DArray* dArr) {
  free(dArr->array);
  dArr->array = nullptr;

  dArr->cap = 0;
  dArr->size = 0;
  dArr->elemSize = 0;
}

void DArrayPushBack(DArray* dArr, void* elem) {
  DArrayResizeUp(dArr);

  memcpy((char*)dArr->array + (dArr->size++)*dArr->elemSize, elem, dArr->elemSize);
}

void DArrayPopBack(DArray* dArr, void* elem) {
  DArrayResizeDown(dArr);

  memcpy(elem, (char*)dArr->array + (--dArr->size)*dArr->elemSize, dArr->elemSize);
}

void DArraySetAt(DArray* dArr, void* elem, size_t index) {
  memcpy((char*)dArr->array + index*dArr->elemSize, elem, dArr->elemSize);
}

void DArrayGetAt(DArray* dArr, void* elem, size_t index) {
  memcpy(elem, (char*)dArr->array + index*dArr->elemSize, dArr->elemSize);
}

void DArrayAt(DArray* dArr, size_t index, void** retPtr) {
  *retPtr = (char*)dArr->array + index*dArr->elemSize;
}

void DArrayDump(DArray* dArr, Dump_t* DumpElemFunc, const char* file, size_t line, const char* func) {
  fprintf(stderr, "#####################################################\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "#  dynamic array called from file %s(%lu) from function %s\n", file, line, func);
  fprintf(stderr, "#  {\n");
  fprintf(stderr, "#    array\n");
  fprintf(stderr, "#    {\n");
  // pirnt array
  for (size_t i = 0; i < dArr->size; i++) {
  fprintf(stderr, "#      *[%lu] ", i);
    DumpElemFunc((char*)dArr->array + i*dArr->elemSize);
    fprintf(stderr, "\n");
  }

  for (size_t i = dArr->size; i < dArr->cap; i++) {
  fprintf(stderr, "#       [%lu] ", i);
    DumpElemFunc((char*)dArr->array + i*dArr->elemSize);
    fprintf(stderr, "\n");
  }
  //
  fprintf(stderr, "#    }\n");
  fprintf(stderr, "#    size %lu\n", dArr->size);
  fprintf(stderr, "#    cap %lu\n", dArr->cap);
  fprintf(stderr, "#    elemSize %lu\n", dArr->elemSize);
  fprintf(stderr, "#  }\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "#####################################################\n");
}

//-----------------------------------------------------------------------------

static void DArrayResizeUp(DArray* dArr) {
  if (dArr->cap <= dArr->size) {
    dArr->cap *= DArrayMultiplyConst;

    DArrayRecalloc(dArr);
  }
}

static void DArrayResizeDown(DArray* dArr) {
  if (dArr->size * DArrayMultiplyConst * DArrayMultiplyConst <= dArr->cap) {
    dArr->cap = (dArr->cap > DArrayMultiplyConst) ? (dArr->cap / DArrayMultiplyConst) : DArrayMultiplyConst;

    DArrayRecalloc(dArr);
  }
}

static void DArrayRecalloc(DArray* dArr) {
  dArr->array = realloc(dArr->array, dArr->cap*dArr->elemSize);

  memset((char*)dArr->array + dArr->size*dArr->elemSize, 0, (dArr->cap - dArr->size)*dArr->elemSize);
}
