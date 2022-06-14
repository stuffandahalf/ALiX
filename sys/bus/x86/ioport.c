#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <alix/bus.h>

static int x86_io_send(bus_target_t target, int size, ...);
static int x86_io_receive(bus_target_t target, int size, ...);

struct bus x86_ioport = {
	NULL,
	NULL,
	NULL,
	"ioport",
	x86_io_send,
	x86_io_receive
};

static inline void
outb(uint16_t port, uint8_t byte)
{
	__asm__ __volatile__ (
		"outb %%al, %%dx\n\t"
		:
		: "d"(port), "a"(byte)
	);
}

static inline void
outw(uint16_t port, uint16_t word)
{
	__asm__ __volatile__ (
		"outw %%ax, %%dx\n\t"
		:
		: "d"(port), "a"(word)
	);
}

static inline void
outl(uint16_t port, uint32_t lword)
{
	__asm__ __volatile__ (
		"outl %%eax, %%dx\n\t"
		:
		: "d"(port), "a"(lword)
	);
}

static inline uint8_t
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

static inline uint16_t
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

static inline uint32_t
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

static int
x86_io_send(bus_target_t target, int size, ...)
{
	int exit = 0;
	uint32_t value;
	va_list args;

	va_start(args, size);
	switch (size) {
	case BUS_PAYLOAD_SIZE_8:
		value = va_arg(args, uint32_t) & 0xff;
		outb(target, value);
		break;
	case BUS_PAYLOAD_SIZE_16:
		value = va_arg(args, uint32_t) & 0xffff;
		outw(target, value);
		break;
	case BUS_PAYLOAD_SIZE_32:
		value = va_arg(args, uint32_t);
		outl(target, value);
		break;
	default:
		exit = 1;
		break;
	}
	va_end(args);

	return exit;
}

static int
x86_io_receive(bus_target_t target, int size, ...)
{
	int exit = 0;
	union {
		uint32_t *u32;
		uint16_t *u16;
		uint8_t *u8;
		void *ptr;
	} response;
	va_list args;

	va_start(args, size);

	response.ptr = va_arg(args, void *);

	switch (size) {
	case BUS_PAYLOAD_SIZE_8:
		*response.u8 = inb(target);
		break;
	case BUS_PAYLOAD_SIZE_16:
		*response.u16 = inw(target);
		break;
	case BUS_PAYLOAD_SIZE_32:
		*response.u32 = inl(target);
		break;
	default:
		exit = 1;
	}

	va_end(args);
	return exit;
}
