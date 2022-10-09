#ifndef ALIX_BUS_H
#define ALIX_BUS_H 1

#include <stddef.h>

struct device;

typedef long int bus_target_t;


#define BUS_MESSAGE_INT 1
#define BUS_MESSAGE_BLOCK 2
#define BUS_MESSAGE_TYPE(msg) (msg->type & 3)

#define BUS_MESSAGE_INT8 (1 << 3)
#define BUS_MESSAGE_INT16 (2 << 3)
#define BUS_MESSAGE_INT32 (3 << 3)
#define BUS_MESSAGE_INT64 (4 << 3)
#define BUS_MESSAGE_INT_SZ(msg) (msg->type & (7 << 3))

struct bus_message {
	unsigned int type;
	union {
		unsigned long int i;
		struct {
			size_t len;
			void *ptr;
		} blk;
	};
};

struct bus {
	const char *name;
	int (*enumerate)(struct bus *bus);
	int (*send)(bus_target_t target, struct bus_message *);
	int (*receive)(bus_target_t target, struct bus_message *);
};

#endif /* ALIX_BUS_H */

