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

#define FLASH_VERTICAL      BASE_ADDR+1  // ����    
#define FLASH_HORIZONTAL    BASE_ADDR+2  // ����
#define FLASH_FLOOR         BASE_ADDR+3  // ��
#define FLASH_LINE          BASE_ADDR+4  // ����
#define FLASH_SHAPE         BASE_ADDR+5  // ���
#define FLASH_MYGROUP       BASE_ADDR+6  // �׷�
#define FLASH_LOGO1         BASE_ADDR+7  // ����1
#define FLASH_LOGO2         BASE_ADDR+8  // ����2
#define FLASH_LOGO3         BASE_ADDR+9  // ����3
#define FLASH_SHOW_VER      BASE_ADDR+10 // ��������
#define FLASH_SHOW_VER_DAY  BASE_ADDR+11 // ��������_��¥
#define FLASH_GROUP2  BASE_ADDR+12 // �׷�2
#define FLASH_GROUP3  BASE_ADDR+13 // �׷�3
#define FLASH_GROUP4  BASE_ADDR+14 // �׷�4
#define FLASH_GROUP5  BASE_ADDR+15 // �׷�5
#define FLASH_GROUP6  BASE_ADDR+16 // �׷�6
#define FLASH_GROUP7  BASE_ADDR+17 // �׷�7
#define FLASH_GROUP8  BASE_ADDR+18 // �׷�8
#define FLASH_GROUP9  BASE_ADDR+19 // �׷�9
#define FLASH_GROUP10  BASE_ADDR+20 // �׷�10
#define FLASH_GROUP11  BASE_ADDR+21 // �׷�11
#define FLASH_GROUP12  BASE_ADDR+22 // �׷�12
#define FLASH_GROUP13  BASE_ADDR+23 // �׷�13
#define FLASH_GROUP14  BASE_ADDR+24 // �׷�14
#define FLASH_GROUP15  BASE_ADDR+25 // �׷�15
#define FLASH_GROUP16  BASE_ADDR+26 // �׷�16
#define FLASH_GROUP17 BASE_ADDR+27 // �׷�17
#define FLASH_GROUP18  BASE_ADDR+28 // �׷�18
#define FLASH_GROUP19  BASE_ADDR+29 // �׷�19
#define FLASH_GROUP20  BASE_ADDR+30 // �׷�20
#define FLASH_GROUP21  BASE_ADDR+31 // �׷�21
#define FLASH_DUMMY         BASE_ADDR+32 //

enum{
    FLASH_CAL_LOW_SIDE,
    FLASH_CAL_HIGH_SIDE,
};

typedef struct{
	uint16_t    u16CalLowside;      //������ Ķ���극�̼� ������
    uint16_t    u16CalHighside;     //���� Ķ���극�̼� ������

    uint8_t     u8IsCalExist;       //Calibration �� �����ϴ��� üũ�ϴ� ����, 0-Ķ���극�̼� ������ ����. 1-������������, 2-���� ����, 3-������,��������.


} DataFlash_t;

// ** defined funtions
extern void Write_Dataflash_Byte(UINT16 u16_addr,UINT8 u8_data);
extern void DataFlash_Erase(uint16_t   u16Addr);
extern UINT8 read_APROM_BYTE(UINT16 code *u16_addr);
extern void Set_Dust_Data(UINT16 u16_addr,UINT8 u8_data);
extern void Get_Dust_Cal_Data(uint16_t *u16Buff, uint8_t u8DataType);
extern void DataFlash_WriteByte(uint16_t   u16Addr);

#endif  //_USERUART_H_
