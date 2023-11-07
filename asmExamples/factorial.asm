in

pop rax

call Factorial

push rbx

out

HLT


Factorial:
  push 1
  push rax

  je baseCase

  ; rax--
  push rax
  push -1
  add
  pop rax

  call Factorial

  ; rax++
  push rax+1
  pop rax

  push rbx
  push rax
  mul
  pop rbx

back:
  ret

baseCase:
  push 1
  pop rbx
  jmp back
