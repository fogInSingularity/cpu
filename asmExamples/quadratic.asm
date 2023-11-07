;label is lower case
;funtions is upper case and ret at the start of new line

in ; in a
in ; in b
in ; in c

pop rcx ; c->rcx
pop rbx ; b->rbx
pop rax ; a->rax

call Solve

HLT


;Solve-------------------------------------------------------------------------
Solve:
  push 0
  push rax
  je jmpOnLin ; if a is 0 then jump to linear call
  jmp jmpOnQuad; else jump on quadratic call
back:
ret

;------------------------------------------------------------------------------
jmpOnLin:
  call LinearSolve
  jmp back

;------------------------------------------------------------------------------
jmpOnQuad:
  call QuadraticSolve
  jmp back
;------------------------------------------------------------------------------
QuadraticSolve:
  call Discr

  push rdx
  push 0
  jb twoQuadRoots;if discr > 0

  push rdx
  push 0
  je oneQuadRoot; if discr == 0

  jmp zeroQuadRoots; else discr < 0
  backQuad:
ret
;------------------------------------------------------------------------------
zeroQuadRoots:
  push 0
  out

  jmp backQuad
;------------------------------------------------------------------------------
oneQuadRoot:
  push 1
  out

  ;-b
  push rbx
  push -1
  mul

  ;2a
  push rax
  push 2
  mul

  div
  out

  jmp backQuad
;------------------------------------------------------------------------------
twoQuadRoots:
  push 2
  out

  push rdx ; save d
;d -> sqrt(d) in rdx
  push rdx
  sqrt
  pop rdx

;-b
  push rbx
  push -1
  mul

; -b - sqrt(d)
  push rdx
  sub

;2a
  push rax
  push 2
  mul

;fisrt root:
  div
  out

;----------------

;-b
  push rbx
  push -1
  mul

; -b - sqrt(d)
  push rdx
  add

;2a
  push rax
  push 2
  mul

;second root:
  div
  out

  pop rdx

  ret
;------------------------------------------------------------------------------
Discr:
;b**2:
  push rbx
  push rbx
  mul

;-4ac:
  push rax
  push rcx
  mul
  push -4
  mul

;+
  add
  pop rdx ; discr -> rdx
ret
;------------------------------------------------------------------------------
LinearSolve:
  push rbx
  push 0
  je strangeRoots;if b == 0
  jmp oneLinRoot

backLin:
ret
;------------------------------------------------------------------------------
strangeRoots:
  push rcx
  push 0
  je IntfLinRoots
  jmp ZeroLinRoots

backStrange:
  jmp backLin
;------------------------------------------------------------------------------
oneLinRoot:
  push 1
  out
;-c/2b
  push rcx
  push rbx
  div
  push -1
  mul

  out

  jmp backLin

IntfLinRoots:
  push 1
  push 0
  div ; way to print inf
  out

  jmp backStrange

ZeroLinRoots:
  push 0
  out

  jmp backStrange
