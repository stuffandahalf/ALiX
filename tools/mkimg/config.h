#ifndef CONFIG_H
#define CONFIG_H 1

#include <stdio.h>

extern const char *ifname, *ofname;
extern FILE *ifp, *ofp;

struct command {
	const char *lname;
	const char sname;
	int (*handler)(int argc, char **argv);
};

extern int format(int argc, char **argv);
extern int size(int argc, char **argv);
extern int part(int argc, char **argv);

#endif /* CONFIG_H */
