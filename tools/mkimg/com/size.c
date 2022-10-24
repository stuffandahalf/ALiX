#include <ctype.h>
#include <string.h>

#include <config.h>

static int
usage(const char *name)
{
	fprintf(stderr, "%s device | block \\d+[kMG]\n", name);
	return 1;
}

static int
parse(char *str, size_t *out)
{
	size_t sz;
	char *cp;

	sz = 0;
	for (cp = str; *cp != '\0' && isdigit(*cp); cp++) {
		if (cp != str) {
			sz *= 10;
		}
		sz += *cp - '0';
	}
	if (!strcmp(cp, "k")) {
		sz *= 1024;
	} else if (!strcmp(cp, "M")) {
		sz *= 1024 * 1024;
	} else if (!strcmp(cp, "G")) {
		sz *= 1024 * 1024 * 1024;
	} else if (strcmp(cp, "")) {
		fprintf(stderr, "unrecognized size modifier \"%s\"\n", cp);
		return 1;
	}

	*out = sz;

	return 0;
}

static int
dev_size(int argc, char **argv)
{
	/* TODO: check if output is a block device with fixed size */
	if (parse(argv[1], &ofsz)) {
		return 1;
	}
	fprintf(stderr, "requested size %lu\n", ofsz);

	if (fseek(ofp, ofsz - 1, SEEK_SET)) {
		perror("Failed to seek to end of file");
	}
	fprintf(ofp, "%c", 0);

	return 0;
}

static int
blk_size(int argc, char **argv)
{
	if (pm_type >= 0) {
		fprintf(stderr, "cannot change block size after disk has been modified\n");
		return 1;
	}

	return parse(argv[1], &blksz);
}

struct command subcmds[] = {
	{ "device", 'd', dev_size },
	{ "block", 'b', blk_size }
};
size_t subcmdsc = sizeof(subcmds) / sizeof(struct command);

int
size(int argc, char **argv)
{
	int i;

	if (argc != 3) {
		return usage(argv[0]);
	}

	for (i = 0; i < subcmdsc; i++) {
		if (strlen(argv[1]) == 1 && argv[i][0] == subcmds[i].sname ||
				!strcmp(argv[1], subcmds[i].lname)) {
			return subcmds[i].entry(argc - 1, argv + 1);
		}
	}

	fprintf(stderr, "Unable to find subcommand \"%s\"\n", argv[1]);

	return 1;
}
