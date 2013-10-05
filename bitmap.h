#ifndef BITMAP_H
#define BITMAP_H 

#include <stdio.h>
#include <stdlib.h>

#define BYTE 8

#pragma pack(push, 1)

typedef struct tagBITMAPFILEHEADER
{
    short bfType        :2*BYTE; //specifies the file type
    int bfSize          :4*BYTE; //specifies the size in bytes of the bitmap file
    short bfReserved1   :2*BYTE; //reserved; must be 0
    short bfReserved2   :2*BYTE; //reserved; must be 0
    int bfOffBits       :4*BYTE; //species the offset in bytes from the bitmapfileheader to the bitmap bits
} BITMAPFILEHEADER;

#pragma pack(pop)

#pragma pack(push, 1)

typedef struct tagBITMAPINFOHEADER
{
    int biSize          :4*BYTE; //specifies the number of bytes required by the struct
    int biWidth         :4*BYTE; //specifies width in pixels
    int biHeight        :4*BYTE; //species height in pixels
    short biPlanes      :2*BYTE; //specifies the number of color planes, must be 1
    short biBitCount    :2*BYTE; //specifies the number of bit per pixel
    int biCompression   :4*BYTE; //spcifies the type of compression
    int biSizeImage     :4*BYTE; //size of image in bytes
    int biXPelsPerMeter :4*BYTE; //number of pixels per meter in x axis
    int biYPelsPerMeter :4*BYTE; //number of pixels per meter in y axis
    int biClrUsed       :4*BYTE; //number of colors used by th ebitmap
    int biClrImportant  :4*BYTE; //number of colors that are important
} BITMAPINFOHEADER;

#pragma pack(pop)

unsigned char *LoadBitmapFile(FILE * filePtr, BITMAPINFOHEADER *bitmapInfoHeader, BITMAPFILEHEADER * bitmapFileHeader);

#endif