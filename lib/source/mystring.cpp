#include "../include/mystring.h"

char* Strchr(String* str, char ch) { //memchr
  ASSERT(str != nullptr);
  ASSERT(str->str != nullptr);

  char* move = str->str;
  while (move < str->str + str->len) {
    if (*move == ch) {
      return move;
    }

    move++;
  }

  return nullptr;
}

int Strcmp(String* strA, String* strB) {
  ASSERT(strA != nullptr);
  ASSERT(strA->str != nullptr);
  ASSERT(strB != nullptr);
  ASSERT(strB->str != nullptr);

  return strncmp(strA->str, strB->str, MIN(strA->len, strB->len));
}

ssize_t Fputs(String* str, FILE* file) {
  ASSERT(str != nullptr);
  ASSERT(str->str != nullptr);
  ASSERT(file != nullptr);

  char* move = str->str;

  while (move < str->str + str->len) {
    if (fputc(*move, file) == EOF) {
      return EOF;
    }

    move++;
  }

  return (ssize_t)str->len;
}
