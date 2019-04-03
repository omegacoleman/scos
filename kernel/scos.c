#include "util.h"
#include "machine.h"
#include "sched.h"
#include "mm.h"
#include "syscalls.h"
#include "block.h"
#include "vs.h"

/**
 * Start a funny shell to try interesting things in this
 * os.
 * Should be started as a process.
 */
void startos();

/**
 * Start the system.
 */
int scos()
{
    machineinit();
    vsinit();
    mminit();
    schedinit();
    blockinit();
    printcolorstr("\n----BOOT-INIT-SUCCESS--------\n", 0x2);
    newprocess("SCR1SH", (long) startos, &(basicvscreens[1]));
    newprocess("SCR2SH", (long) startos, &(basicvscreens[2]));
    newprocess("SCR3SH", (long) startos, &(basicvscreens[3]));
    newprocess("SCR4SH", (long) startos, &(basicvscreens[4]));
    newprocess("SCR5SH", (long) startos, &(basicvscreens[5]));
    newprocess("SCR6SH", (long) startos, &(basicvscreens[6]));
    changevs(&(basicvscreens[1]));
    unmaskirq(0);
    start();
    while(1);
    return 0;
}

/**
 * Start a funny shell to try interesting things in this
 * os.
 * Should be started as a process.
 */
void startos()
{
    SYSprintstr("Welcome to Scos! \n");
    SYSprintstr("VS ");
    SYSprintstr(vsitoa(SYSgetvs(), 10));
    SYSprintstr("\n");
    char *buffer = (char *) SYSmalloc(512);
    char *copybuffer = SYSmalloc(1024);
    unsigned long lpid, on = 0;
    while (1)
    {
        SYSprintstr("\nSCOS# ");
        SYSscanstr(buffer);
        if (kstrcmp(buffer, ""))
        {
        }
        else if (kstrcmp(buffer, "help"))
        {
            SYSprintstr("\
Enable commands : \n\
{hd,fd}reset -- reset the drive\n\
installtofloppy -- copy scos from fd0 to fd1\n\
light{on,of} -- a mini term light\n\
\n\
Have fun! \n\
\n");
        }
        else if (kstrcmp(buffer, "hdreset"))
        {
            SYSdevicereset(DEVICE_HD);
        }
        else if (kstrcmp(buffer, "fdreset"))
        {
            SYSdevicereset(DEVICE_FLOPPY);
        }
        else if (kstrcmp(buffer, "installtofloppy"))
        {
            SYSprintstr("Copying SCOS from fd0 to fd1 ...\n");
            SYSprintcolorstr("                                        ", 0xc0);
            int i = 0, k = 0;;
            for (i = 0; i < 1474560; i+=1024)
            {
                SYSdoblock(DEVICE_FLOPPY, 1, \
                (unsigned long)copybuffer, 1024, i, 0);
                SYSdoblock(DEVICE_FLOPPY, 0, \
                (unsigned long)copybuffer, 1024, i, 1);
                if (k > 5)
                {
                    int p = (i * 40) / 1474560;
                    int j;
                    for (j = 0; j < 40; j++)
                    {
                        SYSprintstr("\b");
                    }
                    for (j = 0; j < p; j++)
                    {
                        SYSprintcolorstr(" ", 0x10);
                    }
                    for (; j < 40; j++)
                    {
                        SYSprintcolorstr(" ", 0xc0);
                    }
                    k = 0;
                }
                else
                {
                    k++;
                }
            }
            int j;
            for (j = 0; j < 40; j++)
            {
                SYSprintstr("\b");
            }
            SYSprintcolorstr("                                        ", 0x10);
            SYSprintstr("\nOk.");
        }
        else if (kstrcmp(buffer, "lighton"))
        {
            if (!on)
            {
                lpid = SYSnewproc("LIGHT", (unsigned long) light);
                SYSprintstr(vsitoa(lpid,10));
                on = 1;
            }
            else
            {
                printstr("It is already on!");
            }
        }
        else if (kstrcmp(buffer, "lightoff"))
        {
            if (on)
            {
                SYSkillproc(lpid);
                on = 0;
            }
            else
            {
                printstr("It is already off!");
            }
        }
        else
        {
            SYSprintstr("I don't know what you mean : \"");
            SYSprintstr(buffer);
            SYSprintstr("\"");
        }
    }
}




