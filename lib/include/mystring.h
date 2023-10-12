#ifndef MYSTRING_H
#define MYSTRING_H

#include <stdio.h>

/// @brief holds string
struct String {
    char* str;   ///< string intself
    ssize_t len; ///< len of string
};

#endif
