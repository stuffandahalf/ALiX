.POSIX:

CFLAGS+=-DNDEBUG=1 -D_POSIX_C_SOURCE=200112L -I../

OBJS=main.o dev.o mbr.o

all: testfs.v7x86fs

testfs.v7x86fs: $(OBJS)
	$(CC) $(LDFLAGS) -o testfs.v7x86fs $(OBJS)

clean:
	rm -f testfs.v7x86fs $(OBJS)

run: testfs.v7x86fs
	./testfs.v7x86fs $$HOME/hd0.img

