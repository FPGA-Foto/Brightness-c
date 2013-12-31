#include <math.h>
#include <string.h>

#include "bitmap.h"
#include "hsl.h"
#include "blur.h"
#include "fisheye.h"

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

        FILE *filePtr; // File pointer
        filePtr = fopen("image.bmp", "rb");
        if (filePtr == NULL) {
            return 0;
        }

        BITMAPFILEHEADER bitmapFileHeader; //our bitmap file header
        fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);

        if (bitmapFileHeader.bfType != 0x4D42) {
            fclose(filePtr);
            printf("%s\n", "Error: Not a BMP file.");
            return 0;
        }

        BITMAPINFOHEADER bitmapInfoHeader;
        Pixel * bitmapData = LoadBitmapFile(filePtr, &bitmapInfoHeader, &bitmapFileHeader);

        if (bitmapData == NULL) {
            printf("%s\n", "Error: Data is empty.");
            return 0;
        }

        printf("Width: %d\nHeight: %d\nAspect ratio: %f\n", 
            bitmapInfoHeader.biWidth, bitmapInfoHeader.biHeight, 
            ((float) bitmapInfoHeader.biWidth / (float) bitmapInfoHeader.biHeight));

        // setBlur(bitmapData, 4, bitmapInfoHeader.biSizeImage, bitmapInfoHeader.biWidth, bitmapInfoHeader.biHeight);
        // setFishEye(bitmapData, bitmapInfoHeader.biSizeImage, bitmapInfoHeader.biWidth, bitmapInfoHeader.biHeight);
        setHue(bitmapData, hue, bitmapInfoHeader.biSizeImage);
        // setSaturation(bitmapData, saturation, bitmapInfoHeader.biSizeImage);
        // setLightness(bitmapData, brightness, bitmapInfoHeader.biSizeImage);

        FILE * writeFile = fopen("out.bmp", "wb");

        fwrite(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, writeFile);
        fwrite(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, writeFile);
        fwrite(bitmapData, bitmapInfoHeader.biSizeImage, 1, writeFile);
    }
}