#include <string.h>

#include <config.h>
#include <util.h>
#include <pm/pm.h>
#include <fs/fs.h>
#include <fs/fat.h>

extern struct pm *pms[];
extern size_t pmsc;

struct fs *fss[] = {
	&fat16_fs
};
size_t fsc = sizeof(fss) / sizeof(struct fs *);

static int
usage(const char *name)
{
	fprintf(stderr, "%s index type start (end|+length)\n", name);
	return 1;
}

int
part(int argc, char **argv)
{
	int i;
	size_t index = 0;
	struct fs *fsconf = NULL;
	unsigned long start = 1;
	unsigned long end = 0;
	struct pm *pmconf = pms[pm_type];

	if (argc != 5) {
		return usage(argv[0]);
	}

	if (parsesz(argv[1], &index)) {
		fprintf(stderr, "invalid partition index provided \"%s\"\n", argv[1]);
		return 1;
	}

	if (parsesz(argv[3], &start)) {
		fprintf(stderr, "invalid partition block provided \"%s\"\n", argv[3]);
		return 1;
	}

	if (parsesz(argv[4] + (argv[4][0] == '+'), &end)) {
		fprintf(stderr, "invalid partition block provided \"%s\"\n", argv[4]);
	}
	/* if end was provided as a relative offset, calculate the end block */
	if (argv[4][0] == '+') {
		end = end / blksz + !!(end % blksz) + start;
	}

	for (i = 0; i < fsc; i++) {
		if (!strcmp(argv[2], fss[i]->name)) {
			fsconf = fss[i];
			if (pmconf->partadd(index, fsconf->type, start, end)) {
				return 1;
			}
			break;
		}
	}
	if (i == fsc) {
		fprintf(stderr, "unrecognized filesystem \"%s\"\n", argv[2]);
		return 1;
	}

	fsconf->init(start, end);

	return 0;
}
