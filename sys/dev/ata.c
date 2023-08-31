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
static void ata_ioctl(dev_t device);

struct dev ata = {
	"ata",

	LIST_INIT,

	ata_attach,
	ata_detach,

	ata_open,
	ata_close,
	ata_read,
	ata_write,
	ata_ioctl,

	NULL
};

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

static int
ata_attach(dev_t parent)
{
	NOT_IMPLEMENTED();
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

static void
ata_ioctl(dev_t device)
{
	NOT_IMPLEMENTED();
}


/* drive commands */

static void
ata_identify(dev_t device)
{

}
