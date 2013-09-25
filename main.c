#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define BYTE 8

#ifndef max
    #define max( a, b ) ( ((a) > (b)) ? (a) : (b) )
#endif

#ifndef min
    #define min( a, b ) ( ((a) < (b)) ? (a) : (b) )
#endif

#pragma pack(push, 1)

typedef struct tagBITMAPFILEHEADER
{
    short bfType        :2*BYTE; //specifies the file type
    int bfSize              :4*BYTE; //specifies the size in bytes of the bitmap file
    short bfReserved1   :2*BYTE; //reserved; must be 0
    short bfReserved2   :2*BYTE; //reserved; must be 0
    int bfOffBits           :4*BYTE; //species the offset in bytes from the bitmapfileheader to the bitmap bits
} BITMAPFILEHEADER;

#pragma pack(pop)

#pragma pack(push, 1)

typedef struct tagBITMAPINFOHEADER
{
    int biSize                  :4*BYTE; //specifies the number of bytes required by the struct
    int biWidth                 :4*BYTE; //specifies width in pixels
    int biHeight                :4*BYTE; //species height in pixels
    short biPlanes              :2*BYTE; //specifies the number of color planes, must be 1
    short biBitCount            :2*BYTE; //specifies the number of bit per pixel
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

void setBrightness(unsigned char * data, int value, int imageSize) {
    int i;
    for (i = 0; i < imageSize; i++) {

        if (data[i] + value > 255) {
            data[i] = 255;
        }
        else if (data[i] + value < 0) {
            data[i] = 0;                
        }
        else {
            data[i] += value;
        }
    }
}

void setContrast(unsigned char * data, int value, int imageSize) {
    int i;
    for (i = 0; i < imageSize; i++) {
        int factor = (259 * (value + 255)) / (255 * (259 - value));
        int color = factor * (data[i] - 128) + 128;    

        if (color > 255) {
            data[i] = 255;
        }
        else if (color < 0) {
            data[i] = 0;                
        }
        else {
            data[i] = color;
        }
    }
}

float * rgbToHsl(float * red, float * green, float * blue) {
    *red /= 255, *green /= 255, *blue /= 255;
    float max = fmax(fmax(*red, *green), *blue), min = fmin(fmin(*red, *green), *blue);
    float hue, saturation, lightness = (max + min) / 2;

    if (max == min) {
        hue = saturation = 0; // achromatic
    } else {
        float d = max - min;
        saturation = lightness > 0.5 ? d / (2 - max - min) : d / (max + min);

        if (max == *red) {
            hue = (*green - *blue) / d + (*green < *blue ? 6 : 0);
        } else if (max == *green) {
            hue = (*blue - *red) / d + 2;
        } else if (max == *blue) {
            hue = (*red - *green) / d + 4;
        }

        hue /= 6;
    }

    float * hsl = malloc(3 * sizeof(float));
    hsl[0] = hue;
    hsl[1] = saturation;
    hsl[2] = lightness;

    return hsl;
}


float * HSLtoRGB(float * hue, float * saturation, float * lightness) {
    
    float red, green, blue; //this function works with floats between 0 and 1
    float temp1, temp2, tempr, tempg, tempb;
    
    *hue /= 256.0;
    *saturation /= 256.0;
    *lightness /= 256.0;

    //If saturation is 0, the color is a shade of gray
    if (*saturation == 0) {
        red = green = blue = *lightness;
    }
    //If saturation > 0, more complex calculations are needed
    else {
        //Set the temporary values      
        if (*lightness < 0.5) temp2 = *lightness * (1 + *saturation);      
        else temp2 = (*lightness + *saturation) - (*lightness * *saturation);     
        
        temp1 = 2 * *lightness - temp2;    
        tempr = *hue + 1.0 / 3.0;    
        
        if (tempr > 1) tempr--;
        
        tempg = *hue;     
        tempb = *hue - 1.0 / 3.0;
        
        if (tempb < 0) tempb++; 
        
        //Red     
        if (tempr < 1.0 / 6.0) red = temp1 + (temp2 - temp1) * 6.0 * tempr;      
        else if (tempr < 0.5) red = temp2;   
        else if (tempr < 2.0 / 3.0) red = temp1 + (temp2 - temp1) * ((2.0 / 3.0) - tempr) * 6.0;
        else red = temp1; 
        
        //Green       
        if (tempg < 1.0 / 6.0) green = temp1 + (temp2 - temp1) * 6.0 * tempg;    
        else if (tempg < 0.5) green = temp2;
        else if (tempg < 2.0 / 3.0) green = temp1 + (temp2 - temp1) * ((2.0 / 3.0) - tempg) * 6.0;
        else green = temp1; 
        
        //Blue    
        if (tempb < 1.0 / 6.0) blue = temp1 + (temp2 - temp1) * 6.0 * tempb;   
        else if (tempb < 0.5) blue = temp2; 
        else if (tempb < 2.0 / 3.0) blue = temp1 + (temp2 - temp1) * ((2.0 / 3.0) - tempb) * 6.0;    
        else blue = temp1;

        float * rgb = malloc(3 * sizeof(float));
        rgb[0] = red;
        rgb[1] = green;
        rgb[2] = blue;

        return rgb;
    }
}

void setHue(unsigned char * data, int value, int imageSize) {
    int i;
    for (i = 0; i < imageSize; i += 3) {

        float * red = (float *) data[i], * green = (float *) data[i+1], * blue = (float *) data[i+2];
        float * hsl = rgbToHsl(red, green, blue);

        hsl[0] *= value;

        printf("%f, %f, %f\n", hsl[0], hsl[1], hsl[2]);

        // float rgb[] = HSLtoRGB(hsl[0], hsl[1], hsl[2]);

        // float hue = atan2(sqrt(3) * (green - blue), 2 * red - green - blue);

        // printf("%f\n", hue);

        // if (color > 255) {
        //     data[i] = 255;
        // }
        // else if (color < 0) {
        //     data[i] = 0;                
        // }
        // else {
        //     data[i] = color;
        // }
    }
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

    // setBrightness(bitmapData, 50, bitmapInfoHeader.biSizeImage);
    // setContrast(bitmapData, 180, bitmapInfoHeader.biSizeImage);
    setHue(bitmapData, 180, bitmapInfoHeader.biSizeImage);

    FILE * writeFile = fopen("out.bmp", "wb");

    fwrite(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, writeFile);
    fwrite(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, writeFile);
    fwrite(bitmapData, bitmapInfoHeader.biSizeImage, 1, writeFile);

}