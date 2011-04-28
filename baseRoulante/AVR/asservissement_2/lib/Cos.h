
#ifndef _FP_MATH_H_
#define _FP_MATH_H_

#define FP_PI 205887

#include <stdint.h>

int32_t fp_cos(int32_t theta);
int32_t fp_sin(int32_t theta);

int32_t fp_mult(int32_t a, int32_t b);
int32_t fp_div(int32_t a, int32_t b);

#endif
