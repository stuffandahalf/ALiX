#ifndef ALIX_MOUNT_H
#define ALIX_MOUNT_H 1

#include <alix/dev.h>
#include <alix/device.h>
#include <alix/fs.h>
#include <alix/inode.h>

struct mount {
	dev_t m_dev;
	struct fs *m_fs;
	struct inode *m_in;
	void *m_config;
};

#endif /* ALIX_MOUNT_H */
