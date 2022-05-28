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

.section .text
.global _start
.type _start, @function
_start:
	# setup stack
	movl $stack_top, %esp

	pushl %ebx
	call setup32
	popl %ebx

	call main

	cli
	hlt
1:
	jmp 1b

.size _start, . - _start
/*
gdtr:
	.word 0
	.long 0

gdt_start:
null:
	.long 0, 0
code:
	.word 0xffff
	.word 0x0000
	.byte 0x00
	.byte 0x9a
	.byte 0b11001111
	.byte 0x00
data:
	.word 0xffff
	.word 0x0000
	.byte 0x00
	.byte 0x92
	.byte 0b11001111
	.byte 0x00
gdt_end:
*/

.global outb
.type outb, @function
outb:
	pushl %ebp
	movl %esp, %ebp

	movw 8(%ebp), %dx
	movb 12(%ebp), %al
	outb %al, %dx

	popl %ebp
	ret
.size outb, . - outb

.global inb
.type inb, @function
inb:
	pushl %ebp
	movl %esp, %ebp

	movw 8(%ebp), %dx
	inb %dx, %al

	popl %ebp
	ret
.size inb, . - inb
