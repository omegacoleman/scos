#include "vs.h"
#include "util.h"
#include "screen.h"
#include "machine.h"

/**
 * All lines goes up.
 */
void scroll(virtualscreen *vs);
/**
 * Out char to the vs.
 */
void outchar(char ch, unsigned char color, unsigned long location, \
virtualscreen *vs);

/**
 * Init the screen to normal state.
 */
void screeninit()
{
}

/**
 * Print out the string and move the 
 * cursor to next location.
 */
void printstr(char *str)
{
    printvsstr(str, currentvs);
}

/**
 * Print out the string with color and move the 
 * cursor to next location.
 */
void printcolorstr(char *str, unsigned char currentcolor)
{
    printvscolorstr(str, currentcolor, currentvs);
}

/**
 * Print out a char and move 
 * cursor next location.
 */
void printchar(char ch)
{
    printvschar(ch, currentvs);
}

/**
 * Print out the string to a vs and move the 
 * cursor to next location.
 */
void printvsstr(char *str, virtualscreen *screen)
{
    char buffer[255];
    kstrcpy(buffer, str);
    unsigned int i = 0;
    while (str[i] != '\0')
    {
        printvschar(str[i], screen);
        i = i + 1;
    }
}

/**
 * Print out the string with color and move the 
 * cursor to next location.
 */
void printvscolorstr(char *str, unsigned char currentcolor, \
virtualscreen *screen)
{
    char buffer[255];
    kstrcpy(buffer, str);
    unsigned int i = 0;
    while (str[i] != '\0')
    {
        printvschar(str[i], screen);
        if (str[i] > 0x1f)
        {
            outchar(str[i], currentcolor, screen->cursor - 1, screen);
        }
        i = i + 1;
    }
}

/**
 * Print out a char to a vs and move 
 * cursor next location.
 */
void printvschar(char ch, virtualscreen *screen)
{
    switch(ch)
    {
        case '\n':
        screen->cursor = \
        screen->cursor - (screen->cursor % ONE_LINE) + ONE_LINE;
        break;
        case '\b':
        screen->cursor = screen->cursor - 1;
        outchar(' ', 0x7, screen->cursor, screen);
        flushcursor();
        return;
        case '\0':
        return;
        default:
        outchar(ch, 0x7, screen->cursor, screen);
        screen->cursor = screen->cursor + 1;
        break;
    }
    if ((screen->cursor / ONE_LINE) > (LINES_NR - 1))
    {
        scroll(screen);
    }
    flushcursor();
}

/**
 * Out char to the vs.
 */
void outchar(char ch, unsigned char color, unsigned long location, \
virtualscreen *vs)
{
    unsigned char *firstscreen = vs->data;
    firstscreen[location * 2] = ch;
    firstscreen[location * 2 + 1] = color;
}

/**
 * All lines goes up.
 */
void scroll(virtualscreen *vs)
{
    unsigned char *firstscreen = vs->data;
    unsigned long i;
    for (i = 0; i < ((ONE_SCREEN - ONE_LINE) * 2); i++)
    {
        firstscreen[i] = firstscreen[i + (ONE_LINE * 2)];
    }
    for (; i < (ONE_SCREEN * 2); i++)
    {
        firstscreen[i] = (i % 2) ? 0x7 : 0;
    }
    vs->cursor = vs->cursor - ONE_LINE;
    flushcursor();
}

/**
 * Return the cursor's location.
 */
unsigned int getcursor()
{
    outbyte(0x3D4, 0xE);
    unsigned char high = inbyte(0x3D5);
    outbyte(0x3D4, 0xF);
    unsigned char low = inbyte(0x3D5);
    unsigned int location = connect8to16(high, low);
    return location;
}

/**
 * Set the cursor's location, not change vs value.
 */
void setcursor(unsigned int location)
{
    outbyte(0x3D4, 0xE);
    outbyte(0x3D5, highof16(location));
    outbyte(0x3D4, 0xF);
    outbyte(0x3D5, lowof16(location));
}

/**
 * Set where screen display start.
 */
void setscreenstart(unsigned int start)
{
    outbyte(0x3d4, 0xc);
    outbyte(0x3d5, highof16(start));
    outbyte(0x3d4, 0xd);
    outbyte(0x3d5, lowof16(start));
}

/**
 * Return where screen display start.
 */
unsigned int getscreenstart()
{
    unsigned int high, low;
    outbyte(0x3d4, 0xc);
    high = inbyte(0x3d5);
    outbyte(0x3d4, 0xd);
    low = inbyte(0x3d5);
    return connect8to16(high, low);
}


