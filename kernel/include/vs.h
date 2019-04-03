#ifndef _VS_H_
#define _VS_H_

/** The video memory starts here **/
#define VIDEO_MEM_START 0xb8000
/**
 * Normal screen params.
 */
#define ONE_LINE 80
#define LINES_NR 25
#define ONE_SCREEN (ONE_LINE * 25)
/**
 * Refresh the cursor location on screen.
 */
#define flushcursor() \
setcursor(currentvs->dmemnr * ONE_SCREEN + currentvs->cursor);

/**
 * Basic data for the virtual screen.
 */
typedef struct _virtualscreen
{
    unsigned char nr;
    unsigned char dmemnr;
    unsigned char *data;
    unsigned long cursor;
    unsigned int scanning;
    char *scanbuffer;
} virtualscreen;

/**
 * EIght basic screens
 */
virtualscreen basicvscreens[8];
/**
 * The vs showing.
 */
virtualscreen *currentvs;

/**
 * Init the screen data and screen&keyboard device.
 */
void vsinit();
/**
 * Change vs showing.
 */
void changevs(virtualscreen *vs);
/**
 * These are from screen.c and keyboard.c.
 * For printing&scanning.
 */
void printstr(char *str);
void printcolorstr(char *str, unsigned char currentcolor);
void printvscolorstr(char *str, unsigned char currentcolor, virtualscreen *screen);
int isscanning(virtualscreen *vs);
void printvsstr(char *str, virtualscreen *screen);
void scanstrstart(char *buffer, virtualscreen *vs);

#endif

