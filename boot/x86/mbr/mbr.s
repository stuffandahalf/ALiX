	.code16
	.text
	.globl _start

LOAD_ADDR = 0x7c00
LINK_ADDR = top
MBR_SZ = bottom - top

top:
	cli

	xorw %ax, %ax
	movw %ax, %ss
	movw $top, %sp

	pushw %es
	pushw %di

	movw $LOAD_ADDR, %si
	movw $LINK_ADDR, %di
	movw $MBR_SZ, %cx
	rep movsb

	ljmp $0x0000, $_start

_start:
	sti

	movb $0x04, %cl
	movw $part_tab, %si
1:
	decb %cl
	addw $0x0010, %si
	testb $0x80, (%si)
	call load_vbr
	jc 2f	# failed to load vbr
	# move vbr boot sig into %ax
	movw 500(%di), %ax
	cmpw %ax, boot_sig
	je 3f
2:
	testb %cl, %cl
	jnz 1b
	
	# return to bios
	int $0x18
3:
	popw %di
	popw %es
	jmp LOAD_ADDR

load_vbr:
	pushw %dx
	movb %dl, %al
	xorl %edx, %edx
	movl 8(%si), %eax
	movw $LOAD_ADDR, %di
	call read
	popw %dx
	ret

halt:
	cli
	hlt
	jmp halt

read:
	.include "../bios/comboread.s"

	.org top+446

part_tab:
	.fill 16, 1, 0
	.fill 16, 1, 0
	.fill 16, 1, 0
	.fill 16, 1, 0

boot_sig:
	.byte 0x55, 0xaa

bottom:
