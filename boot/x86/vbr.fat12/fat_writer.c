#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <locale.h>

#define BOOT_SEC_SZ 512

struct fat_bpb {
	uint8_t jmp[3];
	char oem[8];
	uint16_t bytes_per_sector;
	uint8_t sectors_per_cluster;
	uint16_t reserved_sectors;
	uint8_t n_fats;
	uint16_t n_dir_entries;
	uint16_t n_sectors_short;
	uint8_t media_descriptor;
	uint16_t sectors_per_fat;
	uint16_t sectors_per_track;
	uint16_t n_heads;
	uint32_t hidden_sectors;
	uint32_t n_sectors_long;
} __attribute__((__packed__));

struct fat12_ebpb {
	struct fat_bpb bpb;
	uint8_t drive_num;
	uint8_t reserved;
	uint8_t signature;
	uint32_t serial;
	char label[12];
	char identifier[8];
} __attribute__((__packed__));

struct fat16_ebpb {
	struct fat_bpb bpb;

} __attribute__((__packed__));

struct fat32_ebpb {
	struct fat_bpb bpb;

} __attribute__((__packed__));

void usage(char *fname, FILE *stream);
int configure(int argc, char **argv);

void process(struct fat_bpb *mkfs, struct fat_bpb *alix);
void process12(struct fat12_ebpb *mkfs, struct fat12_ebpb *alix);
void process16(struct fat16_ebpb *mkfs, struct fat16_ebpb *alix);
void process32(struct fat32_ebpb *mkfs, struct fat32_ebpb *alix);

int fat_version = 0;
FILE *bootsec = NULL;
FILE *dskimg = NULL;
union bootsector {
	uint8_t raw[BOOT_SEC_SZ];
	struct fat_bpb bpb;
	struct fat12_ebpb ebpb12;
	struct fat16_ebpb ebpb16;
	struct fat32_ebpb ebpb32;
} mkfs_bs, alix_bs;

void release(void)
{
	if (bootsec) {
		fclose(bootsec);
	}
	if (dskimg) {
		fclose(dskimg);
	}
}

int main(int argc, char **argv)
{
	int c;

	setlocale(LC_ALL, "");
	atexit(release);

	if (configure(argc, argv)) {
		return 1;
	}

	printf("Updating boot sector for FAT%d partition\n", fat_version);

	c = fread(mkfs_bs.raw, sizeof(uint8_t), BOOT_SEC_SZ, dskimg);
	if (c < BOOT_SEC_SZ) {
		fprintf(stderr, "Failed to read boot sector from disk image (expected: %d, read: %d)\n", BOOT_SEC_SZ, c);
		return 1;
	}
	c = fread(alix_bs.raw, sizeof(uint8_t), BOOT_SEC_SZ, bootsec);
	if (c < BOOT_SEC_SZ) {
		fprintf(stderr, "Failed to read boot sector from alix boot sector (expected: %d, read: %d)\n", BOOT_SEC_SZ, c);
		return 1;
	}

	switch (fat_version) {
	case 12:
		process12(&mkfs_bs.ebpb12, &alix_bs.ebpb12);
		break;
	case 16:
		process16(&mkfs_bs.ebpb16, &alix_bs.ebpb16);
		break;
	case 32:
		process32(&mkfs_bs.ebpb32, &alix_bs.ebpb32);
		break;
	}

	fseek(dskimg, 0, SEEK_SET);
	c = fwrite(alix_bs.raw, sizeof(uint8_t), BOOT_SEC_SZ, dskimg);
	if (c != BOOT_SEC_SZ) {
		perror(NULL);
		fprintf(stderr, "Failed to write modified boot sector (expected: %d, wrote %d)\n", BOOT_SEC_SZ, c);
		return 1;
	}

	return 0;
}

int configure(int argc, char **argv)
{
	int c;
	char *end;
	while ((c = getopt(argc, argv, "F:b:h")) != -1) {
		switch (c) {
		case 'F':
			fat_version = strtol(optarg, &end, 10);
			if (*end) {
				fprintf(stderr, "Failed to parse fat version \"%s\"\n", optarg);
				usage(argv[0], stderr);
				return 1;
			}
			break;
		case 'b':
			if (bootsec) {
				fprintf(stderr, "Boot sector already specified\n");
				return 1;
			}
			bootsec = fopen(optarg, "rb");
			if (!bootsec) {
				fprintf(stderr, "Failed to open boot sector file \"%s\"\n", optarg);
				return 1;
			}
			break;
		case 'h':
			usage(argv[0], stdout);
			break;
		case '?':
		default:
			usage(argv[0], stderr);
			return 1;
		}
	}

	if (optind == argc) {
		fprintf(stderr, "Missing disk image file\n");
		usage(argv[0], stdout);
		return 1;
	}

	if (optind > argc) {
		fprintf(stderr, "Too many options provided\n");
		usage(argv[0], stdout);
		return 1;
	}

	dskimg = fopen(argv[optind], "r+b");
	if (!dskimg) {
		fprintf(stderr, "Failed to open disk image file \"%s\"\n", argv[optind]);
		return 1;
	}

	return 0;
}

void process(struct fat_bpb *mkfs, struct fat_bpb *alix)
{
	alix->bytes_per_sector = mkfs->bytes_per_sector;
	alix->reserved_sectors = mkfs->reserved_sectors;
	alix->n_fats = mkfs->n_fats;
	alix->n_dir_entries = mkfs->n_dir_entries;
	alix->n_sectors_short = mkfs->n_sectors_short;
	alix->media_descriptor = mkfs->media_descriptor;
	alix->sectors_per_fat = mkfs->sectors_per_fat;
	alix->sectors_per_track = mkfs->sectors_per_track;
	alix->n_heads = mkfs->n_heads;
	alix->hidden_sectors = mkfs->hidden_sectors;
	alix->n_sectors_long = mkfs->n_sectors_long;
}

void process12(struct fat12_ebpb *mkfs, struct fat12_ebpb *alix)
{
	int i;

	process(&mkfs->bpb, &alix->bpb);
	mkfs->drive_num = alix->drive_num;
	mkfs->reserved = alix->reserved;
	mkfs->signature = alix->signature;
	mkfs->serial = alix->serial;
	for (i = 0; i < 12; i++) {
		mkfs->label[i] = alix->label[i];
	}
}

void process16(struct fat16_ebpb *mkfs, struct fat16_ebpb *alix)
{
	process(&mkfs->bpb, &alix->bpb);
	
}

void process32(struct fat32_ebpb *mkfs, struct fat32_ebpb *alix)
{
	process(&mkfs->bpb, &alix->bpb);
	
}

void usage(char *fname, FILE *stream)
{
	fprintf(stream, "USAGE: %s -F FAT_SZ -b BOOT_SECTOR [-h]\n", fname);
}

