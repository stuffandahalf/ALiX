#include <alix/log.h>

#define KLOG_BUFSZ 256
static char klog_buf[KLOG_BUFSZ] = { 0 };
static unsigned int klog_i = 0;

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