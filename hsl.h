#ifndef HSL_H
#define HSL_H

#include <stdlib.h>
#include <math.h>

#ifndef max
    #define max( a, b ) ( ((a) > (b)) ? (a) : (b) )
#endif

#ifndef min
    #define min( a, b ) ( ((a) < (b)) ? (a) : (b) )
#endif

void setContrast(unsigned char * data, int value, int imageSize);

float * RGBtoHSL(int red_ptr, int green_ptr, int blue_ptr);

int * HSLtoRGB(float hue, float saturation, float lightness);

void setHue(unsigned char * data, float value, int imageSize);

void setSaturation(unsigned char * data, float value, int imageSize);

void setLightness(unsigned char * data, float value, int imageSize);

#endif