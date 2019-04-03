
#include "block.h"
#include "floppy.h"
#include "hd.h"

void blockinit()
{
    floppyinit();
    hdinit();
}

void doblock(unsigned int device, int read, unsigned long buffer, 
unsigned long len, unsigned long start, unsigned long drive)
{
    switch (device)
    {
        case DEVICE_FLOPPY : dorw(read, buffer, len, start, drive); break;
        case DEVICE_HD : dohdrw(read, buffer, len, start, drive); break;
    }
}

void devicereset(unsigned int device)
{
    switch (device)
    {
        case DEVICE_FLOPPY : recalibrate(); break;
        case DEVICE_HD : hdreset(); break;
    }
}


