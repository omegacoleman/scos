#
# Makefile for scos block device support
#

# Compilers and parameters
ASM = nasm
ASMFLAGS = -f elf
CC = gcc
CCFLAGS = -I ../include -c -fno-builtin -Wall
CCDEPFLAGS = -M -I ../include
ASMDEPFLAGS = -M -f elf

BLKDEVPARTS = asm_floppy.o floppy.o asm_hd.o hd.o block.o

.PHONY = ofiles all remove dep

ofiles : dep
	make -f tmp_Makefile all
	rm -rf tmp_Makefile
	cp $(BLKDEVPARTS) ../

all : $(BLKDEVPARTS)

remove : 
	rm -rf $(BLKDEVPARTS)

dep : 
	cp Makefile tmp_Makefile -f
	(for i in *.c; \
	do \
	$(CC) $(CCDEPFLAGS) $$i >> tmp_Makefile; \
	echo "	$(CC) $(CCFLAGS) $$i" >> tmp_Makefile; \
	echo  >> tmp_Makefile; \
	done)
	(for i in *.asm; \
	do \
	echo -n "asm_" >> tmp_Makefile; \
	$(ASM) $(ASMDEPFLAGS) $$i >> tmp_Makefile; \
	echo -n "	$(ASM) $(ASMFLAGS) $$i" >> tmp_Makefile; \
	echo -e " -o \x24\x40" >> tmp_Makefile; \
	echo  >> tmp_Makefile; \
	done)

# DEP HERE

