


#include "mm.h" 
#include "sched.h"
#include "util.h"
#include "vs.h"

/**
 * Kernel pte entries.
 */
unsigned long *kernelpagedescs;
/**
 * Memory length(in mb).
 */
unsigned long memlen;

/**
 * Init kernel pde&pte and startup paging.
 */
void startuppaging(unsigned long dirs);
/**
 * Read bios params and return memory length (in mb).
 */
unsigned long getmem();


/**
 * Get the mem len, and from this to startup paging.
 * Lock the pages cannot be used.
 */
void mminit()
{
    memlen = getmem();
    startuppaging(memlen);
    kernelpagedescs = (unsigned long *) \
    KERNEL_PAGING_PAGES_START + PAGE_DIRS_LEN;
    unsigned long i;
    for (i = 0; i < (page(KERNEL_USER_PAGES_START)); i++)
    {
        kernelpagedescs[i] |= KERNEL_USER_USED_MASK;
    }
}

/**
 * Locate a user page. Use an AVL bit to find it used or unused.
 */
unsigned long getuserpages(unsigned long len)
{
    unsigned long i, j, start;
    j = 0;
    for (i = 0; i < (memlen * PAGE_LEN); i++)
    {
        if (kernelpagedescs[i] & KERNEL_USER_USED_MASK)  // used?
        {
            j = 0;
        }
        else
        {
            if (j == 0)
            {
                start = i;
            }
            j++;
            if (j >= len)
            {
                break;
            }
        }
    }
    if (j == 0)
    {
        return 0;
    }
    for (i = start; i < (start + j); i++)
    {
        kernelpagedescs[i] |= KERNEL_USER_USED_MASK;
    }
    return start * PAGE_LEN;
}

/**
 * Unlocate user pages.
 */
unsigned long freeuserpages(unsigned long start, unsigned long len)
{
    unsigned long i;
    unsigned long pgstart = page(start);
    for (i = pgstart; i < (pgstart + len); i++)
    {
        kernelpagedescs[i] -= \
        kernelpagedescs[i] & KERNEL_USER_USED_MASK;
    }
    return 0;
}

/**
 * Convert user tasks' liner addr to phys addr.
 */
unsigned long userlinertophys(unsigned long liner)
{
    unsigned long phys;
    phys = currentprocess->paging->pte[page(liner)] & 0xfff000;
    phys += liner & 0xfff;
    return phys;
}

unsigned long maxid = 0;
unsigned long pgsalloc(allocatedpage *locates, pagingdata *data, int pages)
{
    unsigned int i, j, q;
    i = 0;
    j = 0;
    maxid += 1;
    while (j < pages)
    {
        if (locates[i].usestate == ALLOC_STATE_UNUSED)
        {
            if (j == 0)
            {
                q = pgalloc(&(locates[i]), data);
            }
            else
            {
                pgalloc(&(locates[i]), data);
            }
            locates[i].id = maxid;
            j++;
        }
        i++;
    }
    return q;
}

void pgsfree(allocatedpage *locates, unsigned long all, pagingdata *data, unsigned long id)
{
    int i;
    for (i = 0; i < all; i++)
    {
        if (locates[i].usestate == ALLOC_STATE_USED)
        {
            if (locates[i].id == id)
            {
                pgfree(&(locates[i]), data);
            }
        }
    }
}

unsigned long pgalloc(allocatedpage *locate, pagingdata *data)
{
    unsigned long addr = getuserpages(1);
    locate->usestate = ALLOC_STATE_USED;
    locate->pagephys = page(addr);
    unsigned long i;
    for (i = page(USER_ALLOCATED_START); \
    i < page(USER_ALLOCATED_END); i++)
    {
        if (!(data->pte[i] & 1024))
        {
            data->pte[i] = 0x7;
            data->pte[i] += addr;
            data->pte[i] |= 1024;
            locate->pageuser = i;
            return i * PAGE_LEN;
        }
    }
    reportbug(BUG_LEVEL_FAULT, "Mem too short to allocate!!");
    return 0;
}

void pgfree(allocatedpage *locate, pagingdata *data)
{
    freeuserpages(locate->pagephys * PAGE_LEN, 1);
    locate->usestate = ALLOC_STATE_UNUSED;
    data->pte[locate->pageuser] = 0;
}

unsigned long findpageidfromaddr(unsigned long addr, allocatedpage *locates)
{
    unsigned long user = addr / PAGE_LEN;
    unsigned long i = 0;
    while (1)
    {
        if (locates[i].pageuser == user)
        {
            return locates[i].id;
        }
        i++;
    }
}

/**
 * Copy memory in bytes.
 */
void copymem(unsigned long *dest, unsigned long *source, 
unsigned long len)
{
    unsigned long i;
    for (i = 0; i < (len / sizeof(unsigned long)); i++)
    {
        dest[i] = source[i];
    }
}

/**
 * Assign pde entries to ptes.
 */
void initpagingdata(pagingdata *data, unsigned long code, 
unsigned long len, unsigned long stack, unsigned long kernstack)
{
    unsigned long i;
    for (i = 0; i < PAGE_DIRS_LEN; i++)
    {
        data->pde[i] = 7 + ((unsigned long) &(data->pte[i * 1024]));
    }
    for (i = page(USER_KERNEL_STACK); \
    i < page(USER_KERNEL_STACK_TOP); i++)
    {
        data->pte[i] = 7 + kernstack + (i * PAGE_LEN);
    }
    for (i = page(USER_STACK); \
    i < page(USER_STACK_TOP); i++)
    {
        data->pte[i] = 7 + stack + \
        ((i - page(USER_STACK)) * PAGE_LEN);
    }
    for (i = page(USER_KERNEL_START); \
    i < page(USER_KERNEL_END); i++)
    {
        data->pte[i] = 7 + (i * PAGE_LEN);
    }
}

/**
 * Read bios params area and get mem size.
 */
unsigned long getmem()
{
    outbyte(0x70, 0x17);
    unsigned long exlow = inbyte(0x71);
    outbyte(0x70, 0x18);
    unsigned long exhigh = inbyte(0x71);
    unsigned long exmem = connect8to16(exhigh, exlow);
    unsigned long mem = 1024 + exmem;
    printstr("\nMem size : 0x");
    printstr(vsitoa(mem, 16));
    printstr("kbytes.\n");
    return (mem/1024);
}
