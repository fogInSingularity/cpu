#include "../include/assemble.h"

//TODO - label: jmp l want throw error

//static-----------------------------------------------------------------------

static size_t WalkthroughCnt = 0;
static const size_t MinBinBufAlloc = 64;
static const size_t MultConst = 2;
static FILE* ErrorStream = stdout;

static char* CntLen(String* str, char* move, String* iterOver);
static double IsNumberPassed(String str);
static Regs IsRegPassed(String str);
static char* IgnoreSpaces(char* move, String* strBound);

//TODO if that fits line should use {}

//public-----------------------------------------------------------------------

AsmError Assembler::Ctor(int argc, char** argv) {
  ASSERT(argv != nullptr);

  if (argc < 3) return AsmError::NOT_ENOUGH_FILES;

  FILE* scriptf = fopen(argv[1], "r");
  if (scriptf == nullptr) return AsmError::FILE_TO_READ_NOT_EXIST;

  GetData(&scriptFile, scriptf);
  if (scriptf != nullptr) { fclose(scriptf); }

  binbuf = {nullptr, 0, MinBinBufAlloc};
  binbuf.buf = (byte_t*)calloc(MinBinBufAlloc, sizeof(byte_t));
  if (binbuf.buf == nullptr) {
    return AsmError::SETUP_CANT_ALLOC;
  }

  DArrayError DAError = labelArr.Ctor(sizeof(Label));
  if (DAError != DArrayError::SUCCESS) {
    return AsmError::DARR_CTOR_CANT_ALLOC;
  }

  ErrorLine = nullptr;
  ErrorLineNumber = 0;
  signature = Signature;

  return AsmError::SUCCESS;
}

void Assembler::Dtor(int argc, char** argv) {
  ASSERT(argv != nullptr);

  USE_VAR(argc);

  if (argc >= 3) {
    FILE* byteFile = fopen(argv[2], "wb");
    if (binbuf.buf != nullptr) {
      fwrite(binbuf.buf, sizeof(char), binbuf.size, byteFile);
    }
    fclose(byteFile);
  }

  free(binbuf.buf);
  binbuf.buf = nullptr;
  binbuf.size = 0;
  binbuf.cap = 0;

  labelArr.Dtor();

  ErrorLine = nullptr;
  ErrorLineNumber = 0;

  FreeData(&scriptFile);
}

void Assembler::ThrowError(AsmError error) {
  switch (error) {
    case AsmError::SUCCESS:
      //---//
      break;

    case AsmError::NOT_ENOUGH_FILES:
      ERROR_M("too few files passed to program");
      break;
    case AsmError::FILE_TO_READ_NOT_EXIST:
      ERROR_M("cant open source file");
      break;
    case AsmError::SETUP_CANT_ALLOC:
      ERROR_M("unable to allocate memory for bytecode bufer");
      break;
    case AsmError::DARR_CTOR_CANT_ALLOC:
      ERROR_M("unable to allocate memory for darray");
      break;
    case AsmError::BINBUF_CANT_ALLOC:
      ERROR_M("unable to allocate memory for binbuf");
      break;

    case AsmError::STX_IDK_ARG:
      VERBOSE_ERROR_M("unknown argument passed:", ErrorLineNumber, ErrorLine);
      break;
    case AsmError::STX_IDK_REG:
      VERBOSE_ERROR_M("unknown regester passed:", ErrorLineNumber, ErrorLine);
      break;
    case AsmError::STX_IDK_CMD:
      VERBOSE_ERROR_M("unknown command passed:", ErrorLineNumber, ErrorLine);
      break;
    case AsmError::STX_IDK_LABEL:
      VERBOSE_ERROR_M("unknown label passed:", ErrorLineNumber, ErrorLine);
      break;
    case AsmError::STX_IDK_MEM_ACS:
      VERBOSE_ERROR_M("wrong memory access:", ErrorLineNumber, ErrorLine);
      break;
    default:
      ASSERT(0 && "UNKNOWN ERROR CODE");
      break;
  }
}

AsmError Assembler::Assemble() {
  AsmError error = AsmError::SUCCESS;

  // first walkthrough
  error = Walkthrough();
  if (error != AsmError::SUCCESS) return error;

  binbuf.size = 0;
  ErrorLineNumber = 0;

  // second walkthrough
  error = Walkthrough();
  if (error != AsmError::SUCCESS) return error;

  return error;
}

//private----------------------------------------------------------------------

AsmError Assembler::Walkthrough() {
  AsmError error = AsmError::SUCCESS;

  WalkthroughCnt++;
  error = StoreVoid(&signature, sizeof(signature));
  if (error != AsmError::SUCCESS) return error;

  String* sourceMove = scriptFile.text;
  while (sourceMove < scriptFile.text + scriptFile.nLines) {
    error = ParseAndStore(sourceMove);
    if (error != AsmError::SUCCESS) return error;

    sourceMove++;
  }

  return error;
}

AsmError Assembler::ParseAndStore(String* line) {
  ASSERT(line != nullptr);
  ASSERT(line->str != nullptr);

  ErrorLine = line;
  ErrorLineNumber++;

  char* colon = Strchr(line, ':');
  if (colon != nullptr) { // lable or fn call
    if (WalkthroughCnt == 1) {
      char* moveStr = IgnoreSpaces(line->str, line);
                                                         // size = смещение от начала
      Label lbl = {{moveStr, (size_t)(colon - moveStr)}, (jmpAdr_t)binbuf.size};
      labelArr.PushBack(&lbl);
    }
  } else {
    String cmd = {nullptr, 0};
    String arg = {nullptr, 0};

    char* moveStr = line->str;
    while(moveStr < line->str + line->len) {
      if (*moveStr == ';') {
        break;
      } else if (isspace(*moveStr)) {
        moveStr++;
      } else if (cmd.str == nullptr) {
        moveStr = CntLen(&cmd, moveStr, line);
      } else if (arg.str == nullptr) {
        moveStr = CntLen(&arg, moveStr, line);
      }
    }

    if (cmd.str == nullptr) return AsmError::SUCCESS;

    //convert
    AsmError error = AsmError::SUCCESS;

    #define DEF_CMD(name, cmdId, isJump, allowedArgs, ...)   \
      if (strncmp(cmd.str, #name, cmd.len) == 0) {           \
        if (isJump) {                                        \
          error = ParseJmp(&arg, cmdId, allowedArgs);        \
          if (error != AsmError::SUCCESS) {return error;}    \
        } else {                                             \
          error = ParseCmd(&arg, cmdId, allowedArgs);        \
          if (error != AsmError::SUCCESS) {return error;}    \
        }                                                    \
      } else
    /* end of defenition */

    #include "../../shared/include/commandSet.h"

    #undef DEF_CMP
    /* else */ {
      return AsmError::STX_IDK_CMD;
    };
  }

  return AsmError::SUCCESS;
}

AsmError Assembler::ParseJmp(String* arg, cmdKey_t cmdId, uint64_t allowedArgs) {
  ASSERT(arg != nullptr);

  USE_VAR(allowedArgs);

  if (Strchr(arg, '[') != nullptr) {
    return AsmError::STX_IDK_MEM_ACS;
  } else if (Strchr(arg, ']') != nullptr) {
    return AsmError::STX_IDK_MEM_ACS;
  }

  cmdKey_t cmdKey = 0 | cmdId | ARG_IMMED;
  jmpAdr_t jmpAdr = -1;

  if (WalkthroughCnt == 1) { //empty adr
    jmpAdr = 0;
  } else { // actual adr
    Label* ptr = nullptr;

    for (size_t i = 0; i < labelArr.size; i++) {
      labelArr.At((void**)&ptr, i);
      if (Strcmp((String*)ptr, arg) == 0) {
        jmpAdr = ptr->pos;

        break;
      }
    }

    // NOTE: add jmp 5    (global index)
    //    or add jmp 5/-5 (relative index)
    if (jmpAdr < 0) {
      return AsmError::STX_IDK_LABEL;
    }
  }

  return StoreJmp(cmdKey, jmpAdr);
}

AsmError Assembler::ParseCmd(String* strArg, cmdKey_t cmdId, uint64_t allowedArgs) {
  ASSERT(strArg != nullptr);

  if (strArg->str == nullptr) {
    return ParseCmdNoArgs(cmdId, allowedArgs);
  } else {
    return ParseCmdWithArgs(strArg, cmdId, allowedArgs);
  }
}

AsmError Assembler::ParseCmdNoArgs(cmdKey_t cmdId, uint64_t allowedArgs) {
  cmdKey_t cmdKey = 0 | cmdId;

  if ((AllowedArgs::____ & allowedArgs) == 0) {
    return AsmError::STX_IDK_ARG;
  }

  return StoreCmd(cmdKey, 0, 0);
}

AsmError Assembler::ParseCmdWithArgs(String* strArg, cmdKey_t cmdId, uint64_t allowedArgs) {
  ASSERT(strArg != nullptr);
  ASSERT(strArg->str != nullptr);

  String arg = {strArg->str, strArg->len};

  char* leftBracket  = Strchr(&arg, '[');
  char* rightBracket = Strchr(&arg, ']');
  char* plusSign     = Strchr(&arg, '+');
  uint8_t param = 0;

  cmdKey_t cmdKey = 0 | cmdId;

  if ((leftBracket == nullptr) && (rightBracket == nullptr)) {
    ;
  } else if ((leftBracket != nullptr) && (rightBracket != nullptr)) {
    param |= PassedArg::MEMORY_PASSED;
    cmdKey |= BitFlags::ARG_MEM_ACS;

    arg.len -= 2; // ignore []
    arg.str++;
  } else {
    return AsmError::STX_IDK_ARG;
  }

  Regs regId = Regs::UNKNOWN_REG;
  immed_t immed = NAN;

  if (plusSign != nullptr) {
    param |= PassedArg::IMMED_PASSED | PassedArg::REG_PASSED;
    cmdKey |= BitFlags::ARG_IMMED | BitFlags::ARG_REG;

    regId = IsRegPassed({arg.str, (size_t)(plusSign - arg.str)});
    immed = IsNumberPassed({plusSign + 1, arg.len - (size_t)(plusSign - arg.str) - 1});

    if ((regId == Regs::UNKNOWN_REG) || (isnan(immed))) {
      return AsmError::STX_IDK_ARG;
    }
  } else {
    immed = IsNumberPassed(arg);
    regId = IsRegPassed(arg);

    if (!isnan(immed) && (regId != Regs::UNKNOWN_REG)) {
      // immed and regId shouldt be passed at the same time
      // is it even possible
      return AsmError::STX_IDK_ARG;
    }

    if (!isnan(immed)) {
      param |= PassedArg::IMMED_PASSED;
      cmdKey |= BitFlags::ARG_IMMED;
    }
    if ((regId != Regs::UNKNOWN_REG)) {
      param |= PassedArg::REG_PASSED;
      cmdKey |= BitFlags::ARG_REG;
    }
  }

  if (((1UL << param) & allowedArgs) == 0) {
    return AsmError::STX_IDK_ARG;
  }

  return StoreCmd(cmdKey, (regId_t)regId, immed);
}

AsmError Assembler::StoreJmp(cmdKey_t cmdKey, jmpAdr_t jmpAdr) {
  AsmError error = AsmError::SUCCESS;

  error = StoreVoid(&cmdKey, sizeof(cmdKey));
  if (error != AsmError::SUCCESS) return error;

  error = StoreVoid(&jmpAdr, sizeof(jmpAdr));
  if (error != AsmError::SUCCESS) return error;

  return error;
}

AsmError Assembler::StoreCmd(cmdKey_t cmdKey, regId_t regId, immed_t immed) {
  AsmError error = AsmError::SUCCESS;

  error = StoreVoid(&cmdKey, sizeof(cmdKey));
  if (error != AsmError::SUCCESS) return error;

  if (cmdKey & BitFlags::ARG_REG) {
    error = StoreVoid(&regId, sizeof(regId));
    if (error != AsmError::SUCCESS) return error;
  }

  if (cmdKey & BitFlags::ARG_IMMED) {
    error = StoreVoid(&immed, sizeof(immed));
    if (error != AsmError::SUCCESS) return error;
  }

  return error;
}

AsmError Assembler::StoreVoid(void* elem, size_t elemSize) {
  ASSERT(elem != nullptr);

  if (binbuf.cap - binbuf.size < elemSize) {
    AsmError error = Recalloc();
    if (error != AsmError::SUCCESS) return error;
  }

  memcpy(binbuf.buf + binbuf.size, elem, elemSize);
  binbuf.size += elemSize;

  return AsmError::SUCCESS;
}

AsmError Assembler::Recalloc() {
  binbuf.cap *= MultConst;
  byte_t* holdBuf = binbuf.buf;

  binbuf.buf = (byte_t*)realloc(binbuf.buf, binbuf.cap);
  if (binbuf.buf == nullptr) {
    binbuf.cap /= MultConst;
    binbuf.buf = holdBuf;

    return AsmError::BINBUF_CANT_ALLOC;
  }

  return AsmError::SUCCESS;
}

//static-----------------------------------------------------------------------

static char* CntLen(String* str, char* move, String* iterOver) {
  ASSERT(str != nullptr);
  ASSERT(move != nullptr);
  ASSERT(iterOver != nullptr);

  str->str = move;

  while (!isspace(*move) && (move < iterOver->str + iterOver->len) && (*move != ';')) {
    str->len++;

    move++;
  }

  return move;
}

static double IsNumberPassed(String str) {
  ASSERT(str.str != nullptr);
  // если в строке нашлась не цифра или больше 1 точки то это NAN
  size_t cntDots = 0;
  size_t cntSign = 0;
  bool flag = true;

  char* moveStr = str.str;
  while (moveStr < str.str + str.len) {
    if (isdigit(*moveStr)) {
      ;
    } else if (*moveStr == '.') {
      if (cntDots >= 1) {
        flag = false;
      } else {
        cntDots++;
      }
    } else if ((*moveStr == '-') || (*moveStr == '+')) {
      if (cntSign >= 1) {
        flag = false;
      } else {
        cntSign++;
      }
    } else {
      flag = false;
    }

    moveStr++;
  }

  if (flag == false) {
    return NAN;
  } else {
    return atof(str.str);
  }
}

static Regs IsRegPassed(String str) {
  ASSERT(str.str != nullptr);

  static const size_t kRegLen = 3;

  //логика функции строиться на том что мы понимаем всё о регистрах
  if (str.len != kRegLen) {
    return Regs::UNKNOWN_REG;
  }

  if ((str.str[0] != 'r') || (str.str[2] != 'x')) {
    return Regs::UNKNOWN_REG;
  }

  regId_t regKey = (regId_t)(str.str[1] - 'a' + 1);
  switch ((Regs)regKey) { //NOTE
    case Regs::RAX:
      return Regs::RAX;
      break;
    case Regs::RBX:
      return Regs::RBX;
      break;
    case Regs::RCX:
      return Regs::RCX;
      break;
    case Regs::RDX:
      return Regs::RDX;
      break;
    case Regs::UNKNOWN_REG:
    default:
      return Regs::UNKNOWN_REG;
      break;
  }
}

static char* IgnoreSpaces(char* move, String* strBound) {
  while (isspace(*move) && (move < strBound->str + strBound->len)) {
    move++;
  }

  return move;
}
