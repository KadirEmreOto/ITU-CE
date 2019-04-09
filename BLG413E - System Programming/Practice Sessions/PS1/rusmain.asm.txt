segment .data
prompt db       "Enter numbers: ",0
input  db       "%d %d",0
output db       "The result is: %d",10,0

segment .text
extern printf,scanf,russian
global main

main:
	push	ebp
	mov	ebp,esp
	sub	esp,12

        push    prompt
        call    printf
        add     esp,4

	lea	ecx,[ebp-8]; &y
	lea	edx,[ebp-4]; &x
        push    ecx
        push    edx
        push    input
        call    scanf
        add     esp,12

        push    dword [ebp-8]; y
        push    dword [ebp-4]; x
        call    russian
        add     esp,8
	mov	[ebp-12],eax

        push    dword [ebp-12]; z
        push    output
        call    printf
        add     esp,8

	mov	esp,ebp
	pop	ebp
        ret
