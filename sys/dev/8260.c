#include <alix/dev.h>

static void sr8260_open(dev_t device, int flags);
static void sr8260_read(dev_t device);
static void sr8260_write(dev_t device);
static void sr8260_ioctl(dev_t device);

struct dev sr8260 = { "sr8260", sr8260_open, sr8260_read, sr8260_write, sr8260_ioctl };

static void
sr8260_open(dev_t device, int flags)
{

}

static void
sr8260_read(dev_t device)
{

}

static void
sr8260_write(dev_t device)
{

}

static void
sr8260_ioctl(dev_t device)
{

}
