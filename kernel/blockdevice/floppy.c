
#include "util.h"
#include "vs.h"
#include "machine.h"
#include "floppy.h"

#define writedor(x) (outbyte(0x3f2, x))
#define readfdcstatus() (inbyte(0x3f4))
#define NORMAL_DOR 0xc
#define gendor(ena, enb, enc, end, sel) (NORMAL_DOR + \
(ena ? 0x10 : 0) + \
(enb ? 0x20 : 0) + \
(enc ? 0x40 : 0) + \
(end ? 0x80 : 0) + sel)
#define FDC_STATUS_RQM 0x80
#define FDC_STATUS_DIO 0x40
#define statusrqm(a) ((a & FDC_STATUS_RQM) > 0)
#define statusfdctocpu(a) (((a & FDC_STATUS_DIO) > 0) && statusrqm(a))
#define statuscputofdc(a) (((a & FDC_STATUS_DIO) == 0) && statusrqm(a))
#define SECTOR_SIZE 512
#define SPC 18
#define HEADS 2
#define DISK_SIZE 1474560

unsigned char driveselected;
unsigned char motor[4];

void delay();
void writefdcdata(unsigned char data);
unsigned long readfdcdata();
void dodma(int read, unsigned long buffer, unsigned long len);
void dofdcrw(int read, unsigned long start);
void convertsectornr(unsigned long sectornr, unsigned long *sec, \
unsigned long *track, unsigned long *head);
void choosedrive(unsigned char which);


void floppyinit()
{
    choosedrive(0);
    recalibrate();
}

void choosedrive(unsigned char which)
{
    driveselected = which;
    if (motor[which] == 0)
    {
        motor[which] = 1;
        writedor(gendor(motor[0], motor[1], motor[2], motor[3], which));
        delay();
    }
    else
    {
        writedor(gendor(motor[0], motor[1], motor[2], motor[3], which));
    }
}

void delay()
{
    int i;
    for (i = 0; i < 1000000; i++)
    {
    }
}

void writefdcdata(unsigned char data)
{
    unsigned long i = 0;
    while (!(statuscputofdc(readfdcstatus())))
    {
        if (i > 10000000)
        {
            return;
        }
        i++;
    }
    outbyte(0x3f5, data);
}

unsigned long readfdcdata()
{
    unsigned long i = 0;
    while (!(statusfdctocpu(readfdcstatus())))
    {
        if (i > 10000000)
        {
            return 0;
        }
        i++;
    }
    return inbyte(0x3f5);
}

void recalibrate()
{
    printstr("Floppy recalibrated. \n");
    writefdcdata(0x07);
    writefdcdata(driveselected);
    writefdcdata(0x07);
    writefdcdata(driveselected + 4);
}

#define TMP_BUFFER 0x1000
void dorw(int read, unsigned long buffer, unsigned long len, 
unsigned long start, unsigned long drive)
{
    if (len > (64 * 1024))
    {
        reportbug(0x77, "Disk syscall param error : \
length out of range, setting to 64k. ");
        len = 64 * 1024;
    }
    if ((start + len) > DISK_SIZE)
    {
        reportbug(0x77, "Disk syscall param error : address \
more than disk size. ");
        return;
    }
    while (drive != driveselected)
    {
        choosedrive(drive);
    }
    if (buffer > 0x100000)
    {
        if (!read)
        {
            copymem((long unsigned int *) TMP_BUFFER, \
            (long unsigned int *) buffer, len);
        }
        dodma(read, TMP_BUFFER, len);
        dofdcrw(read, start);
        if (read)
        {
            copymem((long unsigned int *) buffer, \
            (long unsigned int *) TMP_BUFFER, len);
        }
    }
    else
    {
        dodma(read, buffer, len);
        dofdcrw(read, start);
    }
}

void dodma(int read, unsigned long buffer, unsigned long len)
{
    len--;
    outbyte(0x0a, 0x06);
    outbyte(0x0c, (read ? 0x46 : 0x4a));
    outbyte(0x0b, (read ? 0x46 : 0x4a));
    outbyte(0x04, lowof16(lowof32(buffer)));
    outbyte(0x04, highof16(lowof32(buffer)));
    outbyte(0x81, lowof16(highof32(buffer)));
    outbyte(0x05, lowof16(len));
    outbyte(0x05, highof16(len));
    outbyte(0x0a, 0x02);
}

void dofdcrw(int read, unsigned long start)
{
    unsigned long sector, track, head;
    convertsectornr(start / SECTOR_SIZE, &sector, &track, &head);
    writefdcdata((read?0xe6:0xc5));
    writefdcdata(driveselected + (head<<2));
    writefdcdata(track);
    writefdcdata(head);
    writefdcdata(sector);
    writefdcdata(SECTOR_SIZE / 256);
    writefdcdata(SPC);
    writefdcdata(SECTOR_SIZE / 256 + 1);
    writefdcdata(0);
    int i;
    for (i = 0; i < 7; i++)
    {
        readfdcdata();
    }
}

void convertsectornr(unsigned long sectornr, unsigned long *sec, \
unsigned long *track, unsigned long *head)
{
    unsigned long q;
    *sec = sectornr % SPC + 1;
    q = sectornr / SPC;
    *track = q / HEADS;
    *head = q % HEADS;
}






