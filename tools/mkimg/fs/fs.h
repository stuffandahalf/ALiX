#ifndef FS_FS_H
#define FS_FS_H 1

#include <stdint.h>

struct fs {
	struct fs_type {
		uint8_t mbr;
		uint8_t gpt[16];
		const char *apm;
	} type;
	const char *name;
	int (*init)(unsigned long int start, unsigned long int end);
	int (*write)(const char *path);
};

#endif /* FS_FS_H */
