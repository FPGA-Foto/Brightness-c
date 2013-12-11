#include "fisheye.h"

void setFishEye(Pixel * data, int imageSize, int widthInt, int heightInt) {

	Pixel * newData = malloc(imageSize);
	memcpy(newData, data, imageSize);

	long width = toFixedPoint(widthInt), height = toFixedPoint(heightInt);
	long yFactor = toFixedPoint(2.0), yOffset = toFixedPoint(1.0);
	long xFactor = toFixedPoint(2.5), xOffset = toFixedPoint(1.0);
	int i = 0;
	for (long currentY = 0; currentY < height; currentY = addFixedPoint(currentY, 1)) {

		long normalizedY = divideFixedPoint(multiplyFixedPoints(yFactor, currentY), height) - yOffset;
		printf("normalizedY: %f\n", fromFixedPoint(normalizedY));
		long normalizedY2 = multiplyFixedPoints(normalizedY, normalizedY);
		printf("normalizedY2: %f\n", fromFixedPoint(normalizedY2));

		for (long currentX = 0; currentX < width; currentX = addFixedPoint(currentX, 1)) {
			// printf("\n");

			// printf("currentX: %d == width: %d\n", currentX, width);

			long normalizedX = divideFixedPoint(multiplyFixedPoints(xFactor, currentX), width) - xOffset;
			printf("normalizedX: %f\n", fromFixedPoint(normalizedX));
			long normalizedX2 = multiplyFixedPoints(normalizedX, normalizedX);
			printf("normalizedX2: %f\n", fromFixedPoint(normalizedX2));

			long distanceToCenter = fixedSqrt(normalizedX2 + normalizedY2);
			// printf("distanceToCenter: %f\n", fromFixedPoint(distanceToCenter));

			if (distanceToCenter >= 0 && distanceToCenter <= 65536) { // 65536 = 1.0
				long newDistanceToCenter = (distanceToCenter + (65536 - fixedSqrt(65536 - distanceToCenter*distanceToCenter))) / 2; // 131072 = 2.0
				// printf("newDistanceToCenter: %f\n", fromFixedPoint(newDistanceToCenter));				

				if (newDistanceToCenter <= 65536) {
					long angleTheta = atan2_fp(normalizedY, normalizedX);
					// printf("angleTheta: %f\n", fromFixedPoint(angleTheta));
					long newNormalizedX = multiplyFixedPoints(newDistanceToCenter, fixedCos(angleTheta));
					long newNormalizedY = multiplyFixedPoints(newDistanceToCenter, fixedSin(angleTheta));

					long x2 = multiplyFixedPoints((newNormalizedX + 65536), (width / 2));
                    // printf("x2: %f\n",fromFixedPoint(x2));

                    // map from -1 ... 1 to image coordinates
                    long y2 = multiplyFixedPoints((newNormalizedY + 65536), (height / 2));
                    // printf("y2: %f\n", fromFixedPoint(y2));
                    // find (x2,y2) position from source pixels
                    int sourcePosition = (int) fromFixedPoint(multiplyFixedPoints(y2, width) + x2);            
                    
                    // make sure that position stays within arrays
                    if (sourcePosition >= 0 & sourcePosition < multiplyFixedPoints(width, height)) {
                    	int destinationPosition = (int) fromFixedPoint(multiplyFixedPoints(currentY, width) + currentX);
                        // printf("%d == %d\n", sourcePosition, destinationPosition);
                        newData[destinationPosition + 0] = data[sourcePosition];    
                    }
				}
				// printf("i: %d\n", i);
				i++;
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