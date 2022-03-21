#include <regex.h>
#include <stdlib.h>

int regcomp(regex_t *RESTRICT preg, const char *RESTRICT pattern,
		int cflags)
{
	
	regex_t *re = preg;
	const char *c, *seq;
	unsigned int seq_l = 0;

	for (c = pattern; *c != '\0'; c++) {
		switch (*c) {
		/*case '(':
			break;
		case ')':
			break;
		case '{':
			break;
		case '}':
			break;
		case '[':
			break;
		case ']':
			break;*/
		case '?':
			break;
		case '*':
			break;
		case '+':
			break;
		default:
			if (!seq_l) {
				seq = c;
				seq_l = 1;
			} else {
				seq_l++;
			}
			break;
		}
	}
}

