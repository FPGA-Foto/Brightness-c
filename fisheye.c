#include "fisheye.h"

void setFishEye(unsigned char * data, double value, int imageSize, int width, int height) {

	int currentY;
	for (currentY = 0; currentY < height; currentY++) {

		// Distance in height between 0 and 1
		double normalizedY = ((2.0 * currentY) / height) - 1.0;
		double normalizedY2 = normalizedY*normalizedY;

		int currentX;
		for (currentX = 0; currentX < width; currentX += 3) {
			// Distance in width between 0 and 1
			double normalizedX = ((2.0 * currentX) / width) - 1.0;
			double normalizedX2 = normalizedX*normalizedX;
			
			double distanceToCenter = sqrt(normalizedX2+normalizedY2);

			if (distanceToCenter >= 0.0 && distanceToCenter <= 1.0) {
				double newDistanceToCenter = sqrt(1.0 - distanceToCenter*distanceToCenter);
				newDistanceToCenter = (distanceToCenter + (1.0 - newDistanceToCenter)) / 2.0;

				if (newDistanceToCenter <= 1.0) {
					double angleTheta = atan2(normalizedY, normalizedX);
					double newNormalizedX = newDistanceToCenter * cos(angleTheta);
					double newNormalizedY = newDistanceToCenter * sin(angleTheta);

					int x2 = (int) (((newNormalizedX + 1) * (width)) / 2.0);
                    // map from -1 ... 1 to image coordinates
                    int y2 = (int) (((newNormalizedY + 1) * height) / 2.0);
                    // find (x2,y2) position from source pixels
                    int originalPosition = (int) (y2 * (width) + x2);            
                    // make sure that position stays within arrays
                    if (originalPosition >= 0 & originalPosition < (width)*height) {
                    	int newPosition = (int) (currentY * (width) + currentX);
                        // get new pixel (x2,y2) and put it to target array at (x,y)
                        // printf("[%d, %d] [%d, %d]\n", currentX, currentY, x2, y2);
                        data[newPosition] = data[originalPosition*3];    
                        data[newPosition+1] = data[originalPosition*3+1];    
                        data[newPosition+2] = data[originalPosition*3+2];    
                    }
				}
			}

		}
	}

	// for (int y=0; y<h; y++) {                                
 //        // normalize y coordinate to -1 ... 1
 //        double ny = ((2*y)/h)-1;                        
 //        // pre calculate ny*ny
 //        double ny2 = ny*ny;                                
 //        // for each column
 //        for (int x=0;x<w;x++) {                            
 //            // normalize x coordinate to -1 ... 1
 //            double nx = ((2*x)/w)-1;                    
 //            // pre calculate nx*nx
 //            double nx2 = nx*nx;
 //            // calculate distance from center (0,0)
 //            // this will include circle or ellipse shape portion
 //            // of the image, depending on image dimensions
 //            // you can experiment with images with different dimensions
 //            double r = Math.sqrt(nx2+ny2);                
 //            // discard pixels outside from circle!
 //            if (0.0<=r&&r<=1.0) {                            
 //                double nr = Math.sqrt(1.0-r*r);            
 //                // new distance is between 0 ... 1
 //                nr = (r + (1.0-nr)) / 2.0;
 //                // discard radius greater than 1.0
 //                if (nr<=1.0) {
 //                    // calculate the angle for polar coordinates
 //                    double theta = Math.atan2(ny,nx);         
 //                    // calculate new x position with new distance in same angle
 //                    double nxn = nr*Math.cos(theta);        
 //                    // calculate new y position with new distance in same angle
 //                    double nyn = nr*Math.sin(theta);        
 //                    // map from -1 ... 1 to image coordinates
 //                    int x2 = (int)(((nxn+1)*w)/2.0);        
 //                    // map from -1 ... 1 to image coordinates
 //                    int y2 = (int)(((nyn+1)*h)/2.0);        
 //                    // find (x2,y2) position from source pixels
 //                    int srcpos = (int)(y2*w+x2);            
 //                    // make sure that position stays within arrays
 //                    if (srcpos>=0 & srcpos < w*h) {
 //                        // get new pixel (x2,y2) and put it to target array at (x,y)
 //                        dstpixels[(int)(y*w+x)] = srcpixels[srcpos];    
 //                    }
 //                }
 //            }
 //        }
 //    }
}