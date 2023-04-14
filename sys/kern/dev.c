#include <stddef.h>

#include <config.h>
#include <alix/dev.h>
#include <alix/device.h>
#include <alix/mem.h>
#include <alix/log.h>
#include <alix/util.h>

#define DEVTAB_INCSZ 5;
#define DEVINST_INCSZ 3;

// struct device *devtree = NULL;
// struct device **devtab = NULL;
struct dev **devtab = NULL;
size_t devtab_sz = 0, devtab_len = 0;

// LIST(struct dev) devtab = LIST_INIT;

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

	for (i = 0; i < devtab_len && !present; i++) {
		present = devtab[i] == driver;
	}

	if (!present) {
		if (devtab_sz == devtab_len) {
			devtab_sz += DEVTAB_INCSZ;
			devtab = krealloc(devtab, sizeof(struct dev) * devtab_sz);
			if (!devtab) {
				PANIC();
			}
		}

		devtab[devtab_len] = driver;
		devtab_len++;
	}

	for (i = 0; i < driver->instances.length && driver->instances.list[i] != NULL; i++);
	d->name = mkdevnm(driver, i);
	klogs(d->name);
	klogc('\n');
	LIST_INSERT(dev_t, &driver->instances, i, d);

	// for (devnmlen = 0; driver->name[devnmlen] != '\0'; devnmlen++);
	// for (j = 1; i / j > 0; j++);
	// devnmlen += j + 1;
	
	// d->name = kalloc(sizeof(char) * (devnmlen + j + 1));
	// for (i = 0; i < devnmlen; i++) {
	// 	d->name[i]
	// }


	return d;
}

void
destroy_dev(dev_t device)
{
	NOT_IMPLEMENTED();
}
