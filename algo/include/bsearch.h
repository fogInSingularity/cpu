#ifndef BSEARCH_H
#define BSEARCH_H

#include <stddef.h>
#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include <stdint.h>
#include <memory.h>

/// @brief alias for compare function
typedef int compare_t(const void* a, const void* b);

void* BSearch(const void* elem, void* left, size_t size, size_t elSize, compare_t* CompareFunc);

#endif // BSEARCH_H
