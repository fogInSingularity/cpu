#include "disassemble.h"

//static-----------------------------------------------------------------------

static const size_t kMinRevBufAlloc = 64;
static const size_t kMultRev = 2;
static const size_t kMultConst = 2;

static int CmpLabels(const void* a, const void* b);
// static String* StrConcat(String* strA, String* strB, String* newStr);

//public-----------------------------------------------------------------------

DisError DisAssembler::Ctor(int argc, char** argv) {
  ASSERT(argv != nullptr);

  if (argc < 3) { return DisError::NOT_ENOUGH_FILES; }

  FILE* binf = fopen(argv[1], "r");
  if (binf == nullptr) { return DisError::FILE_READ_NOT_EXIST; }

  binData_ = {nullptr, 0};
  GetData(&binData_, binf);
  fclose(binf);

  DArrayError DAerror =  labelArr_.Ctor(sizeof(Label));
  if (DAerror != DArrayError::SUCCESS) {
    return DisError::DARR_CTOR_CANT_ALLOC;
  }

  reversed_ = {nullptr, 0, 0};
  reversed_.buf = (char*)calloc(kMinRevBufAlloc, sizeof(char));
  if (reversed_.buf == nullptr) {
    return DisError::SETUP_CANT_ALLOC;
  }
  reversed_.cap = kMinRevBufAlloc;


  if (*(uint32_t*)binData_.buf != Signature) { return DisError::UNKNOWN_SIGNATURE; }
  binData_.buf +=  sizeof(Signature);
  binData_.bufSz -= sizeof(Signature);
  ip_ = binData_.buf;

  curLine_ = 0;
  lastLabel_ = 0;
  walkthroughCnt_ = 0;

  return DisError::SUCCESS;
}

void DisAssembler::Dtor(int argc, char** argv) {
  if (argc >= 3) {
    FILE* asmf = fopen(argv[2], "w");
    fwrite(reversed_.buf, reversed_.size, sizeof(char), asmf);
  }
  free(reversed_.buf);
  reversed_ = {nullptr, 0, 0};

  if (binData_.buf != nullptr) {
    FreeData(&binData_);
  }

  labelArr_.Dtor();

  ip_ = nullptr;
  curLine_ = 0;
  lastLabel_ = 0;
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
      ERROR_M("unable to allocate memory for disassembled code buffer");
      break;
    case DisError::STRBUF_CANT_REALLOC:
      ERROR_M("unable to reallocate memory for disassembled code bufer");
      break;
    case DisError::UNKNOWN_SIGNATURE:
      ERROR_M("unknown signature");
      break;
    case DisError::DARR_CTOR_CANT_ALLOC:
      ERROR_M("unable to allocate memory for darray");
      break;
    default:
      ASSERT(0 && "UNKNOWN ERROR CODE");
      break;
  }
}

DisError DisAssembler::DisAssemble() {
  DisError error = DisError::SUCCESS;

  error = Walkthrough(); // first walkthrough
  if (error != DisError::SUCCESS) { return error; }

  ip_ = binData_.buf;
  curLine_ = 0;

  error = Walkthrough(); //second walkthrough
  if (error != DisError::SUCCESS) { return error; }

  return DisError::SUCCESS;
}

//private----------------------------------------------------------------------
DisError DisAssembler::Walkthrough() {
  walkthroughCnt_++;
  while (ip_ < binData_.buf + binData_.bufSz) {
    curLine_++;
    DisError error = Reverse();
    if (error != DisError::SUCCESS) { return error; }
  }

  return DisError::SUCCESS;
}

DisError DisAssembler::Reverse() {
  cmdKey_t cmdKey = *ip_;
  ip_ += sizeof(cmdKey);

  regId_t regId = 0;
  arg_t arg = 0;

  if (cmdKey & BitFlags::ARG_REG) {
    regId = *ip_;
    ip_ += sizeof(regId);
  }

  if (cmdKey & BitFlags::ARG_IMMED) {
    arg = *(arg_t*)ip_;
    ip_ += sizeof(arg);
  }

  //FIXME
  /* start if def */
  #define DEF_CMD(name, cmdId, isJmp, allowedArgs, ...)                       \
    case cmdId:                                                               \
      {                                                                       \
        cmdName = {#name, sizeof(#name)-1};                                     \
        if (isJmp) {                                                          \
          ReverseJmp(&cmdName);                                               \
        } else {                                                              \
          ReverseCmd(&cmdName)                                                \
        }                                                                     \
      }                                                                       \
      break;
  /* end of def */

  String cmdName = {nullptr, 0};
  switch (cmdKey & BitFlags::CODE_ID_MASK) {
    #include "commandSet.h"

    default:
      //FIXME return error, shouldnt assert
      ASSERT(0 && "UNKNOWN COMMAND KEY");
      break;
  }

  if (walkthroughCnt_ == 2) {
    // Label lbl = {0, 0, 0};
    Label lbl = {0, ip_ - binData_.buf};
    //FIXME use qsort and bsearch
    Label* lblPtr = (Label*)LSearch(&lbl, labelArr_.array, labelArr_.size,
                                    labelArr_.elemSize, &CmpLabels);
    if (lblPtr != nullptr) {
      WriteLabel(lblPtr);
    }
  }
}

DisError DisAssembler::ReverseJmp(String* cmdName, arg_t arg) {
  // Label lbl = {0, 0, arg};
  Label lbl = {0, arg};
  Label* lblPtr = (Label*)LSearch(&lbl, labelArr_.array, labelArr_.size,
                                  labelArr_.elemSize, &CmpLabels);
  if (walkthroughCnt_ == 1) {
    if (lblPtr == nullptr) {
      lbl.labelId = ++lastLabel_;
      labelArr_.PushBack(&lbl);
    }
  } else {
    if (lblPtr == nullptr) { return DisError::UNKNOWN_LABEL; }
    WriteJmp(cmdName, lblPtr);
  }
}

DisError DisAssembler::ReverseCmd(String* cmdName, regId_t regId, arg_t arg) {
  ASSERT(cmdName != nullptr);
  ASSERT(cmdName->str != nullptr);

  //if walkthrough == 1 then we shoudnt do anything
  if (walkthroughCnt_ == 2) {

  }
}

DisError DisAssembler::WriteLabel(Label* lbl) {
  ASSERT(lbl != nullptr);

  String lblName  = {nullptr, 0};
  ConvertLabel(lbl, &lblName);

  AppendToStr();
}

DisError DisAssembler::AppendToStr(String* str) {
  ASSERT(str != nullptr);

  if (reversed_.cap - reversed_.size < str->len) {
    DisError error = Recalloc(str->len);
    if (error != DisError::SUCCESS) { return error; }
  }

  memcpy(reversed_.buf, str->str, str->len);

  return DisError::SUCCESS;
}

//NOTE: what faster (rare int divs) or (extra mult most of the time)?
DisError DisAssembler::Recalloc(size_t size) {
  char* hold = reversed_.buf;
  reversed_.cap *= kMultConst;

  reversed_.buf = (char*)realloc(reversed_.buf,  reversed_.cap);
  if (reversed_.buf == nullptr) {
    reversed_.buf = hold;
    reversed_.cap /= kMultConst;

    return DisError::STRBUF_CANT_REALLOC;
  }

  memset(reversed_.buf + reversed_.size, 0, reversed_.cap - reversed_.size);

  return DisError::SUCCESS;
}
//static-----------------------------------------------------------------------

static int CmpLabels(const void* a, const void* b) {
  const Label* lblA = (const Label*)a;
  const Label* lblB = (const Label*)b;

  return (lblA->byteShift == lblB->byteShift) ? 0 : 1;
}

static void ConvertLabel(Label* lbl, String* lblName) {
  
}

// // strA + strB
// static String* StrConcat(String* strA, String* strB, String* newStr) {
//   ASSERT(strA != nullptr);
//   ASSERT(strB != nullptr);
//   ASSERT(newStr != nullptr);

//   *newStr = {nullptr, 0};

//   newStr->len = strA->len + strB->len;
//   newStr->str = (char*)calloc(newStr->len, sizeof(char));
//   if (newStr->str == nullptr) {
//     newStr->len = 0;
//   }

//   return newStr;
// }
