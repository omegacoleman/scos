

#ifndef _MM_H_
#define _MM_H_

#define PAGE_DIRS_LEN 0x1000
#define PAGE_LEN 0x1000
#define page(n) (n / PAGE_LEN)
/**
 * The liner memory map, detail in mm_map.
 */
/** Kernel paging map **/
#define KERNEL_PAGES_START 0x100000
#define KERNEL_PAGING_PAGES_START 0x200000
#define KERNEL_USER_PAGES_START 0x400000
/** User paging map **/
#define USER_KERNEL_STACK 0x0
#define USER_KERNEL_STACK_TOP 0x4000
#define USER_STACK 0x4000
#define USER_STACK_TOP 0x8000
#define USER_KERNEL_START 0x100000
#define USER_KERNEL_END 0x200000
#define USER_ALLOCATED_START 0x200000
#define USER_ALLOCATED_END 0x300000
#define USER_CODE_START 0x300000
#define USER_CODE_END 0x400000
#define USER_END 0x400000
/** The allocated user pages' states **/
#define ALLOC_STATE_UNUSED 0
#define ALLOC_STATE_USED 1
/** The kernel paged used sign, an AVL bit of a pte entry **/
#define KERNEL_USER_USED_MASK 512

/**
 * The user tasks paging data.
 */
typedef struct _pagingdata
{
    unsigned long pde[PAGE_DIRS_LEN];
    unsigned long pte[page(USER_END) - 1];
} pagingdata;

/**
 * The user tasks allocated data.
 */
typedef struct _allocatedpage
{
    unsigned long id; /** The allloc id **/
    unsigned long usestate; /** Used or not **/
    unsigned long pagephys; /** The physical page nr **/
    unsigned long pageuser; /** The user liner page nr **/
} allocatedpage;

/**
 * Memory manage module init code
 */
void mminit();
/**
 * Copy memory, easy and simple but ugly.
 */
void copymem(unsigned long *dest, unsigned long *source, 
unsigned long len);
/**
 * Locate&unlocate pages in user area.
 */
unsigned long freeuserpages(unsigned long start, unsigned long len);
unsigned long getuserpages(unsigned long len);
/**
 * Assign pde entries to ptes, and do user mem mapping.
 */
void initpagingdata(pagingdata *data, unsigned long code, 
unsigned long len, unsigned long stack, unsigned long kernstack);
/**
 * Convert user liner addr to phys addr.
 */
unsigned long userlinertophys(unsigned long liner);
/**
 * Allocate or free pages.
 */
unsigned long pgsalloc(allocatedpage *locates, pagingdata *data, int pages);
void pgsfree(allocatedpage *locates, unsigned long all, pagingdata *data, unsigned long id);
// DO NOT USE THESE TWO FUNCS BELOW
// EXPECT YOU KNOW WHAT YOU ARE DOING
/**
 * Alloc or free one page without checking page alloc id
 */
unsigned long pgalloc(allocatedpage *locate, pagingdata *data);
void pgfree(allocatedpage *locate, pagingdata *data);
/**
 * Get pgae id from page liner addr.
 */
unsigned long findpageidfromaddr(unsigned long addr, allocatedpage *locates);

#endif








