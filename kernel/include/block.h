

#ifndef _BLOCK_H_
#define _BLOCK_H_

/**
 * The device nums of devices.
 */
#define DEVICE_FLOPPY 0x10
#define DEVICE_HD 0x11

/**
 * Init all block devices.
 */
void blockinit();
/**
 * Read or write block device.
 */
void doblock(unsigned int device, int read, unsigned long buffer, 
unsigned long len, unsigned long start, unsigned long drive);
/**
 * If always wrongs, try this.
 */
void devicereset(unsigned int device);

#endif



