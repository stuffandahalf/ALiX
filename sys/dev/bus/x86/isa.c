#include <stddef.h>
#include <stdint.h>

#include <alix/dev.h>
#include <alix/device.h>
#include <alix/mem.h>
#include <alix/log.h>
#include <alix/util.h>

#include <alix/dev/class/bus.h>
#include <alix/dev/ns8250.h>
// TODO: LPT drivers
#include <alix/dev/ata.h>


static int isa_attach(dev_t parent);
static void isa_detach(dev_t parent);

static int isa_open(dev_t device, unsigned int channel, int flags);
static int isa_close(dev_t device, unsigned int channel);
static int isa_read(dev_t device, unsigned int channel, void *buffer, size_t n);
static int isa_write(dev_t device, unsigned int channel, void *buffer, size_t n);
static int isa_ioctl(dev_t device, unsigned long int request, ...);

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

//uint16_t ata_ports[] = { 0x1f0, 0x170, 0x1e8, 0x168 };
uint16_t ata_ports[][2] = {
	{ 0x1f0, 0x3f6 },
	{ 0x170, 0x376 },
	{ 0x1e8, 0x3e6 },
	{ 0x168, 0x366 }
};
size_t ata_ports_cnt = LEN(ata_ports);

#define ISA_FEATURES DEV_FALL
struct dev isa = DEV_INIT(isa, ISA_FEATURES);

// struct isa_port_width {
// 	uint8_t r;
// 	uint8_t w;
// };
// struct isa_port_config {
// 	uint16_t base_port;
// 	LIST(struct isa_port_width) port_widths;
// };
// struct isa_config {
// 	//uint16_t base_port;
// 	//uint8_t *port_widths;
// 	LIST(struct isa_port_config) regions;
// 	size_t configured_regions;
// };

struct isa_port {
	uint16_t port;
	uint8_t read_sz;
	uint8_t write_sz;
};

struct isa_config {
	uint8_t nports;
	uint8_t *open;
	struct isa_port *ports;
};

#define TOTAL_PORTS (1 << 16)
#define PORT_WORDS (TOTAL_PORTS / 16)
uint16_t open_ports[PORT_WORDS] = { 0 };

#define in_fnc(sz, nm, reg) \
static uint##sz##_t \
in##nm(uint16_t port) { \
	uint##sz##_t nm; \
	__asm__ __volatile__ ( \
		"in" #nm " %%dx, %" #reg "\n\t" \
		: "=a"(nm) \
		: "d"(port) \
	); \
	return nm; \
}

#define out_fnc(sz, nm, reg) \
static void \
out##nm(uint16_t port, uint##sz##_t data) { \
	__asm__ __volatile__ ( \
		"out" #nm " %" #reg ", %%dx\n\t" \
		: \
		: "d"(port), "a"(data) \
	); \
}

#define io_fnc(sz, nm, reg) \
	in_fnc(sz, nm, reg); \
	out_fnc(sz, nm, reg);

io_fnc(8, b, %al);
io_fnc(16, w, %ax);
io_fnc(32, l, %eax);

static void
del_cfg(struct isa_config *cfg)
{
	if (cfg) {
		kfree(cfg->open);
		kfree(cfg->ports);
	}
	kfree(cfg);
}

static struct isa_config *
new_cfg(uint8_t n)
{
	int i;
	struct isa_config *cfg = NULL;

	cfg = kalloc(sizeof(struct isa_config));
	if (!cfg) {
		del_cfg(cfg);
		return NULL;
	}
	cfg->nports = n;
	cfg->ports = NULL;
	cfg->open = NULL;

	cfg->ports = kalloc(sizeof(struct isa_port) * n);
	if (!cfg->ports) {
		del_cfg(cfg);
		return NULL;
	}

	cfg->open = kalloc(sizeof(uint8_t) * n / 8);
	if (!cfg->open) {
		del_cfg(cfg);
		return NULL;
	}

	for (i = 0; i < n; i++) {
		cfg->ports[i].port = 0;
		cfg->ports[i].read_sz = 0;
		cfg->ports[i].write_sz = 0;
		cfg->open[i / 8] &= ~(1 << (i % 8));
	}

	return cfg;
}

static int
isa_attach(dev_t parent)
{
	int i, j;
	dev_t bus, dev;
	struct isa_config *config;
	// struct isa_port_config port_config;

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
			config = new_cfg(NS8250_PORT_COUNT);
			if (!config) {
				klogs("Failed to allocate config for isa bus\n");
				destroy_dev(bus);
				continue;
			}
			for (j = 0; j < NS8250_PORT_COUNT; j++) {
				config->ports[j].port = bda->com_ports[i] + j;
				config->ports[j].read_sz = 8;
				config->ports[j].write_sz = 8;
			}

			bus->config = config;
			if (ns8250.attach(bus)) {
				del_cfg(config);
				bus->config = NULL;
				destroy_dev(bus);
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

	for (i = 0; i < ata_ports_cnt; i++) {
		bus = create_dev(&isa, 0, parent);
		if (!bus) {
			continue;
		}
		// config = kalloc(sizeof(struct isa_config));
		// if (!config) {
		// 	destroy_dev(bus);
		// 	continue;
		// }
		//config->base_port = ata_ports[i];
		// //config->port_widths = NULL;
		// LIST_INITIALIZE(&config->regions);
		// config->configured_regions = 0;

		// port_config.base_port = ata_ports[i][0];
		// LIST_INITIALIZE(&port_config.port_widths);
		// LIST_APPEND(struct isa_port_config, &config->regions, port_config);

		// port_config.base_port = ata_ports[i][1];
		// LIST_INITIALIZE(&port_config.port_widths);
		// LIST_APPEND(struct isa_port_config, &config->regions, port_config);

		// bus->config = config;
		if (ata.attach(bus)) {
			// free_isa_config(config);
			del_cfg(config);
			bus->config = NULL;
			destroy_dev(bus);
		}
	}

	return 0;
}

static void
isa_detach(dev_t parent)
{
	NOT_IMPLEMENTED();
}

// struct isa_port_config *
// port_lookup(struct isa_config *cfg, unsigned int *channel)
// {
// 	size_t i, n = 0;
// 	for (i = 0; i < cfg->regions.length; i++) {
// 		if (0 <= *channel && *channel < cfg->regions.list[i].port_widths.length) {
// 			return &cfg->regions.list[i];
// 		}
// 		*channel -= cfg->regions.list[i].port_widths.length;
// 	}

// 	return NULL;
// }

static int
isa_open(dev_t device, unsigned int channel, int flags)
{
	struct isa_config *cfg = (struct isa_config *)device->config;
	int i = channel / 8;
	int mask = 1 << (channel % 8);

	if (cfg->open[i] & mask) {
		return 1;
	}
	cfg->open[i] |= mask;
	return 0;
}

static int
isa_close(dev_t device, unsigned int channel)
{
	struct isa_config *cfg = (struct isa_config *)device->config;
	int i = channel / 8;
	int mask = 1 << (channel % 8);
	if (!(cfg->open[i] & mask)) {
		return 1;
	}

	cfg->open[i] &= ~mask;
	return 0;
}

static int
isa_read(dev_t device, unsigned int channel, void *buffer, size_t n)
{

	struct isa_config *cfg = device->config;

	uint16_t port = cfg->ports[channel].port;
	uint8_t portsz = cfg->ports[channel].read_sz / 8;

	int i = channel / 8;
	int mask = 1 << (channel % 8);
	size_t j;

	if (!(cfg->open[i] & mask)) {
		return -1;
	}

	for (j = 0; j * portsz < n; j++) {
		switch (portsz) {
		case 1:
			((uint8_t *)buffer)[j] = inb(port);
			break;
		case 2:
			((uint16_t *)buffer)[j] = inw(port);
			break;
		case 4:
			((uint32_t *)buffer)[j] = inl(port);
			break;
		default:
			return -1;
		}
	}

	return j * portsz;
}

static int
isa_write(dev_t device, unsigned int channel, void *buffer, size_t n)
{
	struct isa_config *cfg = device->config;

	uint16_t port = cfg->ports[channel].port;
	uint8_t portsz = cfg->ports[channel].write_sz / 8;

	int i = channel / 8;
	int mask = 1 << (channel % 8);
	size_t j;

	if (!(cfg->open[i] & mask)) {
		return -1;
	}

	for (j = 0; j * portsz < n; j++) {
		switch (portsz) {
		case 1:
			outb(port, ((uint8_t *)buffer)[j]);
			break;
		case 2:
			outw(port, ((uint16_t *)buffer)[j]);
			break;
		case 4:
			outl(port, ((uint32_t *)buffer)[j]);
			break;
		default:
			return -1;
		}
	}

	return j * portsz;
}

static int
isa_ioctl(dev_t device, unsigned long int request, ...)
{
	NOT_IMPLEMENTED();
	return 0;
}

#if 0
static int
isa_res_req(dev_t device, size_t nrequests, const struct resource_request *requests)
{
	size_t i, j;
	struct isa_config *cfg = (struct isa_config *)device->config;
	struct isa_port_config *port_cfg = NULL;
	struct isa_port_width port_width = { 0, 0 };
	unsigned int channel;
	for (i = 0; i < nrequests; i++) {
		switch (requests[i].type) {
		//case RESOURCE_REQUEST_REGIONS:

		// 	break;
		case RESOURCE_REQUEST_CHANNELS:
			if (cfg->configured_regions >= cfg->regions.length) {
				return 1;
			}
			port_cfg = &cfg->regions.list[cfg->configured_regions++];
			//cfg->port_widths = krealloc(cfg->port_widths, requests[i].nchannels * sizeof(uint8_t));
			/*if (!cfg->port_widths) {
				return 1;
			}*/
			for (j = 0; j < requests[i].nchannels; j++) {
				LIST_APPEND(struct isa_port_width, &port_cfg->port_widths, port_width);
			}
			device->nchannels += requests[i].nchannels;
			break;
		case RESOURCE_REQUEST_CHANNEL_SIZE:
			channel = requests[i].channelsz.channel;
			port_cfg = port_lookup(cfg, &channel);
			if (!port_cfg->port_widths.length || port_cfg->port_widths.length <= channel) {
				return 1;
			}
			if (requests[i].channelsz.size != 32 && requests[i].channelsz.size != 16 && requests[i].channelsz.size != 8) {
				return 1;
			}
			if (requests[i].channelsz.channel >= device->nchannels) {
				return 1;
			}

			if (requests[i].channelsz.mode & RESOURCE_REQUEST_CHANNEL_SIZE_R) {
				port_cfg->port_widths.list[channel].r = requests[i].channelsz.size;
			}
			if (requests[i].channelsz.mode & RESOURCE_REQUEST_CHANNEL_SIZE_W) {
				port_cfg->port_widths.list[channel].w = requests[i].channelsz.size;
			}
			break;
		default:
			return 1;
		}
	}
	return 0;
}
#endif
