	.code16
	.text
	.globl _start

_start:
	cli

	/ set up stack
	xorw %ax, %ax
	movw %ax, %ss
	movw $top, %sp

	movw %ax, %ds
	movw %ax, %es
	ljmp $0x0000, $main

main:
	movw $str, %si
	call print
	
halt:
	cli
	hlt

	.data
str:
	.asciz "BOOTLD.BIN RUNNING!"
