.POSIX:

OBJS=bootld.o ../read.o ../print.o

all: bootld.bin

bootld.bin: bootld.elf
	objcopy -O binary bootld.elf bootld.bin

bootld.elf: linker.ld $(OBJS)
	$(LD) $(LDFLAGS) -Tlinker.ld -obootld.elf $(OBJS)

clean:
	rm -f bootld.bin bootld.elf bootld.o
	rm -f ../read.o
	rm -f ../print.o

