

#include "util.h"
#include "sched.h"
#include "mm.h"
#include "syscalls.h"
#include "machine.h"
#include "vs.h"

/**
 * The len of unknown int process code.
 */
#define NORMAL_INT_LEN 10 
/**
 * The time param write to 8253
 */
#define LATCH 11931800 / HZ

/**
 * Ldt for every process.
 */
descriptor normalldt[3] = \
{{0, 0},                             // Empty.
{0x00000fff, 0x00c0fa00},            // Callable segment, 0x3ff
{0x00000fff, 0x00c0f200}};           // Data segment, 0x3ff

/**
 * Ldt desc for every process.
 */
descriptor normalldtdesc;

/**
 * The hardware error messages.
 */
char messageserrs[ERRS][50] = {
"Devide error.", 
"Debug.", 
"Nmi.", 
"Breakpoint.",
"Overflow.",
"Bounds check.", 
"Invalid opcode.", 
"Device not available.", 
"Double fault.", 
"Coprocessor segment overrun.", 
"Invalid TSS", 
"Segment not present.", 
"Stack segment.", 
"General protection.",
"Page fault.", 
"Reserved.", 
"Coprocessor error."
};

// the error levels of hardware err ints.
unsigned int levelserrs[ERRS] = \
{0,0,2,1,1,0,0,2,2,0,0,0,0,0,0,0};

/**
 * Cpu init funcs.
 */
void init8259A();
void changegdt();
void changeidt();
void initidt();
void initldtdesc(descriptor desc, descriptor ldt[]);
/**
 * Int asm entries and c-funcs.
 */
void zerointentry();
void interrupted(unsigned int code);
void goerrs(unsigned int errcode);
void timeirq();
void syscallentry();
unsigned long syscalled(long esi, long edi, long ebp, long eax, long ebx, \
long ecx, long edx);
void restart();

/**
 * Init everything defined in this file 
 * for the machine.
 */
void machineinit()
{
    disableint();
    init8259A();
    changegdt();
    initidt();
    changeidt();
    setdoint(TIME_IRQ, (long) timeirq);
    setdoint(SYSCALL_INT, (long) syscallentry);
    initldtdesc(LDT_DESC, normalldt);
}

/**
 * Set normal interrupt gate to every
 * interrupt, that will only been used when
 * error -- hardware error (int0~16) or
 * software error (ints cannot understand).
 *
 * If it is a system call or 8259a irq, it
 * must reinit later, or it will just like
 * a normal ca't understand interrupt.
 */
void initidt()
{ 
    int i;
    for (i = 0; i < 0xFF; i++)
    {
        setdoint(i, \
        (((long) zerointentry) + i * NORMAL_INT_LEN));
    }
}

/**
 * Report hardware errors defined in header
 * file machine.h.
 */
void goerrs(unsigned int errcode)
{
    reportbug(levelserrs[errcode], messageserrs[errcode]);
}

/**
 * Report anything not right. If it is really
 * bad, halt with IF=0 (not interruptable).
 */
void reportbug(unsigned int level, char *message)
{
    char *type;
    switch (level)
    {
        case 0: type = "WARNING";break;
        case 1: type = "ERROR";break;
        case 2: type = "FAULT";break;
        case 0x77: type = "HINT";break; 
        default: type = "BUG";break;
    }
    printstr("\n");
    printstr(type);
    printstr(": ");
    printstr(message);
    if (level == 0x77)
    {
        return;
    }
    disableint();
    while (1);
}

/**
 * Send ICWs to 8259A M-S clips, move 
 * interrupts from the unable address
 * to 0x20 to 0x30.
 */
void init8259A()
{
    outbyte(0x20, 0x11);
    outbyte(0xA0, 0x11);
    outbyte(0x21, 0x20);
    outbyte(0xA1, 0x28);
    outbyte(0x21, 0x4);
    outbyte(0xA1, 0x2);
    outbyte(0x21, 0x1);
    outbyte(0xA1, 0x1);
    // Mask all.
    outbyte(0x21, 0xFF);
    outbyte(0xA1, 0xFF);
    // Init 8253 
    outbyte(0x43, 0x36);
    outbyte(0x40, lowof16(LATCH));
    outbyte(0x40, highof16(LATCH));
}

/**
 * Enable the interrupt.
 */
void unmaskirq(unsigned int irq)
{
    unsigned int masks;
    if (irq > 7)
    {
        irq -= 8;
        masks = inbyte(0xA1);
        masks -= masks & (1 << irq);
        outbyte(0xA1, masks);
    } else {
        masks = inbyte(0x21);
        masks -= masks & (1 << irq);
        outbyte(0x21, masks);
    }
}

/**
 * Disable the interrupt.
 */
void maskirq(unsigned int irq)
{
    unsigned int masks;
    if (irq > 7)
    {
        irq -= 8;
        masks = inbyte(0xA1);
        masks += (masks & (1 << irq)) > 0 ? 0 : (1 << irq);
        outbyte(0xA1, masks);
    } else {
        masks = inbyte(0x21);
        masks += (masks & (1 << irq)) > 0 ? 0 : (1 << irq);
        outbyte(0x21, masks);
    }
}

/**
 * Time interrupt method.
 */
void timeint()
{
    schedule();
}

/**
 * The interrupt method for any error
 * interrupts, including interrupts not
 * need and interrupts from hardware
 * error.
 */
void interrupted(unsigned int code)
{
    if ((code >= ERR_START_INT) && (code <= ERR_END_INT)) {
        goerrs(code);
    }
    else
    {
        char message[] = "Unexpect interrupt -- 0x::.\n";
        char scode[3];
        kstrcpy(scode, vsitoa(code, 16));
        char it = ' ';
        int i = -1;
        int ki = 0;
        while (it != '\0')
        {
            if (it == ':')
            {
                message[i] = scode[ki];
                ki = ki + 1;
            }
            i = i + 1;
            it = message[i];
        }
        reportbug(0x77, message);
    }
}

/**
 * Init the task desc.
 */
void inittssdesc(descriptor desc, state *tss)
{
    unsigned long tssstart=((unsigned int) tss);
    unsigned int tssdesc[4];
    tssdesc[0] = 0x0068;
    tssdesc[1] = lowof32(tssstart);
    tssdesc[2] = 0xe900 + lowof16(highof32(tssstart));
    tssdesc[3] = highof16(highof32(tssstart)) * 0x100;
    // Remember little-endian.
    desc[0] = connect16to32(tssdesc[1], tssdesc[0]);
    desc[1] = connect16to32(tssdesc[3], tssdesc[2]);
}

/**
 * Init the ldt desc.
 */
void initldtdesc(descriptor desc, descriptor ldt[])
{
    unsigned long ldtstart=((unsigned int) ldt);
    unsigned int ldtdesc[4];
    ldtdesc[0] = 0xffff;
    ldtdesc[1] = lowof32(ldtstart);
    ldtdesc[2] = 0xe200 + lowof16(highof32(ldtstart));
    ldtdesc[3] = highof16(highof32(ldtstart)) * 0x100;
    // Remember little-endian.
    desc[0] = connect16to32(ldtdesc[1], ldtdesc[0]);
    desc[1] = connect16to32(ldtdesc[3], ldtdesc[2]);
}

/**
 * Load the tss descriptor to gdt.
 */
void reloadstate(unsigned long pid, unsigned long *tssdesc)
{
    TSS_DESC[pid][0] = tssdesc[0];
    TSS_DESC[pid][1] = tssdesc[1];
}

/**
 * Init the task state (tss for i386).
 */
void inittaskstate(state *tss, long entry, pagingdata *paging)
{
    tss->backlink = 0;
    tss->esp0 = USER_KERNEL_STACK_TOP;
    tss->ss0 = GDT_RW;
    tss->eip = entry;
    tss->eflags = 0x3200;
    tss->gs = LDT_RW;
    tss->ss = LDT_RW;
    tss->esp = USER_STACK_TOP;
    tss->cs = LDT_CALLABLE;
    tss->ds = LDT_RW;
    tss->es = LDT_RW;
    tss->fs = LDT_RW;
    tss->bmp = 0x8000000;
    tss->ldt = GDT_LDT;
    tss->cr3 = (long) paging->pde;
}

/**
 * Really change to one process.
 */
void goproc(unsigned long pid)
{
    restart(GDT_TSS(pid));
}

/**
 * Change regs to params and call dosyscall().
 */
unsigned long syscalled(long esi, long edi, long ebp, long eax, long ebx, \
long ecx, long edx)
{
    long params[] = {ebx, ecx, edx, esi, edi, ebp};
    return dosyscall(eax, params);
}

