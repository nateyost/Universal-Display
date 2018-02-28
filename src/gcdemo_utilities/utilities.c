#include "utilities.h"
#include "timer.h"




uint32_t convert_range(uint32_t value, uint32_t in_hi, uint32_t in_lo, uint32_t out_hi, uint32_t out_lo)
{
    float slope = (((float)out_hi - (float)out_lo)/ ((float)in_hi - (float)in_lo));
    float point = slope * ((float)value - (float)in_lo) + (float)out_lo;
    return (uint32_t)point;

}

