#ifndef RTCC_H
#define RTCC_H

#ifdef __cpluslus
	extern "C" {
#endif // __cplusplus

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct rtcc_tag {
    union {
        struct {
            union {
                struct {
                    uint8_t sec_ones:4;
                    uint8_t sec_tens:4;
                };
                uint8_t sec;
            };
            union {
                struct {
                    uint8_t min_ones:4;
                    uint8_t min_tens:4;
                };
                uint8_t min;
            };
        };
        uint16_t min_sec;
    };
    union {
        struct {
            union {
                struct {
                    uint8_t hour_ones:4;
                    uint8_t hour_tens:4;
                };
                uint8_t hour;
            }; 
            union {
                struct {
                    uint8_t weekday_ones:4;
                    uint8_t weekday_tens:4;
                };
                uint8_t weekday;
            };
        };
        uint16_t weekday_hour;
    };
    union {
        struct {
            union {
                struct {
                    uint8_t day_ones:4;
                    uint8_t day_tens:4;
                };
                uint8_t day;
            };
            union {
                struct {
                    uint8_t month_ones:4;
                    uint8_t month_tens:4;
                };
                uint8_t month;
            };
        };
        uint16_t month_day;
    };
    union {
        struct {
            uint8_t year_ones:4;
            uint8_t year_tens:4;
        };
        uint8_t year;
    };
}rtcc;

void rtcc_read(rtcc *r);
void rtcc_set(rtcc *r);
void rtcc_cpy(rtcc *dst, rtcc *src);
rtcc* rtcc_new(void);

#ifdef __cplusplus
	}
#endif // __cplusplus

#endif //RTCC_H
