#include <arch/log.h>

unsigned long int
log2(unsigned long int n)
{
	unsigned long int ret;
	__asm__ __volatile__ ("bsrl %1, %0\n\t" : "=r"(ret) : "r"(n));
	return ret;
}
