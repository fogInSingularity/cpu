#include "../include/vm.h"

// //----------------------------------------------

// static FILE* InStream = stdin;
// static FILE* OutStream = stdout;

// static void VMDump(VM* vm, const char* file, size_t line, const char* func);
// static void ExecuteCode(VM* vm);

// #define VM_DUMP(vm) VMDump(vm, __FILE__, __LINE__, __func__);

// //----------------------------------------------

// void VMExecute(BinData* byteData, Stack* stk) {
//   ASSERT(byteData      != nullptr);
//   ASSERT(byteData->buf != nullptr);
//   ASSERT(stk           != nullptr);

//   VM vm = {};
//   vm.binData = byteData;
//   vm.ip = byteData->buf;
//   vm.cpu.regs = {0, 0, 0, 0};
//   vm.cpu.stk = stk;

//   while (vm.ip <= byteData->buf + byteData->bufSz) {
//     VM_DUMP(&vm);
//     ExecuteCode(&vm);
//   }
// }

// //----------------------------------------------

// __attribute_used__
// static void VMDump(VM* vm, const char* file, size_t line, const char* func) {
//   fprintf(stderr, "#####################################################\n");
//   fprintf(stderr, "\n");
//   fprintf(stderr, "#  VM called from  file %s(%lu) from function %s\n", file, line, func);
//   fprintf(stderr, "#  {\n");
//   fprintf(stderr, "#    binData\n");
//   fprintf(stderr, "#    {\n");
//   fprintf(stderr, "#      buf\n");
//   fprintf(stderr, "#      {\n");
//   fprintf(stderr, "#        ");
//   for (size_t i = 0; i < vm->binData->bufSz; i++) {
//     fprintf(stderr, "%.2X ", vm->binData->buf[i]);
//   }
//   fprintf(stderr, "\n");
//   fprintf(stderr, "#      }\n");
//   fprintf(stderr, "#      bufSz %lu\n", vm->binData->bufSz);
//   fprintf(stderr, "#    }\n");
//   fprintf(stderr, "#    ip %p\n", vm->ip);
//   fprintf(stderr, "#    cpu\n");
//   fprintf(stderr, "#    {\n");
//   fprintf(stderr, "#      regs\n");
//   fprintf(stderr, "#      {\n");
//   fprintf(stderr, "#        rax %lf\n", vm->cpu.regs.rax);
//   fprintf(stderr, "#        rbx %lf\n", vm->cpu.regs.rbx);
//   fprintf(stderr, "#        rcx %lf\n", vm->cpu.regs.rcx);
//   fprintf(stderr, "#        rdx %lf\n", vm->cpu.regs.rdx);
//   fprintf(stderr, "#      {\n");
//   fprintf(stderr, "#      stk {}\n");
//   fprintf(stderr, "#    }\n");
//   fprintf(stderr, "#  }\n");
//   fprintf(stderr, "\n");

//   STACK_DUMP(vm->cpu.stk, 0);

//   fprintf(stderr, "#####################################################\n");
// }

// static void ExecuteCode(VM* vm) {
//   ASSERT(vm->binData != nullptr);
//   ASSERT(vm->binData->buf != nullptr);
//   ASSERT(vm->ip != nullptr);
//   ASSERT(vm->cpu.stk != nullptr);

//   cmdKey_t cmdKey = *vm->ip;
//   regId_t reg = 0;
//   arg_t arg = 0;
//   Args tArgs = Args::INIT_ARGS;
//   // immed_t immed = 0;
//   // jmpAdr_t jmpAdr = 0;

//   USE_VAR(cmdKey);
//   USE_VAR(reg);
//   USE_VAR(arg);
//   USE_VAR(tArgs);
//   // USE_VAR(immed);
//   // USE_VAR(jmpAdr);

//   if (CHECK_BIT_FLAG(cmdKey, BitFlags::ARG_IMMED | BitFlags::ARG_REG)) {        // регистр число
//     reg = *(regId_t*)(vm->ip + sizeof(cmdKey_t));
//     arg = *(arg_t*)(vm->ip + sizeof(cmdKey_t) + sizeof(regId_t));

//     vm->ip += sizeof(cmdKey_t) + sizeof(regId_t) + sizeof(arg_t);

//     tArgs = Args::REG_AND_IMMED;
//   } else if (CHECK_BIT_FLAG(cmdKey, BitFlags::ARG_REG)) { // регистр
//     reg = *(regId_t*)(vm->ip+sizeof(cmdKey_t));

//     vm->ip += sizeof(cmdKey_t) + sizeof(regId_t);

//     tArgs = Args::ONLY_REG;
//   } else if (CHECK_BIT_FLAG(cmdKey, BitFlags::ARG_IMMED)) { // число
//     arg = *(immed_t*)(vm->ip + sizeof(cmdKey_t));
//     vm->ip += sizeof(cmdKey_t) + sizeof(arg_t);

//     tArgs = Args::ONLY_IMMED;
//   } else {                         // нет аргументов
//     vm->ip += sizeof(cmdKey_t);

//     tArgs = Args::NO_ARGS;
//   }

//   cmdKey = cmdKey & BitFlags::CODE_ID_MASK;

// /* start of def */
// #define DEF_CMD(name, num, isJmp, aldArgs, ...) \
//   case num: \
//     { __VA_ARGS__ } \
//     break;
// /* end of def */

//   switch (cmdKey) {// коды
//     #include "../../shared/include/commandSet.h"

//     default:
//       ASSERT(0 && "!  unknown command cmdKey");
//       break;
//   }

// #undef DEF_CMD
// }

VMError VM::SetUp(int argc, char** argv) {
  ASSERT(argv != nullptr);

  if (argc < 2) return VMError::NOT_ENOUGH_FILES;

  FILE* bytef = fopen(argv[1], "r");
  if (bytef == nullptr) return VMError::FILE_TO_READ_NOT_EXIST;

  GetData(binData, bytef);
  fclose(bytef);

  StackCtor(cpu.stk);

  cpu.regs = {};
}
