#ifndef STD_H
#define STD_H

#include <stdio.h>

#define SHIFT_AMOUNT 16 // 2^16 = 65536
#define SHIFT_MASK ((1 << SHIFT_AMOUNT) - 1)
#define PI 3.14
// #define MULTIPLY_FP_RESOLUTION_BITS	15

// #define toFixedPoint(number) ((long) ( (((long) number) << SHIFT_AMOUNT) + (number - (long) number) * (1 << SHIFT_AMOUNT)))  

long toFixedPoint(float number);

// float fromFixedPoint(long number);

float fromFixedPoint(long number);

long newFixedPoint(float n);

long addFixedPoint(long a, int b);

long subFixedPoint(long a, int b);

long multiplyWithFixedPoint(long a, int b);

long multiplyFixedPoints(long a, long b);

long divideFixedPoint(long a, long b);

// 0.0033
void generateLUT();


long arctan2(long y, long x);

long atan2_fp(long y_fp, long x_fp);

long fixedSqrt(long radicand);

long fixedAtan2(long radicand);

long fixedCos(long radicand);

long fixedSin(long radicand);

#endif