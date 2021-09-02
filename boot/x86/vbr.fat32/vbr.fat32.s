	.code16
	.text
	.globl _start

top:
	cli

	/ set up stack
	xorw %ax, %ax
	movw %ax, %ss
	movw $top, %sp

	/ preserve PNP arguments
	pushw %es
	pushw %di

	/ clear remaining segments
	movw %ax, %es

	/ long jump to clear %cs
	ljmp $0x0000, $_start

_start:
	sti

	movw $str, %si
	call print

halt:
	cli
	hlt

	.include "../print.s"

str: .asciz "Hello World!"

	.org top+510

boot_sig:
	.byte 0x55, 0xaa

bottom:
