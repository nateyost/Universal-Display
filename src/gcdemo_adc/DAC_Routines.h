/********************************************************************
 FileName:      DAC_Routines.h
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
 ********************************************************************/

#ifndef __DAC_ROUTINES__H
#define __DAC_ROUTINES__H



//------------------------------------------------------------------------------
//Public prototypes
//------------------------------------------------------------------------------
void DAC1_UpdateOutput(unsigned int DACCode10Bit);
void DAC2_UpdateOutput(unsigned int DACCode10Bit);
void DAC_Disable(unsigned char DACModuleNumber);
void DAC1_OutputContinuousSineWave(void);



#endif //__DAC_ROUTINES__H

//*******************************
//EOF DAC_Routines.h

