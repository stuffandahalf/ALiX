	.code16
	.globl _start

top:
_start:
	xorw %ax, %ax
	movw %ax, %es
	movw %ax, %ds
	movw %ax, %ss
	movw $top, %sp
	ljmp $0x0000, $main

main:
	sti

	/ load this boot sector to address 0x500

	/ store number of sectors to read in %dh
	movb $0x01, %dh
	pushw %dx
	xorw %ax, %ax
	pushw %ax
	pushw $0x0500
	pushw %ax
	pushw %ax
	call read

pause:
	cli
	hlt

	.include "../read.s"

	.org top+510

boot_sig:
	.byte 0x55, 0xaa

