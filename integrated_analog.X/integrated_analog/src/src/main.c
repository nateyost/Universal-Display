/********************************************************************
 FileName:      main.c
 Dependencies:  See INCLUDES section
 Processor:	    PIC24FJ128GC010 Advanced Analog Microcontroller
 Hardware:      This code is based around the analog peripherals of the
                PIC24FJ128GC010 Family of advanced analog microcontrollers.
 Complier:      Microchip XC16 V 1.11 or greater
 Company:       Microchip Technology, Inc.

 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the "Company") for its PIC® Microcontroller is intended and
 supplied to you, the Company's customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 ********************************************************************
 File Description:

 Change History:
  Rev   Description
  ----  -----------------------------------------
  0.1   Initial version 11/15/2012 by J.James
  0.2   Revised 5/1/2013 by P. Schreiber
  0.3   Initial relese 5/24/2013 by P. Schreiber

********************************************************************/
#include <xc.h>
#include "main.h"
#include "ADC_SigmaDelta.h"
#include "ui.h"
#include <math.h>			 // need this to calculate the sin function
#define SINE_POINTS 512                  // number of output points in the sine wave
#define MIC_PEAK_READING 250             // for unamplified mic. For Appendix C amp, change to ~4000.

unsigned int DACSineCodes[SINE_POINTS]; //declare a RAM buffer space
void DAC2_OutputContinuousSineWave(double SineFrequency); // function prototype

_CONFIG4(DSWDTPS_DSWDTPS1F // 1:68719476736 (25.7 Days)
        & DSWDTOSC_LPRC // DSWDT uses LPRC as reference clock
        & DSBOREN_OFF // DSBOR Disabled
        & DSWDTEN_OFF // DSWDT Disabled
        & DSSWEN_OFF // Deep Sleep operation is always disabled
        & RTCBAT_ON // RTC operation is continued through VBAT
        & PLLDIV_DIV2 // Oscillator divided by 2 (8 MHz input)
        & I2C2SEL_SEC // I2C2 is multiplexed to SDA2/RF4 and SCL2/RF5 (not used for demo)
        & IOL1WAY_OFF) // The IOLOCK bit can be set and cleared using the unlock sequence

_CONFIG3(WPFP_WPFP0 // Page 0 (0x00)
        & SOSCSEL_ON // SOSC circuit selected
        & WDTWIN_PS25_0 // Watch Dog Timer Window Width is 25 percent
        & BOREN_OFF // Brown-out Reset Disabled
        & WPDIS_WPDIS // Disabled
        & WPCFG_WPCFGDIS // Disabled
        & WPEND_WPSTARTMEM) // Write Protect from page 0 to WPFP

_CONFIG2(POSCMD_XT // XT Oscillator Enabled
        & WDTCLK_LPRC // WDT uses SOSC input
        & OSCIOFNC_ON // OSCO/CLKO/RC15 functions as port I/O (RC15)
        & FCKSM_CSDCMD // Clock switching and Fail-Safe Clock Monitor are disabled
        & FNOSC_PRIPLL // Primary Oscillator with PLL module (XTPLL,HSPLL, ECPLL)
        & ALTADREF_AVREF_RB // AVREF+/AVREF- are mapped to RB0/RB1
        & ALTCVREF_CVREF_RB // CVREF+/CVREF- are mapped to RB0/RB1
        & WDTCMX_LPRC // WDT always uses LPRC as its clock source
        & IESO_OFF) // Disabled

_CONFIG1(WDTPS_PS32768 // 1:32,768 (not used)
        & FWPSA_PR128 // 1:128
        & WINDIS_OFF // Standard Watchdog Timer
        & FWDTEN_WDT_DIS // WDT disabled in hardware; SWDTEN bit disabled
        & ICS_PGx3 // Emulator functions are shared with PGEC3/PGED3
        & LPCFG_ON // Low voltage regulator controlled in sw by RETEN bit
        & GWRP_OFF // Disabled
        & GCP_OFF // Code protection is disabled
        & JTAGEN_OFF) // Disabled

#define USE_SOSC        // Use the on-board 32.768KHz SOSC for RTCC and for the LCD timing generator

#define TC77CS _LATE9
#define TC77CS_TRIS _TRISE9

#define SW_VDD _LATA9
#define SW_VDD_TRIS _TRISA9
#define LED1 _LATE7
#define LED2 _LATB6

#define CLOCK 0
#define ADC_16BIT_SIGMA_DELTA 1
#define ADC_12BIT_PIPELINE 2
#define DAC_10BIT 3
#define TEMPERATURE 4
#define LCD_TEST 5
#define AUDIO 6
#define MAX_STATE 7

#define FOSC 32000000	/*Hz*/

lcd *LCD37x7;
ui *gcui;
rtcc *clock;

uint16_t adcPotReading;
uint16_t adcLiteReading;
int16_t  adcExtReading;

uint16_t adc16_reading_ch0;
int16_t adc16_reading_ch1;
uint8_t temperature;

// Stores screen state when switched to sleep
volatile int8_t previousMenu;
volatile int8_t previousSubMenu;

int main() {

    // Disable nested interrupts
    _NSTDIS = 1;

    CalSineTable();
    PowerManagedModulesInit();
    LCDInit();
    RTCCInit();

    while (1) {
        if (gcui->goto_sleep == 0)
        {
            if (USBGetDeviceState() == DETACHED_STATE)
            {
                USBDeviceAttach();
            }
            CDCTxService();
        } 
        else
        {
            // Prepare for sleep
            SleepInit();

            // Update time every RTCC interrupt (minute)
            LCD37x7->blink_enable = 0;
            LCD37x7->blink_mask = 0;
            lcd_printf("%02x:%02x%c%c%c", clock->hour, clock->min, 0x85, 0x83, 0x84);
            lcd_canvas_raster(LCD37x7);
            // RTCC alarm every minute
            ALCFGRPTbits.AMASK = 2;
            LCD37x7->iconALL = 0;
            LCD37x7->iconMICROCHIP = 1;
            lcd_icon_raster_mcu(LCD37x7);

            Sleep();
        }
    }
    return 0;
}

void LCDInit() {
    // Initialize LCD: no charge pump, 8 common drivers
    LCDPSbits.WFT = 0; // Type A waveform
    LCDPSbits.LP = 2; // LCD Prescaller 1:3
    LCDCONbits.LMUX = 0x07; // 8 commons, 1/3 Bias
#ifdef USE_SOSC
    LCDCONbits.CS = 2; // Clock is SOSC
#else
    LCDCONbits.CS = 1; // Clock is LPRC
#endif
    LCDREFbits.VLCD1PE = 0; // Enable internal bias
    LCDREFbits.VLCD2PE = 0;
    LCDREFbits.VLCD3PE = 0;
    LCDREFbits.LRLAP = 0x03; // ladder in High-Power Interval A (transition)
    LCDREFbits.LRLBP = 0x03; // ladder in High-Power Interval B (steady state, for higher contrast ratio))
    LCDREFbits.LRLAT = 0x03; // Internal LCD reference ladder is in A Power mode for 3 clocks and B Power mode for 13 clocks
    LCDREFbits.LCDIRE = 1; // Internal Reference Enable
    LCDREFbits.LCDCST = 2; // Contrast is 2/7ths of maximum
    LCDCONbits.LCDEN = 1; // enable LCD module
}

void RTCCInit() {
    // Initialize RTCC
    asm volatile("DISI #0x3fff");
    __builtin_write_RTCWEN();
    asm volatile("DISI #0");
#ifdef USE_SOSC
    RTCPWCbits.RTCLK = 0; // SOSC
#else
    RTCPWCbits.RTCLK = 1; // LPRC
#endif
    RCFGCALbits.RTCPTR = 3; // Reset Pointer
    RCFGCALbits.RTCOE = 0;
    ALCFGRPTbits.AMASK = 0; // 1/2s alarm
    ALCFGRPTbits.CHIME = 1; // Chime enabled
    ALCFGRPTbits.ALRMEN = 1; // Alarm enabled
    RCFGCALbits.RTCEN = 1;
    _RTCIF = 0;
    _RTCIP = 6;
    _RTCIE = 1;

    clock = (rtcc *) rtcc_new();
    char *time = __TIME__;
    clock->hour_tens = time[0] - '0';
    clock->hour_ones = time[1] - '0';
    clock->min_tens = time[3] - '0';
    clock->min_ones = time[4] - '0';
    rtcc_set(clock);
    rtcc_read(clock);

    LCD37x7 = (lcd *) lcd_new();

    gcui = (ui *) ui_new(demoMenu);

    gcui->btn_cb = ui_mtouch_cb;
    gcui->default_btn_cb = ui_mtouch_cb;
    gcui->menu_state = CLOCK;
    gcui->p_lcd = LCD37x7;
    gcui->Fosc = FOSC;
    gcui->refresh_rate_while_scrolling = 42;
    gcui->refresh_rate_not_scrolling = 10;
    gcui->goto_sleep = 0;

}

void PowerManagedModulesInit() {
    // Initialize SD ADC
        ADC_SD_Init();

    // Call MTouchInit() before our custom settings. MTouchInit() will clear out our settings otherwise.
    // Initialize mTouch
        MTouchInit();
        MTouchSetSensor(0, CH0_TRIS, CH0_LAT, CH0_IO_BIT, CH0_AN_NUM, 1000, -1, -1); // sensor #0
        MTouchSetSensor(1, CH1_TRIS, CH1_LAT, CH1_IO_BIT, CH1_AN_NUM, 1000, -1, -1); // sensor #1
        MTouchSetSensor(2, CH2_TRIS, CH2_LAT, CH2_IO_BIT, CH2_AN_NUM, 1000, -1, -1); // sensor #2
        MTouchSetButton(0, 0, DECODE_PRESS_RELEASE);
        MTouchSetButton(1, 1, DECODE_PRESS_RELEASE);
        MTouchSetButton(2, 2, DECODE_PRESS_RELEASE);
    //
    // RESET out to connector
    //
        _TRISE5 = 0;
        _ANSE5 = 0;
        _LATE5 = 0 ;
    
    //
    // INT pin into MPU from connector
    //
        _TRISE6 = 1;
        _ANSE6 = 0;

    // Init SW_VDD

        SW_VDD_TRIS = 0;
        SW_VDD = 1;
        _ANSA9 = 0;
 
    // Initialize LEDs to OFF
        _TRISE7 = 0;
        _LATE7 = 0;
        _TRISB6 = 0;
        _LATB6 = 0;

    // Initialize INT0 Switch
        _TRISD0 = 1;
        _ANSD0 = 0;
        _INT0IF = 0;
        _INT0IE = 1;

    // Initialize Timer 1
        T1CONbits.TON = 1;
        T1CONbits.TCKPS = 3;
        PR1 = 1488;
        _T1IF = 0;
        _T1IE = 1;

    // Initialize Timer 2: 12b Pipeline ADC update rate
        T2CONbits.TON = 1;
        T2CONbits.TCKPS = 0b01;     // prescale divisor of 8
        PR2 = 48000;
        _T2IF = 0;
        _T2IE = 1;

    // Initialize Timer 3
        T3CONbits.TON = 1;
        T3CONbits.TCKPS = 0;
        PR3 = 2000;
        _T3IF = 0;
        _T3IE = 1;

    // Initialize Timer 4
        T4CONbits.TON = 1;
        _T4IP = 7; // audio out has highest priority
        _T4IE = 0; // disable interrupts, DMA will trigger internally

    // Initialize Timer 5: refresh rate for SD_ADC reading and temperature reading
        T5CONbits.TON = 1;
        T5CONbits.TCKPS = 0b10; // prescale divisor of 64
        PR5 = 50000;
        IPC7bits.T5IP = 1;      // set to a low priority to allow ADC to read
        _T5IF = 0;
        _T5IE = 1;

    // Initialize SPI
        __builtin_write_OSCCONL(OSCCON & 0xBF);
        _RP24R = 8; // sck
        _SDI1R = 20; // miso
        _RP25R = 7; // mosi
        __builtin_write_OSCCONL(OSCCON | 0x40);

        _ANSD5 = 0; // miso
        _ANSD4 = 0; // mosi

        TC77CS_TRIS = 0;
        TC77CS = 1;

        _MSTEN = 1;
        _MODE16 = 1;
        _CKP = 0;
        _CKE = 0;
        _SPIROV = 0;
        _SPIEN = 1;
        _INT0IE = 1;

    // Initliaze USB
        _USB1IF = 0;
        _USB1IP = 5;
        _USB1IE = 1;
        USBDeviceInit();
}

void SleepInit() {
    // Disable SPI
    // SDI, SDO, SCK, CS to zero
        _SPIEN = 0;
        TC77CS = 0;

    // Disable DACs
    // DAC's outputs to zero
        DAC1CONbits.DACEN = 0;
        DAC2CONbits.DACEN = 0;

    // Disable interrupts
        _T1IE = 0;
        _T2IE = 0;
        _T3IE = 0;
        _T4IE = 0;
        _T5IE = 0;
        _USB1IE = 0;

    // External components power down
        SW_VDD = 0;
        TC77CS = 0;     // to prevent back-driving through pullup

    // Modules power down
        _SPI1MD = 1;
        _T1MD = 1;
        _T2MD = 1;
        _T3MD = 1;
        _T4MD = 1;
        _T5MD = 1;
        _LCDMD = 0; // Keep LCD on during sleep
        _USBMD = 1;
        _RTCCMD = 0; // Keep RTCC on during sleep
        _DAC1MD = 1;
        _DAC2MD = 1;
        _SDADMD = 1;
        _ADC1MD = 1;
        _CTMUMD = 1;

    // RTCC alarm every minute
        ALCFGRPTbits.AMASK = 2;

    // Save the current screen to restore
        previousMenu = gcui->menu_state;
        previousSubMenu = gcui->submenu_state;
}

void WakeupInit() {
    // Modules power on
        _SPI1MD = 0;
        _T1MD = 0;
        _T2MD = 0;
        _T3MD = 0;
        _T4MD = 0;
        _T5MD = 0;
     _USBMD = 0;
        _DAC1MD = 0;
        _DAC2MD = 0;
        _SDADMD = 0;
        _ADC1MD = 0;
        _CTMUMD = 0;

        CalSineTable();                 // reload the sine table RAM
        PowerManagedModulesInit();      // recalibrate the ADC

    // Jump to previous screen
        gcui->menu_state = previousMenu;
        gcui->submenu_state = previousSubMenu;

    // RTCC alarm every half second
        ALCFGRPTbits.AMASK = 0;
}

void demoMenu(void) {


        gcui->menu_state_max = MAX_STATE;

        static uint32_t scaled_pot;
        static uint32_t scaled_lite;
        static uint16_t scaled_audio;
        switch (gcui->menu_state) {

        case CLOCK:
            
            LCD37x7->iconMICROCHIP = 1;

            strcpy(gcui->demo_title, "CLOCK");
            strcpy(gcui->demo_description, "HH:MM");

            gcui->submenu_state_max = 5;
            switch (gcui->submenu_state) {
                case 0:
                    gcui->submenu_using_lr = 0;
                    rtcc_read(clock);
                    LCD37x7->blink_mask = BLINK_COLON;
                    lcd_printf("%c%02x:%02x%c", 0x87, clock->hour, clock->min, 0x87);
                    break;
                case 1:
                    gcui->submenu_using_lr = 1;
                    LCD37x7->blink_mask = BLINK_HOUR_T;

                    lcd_printf("%c%02x:%02x%c", 0x7f, clock->hour, clock->min, 0x7e);
                    if (gcui->btn_left_event) {
                        gcui->btn_left_event = 0;
                        rtcc_read(clock);
                        clock->hour_tens--;
                        rtcc_set(clock);
                    }
                    if (gcui->btn_right_event) {
                        gcui->btn_right_event = 0;
                        rtcc_read(clock);
                        clock->hour_tens++;
                        rtcc_set(clock);
                    }
                    break;
                case 2:
                    LCD37x7->blink_mask = BLINK_HOUR_O;
                    lcd_printf("%c%02x:%02x%c", 0x7f, clock->hour, clock->min, 0x7e);
                    if (gcui->btn_left_event) {
                        gcui->btn_left_event = 0;
                        rtcc_read(clock);
                        clock->hour_ones--;
                        rtcc_set(clock);
                    }
                    if (gcui->btn_right_event) {
                        gcui->btn_right_event = 0;
                        rtcc_read(clock);
                        clock->hour_ones++;
                        rtcc_set(clock);
                    }
                    break;
                case 3:
                    LCD37x7->blink_mask = BLINK_MIN_T;
                    lcd_printf("%c%02x:%02x%c", 0x7f, clock->hour, clock->min, 0x7e);
                    if (gcui->btn_left_event) {
                        gcui->btn_left_event = 0;
                        rtcc_read(clock);
                        clock->min_tens--;
                        rtcc_set(clock);
                    }
                    if (gcui->btn_right_event) {
                        gcui->btn_right_event = 0;
                        rtcc_read(clock);
                        clock->min_tens++;
                        rtcc_set(clock);
                    }
                    break;
                case 4:
                    LCD37x7->blink_mask = BLINK_MIN_O;
                    lcd_printf("%c%02x:%02x%c", 0x7f, clock->hour, clock->min, 0x7e);
                    if (gcui->btn_left_event) {
                        gcui->btn_left_event = 0;
                        rtcc_read(clock);
                        clock->min_ones--;
                        rtcc_set(clock);
                    }
                    if (gcui->btn_right_event) {
                        gcui->btn_right_event = 0;
                        rtcc_read(clock);
                        clock->min_ones++;
                        rtcc_set(clock);
                    }
                    break;
                default:
                    break;
            }
            break;

        case ADC_16BIT_SIGMA_DELTA:
            
            sprintf(gcui->demo_title, "%c%c ADC", 0x80, 0x81);
            strcpy(gcui->demo_description, "CH:DATA");
            lcd_printf("");

            gcui->submenu_state_max = 1;
            switch (gcui->submenu_state)
            {
                case 0:
                    lcd_printf("%02d:%04x", 1, adc16_reading_ch1);
                    break;
            }

            break;

        case ADC_12BIT_PIPELINE:
            
//
// Both DACs off when entering menu
//
            DAC1CONbits.DACEN = 0;
            DAC2CONbits.DACEN = 0;
            strcpy(gcui->demo_title, "12bPipe");
            strcpy(gcui->demo_description, "lineADC");
            lcd_printf("");
            gcui->submenu_state_max = 4;
            printf_usb("%04x\t%04x\t%04x\n\r", adcPotReading, adcLiteReading, adcExtReading);
            uint16_t adcPot10, adcLite10, adcExt10;
            adcPot10 = adcPotReading >> 2;
            adcLite10 = adcLiteReading >> 2;
            adcExt10 = adcExtReading >> 2;
            switch (gcui->submenu_state) {
                case 0:
                    scaled_pot = convert_range(((uint32_t) adcPotReading), 0x0FFF, 0x0000, 37, 0);
                    scaled_lite = convert_range((uint32_t) 4095 - ((uint32_t) adcLiteReading), 0x0FFF, 0x0000, 37, 0);
                    gcui->submenu_using_lr = 0;
                    lcd_draw_rbox_fill(LCD37x7, 0, 0, 3, scaled_pot + 1);
                    lcd_draw_rbox_fill(LCD37x7, 0, 4, 3, scaled_lite);
                    break;
                case 1: // Pot
                    lcd_printf("%02d:%04x", 19, adcPotReading);
                    break;
                case 2: // Light Sensor
                    lcd_printf("%02d:%04x", 22, adcLiteReading);
                    break;
                case 3: // External Channel
                    lcd_printf("%02d:%04x", 8, adcExtReading);
                    break;
                default:
                    break;
            }
            break;

        case DAC_10BIT:
            TC77CS = 1;
 //
// Both DACs on (DMA has been always running in the background)
//
            DAC1CONbits.DACEN = 1;
            DAC2CONbits.DACEN = 1;
         //
         // sussessive pressing of O pad will play up the scale
         //
            strcpy(gcui->demo_title, "10b DAC");
            strcpy(gcui->demo_description, "Sines");
            gcui->submenu_state_max = 3;

            switch (gcui->submenu_state) {
                case 0:
                    lcd_printf("110Hz");
                    DAC2_OutputContinuousSineWave(110); // low A
                    break;
                case 1:
                    lcd_printf("220Hz");
                    DAC2_OutputContinuousSineWave(220); // middle A
                    break;
                case 2:
                    lcd_printf("440Hz");
                    DAC2_OutputContinuousSineWave(440); // high A
                    break;
                 default:
                    break;
            }

            break;

        case TEMPERATURE:
            TC77CS = 0;
//
// Both DACs off when entering menu
//
            DAC1CONbits.DACEN = 0;
            DAC2CONbits.DACEN = 0;
            strcpy(gcui->demo_title, "Temp");
            strcpy(gcui->demo_description, "TC77");
            LCD37x7->iconTHERMO = 1;
            gcui->submenu_state_max = 2;
            switch (gcui->submenu_state) {
                case 0:
                    lcd_printf("%02d%cC", temperature, LCD_DEGREE);
                    LCD37x7->iconFAREN = 0;
                    LCD37x7->iconCELCIUS = 1;
                    break;
                case 1:
                    lcd_printf("%02.0f%cF", ((float) temperature)*1.8 + 32.0, LCD_DEGREE);
                    LCD37x7->iconCELCIUS = 0;
                    LCD37x7->iconFAREN = 1;
                    break;
                default:
                    break;
            }

            break;
        case LCD_TEST:
            TC77CS = 1;
            gcui->btn_cb = uiBehavior_autoScrollSubmenu;
            strcpy(gcui->demo_title, "LCD");
            strcpy(gcui->demo_description, "TEST");
            uint8_t index;
            if (!LCD37x7->icons_disabled) {
                LCD37x7->iconALL = 0xFFFFFFFFFFFFFFFF;
            }
            gcui->submenu_state_max = 18;
            switch (gcui->submenu_state) {
                case 0:
                    if (!LCD37x7->printf_disabled) {
                        for (index = 0; index < LCD_MAX_X; index++) {
                            LCD37x7->canvas[index].byte = 0x7F;
                        }
                    }
                    break;
                case 1: lcd_printf("Dollar");
                    LCD37x7->iconDollar = LCD37x7->half_sec;
                    break;
                case 2: lcd_printf("Wheel");
                    LCD37x7->iconWHEEL1 = LCD37x7->half_sec;
                    LCD37x7->iconWHEEL2 = LCD37x7->half_sec;
                    LCD37x7->iconWHEEL3 = LCD37x7->half_sec;
                    LCD37x7->iconWHEEL4 = LCD37x7->half_sec;
                    LCD37x7->iconWHEEL5 = LCD37x7->half_sec;
                    LCD37x7->iconWHEEL6 = LCD37x7->half_sec;
                    LCD37x7->iconWHEEL7 = LCD37x7->half_sec;
                    LCD37x7->iconWHEEL8 = LCD37x7->half_sec;
                    LCD37x7->iconWHEEL9 = LCD37x7->half_sec;
                    LCD37x7->iconWHEEL10 = LCD37x7->half_sec;
                    break;
                case 3: lcd_printf("WiFi");
                    LCD37x7->iconWIFI = LCD37x7->half_sec;
                    break;
                case 4: lcd_printf("Sun");
                    LCD37x7->iconSUN = LCD37x7->half_sec;
                    break;
                case 5: lcd_printf("Pan");
                    LCD37x7->iconPAN = LCD37x7->half_sec;
                    break;
                case 6: lcd_printf("Zigbee");
                    LCD37x7->iconZIG = LCD37x7->half_sec;
                    break;
                case 7: lcd_printf("Cloud");
                    LCD37x7->iconCLOUD = LCD37x7->half_sec;
                    break;
                case 8: lcd_printf("Heart");
                    LCD37x7->iconHEART = LCD37x7->half_sec;
                    break;
                case 9: lcd_printf("Strip");
                    LCD37x7->iconSTRIP = LCD37x7->half_sec;
                    break;
                case 10: lcd_printf("Drop");
                    LCD37x7->iconDROP = LCD37x7->half_sec;
                    break;
                case 11: lcd_printf("Temp");
                    LCD37x7->iconTHERMO = LCD37x7->half_sec;
                    break;
                case 12: lcd_printf("Batt");
                    LCD37x7->iconBAT1 = LCD37x7->half_sec;
                    LCD37x7->iconBAT2 = LCD37x7->half_sec;
                    LCD37x7->iconBAT3 = LCD37x7->half_sec;
                    LCD37x7->iconBAT4 = LCD37x7->half_sec;
                    break;
                case 13: lcd_printf("Farenh");
                    LCD37x7->iconFAREN = LCD37x7->half_sec;
                    break;
                case 14: lcd_printf("Celsius");
                    LCD37x7->iconCELCIUS = LCD37x7->half_sec;
                    break;
                case 15: lcd_printf("Cent");
                    LCD37x7->iconCENT = LCD37x7->half_sec;
                    break;
                case 16: lcd_printf("RMB");
                    LCD37x7->iconRMB = LCD37x7->half_sec;
                    break;
                case 17: lcd_printf("MCHP");
                    LCD37x7->iconMICROCHIP = LCD37x7->half_sec;
                    break;
                default:
                    break;
            }
            break;
        case AUDIO:
            strcpy(gcui->demo_title, "MIC");
            strcpy(gcui->demo_description, "TEST");
            lcd_printf(""); // Clear the canvas

            adcExtReading-= 2047;       // adjust for mid-scale bias voltage
            if (adcExtReading < 0)
            {
              adcExtReading = ~(adcExtReading) + 1;  // absolute value
            }
            if (adcExtReading > MIC_PEAK_READING) adcExtReading = MIC_PEAK_READING;
                scaled_audio = convert_range(((uint32_t) adcExtReading), MIC_PEAK_READING, 0, 37, 1);

                lcd_draw_rbox_fill(LCD37x7, 0, 0, 3, scaled_audio);
            break;
    }
}

        uint8_t spi1_xfer(void) {
        TC77CS = 0;
        uint16_t value;
        SPI1BUF = 0x00;
        while (_SPI1IF != 1);
            _SPI1IF = 0;
            value = SPI1BUF;
            TC77CS = 1;

            value >>= 3;
            value = (uint8_t) ((float) value * 0.0625);
        return value;
}

void uiBehavior_autoScrollSubmenu(ui *p) {
    static uint8_t state_oneshot[] = {0, 0, 0};

    switch (state_oneshot[0]) {
        case 0:
            if (p->button_state[0] == 0x01) {
                state_oneshot[0] = 1;
            }
            break;
        case 1:
            if (p->button_state[0] == 0x02) {
                state_oneshot[0] = 2;
            }
            break;
        case 2:
            p->button_event[0] = 1;
            state_oneshot[0] = 0;
            break;
    }
    switch (state_oneshot[2]) {
        case 0:
            if (p->button_state[2] == 0x01) {
                state_oneshot[2] = 1;
            }
            break;
        case 1:
            if (p->button_state[2] == 0x02) {
                state_oneshot[2] = 2;
            }
            break;
        case 2:
            p->button_event[2] = 1;
            state_oneshot[2] = 0;
            break;
    }

    if (gcui->two_sec_oneshot == 1) {
        gcui->two_sec_oneshot = 0;
        p->button_event[1] = 1;
    } else {
        p->button_event[1] = 0;
    }

}

extern void uiCallback_setScrollSpeedFast() {
    T1CONbits.TON = 1;
    T1CONbits.TCKPS = 3;
    PR1 = 1488; //42Hz
    _T1IF = 0;
    _T1IE = 1;
}

extern void uiCallback_setScrollSpeedSlow() {
    T1CONbits.TON = 1;
    T1CONbits.TCKPS = 3;
    PR1 = 6250; // 10Hz
    _T1IF = 0;
    _T1IE = 1;
}

void ui_mtouch_cb(ui *p) {
    static uint8_t state_oneshot[] = {0, 0, 0};
    int index;
    for (index = 0; index < UI_BTN_MAX; index++) {
        switch (state_oneshot[index]) {
            case 0:
                if (p->button_state[index] == 0x01) {
                    state_oneshot[index] = 1;
                }
                break;
            case 1:
                if (p->button_state[index] == 0x02) {
                    state_oneshot[index] = 2;
                }
                break;
            case 2:
                p->button_event[index] = 1;
                state_oneshot[index] = 0;
                break;
        }
    }
}

//SineFrequency value must be between 0.12 Hz and 2600 Hz.  Actual frequency may
//be somewhat off at high frequencies due to timer granularity.  Check with oscope.

void CalSineTable(void)
{
    unsigned int i;
    double x;
    double SineResult;
    short int DACValue;

        x = 4.5; // Near (3/2) * pi, so we start near minima
        i = 0;
    //First we need to compute the values of a sine wave function
        while (1) {
            SineResult = sin(x);
            x += 6.283 / SINE_POINTS;
        if (x > 6.283)
            x = 0;

            DACValue = SineResult * 128;
            DACValue += 512;						// shift to mid-scale of DAC range
        if (DACValue < 0)
            DACValue = 0;
        if (DACValue > 1023)
            DACValue = 1023;				// force into full-scale range

            DACSineCodes[i] = DACValue;
            i++;
        if (i == SINE_POINTS)
            break;
                    }
    //
    // enable the DACs,
    //
            PMD3bits.DAC1MD = 0; // power up DAC1
            PMD6bits.DAC2MD = 0; // power up DAC2
            _LATA9 = 1; // power up WAKE peripherials
    //
    // Note: it is up to the main app to enable the DACs at the proper time
    //       The DMA will be running in the background, so as not to disturb the LCD menu scan loop
    //
    //Init the DMA to update the DACs automatically without CPU intervention
            DMAL = 0x0800;
            DMAH = 0x2800;
            DMASRC0 = (unsigned int) &DACSineCodes[0];
            DMADST0 = (unsigned int) &DAC2DAT;
            DMACNT0 = SINE_POINTS;
            DMAINT0 = 0x2400; //Timer 4 trigger source

            DMACH0 = 0x0244; //16-bit copies, inc src per op, no ch dst per op, repeated one shot
            DMACON = 0x8001; //DMA enabled, round robin servicing
            DMACH0bits.CHEN = 1; //Enable DMA channel 0

    //Init the DAC2 module to generate the continuous sine wave, due to DMA operation

            DAC2CON = 0x008E; //Left justified, DAC off, AVDD as reference, update output as soon as DACxDAT is written
            IFS4bits.DAC2IF = 0;

    //
    // DMA for DAC1 output
    //
    //Init the DMA to update the DACs automatically without CPU intervention

        DMASRC1 = (unsigned int) &DACSineCodes[0];
        DMADST1 = (unsigned int) &DAC1DAT;
        DMACNT1 = SINE_POINTS;
        DMAINT1 = 0x2400; //Timer 4 trigger source

        DMACH1 = 0x0244; //16-bit copies, inc src per op, no ch dst per op, repeated one shot

        DMACH1bits.CHEN = 1; //Enable DMA channel 1

    //Init the DAC1 module to generate the continuous sine wave, due to DMA operation

        DAC1CON = 0x008E; //Left justified, DAC off, AVDD as reference, update output as soon as DACxDAT is written
        IFS4bits.DAC1IF = 0;
}

void DAC2_OutputContinuousSineWave(double SineFrequency) {

    //Now that we have the sine wave DAC codes stored in RAM, continuously
    //output them to the DAC module.  We will use the DMA to do this, so we can
    //use the CPU for other stuff (like using the ADC and USB modules).

    //Make sure the input SineFrequency is in an attainable range.  If not, make
    //it saturate that the nearest value that is attainable.
        if (SineFrequency < 0.12) {
            SineFrequency = 0.12;
        } else if (SineFrequency > 2600) {
             SineFrequency = 2600;
    }

    //Load the period register for Timer4 for DMA and DAC trigger generation
        PR4 = (FOSC / ((double) SineFrequency * ((double) SINE_POINTS * 2))) - 1; //This value sets the DAC waveform output frequency
        TMR4 = 0x0000;
        T4CON = 0x8000; //1:1 mode, 16-bit
 }

void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void) {
    ui_tasks(gcui); // Calls Menu
    lcd_canvas_raster(LCD37x7);
    gcui->button_state[0] = MTouchGetButtonState(0);
    gcui->button_state[1] = MTouchGetButtonState(1);
    gcui->button_state[2] = MTouchGetButtonState(2);

    _T1IF = 0;
}

void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt(void) {
    _LATE6 ^= 1;
    adcPotReading = adcSingleChannel(19, 0xFF);
    adcLiteReading = adcSingleChannel(22, 0xff);
    adcExtReading = adcSingleChannel(8, 0xff);
    _T2IF = 0;
}

void __attribute__((__interrupt__, __auto_psv__)) _T3Interrupt(void) {
    MTouchInit();
    MTouchAcquisition();
    MTouchDecode();

    uint8_t temp = 0;
    temp += MTouchGetButtonState(0);
    temp += MTouchGetButtonState(1);
    temp += MTouchGetButtonState(2);
    if (temp < 6)
        LED1 = 1;
    else
        LED1 = 0;

    _T3IF = 0;
}
//
// Timer 5 is the SD_ADC and temperature update rate 

void __attribute__((__interrupt__, __auto_psv__)) _T5Interrupt(void) {

    adc16_reading_ch1 = ADC_SD_GetCalibratedResult();
    temperature = spi1_xfer();
    _T5IF = 0;
}

void __attribute__((__interrupt__, __auto_psv__)) _RTCCInterrupt(void) {
    static uint8_t count = 0;
    asm volatile("disi #0x3fff");
    LCD37x7->half_sec ^= 1;
    if (gcui->menu_state == CLOCK) {
        LCD37x7->blink_enable ^= 1;
    } else {
        LCD37x7->blink_enable = 0;
    }
    gcui->half_sec_oneshot = 1;
    count++;
    if ((count & 0x03) == 0x02) {
        gcui->two_sec_oneshot = 1;
    }
    rtcc_read(clock);
    asm volatile("disi #0x0");
    _RTCIF = 0;
}

void __attribute__((__interrupt__, __auto_psv__)) _INT0Interrupt(void) {
    gcui->goto_sleep ^= 1;
    if (gcui->goto_sleep == 0) {
        WakeupInit();
    } else {
    }
    _INT0IF = 0;
}

void __attribute__((interrupt, auto_psv)) _StackError(void) {
    while (1) {
        Nop();
        Nop();
        Nop();
    }
}

void __attribute__((interrupt, auto_psv)) _OscillatorFail(void) {
    while (1) {
        Nop();
        Nop();
        Nop();
    }
}

void __attribute__((interrupt, auto_psv)) _AddressError(void) {
    while (1) {
        Nop();
        Nop();
        Nop();
    }
}

void __attribute__((interrupt, auto_psv)) _MathError(void) {
    while (1) {
        Nop();
        Nop();
        Nop();
    }
}

// Required callback definition for mTouch library's Debug functionality

void LogChar(char ch) {
}
