#include "pipeline.h"

#define adcDisable() ADCON1bits.ADON = 0;
#define adcEnable()  ADCON1bits.ADON = 1;

#define adcConfigBegin() adcDisable(); PMD1bits.ADC1MD = 0; 
#define adcConfigEnd()   adcEnable();

// Global ADC Variable to contain all ADC configuration data;
ADConverter ADC;

void map(ADConverter *adc)
{
    ADCON3 = 0x0000;
    ADCON2 = 0x0300;
    ADCON1 = 0x0000;

/* ADCON1 */
    ADCON1bits.PUMPEN = adc->control.pumpen;
    ADCON1bits.FORM = adc->control.form;
    ADCON1bits.ADSLP = adc->control.adslp;
    ADCON1bits.ADSIDL = adc->control.adsidl;
    ADCON1bits.PWRLVL = adc->control.pwrlvl;

/* ADCON2 */
    ADCON2bits.REFPUMP = adc->control.refpump;
    ADCON2bits.BUFORG = adc->control.buforg;
    ADCON2bits.NVCFG0 = adc->control.nvcfg0;
    ADCON2bits.PVCFG = adc->control.pvcfg;

/* ADCON3 */
    ADCON3bits.ADCS = adc->control.adcs;
    ADCON3bits.ADRC = adc->control.adrc;

}

void adcConfigure(ADConverter *adc)
{
    if(adc->initialized == 0) {

        // Replace the current ADC settings with our own
        memcpy(&ADC, adc, sizeof(ADC));

        adc->initialized = 1;
        // Map static settings
        map(adc);

        adcConfigBegin();

        PMD1bits.ADC1MD = 0;

        ADCON1bits.ADON = 0;

        ADCON1bits.ADON = 1;
        while(ADSTATHbits.ADREADY == 0);

        ADCON1bits.ADCAL = 1;  
        while(ADSTATHbits.ADREADY == 0);

        adcConfigEnd();
    }
    else
    {
    }
}


uint32_t conversionClockFrequency_get(uint32_t frq_source)
{
    return (uint32_t)(1.0 / (2.0 * (1.0/(float)frq_source) * (float)ADCON3bits.ADCS));
}

uint32_t conversionClockFrequency_set(uint32_t frq_source, uint32_t frq_ad)
{
    uint16_t adcs = frq_source / ( 2 * frq_ad );
    if(adcs > 0x21)
    {
        return 0xDEAD;// Error
    }
    else
    {
        ADCON3bits.ADCS = adcs;
        return conversionClockFrequency_get(frq_source);
    }
}

uint32_t sampleHoldChargeTime_set(uint8_t sample_list, uint32_t aq_frq, uint32_t ad_frq)
{
    uint16_t samc = (uint16_t) ( (float)ad_frq/(float)aq_frq );
    switch(sample_list)
    {
        case 0: ADL0CONHbits.SAMC = samc; break;
        case 1: ADL1CONHbits.SAMC = samc; break;
        case 2: ADL2CONHbits.SAMC = samc; break;
        case 3: ADL3CONHbits.SAMC = samc; break;
        default:
                break;
    }
    return 0;
}

uint16_t adcSingleChannel(uint8_t channel, uint8_t sample_period)
{

    if(ADC.initialized == 0)
    {
        adcConfigure(&ADC);
    }

    unsigned int Remainder;
    unsigned int Result;
    unsigned int sample_count = 16;

    ACRES = 0;
    ADL0PTR = 0;
    
    ADTBL0 = channel;
    
    ADL0CONH = 0x0000 | (sample_period & 0x1F);
    ADL0CONL = 0x00;

    ADL0CONLbits.SLTSRC = 0x01;
    ADL0CONLbits.SLEN = 1;  //Sample list enabled!  Accumulation will commence when ACEN is also set.

    ACCONLbits.TBLSEL = 0;
    ACCONLbits.COUNT = sample_count;    

    ACCONHbits.ACEN = 1;

    while(ACCONHbits.ACEN == 1);

    Result = __builtin_divmodud((unsigned long int)ACRES, (unsigned int)sample_count, &Remainder);  

    if(Remainder >= (sample_count >> 1))
    {
        return ++Result;
    }    
    
    return Result;
}

