#ifndef _SCREEN_H_
#define _SCREEN_H_

#include "vs.h"

/**
 * Init the screen to normal state.
 */
void screeninit();
/**
 * Print data to current vs.
 */
void printchar(char ch);
void printstr(char *str);
void printcolorstr(char *str, unsigned char currentcolor);
/**
 * Print data to any vs.
 */
void printvscolorstr(char *str, unsigned char currentcolor, virtualscreen *screen);
void printvsstr(char *str, virtualscreen *screen);
void printvschar(char ch, virtualscreen *screen);
/**
 * Set&get the cursor's real location.'
 */
void setcursor(unsigned int location);
unsigned int getcursor();
/**
 * Set&get the video mem's start location.'
 */
void setscreenstart(unsigned int start);
unsigned int getscreenstart();

#endif
