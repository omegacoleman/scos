
# The Makefile for SCOS

.PHONY = all image remove
VERSION = 0.03

all: image hard

hard:
	dd if=/dev/zero of=c.img bs=512 count=20160

image : 
	make -C kernel
	mv kernel/sysmodule.scmd boot/
	make -C boot VER=$(VERSION)
	mv boot/SCOS.img SCOS.img

remove :
	make -C boot remove
	make -C kernel remove
	rm -rf SCOS.img
	rm -rf c.img

qemutest: image
	qemu -fda SCOS.img -hda c.img -boot once=a
