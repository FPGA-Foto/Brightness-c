struct _Pixel
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};  

struct _ImageInfo
{
	int width;
	int height;
	int bytesPerPixel;
	int stride;
	unsigned char * bytes;
};

typedef struct _ImageInfo ImageInfo;
typedef struct _Pixel Pixel;
//typedef Pixel Pixel;

int setBrightness(unsigned char bytes[], ImageInfo *imageInfo);

int changePixel(Pixel *pixel, int value);