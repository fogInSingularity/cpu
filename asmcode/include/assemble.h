#ifndef ASSEMBLE_H
#define ASSEMBLE_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "fileUtils.h"
#include "binFileUtils.h"
#include "color.h"
#include "utils.h"
#include "darray.h"
#include "hash.h"
#include "qsort.h"
#include "bsearch.h"
#include "commands.h"
#include "errorMes.h"

struct Label {        // REVIEW hash collision
  uint32_t labelKey;
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
  SUCCESS                = 0,

  NOT_ENOUGH_FILES       = 1,
  FILE_TO_READ_NOT_EXIST = 2,
  SETUP_CANT_ALLOC       = 3,
  DARR_CTOR_CANT_ALLOC   = 4,
  BINBUF_CANT_ALLOC      = 5,

  STX_IDK_ARG            = 6,
  STX_IDK_REG            = 7,
  STX_IDK_CMD            = 8,
  STX_IDK_LABEL          = 9,
  STX_IDK_MEM_ACS        = 10,
};

struct Assembler {
 public:
  BinBuf binbuf_;
  DArray labelArr_;
  FileData scriptFile_;

  AsmError Ctor(int argc, char** argv);
  void Dtor(int argc, char** argv);
  void ThrowError(AsmError error);

  AsmError Assemble();
 private:
  String* errorLine_;
  size_t errorLineNumber_;
  uint32_t signature_;

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
