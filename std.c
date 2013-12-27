#include <stdio.h>
#include <math.h>
#include "std.h"

static long SQRT_LUT[4900];
static long COS_LUT[4900];
static long SIN_LUT[4900];

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
	for (int i = 0; i < 4900; i++) {
		float radicand = 0.0033 * i;
		SQRT_LUT[i] = toFixedPoint(sqrt(radicand));
		// printf("%ld\n", SQRT_LUT[i]);
	}

	// Generate atan2 LUT
	// for (int i = 0; i < 1200; i++) {
	// 	float radicand = 0.0033 * i;
	// 	SQRT_LUT[i] = toFixedPoint(atan2(radicand, i));
	// }

	// Generate cos LUT
	float radicand = -2;
	for (int i = 0; i < 1904; i++, radicand += 0.0033) {
		COS_LUT[i] = toFixedPoint(cos(radicand));
	}

	// Generate sin LUT
	radicand = -2;
	for (int i = 0; i < 1904; i++, radicand += 0.0033) {
		SIN_LUT[i] = toFixedPoint(sin(radicand));
	}
}

long arctan2(long y, long x) {
	// printf("\n");

	// printf("x: %f, y: %f\n", fromFixedPoint(x), fromFixedPoint(y));

	long fixedPi = toFixedPoint(3.14);
	long c1 = fixedPi/4;
	// printf("c1: %f\n", fromFixedPoint(c1));

	long angle;
	long r;
	long absY = y;

	if (x == 0 && y == 0) return 0;

	if (y < 0) {
		// printf("y is negative\n");
		absY = ~absY+1;
	}
	
	if (x >= 0) {
		// printf("x >= 0\n");

		r = divideFixedPoint(x - absY, x + absY);
		// printf("%f / %f = %f\n", fromFixedPoint(x - absY), fromFixedPoint(x + absY), fromFixedPoint(r));		

		angle = c1 - multiplyFixedPoints(c1, r);
	} else {
		// printf("x < 0\n");
		// printf("%f / %f = %f\n", fromFixedPoint(x - absY), fromFixedPoint(x + absY), fromFixedPoint(r));		
		
		r = divideFixedPoint(x + absY, absY - x);
		// printf("r: %f\n", fromFixedPoint(r));

		angle = (3 * c1) - multiplyFixedPoints(c1, r);
	}

	return y > 0 ? angle : -angle;
}

long fixedAbs(long number) {
	return number < 0 ? ~number+1 : number;
}

long fixedSqrt(long radicand) {
	long newRadicand = radicand / 216;
	return SQRT_LUT[newRadicand];
}

long fixedCos(long radicand) {
	long min = toFixedPoint(-2), step = toFixedPoint(0.0033);
	int index = (int) fromFixedPoint(fixedAbs(divideFixedPoint(min - radicand, step)));
	return COS_LUT[index];
}

long fixedSin(long radicand) {
	long min = toFixedPoint(-2), step = toFixedPoint(0.0033);
	int index = (int) fromFixedPoint(fixedAbs(divideFixedPoint(min - radicand, step)));
	return SIN_LUT[index];
}