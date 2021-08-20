.POSIX:

LDFLAGS=-T linker.ld

all: boot0.bin

boot0.elf: boot0.o
	$(LD) $(LDFLAGS) -o boot0.elf boot0.o
	strip --remove-section=.note.gnu.property boot0.elf

boot0.bin: boot0.elf
	objcopy -O binary boot0.elf boot0.bin

debug: boot0.bin
	qemu-system-i386 -hda boot0.bin -S -s &
	gdb boot0.elf -ex 'target remote localhost:1234'

clean:
	rm -f boot0.bin boot0.elf boot0.o
