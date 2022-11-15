	.code16
	.text
	.globl read

# ebx:eax = 64-bit lba
# cx = 16-bit number of sectors to read
# dl = drive number
# es:di = segment:address of destination
read:

read.check:
	pushw %ax
	pushw %bx
	movb $0x41, %ah
	movw $0x55aa, %bx
	int $0x13
	popw %bx
	popw %ax
	jc read.end

read.ext:
	movw $_pkt, %si
	movw %cx, 2(%si)
	movw %di, 4(%si)
	movw %es, 6(%si)
	movl %eax, 8(%si)
	movl %ebx, 12(%si)
	int $0x13

read.legacy:
	# TODO

read.end:
	ret

_pkt:
	.word 0x10
_pkt.nsec:
	.word 0
_pkt.offset:
	.word 0
_pkt.segment:
	.word 0
_pkt.lba:
	.long 0
	.long 0
