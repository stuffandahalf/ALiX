#include <stddef.h>
#include <stdint.h>

#include <alix/dev.h>
#include <alix/device.h>
#include <alix/log.h>
#include <alix/mem.h>
#include <alix/util.h>

#include <alix/dev/class/tty.h>
#include <alix/dev/ns8250.h>

static int ns8250_attach(dev_t parent);
static void ns8250_detach(dev_t parent);

static int ns8250_open(dev_t device, unsigned int channel, int flags);
static int ns8250_close(dev_t device, unsigned int channel);
static int ns8250_read(dev_t device, unsigned int channel, void *buf, size_t n);
static int ns8250_write(dev_t device, unsigned int channel, void *buf, size_t n);
static void ns8250_ioctl(dev_t device);

struct dev ns8250 = {
	"ttyNS",

	LIST_INIT,

	ns8250_attach,
	ns8250_detach,

	ns8250_open,
	ns8250_close,
	ns8250_read,
	ns8250_write,
	ns8250_ioctl,

	NULL
};

static const struct resource_request init_reqs[] = {
	{
		.type = RESOURCE_REQUEST_CHANNELS,
		.nchannels = NS8250_PORT_COUNT
	},
	{
		.type = RESOURCE_REQUEST_CHANNEL_SIZE,
		.channelsz = { .channel = 0, .size = 8 }
	},
	{
		.type = RESOURCE_REQUEST_CHANNEL_SIZE,
		.channelsz = { .channel = 1, .size = 8 }
	},
	{
		.type = RESOURCE_REQUEST_CHANNEL_SIZE,
		.channelsz = { .channel = 2, .size = 8 }
	},
	{
		.type = RESOURCE_REQUEST_CHANNEL_SIZE,
		.channelsz = { .channel = 3, .size = 8 }
	},
	{
		.type = RESOURCE_REQUEST_CHANNEL_SIZE,
		.channelsz = { .channel = 4, .size = 8 }
	},
	{
		.type = RESOURCE_REQUEST_CHANNEL_SIZE,
		.channelsz = { .channel = 5, .size = 8 }
	},
	{
		.type = RESOURCE_REQUEST_CHANNEL_SIZE,
		.channelsz = { .channel = 6, .size = 8 }
	},
	{
		.type = RESOURCE_REQUEST_CHANNEL_SIZE,
		.channelsz = { .channel = 7, .size = 8 }
	}
};
static const size_t init_reqs_sz = sizeof(init_reqs) / sizeof(struct resource_request);

static int
ns8250_attach(dev_t parent)
{
	dev_t port;
	if (parent->driver->res_req(parent, init_reqs_sz, init_reqs)) {
		return 1;
	}

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

	return 0;
}

static void
ns8250_detach(dev_t device)
{
	NOT_IMPLEMENTED();
	destroy_dev(device);
}

static int
ns8250_open(dev_t device, unsigned int channel, int flags)
{
	int i;
	char byte;
	if (channel != 0) {
		return 1;
	}

	/* open all required parent channels */
	for (i = 0; i < NS8250_PORT_COUNT; i++) {
		DEV_PARENT_OPEN(device, i, 0);
	}

	/* initialize device */
	// ((struct tty *)device->config)->baud = 9600;
	byte = NS8250_LINE_CTRL_BITS_8 | NS8250_LINE_CTRL_PARITY_NONE | NS8250_LINE_CTRL_STOP_BITS_1;
	DEV_PARENT_WRITE(device, NS8250_PORT_LINE_CTRL, &byte, 1);

	return 0;
}

static int
ns8250_close(dev_t device, unsigned int channel)
{
	NOT_IMPLEMENTED();
	return 0;
}

static int
ns8250_read(dev_t device, unsigned int channel, void *buf, size_t n)
{
	NOT_IMPLEMENTED();
	return 0;
}

static int
ns8250_write(dev_t device, unsigned int channel, void *buf, size_t n)
{
	NOT_IMPLEMENTED();
	return 0;
}

static void
ns8250_ioctl(dev_t device)
{
	NOT_IMPLEMENTED();
}
