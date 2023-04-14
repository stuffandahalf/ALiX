#ifndef ALIX_FS_H
#define ALIX_FS_H 1

#include <alix/dev.h>
#include <alix/mount.h>
#include <sys/types.h>

#define INODE_TYPE_FILE 0
#define INODE_TYPE_DIR 1
#define INODE_TYPE_LINK 2
#define INODE_TYPE_SPECIAL 3
struct inode {
	ino_t inum;
	unsigned char type;
	// struct fs *filesystem;
	struct mount *mnt;
	void *metadata;
};

#define FS_OPEN_FILE 0
#define FS_OPEN_DIR 1

struct fs {
	int (*mount)(struct mount *mnt, dev_t device);
	struct inode *(*geti)(struct mount *mnt, ino_t inum);
	int (*read)(struct inode *ino, void *buf, size_t count);
	int (*write)(struct inode *ino, void *buf, size_t count);
};

int mount(struct inode *ino, dev_t device, struct fs *filsys);
struct inode *geti(const char *fname);

#endif /* ALIX_FS_H */
