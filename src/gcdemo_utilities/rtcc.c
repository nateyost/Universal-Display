#include "rtcc.h"
#ifdef __XC16__
#include <xc.h>
#endif
#define    MINUTES_SECONDS 0
#define    WEEKDAY_HOURS 1
#define    MONTH_DAY  2
#define    YEAR  3

#ifdef __XC16__
rtcc flash_clock __attribute__((persistent));
#else
rtcc flash_clock;
#endif

void rtcc_set(rtcc *r)
{
    if(r->hour_tens > 2) {
        r->hour_tens = 0;
    }
    if(r->hour_ones > 9) {
        r->hour_ones = 0;
    }
    if(r->min_tens > 5) {
        r->min_tens = 0; 
    }
    if(r->min_ones > 9) {
        r->min_ones = 0; 
    }
    if(r->hour > 0x24) {
        r->hour = 0x24;
    }
    if(r->min > 0x59) {
        r->min = 0x59; 
    }


#ifdef __XC16__
    RCFGCALbits.RTCPTR = 3;
    RTCVAL = r->year;
    RTCVAL = r->month_day;
    RTCVAL = r->weekday_hour;
    RTCVAL = r->min_sec;
    flash_clock.year         = r->year;         
    flash_clock.month_day    = r->month_day;
    flash_clock.weekday_hour = r->weekday_hour;
    flash_clock.min_sec      = r->min_sec;
#endif // __XC16__
}
void rtcc_read(rtcc *r)
{   
#ifdef __XC16__
    //if(RCFGCALbits.RTCSYNC == 0) // Add this later
    r->year = RTCVAL;
    RCFGCALbits.RTCPTR = 2;
    r->month_day =RTCVAL;  
    RCFGCALbits.RTCPTR = 1;
    r->weekday_hour = RTCVAL;  
    RCFGCALbits.RTCPTR = 0;
    r->min_sec = RTCVAL;  
#endif // __XC16__
}

rtcc* rtcc_new(void)
{
    rtcc *r = (rtcc *)malloc(sizeof(rtcc)); 

    r->year         = flash_clock.year;
    r->month_day    = flash_clock.month_day;
    r->weekday_hour = flash_clock.weekday_hour;
    r->min_sec      = flash_clock.min_sec;

    return r; 
}

void rtcc_cpy(rtcc *dst, rtcc *src)
{
    dst->year         = src->year ;
    dst->weekday_hour = src->weekday_hour ;
    dst->month_day    = src->month_day ;
    dst->min_sec      = src->min_sec ;
}

