DEF_CMD(HLT, 0, false, AllowedArgs::____, {
  exit(0);
})

DEF_CMD(out, 1, false, AllowedArgs::____, {
  elem_t tmp = 0;
  StackPop(vm->cpu.stk, &tmp);
  fprintf(OutStream, "Out: %lf\n", tmp);
  StackPush(vm->cpu.stk, tmp);
})

DEF_CMD(push, 2, false,
  AllowedArgs::___I | AllowedArgs::__R_ | AllowedArgs::__RI |
  AllowedArgs::M__I | AllowedArgs::M_R_ | AllowedArgs::M_RI, {
  switch (tArgs) {
    case REG_AND_IMMED:
      switch (reg) {
        case RAX:
          StackPush(vm->cpu.stk, vm->cpu.regs.rax + arg);
          break;
        case RBX:
          StackPush(vm->cpu.stk, vm->cpu.regs.rbx + arg);
          break;
        case RCX:
          StackPush(vm->cpu.stk, vm->cpu.regs.rcx + arg);
          break;
        case RDX:
          StackPush(vm->cpu.stk, vm->cpu.regs.rdx + arg);
          break;
        default:
          ASSERT(0 && "!  Unknown regester");
          break;
      }

      break;
    case ONLY_REG:
      switch (reg) {
        case RAX:
          StackPush(vm->cpu.stk, vm->cpu.regs.rax);
          break;
        case RBX:
          StackPush(vm->cpu.stk, vm->cpu.regs.rbx);
          break;
        case RCX:
          StackPush(vm->cpu.stk, vm->cpu.regs.rcx);
          break;
        case RDX:
          StackPush(vm->cpu.stk, vm->cpu.regs.rdx);
          break;
        default:
          ASSERT(0 && "!  Unknown regester");
          break;
      }

      break;
    case ONLY_IMMED:
      StackPush(vm->cpu.stk, arg);

      break;
    case INIT_ARGS:
    case NO_ARGS:
    default:
      ASSERT(0 && "!  unknown args");

      break;
  }
})

DEF_CMD(pop, 3, false,
  AllowedArgs::__R_ | AllowedArgs::M__I | AllowedArgs::M_R_ | AllowedArgs::M_RI, {
  switch (reg) {
    case RAX:
      StackPop(vm->cpu.stk, &vm->cpu.regs.rax);
      break;
    case RBX:
      StackPop(vm->cpu.stk, &vm->cpu.regs.rbx);
      break;
    case RCX:
      StackPop(vm->cpu.stk, &vm->cpu.regs.rcx);
      break;
    case RDX:
      StackPop(vm->cpu.stk, &vm->cpu.regs.rdx);
      break;
    default:
      ASSERT(0 && "!  Unknown regester");
      break;
  }
})

DEF_CMD(in, 4, false, AllowedArgs::____, {
  elem_t tmp = 0;
  fscanf(InStream, "%lf", &tmp);
  StackPush(vm->cpu.stk, tmp);
})

DEF_CMD(add, 5, false, AllowedArgs::____, {
  elem_t firstElem = 0;
  elem_t secondElem = 0;

  StackPop(vm->cpu.stk, &firstElem);
  StackPop(vm->cpu.stk, &secondElem);

  StackPush(vm->cpu.stk, firstElem + secondElem);
})

DEF_CMD(sub, 6, false, AllowedArgs::____, {
  elem_t firstElem = 0;
  elem_t secondElem = 0;

  StackPop(vm->cpu.stk, &firstElem);
  StackPop(vm->cpu.stk, &secondElem);

  StackPush(vm->cpu.stk, secondElem - firstElem);
})

DEF_CMD(mul, 7, false, AllowedArgs::____, {
  elem_t firstElem = 0;
  elem_t secondElem = 0;

  StackPop(vm->cpu.stk, &firstElem);
  StackPop(vm->cpu.stk, &secondElem);

  StackPush(vm->cpu.stk, secondElem * firstElem);
})

DEF_CMD(div, 8, false, AllowedArgs::____, {
  elem_t firstElem = 0;
  elem_t secondElem = 0;

  StackPop(vm->cpu.stk, &firstElem);
  StackPop(vm->cpu.stk, &secondElem);

  StackPush(vm->cpu.stk, secondElem / firstElem);
})

DEF_CMD(sqrt, 9, false, AllowedArgs::____, {
  elem_t tmp = 0;

  StackPop(vm->cpu.stk, &tmp);

  StackPush(vm->cpu.stk, sqrt(tmp));
})

DEF_CMD(cos, 10, false, AllowedArgs::____, {
  elem_t tmp = 0;

  StackPop(vm->cpu.stk, &tmp);

  StackPush(vm->cpu.stk, cos(tmp));
})

DEF_CMD(sin, 11, false, AllowedArgs::____, {
  elem_t tmp = 0;

  StackPop(vm->cpu.stk, &tmp);

  StackPush(vm->cpu.stk, sin(tmp));
})

DEF_CMD(jmp, 12, true, AllowedArgs::_L__, {
  vm->pos = vm->binData->buf + (size_t)arg;
})

DEF_CMD(ja, 13, true, AllowedArgs::_L__, {
  elem_t tmp1 = 0;
  elem_t tmp2 = 0;
  StackPop(vm->cpu.stk, &tmp1);
  StackPop(vm->cpu.stk, &tmp2);
  if (tmp1 > tmp2) {
    vm->pos = vm->binData->buf + (size_t)arg;
  }

  StackPush(vm->cpu.stk, tmp2);
  StackPush(vm->cpu.stk, tmp1);
})

DEF_CMD(jae, 14, true, AllowedArgs::_L__, {
  elem_t tmp1 = 0;
  elem_t tmp2 = 0;
  StackPop(vm->cpu.stk, &tmp1);
  StackPop(vm->cpu.stk, &tmp2);
  if (tmp1 >= tmp2) {
    vm->pos = vm->binData->buf + (size_t)arg;
  }

  StackPush(vm->cpu.stk, tmp2);
  StackPush(vm->cpu.stk, tmp1);
})

DEF_CMD(jb, 15, true, AllowedArgs::_L__, {
  elem_t tmp1 = 0;
  elem_t tmp2 = 0;
  StackPop(vm->cpu.stk, &tmp1);
  StackPop(vm->cpu.stk, &tmp2);
  if (tmp1 < tmp2) {
    vm->pos = vm->binData->buf + (size_t)arg;
  }

  StackPush(vm->cpu.stk, tmp2);
  StackPush(vm->cpu.stk, tmp1);
})

DEF_CMD(jbe, true, AllowedArgs::_L__, {
  elem_t tmp1 = 0;
  elem_t tmp2 = 0;
  StackPop(vm->cpu.stk, &tmp1);
  StackPop(vm->cpu.stk, &tmp2);
  if (tmp1 <= tmp2) {
    vm->pos = vm->binData->buf + (size_t)arg;
  }

  StackPush(vm->cpu.stk, tmp2);
  StackPush(vm->cpu.stk, tmp1);
})

DEF_CMD(je, true, AllowedArgs::_L__, {
  elem_t tmp1 = 0;
  elem_t tmp2 = 0;
  StackPop(vm->cpu.stk, &tmp1);
  StackPop(vm->cpu.stk, &tmp2);
  if (tmp1 == tmp2) {
    vm->pos = vm->binData->buf + (size_t)arg;
  }

  StackPush(vm->cpu.stk, tmp2);
  StackPush(vm->cpu.stk, tmp1);
})

DEF_CMD(jne, true, AllowedArgs::_L__, {
  elem_t tmp1 = 0;
  elem_t tmp2 = 0;
  StackPop(vm->cpu.stk, &tmp1);
  StackPop(vm->cpu.stk, &tmp2);
  if (tmp1 != tmp2) {
    vm->pos = vm->binData->buf + (size_t)arg;
  }

  StackPush(vm->cpu.stk, tmp2);
  StackPush(vm->cpu.stk, tmp1);
})
