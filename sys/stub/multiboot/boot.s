# derived from boot.s found here
# https://wiki.osdev.org/Bare_Bones

.code32

.set ALIGN, 1 << 0
.set MEMINFO, 1 << 1
.set FLAGS, ALIGN | MEMINFO
.set MAGIC, 0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bss
.align 16
stack_bottom:
.skip 16384 # 16KiB
stack_top:

mbd:
	.long 0

.section .text
.global _start
.type _start, @function
_start:
	# setup stack
	movl $stack_top, %esp
	movl %ebx, mbd

	pushl %ebx
	call init_mem

	call main

	cli
	hlt
1:
	jmp 1b

.size _start, . - _start
