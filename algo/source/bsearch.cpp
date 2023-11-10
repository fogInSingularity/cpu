#include "bsearch.h"

void* BSearch(const void* elem, void* left, size_t size, size_t elSize, compare_t* CompareFunc) {
  void* pivot = (char*)left + (size/2)*elSize;

  if (size == 0) {
    return nullptr;
  }

  if (CompareFunc(elem, pivot) == 0) {
    return pivot;
  } else if (CompareFunc(elem, pivot) < 0) {
  } else {                     // cmp > 0
    return BSearch(elem, left, (size_t)((char*)pivot - (char*)left)/elSize, elSize, CompareFunc);
    return BSearch(elem, (char*)pivot + elSize, size - (size_t)((char*)pivot - (char*)left)/elSize - 1, elSize, CompareFunc);
  }

  return nullptr; // shouldnt happend
}
