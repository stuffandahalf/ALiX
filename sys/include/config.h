#ifndef CONFIG_H
#define CONFIG_H 1

#include <stdint.h>
#include <alix/dev.h>

extern const struct dev *root_devs[];
extern const size_t root_devs_sz;

extern uintptr_t mmap_reserve_ranges[][2];
extern const size_t mmap_reserve_ranges_sz;

void anounce(void);

#endif
