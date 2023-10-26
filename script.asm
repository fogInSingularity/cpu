out
in
add
sub
mul
div
sqrt
cos
sin

push 1
push rax
push rax+1
push [1]
push [rax]
push [rax+1]

pop rax
pop [1]
pop [rax]
pop [rax+1]
;pop [label] ; ?

label:
jmp label
ja label
jae label
jb label
jbe label
je label
jne label

HLT
