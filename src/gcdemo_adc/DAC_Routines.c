/********************************************************************
 FileName:      DAC_Routines.c
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

********************************************************************/

#include <p24Fxxxx.h>
#include "./Analog/PIC24FJxxxGC0xx/DAC_Routines.h"
#include <math.h>


//------------------------------------------------------------------------------
//Function: void DAC1_UpdateOutput(unsigned int DACCode10Bit)
//Description: Initializes, enables, and updates the DAC1 module to output
//the 10-bit DAC code provided (DACCode10Bit).  The function assumes
//AVDD will be used as the reference, the output will be driven to the 
//RG9/AN49/DAC1 pin.  This function also executes blocking delay so as to
//allow the analog output to settle to the new value, before returning from 
//this function.
//------------------------------------------------------------------------------
void DAC1_UpdateOutput(unsigned int DACCode10Bit)
{
    unsigned int i;

    //Error check the input value to make sure it fits in the 10-bit DAC1DAT register
    if(DACCode10Bit > 1023)
        DACCode10Bit = 1023;
    
    DAC1DAT = DACCode10Bit;
    
    //Turn on and initialize the DAC module, if it isn't enabled already
    if(DAC1CONbits.DACEN == 0)
    {
        //Make sure RG9/DAC1 pin is configured for analog mode and is tri-stated
        ANSGbits.ANSG9 = 1;     //Configure RG9/AN49/DAC1 pin for analog mode
        TRISGbits.TRISG9 = 1;   //Make sure RG9/AN49/DAC1 pin is tri-stated

        DAC1CON = 0x1082;   //DAC continues in sleep, output to pin enabled, no triggers/update on DAC1DAT write, use AVDD as reference
        DAC1CONbits.DACEN = 1;      
        DAC1DAT = DACCode10Bit;
    }    
    
    //Execute some settling time for DAC analog output to reach the target value.
    for(i = 0; i < 10u; i++);   //~3.6us @ 32MHz
    //The DAC should now be at the expected level.
}    


//------------------------------------------------------------------------------
//Function: void DAC2_UpdateOutput(unsigned int DACCode10Bit)
//Description: Initializes, enables, and updates the DAC2 module to output
//the 10-bit DAC code provided (DACCode10Bit).  The function assumes
//AVDD will be used as the reference, the output will be driven to the 
//RB13/AN13/DAC2 pin.  This function also executes blocking delay so as to
//allow the analog output to settle to the new value, before returning from 
//this function.
//------------------------------------------------------------------------------
void DAC2_UpdateOutput(unsigned int DACCode10Bit)
{
    unsigned int i;
    
    //Error check the input value to make sure it fits in the 10-bit DAC1DAT register
    if(DACCode10Bit > 1023)
        DACCode10Bit = 1023;

    DAC2DAT = DACCode10Bit;
    
    //Turn on and initialize the DAC module, if it isn't enabled already
    if(DAC2CONbits.DACEN == 0)
    {
        //Make sure RG9/DAC1 pin is configured for analog mode and is tri-stated
        ANSBbits.ANSB13 = 1;     //Configure RB13/AN13/DAC2 pin for analog mode
        TRISBbits.TRISB13 = 1;   //Make sure RB13/AN13/DAC2 pin is tri-stated

        DAC2CON = 0x1082;   //DAC continues in sleep, output to pin enabled, no triggers/update on DAC1DAT write, use AVDD as reference
        DAC2CONbits.DACEN = 1;      
        DAC2DAT = DACCode10Bit;
    }    
    
    //Execute some settling time for DAC analog output to reach the target value.
    for(i = 0; i < 10u; i++);   //~3.6us @ 32MHz
    //The DAC should now be at the expected level.
}    


//------------------------------------------------------------------------------
//Function: void DAC_Disable(unsigned char DACModuleNumber)
//Description: Disables the specified DAC module.
//------------------------------------------------------------------------------
void DAC_Disable(unsigned char DACModuleNumber)
{
    if(DACModuleNumber == 1)
    {
        DAC1CONbits.DACEN = 0;
    }    
    else if(DACModuleNumber == 2)
    {
        DAC2CONbits.DACEN = 0;
    }    
}    



//------------------------------------------------------------------------------
//Function: void DAC1_OutputContinuousSineWave(void)   
//Description: Infinite blocking function that generates a continuous sine 
//waveform on the DAC1 output pin.
//------------------------------------------------------------------------------
//void DAC1_OutputContinuousSineWave(void)    
//{
//    unsigned int i;
//    double x;
//    double SineResult;
//    short int DACValue;
//    unsigned int DACSineCodes[1024];
//    x = 0;
//    i = 0;
//    //First we need to compute the values of a sine wave function
//    while(1)
//    {
//        SineResult = sin(x);
//        x += 6.283 / 1024;
//        if(x > 6.283)
//            x = 0;
//            
//        DACValue = SineResult * 512;
//        DACValue += 512;
//        if(DACValue < 0)
//            DACValue = 0;
//        if(DACValue > 1023)
//            DACValue = 1023;
//            
//        DACSineCodes[i] = DACValue;
//        i++;
//        if(i == 1024)
//            break;
//    }
//    
//    //Now that we have the sine wave DAC codes stored in RAM, continuously
//    //output them to the DAC module.  Note this is currently an infinite
//    //blocking function.  This should be improved by using the DMA and a
//    //periodic timer based trigger so the DAC sine generator can operate 
//    //in the background instead.
//    i = 0;
//    while(1)
//    {
//        DAC1_UpdateOutput(DACSineCodes[i++]);
//        if(i >= 1024)
//            i = 0;
//    }    
//}    

//*******************************
//EOF 

