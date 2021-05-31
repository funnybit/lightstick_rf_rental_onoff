#include "Common.h"
#include "N76E003.h"
#include "Function_define.h"
#include "SFR_Macro.h"

#include <string.h>

#include "UART.h"
#include "ADC.h"


UINT8 rcvBufferForCheck[2];
UINT8 dataCheckCount = 0;
UINT8 dataBodyCheckCount = 0;
UINT16 rcvBufferIndex = 0;

#define ATCMD_MAX_SIZE 130
UINT8 rcvBuffer[ATCMD_MAX_SIZE];
UINT8 rcvBufferUart[ATCMD_MAX_SIZE];

#define PACKET_SIZE 130

extern bit isParsingData;
extern UINT8 g_TI;

UINT8 buf;



void UART_Hal_Init76E003(uint16_t    u16Baudrate){		//DEBUGCHRIS
    P06_Quasi_Mode;
    P07_Quasi_Mode;

    //P06_PushPull_Mode;
    //P07_Input_Mode;

    SCON = 0x52;     //UART0 Mode1,REN=1,TI=1
    set_SMOD;        //UART0 Double Rate Enable
    T3CON &= 0xF8;   //T3PS2=0,T3PS1=0,T3PS0=0(Prescale=1)
    set_BRCK;        //UART0 baud rate clock source = Timer3

    #ifdef FOSC_160000
    RH3    = HIBYTE(65536 - (1000000/u16Baudrate)-1);  		/*16 MHz */
    RL3    = LOBYTE(65536 - (1000000/u16Baudrate)-1);			/*16 MHz */
    #endif
    set_TR3;         //Trigger Timer3

    set_ES; //Serial Port0 Interrupt Enable
//  set_ES_1;   //Serial Port1 Interrupt Enable
}

void calc_UART_baud_rate(void)
{
	clr_TR3;
	//RH3    = HIBYTE(65536 - (750000/2400)-1);  	/*16 MHz */
    //RL3    = LOBYTE(65536 - (750000/2400)-1);		/*16 MHz */

  //RH3    = HIBYTE(65536 - (750000/9600)-1);  		/*16 MHz */
//	RL3    = LOBYTE(65536 - (750000/9600)-1);		/*16 MHz */
	
	//RH3    = HIBYTE(65536 - (750000/38400)-1);  		/*16 MHz */
	//RL3    = LOBYTE(65536 - (750000/38400)-1);		/*16 MHz */
	
	RH3    = HIBYTE(65536 - (740000/57600)-1);  		/*16 MHz */
	RL3    = LOBYTE(65536 - (740000/57600)-1);		/*16 MHz */
	
	//RH3    = HIBYTE(65536 - (750000/57600)-1);  		/*16 MHz */
	//RL3    = LOBYTE(65536 - (750000/57600)-1);		/*16 MHz */

    set_TR3;         //Trigger Timer3
	set_TI;			 //For printf function must setting TI = 1
}

void SerialPort0_ISR(void) interrupt 4 
{
	//Send_Data_To_UART0('X');
	buf = 0;
    
	if (RI==1) 
    {                                       /* if reception occur */
        clr_RI;                             /* clear reception flag for next reception */
		
		buf = SBUF;
		
		if(buf != 0)
		{
            rcvBufferForCheck[0] = buf;

			/*
            if( (rcvBufferIndex > 20) || ( dataBodyCheckCount > 20) ) { 
                rcvBufferIndex = 0;     
                dataCheckCount = 0; 
                dataBodyCheckCount = 0; 
            } 
			*/

            if( dataCheckCount == 0) {
                if( rcvBufferForCheck[0] == 0x5a ) {                    
                    dataCheckCount = 1;                   
                }
                else {
                    dataCheckCount = 0;     
                }
            }
            else if( dataCheckCount == 1) {
                if( rcvBufferForCheck[0] == 0xa5 ) {
                    dataCheckCount = 2;
                }
                else {
                    dataCheckCount = 0;       
                }
            }
            else if( dataCheckCount == 2) {
                if( rcvBufferForCheck[0] == 0xc3 ) {
                    dataCheckCount = 3;
                    //rcvBufferIndex = 0;
                    //dataBodyCheckCount = 0;
                }
                else {
                    dataCheckCount = 0;       
                }
            }
			else {   
				++dataBodyCheckCount;
				
				if( dataBodyCheckCount <= PACKET_SIZE - 3 ) {				
					rcvBufferUart[rcvBufferIndex] = buf;
					rcvBufferIndex++;
				}
				
				if( dataBodyCheckCount == PACKET_SIZE - 3 ) {
					//Send_Data_To_UART0('W');
					
					/*
					 if( rcvBufferUart[PACKET_SIZE - 5] == 0xf0 ) {   // 14번 위치
						 Send_Data_To_UART0('1');
					 }
					 else if( rcvBufferUart[PACKET_SIZE - 6] == 0xf0 ) {   // 13번 위치
						 Send_Data_To_UART0('2');
					 }
					 else if( rcvBufferUart[PACKET_SIZE - 4] == 0xf0 ) {   // 15번 위치
						 Send_Data_To_UART0('3');
					 }
					*/
					 if( rcvBufferUart[PACKET_SIZE - 5] == 0xf0 ) {
						// Send_Data_To_UART0('2');
						 
						    dataCheckCount = 0;
                            rcvBufferIndex = 0;
                            dataBodyCheckCount = 0;
                            //memcpy( &rcvBuffer[0], rcvBufferUart, 12 );
							memcpy( &rcvBuffer[0], rcvBufferUart, ATCMD_MAX_SIZE );
						 
							if(is5VMode() != BATT_LVL_1) {
								isParsingData = TRUE;		
							}
							
							memset(rcvBufferUart, 0, ATCMD_MAX_SIZE);
					 }
					 else {
						  rcvBufferIndex = 0;
							dataBodyCheckCount = 0;
							dataCheckCount = 0;
							//MemSet(rcvBuffer, 0, ATCMD_MAX_SIZE);
							memset(rcvBufferUart, 0, ATCMD_MAX_SIZE);
					 }
					 
					//rcvBufferIndex = 0;
					//dataBodyCheckCount = 0;
					//dataCheckCount = 0;
				}
				
				//if( rcvBufferUart[10] == 0xf0 ) {
                        
				//		Send_Data_To_UART0('W');
				//}
			}
		}
    }
	
	
	if(TI==1)
    {
      clr_TI;                             /* if emission occur */
	  g_TI = 1;
    }
}


//Debug UART Hex data TX MSG
void Send_UINT8_To_UART0(UINT8 buf)
{
	UINT8 High=0;
	UINT8 Low=0;
	UINT8 High_asc=0;
	UINT8 Low_asc=0;
	
	High = buf>>4 & 0x0f;
	Low =  buf & 0x0f;
	
	if(Low<=9)
	{
		Low_asc = '0' + Low;
	}
	else
	{
		Low_asc = 'a' + Low-10;
	}
	
	if(High<=9)
	{
		High_asc = '0' + High;
	}
	else
	{
		High_asc = 'a' + High-10;
	}
		
	Send_Data_To_UART0(High_asc);
	Send_Data_To_UART0(Low_asc);
}