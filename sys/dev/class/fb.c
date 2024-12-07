#include <stddef.h>
#include <stdint.h>

#include <alix/dev.h>
#include <alix/device.h>

#include <alix/dev/class/fb.h>

#define FB_MODE_MONO_CHAR 0
#define FB_MODE_COLOUR_CHAR 1

struct fb_util {
	void (*setcolour)(dev_t fb, unsigned int colour);
	
};

struct fb {
	void *base;
	size_t width;
	size_t height;
	unsigned int mode;

	struct fb_util *util;
};

int
init_fb(dev_t fb, size_t width, size_t height, unsigned int mode)
{
	return 0;
}