#include <stddef.h>
#include <stdint.h>

#include <alix/device.h>
#include <alix/fs.h>
#include <alix/mem.h>
#include <alix/lib/string.h>

static int devfs_mount(struct mount *mnt, dev_t device);
static struct inode *devfs_geti(struct mount *mnt, ino_t inum);
static int devfs_readdir(struct file *fp, struct dirent *dp);
static int devfs_read(struct file *fp, void *buf, size_t count);
static int devfs_write(struct file *fp, void *buf, size_t count);

struct fs devfs = {
	devfs_mount,
	devfs_geti,
	// devfs_read,
	// devfs_write,
	devfs_readdir
};

extern LIST(dev_t) devices;

#define DEVFS_ROOT_INO ((ino_t)0)
#define DEVFS_PERMS ( \
	USER_PERMS(INODE_FLAG_PERM_READ) | \
	GROUP_PERMS(INODE_FLAG_PERM_READ) | \
	OTHER_PERMS(INODE_FLAG_PERM_READ) \
)

static int
devfs_mount(struct mount *mnt, dev_t device)
{
	mnt->rooti = devfs_geti(mnt, DEVFS_ROOT_INO);
	return 0;
}

static struct inode *
devfs_geti(struct mount *mnt, ino_t inum)
{
	struct inode *ino = NULL;

	if (inum != DEVFS_ROOT_INO && inum > devices.length) {
		return NULL;
	}

	ino = kalloc(sizeof(struct inode));
	if (!ino) {
		return NULL;
	}
	ino->inum = inum;
	ino->flags = DEVFS_PERMS;
	ino->mnt = mnt;
	ino->owner = 0; /* TODO: set constant root uid */
	ino->group = 0;

	if (inum == DEVFS_ROOT_INO) {
		ino->flags |= INODE_FLAG_TYPE_DIR;
	} else if (inum <= devices.length) {
		ino->flags |= INODE_FLAG_TYPE_SPECIAL;
	}

	return ino;
}

static int
devfs_readdir(struct file *fp, struct dirent *dp)
{
	dev_t d;
	size_t i, l;

	if (!(fp->ino->flags & INODE_FLAG_TYPE_DIR)) {
		return 1;
	}

	d = devices.list[fp->state.d.i];
	l = strlen(d->name) + 1;

	dp->d_ino = fp->state.d.i + 1;
	for (i = 0; i < l; i++) {
		dp->d_name[i] = d->name[i];
	}

	return 0;
}

static int
devfs_read(struct file *fp, void *buf, size_t count)
{
	return 0;
}

static int
devfs_write(struct file *fp, void *buf, size_t count)
{
	return 0;
}
