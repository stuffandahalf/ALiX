#include <stddef.h>
#include <stdint.h>

#include <alix/fs.h>
#include <alix/log.h>
#include <alix/mem.h>
#include <alix/mount.h>
#include <alix/util.h>

extern struct fs devfs;

struct inode root = {

};
LIST(struct mount *) mounts = LIST_INIT;

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
	const char *d1 = fname, *d2;

	do {
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

		for (d2 = d1; *d2 != '\0' && *d2 != '/'; d2++) {
			klogc(*d2);
		}
		if (d2 != d1) {
			klogc('\n');
		}

		ino = m->fs->geti(m, ino->inum);

		d1 = d2;
		while (*d1 == '/') {
			d1++;
		}
	} while (*d1 != '\0');

	return ino;
}
