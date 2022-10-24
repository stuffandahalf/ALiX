#ifndef FS_FAT_H
#define FS_FAT_H 1

#include <stdint.h>

#include <fs/fs.h>

#define MBR_PART_TYPE_FAT12 0x01
//#define MBR_PART_TYPE_FAT12_ALT 0x06
#define MBR_PART_TYPE_FAT16 0x04
#define MBR_PART_TYPE_FAT16B 0x06
#define MBR_PART_TYPE_FAT32 0x0c

#define BPB_FIELDS \
	uint8_t jmp[3]; \
	uint8_t oem[8]; \
	uint16_t secsz; \
	uint8_t clustersz; /* in number of sectors */ \
	uint16_t ressecc; /* reserved sectors */ \
	uint8_t fatc; \
	uint16_t rootdirl; \
	uint16_t secc; \
	uint8_t media; \
	uint16_t fatsz; /* in number of sectors */ \
	uint16_t tracksz; /* in number of sectors */ \
	uint16_t headc; \
	uint32_t empty; /* number of sectors preceding partition */ \
	uint32_t lsecc; \

#define EBPB_FIELDS \
	uint8_t drvnum; \
	uint8_t rsrvd; \
	uint8_t extsig; \
	uint32_t id; \
	uint8_t label[11]; \
	uint8_t fstype[8];

#define F32EBPB_FIELDS \
	uint32_t lfatsz; /* in number of sectors */ \
	uint16_t mirroring; /* describe how each fat table should be mirrors */ \
	uint16_t version; \
	uint32_t root; /* cluster of root */ \
	uint16_t fsinfo; /* sector number of fs info sector */ \
	uint16_t firstbs; /* sector of first copy of boot sector */ \
	uint8_t rsrvd2[12]; \

struct bpb {
	BPB_FIELDS
	uint8_t code[474];
	uint8_t sig[2];
} __attribute__((__packed__));

struct ebpb {
	BPB_FIELDS
	EBPB_FIELDS
	uint8_t code[448];
	uint8_t sig[2];
} __attribute__((__packed__));

struct f32ebpb {
	BPB_FIELDS
	F32EBPB_FIELDS
	EBPB_FIELDS
	uint8_t code[420];
	uint8_t sig[2];
} __attribute__((__packed__));

extern struct fs fat12_fs;
extern struct fs fat16_fs;
extern struct fs fat32_fs;

#endif /* FS_FAT_H */
