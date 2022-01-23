.POSIX:

all:    libstub.a 

libstub.a: entry.o  
	$(AR) $(ARFLAGS) -o libstub.a entry.o 

entry.o : entry.c
	$(CC) $(CFLAGS) -c -o entry.o  entry.c

clean:
	rm -f entry.o
	rm -f stub

