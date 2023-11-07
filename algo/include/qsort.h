#ifndef SORT_H
#define SORT_H

#include <stddef.h>
#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include <stdint.h>
#include <memory.h>

/// @brief alias for compare function
typedef int Compare_t(const void* a, const void* b);

void Sort(void* left, void* right, size_t elSize, Compare_t* CompareFunc);

#endif // SORT_H
