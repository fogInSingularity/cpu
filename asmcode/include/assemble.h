#ifndef ASSEMBLE_H
#define ASSEMBLE_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "../../darray/include/darray.h"
#include "../../lib/include/fileUtils.h"
#include "../../shared/include/commands.h"


struct Label {
  String name;
  jmpAdr_t pos;
};

struct BinBuf {
  byte_t* buf;
  size_t size;
  size_t cap;
};


struct Assembler {
  BinBuf binbuf;
  DArray labelArr;

  FileData scriptFile;
  FILE* byteFile;
};


void Assemble(FileData* input, FILE* output);

void DumpLabel(void* label);

#endif // ASSEMBLE_H
