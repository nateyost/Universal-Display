#ifndef J_MATH_H
#define J_MATH_H

#ifdef __cpluslus
	extern "C" {
#endif // __cplusplus

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
typedef struct vector_u_tag
{
    int32_t offset;
    int32_t zero;
    uint32_t n;
    double *p_double;
}vector_u;

uint16_t arraySafeIndex(int32_t n, int32_t v_zero, int32_t v_offset);

#ifdef __cplusplus
	}
#endif // __cplusplus

#endif //MATH_VIRTUAL_INDEX_H

