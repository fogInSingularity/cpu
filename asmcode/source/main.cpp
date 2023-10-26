#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <ctype.h>

#include "../include/assemble.h"
#include "../../lib/include/fileUtils.h"
#include "../../lib/include/debug.h"
#include "../include/assemble.h"
#include "../../darray/include/darray.h"

int main(int argc, char** argv) {
  if (argc < 3) {
    fprintf(stderr, "!  not enough files\n");
    return 0;
  }

  FILE* scriptFile = fopen(argv[1], "r");
  CHECK_FILE(scriptFile, argv[1], 0);

  FILE* byteCodeFile = fopen(argv[2], "wb");
  CHECK_FILE(byteCodeFile, argv[2], 0); //NOTE - creates file insted failing

  FileData scriptData = {};
  GetData(&scriptData, scriptFile);
  fclose(scriptFile);

  Assemble(&scriptData, byteCodeFile);

  fclose(byteCodeFile);
  FreeData(&scriptData);

  return 0;
}
