
#ifndef _SCHED_H_
#define _SCHED_H_

/**
 * These three files are sub files of this file.
 */
#include "machine.h"
#include "mm.h"
#include "vs.h"

/**
 * The max proccess num.
 */
#define PROC_MAX 64

/**
 * The basic infomations of a process.
 */
typedef struct _process
{
    char name[5];  /** The process name **/
    unsigned int pid; /** Each Proc has a different ID **/
    unsigned int level; /** Schedule level, level 0 -- unused **/
    state tss; /** Every process has an own tss and its desc **/
    descriptor tssdesc;
    unsigned long stack; /** The REAL stack addr **/
    unsigned long kernstack; /** When interrupts, use this. **/
    pagingdata *paging; /** Its own paging data **/
    allocatedpage allocatedpages[32];
    virtualscreen *vs; /** The screen it can print things. **/
} process;

/**
 * The process running.
 */
extern process *currentprocess;
/**
 * When no process running, run this.
 */
extern process *idle;

/**
 * Main schedule func. Level bigger tasks work more.
 */
void schedule();
/**
 * Start muti-proc working, move into user mode and start
 * to run tasks created.
 */
void start();
/**
 * Create process.
 */
process *newprocess(char *name, long entry, virtualscreen *vs);
/**
 * Kill currentprocess.
 */
void killcurrent();
/**
 * Change current.
 */
void switchto(process *which);
/**
 * Init the tasks-schedule module.
 */
void schedinit();
/**
 * Delete the process.
 */
void removeprocess(unsigned long pid);


#endif

