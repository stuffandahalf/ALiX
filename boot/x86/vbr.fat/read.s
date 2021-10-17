/ %dx:%ax = 32-bit lba
/ %bx = drive number
/ maybe %cx = nuber of sectors
/ %es:%di = destination buffer
read:
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

read_m:
	/ use extensions


read_l:
	/ legacy read

	/ get drive parameters
	pushw %di
	movb $0x08, %ah
	xorw %di, %di
	movw %di, %es
	int $0x13
	popw %di

	/ pop lba off stack
	movw %dx, %bx
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
	movb %bh, %bl
	xorb %bh, %bh
/	incb %bl
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
	movw $0x0201, %ax
	
	/ set up target address
	/ set up sector count
	/ call int 13h
	movw %di, %bx
	int $0x13

	ret

