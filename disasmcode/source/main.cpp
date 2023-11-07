#include <stdio.h>

#include "../include/disassemble.h"

int main(int argc, char** argv) {
  DisError error = DisError::SUCCESS;
  DisAssembler disassembler = {};

  error = disassembler.Ctor(argc, argv);

  if (error == DisError::SUCCESS) {
    error = disassembler.DisAssemble();
  }
  
  disassembler.ThrowError(error);
  disassembler.Dtor(argc, argv);
}
