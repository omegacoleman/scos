#
# Makefile for scos char device support
#

# Compilers and parameters
ASM = nasm
ASMFLAGS = -f elf
CC = gcc
CCFLAGS = -I ../include -c -fno-builtin -Wall
CCDEPFLAGS = -M -I ../include
ASMDEPFLAGS = -M -f elf

CHRDEVPARTS = asm_keyboard.o asm_screen.o vs.o screen.o keyboard.o

.PHONY = ofiles all remove dep

ofiles : dep
	make -f tmp_Makefile all
	rm -rf tmp_Makefile
	cp $(CHRDEVPARTS) ../

all : $(CHRDEVPARTS)

remove : 
	rm -rf $(CHRDEVPARTS)

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

