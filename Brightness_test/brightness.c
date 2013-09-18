#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "brightness.h"

int setBrightness(unsigned char bytes[], ImageInfo *imageInfo)
{
	Pixel pixel;
	int row = 0;
	int colum = 0;
	int i = 0;
	int b = 0;

	imageInfo->stride = imageInfo->width + imageInfo->bytesPerPixel;

	for(row = 0; row < imageInfo->height; row++)
	{
		for(i = 0; i < imageInfo->stride; i+=imageInfo->bytesPerPixel)
		{
			for(b = 0; b < imageInfo->bytesPerPixel; b++)
			{
				switch (b)
				{
					case 0:
						pixel.red = bytes[(i+b)];
						break;
					case 1:
						pixel.green = bytes[(i+b)];
						break;
					case 2:
						pixel.blue = bytes[(i+b)];
						break;
					default:
						break;
				}
			}
		}
	}
}

int changePixel(Pixel *pixel, int value)
{
	pixel->red = ((pixel->red)+value);
	pixel->green = ((pixel->green)+value);
	pixel->blue = ((pixel->blue)+value);
	
}

