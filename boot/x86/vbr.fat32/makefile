.POSIX:

all: vbr.fat32.bin

vbr.fat32.bin: vbr.fat32.elf
	objcopy -O binary vbr.fat32.elf vbr.fat32.bin

vbr.fat32.elf: vbr.fat32.o
	$(LD) $(LDFLAGS) -T linker.ld -o vbr.fat32.elf vbr.fat32.o

vbr.fat32.o: vbr.fat32.s ../read.s ../print.s
	$(AS) $(ASFLAGS) -o vbr.fat32.o vbr.fat32.s

clean:
	rm -f vbr.fat32.bin vbr.fat32.elf vbr.fat32.o

