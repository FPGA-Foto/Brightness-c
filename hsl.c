#include "hsl.h"

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

long * RGBtoHSL(int originalRed, int originalGreen, int originalBlue) { 
    long red, green, blue, hue, saturation, lightness; //this function works with floats between 0 and 1 
    red = toFixedPoint(originalRed) / 255;
    green = toFixedPoint(originalGreen) / 255;
    blue = toFixedPoint(originalBlue) / 255;

    // printf("%f, %f, %f\n", red, green, blue);

    long maxColor = max(red, max(green, blue)); 
    long minColor = min(red, min(green, blue));

    //Red == Green == Blue, so it's a shade of gray
    if (red == green && green == blue) {   
        hue = 0; //it doesn't matter what value it has       
        saturation = 0;       
        lightness = red; //doesn't matter if you pick red, green, or blue   
    }

    else {   
        lightness = (minColor + maxColor) / 2;     
        
        // printf("maxColor: %ld, minColor: %ld\n", maxColor, minColor);

        if (lightness < toFixedPoint(0.5)) saturation = divideFixedPoint(maxColor - minColor, maxColor + minColor);
        else saturation = divideFixedPoint(maxColor - minColor, toFixedPoint(2.0) - maxColor - minColor);
        
        if (red == maxColor) hue = divideFixedPoint(green - blue, maxColor - minColor);
        else if (green == maxColor) hue = toFixedPoint(2.0) + divideFixedPoint(blue - red, maxColor - minColor);
        else hue = toFixedPoint(4.0) + divideFixedPoint(red - green, maxColor - minColor);
        
        hue /= 6; //to bring it to a number between 0 and 1
        // printf("%f\n", fromFixedPoint(hue));
        if (hue < 0)  hue = hue + toFixedPoint(1.0);

    }

    long * hsl = malloc(3 * sizeof(long));
    hsl[0] = hue;
    hsl[1] = saturation;
    hsl[2] = lightness;
    
    return hsl;
}

int * HSLtoRGB(long hue, long saturation, long lightness) {

    long red, green, blue; //this function works with floats between 0 and 1
    long temp1, temp2, tempRed, tempGreen, tempBlue;
    int * rgb = malloc(3 * sizeof(int));


    if (lightness < toFixedPoint(0.5)) temp2 = multiplyFixedPoints(lightness, toFixedPoint(1.0) + saturation);      
    else temp2 = (lightness + saturation) - multiplyFixedPoints(lightness, saturation);     
    
    temp1 = 2 * lightness - temp2;
    tempRed = hue + toFixedPoint(1.0) / 3;
    
    if (tempRed > toFixedPoint(1.0)) tempRed = tempRed - toFixedPoint(1.0);
    
    tempGreen = hue;
    tempBlue = hue - toFixedPoint(1.0) / 3.0;
    
    if (tempBlue < 0) tempBlue = tempBlue + toFixedPoint(1.0); 
    
    //Red     
    if (tempRed < toFixedPoint(1.0) / 6) red = temp1 + multiplyFixedPoints((temp2 - temp1) * 6, tempRed);      
    else if (tempRed < toFixedPoint(0.5)) red = temp2;
    else if (tempRed < toFixedPoint(2.0) / 3) red = temp1 + multiplyFixedPoints(temp2 - temp1, toFixedPoint(2.0) / 3 - tempRed) * 6;
    else red = temp1;
    
    //Green       
    if (tempGreen < toFixedPoint(1.0) / 6) green = temp1 + multiplyFixedPoints((temp2 - temp1) * 6, tempGreen);    
    else if (tempGreen < toFixedPoint(0.5)) green = temp2;
    else if (tempGreen < toFixedPoint(2.0) / 3) green = temp1 + multiplyFixedPoints(temp2 - temp1, toFixedPoint(2.0) / 3 - tempGreen) * 6;
    else green = temp1;
    
    //Blue    
    if (tempBlue < toFixedPoint(1.0) / 6) blue = temp1 + multiplyFixedPoints((temp2 - temp1) * 6, tempBlue);   
    else if (tempBlue < toFixedPoint(0.5)) blue = temp2;
    else if (tempBlue < toFixedPoint(2.0) / 3) blue = temp1 + multiplyFixedPoints(temp2 - temp1, toFixedPoint(2.0) / 3 - tempBlue) * 6;    
    else blue = temp1;

    rgb[0] = red < toFixedPoint(1.0) ? fromFixedPoint(red * 255) : 255;
    rgb[1] = green < toFixedPoint(1.0) ? fromFixedPoint(green * 255) : 255;
    rgb[2] = blue < toFixedPoint(1.0) ? fromFixedPoint(blue * 255) : 255;

    return rgb;
}


// Hue values: 0.0 - 2.0 
// void setHue(Pixel * data, float value, int imageSize) {
//     // value /= 100;

//     int i;
//     for (i = 0; i < imageSize; i += 1) {

//         printf("%d, %d, %d\n", data[i].red, data[i].green, data[i].blue);

//         // Convert pixel to HSL
//         // long * hsl = RGBtoHSL(data[i].red, data[i].green, data[i].blue);

//         // // Apply value
//         // // printf("%f\n", hsl[0]);
//         // // printf("%f\n", fromFixedPoint(hsl[0]));
//         // // hsl[0] *= value;
//         // // printf("%f\n", fromFixedPoint(hsl[0]));

//         // // Convert back to RGB
//         // int * rgb = HSLtoRGB(hsl[0], hsl[1], hsl[2]);

//         // // printf("first: %d\n", data[i].red);
//         // data[i].red = rgb[0];
//         // data[i].green = rgb[1];
//         // data[i].blue = rgb[2];
//         // // printf("after: %d\n", data->red);

//         // free(rgb);
//     }
// }

void setHue(unsigned char * data, float value, int imageSize) {
    // value /= 100;

    int i;
    for (i = 0; i < imageSize; i += 3) {
        // printf("%d, %d, %d\n", data[i], data[i+1], data[i+2]);

        // Convert pixel to HSL
        long * hsl = RGBtoHSL(data[i], data[i+1], data[i+2]);

        // Apply value
        // printf("%f\n", hsl[0]);
        hsl[0] *= value;

        // Convert back to RGB
        int * rgb = HSLtoRGB(hsl[0], hsl[1], hsl[2]);

        data[i] = rgb[0];
        data[i+1] = rgb[1];
        data[i+2] = rgb[2];

        free(rgb);
    }
}

// Saturation values: 0.0 - 2.0
void setSaturation(unsigned char * data, float value, int imageSize) {
    // value /= 100;

    int i;
    for (i = 0; i < imageSize; i += 3) {

        // Convert pixel to HSL
        long * hsl = RGBtoHSL(data[i], data[i+1], data[i+2]);

        // Apply value
        if (hsl[1] * value > 1.0) {
            hsl[1] = 1.0;
        } else if (hsl[1] * value < 0.0) {
            hsl[1] = 0.0;
        }
        else {
            hsl[1] *= value;
        }

        // Convert back to RGB
        int * rgb = HSLtoRGB(hsl[0], hsl[1], hsl[2]);

        data[i] = rgb[0];
        data[i+1] = rgb[1];
        data[i+2] = rgb[2];

        free(rgb);
    }
}

// Lightness values: 0.0 - 2.0
void setLightness(unsigned char * data, float value, int imageSize) {
    int i;
    for (i = 0; i < imageSize; i += 3) {

        // Convert pixel to HSL
        long * hsl = RGBtoHSL(data[i], data[i+1], data[i+2]);

        // Apply value
        hsl[2] *= value;

        // Convert back to RGB
        int * rgb = HSLtoRGB(hsl[0], hsl[1], hsl[2]);

        data[i] = rgb[0];
        data[i+1] = rgb[1];
        data[i+2] = rgb[2];

        free(rgb);
    }
}