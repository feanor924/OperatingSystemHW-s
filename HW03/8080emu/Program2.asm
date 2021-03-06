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
CREAT		equ 12
CLOSE		equ 13
OPEN     	equ 14
READ 		equ 15
WRITE		equ 16

WRITE_MEM	equ 20

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


begin:
	LXI SP,stack 	; always initialize the stack pointer

	;--------------- TESTING INPUT INTEGER


	MVI B,30
	MVI C,30

	MVI A, READ_STR		; store the OS call code to A
	call GTU_OS	; call the OS

	MVI B,30
	MVI C,30

	MVI A, OPEN		; store the OS call code to A
	call GTU_OS		; call the OS
	
	MVI D,50
	MVI E,3
	MVI A, READ		; store the OS call code to A
	call GTU_OS	; call the OS

	MVI B,3
	MVI A, CLOSE		; store the OS call code to A
	call GTU_OS	; call the OS

	hlt

