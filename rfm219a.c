#include "evb.h"
#include "UART.h"
//#include <pio.h>            /* Programmable I/O configuration and control */
//#include "debug_msg.h"

//*********************************************************
//IO mapping
//*********************************************************
//		 MCU     RFM219A
// 0.   PORChk---
// 1.	 nCS-----CSB
// 2.	 SCK-----SCK
// 3.	MOSI-----SDA
// 4.	MISO-----FCSB
// 5.	nIRQ0----GIO1
// 6.	nIRQ1----GIO2
// 7.	RFData---GIO4
// 8.	DIO0-----GIO3
// 9.	DIO1-----
//*********************************************************

byte FreqSel;   //주파수 설정 인덱스

extern bool m_is_rf_reinit;

int g_bReqeust_RF_init;   //RF_init 요청 변수
//#define MIN_SELECT_FREQ_INDEX   1
//#define MAX_SELECT_FREQ_INDEX   6

//SelectFreq 161228 기준
// 1 : 447.9MHz
// 2 : 429MHz
// 3 : 433.92MHz
// 4 : 447.862MHz
// 5 : 447.950MHz
// 6 : 447.987MHz

enum ENUM_SELECT_FREQ
{
    MIN_SELECT_FREQ_INDEX = 1,
    SELECT_FREQ_447_9 = MIN_SELECT_FREQ_INDEX,
    SELECT_FREQ_429,
    SELECT_FREQ_433_92,
    SELECT_FREQ_447_862,
    SELECT_FREQ_447_950,
    SELECT_FREQ_447_987,
    MAX_SELECT_FREQ_INDEX = SELECT_FREQ_447_987
};

//*********************************************************
//Reg mapping
//*********************************************************

#define	INTCTL_B	0x4200
	#define	RX_DONE_CLR		0x01
	#define	CRC_PS_CLR		0x02
	#define	NODE_PS_CLR		0x04
	#define	SYNC_PS_CLR		0x08
	#define	PREAM_PS_CLR	0x10
	#define	RSSI_VLD_CLR	0x20
	#define RX_TMO_CLR		0x40
	#define	SL_TMO_CLR		0x80

#define	INTCTL_D	0x4400
	#define	FIFO_WR_METH	0x01
	#define	FIFO_WR_EN		0x02
	#define	FIFO_CLR		0x04
	#define	FIFO_OVF_FLG	0x08
	#define	FIFO_TH_FLG		0x10
	#define	FIFO_NMTY_FLG	0x20
	#define	FIFO_FULL_FLG	0x40
	#define	PKT_LEN_ERR_FLG	0x80	//?

#define	OP_MODE		0x4700
	#define OP_EEPROM	0x01
	#define	OP_STANDBY	0x02	
	#define	OP_FS		0x04
	#define	OP_RX		0x08
	#define	OP_SLEEP	0x10
	
	#define	RD_IDLE		0x00
	#define	RD_SLEEP	0x20
	#define	RD_STBY		0x40
	#define	RD_FS		0x60
	#define	RD_RX		0x80
	#define	RD_EEPROM	0xA0
	
	#define OP_MASK		0xE0

#define	SOFT_RST 	0x4FFF

//*********************************************************
//RF Table
//*********************************************************

// 18.04.28 duty off
/*
const word RFM219FreqTbl[2][10] = {
    //{0x0080, 0x0122, 0x022C, 0x039B, 0x0407, 0x0D1E, 0x110F, 0x2956, 0x3292, 0x3BEB},   //160824_004 447.9MHz, 9600
	  {0x00C0, 0x0122, 0x022C, 0x039B, 0x0407, 0x0D2E, 0x110A, 0x2956, 0x3292, 0x3BEB},   // 447.9 57400bps(수정완료)
	//{0x0080, 0x0122, 0x022C, 0x039B, 0x0407, 0x0D1E, 0x110F, 0x2956, 0x3292, 0x3BEB},   // 447.9 19200 bps(수정완료)
	  //{0x00A0, 0x0122, 0x022C, 0x039B, 0x0407, 0x0D2E, 0x110A, 0x2956, 0x3292, 0x3BEB},   // 447.9 38400 bps(수정완료)
    {0x0062, 0x0121, 0x0238, 0x0358, 0x0400, 0x0D1E, 0x1114, 0x2956, 0x328E, 0x3BEB},   //161124 429MHz
    //{0x0062, 0x0121, 0x0266, 0x0366, 0x0406, 0x0D1E, 0x1114, 0x2956, 0x3290, 0x3BEB},   //161124 433.92MHz
    //{0x0062, 0x0122, 0x0233, 0x038F, 0x0407, 0x0D1E, 0x1114, 0x2956, 0x3292, 0x3BEB},   //161228 447.862MHz
    //{0x0062, 0x0122, 0x02ED, 0x03AA, 0x0407, 0x0D1E, 0x1114, 0x2956, 0x328F, 0x3BEB},   //161228 447.950MHz
    //{0x0062, 0x0122, 0x0296, 0x03B6, 0x0407, 0x0D1E, 0x1114, 0x2956, 0x328F, 0x3BEB},   //161228 447.987MHz
};
*/

const word RFM219FreqTbl[10] = {
	0x00C0, 0x0122, 0x022C, 0x039B, 0x0407, 0x0D2E, 0x110A, 0x2956, 0x3292, 0x3BEB   // 447.9 57400bps(수정완료)
};

const word RFM219ConfigTbl[52] = {
	// 57600
	///*
	0x0500,
    0x0600,
    0x0771,
    0x08B0,
    0x090E,
    0x0A00,
    0x0B00,
    0x0C73,
    0x0E13,
    0x0F10,
    0x1084,
    0x12E0,
    0x1300,
    0x1415,
    0x157F,
    0x1600,
    0x1700,
    0x1801,
    0x1900,
    0x1A00,
    0x1BA4,
    0x1C01,
    0x1D01,
    0x1E50,
    0x1F81,
    0x2003,
    0x2184,
    0x2200,
    0x2380,
    0x2419,
    0x2500,
    0x2600,
    0x2780,
    0x28AC,
    0x2A53,
    0x2BD4,
    0x2C40,
    0x2D49,
    0x2EFF,
    0x2F50,
    0x3038,
    0x3100,
    0x33FA,
    0x3400,
    0x3500,
    0x3600,
    0x37C0,
    0x3800,
    0x3900,
    0x3A20,
    0x3C03,
    0x3D00
		//*/
		
	/*
	// 38400
	0x0500,
0x0600,
0x0771,
0x08B0,
0x0915,
0x0A00,
0x0B00,
0x0C73,
0x0E26,
0x0F90,
0x1084,
0x12E0,
0x1300,
0x1415,
0x157F,
0x1600,
0x1700,
0x1801,
0x1900,
0x1A00,
0x1BA4,
0x1C01,
0x1D01,
0x1E50,
0x1F81,
0x2003,
0x2184,
0x2200,
0x2380,
0x2419,
0x2500,
0x2600,
0x2780,
0x28AC,
0x2A53,
0x2BD4,
0x2C40,
0x2D49,
0x2EFF,
0x2F56,
0x3026,
0x3100,
0x33FA,
0x3400,
0x3500,
0x3600,
0x37C0,
0x3800,
0x3900,
0x3A20,
0x3C03,
0x3D00
*/
	/*
	// 9600
   0x0500,
   0x0600,
   0x0771,
   0x08B0,
   0x0955,
   0x0A00,
   0x0B00,
   0x0C62,
   0x0E4C,
   0x0F90,
   0x1084,
   0x12E0,
   0x1300,
   0x1405,
   0x157F,
   0x1600,
   0x1700,
   0x1801,
   0x1900,
   0x1A00,
   0x1BA4,
   0x1C01,
   0x1D01,
   0x1E50,
   0x1F81,
   0x2003,
   0x2184,
   0x2200,
   0x2380,
   0x2419,
   0x2500,
   0x2600,
   0x2780,
   0x28AC,
   0x2A53,
   0x2BD4,
   0x2C40,
   0x2D49,
   0x2EFF,
   0x2F59,
   0x3012,
   0x3100,
   0x33FA,
   0x3400,
   0x3500,
   0x3600,
   0x37C0,
   0x3800,
   0x3900,
   0x3A20,
   0x3C07,
   0x3D00
	 */
	 
	 // 19200
	 /*
	 0x0500,
0x0600,
0x0771,
0x08B0,
0x092A,
0x0A00,
0x0B00,
0x0C62,
0x0E4C,
0x0F90,
0x1084,
0x12E0,
0x1300,
0x1415,
0x157F,
0x1600,
0x1700,
0x1801,
0x1900,
0x1A00,
0x1BA4,
0x1C01,
0x1D01,
0x1E50,
0x1F81,
0x2003,
0x2184,
0x2200,
0x2380,
0x2419,
0x2500,
0x2600,
0x2780,
0x28AC,
0x2A53,
0x2BD4,
0x2C40,
0x2D49,
0x2EFF,
0x2F59,
0x3013,
0x3100,
0x33FA,
0x3400,
0x3500,
0x3600,
0x37C0,
0x3800,
0x3900,
0x3A20,
0x3C07,
0x3D00
*/
};



/******************************************************************************
**변鑒츰냔：RFM219_EntryRx
**변鑒묘콘：RFM219?阮첵廢戮錟?
**渴흙꽝鑒：轟
**渴놔꽝鑒：0――榴檄嫩끽
******************************************************************************/
byte RFM219A_EntryRx(void)
{
    byte tmp;
 
    //칩 환경설정
    RFM219A_Config();
 
    //Operating state RX 상태로 변경
    tmp = SPI3Read((byte)(OP_MODE>>8));		//샘굶토零
    tmp &= OP_MASK;
    SPI3Write(OP_MODE+tmp+OP_RX);

    return(1);				//럿쀼攣끽
}

/******************************************************************************
**변鑒츰냔：RFM219_Sleep
**변鑒묘콘：RFM219慷추
**渴흙꽝鑒：轟
**渴놔꽝鑒：轟
******************************************************************************/
void RFM219A_Sleep(void)
{
    byte tmp;	
    tmp = SPI3Read((byte)(OP_MODE>>8));
    tmp &= OP_MASK;
    SPI3Write(OP_MODE+tmp+OP_SLEEP);
}

/******************************************************************************
**변鑒츰냔：RFM219_Standby
**변鑒묘콘：RFM219?阮첵廢?Standby
**渴흙꽝鑒：轟
**渴놔꽝鑒：轟
******************************************************************************/
void RFM219A_Standby(void)
{
    byte tmp;	
    tmp = SPI3Read((byte)(OP_MODE>>8));
    tmp &= OP_MASK;
    SPI3Write(OP_MODE+tmp+OP_STANDBY);	
}

//*****************************************************
//Auxrl
//*****************************************************

void RFM219A_ClearIRQ(void)
{
    SPI3Write(INTCTL_B+0xFF);
}

/******************************************************************************
**변鑒츰냔：RFM219_Config
**변鑒묘콘：놓迦뺏，빈쏵흙Standby친駕
**渴흙꽝鑒：轟
**渴놔꽝鑒：轟
******************************************************************************/
void RFM219A_Config(void)
{
    byte i;	

    RFM219A_Sleep();				//state를 sleep으로 변경
    RFM219A_Standby();				//state를 standby으로 변경 
 
    //주파수 설정 쓰기
    for(i=0;i<9;i++) 
        SPI3Write(RFM219FreqTbl[i]);
    //RFPDK 환경설정값 쓰기
    for(i=0;i<52;i++)
        SPI3Write(RFM219ConfigTbl[i]);

    RFM219A_ClearIRQ();             //IRQ클리어
    RFM219A_Standby();				//state를 standby으로 변경 
}
/*
void PioSet(int x, int y)
{
	
}
*/
/******************************************************************************
**변鑒츰냔：SPI3Write
**변鑒묘콘：SPI畇흙寧몸16鑒앴（멕8貫뒈囹，됴8貫鑒앴）
**渴흙꽝鑒：WrPara
**渴놔꽝鑒：轟
******************************************************************************/
void SPI3Write(word WrPara)								
{                                                       
    byte bitcnt;    
    
    //Output 설정
    Output_MOSI();			//SDA槨渴놔
    
    PioSet(PIO_SCL, FALSE);//Clr_SCK();				//鬧雷SCK邱헌0，괏넣됴
    PioSet(PIO_CSB, FALSE);//Clr_nCS();		
  
    for(bitcnt=16; bitcnt!=0; bitcnt--)
    {
        PioSet(PIO_SCL, FALSE); //Clr_SCK();	
        
        if(WrPara&0x8000)
            PioSet(PIO_SDA, TRUE); //Set_MOSI();
        else
            PioSet(PIO_SDA, FALSE); //Clr_MOSI();
        
        PioSet(PIO_SCL, TRUE);//Set_SCK();
        WrPara <<= 1;
    }
    
    PioSet(PIO_SCL, FALSE);//Clr_SCK();	
    PioSet(PIO_CSB, TRUE); //Set_nCS();
}        

/******************************************************************************
**변鑒츰냔：SPI3Read
**변鑒묘콘：SPI뗍혤寧몸뒈囹鑒앴
**渴흙꽝鑒：adr
**渴놔꽝鑒：轟
******************************************************************************/
byte SPI3Read(byte adr)
{
    byte tmp;	
    SPI3Cmd8bit(0x80+adr);			//뒈囹
    tmp = SPI3Read8bit();	
	
	//Debug msg
	////Send_UINT8_To_UART0(tmp);
	////Send_Data_To_UART0('\r');
	////Send_Data_To_UART0('\n');
    PioSet(PIO_CSB, TRUE); //Set_nCS();
    return(tmp);
}  					

/******************************************************************************
**변鑒츰냔：SPI3Cmd8bit
**변鑒묘콘：SPI畇흙꽝鑒8bit
**渴흙꽝鑒：WrPara
**渴놔꽝鑒：轟
**鬧雷：    괏즛nCS渴놔Low
******************************************************************************/
void SPI3Cmd8bit(byte WrPara)
{
    byte bitcnt;	
    
    PioSet(PIO_SDA, FALSE); //데이터를 보내지 않을때 SDA를 Low로 떨어뜨려놓기위한 처리, 이선빈 책임님 요구사항
    Output_MOSI();			//SDA槨渴놔
    
    PioSet(PIO_SCL, FALSE);//Clr_SCK();				//鬧雷SCK邱헌0，괏넣됴
    PioSet(PIO_CSB, FALSE);//Clr_nCS();		
    
    for(bitcnt=8; bitcnt!=0; bitcnt--)
    {
        PioSet(PIO_SCL, FALSE);//Clr_SCK();	
        if(WrPara&0x80)
            PioSet(PIO_SDA, TRUE);//Set_MOSI();
        else
            PioSet(PIO_SDA, FALSE); //Clr_MOSI();
        PioSet(PIO_SCL, TRUE); //Set_SCK();
        WrPara <<= 1; 		
    }
}

/******************************************************************************
**변鑒츰냔：SPI3Read8bitt
**변鑒묘콘：SPI뗍혤꽝鑒8bit
**渴흙꽝鑒：뗍혤8bit鑒앴――RdPara
**渴놔꽝鑒：轟
**鬧雷：    괏즛nCS渴놔Low
******************************************************************************/
byte SPI3Read8bit(void)
{
    byte RdPara = 0;
    byte bitcnt;
    
    PioSet(PIO_SDA, FALSE); //데이터를 보내지 않을때 SDA를 Low로 떨어뜨려놓기위한 처리, 이선빈 책임님 요구사항
    PioSet(PIO_CSB, FALSE); //Clr_nCS(); 
    Input_MOSI();			//뗍FIFO，郭넣SDI槨H		
    
    for(bitcnt=8; bitcnt!=0; bitcnt--)
    {
        PioSet(PIO_SCL, FALSE);//Clr_SCK();
        RdPara <<= 1;
        PioSet(PIO_SCL, TRUE); //Set_SCK();
        if(PioGet(PIO_SDA))//MOSI_H())
            RdPara |= 0x01;
    } 
    
    PioSet(PIO_SCL, FALSE); //Clr_SCK();
    Output_MOSI();
    PioSet(PIO_CSB, TRUE); //Set_nCS();				//*늪뇹꼇밑균nCS，賈痰젯崎친駕*
    PioSet(PIO_SDA, FALSE); //데이터를 보내지 않을때 SDA를 Low로 떨어뜨려놓기위한 처리, 이선빈 책임님 요구사항
    
    return(RdPara);
}

void rf_init(void)
{
    //pio 설정
    //PioSetModes((1UL << PIO_SCL) | (1UL << PIO_CSB) | (1UL << PIO_SDA), pio_mode_user);
    
    //PioSetDir(PIO_SCL, PIO_DIR_OUTPUT);
    //PioSetDir(PIO_CSB, PIO_DIR_OUTPUT);
    //PioSetDir(PIO_SDA, PIO_DIR_OUTPUT); 

	//#define PIO_SCL     P13//(5)
	//#define PIO_CSB     P10//(6)
	//#define PIO_SDA     P14//(7)

	P13_Quasi_Mode;	//SCL
	P10_Quasi_Mode;	//CSB
	P14_Quasi_Mode;	//SDA
	
	PIO_SCL = 0;
	PIO_CSB = 0;
	PIO_SDA = 0;
	
    
    //주파수 인덱스 설정
#ifdef RF_MODE_447_9_KOR    
    FreqSel = SELECT_FREQ_447_9;
#elif defined RF_MODE_429_JPN
    FreqSel = SELECT_FREQ_429;
#endif    

	//RF 칩 User Register 디폴트값 초기화
    set_default_register();
    
    //RF 칩 환경설정 및 RX 상태로 변경
    RFM219A_EntryRx();
}

//RFPDK값 초기설정값과 비교확인하는 함수
void compare_RFPDK_config(void)
{
    int i;
    byte rf_data;

    //DebugWriteString("RFM219FreqTbl :");
	
	/*
	//Send_Data_To_UART0('R');
		  //Send_Data_To_UART0('F');
	//Send_Data_To_UART0(':');
	*/
    for(i=0;i<9;i++) 
    {
		/*
		//Send_Data_To_UART0('[');
			//Send_UINT8_To_UART0(FreqSel-1);
			//Send_Data_To_UART0(']');
		  //Send_Data_To_UART0('[');
			//Send_UINT8_To_UART0(i);
			//Send_Data_To_UART0(']');
			//Send_Data_To_UART0(' ');
		//Send_UINT8_To_UART0(RFM219FreqTbl[FreqSel-1][i]>>8);
		  //Send_UINT8_To_UART0(RFM219FreqTbl[FreqSel-1][i] & 0x00ff);
		//Send_Data_To_UART0(' ');
		*/
      rf_data = SPI3Read(RFM219FreqTbl[i] >> 8);
      
      if(rf_data != (RFM219FreqTbl[i] & 0x00ff))
      {
		/*  
		  //Send_Data_To_UART0('[');
			//Send_UINT8_To_UART0(FreqSel-1);
			//Send_Data_To_UART0(']');
		  //Send_Data_To_UART0('[');
			//Send_UINT8_To_UART0(i);
			//Send_Data_To_UART0(']');
			//Send_Data_To_UART0(' ');
		//Send_UINT8_To_UART0(RFM219FreqTbl[FreqSel-1][i]>>8);
		  //Send_UINT8_To_UART0(RFM219FreqTbl[FreqSel-1][i] & 0x00ff);
		//Send_Data_To_UART0(' ');
		
		  //Send_UINT8_To_UART0(RFM219FreqTbl[FreqSel-1][i]>>8);
		  //Send_UINT8_To_UART0(rf_data);
		  //Send_Data_To_UART0(' ');
		  //Send_Data_To_UART0('d');
		  //Send_Data_To_UART0(' ');
		  //Send_Data_To_UART0('v');
		  //Send_Data_To_UART0('a');
		  //Send_Data_To_UART0('l');
		  //Send_Data_To_UART0('*');
		  //Send_Data_To_UART0('*');
		  
		  //Send_UINT8_To_UART0(RFM219FreqTbl[FreqSel-1][i]>>8);
		  //Send_UINT8_To_UART0(RFM219FreqTbl[FreqSel-1][i] & 0x00ff);
		  //Send_Data_To_UART0('\r');
		  //Send_Data_To_UART0('\n');
		  */
           g_bReqeust_RF_init = TRUE;
		  //
      }
	  /*
	  else
	  {
		  //Send_Data_To_UART0('\r');
		  //Send_Data_To_UART0('\n');
	  }
	  */
    }
    //DebugWriteString("\r\n");
    //DebugWriteString("RFM219ConfigTbl :\r\n");
	
	/*
	 //Send_Data_To_UART0('\r');
		  //Send_Data_To_UART0('\n');
    
	//Send_Data_To_UART0('R');
		  //Send_Data_To_UART0('F');
	//Send_Data_To_UART0(':');
	*/
    for(i=0;i<52;i++)
    {
		/*
		 //Send_Data_To_UART0('[');
			//Send_UINT8_To_UART0(i);
			//Send_Data_To_UART0(']');
			//Send_Data_To_UART0(' ');
		//Send_UINT8_To_UART0(RFM219ConfigTbl[i]>>8);
		  //Send_UINT8_To_UART0(RFM219ConfigTbl[i] & 0x00ff);
		//Send_Data_To_UART0(' ');
		*/
		
      rf_data = SPI3Read(RFM219ConfigTbl[i] >> 8);
      
      if(rf_data != (RFM219ConfigTbl[i] & 0x00ff))
      {
		  /*
		  //Send_Data_To_UART0('[');
			//Send_UINT8_To_UART0(i);
			//Send_Data_To_UART0(']');
			//Send_Data_To_UART0(' ');
		//Send_UINT8_To_UART0(RFM219ConfigTbl[i]>>8);
		  //Send_UINT8_To_UART0(RFM219ConfigTbl[i] & 0x00ff);
		//Send_Data_To_UART0(' ');
		  
		  //Send_UINT8_To_UART0(RFM219ConfigTbl[i]>>8);
		  //Send_UINT8_To_UART0(rf_data);
		  //Send_Data_To_UART0(' ');
		  //Send_Data_To_UART0('d');
		  //Send_Data_To_UART0(' ');
		  //Send_Data_To_UART0('v');
		  //Send_Data_To_UART0('a');
		  //Send_Data_To_UART0('l');
		  //Send_Data_To_UART0('*');
		  //Send_Data_To_UART0('*');
		  //Send_UINT8_To_UART0(RFM219ConfigTbl[i]>>8);
		  //Send_UINT8_To_UART0(RFM219ConfigTbl[i] & 0x00ff);
		  //Send_Data_To_UART0('\r');
		  //Send_Data_To_UART0('\n');
		  */
           g_bReqeust_RF_init = TRUE;
      }
	  /*
	  else
	  {
		  //Send_Data_To_UART0('\r');
		  //Send_Data_To_UART0('\n');
	  }
	  */
      
      //DebugWriteString(", ");
      //DebugWriteString("\r\n");
    }
}

//User Register 디폴트값 쓰기
void set_default_register(void)
{
    RFM219A_Sleep();				//쏵흙慷추친駕
    RFM219A_Standby();				//쏵흙STB榴檄 
    
    SPI3Write(0x3F00);
    SPI3Write(0x4000);
    SPI3Write(0x4100);
    SPI3Write(0x4200);
    SPI3Write(0x4300);
    SPI3Write(0x4400);
    SPI3Write(0x4500);
    SPI3Write(0x4604);
    SPI3Write(0x4700);
    SPI3Write(0x4f00);
}


void RF_Check_proc(void)
{
	//////////////RF 설정값 읽기 및 초기값과 비교하여 다를경우 초기화 처리
	byte rf_data=0;
	
	if( m_is_rf_reinit == FALSE ){
		//RF 칩 Pio, 주파수 관련 초기 설정
		rf_init();
		
		//RF 칩 User Register 디폴트값 초기화
		set_default_register();
		
		//RF 칩 환경설정 및 RX 상태로 변경
		RFM219A_EntryRx();
		
		m_is_rf_reinit = TRUE;
		////Send_Data_To_UART0('R');
		////Send_Data_To_UART0('I');
	}
	

	rf_data = SPI3Read(0x3f);

	if(rf_data != 0)
		g_bReqeust_RF_init = TRUE;
	
	rf_data = SPI3Read(0x40);
	if(rf_data != 0)
		g_bReqeust_RF_init = TRUE;
	
	rf_data = SPI3Read(0x41);
	if(rf_data != 0)
		g_bReqeust_RF_init = TRUE;

	rf_data = SPI3Read(0x43);
	if(rf_data != 0)
		g_bReqeust_RF_init = TRUE;

	rf_data = SPI3Read(0x44);
	if(rf_data != 0)
		g_bReqeust_RF_init = TRUE;

	rf_data = SPI3Read(0x46);
	if(rf_data != 0x04)
		g_bReqeust_RF_init = TRUE;
	
	//RFPDK값 비교
	compare_RFPDK_config();
	
	//초기값과 다른경우 레지스터 초기화함
	if(g_bReqeust_RF_init == TRUE)
	{
		g_bReqeust_RF_init = FALSE;
		
		//RF 칩 Pio, 주파수 관련 초기 설정
		rf_init();
		
		//RF 칩 User Register 디폴트값 초기화
		set_default_register();
		
		//RF 칩 환경설정 및 RX 상태로 변경
		RFM219A_EntryRx();
		////Send_Data_To_UART0('I');
	}
}