	.code16
	.text
	.globl _start

_start:
	jmp setup
	nop
	.ascii "ALiX    "
data:
bpb:
	.word 512	# bytes per sector
	.byte 0		# sectors per cluster
	.word 0		# reserved sectors
	.byte 0		# number of FATs on disk
	.word 0		# number of directory entries
	.word 0		# number of sectors on volume (if 0, then use large number of sectors)
	.byte 0		# media descriptor
	.word 0		# number of sectors per FAT (FAT 12/16 only)
	.word 0		# sectors per track
	.word 0		# number of heads
	.long 0		# hidden sectors
	.long 0		# large number of sectors
ebpb:
	.byte 0		# drive number
	.byte 0		# reserved (flags in NT)
	.byte 0		# signature (0x28 or 0x29)
	.long 0		# volume serial number
	.ascii "ALiX boot  "	# volume label
	.ascii "FAT12   "	# fs identifier

lba0:
	.long 0
nsecs:
	.word 0

	.equ FNAME_SZ, 8+3
fname:
	.ascii "BOOTLD  "
	.ascii "BIN"

setup:
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
	ljmp $0x0000, $main

main:
	sti

	testb %dl, %dl
	jns 1f

	/ figure out lba0 of current partition
	leaw 8(%si), %si
	movw $lba0, %di
	movw $4, %cx
	rep movsb

	/ $data is 0 for some reason???	
	movw $data, %bp
	jmp halt
1:
	/ store drive number for later use
	movb %dl, 20(%bp)

	/ calculate number of 512 byte sectors to read per fat sector
	xorw %cx, %cx
	movw $0x0200, %bx
	movw (%bp), %ax
/1:
/	subw %bx, %ax
/	incw %cx
/	jnz 1b
/	movw %cx, (nsecs-data)(%bp)

	/ load root directory
	/ lookup file
	/ load file

	

halt:
	cli
	hlt

