/*******************************************************************************
1. Project Name     :
2. Author	        : 
3. Company	        : 
4. A Drafter	    : Christopher, Lee ( DongSeok, Lee )
5. Homepage	        : 
6. Email	        : strchrislee@gmail.com
7. Phone Number     : 010-2464-0102
8. Filename	        : UserDefine.h
9. Version	        : 1.0
10. CreatedDate     : 2017/06/27
11. ModifiedDate	: 
12. License	        : Heumtech
13. MCU Type	    : N76E003
14. Clock Frequency : 
15. Add Source Name :
16. Compiler	    : 
17. SubMenu	        :
*******************************************************************************/
#ifndef _USERDEFINE_H_
#define _USERDEFINE_H_

typedef struct{
	uint16_t    u16CalLowside;      //������ Ķ���극�̼� ������
    uint16_t    u16CalHighside;     //���� Ķ���극�̼� ������

    uint8_t    u8CalLowside;      //������ Ķ���극�̼� ������

    uint8_t     u8IsCalExist;       //Calibration �� �����ϴ��� üũ�ϴ� ����, 0-Ķ���극�̼� ������ ����. 1-������������, 2-���� ����, 3-������,��������.


} System_t;

#define DEFINE_FIRMWARE_VERSION    1

#define DEFINE_BIT_SET  1
#define DEFINE_BIT_CLR  0

#define BIT_SET 1
#define BIT_CLR 0

#define Set_All_GPIO_Input_Mode()   {P0M1=0xFF;P0M1=0;P1M1=0xFF;P1M2=0;P3M1=0xFF;P3M2=0;}

#define SET_GLOBAL_INTERRUPT_ENABLE()   {set_EA;};
#define CLR_GLOBAL_INTERRUPT_ENABLE()   {clr_EA;};

#endif  //_USERDEFINE_H_
