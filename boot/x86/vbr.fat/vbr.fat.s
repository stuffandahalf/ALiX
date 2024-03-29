	.code16
	.text
	.globl _start

.ifndef FAT_VERSION
	.equ FAT_VERSION, 32
.endif

.if FAT_VERSION != 12 && FAT_VERSION != 16 && FAT_VERSION != 32
	.err Invalid definition of FAT_VERSION
.endif

top:
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
.if FAT_VERSION == 12 || FAT_VERSION == 16
	.byte 0		# drive number
	.byte 0		# reserved (flags in NT)
	.byte 0		# signature (0x28 or 0x29)
	.long 0		# volume serial number
	.ascii "ALiX boot  "	# volume label
.if FAT_VERSION == 12
	.ascii "FAT12   "	# fs identifier
.endif
.if FAT_VERSION == 16
	.ascii "FAT16   "	# fs identifier
.endif
.endif
.if FAT_VERSION == 32
	.long 0		# sectors per FAT
	.word 0		# flags
	.word 0		# FAT version
	.long 2		# cluster number of root partition
	.word 0		# sector of FSInfo
	.word 0		# sector of backup boot sector
	.zero 12	# reserved
	.byte 0		# drive number
	.byte 0		# reserved (flags in NT)
	.byte 0		# signature (0x28 or 0x29)
	.long 0		# volume serial number
	.ascii "ALiX boot  "	# volume label
	.ascii "FAT32   "	# fs identifier
.endif

/ data
lba0:
	.long 0

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
	movw $data, %si

1:
	/ store drive number for later use
.if FAT_VERSION == 12 || FAT_VERSION == 16
	movb %dl, 20(%si)
.endif
.if FAT_VERSION == 32
	movb %dl, 46(%si)
.endif

	/ load root directory

halt:
	cli
	hlt

	/.include "../print.s"
	/.include "../read.s"

	/.org top+510

/boot_sig:
/	.byte 0x55, 0xaa

bottom:

