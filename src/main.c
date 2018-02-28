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
 (the "Company") for its PIC� Microcontroller is intended and
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
 v1.1
 v1.2  
 v1.3   
 v1.4
 v1.5
 v1.6
 v1.7 by Nathan Yost 
 v1.16 fixed current draw to start up @ least 3.6mA
 v1.17 update offset calibration
 v1.18 fix problem scale
 v1.22 removed ECC from DEE emulation for correct flash
 v1.23 simplified flash write and read setup constants for flash addresses
 v1.24 fixed the offset to fix decimal place is disprd and added write/read to flash 
 * menu item disprd

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
        & RTCBAT_OFF // RTC operation is continued through VBAT
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

_CONFIG2(POSCMD_NONE // XT Oscillator Enabled
        & WDTCLK_LPRC // WDT uses SOSC input
        & OSCIOFCN_ON // OSCO/CLKO/RC15 functions as port I/O (RC15)
        & FCKSM_CSDCMD // Clock switching and Fail-Safe Clock Monitor are disabled
        & FNOSC_FRCPLL // Primary Oscillator with PLL module (XTPLL,HSPLL, ECPLL)
        & ALTADREF_AVREF_RB // AVREF+/AVREF- are mapped to RB0/RB1
        & ALTCVREF_CVREF_RB // CVREF+/CVREF- are mapped to RB0/RB1
        & WDTCMX_LPRC // WDT always uses LPRC as its clock source
        & IESO_OFF) // Disabled

_CONFIG1(WDTPS_PS32768 // 1:32,768 (not used)
        & FWPSA_PR128 // 1:128
        & WINDIS_OFF // Standard Watchdog Timer
        & FWDTEN_WDT_DIS // WDT disabled in hardware; SWDTEN bit disabled
        & ICS_PGx2 // Emulator functions are shared with PGEC3/PGED3
        & LPCFG_ON // Low voltage regulator controlled in sw by RETEN bit
        & GWRP_OFF // Disabled
        & GCP_OFF // Code protection is disabled
        & JTAGEN_OFF) // Disabled

#define USE_SOSC        // Use the on-board 32.768KHz SOSC for RTCC and for the LCD timing generator

#define CLOCK 0
#define ADC_16BIT_SIGMA_DELTA 1
#define ADC_12BIT_PIPELINE 2
#define DAC_10BIT 3
#define TEMPERATURE 4
#define LCD_TEST 5
#define AUDIO 6
#define MAX_STATE 4
#define MENU 0
#define GRAPH   1
#define UNITS   2
#define READ     3
//#define PASSWORD   4
#define DISPLAY_READ 4
#define CALIBRATION 5
#define RESET     6
#define EXIT     7
#define RUN 8
#define FOSC 1000000	/*Hz*/

//EEPROM ADDRESSES
#define EEPROM_UNITS 0 //6 bytes
#define EEPROM_DECIMAL 6 //1 bytes
#define EEPROM_MIN 7 //4 bytes
#define EEPROM_MAX 11 //5 bytes
#define EEPROM_CAL_OFFSET 16 //7 bytes
#define EEPROM_CAL_SCALE 23 //7 bytes   
#define EEPROM_CAL_READ_OFFSET 30 //7 bytes
#define EEPROM_CAL_READ_SCALE 37 //7 bytes   
#define SN 3
lcd *LCD37x7;
rtcc *clock;

float adc16_reading_ch0;
int16_t adc16_reading_ch1;


// Stores screen state when switched to sleep
volatile int8_t previousMenu;
volatile int8_t previousSubMenu;


unsigned char EE_Write(unsigned int addr, unsigned char len, unsigned int *data)
{
    int i;
    unsigned int error;
    
    for(i=0;i<len;i++)
    {
        error=DataEEWrite(data[i], addr+i);
    }
    return error;

}
unsigned char EE_Read(unsigned int addr, unsigned char len, unsigned int *data,unsigned int min,unsigned int max)
{
    int i;
    int error=0;
   // unsigned int test[20];
    for(i=0;i<len;i++)
    {
        data[i] = DataEERead(addr+i);
        if(data[i]<min || data[i]>max) error=1;
    }
    return error;
}


int main() {
  
    // Disable nested interrupts
    _NSTDIS = 1;
        // Initialize Oscillator, FRC with PLL
    // FRC=8MHz -> /1=8MHz(RCDIV=0) -> /2=4MHz(PLLDIV=1) -> 96MHz PLL -> /3=32MHz -> /1=32MHz=Fosc(CPDIV=0)
    // Fosc=32MHz, Fcy=Fosc/2=16MHz
    //**********************************************************************
    CLKDIVbits.RCDIV = 3;	//FRC postscaler 1 MHz
    CLKDIVbits.CPDIV = 3;       //System Clock (Fosc) is 4 MHz
    CLKDIVbits.DOZE = 7; //1/128 of peripheral frequency
    
    LCD37x7 = (lcd *) lcd_new();
    DisplayInit(LCD37x7);
    LCDInit();
    DataEEInit();
    dataEEFlags.val = 0;

    if(EE_Read(EEPROM_UNITS,6,&LCD37x7->units[0],0,(SEG14_SIZE-1)))
    {
        LCD37x7->units[0] =SEG14_SIZE-1;//'Blank'
        LCD37x7->units[1] =SEG14_SIZE-1;//'Blank'
        LCD37x7->units[2] =SEG14_SIZE-1;//'Blank'
        LCD37x7->units[3] =SEG14_SIZE-1;//'Blank'
        LCD37x7->units[4] =12;//M
        LCD37x7->units[5] =0;//A 
    }
    
    if(EE_Read(EEPROM_DECIMAL,1,&LCD37x7->decimal,1,5)) LCD37x7->decimal=3;
    if(EE_Read(EEPROM_MIN,4,&LCD37x7->seg[DIS_MIN_START],0,(SEG7_SIZE-1)))
    {
           //min
        LCD37x7->seg[DIS_MIN_START]=5;//blank
        LCD37x7->seg[DIS_MIN_START+1]=1;//blank
        LCD37x7->seg[DIS_MIN_START+2]=1;//1
        LCD37x7->seg[DIS_MIN_START+3]=1;//0
    }
    
    if(EE_Read(EEPROM_MAX,5,&LCD37x7->seg[DIS_MAX_START],0,(SEG7_SIZE-1)))
    {
        //max
        LCD37x7->seg[DIS_MAX_START]=3;//blank
        LCD37x7->seg[DIS_MAX_START+1]=1;//1
        LCD37x7->seg[DIS_MAX_START+2]=1;//0
        LCD37x7->seg[DIS_MAX_START+3]=1;//0
        LCD37x7->seg[DIS_MAX_START+4]=1;//0 
    }
    update_min_max(LCD37x7);
    
    if(EE_Read(EEPROM_CAL_OFFSET,6,&LCD37x7->offset_array[0],26,35))
    {           
        calibration_offset(LCD37x7,SN);
    }
    if(EE_Read(EEPROM_CAL_OFFSET+6,1,&LCD37x7->offset_array[6],0,1)) LCD37x7->offset_array[6]=1;//0=positive 1=negative
    if(EE_Read(EEPROM_CAL_SCALE,7,&LCD37x7->scale_array[0],26,35))
    {
        calibration_scale(LCD37x7,SN);
    }
    update_off_scale(LCD37x7);
    PowerManagedModulesInit();
    if(EE_Read(EEPROM_CAL_READ_OFFSET,6,&LCD37x7->read_offset_array[0],26,35))
    {           
        //0
        LCD37x7->read_offset_array[0]=26;
        LCD37x7->read_offset_array[1]=26;
        LCD37x7->read_offset_array[2]=26;
        LCD37x7->read_offset_array[3]=26;
        LCD37x7->read_offset_array[4]=26;//'.'
        LCD37x7->read_offset_array[5]=26;    
    }
    if(EE_Read(EEPROM_CAL_READ_OFFSET+6,1,&LCD37x7->read_offset_array[6],0,1)) LCD37x7->read_offset_array[6]=1;//0=positive 1=negative
    if(EE_Read(EEPROM_CAL_READ_SCALE,7,&LCD37x7->read_scale_array[0],26,35))
    {
        //01.000
        LCD37x7->read_scale_array[0]=26;
        LCD37x7->read_scale_array[1]=27;
        LCD37x7->read_scale_array[2]=26;//'.'
        LCD37x7->read_scale_array[3]=26;
        LCD37x7->read_scale_array[4]=26;
        LCD37x7->read_scale_array[5]=26;
        LCD37x7->read_scale_array[6]=26;
    }
    update_read_off_scale(LCD37x7);
    
    while (1) {
        if (LCD37x7->goto_sleep == 0)
        {

        } 
        else
        {            // Prepare for sleep

            SleepInit();
            lcd_set_canvas(LCD37x7);
            // RTCC alarm every minute
            ALCFGRPTbits.AMASK = 2;

            Sleep();
        }
    }
    return 0;
}
void calibration_offset(lcd *p,unsigned char serial)
{
    if(serial==1)
    {
        p->offset=-0.28;
        //serial number=3000180200001
        p->offset_array[0]=26;
        p->offset_array[1]=26;
        p->offset_array[2]=26;//'.'
        p->offset_array[3]=28;
        p->offset_array[4]=34;
        p->offset_array[5]=26;
        p->offset_array[6]=1;//0=positive 1=negative
    }
    if(serial==2)
    {
        p->offset=-0.278;
        //serial number=3000180200002
        p->offset_array[0]=26;
        p->offset_array[1]=26;
        p->offset_array[2]=26;//'.'
        p->offset_array[3]=28;
        p->offset_array[4]=33;
        p->offset_array[5]=34;
        p->offset_array[6]=1;//0=positive 1=negative
    }
    else
    {
        p->offset=-0.28;
        //serial number=3000180200003
        p->offset_array[0]=26;
        p->offset_array[1]=26;
        p->offset_array[2]=26;//'.'
        p->offset_array[3]=28;
        p->offset_array[4]=34;
        p->offset_array[5]=26;
        p->offset_array[6]=1;//0=positive 1=negative
    }
}
void calibration_scale(lcd *p,unsigned char serial)
{
    if(serial==1)
    {
        p->scale=1.46029;
        //serial number=3000180200001
        p->scale_array[0]=27;
        p->scale_array[1]=26;//'.'
        p->scale_array[2]=30;
        p->scale_array[3]=32;
        p->scale_array[4]=26;
        p->scale_array[5]=28;
        p->scale_array[6]=35;
    }
    if(serial==2)
    {
        p->scale=1.4579;
        //serial number=3000180200002
        p->scale_array[0]=27;
        p->scale_array[1]=26;//'.'
        p->scale_array[2]=30;
        p->scale_array[3]=31;
        p->scale_array[4]=33;
        p->scale_array[5]=35;
        p->scale_array[6]=26;
    }

  
    else
    {
        p->scale=1.454;
        //serial number=3000180200003
        p->scale_array[0]=27;
        p->scale_array[1]=26;//'.'
        p->scale_array[2]=30;
        p->scale_array[3]=31;
        p->scale_array[4]=30;
        p->scale_array[5]=26;
        p->scale_array[6]=26;   
    }
}
void DisplayInit(lcd *p)
{
    calibration_offset(LCD37x7,SN);
    calibration_scale(LCD37x7,SN);
    int i;
    lcd_clear_screen(p);
    p->menu_state = RUN;
    p->Fosc = FOSC;
    p->goto_sleep = 0;
    segInit();
    p->button_count=0;
    p->counter=0;
    p->decimal=3;
    p->enterButton=false;
    p->upButton=false;
    p->downButton=false;
    p->menuButton=false;
    p->read_offset=0;
    p->read_scale=1;
    p->graph_offset=0;
    p->graph_scale=1;
    p->min=4;
    p->max=20;
    p->menu=1;
    for(i=0;i<=DIS_MAX_END;i++)
    {
        p->seg[i]=0;
    }
    p->password[0] =15;//'P'
    p->password[1] =0;//'A'
    p->password[2] =18;//'S'
    p->password[3] =18;//'S'
    p->password[4] =SEG14_SIZE-1;//' '
    p->password[5] =SEG14_SIZE-1;//' '
    
    //menu units
    p->units[0] =SEG14_SIZE-1;//'T'
    p->units[1] =SEG14_SIZE-1;//'E'
    p->units[2] =SEG14_SIZE-1;//'M'
    p->units[3] =SEG14_SIZE-1;//'P'
    p->units[4] =12;//M
    p->units[5] =0;//A

    //min
    p->seg[DIS_MIN_START]=5;//blank
    p->seg[DIS_MIN_START+1]=1;//blank
    p->seg[DIS_MIN_START+2]=1;//1
    p->seg[DIS_MIN_START+3]=1;//0

    //max
    p->seg[DIS_MAX_START]=3;//blank
    p->seg[DIS_MAX_START+1]=1;//1
    p->seg[DIS_MAX_START+2]=1;//0
    p->seg[DIS_MAX_START+3]=1;//0
    p->seg[DIS_MAX_START+4]=1;//0
    
    //
    p->seg[DIS_MENU_START]=SEG14_SIZE-1;//blank
    p->seg[DIS_MENU_START-1]=SEG14_SIZE-1;//1
    p->seg[DIS_MENU_START-2]=SEG14_SIZE-1;//0
    p->seg[DIS_MENU_START-3]=SEG14_SIZE-1;//0
    p->seg[DIS_MENU_START-4]=12;//0
    p->seg[DIS_MENU_START-5]=0;//0


        
        //1
        p->read_scale_array[0]=26;
        p->read_scale_array[1]=27;//1
        p->read_scale_array[2]=26;//'.'
        p->read_scale_array[3]=26;
        p->read_scale_array[4]=26;
        p->read_scale_array[5]=26;
        p->read_scale_array[6]=26;
     
        //0
        LCD37x7->read_offset_array[0]=26;
        LCD37x7->read_offset_array[1]=26;
        LCD37x7->read_offset_array[2]=26;
        LCD37x7->read_offset_array[3]=26;
        LCD37x7->read_offset_array[4]=26;//'.'
        LCD37x7->read_offset_array[5]=26;
        LCD37x7->read_offset_array[6]=0;////0=positive 1=negative
}
void LCDInit() {
    // Initialize LCD: no charge pump, 8 common drivers
    LCDPSbits.WFT = 0; // Type A waveform
    LCDPSbits.LP = 2; // LCD Prescaller 1:3
    LCDCONbits.LMUX = 0x03; // 8 commons, 1/3 Bias
    //segInit();
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
    LCDREFbits.LCDCST = 0; // Contrast is 2/7ths of maximum
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
  
}

void PowerManagedModulesInit() 
{   
    
        _TRISE5 =1;
        _TRISE6 =1;
        _TRISE7 =1;
        _TRISG7 =1;
        
        _ANSE5 = 0;
        _ANSE6 = 0;
        _ANSE7 = 0;
        _ANSG7 = 0;
        
        _CN63IE=1;//enter
        _CN64IE=1;//up
        _CN65IE=1;//down
        _CN9IE=1;//on/off
        
        _CNIF=0;// set CN interrupt flag to 0
        _CNIE=1;
        Nop();
        
    // Initialize Timer 1
        T1CONbits.TIECS = 0;
        T1CONbits.TCKPS = 3;
        T1CONbits.TON = 1;        
        PR1 = 75;
        _T1IF = 0;
        _T1IE = 1;

        // Initialize SD ADC
      ADC_SD_Init();
      
//        // Initialize Timer 5: refresh rate for SD_ADC reading and temperature reading
        T5CONbits.TON = 1;
        T5CONbits.TCKPS = 0b10; // prescale divisor of 64
        PR5 = 10000;
        IPC7bits.T5IP = 1;      // set to a low priority to allow ADC to read
        _T5IF = 0;
        _T5IE = 1;

}

void SleepInit() {

    // Disable interrupts
        _T1IE = 0;
        _T5IE = 0;
      
    //
    // Modules power down
        _T1MD = 1;
        _T5MD = 1;
        _LCDMD = 0; // Keep LCD on during sleep
        _RTCCMD = 0; // Keep RTCC on during sleep
        _SDADMD = 1;
        _ADC1MD = 1;
        _CTMUMD = 1;

    // RTCC alarm every minute
        ALCFGRPTbits.AMASK = 2;

    // Save the current screen to restore
        previousMenu = LCD37x7->menu_state;
        previousSubMenu = LCD37x7->submenu_state;
}

void WakeupInit() {
    // Modules power on
        _T1MD = 0;
        _T5MD = 0;
        _SDADMD = 0;
        _ADC1MD = 0;
        _CTMUMD = 0;

       // CalSineTable();                 // reload the sine table RAM
        PowerManagedModulesInit();      // recalibrate the ADC

    // Jump to previous screen
        LCD37x7->menu_state = previousMenu;
        LCD37x7->submenu_state = previousSubMenu;

    // RTCC alarm every half second
        ALCFGRPTbits.AMASK = 0;
}


void demoMenu(void) {
LCD37x7->menu_state_max = 9;
    LCD37x7->counter++;
    if (LCD37x7->counter>254)
    {
        LCD37x7->counter=0;
    }
    LCD37x7->button_count++;
    if (LCD37x7->button_count>254)
    {
        LCD37x7->button_count=0;
    }

   ///////////////////////////////Menu code//////////////////////////////////
    switch (LCD37x7->menu_state) {
        case MENU:
            //ENTER button must be pressed to continue
            lcd_clear_screen(LCD37x7);
            if(LCD37x7->menu==1) lcd_print_menu(LCD37x7,"GRAPH");
            else if(LCD37x7->menu==2) lcd_print_menu(LCD37x7,"UNITS");
            else if(LCD37x7->menu==3) lcd_print_menu(LCD37x7,"DECML");
            //else if(LCD37x7->menu==4) lcd_print_menu(LCD37x7,"PASWRD");
            else if(LCD37x7->menu==4) lcd_print_menu(LCD37x7,"DISPRD");
            else if(LCD37x7->menu==5) lcd_print_menu(LCD37x7,"CALIB");
            else if(LCD37x7->menu==6) lcd_print_menu(LCD37x7,"RESET");
            else if(LCD37x7->menu==7) lcd_print_menu(LCD37x7,"EXIT");
            
            if(LCD37x7->upButton)
            {
              if(LCD37x7->menu==1) {LCD37x7->menu=8;}
              LCD37x7->menu--;
              LCD37x7->upButton=false;
            }
            //if down button is pressed.
            if(LCD37x7->downButton)
            {
              LCD37x7->menu++;
              if(LCD37x7->menu==8) {LCD37x7->menu=1;}
              LCD37x7->downButton=false;
            }
            if(LCD37x7->enterButton)
            {
            lcd_clear_screen(LCD37x7);
            //  LCD37x7->canvas[LCD37x7->displayCursor].bit3=1;
              LCD37x7->menu_state=LCD37x7->menu;
              LCD37x7->submenu_state=0;
              LCD37x7->enterButton=false;
            }
            break;
            
        case GRAPH:
            LCD37x7->submenu_state_max=5;
            //show decimals
            LCD37x7->canvas[LCD37x7->decimal+10].bit7=1;//min display
            if(LCD37x7->decimal==2)
            {
               LCD37x7->canvas[LCD37x7->decimal+11].bit7=1;//min display 
            }
            LCD37x7->canvas[LCD37x7->decimal+15].bit7=1;
            switch(LCD37x7->submenu_state){
                //SET MIN DISPLAY
                case 0:
                    LCD37x7->displayCursor=DIS_MIN_START;//set cursor to start on the first min 7seg.
                    LCD37x7->submenu_state=1;
                    lcd_clear_screen(LCD37x7);
                    display_min_max(LCD37x7);
                    LCD37x7->canvas[26].bit0=0x1;//turn on the min display outline.
                    break;
                case 1:
                lcd_print_menu(LCD37x7,"MIN");
                lcd_blink(LCD37x7,LCD37x7->displayCursor);
                //if up button is pressed.
                if(LCD37x7->upButton)
                {
                  lcd_7seg_scroll(LCD37x7,1,LCD37x7->displayCursor);
                  LCD37x7->upButton=false;
                }
                //if down button is pressed.
                if(LCD37x7->downButton)
                {
                  lcd_7seg_scroll(LCD37x7,0,LCD37x7->displayCursor);
                  LCD37x7->downButton=false;
                }
                //if enter button is pressed.
                if(LCD37x7->enterButton)
                {
                  //when the min display is set goto setting the max display
                  if(LCD37x7->displayCursor==DIS_MIN_END)
                  {
                      LCD37x7->submenu_state=2;
                      LCD37x7->enterButton=false;
                  }
                  display_7seg_character(LCD37x7,LCD37x7->displayCursor);
                  LCD37x7->displayCursor++;
                  //set seg d to be on to show where the cursor is.
                 // LCD37x7->canvas[LCD37x7->displayCursor].bit3=1;
                  LCD37x7->enterButton=false;
                }
                if(LCD37x7->menuButton)
                {
                    LCD37x7->menu_state=MENU;
                    LCD37x7->menuButton=false;
                }
                break;           
                //Set Max Display
                case 2:
                    lcd_print_menu(LCD37x7,"MAX");
                    lcd_blink(LCD37x7,LCD37x7->displayCursor);
                    LCD37x7->canvas[26].bit1=0x1;//turn on the max display outline.
                    //if up button is pressed.
                    if(LCD37x7->upButton)
                    {
                      lcd_7seg_scroll(LCD37x7,1,LCD37x7->displayCursor);
                      LCD37x7->upButton=false;
                    }
                    //if down button is pressed.
                    if(LCD37x7->downButton)
                    {
                      lcd_7seg_scroll(LCD37x7,0,LCD37x7->displayCursor);
                      LCD37x7->downButton=false;
                    }
                    //if enter button is pressed.
                    if(LCD37x7->enterButton)
                    {
                        if(LCD37x7->displayCursor==DIS_MAX_END)
                        {
                            LCD37x7->submenu_state=0;
                            LCD37x7->enterButton=false;
                            update_min_max(LCD37x7);//updates min and max in LCD37x7
                            LCD37x7->menu_state=MENU;
                            EE_Write(EEPROM_MIN,4,&LCD37x7->seg[DIS_MIN_START]);
                            EE_Write(EEPROM_MAX,5,&LCD37x7->seg[DIS_MAX_START]);
                            break;
                        }
                        display_7seg_character(LCD37x7,LCD37x7->displayCursor);
                        LCD37x7->displayCursor++;
                        //set seg d to be on to show where the cursor is.
                       // LCD37x7->canvas[LCD37x7->displayCursor].bit3=1;
                        LCD37x7->enterButton=false;
                    }
                    if(LCD37x7->menuButton)
                    {
                        LCD37x7->menu_state=MENU;
                        LCD37x7->menuButton=false;
                    }
                break;
            }
            break;
        case UNITS:
            LCD37x7->submenu_state_max=2;
            switch(LCD37x7->submenu_state){
                case 0: 
                    LCD37x7->displayCursor=DIS_MENU_START;
                    LCD37x7->submenu_state=1;
                    lcd_clear_screen(LCD37x7);
                    display_menu_units(LCD37x7);
                    break;
                case 1:
                    lcd_blink(LCD37x7,LCD37x7->displayCursor);
                    if(LCD37x7->upButton)
                    {
                      lcd_14seg_scroll(LCD37x7,1,LCD37x7->displayCursor);
                      LCD37x7->upButton=false;
                    }
                    if(LCD37x7->downButton)
                    {
                      lcd_14seg_scroll(LCD37x7,0,LCD37x7->displayCursor);
                      LCD37x7->downButton=false;
                    }
                    if(LCD37x7->enterButton)
                    {
                        if(LCD37x7->displayCursor==DIS_MENU_END)
                        {
                           LCD37x7->menu_state=MENU; 
                           LCD37x7->units[0]=LCD37x7->seg[11];
                           LCD37x7->units[1]=LCD37x7->seg[10];
                           LCD37x7->units[2]=LCD37x7->seg[9];
                           LCD37x7->units[3]=LCD37x7->seg[8];
                           LCD37x7->units[4]=LCD37x7->seg[7];
                           LCD37x7->units[5]=LCD37x7->seg[6];
                           display_menu_units(LCD37x7);
                           EE_Write(EEPROM_UNITS,6,&LCD37x7->units[0]);
                        }
                      display_menu_character(LCD37x7,LCD37x7->displayCursor);
                      LCD37x7->displayCursor--;
                      LCD37x7->enterButton=false;
                    }
                    if(LCD37x7->menuButton)
                    {
                        LCD37x7->menu_state=MENU;
                        LCD37x7->menuButton=false;
                    }
                    break;
            }
            break;
            // READ sets the decimal place when reading in the ADC value
        case READ:
            LCD37x7->submenu_state_max=2;
            switch(LCD37x7->submenu_state)
            {
                case 0:
                    LCD37x7->canvas[LCD37x7->decimal].bit7=1;
                    LCD37x7->decimal_temp=LCD37x7->decimal;
                    LCD37x7->submenu_state=1;
                    break;
                case 1:
                    lcd_update_sensor_data(LCD37x7,adc16_reading_ch0);//display adc value
                    if(LCD37x7->upButton)
                    {
                      lcd_7seg_decimal(LCD37x7,true);
                      LCD37x7->upButton=false;
                    }
                    //if down button is pressed.
                    if(LCD37x7->downButton)
                    {
                      lcd_7seg_decimal(LCD37x7,false);
                      LCD37x7->downButton=false;
                    }
                    //if enter button is pressed.
                    if(LCD37x7->enterButton)
                    {
                        lcd_print_number_display(LCD37x7,"     ");
                        //update_decimal(LCD37x7);
                        LCD37x7->menu_state=MENU;
                        LCD37x7->submenu_state=0;
                        LCD37x7->enterButton=false;
                        //update_Flash(LCD37x7);
                        EE_Write(EEPROM_DECIMAL,1,&LCD37x7->decimal);
                        if(LCD37x7->decimal!=LCD37x7->decimal_temp)
                        {
                            LCD37x7->menu_state=GRAPH;
                        }
                    }
                    if(LCD37x7->menuButton)
                        {
                            LCD37x7->menu_state=MENU;
                            LCD37x7->menuButton=false;
                        }
            }
            break;
//        case PASSWORD:
//            LCD37x7->submenu_state_max=4;
//            switch(LCD37x7->submenu_state)
//            {
//                case 0:
//                    LCD37x7->displayCursor=11;
//                    LCD37x7->submenu_state=1;
//                    lcd_clear_screen(LCD37x7);
//                    //set to be blank
//                    LCD37x7->seg[11]=SEG14_SIZE-1;
//                    LCD37x7->seg[10]=SEG14_SIZE-1;
//                    LCD37x7->seg[9]=SEG14_SIZE-1;
//                    LCD37x7->seg[8]=SEG14_SIZE-1;
//                    LCD37x7->seg[7]=SEG14_SIZE-1;
//                    LCD37x7->seg[6]=SEG14_SIZE-1;
//                    break;
//                case 1:
//                    lcd_blink(LCD37x7,LCD37x7->displayCursor);
//                    if(LCD37x7->upButton)
//                    {
//                      lcd_14seg_scroll(LCD37x7,true,LCD37x7->displayCursor);
//                      LCD37x7->upButton=false;
//                    }
//                    //if down button is pressed.
//                    if(LCD37x7->downButton)
//                    {
//                      lcd_14seg_scroll(LCD37x7,false,LCD37x7->displayCursor);
//                      LCD37x7->downButton=false;
//                    }
//                    //if enter button is pressed.
//                    if(LCD37x7->enterButton)
//                    {
//                        if(LCD37x7->displayCursor==DIS_MENU_END)
//                        {
//                            if(cmp_password(LCD37x7)==1)
//                            {
//                                LCD37x7->submenu_state=2;
//                                lcd_clear_screen(LCD37x7);
//                                lcd_print_menu(LCD37x7,"OK");
//                            }
//                            else
//                            {
//                                LCD37x7->submenu_state=0;
//                                LCD37x7->displayCursor=11;
//                            }
//                            LCD37x7->enterButton=false;
//                        }
//                        display_menu_character(LCD37x7,LCD37x7->displayCursor);
//                        LCD37x7->displayCursor--;
//                        LCD37x7->enterButton=false;
//                    }
//                    if(LCD37x7->menuButton)
//                    {
//                        LCD37x7->menu_state=MENU;
//                        LCD37x7->menuButton=false;
//                    }
//                    break;
//                case 2:
//                    //if enter button is pressed.
//                    if(LCD37x7->enterButton)
//                    {
//                        LCD37x7->submenu_state=3;
//                        //set to be blank
//                        LCD37x7->seg[11]=SEG14_SIZE-1;
//                        LCD37x7->seg[10]=SEG14_SIZE-1;
//                        LCD37x7->seg[9]=SEG14_SIZE-1;
//                        LCD37x7->seg[8]=SEG14_SIZE-1;
//                        LCD37x7->seg[7]=SEG14_SIZE-1;
//                        LCD37x7->seg[6]=SEG14_SIZE-1;
//                        LCD37x7->displayCursor=11;
//                        lcd_clear_screen(LCD37x7);
//                        LCD37x7->enterButton=false;
//                    }
//                    break;
//                case 3:
//                    lcd_blink(LCD37x7,LCD37x7->displayCursor);
//                    if(LCD37x7->upButton)
//                    {
//                      lcd_14seg_scroll(LCD37x7,true,LCD37x7->displayCursor);
//                      LCD37x7->upButton=false;
//                    }
//                    //if down button is pressed.
//                    if(LCD37x7->downButton)
//                    {
//                      lcd_14seg_scroll(LCD37x7,false,LCD37x7->displayCursor);
//                      LCD37x7->downButton=false;
//                    }
//                    //if enter button is pressed.
//                    if(LCD37x7->enterButton)
//                    {
//                        if(LCD37x7->displayCursor==DIS_MENU_END)
//                        {
//                            LCD37x7->menu_state=MENU;
//                            LCD37x7->submenu_state=0;
//                            LCD37x7->enterButton=false;
//                            update_password(LCD37x7);
//                            update_Flash();
//                        }
//                        display_menu_character(LCD37x7,LCD37x7->displayCursor);
//                        LCD37x7->displayCursor--;
//                        LCD37x7->enterButton=false;
//                    }
//                    break;
//                    
//            }
//            break;
        case DISPLAY_READ:
            LCD37x7->submenu_state_max=4;
            switch(LCD37x7->submenu_state)
            {
                case 0:
                    lcd_print_menu(LCD37x7,"OFFSET");
                    if(LCD37x7->enterButton)
                    {
                        LCD37x7->submenu_state=1;
                        LCD37x7->displayCursor=11;
                        lcd_clear_screen(LCD37x7);
                        lcd_update_read_offset_data(LCD37x7);//display offset value
                        LCD37x7->enterButton=false;
                    }
                break;
                case 1:
                    LCD37x7->canvas[6].bit14=1;//number display decimal
                    lcd_blink(LCD37x7,LCD37x7->displayCursor);
                    lcd_update_sensor_data(LCD37x7,adc16_reading_ch0);//display adc value
                    //if up button is pressed.
                    if(LCD37x7->upButton)
                    {
                      LCD37x7->read_offset=LCD37x7->read_offset*-1;
                      lcd_update_read_offset_data(LCD37x7);//display offset value
                      LCD37x7->upButton=false;
                    }
                    //if down button is pressed.
                    if(LCD37x7->downButton)
                    {
                      LCD37x7->read_offset=LCD37x7->read_offset*-1;
                      lcd_update_read_offset_data(LCD37x7);//display offset value
                      LCD37x7->downButton=false;
                    }
                    //if enter button is pressed.
                    if(LCD37x7->enterButton)
                    {
                        LCD37x7->submenu_state=2;
                        LCD37x7->displayCursor=10;
                        lcd_update_read_offset_data(LCD37x7);//display offset value
                        LCD37x7->enterButton=false;
                    }
                    break;
                case 2:
                    LCD37x7->canvas[6].bit14=1;//number display decimal
                    lcd_blink(LCD37x7,LCD37x7->displayCursor);
                    lcd_update_sensor_data(LCD37x7,adc16_reading_ch0);//display adc value
                    //if up button is pressed.
                    if(LCD37x7->upButton)
                    {
                      lcd_14seg_scroll_read_scale_offset(LCD37x7,1,LCD37x7->displayCursor,0);//increase value of offset
                      LCD37x7->upButton=false;
                    }
                    //if down button is pressed.
                    if(LCD37x7->downButton)
                    {
                      lcd_14seg_scroll_read_scale_offset(LCD37x7,0,LCD37x7->displayCursor,0);//decrease value of offset
                      LCD37x7->downButton=false;
                    }
                    //if enter button is pressed.
                    if(LCD37x7->enterButton)
                    {
                        //lcd_update_read_offset_data(LCD37x7);//display offset value
                        display_menu_character(LCD37x7,LCD37x7->displayCursor);
                        if(LCD37x7->displayCursor==DIS_MENU_END)
                        {
                         LCD37x7->submenu_state=3;
                         LCD37x7->enterButton=false;
                         update_read_offset_array(LCD37x7);
                         EE_Write(EEPROM_CAL_READ_OFFSET,7,&LCD37x7->read_offset_array[0]);
                         lcd_clear_screen(LCD37x7);
                        }
                        
                        LCD37x7->displayCursor--;
                        LCD37x7->enterButton=false;
                    }
                    if(LCD37x7->menuButton)
                    {
                        LCD37x7->menu_state=MENU;
                        LCD37x7->menuButton=false;
                    }
                break;
                case 3:
                    lcd_print_menu(LCD37x7,"SCALE");
                    if(LCD37x7->enterButton)
                    {
                        LCD37x7->submenu_state=4;
                        //set to be blank
                        LCD37x7->displayCursor=11;
                        lcd_clear_screen(LCD37x7);
                        lcd_update_read_scale_data(LCD37x7);//display scale value
                        LCD37x7->enterButton=false;
                    }
                break;
                case 4:
                    LCD37x7->canvas[9].bit14=1;//number display decimal
                    lcd_blink(LCD37x7,LCD37x7->displayCursor);
                    lcd_update_sensor_data(LCD37x7,adc16_reading_ch0);//display adc value
                    //if up button is pressed.
                    if(LCD37x7->upButton)
                    {
                      lcd_14seg_scroll_read_scale_offset(LCD37x7,1,LCD37x7->displayCursor,1);
                      LCD37x7->upButton=false;
                    }
                    //if down button is pressed.
                    if(LCD37x7->downButton)
                    {
                      lcd_14seg_scroll_read_scale_offset(LCD37x7,0,LCD37x7->displayCursor,1);
                      LCD37x7->downButton=false;
                    }
                    //if enter button is pressed.
                    if(LCD37x7->enterButton)
                    {
                      if(LCD37x7->displayCursor==DIS_MENU_END)
                      {
                         LCD37x7->menu_state=MENU;
                         LCD37x7->submenu_state=0;
                         LCD37x7->enterButton=false;
                         LCD37x7->read_scale_array[0]=LCD37x7->seg[11];
                         LCD37x7->read_scale_array[1]=LCD37x7->seg[10];
                         LCD37x7->read_scale_array[2]=26;//0 but will change to  '.'
                         LCD37x7->read_scale_array[3]=LCD37x7->seg[9];
                         LCD37x7->read_scale_array[4]=LCD37x7->seg[8];
                         LCD37x7->read_scale_array[5]=LCD37x7->seg[7];
                         LCD37x7->read_scale_array[6]=LCD37x7->seg[6];
                         EE_Write(EEPROM_CAL_READ_SCALE,7,&LCD37x7->read_scale_array[0]);
                      }
                      display_menu_character(LCD37x7,LCD37x7->displayCursor);
                      LCD37x7->displayCursor--;
                      LCD37x7->enterButton=false;
                    }
                    if(LCD37x7->menuButton)
                    {
                        LCD37x7->menu_state=MENU;
                        LCD37x7->menuButton=false;
                    }
                break;
            }
            break;
        case CALIBRATION:
            LCD37x7->submenu_state_max=4;
            switch(LCD37x7->submenu_state)
            {
                case 0:
                    lcd_print_menu(LCD37x7,"OFFSET");
                    if(LCD37x7->enterButton)
                    {
                        LCD37x7->submenu_state=1;
                        LCD37x7->displayCursor=11;
                        lcd_clear_screen(LCD37x7);
                        lcd_update_offset_data(LCD37x7);//display offset value
                        LCD37x7->enterButton=false;
                    }
                    
                break;
                case 1:
                    LCD37x7->canvas[8].bit14=1;//number display decimal
                    lcd_blink(LCD37x7,LCD37x7->displayCursor);
                    lcd_update_sensor_data(LCD37x7,adc16_reading_ch0);//display adc value
                    //if up button is pressed.
                    if(LCD37x7->upButton)
                    {
                      LCD37x7->offset=LCD37x7->offset*-1;
                      lcd_update_offset_data(LCD37x7);//display offset value
                      LCD37x7->upButton=false;
                    }
                    //if down button is pressed.
                    if(LCD37x7->downButton)
                    {
                      LCD37x7->offset=LCD37x7->offset*-1;
                      lcd_update_offset_data(LCD37x7);//display offset value
                      LCD37x7->downButton=false;
                    }
                    //if enter button is pressed.
                    if(LCD37x7->enterButton)
                    {
                        LCD37x7->submenu_state=2;
                        LCD37x7->displayCursor=10;
                        lcd_update_offset_data(LCD37x7);//display scale value
                        LCD37x7->enterButton=false;
                    }
                    break;
                case 2:
                    LCD37x7->canvas[8].bit14=1;//number display decimal
                    lcd_blink(LCD37x7,LCD37x7->displayCursor);
                    lcd_update_sensor_data(LCD37x7,adc16_reading_ch0);//display adc value
                    //if up button is pressed.
                    if(LCD37x7->upButton)
                    {
                      lcd_14seg_scroll_scale_offset(LCD37x7,1,LCD37x7->displayCursor,0);//increase value of offset
                      LCD37x7->upButton=false;
                    }
                    //if down button is pressed.
                    if(LCD37x7->downButton)
                    {
                      lcd_14seg_scroll_scale_offset(LCD37x7,0,LCD37x7->displayCursor,0);//decrease value of offset
                      LCD37x7->downButton=false;
                    }
                    //if enter button is pressed.
                    if(LCD37x7->enterButton)
                    {
                        lcd_update_offset_data(LCD37x7);//display scale value
                        if(LCD37x7->displayCursor==DIS_MENU_END)
                      {
                         LCD37x7->submenu_state=3;
                         LCD37x7->enterButton=false;
                         //LCD37x7->offset_array[0]=LCD37x7->seg[11];
                         LCD37x7->offset_array[0]=LCD37x7->seg[10];
                         LCD37x7->offset_array[1]=LCD37x7->seg[9];
                         LCD37x7->offset_array[2]=26;//0 but will change to  '.'
                         LCD37x7->offset_array[3]=LCD37x7->seg[8];
                         LCD37x7->offset_array[4]=LCD37x7->seg[7];
                         LCD37x7->offset_array[5]=LCD37x7->seg[6];
                         if(LCD37x7->offset<0)LCD37x7->offset_array[6]=1;
                         else LCD37x7->offset_array[6]=0;
                         EE_Write(EEPROM_CAL_OFFSET,7,&LCD37x7->offset_array[0]);
                         lcd_clear_screen(LCD37x7);
                      }
                        LCD37x7->displayCursor--;
                        LCD37x7->enterButton=false;
                    }
                    if(LCD37x7->menuButton)
                    {
                        LCD37x7->menu_state=MENU;
                        LCD37x7->menuButton=false;
                    }
                break;
                case 3:
                    lcd_print_menu(LCD37x7,"SCALE");
                    if(LCD37x7->enterButton)
                    {
                        LCD37x7->submenu_state=4;
                        //set to be blank
                        LCD37x7->displayCursor=11;
                        lcd_clear_screen(LCD37x7);
                        lcd_update_scale_data(LCD37x7);//display scale value
                        LCD37x7->enterButton=false;
                    }
                break;
                case 4:
                    LCD37x7->canvas[10].bit14=1;//number display decimal
                    lcd_blink(LCD37x7,LCD37x7->displayCursor);
                    lcd_update_sensor_data(LCD37x7,adc16_reading_ch0);//display adc value
                    //if up button is pressed.
                    if(LCD37x7->upButton)
                    {
                      lcd_14seg_scroll_scale_offset(LCD37x7,1,LCD37x7->displayCursor,1);
                      LCD37x7->upButton=false;
                    }
                    //if down button is pressed.
                    if(LCD37x7->downButton)
                    {
                      lcd_14seg_scroll_scale_offset(LCD37x7,0,LCD37x7->displayCursor,1);
                      LCD37x7->downButton=false;
                    }
                    //if enter button is pressed.
                    if(LCD37x7->enterButton)
                    {
                      if(LCD37x7->displayCursor==DIS_MENU_END)
                      {
                         LCD37x7->menu_state=MENU;
                         LCD37x7->submenu_state=0;
                         LCD37x7->enterButton=false;
                         LCD37x7->scale_array[0]=LCD37x7->seg[11];
                         LCD37x7->scale_array[1]=26;//0 but will change to  '.'
                         LCD37x7->scale_array[2]=LCD37x7->seg[10];
                         LCD37x7->scale_array[3]=LCD37x7->seg[9];
                         LCD37x7->scale_array[4]=LCD37x7->seg[8];
                         LCD37x7->scale_array[5]=LCD37x7->seg[7];
                         LCD37x7->scale_array[6]=LCD37x7->seg[6];
                         EE_Write(EEPROM_CAL_SCALE,7,&LCD37x7->scale_array[0]);
                      }
                      display_menu_character(LCD37x7,LCD37x7->displayCursor);
                      LCD37x7->displayCursor--;
                      LCD37x7->enterButton=false;
                    }
                    if(LCD37x7->menuButton)
                    {
                        LCD37x7->menu_state=MENU;
                        LCD37x7->menuButton=false;
                    }
                break;
            }
            break;
        case RESET:
             lcd_print_menu(LCD37x7,"WAIT");
             lcd_set_canvas(LCD37x7); 
             DisplayInit(LCD37x7);
             EE_Write(EEPROM_MIN,4,&LCD37x7->seg[DIS_MIN_START]);
             EE_Write(EEPROM_MAX,5,&LCD37x7->seg[DIS_MAX_START]);            
             EE_Write(EEPROM_UNITS,6,&LCD37x7->units[0]);
             EE_Write(EEPROM_DECIMAL,1,&LCD37x7->decimal);
             EE_Write(EEPROM_CAL_OFFSET,7,&LCD37x7->offset_array[0]);
             EE_Write(EEPROM_CAL_SCALE,7,&LCD37x7->scale_array[0]); 
             EE_Write(EEPROM_CAL_READ_OFFSET,7,&LCD37x7->read_offset_array[0]);
             EE_Write(EEPROM_CAL_READ_SCALE,7,&LCD37x7->read_scale_array[0]);
            LCD37x7->menu_state=MENU;
            break;
        case EXIT:
            LCD37x7->menu_state=RUN;
            break;
        case RUN:
            LCD37x7->submenu_state_max=2;
            run_init(LCD37x7);
            lcd_update_sensor_data(LCD37x7,adc16_reading_ch0);
            lcd_meter(LCD37x7,((adc16_reading_ch0 - LCD37x7->min) * 100) / (LCD37x7->max - LCD37x7->min));
            if(LCD37x7->menuButton)
            {
               LCD37x7->menu_state=MENU; 
               LCD37x7->submenu_state=0;
               LCD37x7->menuButton=false;
            }
            break;
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

void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void) {
    
    demoMenu();
    lcd_set_canvas(LCD37x7);   
    _T1IF = 0;
}

// Timer 5 is the SD_ADC and temperature update rate 
double x2;
double x3;
double x4;
const double C0 = 1.39074;
const double C1 = .3698801 ;
const double C2 = 0.09540517;
const double C3 = -.00649867;
const double C4 = .0001556715;
double temp;
void __attribute__((__interrupt__, __auto_psv__)) _T5Interrupt(void) {
    
    adc16_reading_ch0 = ADC_SD_GetCalibratedResult()*-1;
    //adc16_reading_ch0 = (adc16_reading_ch0 * 0.001007095*(LCD37x7->scale*LCD37x7->graph_scale))+LCD37x7->offset+LCD37x7->graph_offset; 
    adc16_reading_ch0 = ((adc16_reading_ch0 * (3.0/65535)*16)*(LCD37x7->scale))+LCD37x7->offset; 
    x2=adc16_reading_ch0*adc16_reading_ch0;
    x3=x2*adc16_reading_ch0;
    x4=x3*adc16_reading_ch0;
    //curvefit to 4th order polynomial
    temp =  C4*x4 + C3*x3 + C2*x2 + C1*adc16_reading_ch0 +C0;
    adc16_reading_ch0 = (temp*LCD37x7->graph_scale*LCD37x7->read_scale)+LCD37x7->graph_offset+LCD37x7->read_offset;
    _T5IF = 0;
}

void __attribute__((__interrupt__, __auto_psv__)) _RTCCInterrupt(void) {

    _RTCIF = 0;
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
void __attribute__((__interrupt__, no_auto_psv)) _CNInterrupt(void) {
    char enter;
    char up;
    char down;
    char menu;
    up=_RE5;
    enter=_RE6;
    menu=_RE7;
    down=_RG7;

    if(LCD37x7->button_count>4)
    {
    //DOWN
    if(!_RE7)
    {
        LCD37x7->menuButton=true;
    }
    //UP
    else if(!_RE6)
    {
        LCD37x7->enterButton=true;
    }
    //ENTER
    else if(!_RE5)
    {
        LCD37x7->upButton=true;
    }
    //MENU
    else if(!_RG7)
    {
        LCD37x7->downButton=true;
    }
    LCD37x7->button_count=0;
    }
    //reset the flag
    _CNIF = 0; 
}