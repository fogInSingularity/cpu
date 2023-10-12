#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

#include "../../lib/include/debug.h"

void FillBytes(void* dest, const void* src, size_t nElem, size_t sizeElem);

#endif
