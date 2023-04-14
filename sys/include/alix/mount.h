#ifndef ALIX_MOUNT_H
#define ALIX_MOUNT_H 1

struct mount {
	dev_t dev;
	struct fs *fs;
	struct inode *mounti;
	struct inode *rooti;
	void *config;
};

#endif /* ALIX_MOUNT_H */
