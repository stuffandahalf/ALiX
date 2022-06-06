#include <stdint.h>

void
outb(uint16_t port, uint8_t byte)
{
	__asm__ __volatile__ (
		"outb %%al, %%dx\n\t"
		:
		: "d"(port), "a"(byte)
	);
}

void
outw(uint16_t port, uint16_t word)
{
	__asm__ __volatile__ (
		"outw %%ax, %%dx\n\t"
		:
		: "d"(port), "a"(word)
	);
}

void
outl(uint16_t port, uint32_t lword)
{
	__asm__ __volatile__ (
		"outl %%eax, %%dx\n\t"
		:
		: "d"(port), "a"(lword)
	);
}

uint8_t
inb(uint16_t port)
{
	uint8_t byte;
	__asm__ __volatile__ (
		"inb %%dx, %%al\n\t"
		: "=a"(byte)
		: "d"(port)
	);
	return byte;
}

uint16_t
inw(uint16_t port)
{
	uint16_t word;
	__asm__ __volatile__ (
		"inw %%dx, %%ax\n\t"
		: "=a"(word)
		: "d"(port)
	);
	return word;
}

uint32_t
inl(uint16_t port)
{
	uint32_t lword;
	__asm__ __volatile__ (
		"inl %%dx, %%eax\n\t"
		: "=a"(lword)
		: "d"(port)
	);
	return lword;
}
