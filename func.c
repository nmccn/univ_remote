/* 
 * File:   func.c
 * Author: John Capper & Nicholas McClellan

 */
#include "mcc_generated_files/mcc.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>

/* CAPTURING */
void captureOne(void) {
   
    PIR1bits.CCP1IF = 0;            // CCPR1 register when a falling 
    while (!ECCP1_IsCapturedDataReady());   // edge of RC2 is detected
    ECCP_TRIGGER_TOGGLE();
    tmrCounts = CCPR1;    
}

void recordDurations(bool verbose) {
    ECCP_SET_FALLING();
    uint8_t i;
    for(i = 0; i < MAX_HALF_BITS; i++) {
        captureOne();
        bitCounts[i] = tmrCounts;
    }
    for(i = 0; i < MAX_HALF_BITS - 1; i++) {
        bitDurations[i] = (bitCounts[i+1] - bitCounts[i]) * CONVERSION_FACTOR;
        if(verbose)
            printf("%u us\r\n", bitDurations[i]);
    }
}

void setSevenDurations(void) {
    referenceDurations.startLow = bitDurations[0];
    referenceDurations.startHigh = bitDurations[1];
    referenceDurations.dataZeroLow = bitDurations[2];
    referenceDurations.dataZeroHigh = bitDurations[3];
    referenceDurations.dataOneLow = bitDurations[4];
    referenceDurations.dataOneHigh = bitDurations[5];
    referenceDurations.stopLow = bitDurations[32];
}

uint32_t decodeBits(bool printBinary) {
    uint32_t decodedVal;
    uint16_t durationLow, durationHigh;
    uint8_t binaryDigit;
    for(uint8_t i = 2; i < (MAX_DATA_BITS*2)-1; i+=2) {
        durationLow = bitDurations[i];
        durationHigh = bitDurations[i+1];
        if(((durationLow > referenceDurations.dataZeroLow-(referenceDurations.dataZeroLow*ERROR_ALLOWED)) && (durationLow < referenceDurations.dataZeroLow+(referenceDurations.dataZeroLow*ERROR_ALLOWED))) 
           &&( (durationHigh > referenceDurations.dataZeroHigh-(referenceDurations.dataZeroHigh*ERROR_ALLOWED)) && (durationHigh < referenceDurations.dataZeroHigh+(referenceDurations.dataZeroHigh*ERROR_ALLOWED)) )) {
                decodedVal = decodedVal<<1;
                if(printBinary)
                    printf("0");
        }
        /* Checks if Bit 1*/
        else if(( (durationLow > referenceDurations.dataOneLow-(referenceDurations.dataOneLow*ERROR_ALLOWED)) && (durationLow < referenceDurations.dataOneLow+(referenceDurations.dataOneLow*ERROR_ALLOWED)) ) 
            && ((durationHigh > referenceDurations.dataOneHigh-(referenceDurations.dataOneHigh*ERROR_ALLOWED)) && (durationHigh < referenceDurations.dataOneHigh+(referenceDurations.dataOneHigh*ERROR_ALLOWED)))) {
                decodedVal = decodedVal<<1 | 1;
                if(printBinary)
                    printf("1");
        }
        else{
            break;  // Means that it's stop bit
        }
    }   
    if(printBinary)
        printf("\r\n");
    
    return decodedVal;
}

void printSevenDurations(void) {
    printf( "-------------------------------------------------\r\n"
            "Start: 		Lo: %u uS	Hi: %u  uS\r\n"
            "Data 0:		Lo: %u uS	Hi: %u uS\r\n"
            "Data 1:		Lo: %u uS	Hi: %u uS\r\n"
            "Stop:		Lo: %u uS\r\n"
            "Half bits:	%u half bits per button\r\n"
            "-------------------------------------------------\r\n", 
            referenceDurations.startLow, referenceDurations.startHigh, referenceDurations.dataZeroLow, referenceDurations.dataZeroHigh, referenceDurations.dataOneLow, referenceDurations.dataOneHigh, referenceDurations.stopLow, posEdge+negEdge);
}

/* TRANSMISSION */
//void produceWaveform(uint32_t decodedValue) {
//    //uint8_t i;
//    
//    /* Send Start */
//    transmitOne(referenceDurations.startLow);
//    transmitOne(referenceDurations.startHigh);
//    printf("Decoded Value: %lX \r\n", decodedValue);
//        for(int i = 31; i >= 0; i--) {
//        
//        if( (decodedValue>>i) % 2) {   // Bit is a 1
//            printf("1");
//            transmitOne(referenceDurations.dataZeroLow);
//            transmitOne(referenceDurations.dataZeroHigh);
//        } else {  // Bit is a 0
//            transmitOne(referenceDurations.dataOneLow);
//            transmitOne(referenceDurations.dataOneHigh);
//            printf("0");
//        }
//
//    }
//    printf("\r\n");
//    /* Send Stop */
//    transmitOne(referenceDurations.stopLow);
//}

void binaryPrint(uint32_t value) {
    for(int i = 31; i >= 0; i--) {
        printf("%u", (value>>i) % 2);
    }
    printf("\r\n");
}

//void transmitOne(uint16_t us) {
//    CCPR2 = TMR1_ReadTimer() + us/CONVERSION_FACTOR;    // Advance by us
//    PIR2bits.CCP2IF = 0;
//    while (!ECCP2_IsCompareComplete()); // Wait for match
    
//}



typedef enum {TX_IDLE, TX_START, TX_DATA, TX_STOP} txStates;
void ECCP2_ISR(void) {
    static txStates state = TX_IDLE;
    static uint8_t shiftMag;
    static uint8_t sentLow;
    static uint16_t waitDuration;
    
    if(transmitting) {
        switch(state) {
            case TX_IDLE:
                shiftMag = MAX_DATA_BITS-1;
                sentLow = false;
                state = TX_START;
                break;
                
            case TX_START:
                if(!sentLow) {
                    state = TX_DATA;
                    waitDuration = referenceDurations.startHigh;
                    sentLow = false;
                } else{
                    waitDuration = referenceDurations.startLow;
                    sentLow = true;
                }
                break;
                
            case TX_DATA:
                if( (decodedValue>>shiftMag) % 2) {   // Bit is a 1
                    if(!sentLow) {  // Send bitLow if not yet sent
                        waitDuration = referenceDurations.dataOneLow;
                        sentLow = true;
                    } else {  // Send bitHigh if bitLow was just sent
                        waitDuration = referenceDurations.dataOneHigh;
                        sentLow = false;
                        shiftMag--;
                    }
                } else {   // Bit is a 0
                    if(!sentLow) {  // Send bitLow if not yet sent
                        waitDuration = referenceDurations.dataZeroLow;
                        sentLow = true;
                    } else {  // Send bitHigh if bitLow was just sent
                        waitDuration = referenceDurations.dataZeroHigh;
                        sentLow = false;
                        shiftMag--;
                    }
                }
                if(shiftMag == 1) {
                    state = TX_STOP;
                }
                break;
                
            case TX_STOP:
                waitDuration = referenceDurations.stopLow;
                state = TX_IDLE;
                transmitting = false;
                transmittingFinished = true;
                break;
        }
    }
    CCPR2 = TMR1+ waitDuration/CONVERSION_FACTOR;
    // Clear the ECCP2 interrupt flag
    PIR2bits.CCP2IF = 0;
}