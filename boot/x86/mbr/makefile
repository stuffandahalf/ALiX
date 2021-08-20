.POSIX:

all: mbr.bin

mbr.bin: mbr.elf
	objcopy -O binary mbr.elf mbr.bin

mbr.elf: linker.ld mbr.o
	$(LD) $(LDFLAGS) -T linker.ld -o mbr.elf mbr.o

mbr.o: mbr.s ../read.s
	$(AS) $(ASFLAGS) -o mbr.o mbr.s

clean:
	rm -f mbr.bin mbr.elf mbr.o


