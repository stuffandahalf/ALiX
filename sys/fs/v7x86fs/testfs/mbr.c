#include <stdio.h>

#include "mbr.h"
#include "dev.h"

extern char mbr[SECTOR_SZ];

struct mbr_part *
mbr_find_part(unsigned char id)
{
	struct mbr_part *parttab = (struct mbr_part *)(mbr + MBR_PARTTAB_OFFSET);
	int i;
	
	for (i = 0; i < 4; i++) {
		if (parttab[i].type == id) {
			return &parttab[i];
		}
	}

	return NULL;
}

