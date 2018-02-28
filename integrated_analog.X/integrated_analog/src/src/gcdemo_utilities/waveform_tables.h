#ifndef WAVEFORM_TABLES_H
#define WAVEFORM_TABLES_H

#ifdef __cpluslus
	extern "C" {
#endif // __cplusplus

#include <stdint.h>
extern uint16_t wavetable_sin[];
extern uint16_t wavetable_square[];
extern uint16_t wavetable_tri[];
extern uint16_t wavetable_ramp[];

#ifdef __cplusplus
	}
#endif // __cplusplus

#endif //WAVEFORM_TABLES_H

