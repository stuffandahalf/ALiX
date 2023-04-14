#ifndef FS_FS_H
#define FS_FS_H 1

#include <stdint.h>

#include <alix/inode.h>

struct fs {
	struct fs_type {
		uint8_t mbr;
		uint8_t gpt[16];
		const char *apm;
	} type;
	const char *name;
	int (*init)(dev_t d);
	int (*read)(dev_t d, struct inode *ip, void *buf, size_t n);
	int (*write)(dev_t d, struct inode *ip, void *buf, size_t n);
};

#endif /* FS_FS_H */
