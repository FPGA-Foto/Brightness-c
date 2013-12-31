#ifndef HSL_H
#define HSL_H

#include <stdlib.h>
#include <math.h>
#include "std.h"
#include "bitmap.h"

#ifndef max
    #define max( a, b ) ( ((a) > (b)) ? (a) : (b) )
#endif

#ifndef min
    #define min( a, b ) ( ((a) < (b)) ? (a) : (b) )
#endif

void setContrast(unsigned char * data, int value, int imageSize);

long * RGBtoHSL(int originalRed, int originalGreen, int originalBlue);

int * HSLtoRGB(long hue, long saturation, long lightness);

void setHue(unsigned char * data, float value, int imageSize);

void setSaturation(unsigned char * data, float value, int imageSize);

void setLightness(unsigned char * data, float value, int imageSize);

#endif