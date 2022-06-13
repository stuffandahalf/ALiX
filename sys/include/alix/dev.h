#ifndef ALIX_DEV_H
#define ALIX_DEV_H 1

typedef struct device *dev_t;

struct dev {
	const char *name;
	int (*open)(dev_t device, int flags);
	int (*close)(dev_t device);
	int (*read)(dev_t device);
	int (*write)(dev_t device, char c);
	void (*ioctl)(dev_t device);
};

#endif /* ALIX_DEV_H */
