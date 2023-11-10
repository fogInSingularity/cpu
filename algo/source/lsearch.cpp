#include "lsearch.h"

void* LSearch(const void* key, void* base, size_t size, size_t elSize, compare_t* CompareFunc) {
  assert(key != nullptr);
  assert(base != nullptr);
  // assert(CompareFunc != nullptr);

  for (size_t i = 0; i < size; i++) {
    void* curElem = (char*)base + i*elSize;
    if (CompareFunc(key, curElem) == 0) {
      return curElem;
    }
  }

  return nullptr;
}
