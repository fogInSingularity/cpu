#ifndef DEBUG_H
#define DEBUG_H

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

#include "color.h"

#define USE_VAR(v) (void)(v);
#define PRINT_LONG(n) fprintf(stderr, ">>> %ld <<<\n", n);
#define PRINT_INT(n) fprintf(stderr, ">>> %d <<<\n", n);
#define PRINT_DOUBLE(n) fprintf(stderr, ">>> %lf <<<\n", n);

#define CHECK_FILE(file, fileName, retVl) \
  if (file == nullptr) { \
    fprintf(stderr, RED BOLD "!  CANT OPEN FILE: %s\n" RESET, fileName); \
    fprintf(stderr, RED BOLD "!    %s(%d) %s\n" RESET, __FILE__, __LINE__, __PRETTY_FUNCTION__); \
    return retVl; \
  }

#define CHECK_BIT_FLAG(var, flag) ((var & (flag)) == (flag))

#ifdef _DEBUG
#define $        fprintf(stderr, BOLD MAGENTA ">>> %s(%d) %s\n"  RESET,__FILE__, __LINE__, __PRETTY_FUNCTION__);
#define $$(...) {fprintf(BOLD MAGENTA stderr, ">>> %s\n" RESET, #__VA_ARGS__); __VA_ARGS__;}

#define ASSERT(expr) \
  if ((expr) == 0) { \
    printf(BOLD RED "# ERROR:\n"); \
    printf("#   file: %s\n", __FILE__); \
    printf("#   func: %s\n" RESET, __PRETTY_FUNCTION__); \
    abort(); \
    }
#else
#define $ ;
#define $$(...) {__VA_ARGS__;}

#define ASSERT(expr) ;
#endif // _DEBUG

#endif
