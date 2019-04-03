#include <stdio.h>

/**
 * Get the data from a elf file.
 */
int getprogrammeheadercount(unsigned char elfheader[]);
int getprogrammetype(unsigned char header[]);
int getprogrammeoffset(unsigned char elfheader[]);
int getprogrammeaddress(unsigned char elfheader[]);
int getprogrammelength(unsigned char elfheader[]);
int getentry(unsigned char elfheader[]);

/**
 * Convert elf file to scmd file.
 */
int main()
{
    FILE *psystem = fopen("system.bin", "rb");
    unsigned char elf[1474560];
    long size = 0;
    while(!feof(psystem))
    {
        elf[size] = fgetc(psystem);
        size++;
    }
    unsigned char elfheader[0x34];
    int i;
    for (i = 0; i < 0x34; i++)
    {
        elfheader[i] = elf[i];
    }
    int programmecount = getprogrammeheadercount(elfheader);
    printf("Programme header count : 0x%x\n", programmecount);
    unsigned char \
    programmeheaders[programmecount][0x20];
    int j;
    for (j = 0; j < programmecount; j++)
    {
        for (i = 0; i < 0x20; i++)
        {
            programmeheaders[j][i] = elf[i + 0x34 + 0x20 * j];
        }
    }
    FILE *pout = fopen("./sysmodule.scmd", "wb");
    int entry = getentry(elfheader);
    printf("Entry : 0x%x\n", entry);
    fwrite(&entry, sizeof(int), 1, pout);
    for (j = 0; j < programmecount; j++)
    {
        if(getprogrammetype(programmeheaders[j]) == 1)
        {
            printf("Header %d is true.\n", j);
            int offset = getprogrammeoffset(programmeheaders[j]);
            printf("Offset : 0x%x\n", offset);
            int address = getprogrammeaddress(programmeheaders[j]);
            printf("Address : 0x%x\n", address);
            int length = getprogrammelength(programmeheaders[j]);
            printf("Length : 0x%x\n", length);
            unsigned char programme[1474560];
            for (i = 0; i < length; i++) {
                programme[i] = elf[offset + i];
            }
            char *tw = "SCMD";
            fwrite(tw, sizeof(char), 4, pout);
            fwrite(&address, sizeof(int), 1, pout);
            fwrite(&length, sizeof(int), 1, pout);
            fwrite(programme, sizeof(unsigned char), length, pout);
        }
    }
    fclose(pout);
    fclose(psystem);
    return 0;
}

/**
 * Get the text entry of the elf.
 */
int getentry(unsigned char elfheader[])
{
    return (elfheader[24] * 0x1) + \
    (elfheader[25] * 0x100) + \
    (elfheader[26] * 0x10000) + \
    (elfheader[27] * 0x1000000);
}

/**
 * Get the nr of the elf file's program headers.
 */
int getprogrammeheadercount(unsigned char elfheader[])
{
    return (elfheader[44] * 0x1) + \
    (elfheader[45] * 0x100);
}

/**
 * Get the type of the elf file's program header.
 */
int getprogrammetype(unsigned char header[])
{
    return (header[0] * 0x1) + \
    (header[1] * 0x100) + \
    (header[2] * 0x10000) + \
    (header[3] * 0x1000000);
}

/**
 * Get the offset in the file of the elf 
 * file's program header.
 */
int getprogrammeoffset(unsigned char header[])
{
    return (header[4] * 0x1) + \
    (header[5] * 0x100) + \
    (header[6] * 0x10000) + \
    (header[7] * 0x1000000);
}

/**
 * Get the offset in the memory of the elf 
 * file's program header.
 */
int getprogrammeaddress(unsigned char header[])
{
    return (header[8] * 0x1) + \
    (header[9] * 0x100) + \
    (header[10] * 0x10000) + \
    (header[11] * 0x1000000);
}

/**
 * Get the length of a program header.
 */
int getprogrammelength(unsigned char header[])
{
    return (header[16] * 0x1) + \
    (header[17] * 0x100) + \
    (header[18] * 0x10000) + \
    (header[19] * 0x1000000);
}
