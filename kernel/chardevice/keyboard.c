#include "vs.h"
#include "screen.h"
#include "keyboard.h"
#include "machine.h"

/**
 * There are three control keys and three lock 
 * keys on the keyboard.
 */
#define NR_CONTROL 3
#define NR_LOCK 3
/**
 * Masks for control&lock keys.
 */
#define KEY_STATUS_SHIFT 1
#define KEY_STATUS_CTRL 2
#define KEY_STATUS_ALT 4
#define KEY_STATUS_CAPS 8
#define KEY_STATUS_NUM 16
#define KEY_STATUS_SCROLL 32

/**
 * Decode keys.
 */
char decodekey(unsigned char keycode);
char decodeshiftkey(unsigned char keycode);
/**
 * Keyboard interrupt method.
 */
void keyboardint();
/**
 * Mask&unmask the lock&control keys.
 */
void keystatuson(unsigned long key);
void keystatusoff(unsigned long key);
int iskeystatuson(unsigned long key);
/**
 * Return the control&lock key masks from key codes.
 */
int islockkey(unsigned char keycode);
int iscontrolkey(unsigned char keycode);

/**
 * Key map in asm files.
 */
extern char keymap[];
extern char statekeys[];
extern char controls[];
extern char locks[];

/**
 * Status for control&lock keys.
 */
unsigned long status = 0;

/**
 * Init the keyboard input module.
 */
void keyboardinit()
{
    setdoint(KEYBOARD_IRQ, (long) keyboardint);
    unmaskirq(1);
}

/**
 * Wait for a line of input from kernel.
 */
void kscanstr(char *buffer)
{
    currentvs->scanning = 1;
    currentvs->scanbuffer = buffer;
    while (currentvs->scanning);
}

/**
 * Start scanning string.
 */
void scanstrstart(char *buffer, virtualscreen *vs)
{
    if (!vs->scanning)
    {
        vs->scanning = 1;
        vs->scanbuffer = buffer;
    }
}

/**
 * Return its scanning or not.
 */
int isscanning(virtualscreen *vs)
{
    return vs->scanning;
}

/**
 * Keyboard interrupt handle C code, decode key code.
 */
void keyboardinterrupted(unsigned char keycode)
{
    unsigned long ct = iscontrolkey(keycode);
    unsigned long lk = islockkey(keycode);
    char ch = '\0';
    if (keycode > 0x80)
    {
        keycode -= 0x80;
        ct = iscontrolkey(keycode);
        if (ct)
        {
            ct = 1<<(ct - 1);
            keystatusoff(ct);
        }
    }
    else
    {
        if (ct)
        {
            ct = 1<<(ct - 1);
            keystatuson(ct);
        }
        else if (lk)
        {
            lk = 1<<(lk - 1);
            if (iskeystatuson(lk))
            {
                keystatusoff(lk);
            }
            else
            {
                keystatuson(lk);
            }
        }
        else
        {
            if (iskeystatuson(KEY_STATUS_SHIFT))
            {
                ch = decodeshiftkey(keycode);
            }
            else if (iskeystatuson(KEY_STATUS_CAPS))
            {
                ch = decodeshiftkey(keycode);
            }
            else
            {
                ch = decodekey(keycode);
            }
        }
    }
    if (iskeystatuson(KEY_STATUS_ALT))
    {
        if (ch >= '0')
        {
            if (ch <= '6')
            {
                changevs(&(basicvscreens[ch - '0']));
                return;
            }
        }
    }
    if (currentvs->scanning)
    {
        
        if (ch == '\n')
        {
            printvschar(ch, currentvs);
            currentvs->scanbuffer[currentvs->scanning - 1] = '\0';
            currentvs->scanning = 0;
        }
        else if (ch == '\b')
        {
            if (currentvs->scanning == 1)
            {
                return;
            }
            printvschar(ch, currentvs);
            currentvs->scanning--;
            currentvs->scanbuffer[currentvs->scanning - 1] = '\0';
        }
        else if (ch != '\0')
        {
            printvschar(ch, currentvs);
            currentvs->scanbuffer[currentvs->scanning - 1] = ch;
            currentvs->scanning++;
        }
    }
}

/**
 * Get control key mask from key code.
 */
int iscontrolkey(unsigned char keycode)
{
    int i;
    for (i = 0; i < NR_CONTROL; i++)
    {
        if (keycode == controls[i])
        {
            return (i + 1);
        }
    }
    return 0;
}

/**
 * Get lock key mask from key code.
 */
int islockkey(unsigned char keycode)
{
    int i;
    for (i = 0; i < NR_LOCK; i++)
    {
        if (keycode == locks[i])
        {
            return (NR_CONTROL + i + 1);
        }
    }
    return 0;
}

/**
 * Decode the key code.
 */
char decodekey(unsigned char keycode)
{
    return keymap[keycode];
}

/**
 * Decode the shift, alt, ctrl key.
 */
char decodeshiftkey(unsigned char keycode)
{
    return keymap[0xff + keycode];
}

/**
 * Change a bit in status to report a key is active or not.
 */
void keystatuson(unsigned long key)
{
    status += (status & key) ? 0 : key;
}

/**
 * Change a bit in status to report a key is active or not.
 */
void keystatusoff(unsigned long key)
{
    status -= status & key;
}

/**
 * Read a bit in status to report a key is active or not.
 */
int iskeystatuson(unsigned long key)
{
    return ((status & key) > 0);
}


