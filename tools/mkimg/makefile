.POSIX:

CFLAGS+=-D_POSIX_C_SOURCE=200112L -I.

TARGET=mkimg
OBJS=main.o \
	pm/mbr.o \
	fs/fat.o \
	com/format.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJS)

clean:
	rm -f $(TARGET)
	rm -f $(OBJS)


