#include "Common.h"
#include "N76E003.h"
#include "Function_define.h"
#include "SFR_Macro.h"

#include <stdlib.h>

#include "PWM.h"
#include "main.h"
#include "color_control.h"
#include "UART.h"

BONG_INFO_T ccData; //todo 불필요?
SceneWorkMode sceneMode;

extern UINT16 readBatteryValue;


COLOR_DIMMING_DATA_T colorDimmingData;
BLINK_DATA_T blinkData;

extern uint8 rainbowWhere;

uint8 groupCheckMy;

uint8 nowPlayShowNum;              // 현재 재생되어야 할 단방향 프레임 인덱스
uint8 maxPlayShowNum;              // 최대로 재생되어야 할 단방향 프레임 최대수
uint16 nowShowSpeedAdjustCount;    // 재생되는 프레임의 속도조절을 위한 카운트
uint16 maxShowSpeedAdjustCount;    // 재생되는 프레임의 속도조절을 위한 카운터의 최대수

extern void setColor(uint8 color);

uint8 myShowNum=0;
bool dmxBlink;

bool g_bRainbow_color_start = FALSE; //무지개 시작 변수
uint8 g_nRainbow_counter = 0; //무지개 표시 카운트 변수
bool g_bPattern_Repeat_flag = FALSE;    //반복여부

uint8 color_dimming_repeat_mode[MAX_DIMMING_REPEAT_COLOR * 4] = {  //디밍 반복 컬러값 저장 (디밍색상 임시저장소 시작색, 목적색)
    0x00, 0x00, 0x00, BRIGHT_MAX_VALUE,
    0x00, 0x00, 0x00, BRIGHT_MAX_VALUE
}; //todo 불필요?


//todo 위치이동?
bool runAction;

//todo 위치이동?
//LED 제어하고있는 타이머 루프를 정지 시킬때 사용
void stopAction(void) {
    //DEBUG_MSG_STR("stopAction \r\n");
    runAction = FALSE;      //이 플래그를 확인하여 타이머를 재생성하는데 FALSE일때는 더이상 타이머 생성을 하지않음
    //setEventLedState(LED_EVENT_STATE_IDLE);
    bongStatus.syncCalculer = FALSE;	
    bongStatus.syncCounter = 0;
    bongStatus.continueRunCalculer = FALSE;
    bongStatus.continueCounter = 0;
    bongStatus.runActionWithSync = FALSE;
}

//todo 위치이동?
//LED 제어시작시 사용하여 LED 제어 타이머가 반복되게함
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
  사용자가 선택한 색상을 0,0,0 에서 지정한 색까지 서서히 증가하는 값을 셋팅
  frameCount 최초색상부터 목적색까지 도달하는데 걸리는 횟수
  mode 컬러를 반복 or 증가후 종료 등 모드를 지정
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
  2가지 색상을 반복 디밍 하는 함수
  min 디밍 100퍼센트중 최소값
  max 디밍 100퍼센트중 최대값
  frameCount 최초색상부터 목적색까지 도달하는데 걸리는 횟수
  colorSequenc 색상 배열의 순서
  repeat_CountOrignal 반복 횟수 지정
*/
void setActionLedDimmingRepeat(uint8 persentMin, uint8 persentMax, uint16 frameCount, uint8 colorSequenc, uint8 repeat_CountOrignal){
    
    //시작컬러
    //previous color save
    uint16 calTemp = (ccData.r * persentMin) / 100;
    color_dimming_repeat_mode[0] = calTemp;
    calTemp = (ccData.g * persentMin) / 100;
    color_dimming_repeat_mode[1] = calTemp;
    calTemp = (ccData.b * persentMin) / 100;
    color_dimming_repeat_mode[2] = calTemp;
    
    //목적컬러
    //current color save
    calTemp = (ccData.r * persentMax) / 100;
    color_dimming_repeat_mode[4] = calTemp;
    calTemp = (ccData.g * persentMax) / 100;
    color_dimming_repeat_mode[5] = calTemp;
    calTemp = (ccData.b * persentMax) / 100;
    color_dimming_repeat_mode[6] = calTemp;
    
    //이전색 저장
    ccData.previousRed = color_dimming_repeat_mode[colorSequenc*4];
    ccData.previousGreen = color_dimming_repeat_mode[colorSequenc*4+1];
    ccData.previousBlue = color_dimming_repeat_mode[colorSequenc*4+2];

    //시퀀스 카운트 저장
    colorDimmingData.colorSequenc = colorSequenc;
    //시퀀스 카운트를 임시로 증가하여 배열에서 다음 색상을 목적색으로 지정
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
    
    //이전색, 목적색의 컬러증가 감소 값을 계산
    dimmingColorChangeCal(frameCount, DIMMING_REPEAT);  //dimming repeat persent
    
	//반복 횟수 지정
    colorDimmingData.repeat_CountOrignal = repeat_CountOrignal;
    colorDimmingData.repeat_Count = 0;
}

/**
  무지개 컬러를 서서히 색상이 변경하는 값을 셋팅(무한반복)
  frameCount 최초색상부터 목적색까지 도달하는데 걸리는 횟수
  colorSequenc 색상 배열의 순서
*/
void setActionBtRainbowMode(uint16 frameCount, uint8 colorSequenc) {

    /* 시작컬러 저장 */
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
    
    /* 목적 컬러 저장 */
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
    /* 이전색, 목적색의 컬러증가 감소 값을 계산 */
    dimmingColorChangeCal(frameCount, DIMMING_RAINBOW);  //rainbow mode
}



/**
  부드럽게 켜지기/꺼지기 반복 셋팅
  
  flag_onoff = DIMMING_SELECT_COLOR_ON // 부드럽게 켜지기
  flag_onoff = DIMMING_SELECT_COLOR_OFF // 부드럽게 꺼지기
  frameCount led on / led off 유지 시간
  repeat_CountOrignal // 반복 횟수 값
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
    
    //반복 횟수 지정
    colorDimmingData.repeat_CountOrignal = repeat_CountOrignal;
    colorDimmingData.repeat_Count = 0;
}

/**
  2컬러 부드럽게 변환 반복 셋팅

  frameCount led on / led off 유지 시간
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
    
    //이전색 저장
    ccData.previousRed = color_dimming_repeat_mode[colorSequenc*4];
    ccData.previousGreen = color_dimming_repeat_mode[colorSequenc*4+1];
    ccData.previousBlue = color_dimming_repeat_mode[colorSequenc*4+2];

    //시퀀스 카운트 저장
    colorDimmingData.colorSequenc = colorSequenc;
    
    //시퀀스 카운트를 임시로 증가하여 배열에서 다음 색상을 목적색으로 지정
    colorSequenc = colorSequenc + 1;
    if(colorSequenc > MAX_DIMMING_REPEAT_COLOR-1){
        colorSequenc = 0;
    }
    ccData.r = color_dimming_repeat_mode[colorSequenc*4];
    ccData.g = color_dimming_repeat_mode[colorSequenc*4+1];
    ccData.b = color_dimming_repeat_mode[colorSequenc*4+2];
    
    //반복 횟수 지정
    colorDimmingData.repeat_CountOrignal = repeat_CountOrignal;
    colorDimmingData.repeat_Count = 0;
    
    //이전색, 목적색의 컬러증가 감소 값을 계산
    dimmingColorChangeCal(frameCount, DIMMING_REPEAT_TWO_COLOR);  //dimming repeat persent
}




/**
  A색에서 B색으로 변경시 필요한 연산을 한다.
  ex -> A에서 B까지 증가 해야할 값
        증가 or 감소상태 값을 저장
  
  frameCount 최초색상부터 목적색까지 도달하는데 걸리는 횟수
  mode 색상의 변화의 형태(상승 하강 반복, 상승후 종료 등등)
*/
void dimmingColorChangeCal(uint16 frameCount, uint8 mode) {
    //DEBUG_MSG_STR("dimmingColorChangeCal() \r\n");
    
    colorDimmingData.patten = mode;
    
    colorDimmingData.increaseOrderBase = FALSE;
    if(frameCount != 0){
        colorDimmingData.baseCountOrignal = frameCount; //0~100까지 횟수
    }
    colorDimmingData.baseCount = colorDimmingData.baseCountOrignal;

    /* 시작 컬러 지정 */
    colorDimmingData.startColorR = ccData.previousRed;
    colorDimmingData.startColorG = ccData.previousGreen;
    colorDimmingData.startColorB = ccData.previousBlue;
    /* 종료 컬러 지정 */
    colorDimmingData.endColorR = ccData.r;
    colorDimmingData.endColorG = ccData.g;
    colorDimmingData.endColorB = ccData.b;
    /* 현재 컬러 지정 - 중간에 변화하는값을 ccData에 저장하지 않는다*/
    colorDimmingData.currentR = ccData.previousRed;
    colorDimmingData.currentG = ccData.previousGreen;
    colorDimmingData.currentB = ccData.previousBlue;
    colorDimmingData.currentBright = BRIGHT_MAX_VALUE;  

    /* 증가 or 감소 상태를 구한다 */
    //RED
    if(colorDimmingData.startColorR < colorDimmingData.endColorR){
        colorDimmingData.increaseOrderR = TRUE;
        colorDimmingData.gapR_Temp = colorDimmingData.endColorR - colorDimmingData.startColorR;
    } else {
        colorDimmingData.increaseOrderR = FALSE;
        colorDimmingData.gapR_Temp = colorDimmingData.startColorR - colorDimmingData.endColorR;
    }
    /* 증가할 값을 구해낸다 */
    colorDimmingData.gapR = colorDimmingData.gapR_Temp / colorDimmingData.baseCountOrignal;
    colorDimmingData.randomGapR = colorDimmingData.baseCountOrignal / (colorDimmingData.gapR_Temp % colorDimmingData.baseCountOrignal);  //불규칙 증가값 나온다

    /*
      하단에 덧셈 로직에서 gapR값은 일정하게 덧셈을 하고 randomGapR값을 이용하여 randomGapR횟수에 한번씩 1을 증가시킨다.
      
        colorDimmingData.currentR = colorDimmingData.currentR + colorDimmingData.gapR;  //gap 값 만큼 일정하게 더한다.
        if(colorDimmingData.baseCount % colorDimmingData.randomGapR == 0){   //불규칙 카운트일때 1식 더 증가시킨다.
            colorDimmingData.currentR = colorDimmingData.currentR + 1;
        }
        
      1. gapR 값이 0일경우 randomGapR 횟수에 한번씩 1씩증가(천천히 컬러가 변하는경우)
      2. gapR 값이 0 이상 큰경우 빠르게 색상이 변화한다고 할 수 있다.

      1.의 예시의 경우 (1이 값증가 0이 감소..)
      000100010001000100010001 이와같은 패턴으로 값이 증가 - 예시
      2.의 예시의 경우
      333433343334333433343334 이와같은 패턴으로 값이 증가 - 예시
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
    colorDimmingData.randomGapG = colorDimmingData.baseCountOrignal / (colorDimmingData.gapG_Temp % colorDimmingData.baseCountOrignal);  //불규칙 증가값 나온다
    
    
    //BLUE
    if(colorDimmingData.startColorB < colorDimmingData.endColorB){
        colorDimmingData.increaseOrderB = TRUE;
        colorDimmingData.gapB_Temp = colorDimmingData.endColorB - colorDimmingData.startColorB;
    } else {
        colorDimmingData.increaseOrderB = FALSE;
        colorDimmingData.gapB_Temp = colorDimmingData.startColorB - colorDimmingData.endColorB;
    }
    colorDimmingData.gapB = colorDimmingData.gapB_Temp / colorDimmingData.baseCountOrignal;
    colorDimmingData.randomGapB = colorDimmingData.baseCountOrignal / (colorDimmingData.gapB_Temp % colorDimmingData.baseCountOrignal);  //불규칙 증가값 나온다

 

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
  사용자 지정색 반복 깜빡임
  
  frameCount led on / led off 유지 시간
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
  사용자 모두 다른 랜덤 컬러 깜빡임
  
  frameCount led on / led off 유지 시간
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
    blinkData.onCountOrignal = rand()%25 + 1; // (Random16()%frameCount) + 1; //매번 랜덤으로 주기를 생성하되 frame count이내로 주기를 생성
    blinkData.offCount = 0;
    blinkData.offCountOrignal = blinkData.onCountOrignal;
	
	Send_Data_To_UART0('W');
}
*/

/**
  사용자 일정한 패턴으로 컬러 변경 깜빡임
  
  frameCount led on / led off 유지 시간
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
  버튼 클릭시 빠르게 화이트 점멸
  
  frameCount led on / led off 유지 시간
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
  버튼 클릭시 느리게 화이트 점멸
  
  frameCount led on / led off 유지 시간
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
  블링킹할때 반복 호출되며 카운트 증가 and 컬러값 셋팅 및 컬러led를 밝힌다.
  
  1. 컬러셋팅
  2. 카운트 증가
  3. on / off표현 및 패턴반복여부 지정
*/
void actionBlink(void){

	//Send_Data_To_UART0('1');
    //타입에 따른 컬러 선택
    if(blinkData.onCount == 0){
		
		//Send_Data_To_UART0('2');
               
        switch(blinkData.type)
        {
            /* 트와이스 컬러 색상을 반복 셋팅 */
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
            /* 사용자 지정 컬러 값 셋팅 */
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
            /* 랜덤하게 컬러 테이블값 셋팅 */
            case TYPE_ALL_EACH_RANDOM_COLOR_BLINK:
            {
                //blinkData.colorPatternTemp = (Random16()%MAX_USER_COLOR) * 4;
				//blinkData.colorPatternTemp = (RF_check_counter%MAX_USER_COLOR) * 4; //todo 함수 필요
				blinkData.colorPatternTemp = (RF_check_counter%MAX_USER_COLOR); //todo 함수 필요
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
            /* 일정하게 컬러 테이블값 셋팅 */
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
            /* 버튼클릭 화이트 컬러 값 지정(on off 패턴) */
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
    
    /* 카운트 증가 */
    if(blinkData.onCount < blinkData.onCountOrignal){
        /* led on 상태 */
        blinkData.onCount = blinkData.onCount + 1;
        //DEBUG_MSG_STR("LED ON \r\n");
        blinkData.ledState = TRUE;
    } else {
        /* led off 상태 */
        if(blinkData.offCount < blinkData.offCountOrignal){
            blinkData.offCount = blinkData.offCount + 1;
            //DEBUG_MSG_STR("LED OFF \r\n");
            blinkData.ledState = FALSE;
        } else {
            /* 초기화 */
            blinkData.offCount = 0;
            blinkData.onCount = 0;
			
			/*
			if(blinkData.type == TYPE_ALL_USER_COLOR_RANDOM_TIME_BLINK)
            {
				srand(readBatteryValue);
                blinkData.onCountOrignal = rand()%25 + 1; //매번 랜덤으로 주기를 생성하되 frame count이내로 주기를 생성
                blinkData.offCountOrignal = blinkData.onCountOrignal;
            }
			*/
            
            blinkData.colorPatternCount = blinkData.colorPatternCount + 1;
            
            /* 타입에 따라 패턴 최초부터 시작하는 경우 */
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
            /* 타입에 따라 패턴 모두 동작후 종료되는 경우 */
            //if(blinkData.type == TYPE_BT_CONNECT || blinkData.type == TYPE_BT_SMS_NOTI || blinkData.type == TYPE_BTN_RESET_MODE){
            //    if( blinkData.colorPatternCount >= blinkData.colorPatternCountMax ){
                    //이벤트 종료 후 therapy, party, alwayson 상태였다면 원복한다.
            //        lastEventCheck();
            //    }
            //}
        }
    }

    /* led 컬러 표현 */
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

// LED 끄기
void actionLedOff(void) {
	//IOTLightControlDeviceSetColor(0, 0, 0, 0);
	PWM_change_duty(0,0,0);
}

// 컨트롤러에서 선택한 색상 켜기
void actionLedOnWithSelectedColor(void) {
	//IOTLightControlDeviceSetColor(ccData.r, ccData.g, ccData.b, ccData.bright);
	//if(ccData.r == 255 && ccData.g == 255 && ccData.b == 255)
	//	PWM_change_duty(0, 0, 0, 255);
	//else
    //Send_UINT8_To_UART0('C');
	PWM_change_duty(ccData.r, ccData.g, ccData.b);
}

/**
  현재컬러 부드럽게 꺼지기 반복 셋팅
  
  flag_onoff = DIMMING_SELECT_COLOR_ON // 부드럽게 켜지기
  flag_onoff = DIMMING_SELECT_COLOR_OFF // 부드럽게 꺼지기
  frameCount led on / led off 유지 시간
  repeat_CountOrignal // 반복 횟수 값
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
    
    //반복 횟수 지정
    colorDimmingData.repeat_CountOrignal = repeat_CountOrignal;
    colorDimmingData.repeat_Count = 0;
}

void setActionInteractive_1_color(uint16 frameCount,uint8 m_nInteractive_frame_counter_init, uint8 m_nmaxPlayShowNum,uint8 m_nFrame_index)
{
    nowPlayShowNum = m_nInteractive_frame_counter_init;  //프레임 초기화
    maxPlayShowNum = m_nmaxPlayShowNum;   //단계 설정
    myShowNum = m_nFrame_index;    //해당 연출에서 몇번째 스탭에 동작해야하는지에 대한 인덱스 저장
    
    maxShowSpeedAdjustCount = frameCount; //타이머 몇회마다 한번씩 동작하게 할것인지 설정
    nowShowSpeedAdjustCount = frameCount;   //처음 1회는 즉시 실행하기 위함
}

void setActionShowByDMX(void) {
    maxShowSpeedAdjustCount = 35 - ccData.runSpeed;   //타이머 몇회마다 한번씩 동작하게 할것인지 설정
    nowShowSpeedAdjustCount = 35 - ccData.runSpeed;   //처음 1회는 즉시 실행하기 위함
}

// 1색상 증가방향으로 순차 켜지기
// myShowNum : 내꺼 가로 번호다
uint8 action_Increase_On_1_color(void)
{    
    //속도 조절을 위해 매 프레임마다 maxShowSpeedAdjustCount 만큼 카운팅 한다음 실행    
    // 15값이다.
    // nowShowSpeedAdjustCount 1씩 올리면서 15가 되면 아래 루틴을 실행
    if(nowShowSpeedAdjustCount >= maxShowSpeedAdjustCount)
    {
        nowShowSpeedAdjustCount=0;
        nowPlayShowNum++;
        
        if((myShowNum <= nowPlayShowNum) && (myShowNum >= 1))
        {
            //표시
            //IOTLightControlDeviceSetColor(ccData.r,ccData.g, ccData.b, ccData.bright);
					PWM_change_duty(ccData.r, ccData.g, ccData.b);
            
            //전체 표시 완료
            return 1;
        }
        else
        {
            //스타트 이전에는 0,0,0을 킴
            //IOTLightControlDeviceSetColor(0, 0, 0, ccData.bright);
					PWM_change_duty(0, 0, 0);
        }
        
        
        //마지막 스텝보다 큰경우 더이상 돌지 않는다
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

//1색상 증가방향으로 순차 꺼지기
uint8 action_Increase_Off_1_color(void)
{
    
    //속도 조절을 위해 매 프레임마다 maxShowSpeedAdjustCount 만큼 카운팅 한다음 실행      
    if(nowShowSpeedAdjustCount >= maxShowSpeedAdjustCount)
    {
        nowShowSpeedAdjustCount=0;
        
        nowPlayShowNum++;

        //1이 최소값이기 때문에 1~n단계까지만 표시를 해야한다.
        if((myShowNum <= nowPlayShowNum) && (myShowNum >= 1))
        {
            //표시
            //IOTLightControlDeviceSetColor(0, 0, 0, ccData.bright);
					PWM_change_duty(0, 0, 0);
            
            //전체 표시 완료
            return 1;
        }
        //이외의 경우 처리
        else if((myShowNum > maxPlayShowNum) || (myShowNum == 0))
        {
            //꺼짐처리
            //IOTLightControlDeviceSetColor(0, 0, 0, ccData.bright);
					PWM_change_duty(0, 0, 0);
        }
        
        //마지막 스텝보다 큰경우 더이상 돌지 않는다
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

// DMX로 색상켜기(디밍적용 필요함)
// 1 이면 종료
// 0 이면 계속
uint8 actionLedOnByDMX(void) {
    //DebugWriteString("d_color_on\r\n");
   
    
    // 디밍값의 백분율을 구함.    
    groupInfoCmd.dmxDPercent = (groupInfoCmd.dmxD * 4) / 10;   
    
    // R 재계산
    groupInfoCmd.dmxRTemp = (groupInfoCmd.dmxR * groupInfoCmd.dmxDPercent) / 100;
    if( groupInfoCmd.dmxRTemp > 255 ) {
        groupInfoCmd.dmxRN = 255;
    }
    else {
        groupInfoCmd.dmxRN = (groupInfoCmd.dmxR * groupInfoCmd.dmxDPercent) / 100;
    }
    
    // G 재계산
    groupInfoCmd.dmxGTemp = (groupInfoCmd.dmxG * groupInfoCmd.dmxDPercent) / 100;
    if( groupInfoCmd.dmxGTemp > 255 ) {
        groupInfoCmd.dmxGN = 255;
    }
    else {
        groupInfoCmd.dmxGN = (groupInfoCmd.dmxG * groupInfoCmd.dmxDPercent) / 100;
    }
    
    // B 재계산
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
    nowPlayShowNum = m_nInteractive_frame_counter_init;  //프레임 초기화
    maxPlayShowNum = m_nmaxPlayShowNum;   //단계 설정
    myShowNum = m_nFrame_index;    //해당 연출에서 몇번째 스탭에 동작해야하는지에 대한 인덱스 저장
    
    maxShowSpeedAdjustCount = frameCount; //타이머 몇회마다 한번씩 동작하게 할것인지 설정
    nowShowSpeedAdjustCount = frameCount;   //처음 1회는 즉시 실행하기 위함
    
    g_bRainbow_color_start = FALSE; //무지개 시작 변수 초기화
    g_nRainbow_counter = 0; //무지개 표시 카운트 변수 초기화
    g_bPattern_Repeat_flag = m_bPattern_Repeat_flag;    //반복여부 저장
}

//무지개 증가방향으로 순차 켜지기
uint8 action_Increase_On_Rainbow(void)
{    
    //속도 조절을 위해 매 프레임마다 maxShowSpeedAdjustCount 만큼 카운팅 한다음 실행      
    if(nowShowSpeedAdjustCount >= maxShowSpeedAdjustCount)
    {
        nowShowSpeedAdjustCount=0;
        nowPlayShowNum++;
        
        //무지개 시작 시점 판단
        //무지개가 시작되었는가?
        if(g_bRainbow_color_start == FALSE)
        {
            if((myShowNum <= nowPlayShowNum) && (myShowNum >= 1))
            {
                //무지개 시작 시점
                g_bRainbow_color_start = TRUE;
            }
            else
            {
                //스타트 이전에는 0,0,0을 킴
                //IOTLightControlDeviceSetColor(0, 0, 0, ccData.bright);
				PWM_change_duty(0, 0, 0);
				
            }
            
            
            //마지막 스텝보다 큰경우 더이상 돌지 않는다
            if(nowPlayShowNum > maxPlayShowNum)
            {
                return 1;
            }
        }
        
        //무지개 스타트 표시 시작 시점에 도달했는지 확인
        if(g_bRainbow_color_start == TRUE)
        {
            
            //무지개를 다 표시한 경우 반복 패턴인지 확인해야한다.
            if(g_nRainbow_counter >= MAX_RAINBOW_MODE_COLOR)
            {
                //반복 패턴인경우에는 처음색깔부터 다시 시작
                if(g_bPattern_Repeat_flag == TRUE)
                {
                    g_nRainbow_counter=0;
                    //무지개 표시
                    //IOTLightControlDeviceSetColor(color_rainbow_mode[g_nRainbow_counter*4],color_rainbow_mode[g_nRainbow_counter*4 + 1], color_rainbow_mode[g_nRainbow_counter*4 +2], ccData.bright);
					PWM_change_duty(get_color_rainbow_mode_R(g_nRainbow_counter), get_color_rainbow_mode_G(g_nRainbow_counter), get_color_rainbow_mode_B(g_nRainbow_counter));
                    g_nRainbow_counter++;
					
                }
                else
                {
                    //반복이 끝난 경우에는 종료
                    //IOTLightControlDeviceSetColor(0, 0, 0, ccData.bright);
					PWM_change_duty(0, 0, 0);
                    return 1;
                }
            }
            else
            {
                //무지개 표시
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
	groupCheckMy = groupInfo.myGroup;   // 기본적으로 1그룹설정
    
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
		
		// 파랑, 보라, 빨강, 녹색, 노랑, 하얀, 핫핑크, 오렌지
    // 라이트블루, 아쿠아, 자주, 라이트 그린
    if( (groupCheckMy == 1) || (groupCheckMy == 8) || (groupCheckMy == 15) || (groupCheckMy == 22) ) {      // 파랑
        ccData.r = 0x0;
        ccData.g = 0x0;
        ccData.b = 0xff;
    }
    else if( (groupCheckMy == 2) || (groupCheckMy == 9) || (groupCheckMy == 16) || (groupCheckMy == 23) ) {  // 빨강
        ccData.r = 0xff;
        ccData.g = 0x0;
        ccData.b = 0x0;
    }
    else if( (groupCheckMy == 3) || (groupCheckMy == 10) || (groupCheckMy == 17) || (groupCheckMy == 24) ) {  // 녹색
        ccData.r = 0x0;
        ccData.g = 0xff;
        ccData.b = 0x0;
    }
    else if( (groupCheckMy == 4) || (groupCheckMy == 11) || (groupCheckMy == 18) || (groupCheckMy == 25) ) {  // 노랑
        ccData.r = 0xff;
        ccData.g = 0xff;
        ccData.b = 0x0;
    }
    else if( (groupCheckMy == 5) || (groupCheckMy == 12) || (groupCheckMy == 19) ) {  // 보라
        ccData.r = 0xa7;
        ccData.g = 0x0;
        ccData.b = 0xff;
    }
    else if( (groupCheckMy == 6) || (groupCheckMy == 13) || (groupCheckMy == 20) ) {  // 오렌지
        ccData.r = 0xff;
        ccData.g = 0x50;
        ccData.b = 0x0;
    }
    else if( (groupCheckMy == 7) || (groupCheckMy == 14) || (groupCheckMy == 21) ) {  // 녹색 //핑크
        ccData.r = 0x0;
        ccData.g = 0xff;
        ccData.b = 0x0;
    }
    else {                                                                                            // 화이트
        ccData.r = 0xff;
        ccData.g = 0xff;
        ccData.b = 0xff;
    }
		/*
	if( (groupCheckMy == 1) || (groupCheckMy == 8) || (groupInfo.myGroup == 15) ) {      // 파랑
        ccData.r = 0x0;
        ccData.g = 0x0;
        ccData.b = 0xff;
    }
    else if( (groupInfo.myGroup == 2) || (groupInfo.myGroup == 9) || (groupInfo.myGroup == 16) ) {  // 빨간
        ccData.r = 0xff;
        ccData.g = 0x0;
        ccData.b = 0x0;
    }
    else if( (groupInfo.myGroup == 3) || (groupInfo.myGroup == 10) || (groupInfo.myGroup == 17) ) {  // 그린
        ccData.r = 0x0;
        ccData.g = 0xff;
        ccData.b = 0x0;
    }
    else if( (groupInfo.myGroup == 4) || (groupInfo.myGroup == 11) || (groupInfo.myGroup == 18) ) {  // 노랑
        ccData.r = 0xff;
        ccData.g = 0xff;
        ccData.b = 0x0;
    }
    else if( (groupInfo.myGroup == 5) || (groupInfo.myGroup == 12) || (groupInfo.myGroup == 19) ) {  // 보라
        ccData.r = 0xa7;
        ccData.g = 0x0;
        ccData.b = 0xff;
    }
    else if( (groupInfo.myGroup == 6) || (groupInfo.myGroup == 13) || (groupInfo.myGroup == 20) ) {  // 오렌지
        ccData.r = 0xff;
        ccData.g = 0x50;
        ccData.b = 0x0;
    }
    else if( (groupInfo.myGroup == 7) || (groupInfo.myGroup == 14) || (groupInfo.myGroup == 21) ) {  // 녹색
        //ccData.r = 0xff;
        //ccData.g = 0x0;
        //ccData.b = 0x86;
			  ccData.r = 0x0;
        ccData.g = 0xff;
        ccData.b = 0x0;
    }
    else {                                                                                                   // 화이트
        ccData.r = 0xff;
        ccData.g = 0xff;
        ccData.b = 0xff;
    }
		*/
}