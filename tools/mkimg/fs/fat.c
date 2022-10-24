#include <stdio.h>

#include <fs/fs.h>
#include <fs/fat.h>

static int fat16_init(unsigned long int start, unsigned long int end);

struct fs fat16_fs = {
	MBR_PART_TYPE_FAT16,
	"fat16",
	fat16_init,
	NULL
};

static int
fat16_init(unsigned long int start, unsigned long int end)
{
	return 0;
}
