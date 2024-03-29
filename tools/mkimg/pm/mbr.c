#include <stdio.h>

#include <config.h>
#include <pm/pm.h>
#include <pm/mbr.h>
#include <fs/fs.h>

static int mbr_format(const char *template);
static int mbr_partadd(int index, struct fs_type *type, unsigned long int start, unsigned long int length);
static int mbr_write(void);

struct pm mbr_pm = {
	"mbr",
	mbr_format,
	mbr_partadd
};

struct mbr_hdr hdr;

static int
mbr_format(const char *template)
{
	int i;
	FILE *fp;
	size_t c;

	if (template) {
		if (!(fp = fopen(template, "rb"))) {
			perror(NULL);
			return 1;
		}

		c = fread(&hdr, 1, sizeof(struct mbr_hdr), fp);
		if (c != sizeof(struct mbr_hdr)) {
			fprintf(stderr, "Failed to read %zu bytes from \"%s\", read %zu\n",
				sizeof(struct mbr_hdr), template, c);
		}

		fclose(fp);

		if (hdr.signature[0] != MBR_SIGNATURE[0] || hdr.signature[1] != MBR_SIGNATURE[1]) {
			fprintf(stderr, "mbr from template \"%s\" has invalid signature "
				"0x%X, 0x%X (expected 0x%X, 0x%X)\n", template,
				hdr.signature[0], hdr.signature[1],
				MBR_SIGNATURE[0], MBR_SIGNATURE[1]);
			return 1;
		}
	} else {
		hdr.signature[0] = MBR_SIGNATURE[0];
		hdr.signature[1] = MBR_SIGNATURE[1];
	}

	mbr_write();

	return 0;
}

static int
mbr_partadd(int index, struct fs_type *type, unsigned long int start, unsigned long int end)
{
	struct mbr_part *part = NULL;

	if (index >= MBR_MAX_PARTITIONS) {
		fprintf(stderr, "index \"%d\" is invalid for mbr partition maps\n", index);
		return 1;
	}

	part = &hdr.pmap[index];

	part->status = 0x00;
	/*uint16_t chs[3];
	if (lba2chs(start, chs)) {
		return 1;
	}*/
	/* TODO: Figure out what to do with this */
	MBR_PACK_CHS(part->chs_s, 0, 0, 0);
	MBR_PACK_CHS(part->chs_e, 0, 0, 0);
	part->type = type->mbr;
	part->lba_s = start;
	part->sec_c = end - start;

	//~ fprintf(stderr, "adding part %d\ttype: %X\tstart: %lu\tend: %lu\n", type, start, end);
	//~ return 0;
	return mbr_write();
}

static int
mbr_getpartfs(int i, struct fs *filsys)
{
	fprintf(stderr, "%s:%d Not yet implemented\n", __FILE__, __LINE__);
	return 1;
}

static int
mbr_write(void)
{
	if (fseek(ofp, 0, SEEK_SET)) {
		perror("Failed to seek to start of output");
		return 1;

	}
	fwrite(&hdr, 1, sizeof(struct mbr_hdr), ofp);
	if (ferror(ofp)) {
		perror("Failed to write updated mbr to output");
		return 1;
	}

	return 0;
}

