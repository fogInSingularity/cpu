#ifndef ERROR_M_H
#define ERROR_M_H

#include <stdio.h>

#define ERROR_M(errorM)                                                       \
  fputs(RED BOLD "error: " RESET errorM "\n", stdout);

#define VERBOSE_ERROR_M(errorM, lineN, errorLine)                             \
  ERROR_M(errorM);                                                            \
  fprintf(stdout, "%lu:", lineN);                                             \
  Fputs(errorLine, stdout);                                                   \
  fputc('\n', stdout);

#endif // ERROR_M_H
