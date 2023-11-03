#include "../include/vm.h"

//TODO - if corrupt file it will segfall
//TODO - dsl

//static-----------------------------------------------------------------------

static FILE* ErrorStream = stderr;
static FILE* InStream = stdin;
static FILE* OutStream = stdout;

//public-----------------------------------------------------------------------

VMError VM::SetUp(int argc, char** argv) {
  ASSERT(argv != nullptr);

  if (argc < 2) return VMError::NOT_ENOUGH_FILES;

  FILE* bytef = fopen(argv[1], "r");
  if (bytef == nullptr) return VMError::FILE_TO_READ_NOT_EXIST;

  GetData(&binData, bytef);
  fclose(bytef);
  ip = binData.buf;

  if (*(uint32_t*)ip != Signature) return VMError::UNKNOWN_SIGNATURE;
  ip += sizeof(Signature);

  if (StackCtor(&cpu.stk) != StackState::Success) {
    return VMError::SETUP_STACK_CANT_INIT;
  }

  return VMError::SUCCESS;
}

void VM::CleanUp() {
  StackDtor(&cpu.stk);

  FreeData(&binData);
  ip = nullptr;
}

void VM::ThrowError(VMError error) {
  switch (error) {
    case VMError::SUCCESS:
    case VMError::HLT:
      //---//
      break;
    case VMError::NOT_ENOUGH_FILES:
      fputs(ERROR_M " too few files passed to program\n", ErrorStream);
      break;
    case VMError::FILE_TO_READ_NOT_EXIST:
      fputs(ERROR_M " cant open source file\n", ErrorStream);
      break;
    case VMError::SETUP_STACK_CANT_INIT:
      fputs(ERROR_M " unable to allocate memory for stack\n", ErrorStream);
      break;
    case VMError::UNKNOWN_SIGNATURE:
      fputs(ERROR_M " unknown signature\n", ErrorStream);
      break;
    default:
      ASSERT(0 && "UNKNOWN ERROR CODE");
      break;
  }
}

VMError VM::Execute() {
  while (ip < binData.buf + binData.bufSz) {
    VMError error = ExecuteCmd();
    if (error != VMError::SUCCESS) {
      return error;
    }
  }

  return VMError::SUCCESS;
}

//private----------------------------------------------------------------------

VMError VM::ExecuteCmd() {
  cmdKey_t cmdKey = *ip;
  ip += sizeof(cmdKey);

  regId_t regId = 0;
  arg_t arg = NAN;

  if (cmdKey & BitFlags::ARG_REG) {
    regId = *ip;
    ip += sizeof(regId);
  }

  if (cmdKey & BitFlags::ARG_IMMED) {
    memcpy(&arg, ip, sizeof(arg));
    ip += sizeof(arg);
  }

  // ON_DEBUG(fprintf(ErrorStream, ">>> %u %u %f\n", cmdKey, regId, arg);)

/* start of def */
#define DEF_CMD(name, num, isJmp, aldArgs, ...) \
  case num: \
    {} \
    break;
/* end of def */

  switch (cmdKey & BitFlags::CODE_ID_MASK) {
    #include "../../shared/include/commandSet.h"

    default:
      ASSERT(0 && "UNKNOWN COMMAND KEY");
      break;
  }

#undef DEF_CMD

  return VMError::SUCCESS;
}

//static-----------------------------------------------------------------------
