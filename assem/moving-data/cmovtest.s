.section .data
output:
	.asciz "The largest value is %d\n"

values:
	.int 1, 100, 888, 9999, 76, 10000

.section .text
.globl _start
_start:
	movl values, %ebx
	movl $1, %edi

loop:
	movl values(, %edi, 4), %eax
	cmp %ebx, %eax
	cmova %eax, %ebx
	inc %edi
	
	pushl %ebx
	pushl $output
	call printf
	addl $8, %esp

	cmp $6, %edi
	jne loop
	pushl $0
	call exit
