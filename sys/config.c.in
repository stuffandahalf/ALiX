#include <stdarg.h>

int
test(int n, ...)
{
	va_list ap;
	int i, v, r = 0;

	va_start(ap, n);

	for (i = 0; i < n; i++) {
		v = va_arg(ap, int);
		r += v;
	}

	va_end(ap);

	return r;
}