#include <stddef.h>
#include <config.h>
#include <alix/dev.h>
#include <alix/device.h>
#include <alix/mem.h>
#include <alix/log.h>

// struct device *devtree = NULL;
struct device **devtab = NULL;
size_t devtab_sz = 0;

int
init_dev(void)
{
	long int i;
	for (i = 0; i < root_devs_sz; i++) {
		root_devs[i]->attach(NULL);
	}


	return 0;
}

dev_t
create_dev(struct dev *driver, unsigned int nchannels)
{
	struct device *d;

	d = kalloc(sizeof(struct device));
	if (!d) {
		klogs("Failed to allocate ");
		kloglu(sizeof(struct device), 10);
		klogs(" bytes\n");
		return NULL;
	}

	// kfree(d);

	// return NULL;
	return d;
}

void
destroy_dev(dev_t d)
{

}
