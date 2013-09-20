#include <stdio.h>
#include <stdlib.h>

#define BYTE 8

#pragma pack(push, 1)

typedef struct tagBITMAPFILEHEADER
{
    short int bfType        :2*BYTE; //specifies the file type
    int bfSize              :4*BYTE; //specifies the size in bytes of the bitmap file
    short int bfReserved1   :2*BYTE; //reserved; must be 0
    short int bfReserved2   :2*BYTE; //reserved; must be 0
    int bfOffBits           :4*BYTE; //species the offset in bytes from the bitmapfileheader to the bitmap bits
} BITMAPFILEHEADER;

#pragma pack(pop)

#pragma pack(push, 1)

typedef struct tagBITMAPINFOHEADER
{
    int biSize                  :4*BYTE; //specifies the number of bytes required by the struct
    int biWidth                 :4*BYTE; //specifies width in pixels
    int biHeight                :4*BYTE; //species height in pixels
    short int biPlanes          :2*BYTE; //specifies the number of color planes, must be 1
    short int biBitCount        :2*BYTE; //specifies the number of bit per pixel
    int biCompression           :4*BYTE; //spcifies the type of compression
    int biSizeImage             :4*BYTE; //size of image in bytes
    int biXPelsPerMeter         :4*BYTE; //number of pixels per meter in x axis
    int biYPelsPerMeter         :4*BYTE; //number of pixels per meter in y axis
    int biClrUsed               :4*BYTE; //number of colors used by th ebitmap
    int biClrImportant          :4*BYTE; //number of colors that are important
} BITMAPINFOHEADER;

#pragma pack(pop)

unsigned char *LoadBitmapFile(FILE * filePtr, BITMAPINFOHEADER *bitmapInfoHeader, BITMAPFILEHEADER * bitmapFileHeader) {
    unsigned char *bitmapImage;  //store image data
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

    // Close file and return bitmap image data
    fclose(filePtr);

    return bitmapImage;
}

void main() {
    FILE *filePtr; //our file pointer
    filePtr = fopen("image.bmp", "rb");
    if (filePtr == NULL) {
        return;
    }

    BITMAPFILEHEADER bitmapFileHeader; //our bitmap file header
    fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);

    if (bitmapFileHeader.bfType != 0x4D42) {
        fclose(filePtr);
        printf("%s\n", "Error: Not a BMP file.");
        return;
    }

    BITMAPINFOHEADER bitmapInfoHeader;
    unsigned char *bitmapData;
    bitmapData = LoadBitmapFile(filePtr, &bitmapInfoHeader, &bitmapFileHeader);

    if (bitmapData == NULL) {
        printf("%s\n", "Error: Data is empty.");
        return;
    }

    int i;
    for (i = 0; i < bitmapInfoHeader.biSizeImage; i += 3) {
        int j;
        for (j = 0; j < 3; j++) {
            int pixel = bitmapData[i+j], modifier = -50;
            
            if (pixel + modifier > 255) {
                bitmapData[i+j] = 255;
            }
            else if (pixel + modifier < 0) {
                bitmapData[i+j] = 0;                
            }
            else {
                bitmapData[i+j] += modifier;
            }
        }
    }

    FILE * writeFile = fopen("out.bmp", "w");

    fwrite(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, writeFile);
    fwrite(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, writeFile);
    fwrite(bitmapData, bitmapInfoHeader.biSizeImage, 1, writeFile);

}