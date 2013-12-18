#include "fisheye.h"

void setFishEye(Pixel * data, int imageSize, int widthInt, int heightInt) {

	generateLUT();

	Pixel * newData = malloc(imageSize);
	memcpy(newData, data, imageSize);

	long width = toFixedPoint(widthInt), height = toFixedPoint(heightInt);
	long yFactor = toFixedPoint(2.0), yOffset = toFixedPoint(1.0);
	long xFactor = toFixedPoint(2.5), xOffset = toFixedPoint(1.0);
	int i = 0;
	for (long currentY = 0; currentY < height; currentY = addFixedPoint(currentY, 1)) {

		long normalizedY = divideFixedPoint(multiplyFixedPoints(yFactor, currentY), height) - yOffset;
		long normalizedY2 = multiplyFixedPoints(normalizedY, normalizedY);

		for (long currentX = 0; currentX < width; currentX = addFixedPoint(currentX, 1)) {


			long normalizedX = divideFixedPoint(multiplyFixedPoints(xFactor, currentX), width) - xOffset;
			long normalizedX2 = multiplyFixedPoints(normalizedX, normalizedX);

			long distanceRadicand = normalizedX2 + normalizedY2;
			long distanceToCenter = fixedSqrt(distanceRadicand);

			if (distanceToCenter >= 0 && distanceToCenter <= toFixedPoint(1.0)) { // 65536 = 1.0
				long newDistanceToCenter = (distanceToCenter + (toFixedPoint(1.0) - 
					fixedSqrt(toFixedPoint(1.0) - multiplyFixedPoints(distanceToCenter, distanceToCenter)))) / 2; // 131072 = 2.0

				if (newDistanceToCenter <= toFixedPoint(1.0)) {
					long angleTheta = arctan2(normalizedY, normalizedX);
					long newNormalizedX = multiplyFixedPoints(newDistanceToCenter, fixedCos(angleTheta));
					long newNormalizedY = multiplyFixedPoints(newDistanceToCenter, fixedSin(angleTheta));

					int x2 = (int) fromFixedPoint(multiplyFixedPoints( (newNormalizedX + toFixedPoint(1.0)), (width / 2)));

                    // map from -1 ... 1 to image coordinates
                    int y2 = (int) fromFixedPoint(multiplyFixedPoints( (newNormalizedY + toFixedPoint(1.0)), (height / 2)));
                    
                    // find (x2,y2) position from source pixels
                    int sourcePosition = y2 * ((int) fromFixedPoint(width)) + x2; //, fromFixedPoint(multiplyFixedPoints(y2, width) + x2);
                    
                    // make sure that position stays within arrays
                    if (sourcePosition >= 0 & sourcePosition < fromFixedPoint(multiplyFixedPoints(width, height))) {
                    	int destinationPosition = (int) fromFixedPoint(multiplyFixedPoints(currentY, width) + currentX);
                        newData[destinationPosition] = data[sourcePosition];    
                    }
				}
			}
			else {
				int currentPosition = fromFixedPoint(multiplyFixedPoints(currentY, width) + currentX);
        	  	newData[currentPosition].red = 0;
    	      	newData[currentPosition].green = 0;
	          	newData[currentPosition].blue = 0;
            }
		}

		i++;
	}

	memcpy(data, newData, imageSize);

}