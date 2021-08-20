#ifndef MBR_H
#define MBR_H	1

#define MBR_PARTTAB_OFFSET  446

#define MBR_ID_V7X86FS	0x72

struct mbr_part {
    unsigned char status;
    unsigned char s_chs[3];
    unsigned char type;
    unsigned char l_chs[3];
    unsigned int lba;
    unsigned int nsec;
} __attribute__((__packed__));;

struct mbr_part *mbr_find_part(unsigned char id);

#endif /* MBR_H */

