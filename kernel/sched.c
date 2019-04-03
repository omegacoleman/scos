
#include "util.h"
#include "machine.h"
#include "sched.h"

#define STACK_START(array) (((long) array) + sizeof(array))

process proctbl[PROC_MAX];
unsigned int procnum;
process *currentprocess;
process *idle;
unsigned long cpid;
unsigned char timesleft = 3;
extern unsigned int timeentered;

/**
 * while(1);
 */
void idlefunc();

/**
 * Do some init for this file.
 */
void schedinit()
{
    cpid = 0;
    procnum = 0;
    int i;
    for (i = 0; i < PROC_MAX; i++)
    {
        proctbl[i].level = 0;
    }
    idle = currentprocess = newprocess("IDLE", (long) idlefunc, &(basicvscreens[0]));
}

/**
 * Lock forever, never do anything.
 */
void idlefunc()
{
    while (1);
}

/**
 * Create a new process.
 */
process *newprocess(char *name, long entry, virtualscreen *vs)
{
    process *thisnew;
    int i, found;
    found = 0;
    for (i = 0; i < PROC_MAX; i++)
    {
        thisnew = &(proctbl[i]);
        if (thisnew->level == 0)
        {
            found = 1;
            break;
        }
    }
    if (!found)
    {
        return ((process *) 0);
    }
    thisnew->pid = i;
    kstrcpy((char *) thisnew->name, name);
    thisnew->level = 3;
    thisnew->stack = getuserpages(4);
    thisnew->kernstack = getuserpages(4);
    thisnew->paging = (pagingdata *) getuserpages(2);
    for (i = 0; i < 32; i++)
    {
        thisnew->allocatedpages[i].usestate = ALLOC_STATE_UNUSED;
    }
    thisnew->vs = vs;
    inittaskstate(&(thisnew->tss), entry, thisnew->paging);
    initpagingdata(thisnew->paging, entry, 1, thisnew->stack, thisnew->kernstack);
    inittssdesc(thisnew->tssdesc, &(thisnew->tss));
    reloadstate(thisnew->pid, thisnew->tssdesc);
    procnum++;
    return thisnew;
}

/**
 * Free a process's pages and kill it.
 */
void removeprocess(unsigned long pid)
{
    proctbl[pid].level = 0;
    timesleft = 0;
    unsigned long i;
    for (i = 0; i < 32; i++)
    {
        if (proctbl[pid].allocatedpages[i].usestate == \
        ALLOC_STATE_USED)
        {
            pgfree(&(proctbl[pid].allocatedpages[i]), proctbl[pid].paging);
        }
    }
    freeuserpages(proctbl[pid].stack, 4);
    freeuserpages(proctbl[pid].kernstack, 4);
    freeuserpages((unsigned long) proctbl[pid].paging, 2);
    procnum--;
}

/**
 * Change the running proc.
 */
void switchto(process *which)
{
    if (currentprocess == which)
    {
        return;
    }
    currentprocess = which;
    goproc(currentprocess->pid);
}

/**
 * Main schedule func.
 */
void schedule()
{
    if (procnum == 1)
    {
        cpid = 0;
        timesleft = 0;
    }
    else
    {
        if (timesleft > 0) {
            timesleft--;
        }
        if (timesleft <= 0)
        {
            do
            {
                cpid = cpid + 1;
                if (cpid >= PROC_MAX)
                {
                    cpid = 1;
                }
            } while (proctbl[cpid].level == 0);
            timesleft = proctbl[cpid].level;
        }
    }
    switchto(&(proctbl[cpid]));
}

/**
 * Kill current proc.
 */
void killcurrent()
{
    removeprocess(currentprocess->pid);
    schedule();
}

