	.code16
	.text

/ on stack, pointer to data packet
read:
	pushw %bp
	movw %sp, %bp
	pushw %bx
	pushw %si
	movw 4(%bp), %si

	movb $0x41, %ah
	movw $0x55aa, %bx
	int $0x13
	jc 1f

	/ lba
	movb $0x42, %ah
	int $0x13
	
	popw %si
	popw %bx
	popw %bp
	ret

1:	/ chs
	/ get drive parameters
	pushw %dx
	movb $0x08, %ah
	xorw %di, %di
	movw %di, %es
	int $0x13

	/ convert lba into chs
	andw $0x003f, %cx
	/xorb %bh, %bh
	/movb %dh, %bl
	movzxb %dh, %bx
	incw %bx

	/ %cx = sectors per track
	/ %bx = number of heads

	movl 8(%si), %eax
	movl 12(%si), %edx
	/andl $0x000000ff, %ecx
	movzxb %cl, %ecx
	divl %ecx
	incl %edx
	pushl %edx

	movw %ax, %dx
	shrl $0x0000000a, %eax
	xchgw %ax, %dx
	divw %bx

	popl %ebx

	/ %ebx = sector
	/ %ax = cylinder
	/ %dx = head

	movb %bl, %cl
	andb $0x3f, %cl
	movb %al, %ch
	shlb $0x06, %ah
	orb %ah, %cl
	popw %bx	# retrieve drive number
	movb %dl, %dh
	movb %bl, %dl

	movw 6(%si), %bx
	movw %bx, %es
	movw 4(%si), %bx
	movb 2(%si), %al
	movb $0x04, %ah

1:	/ reset
	testb %ah, %ah
	jz 2f
	decb %ah
	pushw %ax

	xorb %ah, %ah
	int $0x13

	movb $0x02, %ah
	int $0x13

	jnc 3f
	popw %ax
	jmp 1b

2:	/ fail
	stc
3:	/ exit
	popw %ax
	popw %si
	popw %bx
	popw %bp
	ret

