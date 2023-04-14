#include <stddef.h>

#include <alix/fs.h>

static int devfs_mount(struct mount *mnt, dev_t device);
static struct inode *devfs_geti(struct mount *mnt, ino_t inum);
static int devfs_read(struct inode *ino, void *buf, size_t count);
static int devfs_write(struct inode *ino, void *buf, size_t count);

struct fs devfs = {
	devfs_mount,
	devfs_geti,
	devfs_read,
	devfs_write
};

extern LIST(dev_t) devtab;

static int
devfs_mount(struct mount *mnt, dev_t device)
{
	return 0;
}

static struct inode *
devfs_geti(struct mount *mnt, ino_t inum)
{
	// struct inode *ino;
	return NULL;
}

static int
devfs_read(struct inode *ino, void *buf, size_t count)
{
	return 0;
}

static int
devfs_write(struct inode *ino, void *buf, size_t count)
{
	return 0;
}
