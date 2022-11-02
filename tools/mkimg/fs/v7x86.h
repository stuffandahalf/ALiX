#ifndef FS_V7X86_H
#define FS_V7X86_H 1

#include <stdint.h>

#define MBR_PART_TYPE_V7X86 0x72

#define V7X86_NICINOD 100
#define V7X86_NICFREE 50
#define V7X86_ROOTINO ((v7x86_ino_t)2)
#define V7X86_BSIZE 512

typedef uint32_t v7x86_daddr_t;
typedef uint16_t v7x86_ino_t;
typedef uint32_t v7x86_time_t;
typedef uint32_t v7x86_off_t;

struct v7x86_superblk {
	uint16_t s_isize; /* size in blocks of i-list */
	v7x86_daddr_t s_fsize; /* size in blocks of entire volume */
	int16_t s_nfree; /* number of addresses in s_free */
	v7x86_daddr_t s_free[V7X86_NICFREE]; /* free block list */
	int16_t s_ninode; /* number of i-nodes in s_inode */
	v7x86_ino_t s_inode[V7X86_NICINOD]; /* free i-node list */
	int8_t s_flock; /* lock during free list manipulation */
	int8_t s_ilock; /* lock during i-list manipulation */
	int8_t s_fmod; /* super block modified flag */
	int8_t s_ronly; /* mounted read-only flag */
	v7x86_time_t s_time; /* last super block update */

	v7x86_daddr_t s_tfree; /* total free blocks*/
	v7x86_ino_t s_tinode; /* total free inodes */
	int16_t s_m; /* interleave factor */
	int16_t s_n; /* " " */
	char s_fname[6]; /* file system name */
	char s_fpack[6]; /* file system pack name */
};

struct v7x86_dinode {
	uint16_t di_mode; /* mode and type of file */
	int16_t di_nlink; /* number of links to file */
	int16_t di_uid; /* owner's user id */
	int16_t di_gid; /* owner's group id */
	v7x86_off_t di_size; /* number of bytes in file */
	char di_addr[40]; /* disk block addresses */
	v7x86_time_t di_atime; /* time last accessed */
	v7x86_time_t di_mtime; /* time last modified */
	v7x86_time_t di_ctime; /* time created */
};
#define INOPB   8       /* 8 inodes per block */

#ifndef	DIRSIZ
#define	DIRSIZ	14
#endif
struct v7x86_direct
{
	v7x86_ino_t d_ino;
	char d_name[DIRSIZ];
};

#endif /* FS_V7X86_H */
