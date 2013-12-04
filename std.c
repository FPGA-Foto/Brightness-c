#include "std.h"

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
	return a + fromFixedPoint(b);
}

long subFixedPoint(long a, int b) {
	return a - fromFixedPoint(b);
}


long multiplyFixedPoint(long a, int b) {
	return fromFixedPoint(a*b);
}

long divideFixedPoint(long a, long b) {
	return (a << SHIFT_AMOUNT) / b;
}

long magic_sqrt(long number) {
	long i;
	long f = toFixedPoint(1.5), 
		x = number/2, 
		y = number;
	

	i = y;

	i = toFixedPoint(0x5f3759df) - (i >> 1);
	y = i;

	y = y * f - x*y*y);
	 
	return number * y;
}

long Q_rsqrt( long number )
{
        long i;
        long x2, y;
        const long threehalfs = toFixedPoint(1.5F);
 
        x2 = number * toFixedPoint(0.5F);
        y  = number;
        i  = * ( long * ) &y;                       // evil floating point bit level hacking
        i  = toFixedPoint(0x5f3759df) - ( i >> 1 );               // what the fuck?
        y  = * ( long * ) &i;
        y  = y * ( threehalfs - ( multiplyFixedPoint(multiplyFixedPoint(x2, y), y) ) );   // 1st iteration
//      y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed
 
        return y;
}

long InvSqrt2(long x) {
   long xhalf = multiplyFixedPoint(toFixedPoint(0.5f), x);
   int i = *(int*) &x; // store floating-point bits in integer
   i = 0x5f3759d5 - (i >> 1); // initial guess for Newton's method
   x = *(float*)&i; // convert new bits into float
   x = x*(1.5f - xhalf*x*x); // One round of Newton's method
   return x;
}

float InvSqrt(float x){
   float xhalf = 0.5f * x;
   int i = *(int*)&x; // store floating-point bits in integer
   i = 0x5f3759d5 - (i >> 1); // initial guess for Newton's method
   x = *(float*)&i; // convert new bits into float
   x = x*(1.5f - xhalf*x*x); // One round of Newton's method
   return x;
}