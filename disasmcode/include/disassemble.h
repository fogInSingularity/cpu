#ifndef DISASSEMBLE_H
#define DISASSEMBLE_H

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <memory.h>

#include "../../lib/include/binFileUtils.h"
#include "../../shared/include/commands.h"
#include "../../shared/include/errorMes.h"
#include "../../lib/include/color.h"
#include "../../lib/include/utils.h"

enum class DisError {
  SUCCESS,

  NOT_ENOUGH_FILES,
  FILE_READ_NOT_EXIST,
  SETUP_CANT_ALLOC,
};

struct strbuf {
  char* buf;
  size_t size;
  size_t cap;
};

struct DisAssembler {
 public:
  BinData binData_;
  strbuf reversed_;

  DisError Ctor(int argc, char** argv);
  void Dtor(int argc, char** argv);
  void ThrowError(DisError error);

  DisError DisAssemble();
 private:
};

#endif // DISASSEMBLE_H
