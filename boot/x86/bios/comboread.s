read:
	pushw %ax
	call test_lba
	jc 1f
	call lbaread
	jmp 2f
1:
	call chsread
2:
	popw %ax
	ret

	.include "../bios/chsread.s"
	.include "../bios/lbaread.s"
