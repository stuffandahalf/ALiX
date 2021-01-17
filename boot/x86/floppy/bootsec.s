# SPDX-License-Identifier: GPL-3.0-only
#
# Copyright (C) 2021 Gregory Norton <gregory.norton@me.com>
# This program is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation, version 3.
# 
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License along with
# this program. If not, see <https://www.gnu.org/licenses/>.

	.code16
	.globl _start

top:

bpb:
	jmp _start
	nop
	.ascii "ALiX    "		/* OEM */
	.word 512				/* bytes per sector */
	.byte 1					/* sectors per cluster */
	.word 1					/* reserved sectors */
	.byte 2					/* number of fat tables */
	.word 224				/* number of root directory entries */
	.word 2880				/* total number of sectors */
	.byte 0xf0				/* media descriptor */
	.word 9					/* sectors per FAT */
	.word 18				/* sectors per track */
	.word 2					/* number of heads */
	.word 0					/* hidden sectors */
#ebpb:
#	.word 0					/* hidden sectors (high word) */
#	.double 2879			/* total number of sectors in filesystem */
#	.byte 0					/* logical drive number */
#	.byte 0					/* reserved */
#	.byte 0x29				/* extended signature */
#	.double 0				/* serial number */
#	.ascii "ALiX BOOT  "	/* volume name */
#	.ascii "FAT12   "		/* Filesystem type */

_start:
	cli
	xorw %ax, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %ss
	movw $top, %sp

	pushw %ax
	pushw $begin
	lret

begin: /* normalized segments and IP */
	sti

load_fat: /* load FAT */
	/* load FAT to address 0x0500 */
	movw $load_fat_str, %si
	call print
	
	movw bpb+22, %ax /* sectors per FAT */
	movw $0x0050, %bx
	movw %bx, %es /* target segment */
	xorw %bx, %bx /* target offset */
	movw bpb+14, %cx /* reserved sectors */
	incw %cx
	xorb %dh, %dh
	
	call load
	jc halt
	
	/* store end of chain marker */
	movw %es:1(%bx), %ax
	shrw $4, %ax
	movw %ax, eoc

load_root: /* load root directory */
	pushw %cx
	movb bpb+16, %cl /* number of FAT tables */
	xorw %ax, %ax
1:	/* loop adding sizeof(FAT) to total in %ax */
	addw bpb+22, %ax /* sizeof(FAT) */
	decb %cl
	jnz 1b
	popw %cx
	/* %ax contains number of sectors reserved for FAT */

	call add_chs

	/* calculate next destination address */
	xorw %bx, %bx
	movw bpb+22, %ax /* sectors per FAT */
2:	/* loop */
	addw bpb+11, %bx /* bytes per sector */
	decw %ax
	jnz 2b
	
	/* %bx = target offset */
	
	/* sectors for root directory = number of root entries * 32 / bytes per sector */
	pushw %dx
	xorw %dx, %dx
	movw bpb+17, %ax /* number of root entries */
	shl $5, %ax /* %ax *= 32 */
	divw bpb+11 /* bytes per sector */
	popw %dx
	
	/* load root */
	movw $load_root_str, %si
	call print
	
	call load
	jc halt
	
	xorb %ah, %ah
	call add_chs
	
	movw %sp, %bp
	pushw %dx
	pushw %cx

find_file:
	movw $locate_file_str, %si
	call print

	movw bpb+17, %cx /* number of root entries */
	
1:
	pushw %cx
	movw $11, %cx
	movw %bx, %di
	movw $target_file, %si
	
	repe cmpsb
	popw %cx
	je 2f
	
	decw %cx
	jz halt
	addw $32, %bx
	jmp 1b
	
2:

load_file:
	mov $load_file_str, %si
	call print

	movw %es:26(%bx), %ax /* first cluster number */
	movw $0x07e0, %bx
	movw %bx, %es
	xorw %bx, %bx
	
	movw $0x0500, %si
	
1:	/* load cluster */
	pushw %ax /* preserve cluster number */
	decw %ax
	decw %ax
	xorw %cx, %cx
	movb bpb+13, %cl /* sectors per cluster */
	mulw %cx
	
	/* restore chs address of data section */
	movw -2(%bp), %dx
	movw -4(%bp), %cx
	
	/* calculate chs of current cluster */
	call add_chs
	
	/* load cluster */
	movb bpb+13, %al /* sectors per cluster */
	call load
	jc halt
	
2:	/* calculate next target address */
	addw bpb+11, %bx /* bytes per sector */
	decb %al
	jnz 2b
	
	popw %ax /* restore cluster number */
	#movw -6(%bp), %ax
	pushw %bx /* preserve target address */
	movw %ax, %cx
	
	/* calculate offset of next cluster number in FAT */
	movw %ax, %bx
	shrw $1, %ax
	addw %ax, %bx
	
	movw (%bx, %si, 1), %ax
	testb $0x01, %cl
	jz 3f
	/* not even */
	shrw $4, %ax
	jmp 4f
3:	/* even */
	andw $0x0FFF, %ax
4:
	popw %bx
	cmpw eoc, %ax
	jne 1b
	
	addw $4, %sp
	
	pushw %es
	xorw %bx, %bx
	movw %bx, %es
	popw %bx
	
	shlw $4, %bx
	jmp *%bx

halt:
	movw $halt_str, %si
	call print
	cli
	hlt

/* parameters */
/* %si = offset address of string to be printed from segment of 0 */
print:
	pushw %ds
	pushw %ax
	
	xorw %ax, %ax
	movw %ax, %ds
	
	movb $0x0e, %ah
1:	/* loop */
	lodsb
	testb %al, %al	# if %al == 0)
	je 2f
	int $0x10
	jmp 1b
2:	/* function exit */
	popw %ax
	popw %ds
	ret

/* parameters */
/* %dx, %dl = drive, %dh = head */
/* %cx, %cl(lower 6 bits) = sector, %cl(upper 2 bits):%ch = cylinder */
/* %bx = target address offset */
/* %es = target segment */
/* %ax, %al = number of sectors to read, %ah = dont care */
/* Handles resetting drive before loading */
load:
	pushw %ax
	
	movb $0x06, reset_counter

1:	/* retry */
	decb reset_counter
	jnz 2f
	stc
	jmp 4f
	
2:	/* reset */
	pushw %si
	movw $reset_str, %si
	call print
	popw %si
	xorb %ah, %ah
	int $0x13
	jc 1b
	
3:	/* load */
	movb $0x02, %ah
	int $0x13
	jc 1b
	
4:	/* function exit */
	popw %ax
	ret

/* CHS parameters as per int 13h */
/* %ax contains number of sectors to add */
add_chs:
	pushw %bx
1:	/* seperate CHS address */
	movb %ch, %bl
	movb %cl, %bh
	shr $6, %bh
	andb $0x3f, %cl
	
	/* %bx = cylinder */
	/* %dh = head */
	/* %cl = sector */

2:	/* calculate next CHS address */
	testw %ax, %ax
	jz 3f
	decw %ax
	
	incb %cl /* increment sector of track */
	cmpb bpb+24, %cl /* sectors per track */
	jbe 2b
	
	movb $1, %cl /* reset sector */
	incb %dh /* increment head */
	cmpb bpb+26, %dh /* number of heads */
	jb 2b
	
	xorb %dh, %dh /* reset head */
	incw %bx /* increment cylinder */
	jmp 2b
	
3:	/* reconstruct CHS address */
	andb $0x3f, %cl
	movb %bl, %ch
	shl $6, %bh
	orb %bh, %cl
	
	popw %bx
	ret
	

target_file:
	.ascii "BOOTLD  SYS"

reset_counter:
	.byte 0
eoc: /* end of chain indicator */
	.word 0

reset_str:
	.asciz "->reset disk\r"
load_fat_str:
	.asciz "loading FAT\r\n"
load_root_str:
	.asciz "loading root\r\n"
locate_file_str:
	.asciz "searching for file\r\n"
load_file_str:
	.asciz "loading file\r\n"
halt_str:
	.asciz "HALTED"

	.org top+510

boot_sig:
	.byte 0x55
	.byte 0xaa
