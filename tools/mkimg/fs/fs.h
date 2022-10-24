#ifndef FS_FS_H
#define FS_FS_H 1

struct fs {
	int (*init)(void);
	int (*write)(const char *path);
};

#endif /* FS_FS_H */
