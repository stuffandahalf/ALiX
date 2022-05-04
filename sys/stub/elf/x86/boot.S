
.section .bss
stack_top:
.skip 16384 # 16KiB
stack_bottom:

.code16

.section .text
.global _start
.type _start, @function
_start:
	#~ movw $stack_bottom > $4, %ax
	#~ movw %ax, %ss
	#~ movw $stack_bottom, %sp

	#call setup32

	cli
	hlt
1:
	jmp 1b


.size _start, . - _start

