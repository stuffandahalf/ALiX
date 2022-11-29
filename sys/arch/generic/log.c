#include <arch/log.h>

unsigned long int
log2(unsigned long int n)
{
	unsigned long int ret = 0;
	while ((n >> ret) > 0) ret++;
	return ret;
}
