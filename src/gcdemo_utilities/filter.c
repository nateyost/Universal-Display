#include "filter.h"

double convert_range_double(double value, double in_hi, double in_lo, double out_hi, double out_lo)
{
    double slope = ((out_hi - out_lo)/ (in_hi - in_lo));
    double point = slope * (value - in_lo) + out_lo;
    return point;

}
filter *filterLowPass(void)
{
    filter *k = (filter *)malloc(sizeof(filter));

    k->a = (double *)calloc(2,sizeof(double));
    k->b = (double *)calloc(2,sizeof(double));
    k->x = (double *)calloc(2,sizeof(double));
    k->y = (double *)calloc(2,sizeof(double));
    uint8_t index;
    for(index = 0; index < 2; index++)
    {
        k->x[index] = 0.0;
        k->y[index] = 0.0;
    }
    return k;
}

filter *filterNewN(uint32_t n)
{
    filter *k = (filter *)malloc(sizeof(filter));
    k->n = n;
    k->a = (double *)calloc(n,sizeof(double));
    k->b = (double *)calloc(n,sizeof(double));
    k->x = (double *)calloc(n,sizeof(double));
    k->y = (double *)calloc(n,sizeof(double));
    uint8_t index;
    for(index = 0; index < n; index++)
    {
        k->x[index] = 0.0;
        k->y[index] = 0.0;
    } 
    return k;
}

void filter_free(filter *k)
{
    free(k->a);
    free(k->b);
    free(k->x);
    free(k->y);
    free(k);
}

void filterSetSinglePoleX(filter *k, double x)
{
    k->a[0] = 1 - x;
    k->b[1] = x;
}

double filterGetXDelay(uint32_t d)
{
    return exp((-1)/(double)d);
}

double filterGetFCDelay(double fc)
{
    return exp(-2*3.14159265*fc);
}

double convertHz2Norm(double fs, double f)
{
	double fNyq = fs/2.0;
	assert(f < fNyq);
	double norm = f/fNyq * 0.5;
	return norm;
}
void filterSetDelay(filter* k, uint32_t d)
{
    double x = filterGetXDelay(d);
    filterSetSinglePoleX(k,x);
}
void filterSetCutoff(filter* k, float fc)
{
	k->fc = fc;
	double norm = convertHz2Norm(k->fs,fc);
	filterSetSinglePoleX(k,filterGetFCDelay(norm));
}

// Place Holder
void filterSetDelayRC(filter* k, float timeRC)
{

}

wave* wave_new(void)
{
    wave *w = (wave *)malloc(sizeof(wave));
    w->sin_value = 0;
    w->step = 0;
    w->rad = 0;
    return w;
}

double filterGetResult(filter *k)
{
	return k->lastResult;
}
double filterSingle(filter *k, double x)
{

    k->x[0] = x;
    k->y[0] = k->a[0]*k->x[0] + k->a[1]*k->x[1] + k->b[1]*k->y[1];

    k->lastResult = k->y[1];

    k->y[1] = k->y[0]; 
    k->x[1] = k->x[0]; 
    return k->lastResult;
}

