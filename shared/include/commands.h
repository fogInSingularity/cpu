#ifndef COMMANDS_H
#define COMMANDS_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "../../lib/include/debug.h"
#include "../../lib/include/fileUtils.h"



static const size_t CommandCount = 12;

enum Commands: uint8_t {
    HLT  = 0,
    OUT  = 1,
    PUSH = 2,
    POP  = 3,
    IN   = 4,
    ADD  = 5,
    SUB  = 6,
    MULT = 7,
    DIV  = 8,
    SQRT = 9,
    COS  = 10,
    SIN  = 11,
};

__attribute_used__
static const char* CommandNames[] = {
    "HLT",
    "out",
    "push",
    "pop",
    "in",
    "add",
    "sub",
    "mult",
    "div",
    "sqrt",
    "cos",
    "sin",
};

static const int CommandIds[] = {
    0,
    1,
    2,
    3,
    4,
    5,
    6,
    7,
    8,
    9,
    10,
    11,
};



enum Regs {
    RAX = 1,
    RBX = 2,
    RCX = 3,
    RDX = 4,
};

static const size_t RegCount = 4;

__attribute_used__
static const char* RegNames[] = {
    "rax",
    "rbx",
    "rcx",
    "rdx",
};

static const int RegIds[] = {
    1,
    2,
    3,
    4,
};



enum BitFlags {
    ARG_FORMAT_IMMED = 0b0001'0000,
    ARG_FORMAT_REG   = 0b0010'0000,
    CODE_ID_MASK     = 0b0000'1111,
};

#endif // COMMANDS_H
