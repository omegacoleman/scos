#ifndef _MACHINE_H_
#define _MACHINE_H_

#include "mm.h"

#define ERR_START_INT 0       /** Error ints start&end nr **/
#define ERR_END_INT 16
#define ERRS (ERR_END_INT - ERR_START_INT + 1) /** Err int nr. **/
#define HZ 1000   /** Clock tick int : 1000hz. **/
#define CLOCKS_PER_SEC 100 /** Jiffies one second add 1. **/
#define GDT_CALLABLE 0x8  /** Selectors. **/
#define GDT_RW 0x10
#define GDT_LDT 0x18
#define GDT_TSS_START 0x20
#define LDT_CALLABLE 0xf
#define LDT_RW 0x17
#define GDT_TSS(index) (GDT_TSS_START + (index * 0x8))
#define SYSCALL_INT 0x4f   /** The system call int nr. **/
#define TIME_IRQ 0x20   /** Clock, keyboard, etc int nr **/
#define KEYBOARD_IRQ 0x21
#define BUG_LEVEL_HINT 0x77 /** Bug levels, for reportbug() **/
#define BUG_LEVEL_WARNING 0  /** 0x77:won't halt ; 0,1,2 : will halt **/
#define BUG_LEVEL_ERROR 1
#define BUG_LEVEL_FAULT 2


/**
 * It's a 386 tss typedef.
 */
typedef struct _state
{
    long backlink;
    long esp0;
    long ss0;
    long esp1;
    long ss1;
    long esp2;
    long ss2;
    long cr3;
    long eip;
    long eflags;
    long eax;
    long ecx;
    long edx;
    long ebx;
    long esp;
    long ebp;
    long esi;
    long edi;
    long es;
    long cs;
    long ss;
    long ds;
    long fs;
    long gs;
    long ldt;
    long bmp;
} state;

/**
 * A descriptor is 8 bytes long.
 */
typedef unsigned long descriptor[2];

/**
 * The TSS descriptors in GDT.
 */
extern descriptor TSS_DESC[];
/**
 * Only one ldt is okay.
 */
extern descriptor LDT_DESC;

/**
 * Init everything defined in machine.c
 * for the machine.
 */
void machineinit();
/**
 * Set the normal int process method.
 */
void setdoint(long index, long address);
/**
 * {Enable,Disable} the interrupt.
 */
void unmaskirq(unsigned int irq);
void maskirq(unsigned int irq);
/**
 * TSS init method.
 */
void inittaskstate(state *tss, long entry, pagingdata *paging);
/**
 * Load the tss desc to correct location.
 */
void reloadstate(unsigned long pid, unsigned long *tssdesc);
/**
 * TSS desc init method.
 */
void inittssdesc(descriptor desc, state *tss);
/**
 * Stop the current and change into another.
 */
void goproc(unsigned long pid);
/**
 * Report any bugs.
 */
void reportbug(unsigned int level, char *message);
/**
 * A function to change color at cursor, looks beautiful!!
 */
void light();

#endif

