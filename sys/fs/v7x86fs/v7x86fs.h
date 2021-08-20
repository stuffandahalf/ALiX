#ifndef FS_V7FS_H
#define FS_V7FS_H	1

/*#include <stdint.h>*/
/*#include <sys/types.h>*/

#define NICFREE	50
#define NICINOD	100

#include <stdint.h>

#define V7FS_ROOT_INODE	2

typedef uint16_t v7fs_ino_t;
typedef int32_t v7fs_daddr_t;
typedef uint32_t v7fs_time_t;
typedef int32_t v7fs_off_t;

struct v7fs_superblock {
	uint16_t s_isize;	/* size in blocks of i-list */
	v7fs_daddr_t s_fsize;	/* size in blocks of entire volume */
	int16_t s_nfree;	/* number of addresses in s_free */
	v7fs_daddr_t s_free[NICFREE]; /* free block list */
	int16_t s_ninode;	/* number of i-nodes in s_inode */
	v7fs_ino_t s_inode[NICINOD];	/* free i-node list */
	uint8_t s_flock;	/* lock during free list manipulation */
	uint8_t s_ilock;	/* lock during i-list manipulation */
	uint8_t s_fmod;	/* super block modified flag */
	uint8_t s_ronly;	/* mounted read-only floag */
	v7fs_time_t s_time;	/* last super block update */
	/* remainder not maintained by this version of the system */
	v7fs_daddr_t s_tfree;	/* total free blocks */
	v7fs_ino_t s_tinode;	/* total free inodes */
	int16_t s_m;	/* interleave factor */
	int16_t s_n;	/* " " */
	char s_fname[6];	/* file system name */
	char s_fpack[6];	/* file system pack name */
} __attribute__((__packed__));

#define INOPB	8
struct v7fs_dinode {
	uint16_t di_mode;	/* mode and type of file */
	int16_t di_nlink;	/* number of links to file */
	int16_t di_uid;	/* owner's user id */
	int16_t di_gid;	/* owner's group id */
	v7fs_off_t di_size; /* number of bytes in file */
	char di_addr[40]; /* disk block addresses */
	v7fs_time_t di_atime; /* time last accessed */
	v7fs_time_t di_mtime; /* time last modified */
	v7fs_time_t di_ctime; /* time created */
} __attribute__((__packed__));
/*
 * the 40 address bytes:
 *	39 used; 13 addresses
 *	of 3 bytes each.
 */

#define V7FS_NAME_MAX	14
struct v7fs_dirent {
	v7fs_ino_t d_ino;
	char d_name[V7FS_NAME_MAX];
} __attribute__((__packed__));

#endif /* FS_V7FS_H */

