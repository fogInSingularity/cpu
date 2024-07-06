#include "vm.h"

//TODO - if corrupt file it will segfault

//static-----------------------------------------------------------------------

static FILE* ErrorStream = stderr;
static FILE* InStream = stdin;
static FILE* OutStream = stdout;

//public-----------------------------------------------------------------------

VMError VM::Ctor(int argc, char** argv) {
  ASSERT(argv != nullptr);

  if (argc < 2) { return VMError::NOT_ENOUGH_FILES; }

  FILE* bytef = fopen(argv[1], "r");
  if (bytef == nullptr) { return VMError::FILE_TO_READ_NOT_EXIST; }

  GetData(&binData, bytef);
  fclose(bytef);
  ip = binData.buf;

  if (*(uint32_t*)ip != Signature) { return VMError::UNKNOWN_SIGNATURE; }
  ip += sizeof(Signature);

  if (StackCtor(&cpu.stk) != StackState::Success) {
    return VMError::SETUP_STACK_CANT_INIT;
  }

  return VMError::SUCCESS;
}

void VM::Dtor() {
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
      ERROR_M("too few files passed to program");
      break;
    case VMError::FILE_TO_READ_NOT_EXIST:
      ERROR_M("cant open source file");
      break;
    case VMError::SETUP_STACK_CANT_INIT:
      ERROR_M("unable to allocate memory for stack");
      break;
    case VMError::UNKNOWN_SIGNATURE:
      ERROR_M("unknown signature");
      break;
    case VMError::STACK_CANT_POP:
      ERROR_M("stack cant pop");
      break;
    case VMError::STACK_CANT_PUSH:
      ERROR_M("stack cant push");
      break;
    case VMError::UNKNOWN_COMMAND_KEY:
      ERROR_M("unknown command key");
      break;
    default:
      ASSERT(0 && "UNKNOWN ERROR CODE");
      break;
  }
}

void VM::Dump(const char* file,const size_t line, const char* func) {
  fprintf(stderr, "#####################################################\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "#  VM called from  file %s(%lu) from function %s\n", file, line, func);
  fprintf(stderr, "#  {\n");
  fprintf(stderr, "#    binData\n");
  fprintf(stderr, "#    {\n");
  fprintf(stderr, "#      buf\n");
  fprintf(stderr, "#        ");
  // for (size_t i = 0; i < binData.bufSz; i++) {
  //   fprintf(stderr, "%.2X ", binData.buf[i]);
  // fprintf(stderr, "#      {\n");
  // }
  fprintf(stderr, "\n");
  fprintf(stderr, "#      }\n");
  fprintf(stderr, "#      bufSz %lu\n", binData.bufSz);
  fprintf(stderr, "#    }\n");
  fprintf(stderr, "#    ip %p\n", ip);
  fprintf(stderr, "#    cpu\n");
  fprintf(stderr, "#    {\n");

  fprintf(stderr, "#      regs\n");
  fprintf(stderr, "#      {\n");
  fprintf(stderr, "#        rax %lf\n", cpu.regs[0]);
  fprintf(stderr, "#        rbx %lf\n", cpu.regs[1]);
  fprintf(stderr, "#        rcx %lf\n", cpu.regs[2]);
  fprintf(stderr, "#        rdx %lf\n", cpu.regs[3]);
  fprintf(stderr, "#      }\n");

  fprintf(stderr, "#      memory\n");
  fprintf(stderr, "#      {\n");
  for (size_t i = 0; i < MemorySize/10; i++) {
  fprintf(stderr, "#        ");
    for (size_t j = 0; j < MemorySize/10; j++) {
      fprintf(stderr, "%.3lf ", cpu.memory[i*10 + j]);
    }
    putc('\n', stderr);
  }
  fprintf(stderr, "#      }\n");

  fprintf(stderr, "#      stk {}\n");
  fprintf(stderr, "#    }\n");
  fprintf(stderr, "#  }\n");
  fprintf(stderr, "\n");

  STACK_DUMP(&cpu.stk, 0);

  fprintf(stderr, "#####################################################\n");
}

void VM::MemDump() {
  char memCpy[MemorySize] = {};
  double max = 0;
  for (size_t i = 0; i < MemorySize; i++) {
    max = MAX(max, abs(cpu.memory[i]));
  }

  for (size_t i = 0; i < MemorySize; i++) {
    memCpy[i] = (char)((cpu.memory[i]/max)*127);
  }

  size_t side = MemorySize / 10;
  for (size_t i = 0; i < side; i++) {
    for (size_t j = 0; j < side; j++) {
      putc(memCpy[i * side + j], ErrorStream);
      putc(' ', ErrorStream);
    }
    putc('\n', ErrorStream);
  }
}

VMError VM::Execute() {
  while (ip < binData.buf + binData.bufSz) {
    // Dump(__FILE__, __LINE__, __func__);
    VMError error = ExecuteCmd();
    if (error != VMError::SUCCESS) { return error; }
  }

  return VMError::SUCCESS;
}

//private----------------------------------------------------------------------

VMError VM::ExecuteCmd() {
  cmdKey_t cmdKey = *ip;
  ip += sizeof(cmdKey);

  regId_t regId = 0;
  arg_t arg = 0;
  arg_t both = 0;

  if (cmdKey & BitFlags::ARG_REG) {
    regId = *ip;
    ip += sizeof(regId);
    both += cpu.regs[regId-1l];
  }

  if (cmdKey & BitFlags::ARG_IMMED) {
    memcpy(&arg, ip, sizeof(arg));
    ip += sizeof(arg);
    both += arg;
  }

  /* start of def */
  #define DEF_CMD(name, cmdId, isJmp, allowedArgs, ...)                       \
    case cmdId:                                                               \
      { __VA_ARGS__ }                                                         \
      break;
  /* end of def */

  switch (cmdKey & BitFlags::CODE_ID_MASK) {
    #include "commandSet.h"

    default:
      return VMError::UNKNOWN_COMMAND_KEY;
      break;
  }

  #undef DEF_CMD

  return VMError::SUCCESS;
}
