
#include "hd.h"
#include "util.h"
#include "machine.h"
#include "vs.h"

unsigned short spt;
unsigned short heads;
unsigned char cylinders;

typedef struct _paramtable
{
    unsigned short cylinders;
    unsigned char heads;
    unsigned short lesswrite;
    unsigned char precomp;
    unsigned char maxecc;
    unsigned char control;
    unsigned char timeout;
    unsigned char formattimeout;
    unsigned char checktimeout;
    unsigned short stop;
    unsigned char spc;
    unsigned char reserved;
} paramtable;

void hdinit()
{
    unsigned long paramtableaddr = *((unsigned long *) 0x104);
    paramtableaddr = \
    highof32(paramtableaddr) * 0x10 + lowof32(paramtableaddr);
    paramtable *table = (paramtable *)paramtableaddr;
    heads = table->heads;
    spt = table->spc;
    cylinders = table->cylinders;
    printstr("Harddisk params : ");
    printstr(vsitoa(cylinders, 10));
    printstr("/");
    printstr(vsitoa(heads, 10));
    printstr("/");
    printstr(vsitoa(spt, 10));
    printstr("\n");
    hdreset();
}

void hdreset()
{
    waitbusy();
    printstr("Harddisk recalibrated.\n");
    outbyte(0x3f6, 0x0);
    outbyte(0x1f1, 0x0);
    outbyte(0x1f2, 0x0);
    outbyte(0x1f3, 0x0);
    outbyte(0x1f4, 0x0);
    outbyte(0x1f5, 0x0);
    outbyte(0x1f6, 0xa0);
    outbyte(0x1f7, 0x10);
}

unsigned short ibuffer[256 * 256];
void dohdrw(int read, unsigned long bufferaddr, 
unsigned long len, unsigned long start, unsigned long drive)
{
    waitbusy();
    unsigned long startsectornr = start / 512;
    unsigned long skipbytes = start % 512;
    unsigned long startsector = startsectornr % spt + 1;
    unsigned long starttracknr = startsectornr / spt;
    unsigned long startcylinder = starttracknr / heads;
    unsigned long starthead = starttracknr % heads;
    unsigned long lensectors = len / 512 + (skipbytes ? 1 : 0);
    unsigned short *buffer = (unsigned short *) bufferaddr;
    if (!read)
    {
        dohdrw(1, (unsigned long) ibuffer, lensectors * 512, \
        startsectornr * 512, drive);
    }
    outbyte(0x3f6, (heads > 8) ? 0x8 : 0x0);
    outbyte(0x1f1, cylinders / 2);
    outbyte(0x1f2, lensectors);
    outbyte(0x1f3, startsector);
    outbyte(0x1f4, lowof16(startcylinder));
    outbyte(0x1f5, highof16(startcylinder));
    outbyte(0x1f6, 0xa0 + starthead + (drive * 0x10));
    outbyte(0x1f7, (read ? 0x20 : 0x30));
    unsigned long i;
    if (read)
    {
        waitbusy();
    }
    for (i = 0; i < (skipbytes / 2); i++)
    {
        if (read)
        {
            inword(0x1f0);
        }
        else
        {
            outword(0x1f0, ibuffer[i]);
        }
    }
    for (; i < ((skipbytes + len) / 2); i++)
    {
        if (read)
        {
            buffer[i - (skipbytes / 2)] = inword(0x1f0);
        }
        else
        {
            outword(0x1f0, buffer[i - (skipbytes / 2)]);
        }
    }
    for (; i < (lensectors * 256); i++)
    {
        if (read)
        {
            inword(0x1f0);
        }
        else
        {
            outword(0x1f0, ibuffer[i]);
        }
    }
}

void waitbusy()
{
    while (inbyte(0x1f7) & 0x80);
}

