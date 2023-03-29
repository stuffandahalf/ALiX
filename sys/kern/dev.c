#include <stddef.h>
#include <config.h>
#include <alix/dev.h>
#include <alix/device.h>

struct device *devtree = NULL;

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
create_dev(struct dev *driver)
{
	return NULL;
}

void
destroy_dev(dev_t d)
{

}
