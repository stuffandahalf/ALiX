#include <stdio.h>

#include <config.h>
#include <pm/pm.h>
#include <pm/mbr.h>

static int mbr_format(const char *template);
static int mbr_partadd(int type, unsigned long int start, unsigned long int length);

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

	//~ fseek(ofp, 0, SEEK_SET);

	return 0;
}

static int
mbr_partadd(int type, unsigned long int start, unsigned long int length)
{
	return 0;
}
