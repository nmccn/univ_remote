/* 
 * File:   helpers.c
 * Author: John Capper & Nicholas McClellan
 */

#include "mcc_generated_files/mcc.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>

/* Involving PIC */
void initializeAll(void) { 
    SYSTEM_Initialize();
    ECCP1_Initialize();
    ECCP2_Initialize();
    INTERRUPT_GlobalInterruptEnable();      // ISR not working? - you probably 
    INTERRUPT_PeripheralInterruptEnable();

    // Print opening message
	printf("Dev'20 Board\r\n");
    printf("Final Project\r\n");
    printf("Install a jumper from RC4 to RC2 only after unplugging PICKit4\r\n");
    printf("Connect a jumper from RC1 to IR Transmitter PIN\r\n");
	printf("\r\n> ");                       // print a nice command prompt
}

void ECCP_SET_RISING(void) {
    CCP1CONbits.CCP1M = 0b0101;
}

void ECCP_SET_FALLING(void) {
    CCP1CONbits.CCP1M = 0b0100;
}

void ECCP_TRIGGER_TOGGLE(void) {
    if(CCP1CONbits.CCP1M == 0b0100) {
        ECCP_SET_RISING();
    } else {
        ECCP_SET_FALLING();
    }
}


/* General Helpers */
void generalCases(char cmd) {
    uint8_t i;
    switch(cmd) {
        //--------------------------------------------
        // Reply with help menu
        //--------------------------------------------
        case '?':
            printMenu();
            break;

        //--------------------------------------------
        // Reply with "ok", used for PC to PIC test
        //--------------------------------------------
        case 'o':
            printf("o:	ok\r\n");
            break;                

        //--------------------------------------------
        // Reset the processor after clearing the terminal
        //--------------------------------------------                      
        case 'Z':
            for (i=0; i<40; i++) printf("\n");
            RESET();                    
            break;

        //--------------------------------------------
        // Clear the terminal
        //--------------------------------------------                      
        case 'z':
            for (i=0; i<40; i++) printf("\n");                            
            break;      
            
        //--------------------------------------------
        // If something unknown is hit, tell user
        //--------------------------------------------
        default:
            printf("Unknown key %c\r\n",cmd);
            break;
    }
}

void printMenu(void) {
    printSevenDurations();
    printf(
            "?: help menu\r\n"
            "o: k\r\n"
            "Z: reset processor\r\n"
            "z: clear the terminal\r\n"
            "\r\n"
            "c: count number of half bits\r\n"
            "t: determine Time periods for all the bits\r\n"
            "d: decode logic 1 periods\r\n"
            "r: Report bit width values\r\n"
            "\r\n"
            "l: cLone 4 donor remote buttons\r\n"
            "1-4: Transmit IR packet\r\n"
            "s: Record button sequence\r\n"
            "S: Transmit recorded button sequence\r\n"
            "i: Print out 7 duration references\r\n"
            "-------------------------------------------------\r\n");
}
