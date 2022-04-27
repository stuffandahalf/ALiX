#ifndef ALIX_X86_BIOS_H
#define ALIX_X86_BIOS_H 1

struct bios_data_area {
	unsigned short int com_ports[4];
	unsigned short int lpt_ports[3];
	unsigned short int ebda_address;
	unsigned short int hw;
	unsigned
};

#endif /* ALIX_X86_BIOS_H */
