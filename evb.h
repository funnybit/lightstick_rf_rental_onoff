#ifndef __EVB_H__
#define __EVB_H__

#include "Common.h"
#include "N76E003.h"
#include "Function_define.h"

//CSR PIO
#define PIO_SCL     P13//(5)
#define PIO_CSB     P10//(6)
#define PIO_SDA     P14//(7)

#define PIO_DIR_OUTPUT  1        /* PIO direction configured as output */
#define PIO_DIR_INPUT   0       /* PIO direction configured as input */

typedef unsigned char byte;
typedef unsigned int  word;
typedef unsigned long lword;

//typedef false FALSE;
//typedef true TRUE;

//void PioSet(int x, int y);
//int PioGet(int x);
//#define DebugWriteString(char* str, ...) 0
#define PioSet(pin,val)	pin=val
#define PioGet(pin)	pin

#define RF_MODE_447_9_KOR

//P1口							//PxSEL PxDIR PxOUT PxIE PxIES 
#define	TxD			0x01		//  1     1     1     0    0   	//TxD
#define RxD			0x02		//  0     0     0	  0    0	//待定

#define nCS			0x08        //  0     1     1     0    0    //nCS
#define	SCK			0x10		//  0     1     0     0    0 	//SCK


#define	Set_nCS()		PioSet(PIO_CSB, TRUE)//(P2OUT |= nCS)
#define	Clr_nCS()		PioSet(PIO_CSB, FALSE)//(P2OUT &= (~nCS))

#define	Set_SCK()		PioSet(PIO_SCL, TRUE)//(P2OUT |= SCK)
#define	Clr_SCK()		PioSet(PIO_SCL, FALSE)//(P2OUT &= (~SCK))

#define	Output_MOSI()	P14_Quasi_Mode//0//PioSetDir(PIO_SDA, PIO_DIR_OUTPUT)//(P2DIR |= MOSI)
#define	Input_MOSI()	P14_Input_Mode//0//PioSetDir(PIO_SDA, PIO_DIR_INPUT)//(P2DIR &= (~MOSI))
#define	Set_MOSI()		PioSet(PIO_SDA, TRUE)//(P2OUT |= MOSI)
#define	Clr_MOSI()		PioSet(PIO_SDA, FALSE)//(P2OUT &= (~MOSI))

#define	MOSI_H()		0//(P2IN&MOSI)

extern byte FreqSel;		//频率选择

//basic func
extern byte RFM219A_EntryRx(void);
extern void RFM219A_Sleep(void);
extern void RFM219A_Standby(void);
extern void RFM219A_ClearIRQ(void);
extern void RFM219A_Config(void);
extern void SPI3Write(word WrPara);
extern byte SPI3Read(byte adr);
extern void SPI3Cmd8bit(byte WrPara);
extern byte SPI3Read8bit(void);

//added func
extern void rf_init(void);
extern void set_default_register(void);
extern void compare_RFPDK_config(void);
extern void RF_Check_proc(void);

extern const word RFM219FreqTbl[10];
extern const word RFM219ConfigTbl[52];

#endif