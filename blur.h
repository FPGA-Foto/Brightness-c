#ifndef BLUR_H
#define BLUR_H

#include <math.h>

int getPixelPositionX(int index, int width);
	
int getPixelPositionY(int index, int width);

int getIndexPixel(int x, int y, int width);

double calcWeight(double factor, int x, int y);

void calcPixelBlur(int index, unsigned char * data, double value, int positionX, int positionY, int width, int height);

void setBlur(unsigned char * data, double value, int imageSize, int width, int height);

#endif