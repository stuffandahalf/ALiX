#include <alix/lib/string.h>

size_t
strlen(const char *s)
{
	size_t l = 0;
	for (l = 0; s[l] != '\0'; l++);
	return l;
}

int
strcmp(const char *s1, const char *s2)
{
	const char *c1, *c2;
	int diff = 0;

	for (c1 = s1, c2 = s2; !diff && *c1 != '\0' && *c2 != '\0'; c1++, c2++) {
		diff = c1 - c2;
	}
	if (!diff) {
		if (*c1 != '\0') {
			diff = 1;
		} else if (*c2 != '\0') {
			diff = -1;
		}
	}

	return diff;
}