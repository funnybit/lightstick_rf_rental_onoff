#include "Common.h"
#include "N76E003.h"
#include "Function_define.h"
#include "SFR_Macro.h"
#include "main.h"

#include "color_control.h"
#include "action_service.h"
#include "control_manager.h"
#include "UART.h"
#include "cheerbong_info.h"

void actionHandler(void) {
		//Send_Data_To_UART0('\r');
	//Send_Data_To_UART0('\n');
	//Send_Data_To_UART0('n');
    if( ccData.runSpeed != RUN_SPEED_NOT ) {
        switch ( sceneMode ) {
            case MODE_OFF :               // ��ü����
                //DEBUG_MSG_STR("MODE_OFF \r\n");
                stopAction();  //Ÿ�̸� ����
                actionLedOff();            
            break;
            
            case MODE_SELECTED_COLOR :    // ���û� �ѱ�
                //DEBUG_MSG_STR("MODE_SELECTED_COLOR \r\n");
                stopAction();
                actionLedOnWithSelectedColor();            
            break;
            /*������ �ڿ������� ���ϴ� ����*/
            case MODE_DIMMING_REPEAT_0_100 :    // ���(0 ~ 100)
            case MODE_DIMMING_REPEAT_20_100 :   // ���(20 ~ 100)
            case MODE_DIMMING_COLOR_CHANGE :    // ������� ���󺯰�(A������ B������ ����)
            case MODE_DIMMING_LED_ON : //���û� ������� on
            case MODE_DIMMING_REPEAT_OFF:   //���û� �ε巴�� OFF �ݺ�
            case MODE_DIMMING_REPEAT_ON:   //���û� �ε巴�� ON �ݺ�
            case MODE_DIMMING_REPEAT_TWO_COLOR:   //2�÷� �ε巴�� ��ȯ �ݺ�
            case MODE_DIMMING_REPEAT_TWO_COLOR_COUNT:   //2�÷� �ε巴�� ��ȯ �ݺ�
            case MODE_BT_RAINBOW_MODE:
			case MODE_DIMMING_OFF_CUR_COLOR:
				case MODE_BTN_RAINBOW:
                //DEBUG_MSG_STR("actionLedDimmingColorChange() \r\n");
    
                if( ccData.runSpeed != RUN_SPEED_NOT ) {
                    bongStatus.continueRunCalculer = TRUE;
                    actionLedDimmingColorChange();
                }
                
            break;


            
            
            case MODE_ALL_RANDOM_COLOR_BLINK : //��ü ����(��ü �����÷��� ����)
            case MODE_ALL_USER_COLOR_BLINK : //��ü ����(���û�)
            case MODE_ALL_EACH_RANDOM_COLOR_BLINK : //��ü ����(��ü ���� �����÷�)       
//			case MODE_ALL_USER_COLOR_RANDOM_TIME_BLINK: //��ü ����(���û� �����ð�)							
            case MODE_BTN_WHITE_BLINK_FAST_MODE :      //��ư ��� ���� ����
            case MODE_BTN_WHITE_BLINK_SLOW_MODE :      //��ư ��� ���� ����
                //DEBUG_MSG_STR("actionBlink() \r\n");
                if( ccData.runSpeed != RUN_SPEED_NOT ) {
                    bongStatus.continueRunCalculer = TRUE;
                    actionBlink();
                }
			break;  
								
			case MODE_INTERACTIVE_VERTICAL_UP_LED_ON_7STEP:     // ����(�Ʒ�->��) ���� ������
            case MODE_INTERACTIVE_HORIZONTAL_UP_LED_ON_7STEP:   // ����(�Ʒ�->��) ���� ������
            case MODE_INTERACTIVE_HORIZONTAL_OUT_LED_ON_3STEP:  // ����(�Ʒ�->��) ���� ������
            case MODE_INTERACTIVE_VERTICAL_OUT_LED_ON_3STEP:    // ����(�Ʒ�->��) ���� ������
            case MODE_INTERACTIVE_LINE_UP_LED_ON:               // ���� ���û� (�Ʒ�->��) ���� �ѱ�
            case MODE_INTERACTIVE_SHAPE_UP_LED_ON:              // ��� ���û� (�Ʒ�->��) ���� �ѱ�
                if(action_Increase_On_1_color() != 0) {
					//Send_Data_To_UART0('s');
					//Send_Data_To_UART0('t');
                    stopAction();
				}
				else {
					bongStatus.continueRunCalculer = TRUE;
				}
            break;
								
			case MODE_INTERACTIVE_VERTICAL_UP_LED_OFF_7STEP:    // ����(�Ʒ�->��) ���� ������
            case MODE_INTERACTIVE_HORIZONTAL_UP_LED_OFF_7STEP:  // ����(�Ʒ�->��) ���� ������
            case MODE_INTERACTIVE_HORIZONTAL_OUT_LED_OFF_3STEP: // ����(�Ʒ�->��) ���� ������
            case MODE_INTERACTIVE_VERTICAL_OUT_LED_OFF_3STEP:   // ����(�Ʒ�->��) ���� ������
            case MODE_INTERACTIVE_SHAPE_UP_LED_OFF:             // ��� ���û� (�Ʒ�->��) ���� ����
                if(action_Increase_Off_1_color() != 0) {
					stopAction();
				}
				else {
					bongStatus.continueRunCalculer = TRUE;
				}
            break;  

			case MODE_DMX_CONTROL_STAY :
                if( actionLedOnByDMX() != 0 ) {              
	                stopAction();
				}
				else {
					bongStatus.continueRunCalculer = TRUE;
				}
            break;
			
            case MODE_INTERACTIVE_HORIZONTAL_UP_RAINBOW_6STEP:         // ����(�Ʒ�->��) ������ �ݺ� - 6�ܰ�
                if(action_Increase_On_Rainbow() != 0)
                        stopAction();
				else
					bongStatus.continueRunCalculer = TRUE;
            break;
                                       
            default :
            break;
        }
    }
}