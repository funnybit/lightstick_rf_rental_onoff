/*******************************************************************************
1. Project Name     : 
2. Author	        : 
3. Company	        : 
4. A Drafter	    : Christopher, Lee ( DongSeok, Lee )
5. Homepage	        : 
6. Email	        : strchrislee@gmail.com
7. Phone Number     : 010-2464-0102
8. Filename	        : UserDataFlash.h
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

#define FLASH_VERTICAL      BASE_ADDR+1  // 세로    
#define FLASH_HORIZONTAL    BASE_ADDR+2  // 가로
#define FLASH_FLOOR         BASE_ADDR+3  // 층
#define FLASH_LINE          BASE_ADDR+4  // 라인
#define FLASH_SHAPE         BASE_ADDR+5  // 모양
#define FLASH_MYGROUP       BASE_ADDR+6  // 그룹
#define FLASH_LOGO1         BASE_ADDR+7  // 글자1
#define FLASH_LOGO2         BASE_ADDR+8  // 글자2
#define FLASH_LOGO3         BASE_ADDR+9  // 글자3
#define FLASH_SHOW_VER      BASE_ADDR+10 // 공연버전
#define FLASH_SHOW_VER_DAY  BASE_ADDR+11 // 공연버전_날짜
#define FLASH_GROUP2  BASE_ADDR+12 // 그룹2
#define FLASH_GROUP3  BASE_ADDR+13 // 그룹3
#define FLASH_GROUP4  BASE_ADDR+14 // 그룹4
#define FLASH_GROUP5  BASE_ADDR+15 // 그룹5
#define FLASH_GROUP6  BASE_ADDR+16 // 그룹6
#define FLASH_GROUP7  BASE_ADDR+17 // 그룹7
#define FLASH_GROUP8  BASE_ADDR+18 // 그룹8
#define FLASH_GROUP9  BASE_ADDR+19 // 그룹9
#define FLASH_GROUP10  BASE_ADDR+20 // 그룹10
#define FLASH_GROUP11  BASE_ADDR+21 // 그룹11
#define FLASH_GROUP12  BASE_ADDR+22 // 그룹12
#define FLASH_GROUP13  BASE_ADDR+23 // 그룹13
#define FLASH_GROUP14  BASE_ADDR+24 // 그룹14
#define FLASH_GROUP15  BASE_ADDR+25 // 그룹15
#define FLASH_GROUP16  BASE_ADDR+26 // 그룹16
#define FLASH_GROUP17 BASE_ADDR+27 // 그룹17
#define FLASH_GROUP18  BASE_ADDR+28 // 그룹18
#define FLASH_GROUP19  BASE_ADDR+29 // 그룹19
#define FLASH_GROUP20  BASE_ADDR+30 // 그룹20
#define FLASH_GROUP21  BASE_ADDR+31 // 그룹21
#define FLASH_DUMMY         BASE_ADDR+32 //

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
extern void Write_Dataflash_Byte(UINT16 u16_addr,UINT8 u8_data);
extern void DataFlash_Erase(uint16_t   u16Addr);
extern UINT8 read_APROM_BYTE(UINT16 code *u16_addr);
extern void Set_Dust_Data(UINT16 u16_addr,UINT8 u8_data);
extern void Get_Dust_Cal_Data(uint16_t *u16Buff, uint8_t u8DataType);
extern void DataFlash_WriteByte(uint16_t   u16Addr);

#endif  //_USERUART_H_
