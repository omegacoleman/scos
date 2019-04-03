#ifndef _HD_H_
#define _HD_H_

void hdinit();
void dohdrw(int read, unsigned long bufferaddr, 
unsigned long len, unsigned long start, unsigned long drive);
void hdreset();
void waitbusy();

#endif



