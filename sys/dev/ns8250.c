#include <stdint.h>

#include <alix/dev.h>

static void ns8250_attach(dev_t parent);
static void ns8250_detach(dev_t parent);

static int ns8250_open(dev_t device, int flags);
static int ns8250_close(dev_t device);
static int ns8250_read(dev_t device, void *buf, size_t n);
static int ns8250_write(dev_t device, void *buf, size_t n);
static void ns8250_ioctl(dev_t device);

struct dev ns8250 = {
	"ns8250",

	ns8250_attach,
	ns8250_detach,

	ns8250_open,
	ns8250_close,
	ns8250_read,
	ns8250_write,
	ns8250_ioctl
};

static void
ns8250_attach(dev_t parent)
{

}

static void
ns8250_detach(dev_t parent)
{

}

static int
ns8250_open(dev_t device, int flags)
{
	return 0;
}

static int
ns8250_close(dev_t device)
{
	return 0;
}

static int
ns8250_read(dev_t device, void *buf, size_t n)
{
	return 0;
}

static int
ns8250_write(dev_t device, void *buf, size_t n)
{
	return 0;
}

static void
ns8250_ioctl(dev_t device)
{

}
