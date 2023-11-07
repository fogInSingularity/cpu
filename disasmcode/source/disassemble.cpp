#include "../include/disassemble.h"

//static-----------------------------------------------------------------------

static const size_t kMinRevBufAlloc = 64;
static const size_t kMultRev = 2;

//public-----------------------------------------------------------------------

DisError DisAssembler::Ctor(int argc, char** argv) {
  ASSERT(argv != nullptr);

  if (argc < 3) { return DisError::NOT_ENOUGH_FILES; }

  binData_ = {nullptr, 0};
  FILE* binf = fopen(argv[1], "r");
  if (binf == nullptr) { return DisError::FILE_READ_NOT_EXIST; }

  GetData(&binData_, binf);
  if (binf != nullptr) { fclose(binf); }

  reversed_ = {nullptr, 0, 0};
  reversed_.buf = (char*)calloc(kMinRevBufAlloc, sizeof(char));
  if (reversed_.buf == nullptr) {
    return DisError::SETUP_CANT_ALLOC;
  }
  reversed_.cap = kMinRevBufAlloc;

  return DisError::SUCCESS;
}

void DisAssembler::Dtor(int argc, char** argv) {
  if (argc >= 3) {
    FILE* asmf = fopen(argv[2], "w");
    fwrite(reversed_.buf, reversed_.size, sizeof(char), asmf);
  }

  free(binData_.buf);
  binData_.bufSz = 0;

  free(reversed_.buf);
  reversed_.buf = nullptr;
  reversed_.size = 0;
  reversed_.cap = 0;
}

void DisAssembler::ThrowError(DisError error) {
  switch (error) {
    case DisError::SUCCESS:
      //---//
      break;
    case DisError::NOT_ENOUGH_FILES:
      ERROR_M("too few files passed to program");
      break;
    case DisError::FILE_READ_NOT_EXIST:
      ERROR_M("cant open binary file");
      break;
    case DisError::SETUP_CANT_ALLOC:
      ERROR_M("unable to allocate memory for disassembled code bufer");
      break;
    default:
      ASSERT(0 && "UNKNOWN ERROR CODE");
      break;
  }
}

DisError DisAssembler::DisAssemble() {

  return DisError::SUCCESS;
}

//private----------------------------------------------------------------------

//static-----------------------------------------------------------------------
