### --------------------------------------------------------------------
### mydc.s
###
### Desk Calculator (dc)
### --------------------------------------------------------------------

	.equ   ARRAYSIZE, 20
	.equ   EOF, -1
	.equ   TRUE, 1
	.equ	FALSE, 0
	.equ    MIN, 12
	.equ    MAX, 8
	.equ    INDEX, -8
	.equ	SUM, -4
	
.section ".rodata"

scanfFormat:
	.asciz "%s"

emptyStack:
    .asciz "dc: stack empty\n"

printInt:
    .asciz "%d\n"
### --------------------------------------------------------------------

        .section ".data"

### --------------------------------------------------------------------

        .section ".bss"
buffer:
        .skip  ARRAYSIZE
iIndex:
		.skip	4

### --------------------------------------------------------------------

	.section ".text"

	## -------------------------------------------------------------
	## int main(void)
	## Runs desk calculator program.  Returns 0.
	## -------------------------------------------------------------

	.globl  main
	.type   main,@function

main:

	pushl   %ebp
	movl    %esp, %ebp
    call input
    movl    $0, %eax
	movl    %ebp, %esp
	popl    %ebp
	ret



	## dc number stack initialized. %esp = %ebp
input:
    pushl %ebp
    movl %esp, %ebp

## The start of the main while loop that takes input:
while_1:
	## scanf("%s", buffer)
	pushl	$buffer
	pushl	$scanfFormat
	call    scanf
	addl    $8, %esp

	## check if user input EOF
	cmp	$EOF, %eax
	je	quit

### if (isdigit(buffer[0]) || buffer[0] == '_') {
	movl $0, %eax
    movb buffer, %al
    pushl %eax
    call isdigit
    addl $4, %esp


    cmp $FALSE, %eax
    jne start
	movb buffer, %al
### $0x5F is the '_' character
    cmpb $0x5F, %al
	jne non_digit

start:
    movb buffer, %al
    movb $0x5F, %bl
    cmpb %al, %bl
    jne atoi_call
### putting the '-' charac at buffer[0]
    movb $0x2D, buffer

atoi_call:
    pushl $buffer
    call atoi
    addl $4, %esp
    pushl %eax

    jmp while_1

non_digit:

### BLOCK FOR P COMMAND:
	movb $0x70, %bl
	cmpb buffer, %bl
	jne afterp
###if (stack.peek() == NULL) {
	cmpl %esp, %ebp
	jne _p

	pushl $emptyStack
	call printf
	addl $4, %esp
	jmp while_1
_p:
### else {printf("%d\n", (int)stack.top());}
	pushl $printInt
	call printf
	addl $4, %esp
	jmp while_1

afterp:

	movb $0x71, %bl
	cmpb buffer, %bl
	jne afterq


### BLOCK FOR Q COMMAND:
_q:
	jmp quit
afterq:

### BLOCK FOR ADDITION COMMAND:
	movb $0x2B, %bl
	cmpb buffer, %bl
	jne after_add

	cmpl %esp, %ebp
	jne _add

	pushl $emptyStack
	call printf
	addl $4, %esp
	jmp while_1

_add:
	### a = (int)stack.pop();
	###pop top of stack into the ebx register
	popl %ebx
	cmpl %esp, %ebp
	jne _add_body

###	if (stack.peek() == NULL)
empty_stack1:
	pushl $emptyStack
	call printf
	addl $4, %esp
	pushl %ebx
	jmp while_1
	
_add_body:
## check if there are two operands:
	movl %esp, %eax
	subl $4, %eax
	cmpl %eax, %ebp
	je empty_stack1
##actually perform the addition operation:
	popl %ecx
	addl %ebx, %ecx
	pushl %ecx
	jmp while_1

after_add:

### BLOCK FOR SUBTRACTION COMMAND:

	### if (buffer[0] == '-') {
	movb $0x2D, %bl
	cmpb buffer, %bl
	jne after_subtract

###	if (stack.peek() == NULL)
	cmpl %esp, %ebp
	jne _subtract

	pushl $emptyStack
	call printf
	addl $4, %esp
	jmp while_1

_subtract:
	### a = (int)stack.pop();
	###pop top of stack into the ebx register
	popl %ebx
	cmpl %esp, %ebp
	jne _subtract_body

	###	if (stack.peek() == NULL)
empty_stack2:
	pushl $emptyStack
	call printf
	addl $4, %esp
	pushl %ebx
	jmp while_1

_subtract_body:
## check if there are two operands:
	movl %esp, %eax
	subl $4, %eax
	cmpl %eax, %ebp
	je empty_stack2
##actually perform the operation:
	popl %ecx
	subl %ebx, %ecx
	pushl %ecx
	jmp while_1

after_subtract:

### BLOCK FOR MULTIPLICATION COMMAND:

### if (buffer[0] == '*')
	movb $0x2A, %bl
	cmpb buffer, %bl
	jne after_multiply

	cmpl %esp, %ebp
	jne _multiply

	pushl $emptyStack
	call printf
	addl $4, %esp
	jmp while_1

_multiply:
	### a = (int)stack.pop();
	###pop top of stack into the eax register
	popl %eax
	cmpl %esp, %ebp
	jne _multiply_body

	###	if (stack.peek() == NULL)
	pushl $emptyStack
	call printf
	addl $4, %esp
	pushl %eax
	jmp while_1

_multiply_body:
	popl %ebx
	imull %ebx
	pushl %eax
	jmp while_1

after_multiply:

### BLOCK FOR DIVISION COMMAND:

### if (buffer[0] == '/')
	movb $0x2F, %bl
	cmpb buffer, %bl
	jne after_divide

	cmpl %esp, %ebp
	jne _divide

	pushl $emptyStack
	call printf
	addl $4, %esp
	jmp while_1

_divide:
	### a = (int)stack.pop();
	###pop top of stack into the ebx register
	popl %ebx
	cmpl %esp, %ebp
	jne _divide_body

	###	if (stack.peek() == NULL)
	pushl $emptyStack
	call printf
	addl $4, %esp
	pushl %ebx
	jmp while_1

_divide_body:
	movl $0, %edx
	popl %eax
	idiv %ebx
	pushl %eax
	jmp while_1

after_divide:

### BLOCK FOR ABSSUM COMMAND:

	### if (buffer[0] == '|')
	movb $0x7C, %bl
	cmpb buffer, %bl
	jne after_abssum

	cmpl %esp, %ebp
	jne _abssum_prep

	pushl $emptyStack
	call printf
	addl $4, %esp
	jmp while_1

_abssum_prep:
	### a = (int)stack.pop();
	###pop top of stack into the eax register
	popl %eax
	cmpl %esp, %ebp
	jne _abssum_exec

	###	if (stack.peek() == NULL)
	pushl $emptyStack
	call printf
	addl $4, %esp
	pushl %ebx
	jmp while_1

	## Pop parameters into eax and ebx registers
_abssum_exec:
	popl %ebx
	cmpl %ebx, %eax
    jg push_ba

    pushl %eax
    pushl %ebx
    jmp abs_call

### maximum (the larger of eax and ebx) is placed at the top of the stack:
push_ba:
    pushl %ebx
    pushl %eax

abs_call:
    call abssum
    addl $8, %esp
    pushl %eax

after_abssum:

	### if (buffer[0] == 'f')
	movb $0x66, %bl
	cmpb buffer, %bl
	jne afterf

	cmpl %esp, %ebp
	jne _f

## if the stack is empty, do nothing and jump back to while_1
	jmp while_1

_f:
	movl $0, iIndex

loop2:
##check the condition:
	movl iIndex, %eax
	addl %esp, %eax
	cmpl %eax, %ebp
	je endloop2

	pushl (%eax)
	pushl $printInt
	call printf
	addl $8, %esp

##increase eax so that it goes deeper in the stack
	addl $4, iIndex
	jmp loop2
endloop2:
	jmp while_1

afterf:
	### if (buffer[0] == 'c')
	movb $0x63, %bl
	cmpb buffer, %bl
	jne afterc

	cmpl %esp, %ebp
	jne _c
##if stack is empty, do nothing and jump back to while_1
	jmp while_1
_c:
    movl %ebp, %esp
    jmp while_1

afterc:

    movb $0x64, %bl
	cmpb buffer, %bl
	jne afterd
###if (stack.peek() == NULL) {
	cmpl %esp, %ebp
	jne _d
	pushl $emptyStack
	call printf
	addl $4, %esp
	jmp while_1
_d:
### else {printf("%d\n", (int)stack.top());}
	movl (%esp), %eax
    pushl %eax
    jmp while_1

afterd:
    movb $0x72, %bl
	cmpb buffer, %bl
	jne after_r
###if (stack.peek() == NULL) {
    cmpl %esp, %ebp
	jne _r
	pushl $emptyStack
	call printf
	addl $4, %esp
	jmp while_1

_r:
    ### a = (int)stack.pop();
	###pop top of stack into the ebx register
	popl %ebx
	cmpl %esp, %ebp
	jne _r_body

	###	if (stack.peek() == NULL): if there was only one element in stack
	pushl $emptyStack
	call printf
	addl $4, %esp
	pushl %ebx
	jmp while_1

_r_body:
    popl %eax
    pushl %ebx
    pushl %eax
    jmp while_1

after_r:
    jmp while_1
### end of the input function
    movl    $0, %eax
	movl    %ebp, %esp
	popl    %ebp
	ret

##### FUNCTION IMPLEMENTATIONS:

abssum:
    pushl %ebp
    movl %esp, %ebp

    pushl $0            ##SUM value
    subl $4, %esp		##INDEX value
    movl MIN(%ebp), %eax
    movl %eax, INDEX(%ebp)

loop1:
    movl INDEX(%ebp), %eax
    cmp MAX(%ebp), %eax
    jg  endloop1
	movl %eax, %ecx
## checking if the value in ecx is negative
## check if the MSB is 1:
    sarl $31, %ecx
	movl $-1, %edx
	cmpl %ecx, %edx
    je abs_val
addition:
    addl %eax, SUM(%ebp)
    incl INDEX(%ebp)
    jmp loop1
abs_val:
## taking absolute value of negative numbers
	negl %eax
    jmp addition
endloop1:
    movl SUM(%ebp), %eax
    movl %ebp, %esp
    popl %ebp
    ret


quit:	
	## return 0
	movl    $0, %eax
	movl    %ebp, %esp
	popl    %ebp
	ret

