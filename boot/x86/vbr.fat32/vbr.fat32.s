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

	/ movw $str, %si
	/ call print

pause:
	testb %dl, %dl
	jns ready

	/ figure out lba0 of current partition
	leaw 8(%si), %si
	movw $lba0, %di
	movw $4, %cx
	rep movsb

ready:
	movw $str, %si
	call print

halt:
	cli
	hlt

	.include "../print.s"

str:
	.asciz "Hello World!"

lba0:
	.long 0

	.org top+510

boot_sig:
	.byte 0x55, 0xaa

bottom:

