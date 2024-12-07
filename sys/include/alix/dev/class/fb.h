#ifndef ALIX_DEV_CLASS_FB_H
#define ALIX_DEV_CLASS_FB_H 1

#include <stddef.h>
#include <stdint.h>

#include <alix/device.h>

#define FB_MODE_CHAR 1
#define FB_MODE_PIXEL



int init_fb(dev_t fb, size_t width, size_t height, unsigned int mode);

#endif /* ALIX_DEV_CLASS_FB_H */