.POSIX:

CFLAGS+=-I../include/

OBJECTS=main.o \
		malloc.o

all: kern.a

kern.a: $(OBJECTS)
	$(AR) $(ARFLAGS) kern.a $(OBJECTS)

clean:
	rm -f kern.a $(OBJECTS)

