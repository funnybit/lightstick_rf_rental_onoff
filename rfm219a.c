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

byte FreqSel;   //林颇荐 汲沥 牢郸胶

extern bool m_is_rf_reinit;

int g_bReqeust_RF_init;   //RF_init 夸没 函荐
//#define MIN_SELECT_FREQ_INDEX   1
//#define MAX_SELECT_FREQ_INDEX   6

//SelectFreq 161228 扁霖
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
	  {0x00C0, 0x0122, 0x022C, 0x039B, 0x0407, 0x0D2E, 0x110A, 0x2956, 0x3292, 0x3BEB},   // 447.9 57400bps(荐沥肯丰)
	//{0x0080, 0x0122, 0x022C, 0x039B, 0x0407, 0x0D1E, 0x110F, 0x2956, 0x3292, 0x3BEB},   // 447.9 19200 bps(荐沥肯丰)
	  //{0x00A0, 0x0122, 0x022C, 0x039B, 0x0407, 0x0D2E, 0x110A, 0x2956, 0x3292, 0x3BEB},   // 447.9 38400 bps(荐沥肯丰)
    {0x0062, 0x0121, 0x0238, 0x0358, 0x0400, 0x0D1E, 0x1114, 0x2956, 0x328E, 0x3BEB},   //161124 429MHz
    //{0x0062, 0x0121, 0x0266, 0x0366, 0x0406, 0x0D1E, 0x1114, 0x2956, 0x3290, 0x3BEB},   //161124 433.92MHz
    //{0x0062, 0x0122, 0x0233, 0x038F, 0x0407, 0x0D1E, 0x1114, 0x2956, 0x3292, 0x3BEB},   //161228 447.862MHz
    //{0x0062, 0x0122, 0x02ED, 0x03AA, 0x0407, 0x0D1E, 0x1114, 0x2956, 0x328F, 0x3BEB},   //161228 447.950MHz
    //{0x0062, 0x0122, 0x0296, 0x03B6, 0x0407, 0x0D1E, 0x1114, 0x2956, 0x328F, 0x3BEB},   //161228 447.987MHz
};
*/

const word RFM219FreqTbl[10] = {
	0x00C0, 0x0122, 0x022C, 0x039B, 0x0407, 0x0D2E, 0x110A, 0x2956, 0x3292, 0x3BEB   // 447.9 57400bps(荐沥肯丰)
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
**函数名称：RFM219_EntryRx
**函数功能：RFM219设置进入接收
**输入参数：无
**输出参数：0——状态异常
******************************************************************************/
byte RFM219A_EntryRx(void)
{
    byte tmp;
 
    //抹 券版汲沥
    RFM219A_Config();
 
    //Operating state RX 惑怕肺 函版
    tmp = SPI3Read((byte)(OP_MODE>>8));		//基本配置
    tmp &= OP_MASK;
    SPI3Write(OP_MODE+tmp+OP_RX);

    return(1);				//返回正常
}

/******************************************************************************
**函数名称：RFM219_Sleep
**函数功能：RFM219睡眠
**输入参数：无
**输出参数：无
******************************************************************************/
void RFM219A_Sleep(void)
{
    byte tmp;	
    tmp = SPI3Read((byte)(OP_MODE>>8));
    tmp &= OP_MASK;
    SPI3Write(OP_MODE+tmp+OP_SLEEP);
}

/******************************************************************************
**函数名称：RFM219_Standby
**函数功能：RFM219设置进入Standby
**输入参数：无
**输出参数：无
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
**函数名称：RFM219_Config
**函数功能：初始化，后进入Standby模式
**输入参数：无
**输出参数：无
******************************************************************************/
void RFM219A_Config(void)
{
    byte i;	

    RFM219A_Sleep();				//state甫 sleep栏肺 函版
    RFM219A_Standby();				//state甫 standby栏肺 函版 
 
    //林颇荐 汲沥 静扁
    for(i=0;i<9;i++) 
        SPI3Write(RFM219FreqTbl[i]);
    //RFPDK 券版汲沥蔼 静扁
    for(i=0;i<52;i++)
        SPI3Write(RFM219ConfigTbl[i]);

    RFM219A_ClearIRQ();             //IRQ努府绢
    RFM219A_Standby();				//state甫 standby栏肺 函版 
}
/*
void PioSet(int x, int y)
{
	
}
*/
/******************************************************************************
**函数名称：SPI3Write
**函数功能：SPI写入一个16数据（高8位地址，低8位数据）
**输入参数：WrPara
**输出参数：无
******************************************************************************/
void SPI3Write(word WrPara)								
{                                                       
    byte bitcnt;    
    
    //Output 汲沥
    Output_MOSI();			//SDA为输出
    
    PioSet(PIO_SCL, FALSE);//Clr_SCK();				//注意SCK先清0，保持低
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
**函数名称：SPI3Read
**函数功能：SPI读取一个地址数据
**输入参数：adr
**输出参数：无
******************************************************************************/
byte SPI3Read(byte adr)
{
    byte tmp;	
    SPI3Cmd8bit(0x80+adr);			//地址
    tmp = SPI3Read8bit();	
	
	//Debug msg
	////Send_UINT8_To_UART0(tmp);
	////Send_Data_To_UART0('\r');
	////Send_Data_To_UART0('\n');
    PioSet(PIO_CSB, TRUE); //Set_nCS();
    return(tmp);
}  					

/******************************************************************************
**函数名称：SPI3Cmd8bit
**函数功能：SPI写入参数8bit
**输入参数：WrPara
**输出参数：无
**注意：    保留nCS输出Low
******************************************************************************/
void SPI3Cmd8bit(byte WrPara)
{
    byte bitcnt;	
    
    PioSet(PIO_SDA, FALSE); //单捞磐甫 焊郴瘤 臼阑锭 SDA甫 Low肺 冻绢哆妨初扁困茄 贸府, 捞急后 氓烙丛 夸备荤亲
    Output_MOSI();			//SDA为输出
    
    PioSet(PIO_SCL, FALSE);//Clr_SCK();				//注意SCK先清0，保持低
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
**函数名称：SPI3Read8bitt
**函数功能：SPI读取参数8bit
**输入参数：读取8bit数据——RdPara
**输出参数：无
**注意：    保留nCS输出Low
******************************************************************************/
byte SPI3Read8bit(void)
{
    byte RdPara = 0;
    byte bitcnt;
    
    PioSet(PIO_SDA, FALSE); //单捞磐甫 焊郴瘤 臼阑锭 SDA甫 Low肺 冻绢哆妨初扁困茄 贸府, 捞急后 氓烙丛 夸备荤亲
    PioSet(PIO_CSB, FALSE); //Clr_nCS(); 
    Input_MOSI();			//读FIFO，维持SDI为H		
    
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
    PioSet(PIO_CSB, TRUE); //Set_nCS();				//*此处不关闭nCS，使用连续模式*
    PioSet(PIO_SDA, FALSE); //单捞磐甫 焊郴瘤 臼阑锭 SDA甫 Low肺 冻绢哆妨初扁困茄 贸府, 捞急后 氓烙丛 夸备荤亲
    
    return(RdPara);
}

void rf_init(void)
{
    //pio 汲沥
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
	
    
    //林颇荐 牢郸胶 汲沥
#ifdef RF_MODE_447_9_KOR    
    FreqSel = SELECT_FREQ_447_9;
#elif defined RF_MODE_429_JPN
    FreqSel = SELECT_FREQ_429;
#endif    

	//RF 抹 User Register 叼弃飘蔼 檬扁拳
    set_default_register();
    
    //RF 抹 券版汲沥 棺 RX 惑怕肺 函版
    RFM219A_EntryRx();
}

//RFPDK蔼 檬扁汲沥蔼苞 厚背犬牢窍绰 窃荐
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

//User Register 叼弃飘蔼 静扁
void set_default_register(void)
{
    RFM219A_Sleep();				//进入睡眠模式
    RFM219A_Standby();				//进入STB状态 
    
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
	//////////////RF 汲沥蔼 佬扁 棺 檬扁蔼苞 厚背窍咯 促甫版快 檬扁拳 贸府
	byte rf_data=0;
	
	if( m_is_rf_reinit == FALSE ){
		//RF 抹 Pio, 林颇荐 包访 檬扁 汲沥
		rf_init();
		
		//RF 抹 User Register 叼弃飘蔼 檬扁拳
		set_default_register();
		
		//RF 抹 券版汲沥 棺 RX 惑怕肺 函版
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
	
	//RFPDK蔼 厚背
	compare_RFPDK_config();
	
	//檬扁蔼苞 促弗版快 饭瘤胶磐 檬扁拳窃
	if(g_bReqeust_RF_init == TRUE)
	{
		g_bReqeust_RF_init = FALSE;
		
		//RF 抹 Pio, 林颇荐 包访 檬扁 汲沥
		rf_init();
		
		//RF 抹 User Register 叼弃飘蔼 檬扁拳
		set_default_register();
		
		//RF 抹 券版汲沥 棺 RX 惑怕肺 函版
		RFM219A_EntryRx();
		////Send_Data_To_UART0('I');
	}
}