#include <stdio.h>
#include <stdlib.h>

#include "../include/vm.h"

int main(int argc, char** argv) {
  VMError error = VMError::SUCCESS;

  VM vm = {};

  error = vm.SetUp(argc, argv);

  if (error == VMError::SUCCESS) {
    error = vm.Execute();
  }

  vm.ThrowError(error);
  vm.CleanUp();

  return 0;
}
