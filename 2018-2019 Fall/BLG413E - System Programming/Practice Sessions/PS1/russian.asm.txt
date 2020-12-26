segment .text
global russian

russian:
    push ebp
    mov  ebp,esp

    mov  ecx,[ebp+8]
    mov  edx,[ebp+12]
    xor  eax,eax
next:
    shr  ecx,1
    jnc  even
    add  eax,edx
even:
    shl  edx,1
    cmp  ecx,0
    jne  next

    pop  ebp
    ret
