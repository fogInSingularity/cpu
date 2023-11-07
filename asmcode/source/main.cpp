#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <ctype.h>

#include "../include/assemble.h"

int main(int argc, char** argv) {
  AsmError error = AsmError::SUCCESS;
  Assembler assembler = {};

  error = assembler.Ctor(argc, argv);

  if (error == AsmError::SUCCESS) {
    error = assembler.Assemble();
  }

  assembler.ThrowError(error);
  assembler.Dtor(argc, argv);

  return 0;
}
