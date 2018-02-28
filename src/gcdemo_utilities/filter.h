#ifndef FILTER_H
#define FILTER_H

#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

typedef struct filter_tag
{
    uint32_t n;
    double *a;
    double *b;
    double *x;
    double *y;
    double lastResult;
    double f;
    double gain;
    double fs;
    double fc;
}filter;

typedef struct wave_tag
{
    double f;
    double fs;
    double phase;
    uint32_t cycles;

    double sin_value;
    double step;
    double rad;
}wave;
double filterGetResult(filter *k);
filter *filterLowPass(void);
void filterSetDelay(filter* k, uint32_t d);
void filterSetCutoff(filter* k, float fc);
double convertHz2Norm(double fs, double f);
double convert_range_double(double value, double in_hi, double in_lo, double out_hi, double out_lo);
filter *filterNewN(uint32_t n);
void filter_free(filter *k);
void filterSetSinglePoleX(filter *k, double x);
double filterGetXDelay(uint32_t d);
double filterGetFCDelay(double fc);
double filter_kernel_fc(filter *k, double fc, double fs);
void filterSetDelay(filter* k, uint32_t d);
wave* wave_new(void);
void filter_inv_dft(FILE* fp, filter *k);
double filterSingle(filter *k, double x);


#endif //FILTER_H
