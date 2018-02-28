#ifndef SIGMA_DELTA_ADC
#define SIGMA_DELTA_ADC
#include "main.h"

#ifdef __cpluslus
	extern "C" {
#endif // __cplusplus

#define    CH0 0
#define    CH1 1

uint16_t sigma_delta_reading(uint8_t channel);

#ifdef __cplusplus
	}
#endif // __cplusplus

#endif //SIGMA_DELTA_ADC
