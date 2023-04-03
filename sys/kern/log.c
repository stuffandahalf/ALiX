#include <stdarg.h>
#include <alix/log.h>

#define KLOG_BUFSZ 1024
static char klog_buf[KLOG_BUFSZ] = { 0 };
static unsigned int klog_i = 0;

#define FORMAT_NEXT (1 << 0)
#define FORMAT_UNSIGNED (1 << 1)
#define FORMAT_LONG (1 << 2)
#define FORMAT_HEX (1 << 3)
#define FORMAT_CHAR (1 << 4)
#define FORMAT_INT (2 << 4)
#define FORMAT_FLOAT (3 << 4)
int
_klogf(const char *fmt, ...)
{
	va_list args;
	int state = 0;
	int base = 10;
	const char *c;
	union {
		unsigned long int lu;
		long int ld;
		float f;
		double lf;
	} arg;

	va_start(args, fmt);
	for (c = fmt; *c != '\0'; c++) {
		if (state) {
			switch (*c) {
			case 'l':
				state |= FORMAT_LONG;
				break;
			case 'u':
				state |= FORMAT_UNSIGNED;
				break;
			case 'x':
				state |= FORMAT_HEX;
				state |= FORMAT_LONG;
			case 'd':
				state = FORMAT_INT | (state & 0x7);
				break;
			case 'c':
				state = FORMAT_CHAR | (state & 0x7);
				break;
			case 'f':
				state = FORMAT_FLOAT | (state & 0x7);
				break;
			default:
				/* retrieve arg */
				switch (state >> 4) {
				case FORMAT_CHAR:
					arg.ld = va_arg(args, short);
					break;
				case FORMAT_INT:
					if (state & FORMAT_LONG) {
						arg.ld = va_arg(args, long int);
					} else {
						arg.ld = va_arg(args, int);
					}
					break;
				case FORMAT_FLOAT:
					if (state & FORMAT_LONG) {
						arg.lf = va_arg(args, double);
					} else {
						arg.f = va_arg(args, float);
					}
					break;
				}
				/* print arg */
				if (state & FORMAT_HEX) {
					base = 16;
				}

				switch (state >> 4) {
				case FORMAT_CHAR:
					klogc((char)arg.ld);
					break;
				case FORMAT_INT:
					if (state & FORMAT_UNSIGNED) {
						if (state & FORMAT_LONG) {
							kloglu(arg.lu, base);
						} else {
							kloglu((unsigned int)arg.lu, base);
						}
					} else {
						if (state & FORMAT_LONG) {
							klogld(arg.ld, base);
						} else {
							klogld((int)arg.ld, base);
						}
					}
					break;
				case FORMAT_FLOAT:
					/* TODO: implement printing floats */
					break;
				}

				state = 0;
				base = 10;
				break;
			}
		}

		if (!state) {
			if (*c == '%') {
				state |= FORMAT_NEXT;
			} else {
				klogc(*c);
			}
		}
	}

	va_end(args);
	return 0;
}

int
klogc(char c)
{
	if (klog_i >= KLOG_BUFSZ) return 0;

	klog_buf[klog_i++] = c;
	return 1;
}

int
klogs(const char *str)
{
	const char *c;
	int len = 0;

	for (c = str; *c != '\0'; c++) {
		len += klogc(*c);
	}
	return len;
}

int
klogld(long int ld, unsigned int base)
{
	int count = 0;
	if (ld < 0) {
		count += klogc('-');
		ld *= -1;
	}
	count += kloglu(ld, base);
	return count;
}

int
kloglu(unsigned long int lu, unsigned int base)
{
	unsigned char d;
	int i, count = 0, rcount = 0;
	unsigned long int mun = 0;

	/* invert number using mun as digit stack */
	do {
		mun += lu % base;
		lu /= base;
		count++;
		if (lu) {
			mun *= base;
		}
	} while (lu);

	/* print reverse order digits from mun */
	for (i = 0; i < count; i++) {
		d = mun % base;
		if (d < 10) {
			rcount += klogc('0' + d);
		} else {
			rcount += klogc('A' + d - 10);
		}
		mun /= base;
	}

	return rcount;
}

#define WIDTH 80
#define HEIGHT 24
#define TABWIDTH 8
void
_klog_flush(void)
{
	long int i, j, p;
	unsigned short *fb = (unsigned short *)0xb8000;

	for (i = 0; i < WIDTH * HEIGHT; i++) {
		fb[i] = 0;
	}

	p = 0;
	for (i = 0; i < klog_i; i++) {
		switch (klog_buf[i]) {
		case '\n':
			p -= (p % WIDTH);
			p += WIDTH;
			break;
		case '\t':
			for (j = 0; j < 1 + TABWIDTH - ((p % WIDTH) / TABWIDTH); j++) {
				fb[p++] = 0x0f00 | ' ';
			}
			break;
		default:
			fb[p++] = 0x0f00 | klog_buf[i];
		}
	}
	klog_i = 0;
}