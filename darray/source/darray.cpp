#include "../include/darray.h"

//public-----------------------------------------------------------------------

DArrayError DArray::Ctor(size_t initElemSize, size_t initCap) {
  elemSize = initElemSize;
  size = 0;
  cap = (DArrayStandartAllocSize > initCap) ?
         DArrayStandartAllocSize : initCap;

  array = calloc(cap, elemSize);
  if (array == nullptr) {
    return DArrayError::CTOR_CANT_ALLOC;
  }

  return DArrayError::SUCCESS;
}

DArrayError DArray::Dtor() {
  free(array);
  array = nullptr;

  cap = 0;
  size = 0;
  elemSize = 0;

  return DArrayError::SUCCESS;
}

DArrayError DArray::PushBack(void* elem) {
  DArrayError error = ResizeUp_();
  if (error != DArrayError::SUCCESS) {
    return error;
  }

  memcpy((char*)array + size*elemSize, elem, elemSize);
  size++;

  return error;
}

DArrayError DArray::PopBack(void* elem) {
  DArrayError error = ResizeDown_();

  if (size <= 0) {
    return DArrayError::EMPTY_DARR_POP;
  }

  if (error != DArrayError::SUCCESS) {
    return error;
  }

  size--;
  memcpy(elem, (char*)array + size*elemSize, elemSize);

  return error;
}

DArrayError DArray::SetAt(void* elem, size_t index) {
  if (index > size - 1) {
    return DArrayError::SETAT_OUT_OF_BOUNDS;
  }

  memcpy((char*)array + index*elemSize, elem, elemSize);

  return DArrayError::SUCCESS;
}

DArrayError DArray::GetAt(void* elem, size_t index) {
  if (index > size - 1) {
    return DArrayError::GETAT_OUT_OF_BOUNDS;
  }

  memcpy(elem, (char*)array + index*elemSize, elemSize);

  return DArrayError::SUCCESS;
}

DArrayError DArray::At(void** retPtr, size_t index) {
  if (index > size - 1) {
    return DArrayError::AT_OUT_OF_BOUNDS;
  }

  *retPtr = (char*)array + index*elemSize;

  return DArrayError::SUCCESS;
}

void DArray::Dump(Dump_t* DumpElemFunc, const char* file, size_t line, const char* func) {
  fprintf(stderr, "#####################################################\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "#  dynamic array called from file %s(%lu) from function %s\n", file, line, func);
  fprintf(stderr, "#  {\n");
  fprintf(stderr, "#    array\n");
  fprintf(stderr, "#    {\n");
  // pirnt array
  for (size_t i = 0; i < size; i++) {
  fprintf(stderr, "#      *[%lu] ", i);
    DumpElemFunc((char*)array + i*elemSize);
    fprintf(stderr, "\n");
  }

  for (size_t i = size; i < cap; i++) {
  fprintf(stderr, "#       [%lu] ", i);
    DumpElemFunc((char*)array + i*elemSize);
    fprintf(stderr, "\n");
  }
  //
  fprintf(stderr, "#    }\n");
  fprintf(stderr, "#    size %lu\n", size);
  fprintf(stderr, "#    cap %lu\n", cap);
  fprintf(stderr, "#    elemSize %lu\n", elemSize);
  fprintf(stderr, "#  }\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "#####################################################\n");
}

//private----------------------------------------------------------------------

DArrayError DArray::ResizeUp_() {
  if (cap <= size) {
    cap *= DArrayMultiplyConst;

    return Recalloc_();
  }

  return DArrayError::SUCCESS;
}

DArrayError DArray::ResizeDown_() {
  if (size * DArrayMultiplyConst * DArrayMultiplyConst <= cap) {
    cap = (cap > DArrayStandartAllocSize) ? (cap / DArrayMultiplyConst) : DArrayStandartAllocSize; //NOTE - might have bug

    return Recalloc_();
  }

  return DArrayError::SUCCESS;
}

DArrayError DArray::Recalloc_() {
  void* hold = array;
  array = realloc(array, cap*elemSize);
  if (array == nullptr) {
    array = hold;
    return DArrayError::RECLC_CANT_ALLOC;
  }

  memset((char*)array + size*elemSize, 0, (cap - size)*elemSize);

  return DArrayError::SUCCESS;
}
