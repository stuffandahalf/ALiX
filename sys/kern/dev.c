#include <stddef.h>

#include <alix/config.h>
#include <alix/dev.h>
#include <alix/device.h>
#include <alix/mem.h>
#include <alix/log.h>
#include <alix/util.h>

#define DEVTAB_INCSZ 5;
#define DEVINST_INCSZ 3;

// struct device *devtree = NULL;
// struct device **devtab = NULL;
// struct dev **devtab = NULL;
// size_t devtab_sz = 0, devtab_len = 0;

// LIST(struct dev) devtab = LIST_INIT;
LIST(struct dev*) drivers = LIST_INIT;
LIST(dev_t) devices = LIST_INIT;

int
init_dev(void)
{
	long int i;
	for (i = 0; i < root_devs_sz; i++) {
		root_devs[i]->attach(NULL);
	}


	return 0;
}

static char *
mkdevnm(struct dev *driver, int instance)
{
	char *name = NULL;
	int instbuf = 0;
	size_t i, drvlen = 0, instlen = 0;

	for (drvlen = 0; driver->name[drvlen] != '\0'; drvlen++);
	do {
		instbuf *= 10;
		instbuf += instance % 10;
		instance /= 10;
		instlen++;
	} while (instance > 0);

	name = kalloc(sizeof(char) * (drvlen + instlen + 1));
	if (!name) {
		return NULL;
	}
	for (i = 0; i < drvlen; i++) {
		name[i] = driver->name[i];
	}
	do {
		name[i++] = '0' + instbuf % 10;
		instbuf /= 10;
	} while (instbuf != 0);
	name[i] = '\0';

	return name;
}

dev_t
create_dev(struct dev *driver, unsigned int nchannels, dev_t parent)
{
	struct device *d;
	int present = 0;
	size_t devnmlen = 0;
	int i, j;

	d = kalloc(sizeof(struct device));
	if (!d) {
		klogs("Failed to allocate ");
		kloglu(sizeof(struct device), 10);
		klogs(" bytes\n");
		return NULL;
	}
	d->nchannels = nchannels;
	d->config = NULL;
	d->parent = parent;
	d->driver = driver;

	for (i = 0; i < drivers.length && !present; i++) {
		present = drivers.list[i] == driver;
	}
	if (!present) {
		LIST_APPEND(struct dev *, &drivers, driver);
	}

	/* find first free index of device instance to create device name */
	for (i = 0; i < driver->instances.length && driver->instances.list[i] != NULL; i++);
	d->name = mkdevnm(driver, i);

	LIST_INSERT(dev_t, &driver->instances, i, d);

#ifndef NDEBUG
	klogs("created device ");
	klogs(d->name);
	klogc('\n');
#endif

	return d;
}

void
add_dev(dev_t newdev)
{
	/* TODO: make sure device doesn't already exist */

	if (!newdev) {
		return;
	}

	LIST_APPEND(dev_t, &devices, newdev);

	klogs(newdev->name);
	klogs(" attached on ");
	if (newdev->parent) {
		klogs(newdev->parent->name);
	} else {
		klogs("root");
	}
	klogc('\n');
}

void
destroy_dev(dev_t device)
{
	NOT_IMPLEMENTED();
}
