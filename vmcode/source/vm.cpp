#include "../include/vm.h"

//----------------------------------------------

static FILE* InStream = stdin;
static FILE* OutStream = stdout;

struct Optn {
    code_t code;
    reg_t reg;
    immed_t cnst;
};

static void ExecuteCode(VM* vm);

static void ExecuteBoth(Cpu* cpu, Optn optn);
static void ExecuteReg(Cpu* cpu, Optn optn);
static void ExecuteCnst(Cpu* cpu, Optn optn);
static void ExecuteNoArg(Cpu* cpu, Optn optn);

static void CommandOut(Cpu* cpu, Optn optn);
static void CommandHalt(Cpu* cpu, Optn optn);
static void CommandIn(Cpu* cpu, Optn optn);
static void CommandAdd(Cpu* cpu, Optn optn);
static void CommandSub(Cpu* cpu, Optn optn);
static void CommandMult(Cpu* cpu, Optn optn);
static void CommandDiv(Cpu* cpu, Optn optn);
static void CommandSqrt(Cpu* cpu, Optn optn);
static void CommandCos(Cpu* cpu, Optn optn);
static void CommandSin(Cpu* cpu, Optn optn);
static void CommandPushCnst(Cpu* cpu, Optn optn);
static void CommandPushReg(Cpu* cpu, Optn optn);
// push reg immed //FIXME
static void CommandPop(Cpu* cpu, Optn optn);

//----------------------------------------------

void VMExecute(BinData* byteData, Stack* stk) {
    ASSERT(byteData      != nullptr);
    ASSERT(byteData->buf != nullptr);
    ASSERT(stk           != nullptr);

    VM vm = {};
    vm.binData = byteData;
    vm.pos = byteData->buf;
    vm.cpu.regs = {0, 0, 0, 0};
    vm.cpu.stk = stk;

    while (*vm.pos != CommandIds[0]) {
        ExecuteCode(&vm);
    }
}

//----------------------------------------------

static void ExecuteCode(VM* vm) {
    ASSERT(vm->binData != nullptr);
    ASSERT(vm->binData->buf != nullptr);
    ASSERT(vm->pos != nullptr);
    ASSERT(vm->cpu.stk != nullptr);
    
    code_t code = *vm->pos;
    if ((code & 0b0011'0000) == 0b0011'0000) {        // регистр число
        reg_t reg = *(reg_t*)(vm->pos + sizeof(code_t));
        immed_t cnst = *(immed_t*)(vm->pos + sizeof(code_t) + sizeof(reg_t));

        ExecuteBoth(&vm->cpu, {(code_t)(code & CODE_ID_MASK), reg, cnst});

        vm->pos += sizeof(code_t) + sizeof(reg_t) + sizeof(immed_t);
    } else if (code & 0b0010'0000) { // регистр
        reg_t reg = *(reg_t*)(vm->pos+sizeof(code_t));

        ExecuteReg(&vm->cpu, {(code_t)(code & 0b0000'1111), reg, 0});

        vm->pos += sizeof(code_t) + sizeof(reg_t);
    } else if (code & 0b0001'0000) { // число
        immed_t cnst = *(immed_t*)(vm->pos + sizeof(code_t));

        ExecuteCnst(&vm->cpu, {(code_t)(code & 0b0000'1111), 0, cnst});

        vm->pos += sizeof(code_t) + sizeof(immed_t);
    } else {                         // нет аргументов
        ExecuteNoArg(&vm->cpu, {(code_t)(code & 0b0000'1111), 0, 0});

        vm->pos += sizeof(code_t);
    }
}

static void ExecuteBoth(Cpu* cpu, Optn optn) {
    ASSERT(cpu != nullptr);
    ASSERT(cpu->stk != nullptr);

    switch (optn.code) {
        case PUSH:
//FIXME
            break;
        default:
            ASSERT(0 && "!  unknown code with 2 args");
            break;
    }
}

static void ExecuteReg(Cpu* cpu, Optn optn) {
    ASSERT(cpu != nullptr);
    ASSERT(cpu->stk != nullptr);

    switch (optn.code) {
        case PUSH:
            CommandPushReg(cpu, optn);
            break;
        case POP:
            CommandPop(cpu, optn);
            break;
        default:
            ASSERT(0 && "!  unknown code with reg");
            break;
    }
}

static void ExecuteCnst(Cpu* cpu, Optn optn) {
    ASSERT(cpu != nullptr);
    ASSERT(cpu->stk != nullptr);

    switch (optn.code) {
        case PUSH:
            CommandPushCnst(cpu, optn);
            break;
        default:
            ASSERT(0 && "!  unknown code with cnst");
            break;
    }
}

static void ExecuteNoArg(Cpu* cpu, Optn optn) {
    ASSERT(cpu != nullptr);
    ASSERT(cpu->stk != nullptr);

    switch (optn.code) {
        case HLT:
            CommandHalt(cpu, optn);
            break;
        case OUT:
            CommandOut(cpu, optn);
            break;
        case IN:
            CommandIn(cpu, optn);
            break;
        case ADD:
            CommandAdd(cpu, optn);
            break;
        case SUB:
            CommandSub(cpu, optn);
            break;
        case MULT:
            CommandMult(cpu, optn);
            break;
        case DIV:
            CommandDiv(cpu, optn);
            break;
        case SQRT:
            CommandSqrt(cpu, optn);
            break;
        case COS:
            CommandCos(cpu, optn);
            break;
        case SIN:
            CommandSin(cpu, optn);
            break;
        default:
            ASSERT(0 && "!  unknown code with no args");
            break;
    }
}

static void CommandOut(Cpu* cpu, Optn optn) {
    ASSERT(cpu != nullptr);
    ASSERT(cpu->stk != nullptr);

    USE_VAR(optn);

    elem_t tmp = 0;
    StackPop(cpu->stk, &tmp);

    fprintf(OutStream, "Out: %d\n", tmp);
}

static void CommandHalt(Cpu* cpu, Optn optn) {
    ASSERT(cpu != nullptr);
    ASSERT(cpu->stk != nullptr);

    USE_VAR(optn);

    exit(0);
}

static void CommandIn(Cpu* cpu, Optn optn) {
    ASSERT(cpu != nullptr);
    ASSERT(cpu->stk != nullptr);

    USE_VAR(optn);

    elem_t tmp = 0;

    fscanf(InStream, "%d", &tmp);
    StackPush(cpu->stk, tmp);
}

static void CommandAdd(Cpu* cpu, Optn optn) {
    ASSERT(cpu != nullptr);
    ASSERT(cpu->stk != nullptr);

    elem_t firstElem = 0;
    elem_t secondElem = 0;

    USE_VAR(optn);

    StackPop(cpu->stk, &firstElem);
    StackPop(cpu->stk, &secondElem);

    StackPush(cpu->stk, firstElem + secondElem);
}

static void CommandSub(Cpu* cpu, Optn optn) {
    ASSERT(cpu != nullptr);
    ASSERT(cpu->stk != nullptr);

    USE_VAR(optn);

    elem_t firstElem = 0;
    elem_t secondElem = 0;

    StackPop(cpu->stk, &firstElem);
    StackPop(cpu->stk, &secondElem);

    StackPush(cpu->stk, secondElem - firstElem);
}

static void CommandMult(Cpu* cpu, Optn optn) {
    ASSERT(cpu != nullptr);
    ASSERT(cpu->stk != nullptr);

    USE_VAR(optn);

    elem_t firstElem = 0;
    elem_t secondElem = 0;

    StackPop(cpu->stk, &firstElem);
    StackPop(cpu->stk, &secondElem);

    StackPush(cpu->stk, secondElem * firstElem);
}

static void CommandDiv(Cpu* cpu, Optn optn) {
    ASSERT(cpu != nullptr);
    ASSERT(cpu->stk != nullptr);

    USE_VAR(optn)

    elem_t firstElem = 0;
    elem_t secondElem = 0;

    StackPop(cpu->stk, &firstElem);
    StackPop(cpu->stk, &secondElem);

    StackPush(cpu->stk, secondElem / firstElem);
}

static void CommandSqrt(Cpu* cpu, Optn optn) {
    ASSERT(cpu != nullptr);
    ASSERT(cpu->stk != nullptr);

    USE_VAR(optn);

    elem_t tmp = 0;

    StackPop(cpu->stk, &tmp);

    StackPush(cpu->stk, (elem_t)sqrt(tmp));
}

static void CommandCos(Cpu* cpu, Optn optn) {
    ASSERT(cpu != nullptr);
    ASSERT(cpu->stk != nullptr);

    USE_VAR(optn);

    elem_t tmp = 0;

    StackPop(cpu->stk, &tmp);

    StackPush(cpu->stk, (elem_t)cos(tmp));
}

static void CommandSin(Cpu* cpu, Optn optn) {
    ASSERT(cpu != nullptr);
    ASSERT(cpu->stk != nullptr);

    USE_VAR(optn);

    elem_t tmp = 0;

    StackPop(cpu->stk, &tmp);

    StackPush(cpu->stk, (elem_t)sin(tmp));
}

static void CommandPushCnst(Cpu* cpu, Optn optn) {
    ASSERT(cpu != nullptr);
    ASSERT(cpu->stk != nullptr);

    USE_VAR(optn);

    StackPush(cpu->stk, optn.cnst);
}

static void CommandPushReg(Cpu* cpu, Optn optn) {
    ASSERT(cpu != nullptr);
    ASSERT(cpu->stk != nullptr);

    switch (optn.reg) {
        case RAX:
            StackPush(cpu->stk, cpu->regs.rax);
            break;
        case RBX:
            StackPush(cpu->stk, cpu->regs.rbx);
            break;
        case RCX:
            StackPush(cpu->stk, cpu->regs.rcx);
            break;
        case RDX:
            StackPush(cpu->stk, cpu->regs.rdx);
            break;
        default:
            ASSERT(0 && "!  Unknown regester");
            break;
    }
}

// push reg immed //FIXME

static void CommandPop(Cpu* cpu, Optn optn) {
    ASSERT(cpu != nullptr);
    ASSERT(cpu->stk != nullptr);

    switch (optn.reg) {
        case RAX:
            StackPop(cpu->stk, &cpu->regs.rax);
            break;
        case RBX:
            StackPop(cpu->stk, &cpu->regs.rbx);
            break;
        case RCX:
            StackPop(cpu->stk, &cpu->regs.rcx);
            break;
        case RDX:
            StackPop(cpu->stk, &cpu->regs.rdx);
            break;
        default:
            ASSERT(0 && "!  Unknown regester");
            break;
    }
}
