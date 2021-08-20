#ifndef DEV_H
#define DEV_H	1

#define SECTOR_SZ	512

int dev_read(FILE *fp, unsigned long long lba, size_t count, char *buffer);

#endif /* DEV_H */
