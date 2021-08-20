#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "dev.h"
#include "mbr.h"

#include "../v7x86fs.h"

const char *const path = "/test.c";

char mbr[SECTOR_SZ];
char vbr[SECTOR_SZ];
char sblk[SECTOR_SZ];
char *inodel;

int
main(int argc, char **argv)
{
	size_t sz;
	FILE *fp;
	struct mbr_part *part;
	
	printf("Size of dinode is %zu\n", sizeof(struct v7fs_dinode));
	printf("offset of file size is %zu\n", offsetof(struct v7fs_dinode, di_size));

	if (argc != 2) {
		fprintf(stderr, "Missing file argument\n");
		return 1;
	}

	fp = fopen(argv[1], "rb");
	if (!fp) {
		perror("Failed to open image file");
		return 1;
	}

	/*sz = fread(mbr, sizeof(char), SECTOR_SZ, fp);
	if (sz != SECTOR_SZ) {
		fclose(fp);
		perror("Failed to read MBR");
		return 1;
	}*/
	if (!dev_read(fp, 0, 1, mbr)) {
		fclose(fp);
		perror("Failed to read MBR");
		return 1;
	}

	part = mbr_find_part(MBR_ID_V7X86FS);
	if (!part) {
		fclose(fp);
		fprintf(stderr, "Failed to locate V7x86 partition\n");
		return 1;
	}
	printf("%x\n", part->type);
	printf("Partition LBA is at %lu\n", part->lba);

	if (!dev_read(fp, part->lba, 1, vbr)) {
		fclose(fp);
		fprintf(stderr, "Failed to read VBR\n");
		return 1;
	}

	printf("Superblock LBA is %d\n", part->lba + 1);
	if (!dev_read(fp, part->lba + 1, 1, sblk)) {
		fclose(fp);
		fprintf(stderr, "Failed to read partition superblock\n");
		return 1;
	}
	
	struct v7fs_superblock *spblk = (struct v7fs_superblock *)sblk;
	printf("size of i-list: %d\n", spblk->s_isize);
	inodel = malloc(sizeof(char) * SECTOR_SZ * (spblk->s_isize - 2));
	printf("number of blocks to read for inode list: %d\n", spblk->s_isize - 2);
	if (!inodel) {
		fclose(fp);
		fprintf(stderr, "Failed to allocate memory for inode list\n");
		return 1;
	}
	printf("Root LBA is %d\n", part->lba + 2);
	if (!dev_read(fp, part->lba + 2, spblk->s_isize - 2, inodel)) {
		free(inodel);
		fclose(fp);
		fprintf(stderr, "Failed to read inode list\n");
		return 1;
	}

	struct v7fs_dinode *inodes = (struct v7fs_dinode *)inodel;
	struct v7fs_dinode *rooti = &inodes[V7FS_ROOT_INODE - 1];
	//struct v7fs_dinode *rooti = &inodes[129];
	printf("%d\n", rooti->di_size);
	//printf("%d\n", rooti->di_nlink);
	printf("file type = %o\n", rooti->di_mode);
	/*for (size_t i = 0; i < sizeof(struct v7fs_dinode); i++) {
		if (i && !(i % 8)) {
			printf("\n");
		}
		printf("0x%X\t", ((char *)rooti)[i] & 0xff);
	}*/
	
	char *raw_dir = malloc(sizeof(char) * SECTOR_SZ);
	if (!raw_dir) {
		free(inodel);
		fclose(fp);
		fprintf(stderr, "Failed to allocate memory for directory\n");
		return 1;
	}
	// disk addresses stored in little-endian
	v7fs_daddr_t faddr = (rooti->di_addr[2] << 16) |
		(rooti->di_addr[1] << 8) | (rooti->di_addr[0]);
	printf("daddr = %ld\n", faddr);
	printf("directory size = %d\n", rooti->di_size);
	printf("addresses = %zu\n", offsetof(struct v7fs_dinode, di_addr));
	if (!dev_read(fp, part->lba + faddr, 1, raw_dir)) {
		free(raw_dir);
		free(inodel);
		fclose(fp);
		fprintf(stderr, "Failed to read root directory\n");
		return 1;
	}
	struct v7fs_dirent *rootdir = (struct v7fs_dirent *)raw_dir;
	for (int i = 0; i < SECTOR_SZ / sizeof(struct v7fs_dirent); i++) {
		printf("%d\t%.14s\n", rootdir[i].d_ino, rootdir[i].d_name);
		//write(1, rootdir[i].d_name, 14);
		//printf("\n");
	}

	free(inodel);
	fclose(fp);
	return 0;
}

