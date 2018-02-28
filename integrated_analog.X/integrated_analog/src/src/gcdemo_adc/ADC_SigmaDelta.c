/********************************************************************
 FileName:      ADC_SigmaDelta.c
 Dependencies:  See INCLUDES section
 Processor:	    PIC24FJ128GC010 Advanced Analog Microcontroller
 Hardware:      This code is based around the analog peripherals of the
                PIC24FJ128GC010 Family of advanced analog microcontrollers.
 Complier:      Microchip C30 Compiler (of at least v3.34)
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
  0.1   Initial version
  0.2   Revised 5/22/2013 by P. Schreiber

********************************************************************/

//Includes
#include <xc.h>
#include "ADC_SigmaDelta.h"

static signed long int offset;
static double SD_gain;

//------------------------------------------------------------------------------
//Description: Initializes the sigma delta ADC module and performs intitial
//measurements to establish gain and offset errors, so that they can be used
//later to get calibrated results.  This API should also be called first,
//prior to calling the other sigma delta ADC API functions.  The measurement
// channel is CH1 (only option in this demo, but can be expanded to include CH0).
//------------------------------------------------------------------------------
void ADC_SD_Init()
{
    unsigned int i;
    unsigned char count;
    
    signed long int maxValue;
    #define EXPECTED_MAX_VALUE  (double)32767 // 0x7FFF is full scale positive
   
    
    //Disable ADC while (re-)configuring it.
        SD1CON1 = 0x0000;

    //Setup Sigma Delta ADC Module, so that it is ready to perform gain and
    //offset measurements (so we get proper gain and offset calibration values to use later).

        SD1CON1bits.PWRLVL = 1; // High power mode
        SD1CON1bits.SDREFP = 0; // Positive Voltage Reference is SVDD
        SD1CON1bits.SDREFN = 0; // Negative Voltage Reference is SVSS
        SD1CON1bits.VOSCAL = 1; // Internal Offset Measurement Enable
        SD1CON1bits.DITHER = 1; // Low Dither, because using SVDD as reference
        SD1CON1bits.SDGAIN = 0; // Gain is 1:1

        SD1CON2bits.RNDRES = 2; // Round result to 16-bit
        SD1CON2bits.SDWM = 1;   // SDxRESH/SDxRESL updated on every Interrupt
        SD1CON2bits.SDINT = 3;  // Interrupt on every data output
        SD1CON2bits.CHOP = 3;   // Chopping should be enabled always

        SD1CON3bits.SDCH = 1;   // Channel 1 is used for this demo code
        SD1CON3bits.SDCS = 1;   // Clock Source is a 8 MHz FRC
        SD1CON3bits.SDOSR = 0;  // Oversampling Ratio (OSR) is 1024 (best quality)
        SD1CON3bits.SDDIV = 1;  // Input Clock Divider is 2 (SD ADC clock is 4MHz)


    //Enable the ADC module now that it is configured
        SD1CON1bits.SDON = 1;

    //Wait for a minimum of five interrupts to be generated.  Need to throw at least
    //the first four away when using interrupt every period option, since the
    //low pass SINC filter needs to be flushed with new data when we change
    //ADC channel or initialize the ADC.  If the external power supply control
    //loop for the SVDD/SVSS supplies will deviate slightly upon step function
    //increase in loading, it is recommended to wait for more than the minimum
    //samples, before using the data and considering it valid, so as to allow
    //the external power supply to settle.

    for(i = 0; i<6; i++)           // (value must be >= 5)
    {
        IFS6bits.SDA1IF = 0;            //Clear interrupt flag
        while(IFS6bits.SDA1IF == 0);    //Wait until hardware says we have a result ready.
        IFS6bits.SDA1IF = 0;            //Clear interrupt flag
    }
    
    	while(IFS6bits.SDA1IF == 0);    //Wait until hardware says we have a result ready.
        IFS6bits.SDA1IF = 0;            //Clear interrupt flag
        offset = (signed short int)SD1RESH;     // cast to accomodate the sign bit extend

    //Switch off offset measurement mode, now that we have the value
        SD1CON1bits.SDON = 0;
        SD1CON1bits.VOSCAL = 0;

    //Now reconfigure ADC so as to measure the SVdd through channel 1,
    //and compare it against the expected result (after offset correction).
    //Use the comparison to compute the optimum gain calibration factor, and
    //save the value, so that it may be used later.

        SD1CON3bits.SDCH = 3;           // point to the SVDD voltage
        SD1CON1bits.SDON = 1;           // SD_ADC back on to make next measurement

    // Wait for a minimum of five interrupts to be generated.
        for(count=0; count<6; count++)
    {
    //Clear interrupt flag.
        IFS6bits.SDA1IF = 0;
    //Wait for the result ready.
        while(IFS6bits.SDA1IF == 0);
    }
    // Save the maximum value to calculate the gain.
        maxValue = (signed short int) SD1RESH;    // must cast as signed as is declared unsigned in header
   
    // Calculate gain.
        SD_gain = EXPECTED_MAX_VALUE/((double)((signed long int)maxValue-offset));

    //Disable ADC while re-configuring it (for normal operation mode, now that
    //we have establish gain/offset calibration values for the ADC).
        SD1CON1bits.SDON = 0;    //Setup Sigma Delta ADC Module for normal reads on CH1
        SD1CON3bits.SDCH = 1;
        SD1CON1bits.SDON = 1;
    
    //Wait for SYNC filter to flush, so the next result the application tries 
    //to read will be valid
        for(count = 0; count < 6; count++)  //throw away first >=4 samples for SINC filter delay
    {
        while(IFS6bits.SDA1IF == 0);    //Wait until hardware says we have a result ready.
        IFS6bits.SDA1IF = 0;            //Clear interrupt flag
    }
    
    //The ADC is now ready, and the first sample of the newly selected channel
    //should be available in SD1RESH.
}    

//------------------------------------------------------------------------------
//Function: signed short int ADC_SD_GetCalibratedResult(BYTE Channel)
//Description: Does what is needed to get an ADC sample from the channel
//specified.  The value that is returned is the raw result generated by
//the hardware, and will therefore contain offset and gain error.
//------------------------------------------------------------------------------
signed short int ADC_SD_GetRawResult()
{
   
    //Wait for a new result
        IFS6bits.SDA1IF = 0;            //Clear interrupt flag
        while(IFS6bits.SDA1IF == 0);    //Wait until hardware says we have a result ready.

         return SD1RESH;
}    

//------------------------------------------------------------------------------
//Function: signed short int ADC_SD_GetCalibratedResult(BYTE Channel)
//Description: Does what is needed to get an ADC sample from the channel
//specified, and then compensate it for offset and gain.  The calibrated
//result is then returned.
//------------------------------------------------------------------------------
signed short int ADC_SD_GetCalibratedResult()
{
    signed long int OffsetCalibratedResult;
    double GainAndOffsetCorrectedResult;
    
    //Get the raw ADC result
        OffsetCalibratedResult = ADC_SD_GetRawResult();
    
    //Correct for offset error measured previously
        OffsetCalibratedResult -= offset;

    //Now compute the gain corrected result.  
        GainAndOffsetCorrectedResult = (double)OffsetCalibratedResult * (double) SD_gain;
    
    //Convert the double into a signed long int.    
        OffsetCalibratedResult = (signed long int)GainAndOffsetCorrectedResult;
    
    //Make sure the result fits in a signed short int.  If not, saturate it so it does.
        if(OffsetCalibratedResult > 32767)
        {
            OffsetCalibratedResult = 32767;
    }    
        else if(OffsetCalibratedResult < -32768)
    {
            OffsetCalibratedResult = -32768;
    }    
    
        return (signed short int)OffsetCalibratedResult;
}    

