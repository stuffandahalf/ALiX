#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <alix/bus.h>

static int x86_io_send(bus_target_t target, struct bus_message *message);
static int x86_io_receive(bus_target_t target, struct bus_message *message);

struct bus x86_ioport_bus = {
	"ioport",
	NULL,
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
x86_io_send(bus_target_t target, struct bus_message *message)
{
	int exit = 0;

	switch (BUS_MESSAGE_TYPE(message)) {
	case BUS_MESSAGE_INT:
		switch (BUS_MESSAGE_INT_SZ(message)) {
		case BUS_MESSAGE_INT8:
			outb(target, message->i);
			break;
		case BUS_MESSAGE_INT16:
			outw(target, message->i);
			break;
		case BUS_MESSAGE_INT32:
			outl(target, message->i);
			break;
		default:
			exit = 1;
			break;
		}
		break;
	default:
		exit = 1;
		break;
	}

	return exit;
}

static int
x86_io_receive(bus_target_t target, struct bus_message *message)
{
	int exit = 0;

	switch (BUS_MESSAGE_TYPE(message)) {
	case BUS_MESSAGE_INT:
		switch (BUS_MESSAGE_INT_SZ(message)) {
		case BUS_MESSAGE_INT8:
			message->i = inb(target);
			break;
		case BUS_MESSAGE_INT16:
			message->i = inw(target);
			break;
		case BUS_MESSAGE_INT32:
			message->i = inl(target);
			break;
		default:
			exit = 1;
			break;
		}
		break;
	/* TODO: Consider allowing support for block messages */
	/*case BUS_MESSAGE_BLOCK:

		break;*/
	default:
		exit = 1;
	}

	return exit;
}
