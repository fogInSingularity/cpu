#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>
#include <stdint.h>
#include <assert.h>
#include <malloc.h>
#include <memory.h>

#include "utils.h"
#include "hash.h"

#ifdef _DEBUG
  #define ON_DEBUG(...) __VA_VARGS__
#else
  #define ON_DEBUG(...)
#endif // _DEBUG

#ifdef _CANARY_PROT
  #define ON_CANARY(...) __VA_ARGS__
#else
  #define ON_CANARY(...)
#endif // _CANARY_PROT

#ifdef _HASH_PROT
  #define ON_HASH(...) __VA_VARGS__
#else
  #define ON_HASH(...)
#endif // _HASH_PROT

typedef double elem_t;
const ssize_t StandardAllocSize = 8;

#ifdef _CANARY_PROT
typedef uint64_t canary_t;
#endif // _CANARY_PROT

struct Stack {
  #ifdef _CANARY_PROT
  canary_t lcanary;
  #endif // _CANARY_PROT

  elem_t* data;
  ssize_t size;
  ssize_t capacity;

  #ifdef _HASH_PROT
  uint32_t stackHash;
  uint32_t dataHash;
  #endif

  #ifdef _CANARY_PROT
  canary_t rcanary;
  #endif // _CANARY_PROT
};

typedef size_t StackError;

enum StackState {
  Success          = 0,
  ErrorCtor        = 1 << 0,
  ErrorDtor        = 1 << 1,
  ErrorRecalloc    = 1 << 2,
  ErrorPopNoEl     = 1 << 3,
  // ---StackOkErrors---
  ErrorStkNull     = 1 << 4,
  ErrorStkLCanary  = 1 << 5,
  ErrorStkRCanary  = 1 << 6,
  ErrorStkSizeNeg  = 1 << 7,
  ErrorStkCapNeg   = 1 << 8,
  ErrorStkHash     = 1 << 9,
  ErrorDataNull    = 1 << 10,
  ErrorDataLCanary = 1 << 11,
  ErrorDataRCanary = 1 << 12,
  ErrorDataHash    = 1 << 13,
};

StackError StackCtor(Stack* stk, ssize_t initCap = StandardAllocSize);

StackError StackDtor(Stack* stk);

StackError StackOk(Stack* stk);

StackError StackPush(Stack* stk, elem_t value);

StackError StackPop(Stack* stk, elem_t* retValue);

void StackDump(Stack* stk, StackError error, const char* file, size_t line, const char* func);

#define STACK_DUMP(stk, error) StackDump(stk, error, __FILE__, __LINE__, __func__);

#ifdef _DEBUG
  #define STACK_ASSERT(stk) \
  do { \
    StackError AssertHoldErr = StackOk(stk); \
    if (AssertHoldErr != StackState::Success) { \
      STACK_DUMP(stk, AssertHoldErr); \
      abort(); \
    } \
  } while (0);
#else
  #define STACK_ASSERT(stk) ;
#endif

#endif // STACK_H
