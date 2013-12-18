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
		// printf("normalizedY: %f\n", fromFixedPoint(normalizedY));
		long normalizedY2 = multiplyFixedPoints(normalizedY, normalizedY);
		// printf("normalizedY2: %f\n", fromFixedPoint(normalizedY2));

		for (long currentX = 0; currentX < width; currentX = addFixedPoint(currentX, 1)) {
			// printf("\n");

			// printf("currentX: %d == width: %d\n", currentX, width);

			long normalizedX = divideFixedPoint(multiplyFixedPoints(xFactor, currentX), width) - xOffset;
			// printf("normalizedX: %f\n", fromFixedPoint(normalizedX));
			long normalizedX2 = multiplyFixedPoints(normalizedX, normalizedX);
			// printf("normalizedX2: %f\n", fromFixedPoint(normalizedX2));

			long distanceRadicand = normalizedX2 + normalizedY2;
			// printf("\ndistanceRadicand: %ld\n", distanceRadicand);
			long distanceToCenter = fixedSqrt(distanceRadicand);
			// printf("normalizedX2: %ld,  normalizedY2: %ld = %ld\n", normalizedX2, normalizedY2, (normalizedX2 + normalizedY2));
			// printf("normalizedX: %f,  normalizedY: %f\n", fromFixedPoint(normalizedX), fromFixedPoint(normalizedY));
			// printf("normalizedY2: %f, normalizedX2: %f: distanceToCenter: %ld\n", fromFixedPoint(normalizedY2), fromFixedPoint(normalizedX2), fromFixedPoint(distanceToCenter));

			if (distanceToCenter >= 0 && distanceToCenter <= toFixedPoint(1.0)) { // 65536 = 1.0
				long newDistanceToCenter = (distanceToCenter + (toFixedPoint(1.0) - 
					fixedSqrt(toFixedPoint(1.0) - multiplyFixedPoints(distanceToCenter, distanceToCenter)))) / 2; // 131072 = 2.0
				// printf("newDistanceToCenter: %f\n", fromFixedPoint(newDistanceToCenter));				

				if (newDistanceToCenter <= toFixedPoint(1.0)) {
					long angleTheta = arctan2(normalizedY, normalizedX);
					// printf("angleTheta: %f\n", fromFixedPoint(angleTheta));
					long newNormalizedX = multiplyFixedPoints(newDistanceToCenter, fixedCos(angleTheta));
					long newNormalizedY = multiplyFixedPoints(newDistanceToCenter, fixedSin(angleTheta));

					int x2 = (int) fromFixedPoint(multiplyFixedPoints( (newNormalizedX + toFixedPoint(1.0)), (width / 2)));
                    // printf("x2: %f\n",fromFixedPoint(x2));

                    // map from -1 ... 1 to image coordinates
                    int y2 = (int) fromFixedPoint(multiplyFixedPoints( (newNormalizedY + toFixedPoint(1.0)), (height / 2)));
                    // printf("y2: %f\n", fromFixedPoint(y2));
                    // find (x2,y2) position from source pixels
                    // int sourcePosition = (int) fromFixedPoint(multiplyFixedPoints(y2, width) + x2);
                    int sourcePosition = y2 * ((int) fromFixedPoint(width)) + x2; //, fromFixedPoint(multiplyFixedPoints(y2, width) + x2);
                    // printf("y2: %d width: %d x2: %d\n", y2, ((int) fromFixedPoint(width)), x2);            
                    // printf("%d\n", sourcePosition);
                    // make sure that position stays within arrays
                    if (sourcePosition >= 0 & sourcePosition < fromFixedPoint(multiplyFixedPoints(width, height))) {
                    	int destinationPosition = (int) fromFixedPoint(multiplyFixedPoints(currentY, width) + currentX);
                        // printf("%d == %d\n", sourcePosition, destinationPosition);
                        newData[destinationPosition] = data[sourcePosition];    
                    }
				}
				// printf("i: %d\n", i);
			}
			else {
				int currentPosition = fromFixedPoint(multiplyFixedPoints(currentY, width) + currentX);
				// currentPosition += toFixedPoint(400);
        	  	newData[currentPosition].red = 0;
    	      	newData[currentPosition].green = 0;
	          	newData[currentPosition].blue = 0;
            }
		}

		i++;
	}

	memcpy(data, newData, imageSize);

}