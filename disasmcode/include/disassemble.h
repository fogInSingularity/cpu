#ifndef DISASSEMBLE_H
#define DISASSEMBLE_H

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <memory.h>

#include "binFileUtils.h"
#include "commands.h"
#include "errorMes.h"
#include "color.h"
#include "utils.h"
#include "darray.h"
#include "lsearch.h"

enum class DisError {
  SUCCESS,

  NOT_ENOUGH_FILES,
  FILE_READ_NOT_EXIST,
  SETUP_CANT_ALLOC,
  STRBUF_CANT_REALLOC,
  UNKNOWN_SIGNATURE,
  DARR_CTOR_CANT_ALLOC,
  UNKNOWN_LABEL,
};

struct Strbuf {
  char* buf;
  size_t size;
  size_t cap;
};

struct Label {
  size_t labelId;
  // size_t line;
  size_t byteShift;
};

struct DisAssembler {
 public:
  BinData binData_;
  DArray labelArr_;
  Strbuf reversed_;
  byte_t* ip_;
  size_t curLine_;
  size_t lastLabel_;
  size_t walkthroughCnt_;

  DisError Ctor(int argc, char** argv);
  void Dtor(int argc, char** argv);
  void ThrowError(DisError error);

  DisError DisAssemble();
 private:
  DisError Reverse();
  DisError ReverseJmp(String* cmdName, arg_t arg);
  DisError ReverseCmd(String* cmdName, regId_t regId, arg_t arg);
  DisError WriteLabel(Label* lbl);
  DisError Walkthrough();
  DisError AppendToStr(String* str);
  DisError Recalloc(size_t size);
};

#endif // DISASSEMBLE_H
