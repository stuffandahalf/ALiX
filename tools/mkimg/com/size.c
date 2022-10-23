#include <ctype.h>
#include <string.h>

#include <config.h>

static int
usage(const char *name)
{
	fprintf(stderr, "%s \\d+[kMG]\n", name);
	return 1;
}

int
size(int argc, char **argv)
{
	char *cp;

	if (argc != 2) {
		return usage(argv[0]);
	}

	/* TODO: check if output is a block device with fixed size */

	ofsz = 0;
	for (cp = argv[1]; *cp != '\0' && isdigit(*cp); cp++) {
		if (cp != argv[1]) {
			ofsz *= 10;
		}
		ofsz += *cp - '0';
	}
	if (!strcmp(cp, "k")) {
		ofsz *= 1024;
	} else if (!strcmp(cp, "M")) {
		ofsz *= 1024 * 1024;
	} else if (!strcmp(cp, "G")) {
		ofsz *= 1024 * 1024 * 1024;
	} else {
		fprintf(stderr, "unrecognized size modifier \"%s\"\n", cp);
		return 1;
	}
	fprintf(stderr, "requested size %lu\n", ofsz);

	return 0;
}
