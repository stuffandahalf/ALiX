	.code16
	.text
	.globl _start

	LOAD_ADDR = 0x7c00
	LINK_ADDR = top
	MBR_SZ = bottom - top

top:
	cli

	/ set up stack
	xorw %ax, %ax
	movw %ax, %ss
	movw $top, %sp

	/ preserve PnP pointer
	pushw %es
	pushw %di

	/ clear remaining segments
	movw %ax, %es
	movw %ax, %ds
	
	/ copy mbr to link address
	movw $LOAD_ADDR, %si
	movw $LINK_ADDR, %di
	movw $MBR_SZ, %cx
	rep movsb

	/ jump to load address
	ljmp $0x0000, $_start

_start:
	sti
	/ start checking partition table entries
	movw $part_tab, %si
1:
/	movb (%si), %bl
	testb $0x80, (%si)
	jnz load
	cmpw $boot_sig, %si
	je noboot
	addw $0x10, %si
	jmp 1b

noboot:
	movw $noboot_str, %si
	call print
	hlt

load:
	movb $0x41, %ah
	movw $0x55aa, %bx
	int $0x13
	jc chsload

lbaload:
	xorw %ax, %ax

	pushw %ax
	pushw %ax
	pushw 10(%si)
	pushw 8(%si)
	pushw %ax
	pushw $LOAD_ADDR
	incw %ax
	pushw %ax
	pushw $0x0010
	movw %sp, %si
	movb $0x42, %ah
	int $0x13
	
	jc ioerr
	addw $0x0010, %sp
	jmp loaded

chsload:
	pushw %dx

	/ get drive parameters
	movb $0x08, %ah
	xorw %di, %di
	int $0x13

	andw $0x003f, %cx

	movb %dh, %bl
	xorb %bh, %bh
	incw %bx
	/ %cx = sectors per track
	/ %bx = nheads

	/ load lba into %dx:%ax
	movw 8(%si), %ax
	movw 10(%si), %dx

	divw %cx
	/ %ax = temp
	/ %dx = sector-1
	incw %dx
	pushw %dx

	xorw %dx, %dx
	divw %bx
	/ %ax = cylinder
	/ %dx = head

	/ reconstruct for chs sector read
	movb %al, %ch
	popw %ax
	movb %al, %cl
	movw %dx, %bx
	popw %dx
	movb %bl, %dh

	/ initialize reset counter
	movb $0x03, %al
	
1:
	testb %al, %al
	jz ioerr
	pushw %ax

	/ reset disk
	xorb %ah, %ah
	int $0x13

	/ attempt read
	movw $0x0201, %ax
	movw $0x7c00, %bx
	int $0x13

	popw %ax

	/ if disk read succeeded
	jnc loaded

	decb %al
	jmp 1b

ioerr:
	movw $fail_str, %si
	call print
	cli
	hlt
	
loaded:
	popw %di
	popw %es
	ljmp $0x0000, $LOAD_ADDR

	.include "../print.s"

str:
	.asciz "Hello World!\r\n"
noboot_str:
	.asciz "Not a bootable volume"
fail_str:
	.asciz "Failed to load volume boot record"

nheads:
	.byte 0
sects_per_track:
	.byte 0

	.org top+446

part_tab:
	/.fill 16, 1, 0
	.byte 0x80, 0x01, 0x01, 0x00, 0x72, 0x0f, 0x7f, 0x27, 0x3f, 0x00, 0x00, 0x00, 0x41, 0x8d, 0x04, 0x00
	.fill 16, 1, 0
	.fill 16, 1, 0
	.fill 16, 1, 0

boot_sig:
	.byte 0x55, 0xaa

bottom:

