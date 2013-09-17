// Last updated 2008/11/04 10:53

// A simple example equivalent to
// convert logo: logo.jpg

#include <wand/magick_wand.h>

int main() {
	test_wand();
}

void test_wand(void)
{
	MagickWand *magick_wand = NULL;
	// Set default fuzz to zero (see below)
	double fuzz = 0.;
	PixelWand *target;

	MagickWandGenesis();

	magick_wand = NewMagickWand();
	MagickReadImage(magick_wand,"image.jpg");

	// Set up the pixelwand containing the colour to be "targeted"
	// by transparency
	target = NewPixelWand();
	PixelSetColor(target, "white");
	
	Image * img = GetImageFromMagickWand(magick_wand);

	BrightnessContrastImage(img, 50.0, 0.0);
	
	MagickWriteImage(NewMagickWandFromImage(img), "logo_white.png");
	
	/* Clean up */
	if (magick_wand) magick_wand = DestroyMagickWand(magick_wand);
	if (target) target = DestroyPixelWand(target);
	MagickWandTerminus();
}