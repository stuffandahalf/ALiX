#include <config.h>

static int
usage(const char *name)
{
	fprintf(stderr, "%s index type start (end|+length)\n", name);
	return 1;
}

int
part(int argc, char **argv)
{
	if (argc != 5) {
		return usage(argv[0]);
	}


	return 0;
}
