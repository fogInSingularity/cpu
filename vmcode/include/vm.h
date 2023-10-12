#ifndef VM_H
#define VM_H

#include <math.h>

#include "../../lib/include/binFileUtils.h"
#include "../../stack/include/stack.h"
#include "../../shared/include/commands.h"

typedef uint8_t code_t;
typedef int reg_t;
typedef int immed_t;

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
    Byte* pos;

    Cpu cpu;
};

void VMExecute(BinData* byteData, Stack* stk);

#endif
