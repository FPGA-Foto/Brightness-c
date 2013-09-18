#include <stdio.h>
#include <stdlib.h>

#define BYTE 8

#pragma pack(push, 1)

typedef struct tagBITMAPFILEHEADER
{
    int bfType      :2*BYTE; //specifies the file type
    int bfSize      :4*BYTE; //specifies the size in bytes of the bitmap file
    int bfReserved1 :2*BYTE; //reserved; must be 0
    int bfReserved2 :2*BYTE; //reserved; must be 0
    int bfOffBits   :4*BYTE; //species the offset in bytes from the bitmapfileheader to the bitmap bits
} BITMAPFILEHEADER;

#pragma pack(pop)

#pragma pack(push, 1)

typedef struct tagBITMAPINFOHEADER
{
    int biSize                  :4*BYTE;  //specifies the number of bytes required by the struct
    long int biWidth            :4*BYTE;  //specifies width in pixels
    long int biHeight           :4*BYTE;  //species height in pixels
    int biPlanes                :2*BYTE; //specifies the number of color planes, must be 1
    int biBitCount              :2*BYTE; //specifies the number of bit per pixel
    int biCompression           :4*BYTE;//spcifies the type of compression
    int biSizeImage             :4*BYTE;  //size of image in bytes
    long int biXPelsPerMeter    :4*BYTE;  //number of pixels per meter in x axis
    long int biYPelsPerMeter    :4*BYTE;  //number of pixels per meter in y axis
    int biClrUsed               :4*BYTE;  //number of colors used by th ebitmap
    int biClrImportant          :4*BYTE;  //number of colors that are important
} BITMAPINFOHEADER;

#pragma pack(pop)

unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader) {
    FILE *filePtr; //our file pointer
    BITMAPFILEHEADER bitmapFileHeader; //our bitmap file header
    unsigned char *bitmapImage;  //store image data
    int imageIdx = 0;  //image index counter
    unsigned char tempRGB;  //our swap variable

    //open filename in read binary mode
    filePtr = fopen(filename,"r");
    if (filePtr == NULL) {
        return NULL;
    }

    // Read the bitmap file header
    int fileType[10];
    fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);

    // printf("%d\n", bitmapFileHeader.bfSize);

    // Verify that this is a bmp file by check bitmap id
    if (bitmapFileHeader.bfType != 0x4D42) {
        fclose(filePtr);
        printf("%s\n", "Error: Not a BMP file.");
        return NULL;
    }

    printf("%s\n", "File Header:");
    printf("Type:  %x\n", bitmapFileHeader.bfType);
    printf("Size:  %d\n", bitmapFileHeader.bfSize);
    printf("Offbits:  %d\n", bitmapFileHeader.bfOffBits);

    // Read the bitmap info header
    fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);

    // Move file point to the beginning of bitmap data
    fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

    printf("\n%s\n", "Info Header:");
    printf("Size: %d\n", bitmapInfoHeader->biSize);
    printf("Bits per pixel: %d\n", bitmapInfoHeader->biBitCount);


    // Allocate enough memory for the bitmap image data
    bitmapImage = (unsigned char*) malloc(bitmapInfoHeader->biSizeImage);

    // Verify memory allocation
    if (!bitmapImage) {
        printf("%s\n", "Error: Memory not allocated.");
        free(bitmapImage);
        fclose(filePtr);
        return NULL;
    }

    // Read in the bitmap image data
    fread(bitmapImage, bitmapInfoHeader->biSizeImage, 1, filePtr);

    // Make sure bitmap image data was read
    if (bitmapImage == NULL) {
        printf("%s\n", "Error: Image data not read.");
        fclose(filePtr);
        return NULL;
    }

    // Swap the r and b values to get RGB (bitmap is BGR)
    for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx += 3) {
        tempRGB = bitmapImage[imageIdx];
        bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
        bitmapImage[imageIdx + 2] = tempRGB;
    }

    // Close file and return bitmap image data
    fclose(filePtr);

    return bitmapImage;
}

void main() {
    BITMAPINFOHEADER bitmapInfoHeader;
    unsigned char *bitmapData;
    bitmapData = LoadBitmapFile("image.bmp", &bitmapInfoHeader);

    if (bitmapData == NULL) {
        printf("%s\n", "Error: Data is empty.");
        return;
    }

    int i;
    for (i = 0; i < bitmapInfoHeader.biSizeImage/10; i += 4) {
        int j;
        for (j = 0; j < 2; j++) {
            printf("%x", bitmapData[i+j]);
        }
        printf("%s", " ");
    }

}