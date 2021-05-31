/*******************************************************************************
1. Project Name     : NIDS_DustSensor_76E003
2. Author	        : 
3. Company	        : 
4. A Drafter	    : Christopher, Lee ( DongSeok, Lee )
5. Homepage	        : 
6. Email	        : strchrislee@gmail.com
7. Phone Number     : 010-2464-0102
8. Filename	        : UserUart.h
9. Version	        : 1.0
10. CreatedDate     : 2017/06/26
11. ModifiedDate	: 
12. License	        : Heumtech
13. MCU Type	    : N76E003
14. Clock Frequency : 
15. Add Source Name :
16. Compiler	    : 
17. SubMenu	        :
*******************************************************************************/
#ifndef _USERDATAFLASH_H_
#define _USERDATAFLASH_H_
// Defined defines

#define BASE_ADDR   0x4700

#define FLASH_CALIBRATION_L_LOW     BASE_ADDR+51     //저감도 캘리브레이션 LOW BYTE
#define FLASH_CALIBRATION_L_HIGH    BASE_ADDR+52     //저감도 캘리브레이션 HIGH BYTE
#define FLASH_CALIBRATION_H_LOW     BASE_ADDR+53     //고감도 캘리브레이션 LOW BYTE
#define FLASH_CALIBRATION_H_HIGH    BASE_ADDR+54     //고감도 캘리브레이션 HIGH BYT

enum{
    FLASH_CAL_LOW_SIDE,
    FLASH_CAL_HIGH_SIDE,
};

typedef struct{
	uint16_t    u16CalLowside;      //저감도 캘리브레이션 데이터
	uint16_t    u16CalHighside;     //고감도 캘리브레이션 데이터

	uint8_t     u8IsCalExist;       //Calibration 이 존재하는지 체크하는 변수, 0-캘리브레이션 데이터 없음. 1-저감도만있음, 2-고감도 있음, 3-저감도,고감도있음.

} DataFlash_t;

// ** defined funtions
extern void write_DATAFLASH_BYTE(UINT16 u16_addr,UINT8 u8_data);
extern UINT8 read_APROM_BYTE(UINT16 code *u16_addr);
/*
extern void DataFlash_Erase(uint16_t   u16Addr);
extern UINT8 read_APROM_BYTE(UINT16 code *u16_addr);
extern void Set_Dust_Data(UINT16 u16_addr,UINT8 u8_data);
extern void Get_Dust_Cal_Data(uint16_t *u16Buff, uint8_t u8DataType);
extern void DataFlash_WriteByte(uint16_t   u16Addr);
*/
#endif  //_USERUART_H_
