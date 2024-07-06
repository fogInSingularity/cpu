out          ;pop -> print
in           ;scan -> push
add          ;pop pop -> add -> push
sub          ;pop pop -> sub -> push (lower - upper)
mul          ;pop pop -> mul -> push
div          ;pop pop -> div -> push (lower/upper)
sqrt         ;pop -> sqrt -> push
cos          ;pop -> cos(in rad) -> push
sin          ;pop -> sin(in rad) -> push

push 1       ;push const
push rax     ;push value in reg(reg not changed)
push rax+1   ;push value in reg + const(reg not changed)
push [1]     ;push value from const memoryadress
push [rax]   ;push value from mem adress that reg holds
push [rax+1] ;push value from mem adress that reg holds + const

             ;pops: old values will be lost
pop rax      ;pop to reg
pop [1]      ;pop to const mem adress
pop [rax]    ;pop to mem adress that holds reg
pop [rax+1]  ;pop to mem adress that holds reg + const
;pop [label] ;_unimplemented feature_

; jumps (will pop 2 top elems)
label:       ;jump to
jmp label    ;unconditional jump
ja label     ;jump if [elem]     | top
             ;          V        |
             ;        [elem]     V bottom

jae label    ;jump if [elem]     | top
             ;          V=       |
             ;        [elem]     V bottom

jb label     ;jump if [elem]     | top
             ;          ^        |
             ;        [elem]     V bottom

jbe label     ;jump if [elem]    | top
             ;          ^=       |
             ;        [elem]     V bottom

je label     ;jump if [elem]     | top
             ;          =        |
             ;        [elem]     V bottom

jne label    ;jump if [elem]     | top
             ;          !=       |
             ;        [elem]     V bottom

;jump +-n    ;_unimplemented feature_

;functions
discr:       ;function defenition (also label)
  ; discrim
  ret        ;return will pop top elem and jump on it

call discr   ;function call (push jump adr where to return)

HLT          ;exit point (will return VMError::HLT)

memdump      ; dumps memroy to stderr
