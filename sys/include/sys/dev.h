#ifndef ALIX_DEV_H
#define ALIX_DEV_H 1

struct cdev {
	int (*open)();
	int (*close)();
	int (*read)(char *buf, int n);
	int (*write)(char *buf, int n);
};

#endif /* ALIX_DEV_H */

