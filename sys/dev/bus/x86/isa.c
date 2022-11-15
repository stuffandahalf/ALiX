#include <alix/dev.h>

static void isa_attach(dev_t parent);
static void isa_detach(dev_t parent);

static int isa_open(dev_t device, int flags);
static int isa_close(dev_t device);
static int isa_read(dev_t device, void *buffer, size_t n);
static int isa_write(dev_t device, void *buffer, size_t n);
static void isa_ioctl(dev_t device);

struct dev isa = {
	"isa",

	isa_attach,
	isa_detach,

	isa_open,
	isa_close,
	isa_read,
	isa_write,
	isa_ioctl
};

static void
isa_attach(dev_t parent)
{
	dev_t d;

	d = create_dev(&isa);
}

static void
isa_detach(dev_t parent)
{

}

static int
isa_open(dev_t device, int flags)
{
	return 0;
}

static int
isa_close(dev_t device)
{
	return 0;
}


static int
isa_read(dev_t device, void *buffer, size_t n)
{
	return 0;
}

static int
isa_write(dev_t device, void *buffer, size_t n)
{
	return 0;
}

static void
isa_ioctl(dev_t device)
{

}
