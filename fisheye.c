#include "fisheye.h"

void setFishEye(Pixel * data, double value, int imageSize, int width, int height) {

	Pixel * newData = malloc(imageSize);
	memcpy(newData, data, imageSize);

	int currentY;
	for (currentY = 0; currentY < height; currentY++) {

		// Distance in height between 0 and 1               +Centers image
		double normalizedY = ((2.0 * currentY) / height) - 1.0;
		double normalizedY2 = normalizedY*normalizedY;

		int currentX;
		for (currentX = 0; currentX < width; currentX++) {
			
			// Distance in width between 0 and 1
			double normalizedX = ((2.5 * currentX) / width) - 1.0;
			double normalizedX2 = normalizedX * normalizedX;

			double distanceToCenter = sqrt(normalizedX2 + normalizedY2);

			if (distanceToCenter >= 0.0 && distanceToCenter <= 1.0) {
				double newDistanceToCenter = (distanceToCenter + 
					(1.0 - sqrt(1.0 - distanceToCenter*distanceToCenter))) / 2.0;

				if (newDistanceToCenter <= 1.0) {
					double angleTheta = atan2(normalizedY, normalizedX);
					double newNormalizedX = newDistanceToCenter * cos(angleTheta);
					double newNormalizedY = newDistanceToCenter * sin(angleTheta);

					int x2 = (int) ((newNormalizedX + 1.0) * (width / 2.0));
                    
                    // map from -1 ... 1 to image coordinates
                    int y2 = (int) ((newNormalizedY + 1.0) * (height / 2.0));
                    
                    // find (x2,y2) position from source pixels
                    int sourcePosition = (int) (y2 * width+ + x2);            
                    
                    // make sure that position stays within arrays
                    if (sourcePosition >= 0 & sourcePosition < width * height) {
                    	int destinationPosition = (int) (currentY * width + currentX);
                        newData[destinationPosition + 400] = data[sourcePosition];    
                    }
				}
			}
			else {
        	  	newData[currentY * width + currentX + 400].red = 0;
    	      	newData[currentY * width + currentX + 400].green = 0;
	          	newData[currentY * width + currentX + 400].blue = 0;
            }
		}
	}

	memcpy(data, newData, imageSize);

}