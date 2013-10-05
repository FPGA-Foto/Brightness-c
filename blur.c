#include "blur.h"

int getPixelPositionX(int index, int width)
{
	return (index % width);
}
	
int getPixelPositionY(int index, int width)
{
	return index / width;
}

int getIndexPixel(int x, int y, int width)
{
	return (y * width) + x;
}

double calcWeight(double factor, int x, int y) {
	double euler = 2.71828182846;
	double pi = 3.14;

	double luh = 2 * pi * (factor * factor);
	double lh = 1 / luh;

	double ruh = (x * x) + (y * y);
	double rlh = 2 * (factor * factor);
	double rh = ruh / rlh;

	double mr = pow(euler, rh);
	double ret = lh * mr;

	return ret;
}

void calcPixelBlur(int index, unsigned char * data, double value, int positionX, int positionY, int width, int height) {
	int precision = value;
	int pixelsleft = precision, pixelsright = precision, pixelstop = precision, pixelsbottom = precision; // pixels die gebruikt worden voor weight
	int i, j;
	int pixelpos;
	double pixelweight;
	char r, g, b;
	char newpixelR = 0;
	char newpixelG = 0;
	char newpixelB = 0;

	index = index * 3;

	if (positionX < precision)
	{
		pixelsleft = positionX;
	}
	else if (positionX >(width - precision))
	{
		pixelsright = width - positionX;
	}

	if (positionY < precision)
	{
		pixelstop = positionY;
	}
	else if (positionY >(height - precision))
	{
		pixelsbottom = height - positionY;
	}

	for (i = -pixelsleft; i < pixelsright; i++)
	{
		for (j = -pixelstop; j < pixelsbottom; j++)
		{
			pixelpos = (index + (i * 3)) + ((j * 3)* width);

			pixelweight = calcWeight(value, i, j);
			r = (char) (data[pixelpos] * pixelweight);
			g = (char) (data[pixelpos + 1] * pixelweight);
			b = (char) (data[pixelpos + 2] * pixelweight);

			newpixelR += r;
			newpixelG += g;
			newpixelB += b;
		}
	}

	data[index] = newpixelR;
	data[index + 1] = newpixelG;
	data[index + 2] = newpixelB;
}

void setBlur(unsigned char * data, double value, int imageSize, int width, int height) {
	int i;
	int positionX;
	int positionY;
	int pixels = imageSize / 3;

	for (i = 0; i < pixels; i++)
	{
		positionX = getPixelPositionX(i, width);
		positionY = getPixelPositionY(i, width);

		calcPixelBlur(i, data, value, positionX, positionY, width, height);
	}
}