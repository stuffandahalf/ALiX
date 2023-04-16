#ifndef ALIX_FS_H
#define ALIX_FS_H 1

#include <alix/config.h>
#include <alix/dev.h>
#include <alix/mount.h>
#include <alix/types.h>

#define INODE_FLAG_TYPE_FILE (1 << 10)
#define INODE_FLAG_TYPE_DIR (2 << 10)
#define INODE_FLAG_TYPE_LINK (3 << 10)
#define INODE_FLAG_TYPE_SPECIAL (4 << 10)
#define INODE_FLAG_PERM_READ (1 << 2)
#define INODE_FLAG_PERM_WRITE (1 << 1)
#define INODE_FLAG_PERM_EXEC (1 << 0)
#define INODE_FLAG_TYPE(flag) ((flag >> 10) & 3)
#define USER_PERMS(p) (p << 6)
#define GROUP_PERMS(p) (p << 3)
#define OTHER_PERMS(p) (p << 0)
struct inode {
	ino_t inum;
	unsigned short flags;
	struct mount *mnt;
	void *fsdat;
	uid_t owner;
	gid_t group;
};

struct file {
	struct inode *ino;
	unsigned short mode;
	unsigned short refc;
	union {
		struct {
			unsigned int i;
		} d;
		struct {

		} f;
	} state;
};

struct dirent {
	ino_t d_ino;
	char d_name[FILE_NAME_LIMIT];
};

struct fs {
	int (*mount)(struct mount *mnt, dev_t device);
	struct inode *(*geti)(struct mount *mnt, ino_t inum);
	int (*readdir)(struct file *fp, struct dirent *dp);
	// int (*read)(struct file *fp, void *buf, size_t count);
	// int (*write)(struct file *fp, void *buf, size_t count);
};

int mount(struct inode *ino, dev_t device, struct fs *filsys);
struct inode *geti(const char *fname);
struct file *open(struct inode *ino, int flags);
void close(struct file *fp);

#endif /* ALIX_FS_H */
