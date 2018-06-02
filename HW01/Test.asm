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

	MVI A, READ_B	; store the OS call code to A
	call GTU_OS		; call the OS, after this, register B hold the value of input

	; ---------------TESTING PRINTING INPUT INTEGER

	MVI A, PRINT_B		; store the OS call code to A
	call GTU_OS	; call the OS


	;--------------- TESTING READ STRING FROM INPUT

	MVI B,30
	MVI C,30

	MVI A, READ_STR	; store the OS call code to A
	call GTU_OS	; call the OS

	;-------------- TESTING WRITING STRING

	MVI B,30
	MVI C,30

	MVI A, PRINT_STR	; store the OS call code to A
	call GTU_OS	; call the OS


	;-------------- TESTING RANDOM NUMBER

	MVI A, GET_RND	; store the OS call code to A
	call GTU_OS	; call the OS

	MVI A, PRINT_B		; store the OS call code to A
	call GTU_OS	; call the OS



	;--------------- TESTING READ FROM MEMORY 

	MVI B,60
	MVI C,60

	MVI A, READ_MEM		; store the OS call code to A
	call GTU_OS	; call the OS

	;--------------- TESTING PRINTING MEMORY CONTENT

	MVI B,60
	MVI C,60

	MVI A, PRINT_MEM		; store the OS call code to A
	call GTU_OS	; call the OS

	


	hlt
