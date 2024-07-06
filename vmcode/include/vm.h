#ifndef VM_H
#define VM_H

#include <math.h>

#include "binFileUtils.h"
#include "stack.h"
#include "commands.h"
#include "errorMes.h"
#include "dsl.h"

typedef double memory_t;

static const size_t NOfRegs = 4;
static const size_t MemorySize = 100;

enum class VMError {
  SUCCESS                = 0,
  HLT                    = 1,
  NOT_ENOUGH_FILES       = 2,
  FILE_TO_READ_NOT_EXIST = 3,
  SETUP_STACK_CANT_INIT  = 4,
  UNKNOWN_SIGNATURE      = 5,
  STACK_CANT_POP         = 6,
  STACK_CANT_PUSH        = 7,
  UNKNOWN_COMMAND_KEY    = 8,
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
  void MemDump();

  VMError Execute();
 private:
  VMError ExecuteCmd();
};

#endif // VM_H
