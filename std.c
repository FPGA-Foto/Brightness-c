#include <stdio.h>
#include <math.h>
#include "std.h"

static long SQRT_LUT[9800];
static long COS_LUT[4900];
static long SIN_LUT[4900];

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

long powerFixedPoint(long a, int power) {
	long answer = a;
	for (int i = 1; i < power; i++) {
		answer = multiplyFixedPoints(answer, a);
	}
	return answer;
}

// 0.0033
void generateLUT() {
	// Generate sqrt LUT
	for (int i = 0; i < 9800; i++) {
		float radicand = 0.0016 * i;
		SQRT_LUT[i] = toFixedPoint(sqrt(radicand));
		// printf("%ld\n", SQRT_LUT[i]);
	}

	// Generate cos LUT
	float radicand = -4;
	for (int i = 0; i < 4900; i++, radicand += 0.0033) {
		COS_LUT[i] = toFixedPoint(cos(radicand));
	}

	// Generate sin LUT
	radicand = -4;
	for (int i = 0; i < 4900; i++, radicand += 0.0033) {
		SIN_LUT[i] = toFixedPoint(sin(radicand));
	}
}

long arctan2(long y, long x) {

	long fixedPi = toFixedPoint(PI);
	long fixedFactor = toFixedPoint(0.1963), fixedFactor2 = toFixedPoint(0.9817);
	long c1 = fixedPi/4;

	long angle;
	long r;
	long absY = y;


	if (y == 0 && x == 0) return 0;

	if (y < 0) {
		absY = ~absY+1;
	}
	
	if (x >= 0) {

		r = divideFixedPoint(x - absY, x + absY);
		// angle = c1 - multiplyFixedPoints(c1, r);
		angle = multiplyFixedPoints(fixedFactor, powerFixedPoint(r, 3)) - 
			multiplyFixedPoints(fixedFactor2, r) + c1;
	} else {
		
		r = divideFixedPoint(x + absY, absY - x);

		// angle = (3 * c1) - multiplyFixedPoints(c1, r);
		angle = multiplyFixedPoints(fixedFactor, powerFixedPoint(r, 3)) - 
			multiplyFixedPoints(fixedFactor2, r) + (3*c1);
	}

	return y >= 0 ? angle : -angle;
}

long fixedAbs(long number) {
	return number < 0 ? ~number+1 : number;
}

long fixedSqrt(long radicand) {
	long newRadicand = radicand / 105;
	return SQRT_LUT[newRadicand];
}

long fixedCos(long radicand) {
	long min = toFixedPoint(-4), step = toFixedPoint(0.0033);
	int index = (int) fromFixedPoint(fixedAbs(divideFixedPoint(min - radicand, step)));
	return COS_LUT[index];
}

long fixedSin(long radicand) {
	long min = toFixedPoint(-4), step = toFixedPoint(0.0033);
	int index = (int) fromFixedPoint(fixedAbs(divideFixedPoint(min - radicand, step)));
	return SIN_LUT[index];
}