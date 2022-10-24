#include <stdio.h>

#include <fs/fs.h>
#include <fs/fat.h>

static int fat16_init(unsigned long int start, unsigned long int end);

struct fs fat16_fs = {
	{
		MBR_PART_TYPE_FAT16,
		{ 0xEB, 0xD0, 0xA0, 0xA2, /* - */ 0xB9, 0xE5, /* - */ 0x44, 0x33, /* - */ 0x87, 0xC0, /* - */ 0x68, 0xB6, 0xB7, 0x26, 0x99, 0xC7 },
		"FAT_16"
	},
	"fat16",
	fat16_init,
	NULL
};

static int
fat16_init(unsigned long int start, unsigned long int end)
{
	return 0;
}
