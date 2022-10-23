#include <pm/pm.h>

static int raw_format(const char *template);
static int raw_addpart(int type, unsigned long int start, unsigned long int length);

struct pm raw_pm = {
	"raw",
	raw_format,
	raw_addpart
};

static int
raw_format(const char *template)
{
	return 0;
}

static int
raw_addpart(int type, unsigned long int start, unsigned long int length)
{
	return 0;
}
