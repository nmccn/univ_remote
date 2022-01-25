//--------------------------------------------------------------------
// Name:            John Capper & Nicholas McClellan
// Date:            Spring 2020
// Purp:            Final Project
//
// Assisted:        The entire class of EENG 383
// Assisted by:     Microchips 18F26K22 Tech Docs 
//-
//- Academic Integrity Statement: I certify that, while others may have
//- assisted me in brain storming, debugging and validating this program,
//- the program itself is my own work. I understand that submitting code
//- which is the work of other individuals is a violation of the course
//- Academic Integrity Policy and may result in a zero credit for the
//- assignment, or course failure and a report to the Academic Dishonesty
//- Board. I also understand that if I knowingly give my original work to
//- another individual that it could also result in a zero credit for the
//- assignment, or course failure and a report to the Academic Dishonesty
//- Board.
//------------------------------------------------------------------------
#include <string.h>
#include "mcc_generated_files/mcc.h"
#include "main.h"
#pragma warning disable 373
#pragma warning disable 520     
#pragma warning disable 751
#pragma warning disable 1498


/*
 MAIN FUNCTION
 */ 
void main (void) {
    uint8_t i;
    char cmd;
    
    initializeAll();
    CCP2CONbits.CCP2M = 0b1001;
    
    

	for(;;) {

		if (EUSART1_DataReady) {			// wait for incoming data on USART
            cmd = EUSART1_Read();
			switch (cmd) {		// and do what it tells you to do
            //--------------------------------------------
            // Determine the number of positive and negative edges in the IR waveform for a single donor remote control button press. v         
            //--------------------------------------------                    
            case 'c':
                negEdge = 0;
                posEdge = 0;
                printf("To begin counting edges, please press a button on the remote \r\n");

                /* Search the array , incrementing both edges until we find '0' */
                for(i = 0; i < MAX_DATA_BITS * 2; i++) {
                    captureOne();
                    bitCounts[i] = tmrCounts;
                    /* If we found a 0 , done looking*/
                    if(bitCounts[i] == 0){
                        break;
                    }
                     /* Otherwise, we will increment an edge*/
                    else{
                        if(i % 2)
                            posEdge++; // Every other index is a different edge.
                        else
                            negEdge++;
                    }
                }
                printf("Number of Rising Edges: %u \r\n", posEdge);
                printf("Number of Falling Edges: %u \r\n", negEdge);   
                break;
                
            //--------------------------------------------
            // Train (Implement to increase technical difficulty)          
            //--------------------------------------------                    
            case 't':
                printf("Send an IR signal to record durations\r\n");
                recordDurations(true);
                setSevenDurations();
                printSevenDurations();
                break;
                
            //--------------------------------------------
            // Decode (Implement to increase technical difficulty)         
            //--------------------------------------------                    
            case 'd':
                printf("Press a button to decode waveform\r\n");
                recordDurations(false);
                decodedValue = decodeBits(false);
                printf("Code: %lX \r\n", decodedValue);
                break;
            
            //--------------------------------------------
            // Learn            
            //--------------------------------------------                    
            case 'l':   
                printf("Let's clone buttons 1-4\r\n");
                for(i = 0; i < 4; i++) {
                    printf("Press the %u button\r\n", i+1);
                    recordDurations(false);
                    decodedValue = decodeBits(false);
                    printf("\tCode: %lX \r\n", decodedValue);
                    numberButtonCodes[i] = decodedValue;
                }
                printf("Finished learning buttons 1-4\r\n");
                break;
           
            //--------------------------------------------
            // Produce an IR waveform that corresponds to the code and durations for that cloned donor remote control button.          
            //--------------------------------------------                    
            case '1':
            case '2':
            case '3':
            case '4':
                printf("Producing signal for %c button\r\n", cmd);
                decodedValue = numberButtonCodes[((uint8_t)(i))-1];
                CCP2CONbits.CCP2M = 0b0010; //Set to toggle on match
                transmitting = true;
                transmittingFinished = false;
                while(!transmittingFinished);
                
//                produceWaveform(numberButtonCodes[((uint8_t)(i))-1]);   // Produce output of corresponding button command
                CCP2CONbits.CCP2M = 0b1001;
                break;
                
            //--------------------------------------------
            // Capture button sequence
            //-------------------------------------------- 
            case 's':
                printf("Press buttons to record a sequence (max: %u buttons). Press any key when done\r\n", MAX_BUTTON_SEQUENCE);
                memset(buttonSequenceCodes, 0, sizeof(buttonSequenceCodes));    // Reset sequence array
                printf("\tSequence:\r\n");
                for(i = 0; i < MAX_BUTTON_SEQUENCE; i++) {
                    if(EUSART1_DataReady)
                        break;
                    recordDurations(false);
                    decodedValue = decodeBits(false);
                    printf("\t\t -%lX \r\n", decodedValue);
                }
                (void) EUSART1_Read();  // Clear flag
                
                printf("Finished recording sequence\r\n");
                
                break;
            //--------------------------------------------
            // Transmit button sequence
            //-------------------------------------------- 
            case 'S':
                printf("Transmitting button sequence:\r\n");
                for(i = 0; i < MAX_BUTTON_SEQUENCE; i++) {
                    if(!buttonSequenceCodes[i])
                        break;
                    printf("\t-%lX \r\n");
//                    produceWaveform(buttonSequenceCodes[i]);
                }
                printf("Finished transmitting\r\n");
                break;
                
            case 'b':
                for(i = 0; i < 4; i++)
                    binaryPrint(numberButtonCodes[i]);
                break;
                    
            //--------------------------------------------
            // Print out 7 reference durations  
            //--------------------------------------------               
            case 'i':

                printSevenDurations();
                break;
                 
			//--------------------------------------------
			// General Cases
			//--------------------------------------------
			default:
				generalCases(cmd);
				break;
			}      
		}
	}
}


