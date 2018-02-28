#ifndef MAIN_H
#define MAIN_H

#ifdef __cpluslus
	extern "C" {
#endif // __cplusplus

#ifdef __XC16__
#include <xc.h>
#endif


#define CH0_TRIS    &TRISD
#define CH0_LAT     &LATD
#define CH0_IO_BIT  2
#define CH0_AN_NUM  25

#define CH1_TRIS    &TRISD
#define CH1_LAT     &LATD
#define CH1_IO_BIT  3
#define CH1_AN_NUM  44

#define CH2_TRIS    &TRISD
#define CH2_LAT     &LATD
#define CH2_IO_BIT  12
#define CH2_AN_NUM  45


#define UI_BTN_LEFT       _RD2
#define UI_BTN_RIGHT      _RD12
#define UI_BTN_ENTER      _RD3 
#define UI_BTN_LEFT_TRIS  _TRISD2
#define UI_BTN_ENTER_TRIS _TRISD3

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "mTouch.h"
#include "ui.h"
#include "lcd.h"
#include "timer.h"
#include "sigma_delta_adc.h"
#include "usb_config.h"
#include "usb_gc010.h"
#include "waveform_tables.h"
#include "rtcc.h"
#include "filter.h"
#include "utilities.h"
#include "pipeline.h"

void PowerManagedModulesInit();
void LCDInit();
void RTCCInit();
void SleepInit();
void WakeupInit();
void CalSineTable();

 void pmd_all();

void pmd_gc_on();

void pmd_gc_off();

void sleepHandler();

uint8_t spi1_xfer(void);

void uiBehavior_autoScrollSubmenu(ui *p);

void demoMenu(void);

extern void uiCallback_setScrollSpeedFast();

extern void uiCallback_setScrollSpeedSlow();

void ui_mtouch_cb(ui *p);

#ifdef __cplusplus
	}
#endif // __cplusplus

#endif //MAIN_H
