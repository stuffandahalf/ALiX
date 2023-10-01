#ifndef ALIX_DEV_ATA_H
#define ALIX_DEV_ATA_H 1

#define ATA_IO_PORT_COUNT 8
#define ATA_CTRL_PORT_COUNT 2
#define ATA_PORT_COUNT (ATA_IO_PORT_COUNT + ATA_CTRL_PORT_COUNT)

#define ATA_PORT_DATA 0
#define ATA_PORT_ERROR 1
#define ATA_PORT_FEATURES 1
#define ATA_PORT_SEC_COUNT 2
#define ATA_PORT_SEC_NUM 3
#define ATA_PORT_CYL_LO 4
#define ATA_PORT_CYL_HI 5
#define ATA_PORT_LBA_LO 3
#define ATA_PORT_LBA_MD 4
#define ATA_PORT_LBA_HI 5
#define ATA_PORT_DRV_HEAD 6
#define ATA_PORT_STATUS 7
#define ATA_PORT_COMMAND 7

#define ATA_PORT_ALT_STATUS 8
#define ATA_PORT_CONTROL 8
#define ATA_PORT_ADDRESS 9

#define ATA_ERROR_AMNF (1 << 0)
#define ATA_ERROR_TKZNF (1 << 1)
#define ATA_ERROR_ABRT (1 << 2)
#define ATA_ERROR_MCR (1 << 3)
#define ATA_ERROR_IDNF (1 << 4)
#define ATA_ERROR_MC (1 << 5)
#define ATA_ERROR_UNC (1 << 6)
#define ATA_ERROR_BBK (1 << 7)

#define ATA_STATUS_ERR (1 << 0)
#define ATA_STATUS_IDX (1 << 1)
#define ATA_STATUS_CORR (1 << 2)
#define ATA_STATUS_DRQ (1 << 3)
#define ATA_STATUS_SRV (1 << 4)
#define ATA_STATUS_DF (1 << 5)
#define ATA_STATUS_RDY (1 << 6)
#define ATA_STATUS_BSY (1 << 7)

#define ATA_DRIVE_SEL_MASTER 0xa0
#define ATA_DRIVE_SEL_SLAVE 0xb0

extern struct dev ata;

#endif /* ALIX_DEV_ATA_H */
