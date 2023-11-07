#ifndef MY_UTILS_H
#define MY_UTILS_H

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#include "debug.h"

void SwapBytes(void* a, void* b, size_t size);

void FillBytes(void* dest, const void* src, size_t nElem, size_t sizeElem);

#endif // MY_UTILS_H
