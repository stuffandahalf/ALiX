	.code16
	.text
	.globl read

/ stack:
/ 0: return address
/ 2: 32-bit lba (little endian)
/ 6: target address
/ 8: target segment
/ 10: drive number
/ 11: # of sectors

read:
	pushw %bp
	movw %sp, %bp

	pushw %ax
	pushw %bx
	pushw %dx

	/ check for int 13h extensions
	movb $0x41, %ah
	movb 12(%bp), %dl
	movw $0x55aa, %bx
	int $0x13
	jc read_l

read_e:
	/ use extensions

	pushw %si

	movb 12(%bp), %dl
	movw $read_e_pkt, %si

	movzx 13(%bp), %ax
	movw %ax, 2(%si)
	movw 10(%bp), %ax
	movw %ax, 6(%si)
	movw 8(%bp), %ax
	movw %ax, 4(%si)
	movw 4(%bp), %ax
	movw %ax, 8(%si)
	movw 6(%bp), %ax
	movw %ax, 10(%si)

	movb $0x42, %ah
	int $0x13

	popw %si
	jmp read_exit

read_e_pkt:
	.word 0x0010
	.byte 0x00	# 16-bit sector count (lo)
	.byte 0x00	# 16-bit sector count (hi, unused)
	.word 0x0000	# target segment
	.word 0x0000	# target offset
	.long 0x00000000 # 64-bit lba (lo)
	.long 0x00000000 # 64-bit lba (hi, unused)

read_l:
	/ legacy read
	pushw %cx
	pushw %di
	movw %es, %di
	pushw %di

	/ get drive parameters
	movb $0x08, %ah
	movb 12(%bp), %dl
	xorw %di, %di
	movw %di, %es
	int $0x13

	/ store number of heads in %bx
	/ pop lba off stack
	movzx %dh, %bx
	incw %bx
	movw 4(%bp), %ax
	movw 6(%bp), %dx

	/ mask cx to sectors per track
	/ and divide lba by sectors per track
	/ then increment resulting sector number
	andw $0x003f, %cx
	divw %cx
	incw %dx

	pushw %dx
	xorw %dx, %dx
	divw %bx
	popw %cx

	/ %ax = cylinder
	/ %dx = head
	/ %cx = sector

	/ cylinder + sector
	andb $0x3f, %cl
	shlb $6, %ah
	orb %ah, %cl
	movb %al, %ch
	/ head number
	movb %dl, %dh
	/ drive number
	movb 12(%bp), %dl
	/ target segment
	movw 10(%bp), %bx
	movw %bx, %es
	/ target address
	movw 8(%bp), %bx
	/ reset counter
	movw $0x0005, %ax
	pushw %ax
1:
	/ number of sectors
	movb 13(%bp), %al
	movb $0x02, %ah

	/ call int 13h
	int $0x13

	popw %ax
	jnc 1f
	decw %ax
	jz 1f
	pushw %ax
	jmp 1b

1:
	popw %di
	movw %di, %es
	popw %di
	popw %cx

read_exit:
	popw %dx
	popw %bx
	popw %ax
	popw %bp
	ret

