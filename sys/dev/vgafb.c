#include <stddef.h>
#include <stdint.h>

#include <alix/dev.h>
#include <alix/device.h>

static int vgafb_attach(dev_t parent);
static void vgafb_detach(dev_t parent);

static int vgafb_open(dev_t device, unsigned int channel, int flags);
static int vgafb_close(dev_t device, unsigned int channel);
static int vgafb_read(dev_t device, unsigned int channel, void *buf, size_t n);
static int vgafb_write(dev_t device, unsigned int channel, void *buf, size_t n);
static int vgafb_ioctl(dev_t device, unsigned long int request, void **args, size_t nargs);

#define VGAFB_FEATURES DEV_FALL
struct dev vgafb = DEV_INIT(vgafb, VGAFB_FEATURES);

#define VGA_BASE ((unsigned short *)0xb8000)
#define VGA_WIDTH 80
#define VGA_HEIGHT 24

static int
vgafb_attach(dev_t parent)
{
	dev_t fb = dev_create(&vgafb, 1, parent);

	if (fb_init(fb, VGA_WIDTH, VGA_HEIGHT)) {
		dev_destroy(fb);
		return 1;
	}

	add_dev(fb);

	return 0;
}

static void
vgafb_detach(dev_t device)
{

}

static int
vgafb_open(dev_t device, unsigned int channel, int flags)
{
	return 0;
}

static int
vgafb_close(dev_t device, unsigned int channel)
{
	return 0;
}

static int
vgafb_read(dev_t device, unsigned int channel, void *buf, size_t n)
{
	return 0;
}

static int
vgafb_write(dev_t device, unsigned int channel, void *buf, size_t n)
{
	return 0;
}

static int
vgafb_ioctl(dev_t device, unsigned long int request, void **args, size_t nargs)
{
	return 0;
}
