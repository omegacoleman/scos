
#include "syscalls.h"
#include "sched.h"
#include "machine.h"
#include "vs.h"
#include "mm.h"
#include "util.h"
#include "block.h"

/**
 * Like createprocess, but returns pid.
 */
unsigned long gennewproc(char *name, unsigned long entry);
/**
 * Because of switch..case syntax cannot define any instance, so
 * I must write a new func to return the vs num.
 */
unsigned long getvsnr();

/**
 * Use swithch..case to explain the sys call and run it.
 */
unsigned long dosyscall(long nr, long params[])
{
    switch (nr)
    {
        case 0x10:
        printvsstr((char *) userlinertophys(params[0]), currentprocess->vs);
        break;
        case 0x11:
        scanstrstart((char *) userlinertophys(params[0]), currentprocess->vs);
        break;
        case 0x12:
        return isscanning(currentprocess->vs);
        break;
        case 0x13:
        printvscolorstr((char *) userlinertophys(params[0]), params[1], currentprocess->vs);
        break;
        case 0x1e:
        killcurrent();
        break;
        case 0x1a:
        removeprocess(params[0]);
        break;
        case 0x1d:
        return gennewproc((char *) params[0], params[1]);
        break;
        case 0x78:
        currentprocess->vs->data[currentprocess->vs->cursor * 2 + 1]++;
        break;
        case 0x51:
        doblock(params[0], params[1], userlinertophys(params[2]), \
        params[3], params[4], params[5]);
        break;
        case 0x52:
        devicereset(params[0]);
        break;
        case 0x60:
        return pgsalloc(currentprocess->allocatedpages, \
        currentprocess->paging, \
        params[0] / 4096 + (params[0] % 4096 > 0 ? 1 : 0));
        break;
        case 0x61:
        pgsfree(currentprocess->allocatedpages, 32, \
        currentprocess->paging, \
        findpageidfromaddr(params[0], currentprocess->allocatedpages));
        break;
        case 0x80:
        return getvsnr();
        break;
        default:
        reportbug(0x77, "Bad system call number.");
        break;
    }
    return 0;
}

/**
 * Because of switch..case syntax cannot define any instance, so
 * I must write a new func to gen a process and returns pid.
 */
unsigned long gennewproc(char *name, unsigned long entry)
{
    process *np = newprocess(name, entry, currentprocess->vs);
    return np->pid;
}

/**
 * Because of switch..case syntax cannot define any instance, so
 * I must write a new func to return the vs num.
 */
unsigned long getvsnr()
{
    return currentprocess->vs->nr;
}

