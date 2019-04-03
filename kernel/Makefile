#
# Makefile for scos kernel
#

# Entry of the kernel
ENTRY = 0x100000

# Compilers and parameters
ASM = nasm
ASMFLAGS = -f elf
CC = gcc
CCFLAGS = -I include -c -fno-builtin -Wall
CCDEPFLAGS = -M -I include
ASMDEPFLAGS = -M -f elf
LD = ld
LDFLAGS = -s -Ttext $(ENTRY)

# Every part of kernel
# First must have _start signal
KERNELPARTS = asm_setup.o asm_machine.o machine.o scos.o asm_util.o \
util.o sched.o asm_sched.o debug.o asm_debug.o syscalls.o asm_syscalls.o \
mm.o asm_mm.o floppy.o asm_floppy.o hd.o asm_hd.o $(CHRDEVPARTS) \
$(BLKDEVPARTS)

.PHONY = kernel remove dep

kernel : dep
	make -C chardevice
	make -C blockdevice
	make -f tmp_Makefile system.bin elftokernel
	rm -rf tmp_Makefile
	./elftokernel > /dev/null

elftokernel : tools/elftokernel.c
	$(CC) -o $@ $<

remove : 
	rm -rf *.o
	make -C chardevice remove
	make -C blockdevice remove
	rm -rf system.bin
	rm -rf elftokernel


dep : 
	(grep "CHRDEVPARTS = " chardevice/Makefile > tmp_Makefile)
	(echo "" >> tmp_Makefile)
	(grep "BLKDEVPARTS = " blockdevice/Makefile >> tmp_Makefile)
	(echo "" >> tmp_Makefile)
	(cat Makefile >> tmp_Makefile)
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


system.bin : $(KERNELPARTS)
	$(LD) $(LDFLAGS) *.o -o $@

# DEP HERE

