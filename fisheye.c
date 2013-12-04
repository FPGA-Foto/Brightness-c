#include "fisheye.h"

void setFishEye(Pixel * data, int imageSize, int widthInt, int heightInt) {

	Pixel * newData = malloc(imageSize);
	memcpy(newData, data, imageSize);

	long width = toFixedPoint(widthInt), height = toFixedPoint(heightInt);
	long xFactor = toFixedPoint(2.5), xOffset = toFixedPoint(1.0);

	for (long currentY = 0; currentY < height; addFixedPoint(currentY, 1)) {

		// Distance in height between 0 and 1
		// double normalizedYn = ((2.0 * currentY) / height) - 1.0;
		// double normalizedY2n = (normalizedYn * normalizedYn);

		long normalizedY = ((2 * currentY) / height) - toFixedPoint(1.0);
		long normalizedY2 = multiplyFixedPoint(normalizedY * normalizedY);

		// printf("y: %d -> %f == %f\n", currentY, fromFixedPoint(normalizedY2), normalizedY2n);

		for (long currentX = 0; currentX < width; addFixedPoint(currentX++, 1)) {

			// double normalizedXn = ((2.5 * currentX)  / width) - 1.0;		
			
			// Distance in width between 0 and 1
			long normalizedX = multiplyFixedPoint(xFactor * currentX)  / width - xOffset;
			
			long normalizedX2 = multiplyFixedPoint(normalizedX * normalizedX);

			// printf("y: %d -> %f == %f\n", currentY, fromFixedPoint(normalizedX2), normalizedXn*normalizedXn);

			// printf("%f\n", sqrt((normalizedYn*normalizedYn) * (normalizedXn * normalizedXn)));
			// printf("sqrt: %d\n", sqrt2(toFixedPoint(25)));
			// printf("sqrt: %f\n", fromFixedPoint(magic_sqrt(25)));

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
                        newData[destinationPosition + 0] = data[sourcePosition];    
                    }
				}
			}
			else {
        	  	newData[currentY * width + currentX + 0].red = 0;
    	      	newData[currentY * width + currentX + 0].green = 0;
	          	newData[currentY * width + currentX + 0].blue = 0;
            }
		}
	}

	memcpy(data, newData, imageSize);

}