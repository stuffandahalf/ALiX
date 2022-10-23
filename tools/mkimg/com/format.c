#include <stddef.h>

#include <config.h>
#include <pm/mbr.h>

const char *pms = {
	"mbr",
	//~ "gpt",
	//~ "apm"
};
size_t pmsc = sizeof(pms) / sizeof(const char *);

static int help(int argc, char **argv);

const struct command subcmds[] = {
	{ "help", 'h', help }
};

int
format(int argc, char **argv)
{
	return 0;
}

static int
help(int argc, char **argv)
{
	return 1;
}
