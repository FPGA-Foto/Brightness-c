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

float * RGBtoHSL(int red_ptr, int green_ptr, int blue_ptr) { 
    float red, green, blue, hue, saturation, lightness; //this function works with floats between 0 and 1 
    red = ((float) red_ptr) / 255.0f; 
    green = ((float) green_ptr) / 255.0f; 
    blue = ((float) blue_ptr) / 255.0f;

    // printf("%f, %f, %f\n", red, green, blue);

    float maxColor = max(red, max(green, blue)); 
    float minColor = min(red, min(green, blue));

    //Red == Green == Blue, so it'saturation a shade of gray
    if (red == green && green == blue) {   
        hue = 0.0; //it doesn't matter what value it has       
        saturation = 0.0;       
        lightness = red; //doesn't matter if you pick red, green, or blue   
    }

    else {   
        lightness = (minColor + maxColor) / 2;     
        
        if (lightness < 0.5) saturation = (maxColor - minColor) / (maxColor + minColor);
        else saturation = (maxColor - minColor) / (2.0 - maxColor - minColor);
        
        if (red == maxColor) hue = (green - blue) / (maxColor - minColor);
        else if (green == maxColor) hue = 2.0 + (blue - red) / (maxColor - minColor);
        else hue = 4.0 + (red - green) / (maxColor - minColor);
        
        hue /= 6; //to bring it to a number between 0 and 1
        if (hue < 0) hue++;
    }

    float * hsl = malloc(3 * sizeof(float));
    hsl[0] = (hue);
    hsl[1] = (saturation);
    hsl[2] = (lightness);
    
    return hsl;
}

int * HSLtoRGB(float hue, float saturation, float lightness) {

    float red, green, blue; //this function works with floats between 0 and 1
    float temp1, temp2, tempRed, tempGreen, tempBlue;
    int * rgb = malloc(3 * sizeof(int));


    if (lightness < 0.5) temp2 = lightness * (1 + saturation);      
    else temp2 = (lightness + saturation) - (lightness * saturation);     
    
    temp1 = 2 * lightness - temp2;    
    tempRed = hue + 1.0 / 3.0;    
    
    if (tempRed > 1) tempRed--;
    
    tempGreen = hue;     
    tempBlue = hue - 1.0 / 3.0;
    
    if (tempBlue < 0) tempBlue++; 
    
    //Red     
    if (tempRed < 1.0 / 6.0) red = temp1 + (temp2 - temp1) * 6.0 * tempRed;      
    else if (tempRed < 0.5) red = temp2;   
    else if (tempRed < 2.0 / 3.0) red = temp1 + (temp2 - temp1) * ((2.0 / 3.0) - tempRed) * 6.0;
    else red = temp1; 
    
    //Green       
    if (tempGreen < 1.0 / 6.0) green = temp1 + (temp2 - temp1) * 6.0 * tempGreen;    
    else if (tempGreen < 0.5) green = temp2;
    else if (tempGreen < 2.0 / 3.0) green = temp1 + (temp2 - temp1) * ((2.0 / 3.0) - tempGreen) * 6.0;
    else green = temp1; 
    
    //Blue    
    if (tempBlue < 1.0 / 6.0) blue = temp1 + (temp2 - temp1) * 6.0 * tempBlue;   
    else if (tempBlue < 0.5) blue = temp2; 
    else if (tempBlue < 2.0 / 3.0) blue = temp1 + (temp2 - temp1) * ((2.0 / 3.0) - tempBlue) * 6.0;    
    else blue = temp1;

    rgb[0] = red < 1.0 ? roundf(red * 255.0) : 255;
    rgb[1] = green < 1.0 ? roundf(green * 255.0) : 255;
    rgb[2] = blue < 1.0 ? roundf(blue * 255.0) : 255;

    return rgb;
}


// Hue values: 0.0 - 2.0 
void setHue(unsigned char * data, float value, int imageSize) {
    // value /= 100;

    int i;
    for (i = 0; i < imageSize; i += 3) {

        // Convert pixel to HSL
        float * hsl = RGBtoHSL(data[i], data[i+1], data[i+2]);

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
        float * hsl = RGBtoHSL(data[i], data[i+1], data[i+2]);

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
        float * hsl = RGBtoHSL(data[i], data[i+1], data[i+2]);

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