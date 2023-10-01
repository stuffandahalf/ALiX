#include <stddef.h>
#include <stdint.h>

#include <alix/dev.h>
#include <alix/device.h>
#include <alix/log.h>
#include <alix/mem.h>
#include <alix/util.h>

#include <alix/dev/ata.h>

static int ata_attach(dev_t parent);
static void ata_detach(dev_t parent);

static int ata_open(dev_t device, unsigned int channel, int flags);
static int ata_close(dev_t device, unsigned int channel);
static int ata_read(dev_t device, unsigned int channel, void *buf, size_t n);
static int ata_write(dev_t device, unsigned int channel, void *buf, size_t n);
static int ata_ioctl(dev_t device, unsigned long int request, void **args, size_t nargs);

static int ata_identify(dev_t parent, uint8_t drvsel, uint16_t *buffer);

#define ATA_FEATURES DEV_FALL
struct dev ata = DEV_INIT(ata, ATA_FEATURES);

#if 0
static const struct resource_request init_reqs[] = {
	{
		.type = RESOURCE_REQUEST_CHANNELS,
		.nchannels = ATA_IO_PORT_COUNT
	},
	{
		.type = RESOURCE_REQUEST_CHANNELS,
		.nchannels = ATA_CTRL_PORT_COUNT
	},
	{
		.type = RESOURCE_REQUEST_CHANNEL_SIZE,
		.channelsz = {
			.channel = ATA_PORT_DATA,
			.size = 16,
			.mode = RESOURCE_REQUEST_CHANNEL_SIZE_R | RESOURCE_REQUEST_CHANNEL_SIZE_W
		}
	},
	{
		.type = RESOURCE_REQUEST_CHANNEL_SIZE,
		.channelsz = {
			.channel = ATA_PORT_ERROR,
			.size = 8,
			.mode = RESOURCE_REQUEST_CHANNEL_SIZE_R | RESOURCE_REQUEST_CHANNEL_SIZE_W
		}
	},
	{
		.type = RESOURCE_REQUEST_CHANNEL_SIZE,
		.channelsz = {
			.channel = ATA_PORT_SEC_COUNT,
			.size = 8,
			.mode = RESOURCE_REQUEST_CHANNEL_SIZE_R | RESOURCE_REQUEST_CHANNEL_SIZE_W
		}
	},
	{
		.type = RESOURCE_REQUEST_CHANNEL_SIZE,
		.channelsz = {
			.channel = ATA_PORT_SEC_NUM,
			.size = 8,
			.mode = RESOURCE_REQUEST_CHANNEL_SIZE_R | RESOURCE_REQUEST_CHANNEL_SIZE_W
		}
	},
	{
		.type = RESOURCE_REQUEST_CHANNEL_SIZE,
		.channelsz = {
			.channel = ATA_PORT_CYL_LO,
			.size = 8,
			.mode = RESOURCE_REQUEST_CHANNEL_SIZE_R | RESOURCE_REQUEST_CHANNEL_SIZE_W
		}
	},
	{
		.type = RESOURCE_REQUEST_CHANNEL_SIZE,
		.channelsz = {
			.channel = ATA_PORT_CYL_HI,
			.size = 8,
			.mode = RESOURCE_REQUEST_CHANNEL_SIZE_R | RESOURCE_REQUEST_CHANNEL_SIZE_W
		}
	},
	{
		.type = RESOURCE_REQUEST_CHANNEL_SIZE,
		.channelsz = {
			.channel = ATA_PORT_DRV_HEAD,
			.size = 8,
			.mode = RESOURCE_REQUEST_CHANNEL_SIZE_R | RESOURCE_REQUEST_CHANNEL_SIZE_W
		}
	},
	{
		.type = RESOURCE_REQUEST_CHANNEL_SIZE,
		.channelsz = {
			.channel = ATA_PORT_STATUS,
			.size = 8,
			.mode = RESOURCE_REQUEST_CHANNEL_SIZE_R | RESOURCE_REQUEST_CHANNEL_SIZE_W
		}
	}
};
static const size_t init_reqs_sz = LEN(init_reqs);
#endif

static int
ata_attach(dev_t parent)
{
	uint16_t identifybuf[256];
	if (!ata_identify(parent, ATA_DRIVE_SEL_MASTER, identifybuf)) {
		
		/* TODO: setup master drive */
	}
	for (;;);
	if (!ata_identify(parent, ATA_DRIVE_SEL_SLAVE, identifybuf)) {
		/* TODO: setup slave drive */
	}

	return 1;
}

static void
ata_detach(dev_t parent)
{
	NOT_IMPLEMENTED();
}

static int
ata_open(dev_t device, unsigned int channel, int flags)
{
	NOT_IMPLEMENTED();
	return 1;
}

static int
ata_close(dev_t device, unsigned int channel)
{
	NOT_IMPLEMENTED();
	return 1;
}

static int
ata_read(dev_t device, unsigned int channel, void *buf, size_t n)
{
	NOT_IMPLEMENTED();
	return 0;
}

static int
ata_write(dev_t device, unsigned int channel, void *buf, size_t n)
{
	NOT_IMPLEMENTED();
	return 0;
}

static int
ata_ioctl(dev_t device, unsigned long int request, void **args, size_t nargs)
{
	NOT_IMPLEMENTED();
	return 0;
}


/* drive commands */

static int
ata_identify(dev_t parent, uint8_t drvsel, uint16_t *buffer)
{
	uint8_t b;
	uint16_t w;
	int i, r = 0;

	for (i = 0; i < ATA_PORT_COUNT; i++) {
		parent->driver->open(parent, i, 0);
	}

	/* set up identify command call */
	w = 0;
	parent->driver->write(parent, ATA_PORT_LBA_LO, &w, 1);
	parent->driver->write(parent, ATA_PORT_LBA_MD, &w, 1);
	parent->driver->write(parent, ATA_PORT_LBA_HI, &w, 1);
	parent->driver->write(parent, ATA_PORT_SEC_COUNT, &w, 1);
	b = drvsel;
	parent->driver->write(parent, ATA_PORT_DRV_HEAD, &b, 1);
	b = ATA_COMMAND_IDENTIFY;
	parent->driver->write(parent, ATA_PORT_COMMAND, &b, 1);

	/* check for presence of drive */
	parent->driver->read(parent, ATA_PORT_STATUS, &b, 1);
	if (!b) {
		r = 1;
		goto identify_exit;
	}

	/* wait for busy signal to clear */
	while (b & ATA_STATUS_BSY) {
		parent->driver->read(parent, ATA_PORT_STATUS, &b, 1);
	}
	/* TODO: check for atapi values */
	while (!(b & (ATA_STATUS_DRQ | ATA_STATUS_ERR))) {
		parent->driver->read(parent, ATA_PORT_STATUS, &b, 1);
	}
	if (b & ATA_STATUS_ERR) {
		r = 1;
		goto identify_exit;
	}

	parent->driver->read(parent, ATA_PORT_DATA, buffer, 256);

identify_exit:
	for (i = 0; i < ATA_PORT_COUNT; i++) {
		parent->driver->close(parent, i);
	}

	return 0;
}
