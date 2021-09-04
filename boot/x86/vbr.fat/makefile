#CFLAGS+=-ffreestanding -nostdlib -c
CFLAGS+=-c -m32
#LDFLAGS+=-m32 -Tlinker.ld -nostdlib
LDFLAGS+=-Tlinker.ld

.POSIX:

all: vbr.fat32.bin vbr.fat16.bin vbr.fat12.bin

vbr.fat32.bin: vbr.fat32.elf
	objcopy -O binary vbr.fat32.elf vbr.fat32.bin

vbr.fat32.elf: vbr.fat32.o
	$(LD) $(LDFLAGS) -o vbr.fat32.elf vbr.fat32.o

vbr.fat32.o: vbr.fat.s
	$(AS) $(ASFLAGS) --defsym FAT_VERSION=32 -o vbr.fat32.o vbr.fat.s

vbr.fat16.bin: vbr.fat16.elf
	objcopy -O binary vbr.fat16.elf vbr.fat16.bin

vbr.fat16.elf: vbr.fat16.o
	$(LD) $(LDFLAGS) -o vbr.fat16.elf vbr.fat16.o

vbr.fat16.o: vbr.fat.s
	$(AS) $(ASFLAGS) --defsym FAT_VERSION=16 -o vbr.fat16.o vbr.fat.s

vbr.fat12.bin: vbr.fat12.elf
	objcopy -O binary vbr.fat12.elf vbr.fat12.bin

vbr.fat12.elf: vbr.fat12.o
	$(LD) $(LDFLAGS) -o vbr.fat12.elf vbr.fat12.o

vbr.fat12.o: vbr.fat.s 
	$(AS) $(ASFLAGS) --defsym FAT_VERSION=12 -o vbr.fat12.o vbr.fat.s

clean:
	rm -f vbr.fat32.bin vbr.fat32.elf vbr.fat32.o
	rm -f vbr.fat16.bin vbr.fat16.elf vbr.fat16.o
	rm -f vbr.fat12.bin vbr.fat12.elf vbr.fat12.o

