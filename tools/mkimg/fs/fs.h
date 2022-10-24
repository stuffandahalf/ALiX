#ifndef FS_FS_H
#define FS_FS_H 1

struct fs {
	int type;
	const char *name;
	int (*init)(unsigned long int start, unsigned long int end);
	int (*write)(const char *path);
};

#endif /* FS_FS_H */
