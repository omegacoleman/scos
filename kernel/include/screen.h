#ifndef _SCREEN_H_
#define _SCREEN_H_

#include "vs.h"

void screeninit();
void flashcolor(unsigned long location);
void changecolor(unsigned long location, unsigned long color);
void printchar(char ch);
void printstr(char *str);
void printcolorstr(char *str, unsigned char currentcolor);
void printvscolorstr(char *str, unsigned char currentcolor, virtualscreen *screen);
void printvsstr(char *str, virtualscreen *screen);
void printvschar(char ch, virtualscreen *screen);
void setcursor(unsigned int location);
unsigned int getcursor();
void setscreenstart(unsigned int start);
unsigned int getscreenstart();

#endif
