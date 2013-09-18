#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "brightness.h"

#define ArraySize 40000

int main()
{
	unsigned char bytes[ArraySize] = { 0 };

	ImageInfo imageInfo;
	imageInfo.bytes = bytes;
	imageInfo.bytesPerPixel = 3;
	imageInfo.height = 50;
	imageInfo.width = 50;
	imageInfo.stride = imageInfo.width * imageInfo.bytesPerPixel;
	
	//setBrightness(bytes);
}