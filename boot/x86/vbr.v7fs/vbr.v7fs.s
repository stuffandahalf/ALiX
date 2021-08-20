	.code16
	.text
	.globl _start

	INODESZ = 64
	BSIZE = 512
	INOPB = BSIZE / INODESZ
	DIRENTSZ = 16
	
	buffer = 0x600

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

	/ initialize partition lba if partitioned media
	movw $lba, %di
	testb %dl, %dl
	js 1f
	pushw 10(%si)
	pushw 8(%si)
	popw (%di)
	popw 2(%di)
1:

	/ retrieve sector size
	movb $0x41, %ah
	movw $0x55aa, %bx
	int $0x13
	jc 1f
	movw $0x1e, %cx
	subw %cx, %sp
	movw %sp, %si
	movw %cx, (%si)
	movb $0x48, %ah
	int $0x13
	jc halt
	pushw 24(%si)
	popw secsz
	addw %cx, %sp
	
	movw $pkt, %bp
	
	/ read superblock
	/movw $superblk, 4(%bp)
	/xorl %eax, %eax
	/incl %eax
	/call blkread
pause:
	/ load root dir inode
	movl $0x00000002, %eax
	call iget
	call iload

	call fsearch
pause1:
	call iget
pause2:
	call iload
pause3:

	jc halt

halt:
	/movw $halted_str, %si
	/call print
	cli
	hlt

/ in:
/ %ax = addition
/ (%bp) = pkt
add_lba:
	addw %ax, 8(%bp)
	jnc 1f
	incw 10(%bp)
	jnc 1f
	incw 12(%bp)
	jnc 1f
	incw 14(%bp)
1:
	ret

/ in:
/ (%bp) = pkt
reset_lba:
	pushw %si
	leaw -2(%bp), %si
	pushw (%si)
	pushw 2(%si)
	popw 10(%bp)
	popw 8(%bp)
	xorw %si, %si
	movw %si, 12(%bp)
	movw %si, 14(%bp)
	popw %si
	ret

/ in:
/ %eax = block
/ %bp = pkt
/ %dl = drive number
/ out:
blkread:
	pushw %di
	pushl %eax

	pushw %dx
	/ multiply block number by block size
	shll $0x09, %eax
	movw %ax, %dx
	shrl $0x10, %eax
	xchgw %ax, %dx
	/ divide byte-offset by sector size
	divw secsz
	/ %ax = lba offset
	/ %dx = block offset

	popw %cx
	xchgw %cx, %dx
	pushw %cx

	pushw 4(%bp)
	pushw 6(%bp)
	pushw %ax
	xorw %ax, %ax
	movw %ax, 6(%bp)
	movw $buffer, 4(%bp)
	popw %ax
	call reset_lba
	call add_lba
	
	pushw %bp
	call read
	popw %bp
	jc halt
	
	popw %es
	popw %di
	movw 4(%bp), %si
	popw %cx
	addw %cx, %si
	movw $BSIZE, %cx
	
	rep movsb

	popl %eax
	popw %di
	ret

/ in:
/ %ax = target inode
/ %bp = packet
/ %dl = drive number
/ out:
/ %di = loaded inode
iget:
	pushw %dx
	xorw %dx, %dx
	movw $INOPB, %bx
	decw %ax
	divw %bx
	/ %ax = block offset
	/ %dx = inode offset
	
	popw %bx
	xchgw %bx, %dx
	/ inode list starts from block 2, third block
	addw $0x0002, %ax
	movw $inodel, %di
	movw %di, 4(%bp)
	call blkread

	shlw $0x0006, %bx
	addw %bx, %di
	
	ret

/ in:
/ %bp = pkt
/ %di = inode
/ %dx = drive number
/ out:
/ %di = target
/ %ecx = fsize
iload:
	pushw %si
	xorl %ecx, %ecx
	leaw 12(%di), %si
	movw $0x000b, %bx
	movw $0x0000, 4(%bp)
	/movw $TARGET_SEGMENT, 6(%bp)
	movw $0x0800, 6(%bp)
1:
	cmpl 8(%di), %ecx
	jge 2f
	
	xorl %eax, %eax
	movb 2(%si), %al
	shll $0x00000010, %eax
	movw (%si), %ax
	call blkread
	
	xorl %eax, %eax
	movw $BSIZE, %ax
	addw %ax, 4(%bp)
	addl %eax, %ecx
	addw $0x0003, %si
	
	jmp 1b
2:
	xorw %si, %si
	movw %si, 6(%bp)
	popw %si
	movw $target, %ax
	movl 8(%di), %ecx
	ret

fsearch:
	movw $target, %di
1:
	pushw %cx
	pushw %di
	
	leaw 2(%di), %di
	movw $fname, %si
	movw $0x05, %cx
	repe cmpsb
	
	popw %di
	popw %cx
	
	je 2f
	
	subl $DIRENTSZ, %ecx
	jnz 1b
	jmp 1f
	
1:
	stc
	ret
2:
	movw (%di), %ax
	ret

	.include "../read.s"
	/.include "../print.s"

/halted_str:
/	.asciz "HALTED"

fname:
	.asciz "boot"

secsz:
	.word 512

	/ lba accessed by pkt-4
lba:
	.word 0 # lo
	.word 0 # hi
pkt:
	.word 0x10 # size of pkt
	.word 0x01 # number of sectors
	.word 0x00 # offset
	.word 0x00 # segment
	.quad 0x0000000000000000 # lba

	.org top+510

boot_sig:
	.byte 0x55, 0xaa

bottom:
/superblk:

	inodel = bottom + BSIZE
	target = inodel + BSIZE
