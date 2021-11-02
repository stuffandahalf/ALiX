	.code16
	.globl _start

top:
	xorw %ax, %ax
	movw %ax, %es
	movw %ax, %ds
	movw %ax, %ss
	movw $top, %sp
	ljmp $0x0000, $_start

_start:
	sti

	/ load this boot sector to address 0x500

	movw %dx, %bx
	xorw %dx, %dx
	xorw %ax, %ax
	movw $0x0500, %di
	call read

pause:
	cli
	hlt

	.include "read.s"

	.org top+510

boot_sig:
	.byte 0x55, 0xaa

