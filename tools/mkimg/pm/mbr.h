#ifndef PM_MBR_H
#define PM_MBR_H 1

#include <stdint.h>

#include <pm/pm.h>

#define MBR_PART_TYPE_FAT12 0x01
//#define MBR_PART_TYPE_FAT12_ALT 0x06
#define MBR_PART_TYPE_FAT16 0x04
#define MBR_PART_TYPE_FAT32 0x0c
#define MBR_PART_TYPE_V7X86 0x72

//~ #define PACK_CHS(c, h, s) ()
#define MBR_UNPACK_C(chs) ((((chs)[1] & 0xc0) << 2) | ((chs)[2]))
#define MBR_UNPACK_H(chs) ((chs)[0])
#define MBR_UNPACK_S(chs) ((chs)[1] & 0x3f)

#if __STDC_VERSION__ >= 199901L
inline
#endif
static void
MBR_PACK_CHS(uint8_t *chs, uint16_t c, uint8_t h, uint8_t s)
{
	chs[0] = h;
	chs[1] = ((c >> 2) & 0xc0) | (s & 0x3f);
	chs[2] = c & 0xff;
}

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

extern struct pm mbr_pm;

#endif /* PM_MBR_H */
