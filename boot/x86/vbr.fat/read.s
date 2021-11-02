	.globl read

/ %dx:%ax = 32-bit lba
/ %bx = drive number
/ maybe %cl = nuber of sectors
/ %es:%di = destination buffer
/read:
	/ check for int13h extensions
	pushw %bx
	pushw %dx
	pushw %ax
/	pushw %di
	movb $0x41, %ah
	movb %bl, %dl
	movw $0x55aa, %bx
	int $0x13

	jc read_l

read_e:
	/ use extensions

read:
read_l:
	/ legacy read
	pushw %di
	movw %es, %di
	pushw %di
	pushw %bx
	pushw %ax
	pushw %dx

	/ get drive parameters
	movb $0x08, %ah
	movb %bl, %dl
	xorw %di, %di
	movw %di, %es
	int $0x13

	/ store number of heads in %bx
	/ pop lba off stack
	movzx %dh, %bx
	incw %bx
	popw %ax
	popw %dx

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

	andb $0x3f, %cl
	shlb $6, %ah
	orb %ah, %cl
	movb %al, %ch
	movb %dl, %dh
	popw %bx
	movb %bl, %dl
	popw %di
	movw %di, %es
	popw %bx
	movw $0x0201, %ax

	/ set up target address
	/ set up sector count
	/ call int 13h
	int $0x13

	ret

