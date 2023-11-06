#ifndef MYSTRING_H
#define MYSTRING_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "utils.h"
#include "debug.h"

/// @brief string type
struct String {
  char* str;   ///< string intself
  size_t len; ///< len of string
};

char* Strchr(String* str, char ch);

int Strcmp(String* strA, String* strB);

ssize_t Fputs(String* str, FILE* file);

// double Atof(String* str);

#endif
