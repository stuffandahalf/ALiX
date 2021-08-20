	.code16
	.text

SUPERBLOCK_ADDR = 500
SUPERBLOCK_MAGIC = 1024

top:
	cli
	xorw %ax, %ax
	movw %ax, %ds
	movw %ax, %ss
	movw $top, %sp
	
	ljmp $0, $_start

	.globl _start
_start:
	sti
	movw $str, %si
	call print
	
	cli
	hlt
	
print:
	pushw %ax
	movb $0x0e, %ah
1:
	lodsb
	testb %al, %al
	jz 2f
	int $0x10
	jmp 1b
2:
	popw %ax
	ret

load_sectors:
	

str:
	.asciz "Hello World!\r\n"

	.org top+510
/	.org top+446
/part0:
/	.zero 16
/part1:
/	.zero 16
/part2:
/	.zero 16
/part3:
/	.zero 16
	
boot_sig:
	.byte 0x55, 0xaa

bottom:
