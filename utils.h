#ifndef _UTIL_H_
#define _UTIL_H_
#include "sys/time.h"

void randomize_matrix(float *A, int m, int n);
double get_sec();
void print_matrix(const float *A, int m, int n);
void print_vector(float *vec, int n);
void copy_matrix(float *src, float *dest, int n);
void verify_matrix(float *mat1, float *mat2, int n);

#endif // _UTIL_H_