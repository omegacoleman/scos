
/**
 * 
 * Copyright (c) Stephen Coleman
 * 
 * This file is for building the disk image for SCOS.
 * 
 */

#include <stdio.h>
#include <stdlib.h>

const char *diskimage = "./SCOS.img";
const int imagesize = 1474560;

void getdata(const char *filename, long size, unsigned char pdata[]);
void adddata(const char *filename, long size, FILE *pdiskimage);
long addmodules(FILE *pdiskimage);
void buildimage(FILE *pdiskimage);

/**
 * The main method for the programme.
 */
int main()
{
    FILE *pdiskimage = fopen(diskimage, "wb");
    buildimage(pdiskimage);
    fclose(pdiskimage);
    return 0;
}

/**
 * Build the image for SCOS.
 */
void buildimage(FILE *pdiskimage)
{
    long diskwritten = addmodules(pdiskimage);
    long blanksize = imagesize - diskwritten;
    long blanklongs = blanksize / sizeof(unsigned char);
    unsigned char blank = 0x0;
    long i;
    for (i = 0; i < blanklongs; i++)
    {
        fwrite(&blank, sizeof(unsigned char), 1, pdiskimage);
    }
}

/**
 * Put everything together here.
 */
long addmodules(FILE *pdiskimage)
{
    long diskwritten = 0;
    diskwritten += 512;
    adddata("./floppysector.sec", 512, pdiskimage);
    diskwritten += 2048;
    adddata("./loader.bin", 2048, pdiskimage);
    diskwritten += 196608;
    adddata("./sysmodule.scmd", 196608, pdiskimage);
    return diskwritten;
}

/**
 * Add data to the image.
 */
void adddata(const char *filename, long size, FILE *pdiskimage)
{
    unsigned char pdata[size / sizeof(unsigned char)];
    getdata(filename, size, pdata);
    fwrite(pdata, sizeof(unsigned char), \
    size / sizeof(unsigned char), pdiskimage);
}

/**
 * Get data from a file.
 */
void getdata(const char *filename, long size, unsigned char pdata[])
{
    FILE *pfile = fopen(filename, "rb");
    long longs = size / sizeof(unsigned char);
    // long pdata[longs];
    fread(pdata, sizeof(unsigned char), longs, pfile);
    fclose(pfile);
}
