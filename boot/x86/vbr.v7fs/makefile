.POSIX:

all: vbr.v7fs.bin

vbr.v7fs.bin: vbr.v7fs.elf
	objcopy -O binary vbr.v7fs.elf vbr.v7fs.bin

vbr.v7fs.elf: vbr.v7fs.o
	$(LD) $(LDFLAGS) -T linker.ld -o vbr.v7fs.elf vbr.v7fs.o

vbr.v7fs.o: vbr.v7fs.s ../read.s ../print.s
	$(AS) $(ASFLAGS) -o vbr.v7fs.o vbr.v7fs.s

clean:
	rm -f vbr.v7fs.bin vbr.v7fs.elf vbr.v7fs.o

