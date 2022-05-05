	.code16
	
buffer = 0x8000

	# %al = drive number
	# %edx:%eax = 64-bit lba
	# %es:%di = destination
lbaread:
	pushal

	movw $lba_packet, %si

	movw %es, 4(%si)
	movw %di, 6(%si)
	movl %eax, 8(%si)
	movl %edx, 12(%si)


	popal
	ret

	# %dl = drive index
	# sets carry flag when not available
test_lba:
	pushw %ax
	pushw %bx

	movw $0x55aa, %bx
	movb $0x41, %ah
	int $0x13

	popw %bx
	popw %ax
	ret

lba_packet:
	.word 0x0010 # packet size
	.word 0x0001 # number of sectors
	.word 0x0000 # target offset
	.word buffer >> 4 # target segment
	.long 0x00000000 # 64-bit lba (lo)
	.long 0x00000000 # 64-bit lba (hi), unused
