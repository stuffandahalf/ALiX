#include <stdio.h>
#include <stdlib.h>

#include <pm/pm.h>
#include <pm/mbr.h>
#include <fs/fs.h>
#include <fs/v7x86.h>

#define BSIZE 512

int process(void);

FILE *fp;

FILE *ofp;

int
main(int argc, char **argv)
{
	int i, ret;

	if (argc < 2) {
		fprintf(stderr, "usage: %s file...\n", argv[0]);
		return 1;
	}

	for (i = 1; i < argc; i++) {
		if (!(fp = fopen(argv[i], "r"))) {
			perror(NULL);
			return 1;
		}
		ret = process();
		fclose(fp);
		if (ret) {
			return 1;
		}
	}

	return 0;
}

static int
bread(unsigned long int lba, void *buffer)
{
	printf("Seeking to lba %lu, offset %lX, target %p\n", lba, lba * BSIZE, buffer);
	if (-1 == fseek(fp, BSIZE * lba, SEEK_SET)) {
		perror(NULL);
		return 1;
	}
	if (1 != fread(buffer, BSIZE, 1, fp)) {
		fprintf(stderr, "Failed to read %d bytes\n", BSIZE);
		return 1;
	}
	return 0;
}

#define INLBA(in) (inlba + ((in) / INOPB))
//#define DISKADDR(addr, i) (bklba + (((addr)[(i) * 3 + 2] << 16) + ((addr)[(i) * 3 + 1] << 8) + ((addr)[(i) * 3 + 0])))
#define DISKADDR(addr, i) (((addr)[(i) * 3 + 2] << 16) + ((addr)[(i) * 3 + 1] << 8) + ((addr)[(i) * 3 + 0]))

int
process(void)
{
	int i;
	struct mbr_hdr *mbr;
	struct mbr_part *part;
	struct v7x86_superblk *superblk;
	struct v7x86_dinode *ino;
	size_t sz, loaded = 0;
	uint8_t *buffer;
	uint8_t *dir, *file;

	unsigned long int inlba = 0, bklba = 0;

	mbr = malloc(BSIZE);
	superblk = malloc(BSIZE);
	buffer = malloc(BSIZE);

	fread(mbr, sizeof(struct mbr_hdr), 1, fp);
	for (i = 0; i < MBR_MAX_PARTITIONS; i++) {
		if (mbr->pmap[i].type == MBR_PART_TYPE_V7X86) {
			break;
		}
	}
	if (i == MBR_MAX_PARTITIONS) {
		return 1;
	}
	part = &(mbr->pmap[i]);

	printf("%p\t%X\t%u\n", part, part->status, part->lba_s);

	/* partition lba 0 is boot code */
	if (bread(part->lba_s + 1, superblk)) {
		return 1;
	}

	inlba = part->lba_s + 2;
	bklba = part->lba_s + 0 + superblk->s_isize;
	//~ bklba = superblk->s_isize;
	fprintf(stderr, "BKLBA %lX %lX\n", bklba, bklba * BSIZE);
	//~ bklba = 0;

	if (bread(INLBA(V7X86_ROOTINO), buffer)) {
		return 1;
	}
	ino = &((struct v7x86_dinode *)buffer)[V7X86_ROOTINO - 1];
	printf("file size %u\n", ino->di_size);

	sz = ino->di_size;
	//~ dir = malloc(sizeof(uint8_t) * (ino->di_size + ino->di_size % BSIZE));
	dir = malloc(sizeof(uint8_t) * BSIZE * 13);
	for (i = 0; i < 13 && ino->di_size > 0; i++) {
		fprintf(stderr, "DISK ADDRESS %lu\n", DISKADDR(ino->di_addr, i));
		if (sz >= BSIZE) {
			sz -= BSIZE;
		} else {
			sz = 0;
		}
		bread(part->lba_s + DISKADDR(ino->di_addr, i), &dir[i * BSIZE]);
	}

	for (i = 0; i < (BSIZE / sizeof(struct v7x86_dirent)); i++) {
	//~ for (i = 0; i < (ino->di_size / sizeof(struct v7x86_dirent)); i++) {
		fprintf(stderr, "%.*s\n", DIRSIZ, ((struct v7x86_dirent *)dir)[i].d_name);
	}

	free(dir);

	free(buffer);
	free(superblk);
	free(mbr);

	//~ while (loaded <

	return 0;
}
