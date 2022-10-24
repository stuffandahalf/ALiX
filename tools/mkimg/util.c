#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include <util.h>

int
parsesz(const char *str, size_t *out)
{
	size_t sz;
	const char *cp;

	sz = 0;
	for (cp = str; *cp != '\0' && isdigit(*cp); cp++) {
		sz *= 10;
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
