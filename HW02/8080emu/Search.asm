
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
GET_RND		equ 7
Texit 		equ 8

; Position for stack pointer
stack   equ 0F000h
	
	org 000H

	jmp begin

; Start of our Operating System
GTU_OS:	PUSH D
	push D
	push H
	push psw
	nop			; This is where we run our OS in C++, see the CPU8080::isSystemCall()
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

	org 5555H


LIST: DW

	org 5955H

LIST1: DW


	org 0049H

	MVI D, 50
	MVI E, 0

	JMP loop12

loop13:
	INX B
	INX B
	DCR H
	JNZ loop13

	STAX B
	JMP loop14


loop12:
	MVI A, GET_RND	; store the OS call code to A
	call GTU_OS	; call the OS

	MOV A,B
	INR E
	MOV H,E
	LXI B,LIST
	JMP loop13

loop14:
	DCR D
	JNZ loop12




------------------------SORTING

	LXI B,LIST
	LDAX B

	LXI B,LIST1
	STAX B

	MVI E,10

	LXI B,LIST

loop_caner:
	
	
	INX B
	DCR E
	JNZ loop_caner
	
	LDAX B
	LXI B,LIST1
	INX B
	STAX B

	MVI E,40

	LXI B,LIST

loop_caner1:
	
	INX B
	DCR E
	JNZ loop_caner1
	LDAX B
	LXI B,LIST1
	INX B
	INX B
	STAX B




loop1:				;while
		MVI E,50
		MVI D,0
		LXI B,LIST
		LDAX B	
loop2:				;for		
		MOV H,A
		INX B
		INX B
		LDAX B
		CMP H
		JC loop3
		DCR E		; --E, list size
		JNZ loop2	; goto loop if E != 0	;for
		DCR D
		JZ loop1
loop4:		
		MVI H,50
		LXI B, LIST; put the address of string in registers B and C
loop5:
		LDAX B
		MOV D,B
		MOV B,A
		MOV B,D
		INX B
		INX B
		DCR H
		JNZ loop5
		JMP main_loop
loop3: 
	
		MOV L,A
		MOV A,H
		STAX B
		DCX B
		DCX B
		MOV A,L
		STAX B
		INX B
		INX B
		LDAX B
		MVI D,1
		DCR E
		JNZ loop2	; goto loop if E != 0	;for
		DCR D
		JZ loop1
		jmp loop4






---------------------------SORTING1



caner_loop2:
	LXI B,LIST1
	LDAX B
	MOV B,A



main_loop:


	MOV H,B     			; X (number to search)
	MVI L, 0				; L (0 )
	MVI E, 49 				; R  (ARRAY SIZE)
	
	; R YI

							; l + (r-l)/2

main_loop1:
	MOV A,E
	MOV D,A
	MVI M,0
	SUB L					; r = r-l

main_loop2:

	INR M 					; r / 2
	DCR A
	DCR A
	JP main_loop2

	DCR M

	MOV A,M 				; 

	ADD L 					; 

	MOV M,A

	LXI B,LIST


main_loop3:
	
	INX B
	INX B
	DCR A
	JP main_loop3

	LDAX B 					;

	; array deki değeri karşılaştır bundan sonra

	MOV E,A

	MOV A,E

	SUB H
	JZ finished_found

	MOV A,E
	SUB H
	JP  minus
	JMP plus

minus:
	DCR M
	MOV A,M 				; r = m - 1;
	MOV E,A
	SUB L
	JM finished
	JMP main_loop1

plus:
	INR M
	MOV L,M 				; l = m + 1; 
	MOV A,D
	MOV E,A
	SUB L
	JM finished
	JMP main_loop1

finished:

string:	dw 'Error',00AH,00H ; null terminated string
	
	LXI B, string		; put the address of string in registers B and C
	MVI A, PRINT_STR	; store the OS call code to A
	call GTU_OS	; call the OS
	
	hlt				; BULUNAMADI

finished_found:

	MOV B,M
	MVI A, PRINT_B	; store the OS call code to A
	call GTU_OS	; call the OS

	

	LXI B,LIST1
	INX B
	LDAX B
	MOV B,A


---------------------------SORTING2


main_loop7:




	MOV H,B     			; X (number to search)
	MVI L, 0				; L (0 )
	MVI E, 49 				; R  (ARRAY SIZE)
	
	; R YI

							; l + (r-l)/2

main_loop17:
	MOV A,E
	MOV D,A
	MVI M,0
	SUB L					; r = r-l

main_loop27:

	INR M 					; r / 2
	DCR A
	DCR A
	JP main_loop27

	DCR M

	MOV A,M 				; 

	ADD L 					; 

	MOV M,A

	LXI B,LIST


main_loop37:
	
	INX B
	INX B
	DCR A
	JP main_loop37

	LDAX B 					;

	; array deki değeri karşılaştır bundan sonra

	MOV E,A

	MOV A,E

	SUB H
	JZ finished_found7

	MOV A,E
	SUB H
	JP  minus7
	JMP plus7

minus7:
	DCR M
	MOV A,M 				; r = m - 1;
	MOV E,A
	SUB L
	JM finished7
	JMP main_loop17

plus7:
	INR M
	MOV L,M 				; l = m + 1; 
	MOV A,D
	MOV E,A
	SUB L
	JM finished7
	JMP main_loop17

finished7:

string:	dw 'Error',00AH,00H ; null terminated string
	
	LXI B, string		; put the address of string in registers B and C
	MVI A, PRINT_STR	; store the OS call code to A
	call GTU_OS	; call the OS
	
	hlt				; BULUNAMADI

finished_found7:

	MOV B,M

	MVI A, PRINT_B	; store the OS call code to A
	call GTU_OS	; call the OS




	LXI B,LIST1
	INX B
	INX B
	LDAX B
	MOV B,A



---------------------------SORTING3


main_loop8:



	MOV H,B     			; X (number to search)
	MVI L, 0				; L (0 )
	MVI E, 49 				; R  (ARRAY SIZE)
	
	; R YI

							; l + (r-l)/2

main_loop18:
	MOV A,E
	MOV D,A
	MVI M,0
	SUB L					; r = r-l

main_loop28:

	INR M 					; r / 2
	DCR A
	DCR A
	JP main_loop28

	DCR M

	MOV A,M 				; 

	ADD L 					; 

	MOV M,A

	LXI B,LIST


main_loop38:
	
	INX B
	INX B
	DCR A
	JP main_loop38

	LDAX B 					;

	; array deki değeri karşılaştır bundan sonra

	MOV E,A

	MOV A,E

	SUB H
	JZ finished_found8

	MOV A,E
	SUB H
	JP  minus8
	JMP plus8

minus8:
	DCR M
	MOV A,M 				; r = m - 1;
	MOV E,A
	SUB L
	JM finished8
	JMP main_loop18

plus8:
	INR M
	MOV L,M 				; l = m + 1; 
	MOV A,D
	MOV E,A
	SUB L
	JM finished8
	JMP main_loop18

finished8:

string:	dw 'Error',00AH,00H ; null terminated string
	
	LXI B, string		; put the address of string in registers B and C
	MVI A, PRINT_STR	; store the OS call code to A
	call GTU_OS	; call the OS
	
	hlt				; BULUNAMADI

finished_found8:

	MOV B,M

	MVI A, PRINT_B	; store the OS call code to A
	call GTU_OS	; call the OS

	MVI A, TExit	; store the OS call code to A
	call GTU_OS	; call the OS


	hlt
	
















		
	



