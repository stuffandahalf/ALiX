#ifndef PM_PM_H
#define PM_PM_H 1

struct pm {
	const char *name;
	int (*format)(const char *template);
	int (*partadd)(int type, unsigned long int start, unsigned long int length);
};

#endif /* PM_PM_H */
