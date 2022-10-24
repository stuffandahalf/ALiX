#include <stdio.h>

#include <fs/fs.h>
#include <fs/v7x86.h>

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
