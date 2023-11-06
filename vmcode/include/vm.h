#ifndef VM_H
#define VM_H

#include <math.h>

#include "../../lib/include/binFileUtils.h"
#include "../../stack/include/stack.h"
#include "../../shared/include/commands.h"
#include "dsl.h"

typedef double memory_t;

static const size_t NOfRegs = 4;
static const size_t MemorySize = 100;

enum class VMError {
  SUCCESS,
  HLT,
  NOT_ENOUGH_FILES,
  FILE_TO_READ_NOT_EXIST,
  SETUP_STACK_CANT_INIT,
  UNKNOWN_SIGNATURE,
  STACK_CANT_POP,
  STACK_CANT_PUSH,
};

struct Cpu {
  reg_t regs[NOfRegs];
  memory_t memory[MemorySize];

  Stack stk;
};

struct VM {
 public:
  BinData binData;
  byte_t* ip;

  Cpu cpu;

  VMError Ctor(int argc, char** argv);
  void Dtor();
  void ThrowError(VMError error);
  void Dump(const char* file,const size_t line, const char* func);

  VMError Execute();
 private:
  VMError ExecuteCmd();
};

#endif // VM_H
