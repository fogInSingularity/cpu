#ifndef VM_H
#define VM_H

#include <math.h>

#include "../../lib/include/binFileUtils.h"
#include "../../stack/include/stack.h"
#include "../../shared/include/commands.h"

struct Regesters {
  reg_t rax;
  reg_t rbx;
  reg_t rcx;
  reg_t rdx;
};

struct Cpu {
  Regesters regs;
  Stack* stk;
};

struct VM {
  BinData* binData;
  byte_t* pos;

  Cpu cpu;
};

void VMExecute(BinData* byteData, Stack* stk);

#endif
