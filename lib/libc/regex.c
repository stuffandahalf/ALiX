/*#include <regex.h>*/
#include "include/regex.h"
#include <stdlib.h>

#include <stdio.h>

struct regpath {
	const char *rp_frag;
	size_t rp_len;
	struct regpath *rp_next;
	size_t rp_nextc;
};

static int
parsepattern(const char *RESTRICT pattern)
{
	const char *c1, *c2, *fragment;
	int count = 0;

	fragment = pattern;
	for (c1 = pattern, c2 = pattern + 1; *c1 != '\0'; c1++, c2++) {
		printf("%c\t%c\n", *c1, *c2);
		switch (*c1) {
		case '(':
			parseparen(c1);
			break;
		default:
			count++;
			break;
		}
	}
	return count;
}

static int
parseparen(const char *RESTRICT pattern)
{
	const char *c1, *c2;
	int count = 0;

	if (!pattern) {
		//return 0;
		count = 0;
		goto exit;
	}

	if (*pattern != '(') {
		count = 0;
		goto exit;
	}
	count++;

	/* parse pattern between parentheses */
	parsepattern(pattern);

	if (*patern != '\0' || *pattern != ')') {
		count = -1 * REG_EPAREN;
		goto exit;
	}
	count++;


exit:
	return count;
}

int
regcomp(regex_t *RESTRICT preg, const char *RESTRICT pattern, int cflags)
{
	int rval = 0, rcount = 0;
	const char *c1, *c2;
	const char *fragment;

	if (!preg) {
		rval = REG_ENULLPTR;
		goto abort;
	}
	/* initialize regex */
	preg->re_flags = cflags;
	preg->re_nsub = 0;
	preg->re_paths = NULL;
	preg->re_npaths = -1;

	if ((rcount = parsepattern(pattern)) <= 0) {
		rval = -1 * rcount;
		goto abort;
	}

	return rval;

abort:
	regfree(preg);
	return rval;
}

