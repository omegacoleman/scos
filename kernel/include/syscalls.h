

#ifndef _SYSCALLS_C_
#define _SYSCALLS_C_

/**
 * Main system call func.
 */
unsigned long dosyscall(long nr, long params[]);

// system call funcs
/**
 * To print/scan str, see screen.c
 */
void SYSprintstr(char *str);
void SYSscanstr(char *buffer);
void SYSprintcolorstr(char *str, unsigned long color);
/**
 * Return the screen proc using.
 */
unsigned long SYSgetvs();
/**
 * To create/kill proc, see sched.c
 */
void SYSkillme();
unsigned long SYSnewproc(char *name, unsigned long entry);
void SYSkillproc(unsigned long pid);
/**
 * Block module, see block.c
 */
void SYSdevicereset(unsigned int device);
void SYSdoblock(unsigned int device, int read, \
unsigned long buffer, unsigned long len, unsigned long start, \
unsigned long drive);
void *SYSmalloc(unsigned long len);
void SYSfree(unsigned long addr);

#endif



