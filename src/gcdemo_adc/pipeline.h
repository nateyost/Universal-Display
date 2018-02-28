#ifndef PIPELINE_H
#define PIPELINE_H

#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <xc.h>

#define SAMPLE_LIST_COUNT 4

#define ADC_OK   0
#define ADC_FAIL 1

typedef struct Control_
{
      unsigned pwrlvl:1;
      unsigned adcal:1;
      unsigned pumpen:1;
      unsigned form:4;
      unsigned adslp:1;
      unsigned adsidl:1;
      unsigned adon:1;

      unsigned adhalt:1;
      unsigned refpump:1;
      unsigned bufint:2;
      unsigned adpwr:2;
      unsigned buforg:1;
      unsigned nvcfg0:1;
      unsigned pvcfg:2;

      unsigned adcs:8;
      unsigned adrc:1;
} Control;

typedef struct Status_ 
{
    unsigned SL0IF:1;
    unsigned SL1IF:1;
    unsigned SL2IF:1;
    unsigned SL3IF:1;
    unsigned ACCIF:1;
    unsigned BUFIF:1;
    unsigned SLERR:4;
    unsigned ADBUSY:1;
    unsigned ADREADY:1;
    unsigned PUMPST:1;

} Status;

typedef struct Accumulator_
{
    unsigned ACIE:1;
    unsigned ACEN:1;
    unsigned COUNT:8;
    unsigned TBLSEL:6;
} Accumulator;

typedef struct MatchHit_ 
{
  unsigned CHH0:1;
  unsigned CHH1:1;
  unsigned CHH2:1;
  unsigned CHH3:1;
  unsigned CHH4:1;
  unsigned CHH5:1;
  unsigned CHH6:1;
  unsigned CHH7:1;
  unsigned CHH8:1;
  unsigned CHH9:1;
  unsigned CHH10:1;
  unsigned CHH11:1;
  unsigned CHH12:1;
  unsigned CHH13:1;
  unsigned CHH14:1;
  unsigned CHH15:1;
  unsigned CHH16:1;
  unsigned CHH17:1;
  unsigned CHH18:1;
  unsigned CHH19:1;
  unsigned CHH20:1;
  unsigned CHH21:1;
  unsigned CHH22:1;
  unsigned CHH23:1;
  unsigned CHH24:1;
  unsigned CHH25:1;
  unsigned CHH26:1;
  unsigned CHH27:1;
  unsigned CHH28:1;
  unsigned CHH29:1;
  unsigned CHH30:1;
  unsigned CHH31:1;
} MatchHit;

typedef struct SampleList_ 
{
      unsigned slsize:6;
      unsigned adch:7;
      unsigned thsrc:1;
      unsigned sltsrc:5;
      unsigned slenclr:1;
      unsigned samp:1;
      unsigned slen:1;
      unsigned diff:1;
      unsigned uctmu:1;
      unsigned samc:5;
      unsigned mulchen:1;
      unsigned pintris:1;
      unsigned ctmen:1;
      unsigned slint:2;
      unsigned asen:1;
      unsigned adnext:7;
      unsigned adlif:1;
      unsigned adtdly:1;
      unsigned lbusy:1;
      unsigned adtact:1;
} SampleList;

typedef struct ADConverter_
{
    struct {
       unsigned initialized:1;
    };
    Control control;
    Status status;
    Accumulator accumulator;
    MatchHit match_hit;
    SampleList sample_list[SAMPLE_LIST_COUNT];

}ADConverter;


void adcConfigure(ADConverter *adc);
uint16_t adcSingleChannel(uint8_t channel, uint8_t sample_period);
uint32_t conversionClockFrequency_get(uint32_t frq_source);
uint32_t conversionClockFrequency_set(uint32_t frq_source, uint32_t frq_ad);

#endif //PIPELINE_H
