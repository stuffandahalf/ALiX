# derived from boot.s found here
# https://wiki.osdev.org/Bare_Bones

#define ASM_FILE 1
#include <multiboot.h>

.code32

#define FLAGS (MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO)
#define CHECKSUM (-(MULTIBOOT_HEADER_MAGIC + FLAGS))

.section .multiboot
.align 4
.long MULTIBOOT_HEADER_MAGIC
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
	call setup32

	call main

	cli
	hlt
1:
	jmp 1b

.size _start, . - _start
