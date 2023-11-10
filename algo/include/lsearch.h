#ifndef LSEARCH_H
#define LSEARCH_H

#include <stddef.h>
#include <stddef.h>
#include <assert.h>
#include <stdint.h>
#include <memory.h>

/// @brief alias for compare function
typedef int compare_t(const void* a, const void* b);

void* LSearch(const void* key, void* base, size_t size, size_t elSize, compare_t* CompareFunc);

#endif
