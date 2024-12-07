#ifndef ALIX_DEV_CLASS_TTY_H
#define ALIX_DEV_CLASS_TTY_H 1

#include <stdint.h>

#include <alix/dev.h>

#define TTY_BAUD_0 0
#define TTY_BAUD_50 50
#define TTY_BAUD_75 75
#define TTY_BAUD_110 110
#define TTY_BAUD_134 134
#define TTY_BAUD_150 150
#define TTY_BAUD_200 200
#define TTY_BAUD_300 300
#define TTY_BAUD_600 600
#define TTY_BAUD_1200 1200
#define TTY_BAUD_1800 1800
#define TTY_BAUD_2400 2400
#define TTY_BAUD_4800 4800
#define TTY_BAUD_9600 9600
#define TTY_BAUD_19200 19200
#define TTY_BAUD_38400 38400
#define TTY_BAUD_57600 57600
#define TTY_BAUD_115200 115200
#define TTY_BAUD_230400 230400
#define TTY_BAUD_460800 460800
#define TTY_BAUD_500000 500000
#define TTY_BAUD_576000 576000
#define TTY_BAUD_921600 921600
#define TTY_BAUD_1000000 1000000
#define TTY_BAUD_1152000 1152000
#define TTY_BAUD_1500000 1500000
#define TTY_BAUD_2000000 2000000

/* TODO: set correct SPARC constant */
#ifdef __SPARC__
#define TTY_BAUD_76800 76800
#define TTY_BAUD_153600 153600
#define TTY_BAUD_307200 307200
#define TTY_BAUD_614400 614400
#else
#define TTY_BAUD_2500000 2500000
#define TTY_BAUD_3000000 3000000
#define TTY_BAUD_3500000 3500000
#define TTY_BAUD_4000000 4000000
#endif

struct tty_util {
	void (*setbaud)(dev_t device, unsigned long int);
	unsigned long int (*getbaud)(dev_t device);
};

struct tty_state {
	uint32_t baud;
	struct tty_util *util;
};

int tty_init(dev_t device, struct tty_util *utils);
void tty_destroy(dev_t device);

#endif /* ALIX_DEV_CLASS_TTY_H */
