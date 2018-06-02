        ; 8080 assembler code
        .hexfile sum.hex
        .binfile sum.com
        ; try "hex" for downloading in hex format
        .download bin  
        .objcopy gobjcopy
        .postbuild echo "OK!"
        ;.nodump

	; OS call list
PRINT_B		equ 1
PRINT_MEM	equ 2
READ_B		equ 3
READ_MEM	equ 4
PRINT_STR	equ 5
READ_STR	equ 6
Texit 		equ 8
Tjoin 		equ 9
Tyield 		equ 10
Tcreate 	equ 11

	; Position for stack pointer
stack   equ 0F000h

	org 000H
	jmp begin

	; Start of our Operating System
GTU_OS:	PUSH D
	push D
	push H
	push psw
	nop	; This is where we run our OS in C++, see the CPU8080::isSystemCall()
		; function for the detail.
	pop psw
	pop h
	pop d
	pop D
	ret
	; ---------------------------------------------------------------
	; YOU SHOULD NOT CHANGE ANYTHING ABOVE THIS LINE        


f1:	dw 'PrintNumbers50.com',00AH,00H ; null terminated string
f3:	dw 'PrintNumbers100.com',00AH,00H ; null terminated string

begin:
	LXI SP,stack 	; always initialize the stack pointer


	LXI B, f1		; put the address of string in registers B and C
	MVI A, Tcreate	; store the OS call code to A
	call GTU_OS	; call the OS

	LXI B, f3		; put the address of string in registers B and C
	MVI A, Tcreate	; store the OS call code to A
	call GTU_OS	; call the OS


	MVI B,2		; put the address of string in registers B and C
	MVI A, Tjoin	; store the OS call code to A
	call GTU_OS	; call the OS

	MVI B,3		; put the address of string in registers B and C
	MVI A, Tjoin	; store the OS call code to A
	call GTU_OS	; call the OS



	
	hlt




