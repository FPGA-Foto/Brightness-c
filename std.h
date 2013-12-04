#ifndef STD_H
#define STD_H

#include <stdio.h>

#define SHIFT_AMOUNT 16 // 2^16 = 65536
#define SHIFT_MASK ((1 << SHIFT_AMOUNT) - 1)

long convertToFixedPoint(float number);

float convertFromFixedPoint(long number);

long sumFixedPoint(long a, int b);

long multiplyFixedPoint(long a, int b);


long magic_sqrt(long number);

#endif