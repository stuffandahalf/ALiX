#include <stdio.h>

#include "dev.h"

int
dev_read(FILE *fp, unsigned long long lba, size_t count, char *buffer)
{
	fseek(fp, lba * SECTOR_SZ, SEEK_SET);
	return fread(buffer, SECTOR_SZ, count, fp) != count * SECTOR_SZ;
}

