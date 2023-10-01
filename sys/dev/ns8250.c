#include <stddef.h>
#include <stdint.h>

#include <alix/dev.h>
#include <alix/device.h>
#include <alix/log.h>
#include <alix/mem.h>
#include <alix/util.h>

#include <alix/dev/class/bus.h>
#include <alix/dev/class/tty.h>
#include <alix/dev/ns8250.h>

static int ns8250_attach(dev_t parent);
static void ns8250_detach(dev_t parent);

static int ns8250_open(dev_t device, unsigned int channel, int flags);
static int ns8250_close(dev_t device, unsigned int channel);
static int ns8250_read(dev_t device, unsigned int channel, void *buf, size_t n);
static int ns8250_write(dev_t device, unsigned int channel, void *buf, size_t n);
static int ns8250_ioctl(dev_t device, unsigned long int request, void **args, size_t nargs);

#define NS8250_FEATURES DEV_FALL
struct dev ns8250 = DEV_INIT3(ns8250, ttyNS, NS8250_FEATURES);

#if 0
static const struct resource_request init_reqs[] = {
	{
		.type = RESOURCE_REQUEST_CHANNELS,
		.nchannels = NS8250_PORT_COUNT
	},
	{
		.type = RESOURCE_REQUEST_CHANNEL_SIZE,
		.channelsz = {
			.channel = 0,
			.size = 8,
			.mode = RESOURCE_REQUEST_CHANNEL_SIZE_R | RESOURCE_REQUEST_CHANNEL_SIZE_W
		}
	},
	{
		.type = RESOURCE_REQUEST_CHANNEL_SIZE,
		.channelsz = {
			.channel = 1,
			.size = 8,
			.mode = RESOURCE_REQUEST_CHANNEL_SIZE_R | RESOURCE_REQUEST_CHANNEL_SIZE_W
		}
	},
	{
		.type = RESOURCE_REQUEST_CHANNEL_SIZE,
		.channelsz = {
			.channel = 2,
			.size = 8,
			.mode = RESOURCE_REQUEST_CHANNEL_SIZE_R | RESOURCE_REQUEST_CHANNEL_SIZE_W
		}
	},
	{
		.type = RESOURCE_REQUEST_CHANNEL_SIZE,
		.channelsz = {
			.channel = 3,
			.size = 8,
			.mode = RESOURCE_REQUEST_CHANNEL_SIZE_R | RESOURCE_REQUEST_CHANNEL_SIZE_W
		}
	},
	{
		.type = RESOURCE_REQUEST_CHANNEL_SIZE,
		.channelsz = {
			.channel = 4,
			.size = 8,
			.mode = RESOURCE_REQUEST_CHANNEL_SIZE_R | RESOURCE_REQUEST_CHANNEL_SIZE_W
		}
	},
	{
		.type = RESOURCE_REQUEST_CHANNEL_SIZE,
		.channelsz = {
			.channel = 5,
			.size = 8,
			.mode = RESOURCE_REQUEST_CHANNEL_SIZE_R | RESOURCE_REQUEST_CHANNEL_SIZE_W
		}
	},
	{
		.type = RESOURCE_REQUEST_CHANNEL_SIZE,
		.channelsz = {
			.channel = 6,
			.size = 8,
			.mode = RESOURCE_REQUEST_CHANNEL_SIZE_R | RESOURCE_REQUEST_CHANNEL_SIZE_W
		}
	},
	{
		.type = RESOURCE_REQUEST_CHANNEL_SIZE,
		.channelsz = {
			.channel = 7,
			.size = 8,
			.mode = RESOURCE_REQUEST_CHANNEL_SIZE_R | RESOURCE_REQUEST_CHANNEL_SIZE_W
		}
	}
};
static const size_t init_reqs_sz = LEN(init_reqs);
#endif

// static const uint8_t ns8250_port_sizes[NS8250_PORT_COUNT] = { 8, 8, 8, 8, 8, 8, 8 };

static int
ns8250_attach(dev_t parent)
{
	dev_t port;
	char c;
	int i;
	// if (parent->driver->res_req(parent, init_reqs_sz, init_reqs)) {
	// 	return 1;
	// }

	port = create_dev(&ns8250, 1, parent);
	if (!port) {
		return 1;
	}
	port->config = kalloc(sizeof(struct tty));
	if (!port->config) {
		klogs("Failed to allocate tty");
		destroy_dev(port);
		return 1;
	}

	// for (i = 0; i < NS8250_PORT_COUNT; i++) {
	// 	DEV_PARENT_IOCTL(port, BUS_IOCTL_CHANNEL_SZ, 8);
	// }

#if 0
	ns8250_open(port, 0, 0);
	for (;;) {
		ns8250_read(port, 0, &c, 1);
		ns8250_write(port, 0, &c, 1);
	}
	ns8250_close(port, 0);
#endif

	return 0;
}

static void
ns8250_detach(dev_t device)
{
	kfree(device->config);
	destroy_dev(device);
}


static inline int
bwrite(dev_t device, unsigned int channel, uint8_t byte)
{
	return DEV_PARENT_WRITE(device, channel, &byte, sizeof(byte));
}

static int
ns8250_open(dev_t device, unsigned int channel, int flags)
{
	int i, r;
	uint8_t byte;
	if (channel != 0) {
		return 1;
	}

	/* open all required parent channels */
	for (i = 0; i < NS8250_PORT_COUNT; i++) {
		if (DEV_PARENT_OPEN(device, i, 0)) {
			return 1;
		}
	}

	/* initialize device */
	// ((struct tty *)device->config)->baud = 9600;

	/* disable interrupts */
	if (bwrite(device, NS8250_PORT_INT_ENABLE, 0) < 1) return 1;
	
	/* enable DLAB */
	if (bwrite(device, NS8250_PORT_LINE_CTRL, 0x80) < 1) return 1;

	/* set divisor to 3, 38400 baud */
	if (bwrite(device, NS8250_PORT_DIV_LO, 0x03) < 1) return 1;
	if (bwrite(device, NS8250_PORT_DIV_HI, 0x00) < 1) return 1;

	/* 8 bytes, no parity, 1 stop bit */
	byte = NS8250_LINE_CTRL_BITS_8 | NS8250_LINE_CTRL_PARITY_NONE | NS8250_LINE_CTRL_STOP_BITS_1;
	if (bwrite(device, NS8250_PORT_LINE_CTRL, byte) < 1) return 1;

	/* enable interrupts */
	if (bwrite(device, NS8250_PORT_INT_ID, 0xc7) < 1) return 1;

	byte = NS8250_MODEM_CTRL_OUT2 | NS8250_MODEM_CTRL_OUT1 | NS8250_MODEM_CTRL_RTS | NS8250_MODEM_CTRL_DTR;
	if (bwrite(device, NS8250_PORT_MODEM_CTRL, byte) < 1) return 1;
	return 0;
}

static int
ns8250_close(dev_t device, unsigned int channel)
{
	int i;

	if (channel != 0) {
		return 1;
	}

	for (i = 0; i < NS8250_PORT_COUNT; i++) {
		DEV_PARENT_CLOSE(device, i);
	}

	return 0;
}

static int
ns8250_read(dev_t device, unsigned int channel, void *buf, size_t n)
{
	uint8_t byte;
	size_t i;
	int ready = 0;

	for (i = 0; i < n; i++) {
		ready = 0;
		do {
			DEV_PARENT_READ(device, NS8250_PORT_LINE_STATUS, &byte, 1);
			ready = byte & NS8250_LINE_STATUS_DR;
		} while (!ready);
		DEV_PARENT_READ(device, NS8250_PORT_DATA, &((uint8_t *)buf)[i], 1);
	}

	return i;
}

/* TODO: implement tty logic */
static int
ns8250_write(dev_t device, unsigned int channel, void *buf, size_t n)
{
	uint8_t byte;
	size_t i;
	int r, ready;

	for (i = 0; i < n; i++) {
		ready = 0;
		do {
			DEV_PARENT_READ(device, NS8250_PORT_LINE_STATUS, &byte, 1);
			ready = byte & NS8250_LINE_STATUS_THRE;
		} while (!ready);
		if (DEV_PARENT_WRITE(device, 0, (uint8_t *)buf + i, 1) != 1) {
			return i;
		}
	}
	return n;
}

static int
ns8250_ioctl(dev_t device, unsigned long int request, void **args, size_t nargs)
{
	NOT_IMPLEMENTED();
	return 0;
}
