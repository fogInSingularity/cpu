#ifndef ASSEMBLE_H
#define ASSEMBLE_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "../../darray/include/darray.h"
#include "../../lib/include/fileUtils.h"
#include "../../shared/include/commands.h"
#include "../../lib/include/color.h"
#include "../../lib/include/utils.h"
#include "../../stack/include/stack.h"

struct Label {
  String name;
  jmpAdr_t pos;
};

struct BinBuf {
  byte_t* buf;
  size_t size;
  size_t cap;
};

namespace PassedArg {
  enum PassArg: uint8_t {
    // MLRI
    IMMED_PASSED  = 0b0001,
    REG_PASSED    = 0b0010,
    LABEL_PASSED  = 0b0100,
    MEMORY_PASSED = 0b1000,
  };
}

enum class AsmError {
  SUCCESS,

  NOT_ENOUGH_FILES,
  FILE_TO_READ_NOT_EXIST,
  SETUP_CANT_ALLOC,
  DARR_CTOR_CANT_ALLOC,
  BINBUF_CANT_ALLOC,

  STX_IDK_ARG,
  STX_IDK_REG,
  STX_IDK_CMD,
  STX_IDK_LABEL,
  STX_IDK_MEM_ACS,
};

struct Assembler {
 public:
  BinBuf binbuf;
  DArray labelArr;
  FileData scriptFile;

  AsmError Ctor(int argc, char** argv);
  void Dtor(int argc, char** argv);
  void ThrowError(AsmError error);

  AsmError Assemble();
 private:
  String* ErrorLine;
  size_t ErrorLineNumber;
  uint32_t signature;

  AsmError Walkthrough();
  AsmError ParseAndStore(String* line);
  AsmError ParseJmp(String* arg, cmdKey_t cmdIdId, uint64_t allowedArgs);
  AsmError StoreJmp(cmdKey_t cmdKey, jmpAdr_t jmpAdr);
  AsmError ParseCmd(String* strArg, cmdKey_t cmdIdId, uint64_t allowedArgs);
  AsmError ParseCmdNoArgs(cmdKey_t cmdIdId, uint64_t allowedArgs);
  AsmError ParseCmdWithArgs(String* arg, cmdKey_t cmdIdId, uint64_t allowedArgs);
  AsmError StoreCmd(cmdKey_t cmdKey, regId_t regId, immed_t immed);
  AsmError StoreVoid(void* elem, size_t elemSize);
  AsmError Recalloc();
};

#endif // ASSEMBLE_H
