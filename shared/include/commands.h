#ifndef COMMANDS_H
#define COMMANDS_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "debug.h"
#include "fileUtils.h"
#include "stack.h"

typedef uint8_t byte_t;
typedef uint8_t cmdKey_t;
typedef uint8_t regId_t;

typedef double immed_t;
typedef double jmpAdr_t;
typedef double arg_t;
typedef double reg_t;

enum class Args {
  INIT_ARGS     = 0,
  NO_ARGS       = 1,
  ONLY_REG      = 2,
  ONLY_IMMED    = 3,
  REG_AND_IMMED = 4,
};

// DEADFA11 stupid little endian
const uint32_t Signature = 0x11FAADDE;

enum class Regs: regId_t {
  UNKNOWN_REG = 0,
  RAX         = 1,
  RBX         = 2,
  RCX         = 3,
  RDX         = 4,
};

enum BitFlags: cmdKey_t {
  CODE_ID_MASK   = 0b000'11111,
  ARG_IMMED      = 0b001'00000,
  ARG_REG        = 0b010'00000,
  ARG_MEM_ACS    = 0b100'00000,
};

namespace AllowedArgs {
  // MLRI
  // ___
  enum Combos: uint64_t {
    ____ = 1 << 0,
    ___I = 1 << 1,
    __R_ = 1 << 2,
    __RI = 1 << 3,
    _L__ = 1 << 4,
    _L_I = 1 << 5, // unused
    _LR_ = 1 << 6, // unused
    _LRI = 1 << 7, // unused
    M___ = 1 << 8, // unused
    M__I = 1 << 9,
    M_R_ = 1 << 10,
    M_RI = 1 << 11,
    ML__ = 1 << 12, // unused
    ML_I = 1 << 13, // unused
    MLR_ = 1 << 14, // unused
    MLRI = 1 << 15, // unused
  };
}
#endif // COMMANDS_H
