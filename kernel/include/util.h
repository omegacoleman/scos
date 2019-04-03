#ifndef _UTIL_H_
#define _UTIL_H_

/**
 * These are for data cutting and sticking.
 * First I write them in functions... how slow,
 * It isn't java!
 */
#define lowof16(data) (data & 0xFF)
#define highof16(data) ((data & 0xFF00) / 0x100)
#define lowof32(data) (data & 0xFFFF)
#define highof32(data) ((data & 0xFFFF0000) / 0x10000)
#define connect8to16(high, low) (high * 0x100 + low)
#define connect16to32(high, low) (high * 0x10000 + low)

/**
 * Out byte {to,from} port.
 */
unsigned char inbyte(unsigned int port);
void outbyte(unsigned int port, unsigned char data);
/**
 * Out word {to,from} port.
 */
unsigned short inword(unsigned int port);
void outword(unsigned int port, unsigned short data);
/**
 * Convert integer to char array.
 */
char *vsitoa(unsigned long toc, unsigned int max);
/**
 * Set IF to {1,0}.
 */
void enableint();
void disableint();
/**
 * If I set it to "strcpy", gcc will warn me.
 * Copy an string from source to dest.
 */
void kstrcpy(char *dest, char *source);
int kstrcmp(char *dest, char *source);

#endif

