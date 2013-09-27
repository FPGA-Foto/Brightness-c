#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define BYTE 8

#pragma pack(push, 1)

typedef struct tagBITMAPFILEHEADER
{
	short bfType : 2 * BYTE; //specifies the file type
	int bfSize : 4 * BYTE; //specifies the size in bytes of the bitmap file
	short bfReserved1 : 2 * BYTE; //reserved; must be 0
	short bfReserved2 : 2 * BYTE; //reserved; must be 0
	int bfOffBits : 4 * BYTE; //species the offset in bytes from the bitmapfileheader to the bitmap bits
} BITMAPFILEHEADER;

#pragma pack(pop)

#pragma pack(push, 1)

typedef struct tagBITMAPINFOHEADER
{
	int biSize : 4 * BYTE; //specifies the number of bytes required by the struct
	int biWidth : 4 * BYTE; //specifies width in pixels
	int biHeight : 4 * BYTE; //species height in pixels
	short biPlanes : 2 * BYTE; //specifies the number of color planes, must be 1
	short biBitCount : 2 * BYTE; //specifies the number of bit per pixel
	int biCompression : 4 * BYTE; //spcifies the type of compression
	int biSizeImage : 4 * BYTE; //size of image in bytes
	int biXPelsPerMeter : 4 * BYTE; //number of pixels per meter in x axis
	int biYPelsPerMeter : 4 * BYTE; //number of pixels per meter in y axis
	int biClrUsed : 4 * BYTE; //number of colors used by th ebitmap
	int biClrImportant : 4 * BYTE; //number of colors that are important
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

int getPixelPositionX(int index, int width)
{
	return (index % width);
}

int getPixelPositionY(int index, int width)
{
	int result = index / width;

	return result;
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

unsigned char calcPixelBlur(int index, unsigned char * data, int positionX, int positionY, int width, int height) {
	int precision = 10;
	int pixelsleft, pixelsright, pixelstop, pixelsbottom; // pixels die gebruikt worden voor weight
	double matrix[10][10];
	int i, j;

	if (positionX < precision)
	{
		pixelsleft = positionX;
	}
	else if (positionX > (width - precision))
	{
		pixelsright = width - positionX;
	}

	if (positionY < precision)
	{
		pixelstop = positionY;
	}
	else if (positionY > (height - precision))
	{
		pixelsbottom = height - positionY;
	}

	for (i = 0; i < precision; i++)
	{
		for (j = 0; j < precision; j++)
		{
			
		}
	}

	return data[index];
}

void setBlur(unsigned char * data, int value, int imageSize, int width, int height) {
	int i;
	int positionX;
	int positionY;
	int pixels = imageSize / 3;

	for (i = 0; i < pixels; i++)
	{
		positionX = getPixelPositionX(i, width);
		positionY = getPixelPositionY(i, width);

		data[i] = calcPixelBlur(i, data, positionX, positionY, width, height);
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

	//setBrightness(bitmapData, 50, bitmapInfoHeader.biSizeImage);
	//setContrast(bitmapData, 180, bitmapInfoHeader.biSizeImage);
	setBlur(bitmapData, 1, bitmapInfoHeader.biSizeImage, bitmapInfoHeader.biWidth, bitmapInfoHeader.biHeight);

	FILE * writeFile = fopen("out.bmp", "wb");

	fwrite(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, writeFile);
	fwrite(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, writeFile);
	fwrite(bitmapData, bitmapInfoHeader.biSizeImage, 1, writeFile);

}