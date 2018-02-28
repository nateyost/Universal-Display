#include "sigma_delta_adc.h"

uint16_t sigma_delta_reading(uint8_t channel)
{
    static uint8_t init = 0;
        
    if(init == 0)
    {
//
// execute once at startup
//
        init = 1;

        SD1CON1bits.SDON = 0;       // must disable converter to write to SFR bits

        SD1CON1bits.PWRLVL = 1;     // 2x bandwidth on input amplifier
        SD1CON1bits.SDGAIN = 0;     // 1X gain
        SD1CON1bits.VOSCAL = 0;     // do not sample offset error

        SD1CON1bits.SDREFN = 0;     // - reference is SVss pin
        SD1CON1bits.SDREFP = 0;     // + reference is SVdd pin

        SD1CON2bits.CHOP = 3;       // enable chopping
        SD1CON2bits.SDINT = 3;      // interrupt on every sample clock
        SD1CON2bits.RNDRES = 2;     // round result to 16 bits

        SD1CON3bits.SDOSR = 0;      // OSR is 1024 for best quality
        
        SD1CON1bits.SDON = 1;       // enable converter
    }

    //while(!SD1CON2bits.SDRDY){}

    IFS6bits.SDA1IF = 0;
    while (IFS6bits.SDA1IF == 0);

    return SD1RESH;
}
