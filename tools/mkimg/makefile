.POSIX:

CFLAGS+=-D_POSIX_C_SOURCE=200112L -I. -I../../sys/include

TARGET=mkimg
OBJS=main.o util.o \
	pm/raw.o pm/mbr.o \
	fs/v7x86.o fs/fat.o \
	com/size.o com/format.o com/part.o

all: $(TARGET) driver

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJS)

driver: driver.o pm/mbr.o fs/v7x86.o
	$(CC) $(LDFLAGS) -o driver driver.o pm/mbr.o fs/v7x86.o

clean:
	rm -f $(TARGET)
	rm -f $(OBJS)
	rm -f driver driver.o


