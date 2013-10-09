#include "bitmap.h"

Pixel *LoadBitmapFile(FILE * filePtr, BITMAPINFOHEADER *bitmapInfoHeader, BITMAPFILEHEADER * bitmapFileHeader) {
    Pixel * bitmapImage;  //store image data
    int imageIdx = 0;  //image index counter
    unsigned char tempRGB;  //our swap variable

    printf("%s\n", "File Header:");
    printf("Type:  %x\n", bitmapFileHeader->bfType);
    printf("Size:  %d\n", bitmapFileHeader->bfSize);
    printf("Offbits:  %d\n", bitmapFileHeader->bfOffBits);

    // Read the bitmap info header
    fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);

    // Move file point to the beginning of bitmap data
    fseek(filePtr, bitmapFileHeader->bfOffBits, SEEK_SET);

    printf("\n%s\n", "Info Header:");
    printf("Size: %d\n", bitmapInfoHeader->biSize);
    printf("Bits per pixel: %d\n", bitmapInfoHeader->biBitCount);


    // Allocate enough memory for the bitmap image data
    // bitmapImage = (unsigned char*) malloc(bitmapInfoHeader->biSizeImage);
    bitmapImage = (Pixel *) malloc(bitmapInfoHeader->biSizeImage);

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

    // Close file and return bitmap image data
    fclose(filePtr);

    return bitmapImage;
}