#!/bin/sh

set -e -o xtrace

if [ $# -lt 1 ]; then
	exit 1
fi

FNAME=$1
shift 1

rm -f $FNAME.bin $FNAME.elf $FNAME.o
as -o$FNAME.o $FNAME
ld $@ -o$FNAME.elf $FNAME.o
objcopy -Obinary $FNAME.elf $FNAME.bin
ls -l $FNAME.bin | cut -d' ' -f5

