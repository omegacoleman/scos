#include "util.h"
#include "machine.h"

/**
 * Convert integer to char array.
 */
char buffer[255];
char *vsitoa(unsigned long toc, unsigned int max)
{
    unsigned int ttoc = toc;
    int i = 0;
    while (ttoc != 0)
    {
        unsigned char last = ttoc % max;
        char lastchar = 'X';
        ttoc = (ttoc - last) / max;
        if (last < 10)
        {
            lastchar = '0' + last;
        } else {
            last = last - 10;
            lastchar = 'A' + last;
        }
        buffer[254 - i] = lastchar;
        i = i + 1;
    }
    if (i == 0)
    {
        buffer[254] = '0';
        i = i + 1;
    }
    buffer[255] = '\0';
    return &(buffer[255 - i]);
}

/**
 * Copy an string from source to dest.
 */
void kstrcpy(char *dest, char *source)
{
    int i = 0;
    while (source[i] != '\0')
    {
        dest[i] = source[i];
        i++;
    }
    dest[i] = 0;
}

/**
 * Return two strings equal or not.
 */
int kstrcmp(char *dest, char *source)
{
    int i = 0;
    while ((source[i] != '\0') || (dest[i] != '\0'))
    {
        if (dest[i] != source[i])
        {
            return 0;
        }
        i++;
    }
    return 1;
}


