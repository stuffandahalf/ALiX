#ifndef PM_MBR_H
#define PM_MBR_H 1

#include <stdint.h>

#define MBR_PART_TYPE_FAT12 0x01
//#define MBR_PART_TYPE_FAT12_ALT 0x06
#define MBR_PART_TYPE_FAT16 0x04
#define MBR_PART_TYPE_FAT32 0x0c
#define MBR_PART_TYPE_V7X86 0x72

struct mbr_part {
	uint8_t status;
	uint8_t chs_s[3];
	uint8_t type;
	uint8_t chs_e[3];
	uint32_t lba_s;
	uint32_t sec_c;
} __attribute__((__packed__));

static const uint8_t MBR_SIGNATURE[] = { 0x55, 0xaa };

struct mbr_hdr {
	uint8_t boot_code[446];
	struct mbr_part pmap[4];
	uint8_t signature[2];
} __attribute__((__packed__));


#endif /* PM_MBR_H */
