#ifndef MYSTRING_H
#define MYSTRING_H

#include <stdio.h>
#include <string.h>

/// @brief string type
struct String {
  char* str;   ///< string intself
  size_t len; ///< len of string
};

char* Strchr(String* str, char ch);

int Strcmp(String* strA, String* strB);

#endif
