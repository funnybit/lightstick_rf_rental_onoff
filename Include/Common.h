#ifndef __COMMON_H__
#define __COMMON_H__

typedef bit                   BIT;
typedef unsigned char         UINT8;
typedef unsigned int          UINT16;
typedef unsigned long         UINT32;

typedef unsigned char         uint8_t;
typedef unsigned int          uint16_t;
typedef unsigned long         uint32_t;

typedef UINT8 bool;
#define FALSE 0
#define TRUE 1

typedef unsigned char         uint8;
typedef unsigned int          uint16;
typedef unsigned long         uint32;


#define     CID_READ				0x0B
#define     DID_READ				0x0C

#define     ERASE_APROM				0x22
#define     READ_APROM				0x00
#define     PROGRAM_APROM			0x21
#define     ERASE_LDROM				
#define     READ_LDROM				
#define     PROGRAM_LDROM			
#define     READ_CFG					0xC0
#define     PROGRAM_CFG				0xE1
#define			READ_UID					0x04

#if 0
void  InitialUART0_Timer1(UINT32 u32Baudrate); //T1M = 1, SMOD = 1
void  InitialUART1_Timer3(UINT32 u32Baudrate);
#endif

void  InitialUART0_Timer3(UINT32 u32Baudrate); //Timer3 as Baudrate, SMOD=1, Prescale=0
void  Send_Data_To_UART0(UINT8 c);
void  InitialUART1(UINT32 u32Baudrate);

void SW_Reset(void);

extern bit BIT_TMP;

#if 0 
void  Send_Data_To_UART1(UINT8 c);
UINT8 Receive_Data_From_UART0(void);
UINT8 Receive_Data_From_UART1(void);
#endif

#endif