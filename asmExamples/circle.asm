push 127
pop [0]

push 0
pop rcx ; rcx = counter

; 35 = '#'
; 46 = '.'
; 127 = normalizer

for:
  push rcx+1 ; rcx++
  pop rcx

  call CalcX

  ; (
  push rcx
  push rbx
  sub

  push 10
  div
  ; )

  pop rax; rax = y

  call ShiftCoor

  ; calc sum of sqrs
  ; (
  push rbx
  push rbx
  mul

  push rax
  push rax
  mul

  add
  ; )

  push 25
  ;if x^2 + y^2 > 25
  ja colorb
  jmp colora

  back:

  push rcx
  push 99
ja for

memdump
HLT
;------------------------------------------------------------------------------
colorb:
  push 35
  pop [rcx]
jmp back

colora:
  push 46
  pop [rcx]
jmp back
;------------------------------------------------------------------------------
ShiftCoor:
  ;shift coords
  push rbx
  push 5
  sub
  pop rbx

  push rax
  push 5
  sub
  pop rax
  ; end of shift
ret
;------------------------------------------------------------------------------
CalcX:
  push rcx
  pop rbx

  while: ; while rbx > 0
  push rbx
  push -10
  add
  pop rbx

  push rbx
  push 0
  jb while

  push rbx
  push 10
  add

  pop rbx
ret
