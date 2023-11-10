#include <stdio.h>
#include <stdlib.h>

#include "vm.h"

int main(int argc, char** argv) {
  VMError error = VMError::SUCCESS;

  VM vm = {};

  error = vm.Ctor(argc, argv);

  if (error == VMError::SUCCESS) {
    error = vm.Execute();
  }

  vm.ThrowError(error);
  vm.Dtor();

  return 0;
}
