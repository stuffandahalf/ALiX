#ifndef PM_PM_H
#define PM_PM_H 1

struct fs_type;

struct pm {
	const char *name;
	int (*format)(const char *template);
	int (*partadd)(int index, struct fs_type *type, unsigned long int start, unsigned long int length);
};

#endif /* PM_PM_H */
