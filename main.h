
/* 
 * File:   main.h
 * Author: John Capper & Nicholas McClellan
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  



#define MAX_DATA_BITS       33
#define MAX_HALF_BITS       MAX_DATA_BITS*2
#define MAX_BUTTON_SEQUENCE 8
#define TMR1_PRESCALER      8
#define HUGE_BUFFER         128
#define ERROR_ALLOWED       0.1f
#define CONVERSION_FACTOR   (TMR1_PRESCALER)/(16)

/* 7 Main Durations */
struct ReferenceDurations {
    uint16_t startLow, startHigh;
    uint16_t dataZeroLow, dataZeroHigh;
    uint16_t dataOneLow, dataOneHigh;
    uint16_t stopLow;
} referenceDurations = {
    890, 385,
    600, 570,
    600, 1650,
    570
};  // Values are in microseconds (us)

/* Edge Counts */
uint8_t posEdge, negEdge;

/* Main Arrays. Holds data for most recently capture waveform */
uint16_t bitCounts[MAX_HALF_BITS];
uint16_t bitDurations[MAX_HALF_BITS];

uint32_t numberButtonCodes[4];
uint32_t buttonSequenceCodes[MAX_BUTTON_SEQUENCE];

uint16_t tmrCounts;

uint8_t transmitting = false;
uint8_t transmittingFinished = false;
uint32_t decodedValue;


/* Capturing */
void capture(void);
void captureOne(void);
void countEdges(void);
void recordDurations(bool verbose);
void setSevenDurations(void);
uint32_t decodeBits(bool printBinary);

void binaryPrint(uint32_t value);

/* Transmission */
//void produceWaveform(uint32_t decodedValue);
//void transmitOne(uint16_t ms);
void ECCP2_ISR(void);

/* PIC HELPERS */
void initializeAll(void);

void ECCP_SET_RISING(void);
void ECCP_SET_FALLING(void);
void ECCP_TRIGGER_TOGGLE(void);


/* General Helpers */
void printMenu(void);
void generalCases(char cmd);
void printBuffer(char *buffer, uint8_t size);

void printSevenDurations(void);

#endif