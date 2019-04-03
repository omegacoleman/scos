#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

int isscanning(virtualscreen *vs);
void scanstrstart(char *buffer, virtualscreen *vs);
void kscanstr(char *buffer);
void keyboardinit();

#endif

