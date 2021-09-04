#!/bin/sh

dd if=./vbr.v7fs.bin of=hd0.img conv=notrunc bs=512 seek=63
qemu-system-i386 -hda hd0.img -s -S -curses

