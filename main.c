#include <math.h>
#include <string.h>

#include "bitmap.h"
#include "hsl.h"

int main(int argc, char *argv[]) {

    if (argc != 4) {
        printf("%s\n", "No parameters have been specified. Please use this command: bmp <hue> <saturation> <brightness>");
    }
    else {
        float hue = atof(argv[1]), saturation = atof(argv[2]), brightness = atof(argv[3]);

        if (hue == 0 || saturation == 0 || brightness == 0) {
            printf("%s\n", "One of the parameters is not a number.");
            return 1;
        }

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

        setHue(bitmapData, hue, bitmapInfoHeader.biSizeImage);
        setSaturation(bitmapData, saturation, bitmapInfoHeader.biSizeImage);
        setLightness(bitmapData, brightness, bitmapInfoHeader.biSizeImage);


        FILE * writeFile = fopen("out.bmp", "wb");

        fwrite(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, writeFile);
        fwrite(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, writeFile);
        fwrite(bitmapData, bitmapInfoHeader.biSizeImage, 1, writeFile);
    }
}