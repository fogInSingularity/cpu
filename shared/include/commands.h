#ifndef COMMANDS_H
#define COMMANDS_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "../../lib/include/debug.h"
#include "../../lib/include/fileUtils.h"
#include "../../stack/include/stack.h"

typedef uint8_t byte_t;
typedef uint8_t cmdKey_t;
typedef uint8_t regId_t;

typedef double immed_t;
typedef double jmpAdr_t;
typedef double arg_t;

typedef double reg_t;

enum Args {
  INIT_ARGS,
  NO_ARGS,
  ONLY_REG,
  ONLY_IMMED,
  REG_AND_IMMED,
};

enum Commands: cmdKey_t {
#define DEF_CMD(name, num, aldArgs, ...) \
  CMD_ ## name = num,

#include "../../shared/include/commandSet.h"

#undef DEF_CMD
};

enum Regs: regId_t {
  RAX = 1,
  RBX = 2,
  RCX = 3,
  RDX = 4,
};

__attribute_used__
static size_t RegIds[] = {
  1, 2, 3, 4
};

enum BitFlags: cmdKey_t {
  CODE_ID_MASK   = 0b000'11111,
  ARG_IMMED      = 0b001'00000,
  ARG_REG        = 0b010'00000,
  ARG_MEM_ACS    = 0b100'00000,
};

// MLRI
// ___
namespace AllowedArgs {
  enum Combos: uint64_t {
    ____ = 1 << 0,
    ___I = 1 << 1,
    __R_ = 1 << 2,
    __RI = 1 << 3,
    _L__ = 1 << 4,
    _L_I = 1 << 5,
    _LR_ = 1 << 6,
    _LRI = 1 << 7,
    M___ = 1 << 8,
    M__I = 1 << 9,
    M_R_ = 1 << 10,
    M_RI = 1 << 11,
    ML__ = 1 << 12,
    ML_I = 1 << 13,
    MLR_ = 1 << 14,
    MLRI = 1 << 15,
  };
}
#endif // COMMANDS_H
