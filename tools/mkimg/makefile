.POSIX:

CFLAGS+=-D _POSIX_C_SOURCE=200112L -I .

TARGET=mkimg
OBJS=main.o \
	pm/raw.o pm/mbr.o \
	fs/fat.o \
	com/size.o com/format.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJS)

clean:
	rm -f $(TARGET)
	rm -f $(OBJS)


