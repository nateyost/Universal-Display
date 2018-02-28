/* 
 * File:   utilities.h
 * Author: C14648
 *
 * Created on February 19, 2013, 1:36 PM
 */

#ifndef UTILITIES_H
#define	UTILITIES_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <xc.h>
#include <stdint.h>

void dac_set_trigger_frequency(uint32_t Fosc, uint32_t f);

void dac_set_trigger_frequency_range(uint32_t Fosc, uint32_t f, uint32_t range_lo, uint32_t range_hi);

uint32_t convert_range(uint32_t value, uint32_t in_hi, uint32_t in_lo, uint32_t out_hi, uint32_t out_lo);



#ifdef	__cplusplus
}
#endif

#endif	/* UTILITIES_H */

