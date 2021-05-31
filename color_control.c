#include "Common.h"
#include "N76E003.h"
#include "Function_define.h"
#include "SFR_Macro.h"

#include <stdlib.h>

#include "PWM.h"
#include "main.h"
#include "color_control.h"
#include "UART.h"

BONG_INFO_T ccData; //todo ���ʿ�?
SceneWorkMode sceneMode;

extern UINT16 readBatteryValue;


COLOR_DIMMING_DATA_T colorDimmingData;
BLINK_DATA_T blinkData;

extern uint8 rainbowWhere;

uint8 groupCheckMy;

uint8 nowPlayShowNum;              // ���� ����Ǿ�� �� �ܹ��� ������ �ε���
uint8 maxPlayShowNum;              // �ִ�� ����Ǿ�� �� �ܹ��� ������ �ִ��
uint16 nowShowSpeedAdjustCount;    // ����Ǵ� �������� �ӵ������� ���� ī��Ʈ
uint16 maxShowSpeedAdjustCount;    // ����Ǵ� �������� �ӵ������� ���� ī������ �ִ��

extern void setColor(uint8 color);

uint8 myShowNum=0;
bool dmxBlink;

bool g_bRainbow_color_start = FALSE; //������ ���� ����
uint8 g_nRainbow_counter = 0; //������ ǥ�� ī��Ʈ ����
bool g_bPattern_Repeat_flag = FALSE;    //�ݺ�����

uint8 color_dimming_repeat_mode[MAX_DIMMING_REPEAT_COLOR * 4] = {  //��� �ݺ� �÷��� ���� (��ֻ��� �ӽ������ ���ۻ�, ������)
    0x00, 0x00, 0x00, BRIGHT_MAX_VALUE,
    0x00, 0x00, 0x00, BRIGHT_MAX_VALUE
}; //todo ���ʿ�?


//todo ��ġ�̵�?
bool runAction;

//todo ��ġ�̵�?
//LED �����ϰ��ִ� Ÿ�̸� ������ ���� ��ų�� ���
void stopAction(void) {
    //DEBUG_MSG_STR("stopAction \r\n");
    runAction = FALSE;      //�� �÷��׸� Ȯ���Ͽ� Ÿ�̸Ӹ� ������ϴµ� FALSE�϶��� ���̻� Ÿ�̸� ������ ��������
    //setEventLedState(LED_EVENT_STATE_IDLE);
    bongStatus.syncCalculer = FALSE;	
    bongStatus.syncCounter = 0;
    bongStatus.continueRunCalculer = FALSE;
    bongStatus.continueCounter = 0;
    bongStatus.runActionWithSync = FALSE;
}

//todo ��ġ�̵�?
//LED ������۽� ����Ͽ� LED ���� Ÿ�̸Ӱ� �ݺ��ǰ���
void startAction(void) {
    runAction = TRUE;
}

UINT8 get_color_rainbow_mode_R(UINT8 index)
{
	UINT8 ret = 0;
	
	switch(index)
	{
		case 0:
		ret = 0xff;
		break;
		case 1:
		ret = 0xff; 
		break;
		case 2:
		ret = 0xff; 
		break;
		case 3:
		ret = 0x00; 
		break;
		case 4:
		ret = 0x01; 
		break;
		case 5:
		ret = 0x00; 
		break;
		case 6:
		ret = 0x80;
		break;
		default:
			ret = 0;
		break;
	}
	
	return ret;
}

UINT8 get_color_rainbow_mode_G(UINT8 index)
{
	UINT8 ret = 0;
	
	switch(index)
	{
		case 0:
		ret = 0x00;
		break;
		case 1:
		ret = 0x40; 
		break;
		case 2:
		ret = 0x60; 
		break;
		case 3:
		ret = 0xff; 
		break;
		case 4:
		ret = 0xa8; 
		break;
		case 5:
		ret = 0x00; 
		break;
		case 6:
		ret = 0x00;
		break;
		default:
			ret = 0;
		break;
	}
	
	return ret;
}


UINT8 get_color_rainbow_mode_B(UINT8 index)
{
	UINT8 ret = 0;
	
	switch(index)
	{
		case 0:
		ret = 0x00;
		break;
		case 1:
		ret = 0x00; 
		break;
		case 2:
		ret = 0x00; 
		break;
		case 3:
		ret = 0x00; 
		break;
		case 4:
		ret = 0xdc; 
		break;
		case 5:
		ret = 0xff; 
		break;
		case 6:
		ret = 0x80;
		break;
		default:
			ret = 0;
		break;
	}
	
	return ret;
}

/*
UINT8 get_color_rainbow_mode_R_btn(UINT8 index)
{
	UINT8 ret = 0;
	
	switch(index)
	{
		case 0:
		ret = 255;  // 40
		break;
		case 1:
		ret = 255;   // 40
		break;
		case 2:
		ret = 255;    // 250
		break;
		case 3:
		ret = 80;   // 250
		break;
		case 4:
		ret = 50;    // 250
		break;
		case 5:
		ret = 50;   // 200
		break;
		case 6:
		ret = 255;  // 200
		break;
		default:
			ret = 0;
		break;
	}
	
	return ret;
}

UINT8 get_color_rainbow_mode_G_btn(UINT8 index)
{
	UINT8 ret = 0;
	
	switch(index)
	{
		case 0:
		ret = 50;   // 245
		break;
		case 1:
		ret = 130;   // 245
		break;
		case 2:
		ret = 255;   // 245
		break;
		case 3:
		ret = 235;   // 245
		break;
		case 4:
		ret = 30;   // 245
		break;
		case 5:
		ret = 30;    // 50
		break;
		case 6:
		ret = 10;    // 50
		break;
		default:
			ret = 0;
		break;
	}
	
	return ret;
}


UINT8 get_color_rainbow_mode_B_btn(UINT8 index)
{
	UINT8 ret = 0;
	
	switch(index)
	{
		case 0:
		ret = 15;   // 15
		break;
		case 1:
		ret = 15;   // 15
		break;
		case 2:
		ret = 10;   // 10
		break;
		case 3:
		ret = 10;   // 10
		break;
		case 4:
		ret = 255;  // 10
		break;
		case 5:
		ret = 255;  // 15
		break;
		case 6:
		ret = 255;   // 15
		break;
		default:
			ret = 0;
		break;
	}
	
	return ret;
}
*/

UINT8 get_color_rainbow_mode_R_btn(UINT8 index)
{
	UINT8 ret = 0;
	
	switch(index)
	{
		case 0:
		ret = 255;  // 40
		break;
		case 1:
		ret = 255;   // 40
		break;
		case 2:
		ret = 255;    // 250
		break;
		case 3:
		ret = 255;   // 250
		break;
		case 4:
		ret = 100;    // 250
		break;
		case 5:
		ret = 50;   // 200
		break;
		case 6:
		ret = 80;  // 200
		break;
		default:
			ret = 0;
		break;
	}
	
	return ret;
}

UINT8 get_color_rainbow_mode_G_btn(UINT8 index)
{
	UINT8 ret = 0;
	
	switch(index)
	{
		case 0:
		ret = 50;   // 245
		break;
		case 1:
		ret = 100;   // 245
		break;
		case 2:
		ret = 255;   // 245
		break;
		case 3:
		ret = 255;   // 245
		break;
		case 4:
		ret = 255;   // 245
		break;
		case 5:
		ret = 50;    // 50
		break;
		case 6:
		ret = 50;    // 50
		break;
		default:
			ret = 0;
		break;
	}
	
	return ret;
}


UINT8 get_color_rainbow_mode_B_btn(UINT8 index)
{
	UINT8 ret = 0;
	
	switch(index)
	{
		case 0:
		ret = 15;   // 15
		break;
		case 1:
		ret = 15;   // 15
		break;
		case 2:
		ret = 10;   // 10
		break;
		case 3:
		ret = 10;   // 10
		break;
		case 4:
		ret = 80;  // 10
		break;
		case 5:
		ret = 255;  // 15
		break;
		case 6:
		ret = 200;   // 15
		break;
		default:
			ret = 0;
		break;
	}
	
	return ret;
}

void get_user_color_table_R(UINT8 index)
{
	switch(index)
	{
		case 0:
		blinkData.currentR = 0xff;
		blinkData.currentG = 0xff;
		blinkData.currentB = 0xff;
		break;
		case 1:
		blinkData.currentR = 0xff; 
		blinkData.currentG = 0x00;
		blinkData.currentB = 0x00; 
		break;
		case 2:
		blinkData.currentR = 0xff; 
		blinkData.currentG = 0x5b; 
		blinkData.currentB = 0x00; 
		break;
		case 3:
		blinkData.currentR = 0xff; 
		blinkData.currentG = 0x8b; 
		blinkData.currentB = 0x00; 
		break;
		case 4:
		blinkData.currentR = 0xff; 
		blinkData.currentG = 0xc8; 
		blinkData.currentB = 0x00; 
		break;
		case 5:
		blinkData.currentR = 0xff; 
		blinkData.currentG = 0xf8; 
		blinkData.currentB = 0x00; 
		break;
		case 6:
		blinkData.currentR = 0xe0;
		blinkData.currentG = 0xff;
		blinkData.currentB = 0x00; 
		break;
		case 7:
		blinkData.currentR = 0xa8;
		blinkData.currentG = 0xff;
		blinkData.currentB = 0x00; 
		break;
		case 8:
		blinkData.currentR = 0x7d;
		blinkData.currentG = 0xff;
		blinkData.currentB = 0x00; 
		break;
		case 9:
		blinkData.currentR = 0x3f;
		blinkData.currentG = 0xff;
		blinkData.currentB = 0x00; 
		break;
		case 10:
		blinkData.currentR = 0x00;
		blinkData.currentG = 0xff;
		blinkData.currentB = 0x00; 
		break;
		case 11:
		blinkData.currentR = 0x00;
		blinkData.currentG = 0xff;
		blinkData.currentB = 0x30;
		break;
		case 12:
		blinkData.currentR = 0x00;
		blinkData.currentG = 0xff;
		blinkData.currentB = 0x73;
		break;
		case 13:
		blinkData.currentR = 0x00;
		blinkData.currentG = 0xff;
		blinkData.currentB = 0xa3;
		break;
		case 14:
		blinkData.currentR = 0x00;
		blinkData.currentG = 0xff;
		blinkData.currentB = 0xe0;
		break;
		case 15:
		blinkData.currentR = 0x00;
		blinkData.currentG = 0xff;
		blinkData.currentB = 0xff;
		break;
		case 16:
		blinkData.currentR = 0x00;
		blinkData.currentG = 0xc9;
		blinkData.currentB = 0xff;
		break;
		case 17:
		blinkData.currentR = 0x00;
		blinkData.currentG = 0x92;
		blinkData.currentB = 0xff;
		break;
		case 18:
		blinkData.currentR = 0x00;
		blinkData.currentG = 0x62;
		blinkData.currentB = 0xff;
		break;
		case 19:
		blinkData.currentR = 0x00;
		blinkData.currentG = 0x4f;
		blinkData.currentB = 0xff;
		break;
		case 20:
		blinkData.currentR = 0x00;
		blinkData.currentG = 0x00;
		blinkData.currentB = 0xff;
		break;
		case 21:
		blinkData.currentR = 0x44;
		blinkData.currentG = 0x00;
		blinkData.currentB = 0xff;
		break;
		case 22:
		blinkData.currentR = 0x6f;
		blinkData.currentG = 0x00;
		blinkData.currentB = 0xff;
		break;
		case 23:
		blinkData.currentR = 0xa7;
		blinkData.currentG = 0x00;
		blinkData.currentB = 0xff;
		break;
		case 24:
		blinkData.currentR = 0xe6;
		blinkData.currentG = 0x00;
		blinkData.currentB = 0xff;
		break;
		case 25:
		blinkData.currentR = 0xff;
		blinkData.currentG = 0x00;
		blinkData.currentB = 0xf3;
		break;
		case 26:
		blinkData.currentR = 0xff;
		blinkData.currentG = 0x00;
		blinkData.currentB = 0xb7;
		break;
		case 27:
		blinkData.currentR = 0xff;
		blinkData.currentG = 0x00;
		blinkData.currentB = 0x86;
		break;
		default:
		blinkData.currentR = 0;
		blinkData.currentG = 0;
		blinkData.currentB = 0;
		break;
	}
}


UINT8 get_random_call_color_list(UINT8 index)
{
	UINT8 ret = 0;
	
	switch(index)
	{
		case 0:
		ret = 23;
		break;
		case 1:
		ret = 18; 
		break;
		case 2:
		ret = 17; 
		break;
		case 3:
		ret = 21; 
		break;
		case 4:
		ret = 4; 
		break;
		case 5:
		ret = 13; 
		break;
		case 6:
		ret = 3;
		break;
		case 7:
		ret = 14;
		break;
		case 8:
		ret = 5;
		break;
		case 9:
		ret = 26;
		break;
		case 10:
		ret = 22;
		break;
		case 11:
		ret = 24;
		break;
		case 12:
		ret = 7;
		break;
		case 13:
		ret = 25;
		break;
		case 14:
		ret = 9;
		break;
		case 15:
		ret = 0;
		break;
		case 16:
		ret = 1;
		break;
		case 17:
		ret = 12;
		break;
		case 18:
		ret = 20;
		break;
		case 19:
		ret = 11;
		break;
		case 20:
		ret = 15;
		break;
		case 21:
		ret = 27;
		break;
		case 22:
		ret = 16;
		break;
		case 23:
		ret = 8;
		break;
		case 24:
		ret = 6;
		break;
		case 25:
		ret = 2;
		break;
		case 26:
		ret = 10;
		break;
		case 27:
		ret = 19;
		break;
		default:
			ret = 0;
		break;
	}
	
	return ret;
}

/**
  ����ڰ� ������ ������ 0,0,0 ���� ������ ������ ������ �����ϴ� ���� ����
  frameCount ���ʻ������ ���������� �����ϴµ� �ɸ��� Ƚ��
  mode �÷��� �ݺ� or ������ ���� �� ��带 ����
*/
void setActionDimmingLedOn(uint16 frameCount, uint8 mode) {

    ccData.previousRed = 0;
    ccData.previousGreen = 0;
    ccData.previousBlue = 0;
    dimmingColorChangeCal(frameCount, mode);
}

void setActionDimmingColorChange(uint16 frameCount, uint8 mode) {
    dimmingColorChangeCal(frameCount, mode);
}

/**
  2���� ������ �ݺ� ��� �ϴ� �Լ�
  min ��� 100�ۼ�Ʈ�� �ּҰ�
  max ��� 100�ۼ�Ʈ�� �ִ밪
  frameCount ���ʻ������ ���������� �����ϴµ� �ɸ��� Ƚ��
  colorSequenc ���� �迭�� ����
  repeat_CountOrignal �ݺ� Ƚ�� ����
*/
void setActionLedDimmingRepeat(uint8 persentMin, uint8 persentMax, uint16 frameCount, uint8 colorSequenc, uint8 repeat_CountOrignal){
    
    //�����÷�
    //previous color save
    uint16 calTemp = (ccData.r * persentMin) / 100;
    color_dimming_repeat_mode[0] = calTemp;
    calTemp = (ccData.g * persentMin) / 100;
    color_dimming_repeat_mode[1] = calTemp;
    calTemp = (ccData.b * persentMin) / 100;
    color_dimming_repeat_mode[2] = calTemp;
    
    //�����÷�
    //current color save
    calTemp = (ccData.r * persentMax) / 100;
    color_dimming_repeat_mode[4] = calTemp;
    calTemp = (ccData.g * persentMax) / 100;
    color_dimming_repeat_mode[5] = calTemp;
    calTemp = (ccData.b * persentMax) / 100;
    color_dimming_repeat_mode[6] = calTemp;
    
    //������ ����
    ccData.previousRed = color_dimming_repeat_mode[colorSequenc*4];
    ccData.previousGreen = color_dimming_repeat_mode[colorSequenc*4+1];
    ccData.previousBlue = color_dimming_repeat_mode[colorSequenc*4+2];

    //������ ī��Ʈ ����
    colorDimmingData.colorSequenc = colorSequenc;
    //������ ī��Ʈ�� �ӽ÷� �����Ͽ� �迭���� ���� ������ ���������� ����
    colorSequenc = colorSequenc + 1;
    if(colorSequenc > MAX_DIMMING_REPEAT_COLOR-1){
        colorSequenc = 0;
    }
    ccData.r = color_dimming_repeat_mode[colorSequenc*4];
    ccData.g = color_dimming_repeat_mode[colorSequenc*4+1];
    ccData.b = color_dimming_repeat_mode[colorSequenc*4+2];
    
    /*
    DEBUG_MSG_STR("previous = ");
    DEBUG_MSG_UINT8(ccData.previousRed);
    DEBUG_MSG_STR(",");
    DEBUG_MSG_UINT8(ccData.previousGreen);
    DEBUG_MSG_STR(",");
    DEBUG_MSG_UINT8(ccData.previousBlue);
    DEBUG_MSG_STR("\r\n"); 
    
    DEBUG_MSG_STR("ccData.r = ");
    DEBUG_MSG_UINT8(ccData.r);
    DEBUG_MSG_STR(",");
    DEBUG_MSG_UINT8(ccData.g);
    DEBUG_MSG_STR(",");
    DEBUG_MSG_UINT8(ccData.b);
    DEBUG_MSG_STR("\r\n\r\n");  
    */
    
    //������, �������� �÷����� ���� ���� ���
    dimmingColorChangeCal(frameCount, DIMMING_REPEAT);  //dimming repeat persent
    
	//�ݺ� Ƚ�� ����
    colorDimmingData.repeat_CountOrignal = repeat_CountOrignal;
    colorDimmingData.repeat_Count = 0;
}

/**
  ������ �÷��� ������ ������ �����ϴ� ���� ����(���ѹݺ�)
  frameCount ���ʻ������ ���������� �����ϴµ� �ɸ��� Ƚ��
  colorSequenc ���� �迭�� ����
*/
void setActionBtRainbowMode(uint16 frameCount, uint8 colorSequenc) {

    /* �����÷� ���� */
    //ccData.previousRed = color_rainbow_mode[colorSequenc*4];
    //ccData.previousGreen = color_rainbow_mode[colorSequenc*4+1];
    //ccData.previousBlue = color_rainbow_mode[colorSequenc*4+2];
	if( rainbowWhere == 2 ) {
		ccData.previousRed = get_color_rainbow_mode_R_btn(colorSequenc);
    ccData.previousGreen = get_color_rainbow_mode_G_btn(colorSequenc);
    ccData.previousBlue = get_color_rainbow_mode_B_btn(colorSequenc);
	}
	else {
	ccData.previousRed = get_color_rainbow_mode_R(colorSequenc);
    ccData.previousGreen = get_color_rainbow_mode_G(colorSequenc);
    ccData.previousBlue = get_color_rainbow_mode_B(colorSequenc);
	}
    colorDimmingData.colorSequenc = colorSequenc;
    colorSequenc = colorSequenc + 1;
    if(colorSequenc > MAX_RAINBOW_MODE_COLOR-1){
        colorSequenc = 0;
    }
    
    /* ���� �÷� ���� */
    //ccData.r = color_rainbow_mode[colorSequenc*4];
    //ccData.g = color_rainbow_mode[colorSequenc*4+1];
    //ccData.b = color_rainbow_mode[colorSequenc*4+2];
		if( rainbowWhere == 2 ) {
			ccData.r = get_color_rainbow_mode_R_btn(colorSequenc);
    ccData.g = get_color_rainbow_mode_G_btn(colorSequenc);
    ccData.b = get_color_rainbow_mode_B_btn(colorSequenc);
		}
		else {
	ccData.r = get_color_rainbow_mode_R(colorSequenc);
    ccData.g = get_color_rainbow_mode_G(colorSequenc);
    ccData.b = get_color_rainbow_mode_B(colorSequenc);
    }
    /* ������, �������� �÷����� ���� ���� ��� */
    dimmingColorChangeCal(frameCount, DIMMING_RAINBOW);  //rainbow mode
}



/**
  �ε巴�� ������/������ �ݺ� ����
  
  flag_onoff = DIMMING_SELECT_COLOR_ON // �ε巴�� ������
  flag_onoff = DIMMING_SELECT_COLOR_OFF // �ε巴�� ������
  frameCount led on / led off ���� �ð�
  repeat_CountOrignal // �ݺ� Ƚ�� ��
*/
void setActionLedDimmingRepeatOnOff(uint16 frameCount, uint8 flag_onoff, uint8 repeat_CountOrignal)
{
    if( flag_onoff == DIMMING_SELECT_COLOR_ON)
    {
        ccData.previousRed = 0;
        ccData.previousGreen = 0;
        ccData.previousBlue = 0;
        
        dimmingColorChangeCal(frameCount, DIMMING_SELECT_COLOR_ON);
    }
    else
    {
        ccData.previousRed = ccData.r;
        ccData.previousGreen = ccData.g;
        ccData.previousBlue = ccData.b;
        
        ccData.r = 0;
        ccData.g = 0;
        ccData.b = 0;
        
        dimmingColorChangeCal(frameCount, DIMMING_SELECT_COLOR_OFF);
    }
    
    //�ݺ� Ƚ�� ����
    colorDimmingData.repeat_CountOrignal = repeat_CountOrignal;
    colorDimmingData.repeat_Count = 0;
}

/**
  2�÷� �ε巴�� ��ȯ �ݺ� ����

  frameCount led on / led off ���� �ð�
*/
void setActionLedDimmingRepeatTwoColor(uint16 frameCount, uint8 colorSequenc, uint8 repeat_CountOrignal)
{
    //previous color save
    color_dimming_repeat_mode[0] = 0xff;
    color_dimming_repeat_mode[1] = 0;
    color_dimming_repeat_mode[2] = 0;
    
    //current color save
    color_dimming_repeat_mode[4] = 0;
    color_dimming_repeat_mode[5] = 0xff;
    color_dimming_repeat_mode[6] = 0;
    
    //������ ����
    ccData.previousRed = color_dimming_repeat_mode[colorSequenc*4];
    ccData.previousGreen = color_dimming_repeat_mode[colorSequenc*4+1];
    ccData.previousBlue = color_dimming_repeat_mode[colorSequenc*4+2];

    //������ ī��Ʈ ����
    colorDimmingData.colorSequenc = colorSequenc;
    
    //������ ī��Ʈ�� �ӽ÷� �����Ͽ� �迭���� ���� ������ ���������� ����
    colorSequenc = colorSequenc + 1;
    if(colorSequenc > MAX_DIMMING_REPEAT_COLOR-1){
        colorSequenc = 0;
    }
    ccData.r = color_dimming_repeat_mode[colorSequenc*4];
    ccData.g = color_dimming_repeat_mode[colorSequenc*4+1];
    ccData.b = color_dimming_repeat_mode[colorSequenc*4+2];
    
    //�ݺ� Ƚ�� ����
    colorDimmingData.repeat_CountOrignal = repeat_CountOrignal;
    colorDimmingData.repeat_Count = 0;
    
    //������, �������� �÷����� ���� ���� ���
    dimmingColorChangeCal(frameCount, DIMMING_REPEAT_TWO_COLOR);  //dimming repeat persent
}




/**
  A������ B������ ����� �ʿ��� ������ �Ѵ�.
  ex -> A���� B���� ���� �ؾ��� ��
        ���� or ���һ��� ���� ����
  
  frameCount ���ʻ������ ���������� �����ϴµ� �ɸ��� Ƚ��
  mode ������ ��ȭ�� ����(��� �ϰ� �ݺ�, ����� ���� ���)
*/
void dimmingColorChangeCal(uint16 frameCount, uint8 mode) {
    //DEBUG_MSG_STR("dimmingColorChangeCal() \r\n");
    
    colorDimmingData.patten = mode;
    
    colorDimmingData.increaseOrderBase = FALSE;
    if(frameCount != 0){
        colorDimmingData.baseCountOrignal = frameCount; //0~100���� Ƚ��
    }
    colorDimmingData.baseCount = colorDimmingData.baseCountOrignal;

    /* ���� �÷� ���� */
    colorDimmingData.startColorR = ccData.previousRed;
    colorDimmingData.startColorG = ccData.previousGreen;
    colorDimmingData.startColorB = ccData.previousBlue;
    /* ���� �÷� ���� */
    colorDimmingData.endColorR = ccData.r;
    colorDimmingData.endColorG = ccData.g;
    colorDimmingData.endColorB = ccData.b;
    /* ���� �÷� ���� - �߰��� ��ȭ�ϴ°��� ccData�� �������� �ʴ´�*/
    colorDimmingData.currentR = ccData.previousRed;
    colorDimmingData.currentG = ccData.previousGreen;
    colorDimmingData.currentB = ccData.previousBlue;
    colorDimmingData.currentBright = BRIGHT_MAX_VALUE;  

    /* ���� or ���� ���¸� ���Ѵ� */
    //RED
    if(colorDimmingData.startColorR < colorDimmingData.endColorR){
        colorDimmingData.increaseOrderR = TRUE;
        colorDimmingData.gapR_Temp = colorDimmingData.endColorR - colorDimmingData.startColorR;
    } else {
        colorDimmingData.increaseOrderR = FALSE;
        colorDimmingData.gapR_Temp = colorDimmingData.startColorR - colorDimmingData.endColorR;
    }
    /* ������ ���� ���س��� */
    colorDimmingData.gapR = colorDimmingData.gapR_Temp / colorDimmingData.baseCountOrignal;
    colorDimmingData.randomGapR = colorDimmingData.baseCountOrignal / (colorDimmingData.gapR_Temp % colorDimmingData.baseCountOrignal);  //�ұ�Ģ ������ ���´�

    /*
      �ϴܿ� ���� �������� gapR���� �����ϰ� ������ �ϰ� randomGapR���� �̿��Ͽ� randomGapRȽ���� �ѹ��� 1�� ������Ų��.
      
        colorDimmingData.currentR = colorDimmingData.currentR + colorDimmingData.gapR;  //gap �� ��ŭ �����ϰ� ���Ѵ�.
        if(colorDimmingData.baseCount % colorDimmingData.randomGapR == 0){   //�ұ�Ģ ī��Ʈ�϶� 1�� �� ������Ų��.
            colorDimmingData.currentR = colorDimmingData.currentR + 1;
        }
        
      1. gapR ���� 0�ϰ�� randomGapR Ƚ���� �ѹ��� 1������(õõ�� �÷��� ���ϴ°��)
      2. gapR ���� 0 �̻� ū��� ������ ������ ��ȭ�Ѵٰ� �� �� �ִ�.

      1.�� ������ ��� (1�� ������ 0�� ����..)
      000100010001000100010001 �̿Ͱ��� �������� ���� ���� - ����
      2.�� ������ ���
      333433343334333433343334 �̿Ͱ��� �������� ���� ���� - ����
    */
    /*
    DEBUG_MSG_STR("colorDimmingData.gapR = ");
    DEBUG_MSG_UINT8(colorDimmingData.gapR);
    DEBUG_MSG_STR(" colorDimmingData.randomGapR = ");
    DEBUG_MSG_UINT8(colorDimmingData.randomGapR);
    DEBUG_MSG_STR("\r\n"); 
    */
    
    //GREEN
    if(colorDimmingData.startColorG < colorDimmingData.endColorG){
        colorDimmingData.increaseOrderG = TRUE;
        colorDimmingData.gapG_Temp = colorDimmingData.endColorG - colorDimmingData.startColorG;
    } else {
        colorDimmingData.increaseOrderG = FALSE;
        colorDimmingData.gapG_Temp = colorDimmingData.startColorG - colorDimmingData.endColorG;
    }
    colorDimmingData.gapG = colorDimmingData.gapG_Temp / colorDimmingData.baseCountOrignal;
    colorDimmingData.randomGapG = colorDimmingData.baseCountOrignal / (colorDimmingData.gapG_Temp % colorDimmingData.baseCountOrignal);  //�ұ�Ģ ������ ���´�
    
    
    //BLUE
    if(colorDimmingData.startColorB < colorDimmingData.endColorB){
        colorDimmingData.increaseOrderB = TRUE;
        colorDimmingData.gapB_Temp = colorDimmingData.endColorB - colorDimmingData.startColorB;
    } else {
        colorDimmingData.increaseOrderB = FALSE;
        colorDimmingData.gapB_Temp = colorDimmingData.startColorB - colorDimmingData.endColorB;
    }
    colorDimmingData.gapB = colorDimmingData.gapB_Temp / colorDimmingData.baseCountOrignal;
    colorDimmingData.randomGapB = colorDimmingData.baseCountOrignal / (colorDimmingData.gapB_Temp % colorDimmingData.baseCountOrignal);  //�ұ�Ģ ������ ���´�

 

    /*
    DEBUG_MSG_STR("gapR = ");
    DEBUG_MSG_UINT8(colorDimmingData.gapR);
    DEBUG_MSG_STR("\r\n");
    DEBUG_MSG_STR("gapR_Temp = ");
    DEBUG_MSG_UINT8(colorDimmingData.gapR_Temp);
    DEBUG_MSG_STR("\r\n");
    DEBUG_MSG_STR("randomGapR = ");
    DEBUG_MSG_UINT8(colorDimmingData.randomGapR);
    DEBUG_MSG_STR("\r\n");
    
    DEBUG_MSG_STR("gapG = ");
    DEBUG_MSG_UINT8(colorDimmingData.gapG);
    DEBUG_MSG_STR("\r\n");
    DEBUG_MSG_STR("gapG_Temp = ");
    DEBUG_MSG_UINT8(colorDimmingData.gapG_Temp);
    DEBUG_MSG_STR("\r\n");
    DEBUG_MSG_STR("randomGapG = ");
    DEBUG_MSG_UINT8(colorDimmingData.randomGapG);
    DEBUG_MSG_STR("\r\n");
    
    DEBUG_MSG_STR("gapB = ");
    DEBUG_MSG_UINT8(colorDimmingData.gapB);
    DEBUG_MSG_STR("\r\n");
    DEBUG_MSG_STR("gapB_Temp = ");
    DEBUG_MSG_UINT8(colorDimmingData.gapB_Temp);
    DEBUG_MSG_STR("\r\n");
    DEBUG_MSG_STR("randomGapB = ");
    DEBUG_MSG_UINT8(colorDimmingData.randomGapB);
    DEBUG_MSG_STR("\r\n");
    */
    
}




/**
  ����� ������ �ݺ� ������
  
  frameCount led on / led off ���� �ð�
*/
void setActionAllUserColorBlink(uint16 frameCount){
    blinkData.type = TYPE_ALL_USER_COLOR_BLINK;
    blinkData.colorPatternCount = 0;
    blinkData.colorPatternCountMax = 1;
    
    blinkData.onCount = 0;
    blinkData.onCountOrignal = frameCount;
    blinkData.offCount = 0;
    blinkData.offCountOrignal = frameCount;
}
/**
  ����� ��� �ٸ� ���� �÷� ������
  
  frameCount led on / led off ���� �ð�
*/
void setActionAllEachRandomColorBlink(uint16 frameCount){
    blinkData.type = TYPE_ALL_EACH_RANDOM_COLOR_BLINK;
    blinkData.colorPatternCount = 0;
    blinkData.colorPatternCountMax = 2;
    
    blinkData.onCount = 0;
    blinkData.onCountOrignal = frameCount;
    blinkData.offCount = 0;
    blinkData.offCountOrignal = frameCount;
}

/*
void setActionAllUserColorRandomTimeBlink(uint16 frameCount){
    blinkData.type = TYPE_ALL_USER_COLOR_RANDOM_TIME_BLINK;
    blinkData.colorPatternCount = 0;
    blinkData.colorPatternCountMax = 1;   
	
	srand(readBatteryValue);
    blinkData.onCount = 0;
    blinkData.onCountOrignal = rand()%25 + 1; // (Random16()%frameCount) + 1; //�Ź� �������� �ֱ⸦ �����ϵ� frame count�̳��� �ֱ⸦ ����
    blinkData.offCount = 0;
    blinkData.offCountOrignal = blinkData.onCountOrignal;
	
	Send_Data_To_UART0('W');
}
*/

/**
  ����� ������ �������� �÷� ���� ������
  
  frameCount led on / led off ���� �ð�
*/
void setActionAllRandomColorBlink(uint16 frameCount){
    blinkData.type = TYPE_ALL_RANDOM_COLOR_BLINK;
    blinkData.colorPatternCount = 0;
    blinkData.colorPatternCountMax = MAX_USER_COLOR;
    
    blinkData.onCount = 0;
    blinkData.onCountOrignal = frameCount;
    blinkData.offCount = 0;
    blinkData.offCountOrignal = frameCount;
}

/**
  ��ư Ŭ���� ������ ȭ��Ʈ ����
  
  frameCount led on / led off ���� �ð�
*/
void setActionBtnWhiteBlinkFastMode(uint16 frameCount){
    blinkData.type = TYPE_BTN_WHITE_BLINK_FAST_MODE;
    blinkData.colorPatternCount = 0;
    blinkData.colorPatternCountMax = MAX_BTN_WHITE_BLINK_COLOR;
    
    blinkData.onCount = 0;
    blinkData.onCountOrignal = frameCount;
    blinkData.offCount = 0;
    blinkData.offCountOrignal = frameCount;
}
/**
  ��ư Ŭ���� ������ ȭ��Ʈ ����
  
  frameCount led on / led off ���� �ð�
*/
void setActionBtnWhiteBlinkSlowMode(uint16 frameCount){
    blinkData.type = TYPE_BTN_WHITE_BLINK_SLOW_MODE;
    blinkData.colorPatternCount = 0;
    blinkData.colorPatternCountMax = MAX_BTN_WHITE_BLINK_COLOR;
    
    blinkData.onCount = 0;
    blinkData.onCountOrignal = frameCount;
    blinkData.offCount = 0;
    blinkData.offCountOrignal = frameCount;
}



/**
  ��ŷ�Ҷ� �ݺ� ȣ��Ǹ� ī��Ʈ ���� and �÷��� ���� �� �÷�led�� ������.
  
  1. �÷�����
  2. ī��Ʈ ����
  3. on / offǥ�� �� ���Ϲݺ����� ����
*/
void actionBlink(void){

	//Send_Data_To_UART0('1');
    //Ÿ�Կ� ���� �÷� ����
    if(blinkData.onCount == 0){
		
		//Send_Data_To_UART0('2');
               
        switch(blinkData.type)
        {
            /* Ʈ���̽� �÷� ������ �ݺ� ���� */
            case TYPE_TWO_COLOR_CHANGE:
            {
                if( blinkData.colorPatternCount == 0){
                    blinkData.currentR = 0xff;
                    blinkData.currentG = 0x00;
                    blinkData.currentB = 0x86;
                    blinkData.currentBright = BRIGHT_MAX_VALUE;
                } else {
                    blinkData.currentR = 0xff;
                    blinkData.currentG = 0x50;
                    blinkData.currentB = 0x00;                
                    blinkData.currentBright = BRIGHT_MAX_VALUE;
                }
            }
            break;
            /* ����� ���� �÷� �� ���� */
            case TYPE_ALL_USER_COLOR_BLINK:
			//case TYPE_ALL_USER_COLOR_RANDOM_TIME_BLINK:
            {
                //DEBUG_MSG_STR("TYPE_ALL_USER_COLOR_BLINK \r\n");
                blinkData.currentR = ccData.r;
                blinkData.currentG = ccData.g;
                blinkData.currentB = ccData.b;
                blinkData.currentBright = ccData.bright;
            }
            break;
            /* �����ϰ� �÷� ���̺� ���� */
            case TYPE_ALL_EACH_RANDOM_COLOR_BLINK:
            {
                //blinkData.colorPatternTemp = (Random16()%MAX_USER_COLOR) * 4;
				//blinkData.colorPatternTemp = (RF_check_counter%MAX_USER_COLOR) * 4; //todo �Լ� �ʿ�
				blinkData.colorPatternTemp = (RF_check_counter%MAX_USER_COLOR); //todo �Լ� �ʿ�
                //Send_Data_To_UART0('1');
                /*
                DEBUG_MSG_STR("blinkData.colorPatternTemp = ");
                DEBUG_MSG_UINT8(blinkData.colorPatternTemp);
                DEBUG_MSG_STR("\r\n");
                */
				
                //blinkData.currentR = user_color_table[blinkData.colorPatternTemp];
                //blinkData.currentG = user_color_table[blinkData.colorPatternTemp+1];
                //blinkData.currentB = user_color_table[blinkData.colorPatternTemp+2];
                //blinkData.currentBright = user_color_table[blinkData.colorPatternTemp+3];
//				blinkData.currentR = get_user_color_table_R(blinkData.colorPatternTemp);
//                blinkData.currentG = get_user_color_table_G(blinkData.colorPatternTemp);
//                blinkData.currentB = get_user_color_table_B(blinkData.colorPatternTemp);
//                blinkData.currentBright = BRIGHT_MAX_VALUE;
				get_user_color_table_R(blinkData.colorPatternTemp);
            }
            break;
            /* �����ϰ� �÷� ���̺� ���� */
            case TYPE_ALL_RANDOM_COLOR_BLINK:
            {
                //blinkData.colorPatternTemp = random_call_color_list[blinkData.colorPatternCount] * 4;
				//blinkData.colorPatternTemp = random_call_color_list[blinkData.colorPatternCount];
				blinkData.colorPatternTemp = get_random_call_color_list(blinkData.colorPatternCount);
                
                /*
                DEBUG_MSG_STR("blinkData.colorPatternCount = ");
                DEBUG_MSG_UINT8(blinkData.colorPatternCount);
                DEBUG_MSG_STR("\r\n");
                DEBUG_MSG_STR("blinkData.colorPatternTemp = ");
                DEBUG_MSG_UINT8(blinkData.colorPatternTemp);
                DEBUG_MSG_STR("\r\n");
                */
                //blinkData.currentR = user_color_table[blinkData.colorPatternTemp];
                //blinkData.currentG = user_color_table[blinkData.colorPatternTemp+1];
                //blinkData.currentB = user_color_table[blinkData.colorPatternTemp+2];
                //blinkData.currentBright = user_color_table[blinkData.colorPatternTemp+3];
//				blinkData.currentR = get_user_color_table_R(blinkData.colorPatternTemp);
//                blinkData.currentG = get_user_color_table_G(blinkData.colorPatternTemp);
//                blinkData.currentB = get_user_color_table_B(blinkData.colorPatternTemp);
//                blinkData.currentBright = BRIGHT_MAX_VALUE;
				get_user_color_table_R(blinkData.colorPatternTemp);
            }
            break;
            /* ��ưŬ�� ȭ��Ʈ �÷� �� ����(on off ����) */
            case TYPE_BTN_WHITE_BLINK_SLOW_MODE:
            case TYPE_BTN_WHITE_BLINK_FAST_MODE:
            {
                //blinkData.currentR = color_btn_white_blink_mode[blinkData.colorPatternCount*4];
                //blinkData.currentG = color_btn_white_blink_mode[blinkData.colorPatternCount*4+1];
                //blinkData.currentB = color_btn_white_blink_mode[blinkData.colorPatternCount*4+2];
                //blinkData.currentBright = color_btn_white_blink_mode[blinkData.colorPatternCount*4+3];
				if(blinkData.colorPatternCount == 0)
				{
					blinkData.currentR = 255;
					blinkData.currentG = 255;
					blinkData.currentB = 255;
				}
				else
				{
					blinkData.currentR = 0;
					blinkData.currentG = 0;
					blinkData.currentB = 0;
				}
					
            }
            break;
            default :
            break;
        }
    }
	
	//Send_Data_To_UART0('3');
    
    /* ī��Ʈ ���� */
    if(blinkData.onCount < blinkData.onCountOrignal){
        /* led on ���� */
        blinkData.onCount = blinkData.onCount + 1;
        //DEBUG_MSG_STR("LED ON \r\n");
        blinkData.ledState = TRUE;
    } else {
        /* led off ���� */
        if(blinkData.offCount < blinkData.offCountOrignal){
            blinkData.offCount = blinkData.offCount + 1;
            //DEBUG_MSG_STR("LED OFF \r\n");
            blinkData.ledState = FALSE;
        } else {
            /* �ʱ�ȭ */
            blinkData.offCount = 0;
            blinkData.onCount = 0;
			
			/*
			if(blinkData.type == TYPE_ALL_USER_COLOR_RANDOM_TIME_BLINK)
            {
				srand(readBatteryValue);
                blinkData.onCountOrignal = rand()%25 + 1; //�Ź� �������� �ֱ⸦ �����ϵ� frame count�̳��� �ֱ⸦ ����
                blinkData.offCountOrignal = blinkData.onCountOrignal;
            }
			*/
            
            blinkData.colorPatternCount = blinkData.colorPatternCount + 1;
            
            /* Ÿ�Կ� ���� ���� ���ʺ��� �����ϴ� ��� */
            if(blinkData.type == TYPE_TWO_COLOR_CHANGE || 
               blinkData.type == TYPE_ALL_RANDOM_COLOR_BLINK || 
               //blinkData.type == TYPE_BT_CALL_NOTI ||
               blinkData.type == TYPE_BTN_WHITE_BLINK_FAST_MODE ||
               blinkData.type == TYPE_BTN_WHITE_BLINK_SLOW_MODE 
               //blinkData.type == TYPE_BT_PARTY_MODE ||
               //blinkData.type == TYPE_FLOOR1_TWO_COLOR_CHANGE ||
               //blinkData.type == TYPE_FLOOR2_TWO_COLOR_CHANGE
               )
            {
                if( blinkData.colorPatternCount >= blinkData.colorPatternCountMax ){
                    blinkData.colorPatternCount = 0;
                }
            }
            /* Ÿ�Կ� ���� ���� ��� ������ ����Ǵ� ��� */
            //if(blinkData.type == TYPE_BT_CONNECT || blinkData.type == TYPE_BT_SMS_NOTI || blinkData.type == TYPE_BTN_RESET_MODE){
            //    if( blinkData.colorPatternCount >= blinkData.colorPatternCountMax ){
                    //�̺�Ʈ ���� �� therapy, party, alwayson ���¿��ٸ� �����Ѵ�.
            //        lastEventCheck();
            //    }
            //}
        }
    }

    /* led �÷� ǥ�� */
    if(blinkData.ledState == TRUE){
        //IOTLightControlDeviceSetColor(blinkData.currentR, blinkData.currentG, blinkData.currentB, blinkData.currentBright);
		//todo IOTLightControlDeviceSetColor
		//if(blinkData.currentR == 255 && blinkData.currentG == 255 && blinkData.currentB == 255)
			//PWM_change_duty(0, 0, 0, 255);
		//else
			//PWM_change_duty(blinkData.currentR, blinkData.currentG, blinkData.currentB, 0);
		PWM_change_duty(blinkData.currentR, blinkData.currentG, blinkData.currentB);
    } else {
        //IOTLightControlDeviceSetColor(0, 0, 0, 0);
		//todo IOTLightControlDeviceSetColor
		PWM_change_duty(0, 0, 0);
    }   
}

// LED ����
void actionLedOff(void) {
	//IOTLightControlDeviceSetColor(0, 0, 0, 0);
	PWM_change_duty(0,0,0);
}

// ��Ʈ�ѷ����� ������ ���� �ѱ�
void actionLedOnWithSelectedColor(void) {
	//IOTLightControlDeviceSetColor(ccData.r, ccData.g, ccData.b, ccData.bright);
	//if(ccData.r == 255 && ccData.g == 255 && ccData.b == 255)
	//	PWM_change_duty(0, 0, 0, 255);
	//else
    //Send_UINT8_To_UART0('C');
	PWM_change_duty(ccData.r, ccData.g, ccData.b);
}

/**
  �����÷� �ε巴�� ������ �ݺ� ����
  
  flag_onoff = DIMMING_SELECT_COLOR_ON // �ε巴�� ������
  flag_onoff = DIMMING_SELECT_COLOR_OFF // �ε巴�� ������
  frameCount led on / led off ���� �ð�
  repeat_CountOrignal // �ݺ� Ƚ�� ��
*/
void setActionLedDimmingRepeatOnOff_CurColor(uint16 frameCount, uint8 flag_onoff, uint8 repeat_CountOrignal)
{
    if( flag_onoff == DIMMING_SELECT_COLOR_ON)
    {
        ccData.previousRed = 0;
        ccData.previousGreen = 0;
        ccData.previousBlue = 0;
        
        dimmingColorChangeCal(frameCount, DIMMING_SELECT_COLOR_ON);
    }
    else
    {
        ccData.previousRed = g_nCur_r;
        ccData.previousGreen = g_nCur_g;
        ccData.previousBlue = g_nCur_b;
        
        ccData.r = 0;
        ccData.g = 0;
        ccData.b = 0;
        
        dimmingColorChangeCal(frameCount, DIMMING_SELECT_COLOR_OFF);
    }
    
    //�ݺ� Ƚ�� ����
    colorDimmingData.repeat_CountOrignal = repeat_CountOrignal;
    colorDimmingData.repeat_Count = 0;
}

void setActionInteractive_1_color(uint16 frameCount,uint8 m_nInteractive_frame_counter_init, uint8 m_nmaxPlayShowNum,uint8 m_nFrame_index)
{
    nowPlayShowNum = m_nInteractive_frame_counter_init;  //������ �ʱ�ȭ
    maxPlayShowNum = m_nmaxPlayShowNum;   //�ܰ� ����
    myShowNum = m_nFrame_index;    //�ش� ���⿡�� ���° ���ǿ� �����ؾ��ϴ����� ���� �ε��� ����
    
    maxShowSpeedAdjustCount = frameCount; //Ÿ�̸� ��ȸ���� �ѹ��� �����ϰ� �Ұ����� ����
    nowShowSpeedAdjustCount = frameCount;   //ó�� 1ȸ�� ��� �����ϱ� ����
}

void setActionShowByDMX(void) {
    maxShowSpeedAdjustCount = 35 - ccData.runSpeed;   //Ÿ�̸� ��ȸ���� �ѹ��� �����ϰ� �Ұ����� ����
    nowShowSpeedAdjustCount = 35 - ccData.runSpeed;   //ó�� 1ȸ�� ��� �����ϱ� ����
}

// 1���� ������������ ���� ������
// myShowNum : ���� ���� ��ȣ��
uint8 action_Increase_On_1_color(void)
{    
    //�ӵ� ������ ���� �� �����Ӹ��� maxShowSpeedAdjustCount ��ŭ ī���� �Ѵ��� ����    
    // 15���̴�.
    // nowShowSpeedAdjustCount 1�� �ø��鼭 15�� �Ǹ� �Ʒ� ��ƾ�� ����
    if(nowShowSpeedAdjustCount >= maxShowSpeedAdjustCount)
    {
        nowShowSpeedAdjustCount=0;
        nowPlayShowNum++;
        
        if((myShowNum <= nowPlayShowNum) && (myShowNum >= 1))
        {
            //ǥ��
            //IOTLightControlDeviceSetColor(ccData.r,ccData.g, ccData.b, ccData.bright);
					PWM_change_duty(ccData.r, ccData.g, ccData.b);
            
            //��ü ǥ�� �Ϸ�
            return 1;
        }
        else
        {
            //��ŸƮ �������� 0,0,0�� Ŵ
            //IOTLightControlDeviceSetColor(0, 0, 0, ccData.bright);
					PWM_change_duty(0, 0, 0);
        }
        
        
        //������ ���ܺ��� ū��� ���̻� ���� �ʴ´�
        if(nowPlayShowNum >= maxPlayShowNum)
        {
            return 1;
        }
        
    }
    else
    {
        nowShowSpeedAdjustCount++;
    }
    
    return 0;
}

//1���� ������������ ���� ������
uint8 action_Increase_Off_1_color(void)
{
    
    //�ӵ� ������ ���� �� �����Ӹ��� maxShowSpeedAdjustCount ��ŭ ī���� �Ѵ��� ����      
    if(nowShowSpeedAdjustCount >= maxShowSpeedAdjustCount)
    {
        nowShowSpeedAdjustCount=0;
        
        nowPlayShowNum++;

        //1�� �ּҰ��̱� ������ 1~n�ܰ������ ǥ�ø� �ؾ��Ѵ�.
        if((myShowNum <= nowPlayShowNum) && (myShowNum >= 1))
        {
            //ǥ��
            //IOTLightControlDeviceSetColor(0, 0, 0, ccData.bright);
					PWM_change_duty(0, 0, 0);
            
            //��ü ǥ�� �Ϸ�
            return 1;
        }
        //�̿��� ��� ó��
        else if((myShowNum > maxPlayShowNum) || (myShowNum == 0))
        {
            //����ó��
            //IOTLightControlDeviceSetColor(0, 0, 0, ccData.bright);
					PWM_change_duty(0, 0, 0);
        }
        
        //������ ���ܺ��� ū��� ���̻� ���� �ʴ´�
        if(nowPlayShowNum >= maxPlayShowNum)
        {
            return 1;
        }
        
    }
    else
    {
        nowShowSpeedAdjustCount++;
    }
    
    return 0;
}

// DMX�� �����ѱ�(������� �ʿ���)
// 1 �̸� ����
// 0 �̸� ���
uint8 actionLedOnByDMX(void) {
    //DebugWriteString("d_color_on\r\n");
   
    
    // ��ְ��� ������� ����.    
    groupInfoCmd.dmxDPercent = (groupInfoCmd.dmxD * 4) / 10;   
    
    // R ����
    groupInfoCmd.dmxRTemp = (groupInfoCmd.dmxR * groupInfoCmd.dmxDPercent) / 100;
    if( groupInfoCmd.dmxRTemp > 255 ) {
        groupInfoCmd.dmxRN = 255;
    }
    else {
        groupInfoCmd.dmxRN = (groupInfoCmd.dmxR * groupInfoCmd.dmxDPercent) / 100;
    }
    
    // G ����
    groupInfoCmd.dmxGTemp = (groupInfoCmd.dmxG * groupInfoCmd.dmxDPercent) / 100;
    if( groupInfoCmd.dmxGTemp > 255 ) {
        groupInfoCmd.dmxGN = 255;
    }
    else {
        groupInfoCmd.dmxGN = (groupInfoCmd.dmxG * groupInfoCmd.dmxDPercent) / 100;
    }
    
    // B ����
    groupInfoCmd.dmxBTemp = (groupInfoCmd.dmxB * groupInfoCmd.dmxDPercent) / 100;
    if( groupInfoCmd.dmxBTemp > 255 ) {
        groupInfoCmd.dmxBN = 255;
    }
    else {
        groupInfoCmd.dmxBN = (groupInfoCmd.dmxB * groupInfoCmd.dmxDPercent) / 100;
    }

    
    if( groupInfoCmd.dmxS == 0x0) {
        //IOTLightControlDeviceSetColor(groupInfoCmd.dmxR, groupInfoCmd.dmxG, groupInfoCmd.dmxB, ccData.bright);
        //IOTLightControlDeviceSetColor(groupInfoCmd.dmxRN, groupInfoCmd.dmxGN, groupInfoCmd.dmxBN, ccData.bright);
		PWM_change_duty(groupInfoCmd.dmxRN, groupInfoCmd.dmxGN, groupInfoCmd.dmxBN);
		
        return 1;
    }
    else {
        if(nowShowSpeedAdjustCount >= maxShowSpeedAdjustCount) {
            /*
            //DebugWriteString("SP:");
            //DebugWriteUint8(ccData.runSpeed);
            //DebugWriteString(",");
            //DebugWriteUint8(maxShowSpeedAdjustCount);
            //DebugWriteString("..\r\n");
            */
            
            nowShowSpeedAdjustCount=0;
        
            if( dmxBlink == TRUE ) {
                dmxBlink = FALSE;
                //IOTLightControlDeviceSetColor(groupInfoCmd.dmxRN, groupInfoCmd.dmxGN, groupInfoCmd.dmxBN, ccData.bright);
                //IOTLightControlDeviceSetColor(groupInfoCmd.dmxR, groupInfoCmd.dmxG, groupInfoCmd.dmxB, ccData.bright);
				PWM_change_duty(groupInfoCmd.dmxRN, groupInfoCmd.dmxGN, groupInfoCmd.dmxBN);
            }
            else {
                dmxBlink = TRUE;
                //IOTLightControlDeviceSetColor(0, 0, 0, 0);
				PWM_change_duty(0, 0, 0);
            }
        }
        else {
            nowShowSpeedAdjustCount++;
        }
        
        return 0;
    }
}



void setActionInteractive_Rainbow(uint16 frameCount,uint8 m_nInteractive_frame_counter_init, uint8 m_nmaxPlayShowNum,uint8 m_nFrame_index, bool m_bPattern_Repeat_flag)
{
    nowPlayShowNum = m_nInteractive_frame_counter_init;  //������ �ʱ�ȭ
    maxPlayShowNum = m_nmaxPlayShowNum;   //�ܰ� ����
    myShowNum = m_nFrame_index;    //�ش� ���⿡�� ���° ���ǿ� �����ؾ��ϴ����� ���� �ε��� ����
    
    maxShowSpeedAdjustCount = frameCount; //Ÿ�̸� ��ȸ���� �ѹ��� �����ϰ� �Ұ����� ����
    nowShowSpeedAdjustCount = frameCount;   //ó�� 1ȸ�� ��� �����ϱ� ����
    
    g_bRainbow_color_start = FALSE; //������ ���� ���� �ʱ�ȭ
    g_nRainbow_counter = 0; //������ ǥ�� ī��Ʈ ���� �ʱ�ȭ
    g_bPattern_Repeat_flag = m_bPattern_Repeat_flag;    //�ݺ����� ����
}

//������ ������������ ���� ������
uint8 action_Increase_On_Rainbow(void)
{    
    //�ӵ� ������ ���� �� �����Ӹ��� maxShowSpeedAdjustCount ��ŭ ī���� �Ѵ��� ����      
    if(nowShowSpeedAdjustCount >= maxShowSpeedAdjustCount)
    {
        nowShowSpeedAdjustCount=0;
        nowPlayShowNum++;
        
        //������ ���� ���� �Ǵ�
        //�������� ���۵Ǿ��°�?
        if(g_bRainbow_color_start == FALSE)
        {
            if((myShowNum <= nowPlayShowNum) && (myShowNum >= 1))
            {
                //������ ���� ����
                g_bRainbow_color_start = TRUE;
            }
            else
            {
                //��ŸƮ �������� 0,0,0�� Ŵ
                //IOTLightControlDeviceSetColor(0, 0, 0, ccData.bright);
				PWM_change_duty(0, 0, 0);
				
            }
            
            
            //������ ���ܺ��� ū��� ���̻� ���� �ʴ´�
            if(nowPlayShowNum > maxPlayShowNum)
            {
                return 1;
            }
        }
        
        //������ ��ŸƮ ǥ�� ���� ������ �����ߴ��� Ȯ��
        if(g_bRainbow_color_start == TRUE)
        {
            
            //�������� �� ǥ���� ��� �ݺ� �������� Ȯ���ؾ��Ѵ�.
            if(g_nRainbow_counter >= MAX_RAINBOW_MODE_COLOR)
            {
                //�ݺ� �����ΰ�쿡�� ó��������� �ٽ� ����
                if(g_bPattern_Repeat_flag == TRUE)
                {
                    g_nRainbow_counter=0;
                    //������ ǥ��
                    //IOTLightControlDeviceSetColor(color_rainbow_mode[g_nRainbow_counter*4],color_rainbow_mode[g_nRainbow_counter*4 + 1], color_rainbow_mode[g_nRainbow_counter*4 +2], ccData.bright);
					PWM_change_duty(get_color_rainbow_mode_R(g_nRainbow_counter), get_color_rainbow_mode_G(g_nRainbow_counter), get_color_rainbow_mode_B(g_nRainbow_counter));
                    g_nRainbow_counter++;
					
                }
                else
                {
                    //�ݺ��� ���� ��쿡�� ����
                    //IOTLightControlDeviceSetColor(0, 0, 0, ccData.bright);
					PWM_change_duty(0, 0, 0);
                    return 1;
                }
            }
            else
            {
                //������ ǥ��
                //IOTLightControlDeviceSetColor(color_rainbow_mode[g_nRainbow_counter*4],color_rainbow_mode[g_nRainbow_counter*4 + 1], color_rainbow_mode[g_nRainbow_counter*4 +2], ccData.bright);
				//PWM_change_duty(color_rainbow_mode[g_nRainbow_counter*4],color_rainbow_mode[g_nRainbow_counter*4 + 1], color_rainbow_mode[g_nRainbow_counter*4 +2]);
				PWM_change_duty(get_color_rainbow_mode_R(g_nRainbow_counter), get_color_rainbow_mode_G(g_nRainbow_counter), get_color_rainbow_mode_B(g_nRainbow_counter));
                
                g_nRainbow_counter++;
            }
        }
        
    }
    else
    {
        nowShowSpeedAdjustCount++;
    }
    
    return 0;
}

void setHorColor(uint8 g) {
	groupCheckMy = groupInfo.myGroup;   // �⺻������ 1�׷켳��
    
    if( g == 1 ) {
        groupCheckMy = groupInfo.myGroup;
    }
    else if( g == 2 ) {
        groupCheckMy = groupInfo.myGroup2;
    }
    else if( g == 3 ) {
        groupCheckMy = groupInfo.myGroup3;
    }
		else if( g == 4 ) {
        groupCheckMy = groupInfo.myGroup4;
    }
		else if( g == 5 ) {
        groupCheckMy = groupInfo.myGroup5;
    }
		else if( g == 6 ) {
        groupCheckMy = groupInfo.myGroup6;
    }
		else if( g == 7 ) {
        groupCheckMy = groupInfo.myGroup7;
    }
		else if( g == 8 ) {
        groupCheckMy = groupInfo.myGroup8;
    }
		else if( g == 9 ) {
        groupCheckMy = groupInfo.myGroup9;
    }
		else if( g == 10 ) {
        groupCheckMy = groupInfo.myGroup10;
    }
		else if( g == 11 ) {
        groupCheckMy = groupInfo.myGroup11;
    }
		else if( g == 12 ) {
        groupCheckMy = groupInfo.myGroup12;
    }
		else if( g == 13 ) {
        groupCheckMy = groupInfo.myGroup13;
    }
		else if( g == 14 ) {
        groupCheckMy = groupInfo.myGroup14;
    }
		else if( g == 15 ) {
        groupCheckMy = groupInfo.myGroup15;
    }
		else if( g == 16 ) {
        groupCheckMy = groupInfo.myGroup16;
    }
		else if( g == 17 ) {
        groupCheckMy = groupInfo.myGroup17;
    }
		else if( g == 18 ) {
         groupCheckMy = groupInfo.myGroup18;
    }
		else if( g == 19 ) {
         groupCheckMy = groupInfo.myGroup19;
    }
		else if( g == 20 ) {
         groupCheckMy = groupInfo.myGroup20;
    }
		else if( g == 21 ) {
         groupCheckMy = groupInfo.myGroup21;
    }
		
		// �Ķ�, ����, ����, ���, ���, �Ͼ�, ����ũ, ������
    // ����Ʈ���, �����, ����, ����Ʈ �׸�
    if( (groupCheckMy == 1) || (groupCheckMy == 8) || (groupCheckMy == 15) || (groupCheckMy == 22) ) {      // �Ķ�
        ccData.r = 0x0;
        ccData.g = 0x0;
        ccData.b = 0xff;
    }
    else if( (groupCheckMy == 2) || (groupCheckMy == 9) || (groupCheckMy == 16) || (groupCheckMy == 23) ) {  // ����
        ccData.r = 0xff;
        ccData.g = 0x0;
        ccData.b = 0x0;
    }
    else if( (groupCheckMy == 3) || (groupCheckMy == 10) || (groupCheckMy == 17) || (groupCheckMy == 24) ) {  // ���
        ccData.r = 0x0;
        ccData.g = 0xff;
        ccData.b = 0x0;
    }
    else if( (groupCheckMy == 4) || (groupCheckMy == 11) || (groupCheckMy == 18) || (groupCheckMy == 25) ) {  // ���
        ccData.r = 0xff;
        ccData.g = 0xff;
        ccData.b = 0x0;
    }
    else if( (groupCheckMy == 5) || (groupCheckMy == 12) || (groupCheckMy == 19) ) {  // ����
        ccData.r = 0xa7;
        ccData.g = 0x0;
        ccData.b = 0xff;
    }
    else if( (groupCheckMy == 6) || (groupCheckMy == 13) || (groupCheckMy == 20) ) {  // ������
        ccData.r = 0xff;
        ccData.g = 0x50;
        ccData.b = 0x0;
    }
    else if( (groupCheckMy == 7) || (groupCheckMy == 14) || (groupCheckMy == 21) ) {  // ��� //��ũ
        ccData.r = 0x0;
        ccData.g = 0xff;
        ccData.b = 0x0;
    }
    else {                                                                                            // ȭ��Ʈ
        ccData.r = 0xff;
        ccData.g = 0xff;
        ccData.b = 0xff;
    }
		/*
	if( (groupCheckMy == 1) || (groupCheckMy == 8) || (groupInfo.myGroup == 15) ) {      // �Ķ�
        ccData.r = 0x0;
        ccData.g = 0x0;
        ccData.b = 0xff;
    }
    else if( (groupInfo.myGroup == 2) || (groupInfo.myGroup == 9) || (groupInfo.myGroup == 16) ) {  // ����
        ccData.r = 0xff;
        ccData.g = 0x0;
        ccData.b = 0x0;
    }
    else if( (groupInfo.myGroup == 3) || (groupInfo.myGroup == 10) || (groupInfo.myGroup == 17) ) {  // �׸�
        ccData.r = 0x0;
        ccData.g = 0xff;
        ccData.b = 0x0;
    }
    else if( (groupInfo.myGroup == 4) || (groupInfo.myGroup == 11) || (groupInfo.myGroup == 18) ) {  // ���
        ccData.r = 0xff;
        ccData.g = 0xff;
        ccData.b = 0x0;
    }
    else if( (groupInfo.myGroup == 5) || (groupInfo.myGroup == 12) || (groupInfo.myGroup == 19) ) {  // ����
        ccData.r = 0xa7;
        ccData.g = 0x0;
        ccData.b = 0xff;
    }
    else if( (groupInfo.myGroup == 6) || (groupInfo.myGroup == 13) || (groupInfo.myGroup == 20) ) {  // ������
        ccData.r = 0xff;
        ccData.g = 0x50;
        ccData.b = 0x0;
    }
    else if( (groupInfo.myGroup == 7) || (groupInfo.myGroup == 14) || (groupInfo.myGroup == 21) ) {  // ���
        //ccData.r = 0xff;
        //ccData.g = 0x0;
        //ccData.b = 0x86;
			  ccData.r = 0x0;
        ccData.g = 0xff;
        ccData.b = 0x0;
    }
    else {                                                                                                   // ȭ��Ʈ
        ccData.r = 0xff;
        ccData.g = 0xff;
        ccData.b = 0xff;
    }
		*/
}