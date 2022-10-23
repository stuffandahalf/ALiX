#include <stddef.h>
#include <string.h>

#include <config.h>
#include <pm/pm.h>
#include <pm/raw.h>
#include <pm/mbr.h>

struct pm *pms[] = {
	&raw_pm,
	&mbr_pm
};
size_t pmsc = sizeof(pms) / sizeof(struct pm *);

static int
usage(const char *name)
{
	fprintf(stderr, "Usage: %s type template\n", name);
	return 1;
}

int
format(int argc, char **argv)
{
	int i;

	if (argc != 3) {
		return usage(argv[0]);
	}

	for (i = 0; i < pmsc && strcmp(argv[1], pms[i]->name); i++);
	if (i == pmsc) {
		fprintf(stderr, "Unsupported partition map \"%s\"\n", argv[1]);
		return 1;
	}

	pm_type = i;
	return pms[i]->format(argv[2]);
}
