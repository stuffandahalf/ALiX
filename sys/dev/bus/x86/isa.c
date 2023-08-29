#include <stddef.h>
#include <stdint.h>

#include <alix/dev.h>
#include <alix/device.h>
#include <alix/mem.h>
#include <alix/log.h>
#include <alix/util.h>

#include <alix/dev/ns8250.h>

static int isa_attach(dev_t parent);
static void isa_detach(dev_t parent);

static int isa_open(dev_t device, unsigned int channel, int flags);
static int isa_close(dev_t device, unsigned int channel);
static int isa_read(dev_t device, unsigned int channel, void *buffer, size_t n);
static int isa_write(dev_t device, unsigned int channel, void *buffer, size_t n);
static void isa_ioctl(dev_t device);
static int isa_resource_request(dev_t device, size_t nrequests, const struct resource_request *requests);

#define BDA_ADDR ((void *)0x0400)
#define BDA_COMPORTS_SZ 4
#define BDA_LPTPORTS_SZ 3
#define BDA_KBDBUF_SZ 32
static const struct bios_data_area {
	uint16_t com_ports[BDA_COMPORTS_SZ];
	uint16_t lpt_ports[BDA_LPTPORTS_SZ];
	uint16_t ebda_addr;
	uint16_t hw_flags;
	uint16_t resrv_mem;
	uint16_t kbd_flags;
	uint8_t kbd_buf[BDA_KBDBUF_SZ];
	uint8_t vid_mode;
	uint8_t vid_cols;
	uint16_t vid_port;
	uint16_t boot_tics;
	uint8_t disk_count;
	uint16_t kbd_buf_start;
	uint16_t kbd_buf_end;
	uint8_t kbd_state;
} __attribute__((__packed__)) *bda = BDA_ADDR;

struct dev isa = {
	"isa",

	LIST_INIT,

	isa_attach,
	isa_detach,

	isa_open,
	isa_close,
	isa_read,
	isa_write,
	isa_ioctl,

	isa_resource_request,
};


struct isa_config {
	uint16_t base_port;
	uint8_t *port_widths;
};

#define TOTAL_PORTS (1 << 16)
#define PORT_WORDS (TOTAL_PORTS / 16)
uint16_t open_ports[PORT_WORDS] = { 0 };

#define in_fnc(sz, nm, reg) \
static uint##sz##_t \
in##nm(uint16_t port) { \
	uint##sz##_t nm; \
	__asm__ __volatile__ ( \
		"in" #nm "%%dx, %" #reg "\n\t" \
		: "=a"(nm) \
		: "d"(port) \
	); \
	return nm; \
}

in_fnc(8, b, %al);
in_fnc(16, w, %ax);
in_fnc(32, l, %eax);

#define out_fnc(sz, nm, reg) \
static void \
out##nm(uint16_t port, uint##sz##_t data) { \
	__asm__ __volatile__ ( \
		"out" #nm "%%dx, %" #reg "\n\t" \
		: \
		: "d"(port), "a"(data) \
	); \
}

out_fnc(8, b, %al);
out_fnc(16, w, %ax);
out_fnc(32, l, %eax);

static int
isa_attach(dev_t parent)
{
	int i;
	dev_t bus, dev;
	struct isa_config *config;

	for (i = 0; i < BDA_COMPORTS_SZ; i++) {
		if (bda->com_ports[i]) {
			// klogf("COM%d: 0x%x\n", i, bda->com_ports[i]);
			klogs("COM");
			kloglu(i, 10);
			klogs(": 0x");
			kloglu(bda->com_ports[i], 16);
			klogc('\n');

			bus = create_dev(&isa, 0, parent);
			if (!bus) {
				klogs("Failed to create dev COM");
				kloglu(i, 10);
				klogc('\n');
				continue;
			}
			bus->parent = NULL;
			config = kalloc(sizeof(struct isa_config));
			if (!config) {
				klogs("Failed to allocate config for isa bus\n");
				kfree(bus);
				continue;
			}
			config->base_port = bda->com_ports[i];
			config->port_widths = NULL;
			bus->config = config;
			if (ns8250.attach(bus)) {
				kfree(config);
				kfree(bus);
				continue;
			}
		}
	}

	for (i = 0; i < BDA_LPTPORTS_SZ; i++) {
		if (bda->lpt_ports[i]) {
			klogs("LPT");
			kloglu(i, 10);
			klogs(": 0x");
			kloglu(bda->lpt_ports[i], 16);
			klogc('\n');
		}
	}
	// d = create_dev(&isa);

	return 0;
}

static void
isa_detach(dev_t parent)
{
	NOT_IMPLEMENTED();
}

static int
isa_open(dev_t device, unsigned int channel, int flags)
{
	unsigned long int port = ((struct isa_config *)device->config)->base_port + channel;
	int i = port / PORT_WORDS;
	int mask = port % PORT_WORDS;
	mask = 1 << mask;
	if (open_ports[i] & mask) {
		return 1;
	}
	open_ports[i] |= mask;
	return 0;
}

static int
isa_close(dev_t device, unsigned int channel)
{
	// NOT_IMPLEMENTED();
	unsigned long int port = ((struct isa_config *)device->config)->base_port + channel;
	int i = port / PORT_WORDS;
	int mask = port % PORT_WORDS;
	mask = 1 << mask;
	if (!(open_ports[i] & mask)) {
		return 1;
	}
	open_ports[i] &= ~mask;
	return 0;
}


static int
isa_read(dev_t device, unsigned int channel, void *buffer, size_t n)
{
	NOT_IMPLEMENTED();
	return 0;
}

static int
isa_write(dev_t device, unsigned int channel, void *buffer, size_t n)
{
	NOT_IMPLEMENTED();
	return 0;
}

static void
isa_ioctl(dev_t device)
{
	NOT_IMPLEMENTED();
}

static int
isa_resource_request(dev_t device, size_t nrequests, const struct resource_request *requests)
{
	size_t i, j;
	struct isa_config *cfg = (struct isa_config *)device->config;
	for (i = 0; i < nrequests; i++) {
		switch (requests[i].type) {
		case RESOURCE_REQUEST_CHANNELS:
			cfg->port_widths = krealloc(cfg->port_widths, requests[i].nchannels * sizeof(uint8_t));
			if (!cfg->port_widths) {
				return 1;
			}
			for (j = 0; j < requests[i].nchannels; j++) {
				cfg->port_widths[j] = 0;
			}
			device->nchannels = requests[i].nchannels;
			break;
		case RESOURCE_REQUEST_CHANNEL_SIZE:
			if (!cfg->port_widths) {
				return 1;
			}
			if (requests[i].channelsz.size != 32 && requests[i].channelsz.size != 16 && requests[i].channelsz.size != 8) {
				return 1;
			}
			if (requests[i].channelsz.channel >= device->nchannels) {
				return 1;
			}
			cfg->port_widths[requests[i].channelsz.channel] = requests[i].channelsz.size;
			break;
		default:
			return 1;
		}
	}
	return 0;
}
