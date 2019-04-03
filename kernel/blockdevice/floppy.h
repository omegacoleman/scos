
#ifndef _FLOPPY_H_
#define _FLOPPY_H_

void floppyinit();
void dorw(int read, unsigned long buffer, 
unsigned long len, unsigned long start, unsigned long drive);
void recalibrate();

#endif



