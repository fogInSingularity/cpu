#ifndef VM_H
#define VM_H

#include <math.h>

#include "../../lib/include/binFileUtils.h"
#include "../../stack/include/stack.h"
#include "../../shared/include/commands.h"

typedef double memory_t;

enum class VMError {
  SUCCESS,
  NOT_ENOUGH_FILES,
  FILE_TO_READ_NOT_EXIST,
};

struct Cpu {
  reg_t regs[4];
  memory_t memory[100];

  Stack* stk;
};

struct VM {
 public:
  BinData* binData;
  byte_t* ip;

  Cpu cpu;

  VMError SetUp(int argc, char** argv);
  void CleanUp(int argc, char** argv);
  void ThrowError(VMError error);

  VMError Execute();
 private:

};

// void VMExecute(BinData* byteData, Stack* stk);

#endif
