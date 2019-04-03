#
# This file is the makefile for SCOS booting module
#

ASM = nasm
ASMFLAGS = 
CC = gcc
CCFLAGS = 

.PHONY = floppy remove

floppy : SCOS.img

floppysector.sec : floppysector.asm
	$(ASM) $(ASMFLAGS) -Dver="\"$(VER)\"" -o $@ $<

loader.bin : loader.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

SCOS.img : floppysector.sec loader.bin createfloppy
	./createfloppy

createfloppy : createfloppy.c
	$(CC) $(CCFLAGS) -o $@ $<

remove : 
	rm -rf floppysector.sec
	rm -rf createfloppy
	rm -rf sysmodule.scmd
	rm -rf loader.bin
