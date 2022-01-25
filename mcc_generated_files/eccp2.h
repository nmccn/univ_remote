/**
  ECCP2 Generated Driver API Header File

  @Company
    Microchip Technology Inc.

  @File Name
    eccp2.h

  @Summary
    This is the generated header file for the ECCP2 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for ECCP2.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.77
        Device            :  PIC18F26K22
        Driver Version    :  2.02
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.05 and above
        MPLAB 	          :  MPLAB X 5.20
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#ifndef ECCP2_H
#define ECCP2_H

/**
  Section: Included Files
*/

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "../main.h"

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif

/** 
  Section: Data type definitions
*/
        
/**
 @Summary
   Defines the values to convert from 16bit to two 8 bit and vice versa

 @Description
   This routine used to get two 8 bit values from 16bit also
   two 8 bit value are combine to get 16bit.

 Remarks:
   None
 */

typedef union CCPR2Reg_tag
{
   struct
   {
      uint8_t ccpr2l;
      uint8_t ccpr2h;
   };
   struct
   {
      uint16_t ccpr2_16Bit;
   };
} CCP2_PERIOD_REG_T ;

/**
  Section: COMPARE Module APIs
*/

/**
  @Summary
    Initializes the ECCP2

  @Description
    This routine initializes the ECCP2_Initialize.
    This routine must be called before any other ECCP2 routine is called.
    This routine should only be called once during system initialization.

  @Preconditions
    None

  @Param
    None

  @Returns
    None

  @Comment
    

  @Example
    <code>
    uint16_t compare;

    ECCP2_Initialize();
    ECCP2_SetCompareCount(compare);
    </code>
 */
void ECCP2_Initialize(void);

/**
  @Summary
    Loads 16-bit compare value.

  @Description
    This routine loads the 16 bit compare value.

  @Preconditions
    ECCP2_Initialize() function should have been
    called before calling this function.

  @Param
    compareCount: 16-bit unsigned value

  @Returns
    None

  @Example
    <code>
    uint16_t compare;

    ECCP2_Initialize();
    ECCP2_SetCompareCount(compare);
    </code>
*/
void ECCP2_SetCompareCount(uint16_t compareCount);

/**
  @Summary
    Implements ISR

  @Description
    This routine is used to implement the ISR for the interrupt-driven
    implementations.

  @Returns
    None

  @Param
    None
*/
void ECCP2_CompareISR(void);

#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif

#endif  //ECCP2_H
/**
 End of File
*/
