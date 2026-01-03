#include <stdio.h>
#include "include/regex.h"

int
main(int argc, char **argv)
{
	int r;
	const char *pattern = "abc";
	regex_t re;

	if ((r = regcomp(&re, pattern, 0))) {
		printf("ERROR\n");
		regfree(&re);
		return 1;
	}
	printf("SUCCESS\n");
	regfree(re);
	return 0;
}
