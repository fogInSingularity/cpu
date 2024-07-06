DEF_CMD(HLT, 0, false, AllowedArgs::____, {
  HALT();
})

DEF_CMD(out, 1, false, AllowedArgs::____, {
  elem_t tmp = 0;
  POP(&tmp);
  OUT_ELEM(tmp);
  // PUSH(tmp);
})

DEF_CMD(push, 2, false, AllowedArgs::___I | AllowedArgs::__R_ | AllowedArgs::__RI |
                        AllowedArgs::M__I | AllowedArgs::M_R_ | AllowedArgs::M_RI, {
  if (MEM_ACCESS) {
    PUSH(GET_FROM_MEM(BOTH_ARGS));
  } else {
    PUSH(BOTH_ARGS);
  }
})

DEF_CMD(pop, 3, false, AllowedArgs::__R_ | AllowedArgs::M__I |
                       AllowedArgs::M_R_ | AllowedArgs::M_RI, {
  if (MEM_ACCESS) {
    MEM_POP();
  } else {
    REG_POP();
  }
})

DEF_CMD(in, 4, false, AllowedArgs::____, {
  elem_t tmp = 0;
  IN_ELEM(&tmp);
  PUSH(tmp);
})

DEF_CMD(add, 5, false, AllowedArgs::____, {
  elem_t firstElem = 0;
  elem_t secondElem = 0;

  POP(&firstElem);
  POP(&secondElem);

  PUSH(firstElem + secondElem);
})

DEF_CMD(sub, 6, false, AllowedArgs::____, {
  elem_t firstElem = 0;
  elem_t secondElem = 0;

  POP(&firstElem);
  POP(&secondElem);

  PUSH(secondElem - firstElem);
})

DEF_CMD(mul, 7, false, AllowedArgs::____, {
  elem_t firstElem = 0;
  elem_t secondElem = 0;

  POP(&firstElem);
  POP(&secondElem);

  PUSH(secondElem * firstElem);
})

DEF_CMD(div, 8, false, AllowedArgs::____, {
  elem_t firstElem = 0;
  elem_t secondElem = 0;

  POP(&firstElem);
  POP(&secondElem);

  PUSH(secondElem / firstElem);
})

DEF_CMD(sqrt, 9, false, AllowedArgs::____, {
  elem_t tmp = 0;

  POP(&tmp);

  PUSH(sqrt(tmp));
})

DEF_CMD(cos, 10, false, AllowedArgs::____, {
  elem_t tmp = 0;

  POP(&tmp);

  PUSH(cos(tmp));
})

DEF_CMD(sin, 11, false, AllowedArgs::____, {
  elem_t tmp = 0;

  POP(&tmp);

  PUSH(sin(tmp));
})

DEF_CMD(jmp, 12, true, AllowedArgs::_L__, {
  JUMP_ON_ADR(ADR);
})

//TODO copypase
DEF_CMD(ja, 13, true, AllowedArgs::_L__, {
  CONDITIONAL_JUMP(tmp1 > tmp2);
})

DEF_CMD(jae, 14, true, AllowedArgs::_L__, {
  CONDITIONAL_JUMP(tmp1 >= tmp2);
})

DEF_CMD(jb, 15, true, AllowedArgs::_L__, {
  CONDITIONAL_JUMP(tmp1 < tmp2);
})

DEF_CMD(jbe, 16, true, AllowedArgs::_L__, {
  CONDITIONAL_JUMP(tmp1 <= tmp2);
})

DEF_CMD(je, 17, true, AllowedArgs::_L__, {
  CONDITIONAL_JUMP(IS_EQUAL(tmp1, tmp2));
})

DEF_CMD(jne, 18, true, AllowedArgs::_L__, {
  CONDITIONAL_JUMP(!IS_EQUAL(tmp1, tmp2));
})

DEF_CMD(call, 19, true, AllowedArgs::_L__, {
  PUSH(IP);
  JUMP_ON_ADR(ADR);
})

DEF_CMD(ret, 20, false, AllowedArgs::____, {
  elem_t tmp = 0;
  POP(&tmp);
  JUMP_ON_ADR(tmp);
})

DEF_CMD(memdump, 21, false, AllowedArgs::____, {
  MEMDUMP();
})
