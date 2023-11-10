#ifndef DEBUG_H
#define DEBUG_H

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

#include "color.h"

#ifdef _DEBUG
#define PRINT_BYTE(n)   fprintf(stderr, ">>> %x <<<\n", n);
#define PRINT_LONG(n)   fprintf(stderr, ">>> %ld <<<\n", n);
#define PRINT_ULONG(n)  fprintf(stderr, ">>> %lu <<<\n", n);
#define PRINT_ULX(n)    fprintf(stderr, ">>> %lX <<<\n", n);
#define PRINT_INT(n)    fprintf(stderr, ">>> %d <<<\n", n);
#define PRINT_UINT(n)    fprintf(stderr, ">>> %u <<<\n", n);
#define PRINT_DOUBLE(n) fprintf(stderr, ">>> %lf <<<\n", n);

#define $        fprintf(stderr, BOLD MAGENTA ">>> %s(%d) %s\n"  RESET,__FILE__, __LINE__, __PRETTY_FUNCTION__);
#define $$(...) {fprintf(BOLD MAGENTA stderr, ">>> %s\n" RESET, #__VA_ARGS__); __VA_ARGS__;}

#define ASSERT(expr) \
  if ((expr) == 0) { \
    fprintf(stderr, BOLD RED "#  ERROR:\n"); \
    fprintf(stderr, "#    file: %s\n", __FILE__); \
    fprintf(stderr, "#    line %d\n", __LINE__); \
    fprintf(stderr, "#    func: %s\n" RESET, __PRETTY_FUNCTION__); \
    abort(); \
    }
#else // ifndef _DEBUG
#define PRINT_LONG(n) ;
#define PRINT_ULONG(n) ;
#define PRINT_ULX(n) ;
#define PRINT_INT(n) ;
#define PRINT_DOUBLE(n) ;

#define $ ;
#define $$(...) {__VA_ARGS__;}

#define ASSERT(expr) ;
#endif // _DEBUG

#endif
