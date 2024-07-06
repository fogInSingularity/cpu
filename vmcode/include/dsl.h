#ifndef DSL_H
#define DSL_H
//TODO - add mem ckeck out of bounds

#define ADR (arg)
#define BOTH_ARGS (both)
#define IP ((double)(ip - binData.buf))
#define MEM_ACCESS cmdKey & BitFlags::ARG_MEM_ACS
#define GET_FROM_MEM(adr) cpu.memory[(size_t)adr]

#define HALT() return VMError::HLT;

#define POP(var)                                                              \
  if (StackPop(&cpu.stk, var) != 0) {return VMError::STACK_CANT_POP;}


#define PUSH(var)                                                             \
  if (StackPush(&cpu.stk, var) != 0) {return VMError::STACK_CANT_PUSH;}


#define OUT_ELEM(var)                                                         \
  fprintf(OutStream, "%lf\n", (var));


#define IN_ELEM(var)                                                          \
  fscanf(InStream, "%lf", (var));


#define JUMP_ON_ADR(adr)                                                      \
  ip = binData.buf + (size_t)adr;


//function
#define MEM_POP()                                                             \
  do {                                                                        \
    size_t memAdr = 0;                                                        \
    if (cmdKey & BitFlags::ARG_IMMED) {                                       \
      memAdr += (size_t)arg;                                                  \
    }                                                                         \
                                                                              \
    if (cmdKey & BitFlags::ARG_REG) {                                         \
      memAdr += (size_t)cpu.regs[regId - 1ul];                                \
    }                                                                         \
                                                                              \
    POP(cpu.memory + memAdr);                                                 \
  } while (0);


#define REG_POP()                                                             \
  POP(cpu.regs + regId - 1ul);

#define MEMDUMP() MemDump();

#define CONDITIONAL_JUMP(condition)                                           \
  elem_t tmp1 = 0;                                                            \
  elem_t tmp2 = 0;                                                            \
  POP(&tmp1);                                                                 \
  POP(&tmp2);                                                                 \
                                                                              \
  if (condition) {                                                            \
    JUMP_ON_ADR(ADR);                                                         \
  }

#endif // DSL_H
