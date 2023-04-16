#include <stddef.h>
#include <stdint.h>

#include <alix/config.h>
#include <alix/fs.h>
#include <alix/log.h>
#include <alix/mem.h>
#include <alix/mount.h>
#include <alix/util.h>

extern struct fs devfs;

struct inode root = {
	.inum = 0,
	.mnt = NULL,
	.flags = INODE_FLAG_TYPE_DIR |
		USER_PERMS(INODE_FLAG_PERM_READ | INODE_FLAG_PERM_WRITE | INODE_FLAG_PERM_EXEC) |
		GROUP_PERMS(INODE_FLAG_PERM_READ | INODE_FLAG_PERM_EXEC) |
		OTHER_PERMS(INODE_FLAG_PERM_READ | INODE_FLAG_PERM_EXEC),
	.owner = 0,
	.group = 0
};
LIST(struct mount *) mounts = LIST_INIT;

struct file filepool[FILE_LIMIT];

int
init_vfs(void)
{
	struct inode *ino;
	/* establish root filesystem and mount */

	mount(&root, NULL, &devfs);
	ino = geti("/ttyNS0");
	klogs("ino: ");
	kloglu((uintptr_t)ino, 16);
	klogc('\n');

	ino = geti("/");
	klogs("ino: 0x");
	kloglu((uintptr_t)ino, 16);
	klogs(" mnt: 0x");
	kloglu((uintptr_t)ino->mnt, 16);
	klogc('\n');

	return 0;
}

int
mount(struct inode *ino, dev_t device, struct fs *filsys)
{
	struct mount *m;
	int i;

	for (i = 0; i < mounts.length; i++) {
		if (mounts.list[i]->rooti == ino) {
			return 1;
		}
	}
	m = kalloc(sizeof(struct mount));
	if (!m) {
		return 1;
	}
	m->dev = device;
	m->fs = filsys;
	m->mounti = ino;
	m->rooti = NULL;
	m->config = NULL;
	if (filsys->mount(m, device)) {
		kfree(m);
		return 1;
	}
	LIST_APPEND(struct mount *, &mounts, m);

	return 0;
}

struct inode *
geti(const char *fname)
{
	int i;
	struct inode *ino = &root;
	struct mount *m = NULL;
	struct file *fp;
	struct dirent dp;
	const char *d1 = fname, *d2;

	do {
		// if current inode is mounted on, set current inode to root of mount
		for (i = 0; i < mounts.length; i++) {
			if (mounts.list[i]->mounti == ino) {
				m = mounts.list[i];
				ino = m->rooti;
				break;
			}
		}
		if (!m) {
			return NULL;
		}

		// find path part
		for (d2 = d1; *d2 != '/' && *d2 != '\0'; d2++);

		// use filesystem function to retrieve inode
		// ino = m->fs->geti(m, ino->inum);
		if (ino->flags & INODE_FLAG_TYPE_DIR) {
			fp = open(ino, INODE_FLAG_PERM_READ);
			if (!fp) {
				return NULL;
			}
			while (!m->fs->readdir(fp, &dp)) {
				klogs(dp.d_name);

				// if (/* dir entry name matches */) {
				// 	ino = m->fs->get(m, d.d_ino);
				// 	break;
				// }
			}
			// ino = m->fs->readdir()
			close(fp);
		}

		// set up for next path part
		d1 = d2;
		while (*d1 == '/') {
			d1++;
		}
	} while (*d1 != '\0');

	return ino;
}

struct file *
open(struct inode *ino, int flags)
{
	return NULL;
}

void
close(struct file *fp)
{

}
