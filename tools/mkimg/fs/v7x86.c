#include <stdio.h>

#include <alix/dev.h>
#include <alix/device.h>

#include <fs/fs.h>
#include <fs/v7x86.h>

//~ static int iread(v7x86_ino_t ino);

struct fs v7x86_fs = {
	{
		MBR_PART_TYPE_V7X86,
		{ 0 },
		"Unix_V7x86"
	},
	"v7x86",
	NULL, //~ v7x86_init,
	NULL //~ v7x86_write
};

static int
iread(dev_t d, v7x86_ino_t ino, uint8_t **buf)
{

}

static int
v7x86_init(dev_t d)
{

	return 0;
}

int
v7x86_read(dev_t d, void *buf, size_t n)
{

}

v7x86_write()
{

}
