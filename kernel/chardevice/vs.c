#include "vs.h"
#include "screen.h"
#include "keyboard.h"

/**
 * Init the basic vs data and call screen and keyboard init funcs.
 */
void vsinit()
{
    int i;
    for (i = 0; i < 8; i++)
    {
        basicvscreens[i].nr = i;
        basicvscreens[i].dmemnr = i;
        basicvscreens[i].data = (unsigned char *)VIDEO_MEM_START \
        + (i * ONE_SCREEN * 2);
        basicvscreens[i].cursor = 0;
        basicvscreens[i].scanning = 0;
    }
    basicvscreens[0].cursor = getcursor();
    changevs(&(basicvscreens[0]));
    screeninit();
    keyboardinit();
}

/**
 * Change the screen mem start to that vs and flush cursor.
 */
void changevs(virtualscreen *vs)
{
    currentvs = vs;
    setscreenstart(vs->dmemnr * ONE_SCREEN);
    flushcursor();
}

