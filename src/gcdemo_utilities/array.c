#ifndef ARRAY_H
#define ARRAY_H

#include "array.h"

uint16_t arraySafeIndex(int32_t n, int32_t v_zero, int32_t v_offset)
{
    if(v_offset == n)
        return v_zero;
    if(v_zero+v_offset >= n)
        return n%(v_offset+v_zero);
    if(v_offset == 0)
        return v_zero;
    if(v_offset + v_zero == 0)
        return 0;
    if(v_offset < 0)
    {
        do {
        v_offset += n;
        } while (v_offset < 0);
    }
    if(v_offset+v_zero < n)
        return v_offset+v_zero;
    
    return 0x00;
}
#endif //MATH_H
