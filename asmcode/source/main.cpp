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
  AsmError error = AsmError::SUCCESS;
  Assembler assembler = {};

  error = assembler.SetUp(argc, argv);

  if (error == AsmError::SUCCESS) {
    error = assembler.Assemble();
  }

  assembler.ThrowError(error);
  assembler.CleanUp(argc, argv);

  return 0;
}
