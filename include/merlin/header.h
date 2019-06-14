#ifndef _HEADER_H
#define _HEADER_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "math.h"
#define SQR(x) (x) * (x)
#define CUB(x) (x) * (x) * (x)

#define data_type float
#define cmost_data_type_FLOAT 1

// #define IMG_P 8
// #define IMG_N 8
// #define IMG_M 8
// #define TS_P 4
// #define TS_N 4
// #define TS_M 4
//
// #define PITCH_P (IMG_P+2)
// #define PITCH_N (IMG_N+2)
// #define PITCH_M (IMG_M+2)
//
// #define IMG_SIZE (PITCH_M*PITCH_N*PITCH_P)

const int IMG_P = 8;
const int IMG_N = 8;
const int IMG_M = 8;
const int TS_P = 4;
const int TS_N = 4;
const int TS_M = 4;

#define PITCH_P = (IMG_P + 2)
#define PITCH_N = (IMG_N + 2)
#define PITCH_M = (IMG_M + 2)

#define IMG_SIZE = (PITCH_M * PITCH_N * PITCH_P)

union cmost_int_float_type {
  int i;
  float f;
};

#endif
