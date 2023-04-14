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
	isa_ioctl
};

struct isa_config {
	uint16_t base_port;
};

#define TOTAL_PORTS (64 * 1024)
#define PORT_WORDS (TOTAL_PORTS / 16)
uint16_t open_ports[PORT_WORDS] = { 0 };

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

			bus = create_dev(&isa, NS8250_PORT_COUNT, parent);
			if (!bus) {
				klogs("Failed to create dev COM");
				kloglu(i, 10);
				klogc('\n');
			}
			bus->parent = NULL;
			config = kalloc(sizeof(struct isa_config));
			if (!config) {
				klogs("Failed to allocate config for isa bus\n");
				kfree(bus);
				continue;
			}
			config->base_port = bda->com_ports[i];
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
