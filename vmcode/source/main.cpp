#include <stdio.h>
#include <stdlib.h>

#include "../../lib/include/binFileUtils.h"
#include "../../stack/include/stack.h"
#include "../include/vm.h"

int main(int argc, char** argv) {
  if (argc < 2) {
    fprintf(stderr, "!  not enough files\n");
    return 0;
  }

  FILE* byteCodeFile = fopen(argv[1], "rb");
  if (byteCodeFile == nullptr) {
    fprintf(stderr, "!  cant open file to read: %s\n", argv[1]);
    return 0;
  }

  BinData byteData = {nullptr, 0};
  GetData(&byteData, byteCodeFile);
  fclose(byteCodeFile);

  Stack stk = {};
  StackCtor(&stk);

  VMExecute(&byteData, &stk);

  StackDtor(&stk);
  FreeData(&byteData);

  return 0;
}
