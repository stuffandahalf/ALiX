.POSIX:

#CFLAGS+=-ffreestanding -nostdlib -c
CFLAGS+=-c -m32
#LDFLAGS+=-m32 -Tlinker.ld -nostdlib
LDFLAGS+=-Tlinker.ld

all: vbr.fat32.bin vbr.fat16.bin vbr.fat12.bin

vbr.fat32.bin: vbr.fat32.elf
	objcopy -O binary vbr.fat32.elf vbr.fat32.bin

vbr.fat32.elf: vbr.fat32.o ../read.o ../boot_sig.o
	$(LD) $(LDFLAGS) -o vbr.fat32.elf vbr.fat32.o ../read.o ../boot_sig.o

vbr.fat32.o: vbr.fat.s
	$(AS) $(ASFLAGS) --defsym FAT_VERSION=32 -o vbr.fat32.o vbr.fat.s

vbr.fat16.bin: vbr.fat16.elf
	objcopy -O binary vbr.fat16.elf vbr.fat16.bin

vbr.fat16.elf: vbr.fat16.o ../read.o ../boot_sig.o
	$(LD) $(LDFLAGS) -o vbr.fat16.elf vbr.fat16.o ../read.o ../boot_sig.o

vbr.fat16.o: vbr.fat.s
	$(AS) $(ASFLAGS) --defsym FAT_VERSION=16 -o vbr.fat16.o vbr.fat.s

vbr.fat12.bin: vbr.fat12.elf
	objcopy -O binary vbr.fat12.elf vbr.fat12.bin

vbr.fat12.elf: vbr.fat12.o ../read.o ../boot_sig.o
	$(LD) $(LDFLAGS) -o vbr.fat12.elf vbr.fat12.o ../read.o ../boot_sig.o

vbr.fat12.o: vbr.fat.s
	$(AS) $(ASFLAGS) --defsym FAT_VERSION=12 -o vbr.fat12.o vbr.fat.s

#read.bin: ../read.o
#	$(LD) $(LDFLAGS) --oformat binary -o read.bin ../read.o

test.bin: test.o ../read.o ../boot_sig.o
	$(LD) $(LDFLAGS) --oformat binary -o test.bin test.o ../read.o ../boot_sig.o

clean:
	rm -f vbr.fat32.bin vbr.fat32.elf vbr.fat32.o
	rm -f vbr.fat16.bin vbr.fat16.elf vbr.fat16.o
	rm -f vbr.fat12.bin vbr.fat12.elf vbr.fat12.o
	rm -f ../read.o
	rm -f ../boot_sig.o
	rm -f ../print.o
	rm -f test.bin test.o
