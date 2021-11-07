.POSIX:

SUBDIRS=mbr vbr.fat12 vbr.v7fs bootld

all: $(SUBDIRS:=.dir)

mbr.dir:
	cd mbr && $(MAKE) all

vbr.fat12.dir:
	cd vbr.fat12 && $(MAKE) all

vbr.v7fs.dir:
	cd vbr.v7fs && $(MAKE) all

clean:
	for SUBDIR in $(SUBDIRS); do cd $$SUBDIR && $(MAKE) clean && cd ..; done

