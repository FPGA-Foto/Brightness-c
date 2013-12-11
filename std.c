#include <stdio.h>
#include <math.h>

static long SQRT_LUT[1200];
static long ATAN2_LUT[1200];
static long COS_LUT[1200];
static long SIN_LUT[1200];

#define SHIFT_AMOUNT 16 // 2^16 = 65536
#define SHIFT_MASK ((1 << SHIFT_AMOUNT) - 1)
#define MULTIPLY_FP_RESOLUTION_BITS	15

// #define toFixedPoint(number) (long) ( (((long) number) << SHIFT_AMOUNT) + (number - (long) number) * (1 << SHIFT_AMOUNT));  

long toFixedPoint(float number) {
	// Convert integer part
	long fixedPointNumber = ((long) number) << SHIFT_AMOUNT;

	// Convert fractional part
	fixedPointNumber += (number - (long) number) * (1 << SHIFT_AMOUNT);

	return fixedPointNumber;
}

float fromFixedPoint(long number) {
	// Convert integer part
	float floatNumber = (number >> SHIFT_AMOUNT);
	
	// Convert fractional part	
	floatNumber += ((float) (number & SHIFT_MASK) / (1 << SHIFT_AMOUNT));
	
	return floatNumber;
}

long newFixedPoint(float n) {
	return toFixedPoint(n);
}

long addFixedPoint(long a, int b) {
	// printf("a: %ld, b: %ld = %ld\n", a, toFixedPoint(b), a + toFixedPoint(b));
	return a + toFixedPoint(b);
}

long subFixedPoint(long a, int b) {
	return a - toFixedPoint(b);
}


long multiplyWithFixedPoint(long a, int b) {
	return fromFixedPoint(a*b);
}

long multiplyFixedPoints(long a, long b) {
	return fromFixedPoint(a*b);
}

long divideFixedPoint(long a, long b) {
	return (a << SHIFT_AMOUNT) / b;
}

// 0.0033
void generateLUT() {
	// Generate sqrt LUT
	for (int i = 0; i < 1200; i++) {
		float radicand = 0.0033 * i;
		SQRT_LUT[i] = toFixedPoint(sqrt(radicand));
	}

	// Generate atan2 LUT
	for (int i = 0; i < 1200; i++) {
		float radicand = 0.0033 * i;
		SQRT_LUT[i] = toFixedPoint(atan2(radicand, i));
	}

	// Generate cos LUT
	for (int i = 0; i < 1200; i++) {
		float radicand = 0.0033 * i;
		COS_LUT[i] = toFixedPoint(cos(radicand));
	}

	// Generate sin LUT
	for (int i = 0; i < 1200; i++) {
		float radicand = 0.0033 * i;
		SIN_LUT[i] = toFixedPoint(sin(radicand));
	}
}

long atan2_fp(long y_fp, long x_fp) {
	long coeff_1 = 45;
	long coeff_1b = -56;	// 56.24;
	long coeff_1c = 11;	// 11.25
	long coeff_2 = 135;

	long angle = 0;

	long r;
	long r3;

	long y_abs_fp = y_fp;
	if (y_abs_fp < 0)
		y_abs_fp = -y_abs_fp;

	if (y_fp == 0) {
		
		if (x_fp >= 0) {
			angle = 0;
		} else {
			angle = 180;
		}
	} else if (x_fp >= 0) {

		r = (((long)(x_fp - y_abs_fp)) << MULTIPLY_FP_RESOLUTION_BITS) / ((long)(x_fp + y_abs_fp));
		r3 = r * r;
		r3 =  r3 >> MULTIPLY_FP_RESOLUTION_BITS;
		r3 *= r;
		r3 =  r3 >> MULTIPLY_FP_RESOLUTION_BITS;
		r3 *= coeff_1c;
		angle = (long) (coeff_1 + ((coeff_1b * r + r3) >> MULTIPLY_FP_RESOLUTION_BITS));
	} else {
		r = (((long)(x_fp + y_abs_fp)) << MULTIPLY_FP_RESOLUTION_BITS) / ((long)(y_abs_fp - x_fp));
		r3 = r * r;
		r3 =  r3 >> MULTIPLY_FP_RESOLUTION_BITS;
		r3 *= r;
		r3 =  r3 >> MULTIPLY_FP_RESOLUTION_BITS;
		r3 *= coeff_1c;
		angle = coeff_2 + ((long) (((coeff_1b * r + r3) >> MULTIPLY_FP_RESOLUTION_BITS)));
	}

	if (y_fp < 0)
		return (-angle);     // negate if in quad III or IV
	else
		return (angle);
}

long fixedSqrt(long radicand) {
	radicand /= 216;
	return SQRT_LUT[radicand];
}

long fixedAtan2(long radicand) {
	radicand /= 216;
	return ATAN2_LUT[radicand];
}

long fixedCos(long radicand) {
	radicand /= 216;
	return COS_LUT[radicand];
}

long fixedSin(long radicand) {
	radicand /= 216;
	return SIN_LUT[radicand];
}