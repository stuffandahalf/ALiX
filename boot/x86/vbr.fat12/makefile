.POSIX:

OBJS=vbr.fat12.o ../read.o ../boot_sig.o

all: vbr.fat12.bin

vbr.fat12.bin: vbr.fat12.elf
	objcopy -O binary vbr.fat12.elf vbr.fat12.bin

vbr.fat12.elf: linker.ld $(OBJS)
	$(LD) $(LDFLAGS) -Tlinker.ld -ovbr.fat12.elf $(OBJS)

test.bin: vbr.fat12.bin fat_writer
	dd if=/dev/zero of=test.bin bs=512 count=2880
	mkfs.fat -F12 test.bin
	./fat_writer -F12 -bvbr.fat12.bin test.bin

clean:
	rm -f vbr.fat12.bin vbr.fat12.elf vbr.fat12.o
	rm -f ../read.o
	rm -f ../boot_sig.o
	rm -f ../print.o
	rm -f test.bin fat_writer
