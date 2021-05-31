#include "Common.h"
#include "N76E003.h"
#include "Function_define.h"
#include "SFR_Macro.h"
#include "main.h"

#include "control_manager.h"
#include "UART.h"
#include "cheerbong_info.h"
#include "color_control.h"

RUN_MODE_T g_run_mode;

#define SHOW_FRAME_RUN_COUNT 10

extern bool dmxBlink;

void changeRunMode(RUN_MODE_T mode) {
    g_run_mode = mode;
}

void actionModeChange(ActionCommand ac) {  // �������� ���۸�带 �����Ѵ�.
    //DEBUG_MSG_STR("actionModeChange = ");
    //DEBUG_MSG_UINT8(ac);
    //DEBUG_MSG_STR("\r\n");
    
	switch ( ac ) {
        case CMD_ALL_OFF :                           // ��ü ����
        	sceneMode = MODE_OFF;
        break;
        case CMD_ALL_ON_COLOR :                      // ���û� �ѱ�
            sceneMode = MODE_SELECTED_COLOR;
        break;
        case CMD_DIMMING_COLOR_CHANGE :              // ������� ���󺯰�
            setActionDimmingColorChange(100, DIMMING_RISE);            
            sceneMode = MODE_DIMMING_COLOR_CHANGE;
        break;
        case CMD_DIMMING_REPEAT_0_100 :              // ���(0 - 100)
        	sceneMode = MODE_DIMMING_REPEAT_0_100;
            setActionLedDimmingRepeat(0,100, 100, 0, 0);   //0~100 ����, 100ȸ�ȿ� 0���� 100���� ����, �÷� ���۰� 0
        break;
        case CMD_DIMMING_REPEAT_20_100 :             // ���(20 - 100)
            sceneMode = MODE_DIMMING_REPEAT_20_100;
            setActionLedDimmingRepeat(20,100, 100, 0, 0);  //20~100 ����, 100ȸ�ȿ� 0���� 100���� ����, �÷� ���۰� 0
        break;				
        case CMD_DIMMING_REPEAT_OFF :             // ���û� �ε巴�� OFF �ݺ�
            sceneMode = MODE_DIMMING_REPEAT_OFF;
            setActionLedDimmingRepeatOnOff(100, DIMMING_SELECT_COLOR_OFF, 0);  //���û�  100ȸ�ȿ� 100���� -> 0���� ����
        break;
        case CMD_DIMMING_REPEAT_ON :             // ���û� �ε巴�� ON �ݺ�
            sceneMode = MODE_DIMMING_REPEAT_ON;
            setActionLedDimmingRepeatOnOff(100, DIMMING_SELECT_COLOR_ON, 0);  //���û�  100ȸ�ȿ� 0���� -> 100���� ����
        break;
		case CMD_DIMMING_OFF_CUR_COLOR :             // �����÷� �ε巴�� OFF
            sceneMode = MODE_DIMMING_OFF_CUR_COLOR;

            setActionLedDimmingRepeatOnOff_CurColor(100, DIMMING_SELECT_COLOR_OFF, 1);  //�����÷�  100ȸ�ȿ� 100���� -> 0���� ����
        break;
        case CMD_DIMMING_REPEAT_TWO_COLOR :             // 2�÷� �ε巴�� ��ȯ �ݺ�
            sceneMode = MODE_DIMMING_REPEAT_TWO_COLOR;
            setActionLedDimmingRepeatTwoColor(100, 0, 0);  //�ε巴�� ��-�� �ݺ�
        break;
        case CMD_DIMMING_REPEAT_TWO_COLOR_COUNT :             // 2�÷� �ε巴�� ��ȯ �ݺ� Ƚ�� ����
            sceneMode = MODE_DIMMING_REPEAT_TWO_COLOR_COUNT;
            setActionLedDimmingRepeatTwoColor(100, 0, 3);  //�ε巴�� ��-�� �ݺ�
        break;
        case CMD_DIMMING_LED_ON :          // ������� LED �ѱ�(���û�)
            setActionDimmingLedOn(100, DIMMING_RISE);
            sceneMode = MODE_DIMMING_LED_ON;
        break;
        case CMD_ALL_USER_COLOR_BLINK :               // ��ü ����(���û�)
            setActionAllUserColorBlink(15);
            sceneMode = MODE_ALL_USER_COLOR_BLINK;
        break;
        case CMD_ALL_EACH_RANDOM_COLOR_BLINK :           // ��ü ����(��ü ���� �����÷�)
            setActionAllEachRandomColorBlink(15);
            sceneMode = MODE_ALL_EACH_RANDOM_COLOR_BLINK;
        break;
        case CMD_ALL_RANDOM_COLOR_BLINK :           // ��ü ����(��ü �����÷��� ����)
            setActionAllRandomColorBlink(15);
            sceneMode = MODE_ALL_RANDOM_COLOR_BLINK;        
        break;
		case CMD_BT_RAINBOW_MODE :           // ��ü ����(��ü �����÷��� ����)
            setActionBtRainbowMode(50, 0);
            sceneMode = MODE_BT_RAINBOW_MODE;   
        break;
       
		/*
		case CMD_ALL_USER_COLOR_RANDOM_TIME_BLINK :               // ��ü ����(���û� �����ð�)

            setActionAllUserColorRandomTimeBlink(40);
            sceneMode = MODE_ALL_USER_COLOR_RANDOM_TIME_BLINK;
        break;
		*/
        
        case CMD_BTN_WHITE_BLINK_SLOW_MODE :                       // BTN �ó����� ȭ��Ʈ ����ŷ slow
            ccData.runSpeed = RUN_SPEED_17;
            setActionBtnWhiteBlinkSlowMode(11);
            sceneMode = MODE_BTN_WHITE_BLINK_SLOW_MODE;
        break;
        case CMD_BTN_WHITE_BLINK_FAST_MODE :                       // BTN �ó����� ȭ��Ʈ ����ŷ fast
            ccData.runSpeed = RUN_SPEED_17;
            setActionBtnWhiteBlinkFastMode(3);
            sceneMode = MODE_BTN_WHITE_BLINK_FAST_MODE;
        break;
				
		case CMD_SHOW_VERTICAL_UP_LED_ON :              // ����(�Ʒ�->��) ���� ������
        	sceneMode = MODE_INTERACTIVE_VERTICAL_UP_LED_ON_7STEP;

            setActionInteractive_1_color(SHOW_FRAME_RUN_COUNT, 0, groupInfoCmd.hallHeight, groupInfo.vertical);            
        break;
        case CMD_SHOW_VERTICAL_UP_LED_OFF :              // ����(�Ʒ�->��) ���� ������
        	sceneMode = MODE_INTERACTIVE_VERTICAL_UP_LED_OFF_7STEP;

            setActionInteractive_1_color(SHOW_FRAME_RUN_COUNT, 0, groupInfoCmd.hallHeight, groupInfo.vertical);           
        break;  
		case CMD_DMX_CONTROL_STAY:                     // DMX ����, strobe�� ���� ����  
				dmxBlink = TRUE;
			//Send_Data_To_UART0('3');	
            sceneMode = MODE_DMX_CONTROL_STAY;
            setActionShowByDMX();
        break;
		
	
		// ������(����)
        case CMD_INTERACTIVE_HORIZONTAL_UP_RAINBOW_6STEP :                   // ����(��->��) ������ �ݺ�
        	sceneMode = MODE_INTERACTIVE_HORIZONTAL_UP_RAINBOW_6STEP;
            setActionInteractive_Rainbow(SHOW_FRAME_RUN_COUNT, 0, groupInfoCmd.hallWidth, groupInfo.horizontal, TRUE);
        break;   
		
				
        default :
        break;     
    }	
}