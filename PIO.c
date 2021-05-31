#include "Common.h"
#include "N76E003.h"
#include "Function_define.h"
#include "SFR_Macro.h"
#include "main.h"

#include "PIO.h"
#include "UART.h"
#include "PWM.h"
#include "control_manager.h"
#include "cheerbong_info.h"
#include "color_control.h"
#include "ADC.h"

#define KEY_PRESSED  (TRUE)
#define KEY_RELEASED (FALSE)

extern bool lowBattery;

extern uint8 rainbowWhere;



bool m_is_click_protect = FALSE;

static bool buttonPressStatus = KEY_PRESSED; //KEY_RELEASED;
extern UINT8 PIO_button_check_flag;

void HandleShortButtonPress(void);

void PIO_Init(void)
{
	
	///*
	P15_Quasi_Mode;      // 충전중 Active low
	P15 = 1;
	
	P15_Input_Mode;
	
	//PIO 인터럽트
	//Enable_INT_Port1;
	
	//Enable_BIT5_RasingEdge_Trig;
	//Enable_BIT5_FallEdge_Trig;
	
	set_EPI;
	set_EA;
	//*/

	// USB 전압 입력(Active Low)
	P12_Quasi_Mode;
	P12 = 0;
	
//	P12_Input_Mode;
	// Enable_BIT2_FallEdge_Trig;       // USB 충전케이블이 꼽힐때 interrupt 발생
	
	/*
	 // 19.11.20 이달의 소녀 보드가 중국에서 왔는데, 키를 누르면 키가 동작안하는 문제가 있었다. 
	 // 확인해보니, P17이 버튼인데 이거 말고 다른 포트에서 인터럽트가 발생했다. 
	 // 노이즈로 발생되는 인터럽트로 땝을 다시 하니 사라지는데, 일단 그 인터럽트가 P12로 들어온다. 
	 // 그래서 inpu mode로 바꾸고, 아래 코드를 사용하지 않도록 주석 처리했다.
	Enable_BIT2_RasingEdge_Trig;
	set_EPI;
	set_EA;
	*/

	// 충전 LED
	P04_Quasi_Mode;
	P04 = 1;         // 1을 주면 꺼진다.
	set_EPI;
	set_EA;
	
	///*
	P17_Quasi_Mode;           // 버튼
	P17 = 0;
	
	P17_Input_Mode;
	//*/2
	//PIO 인터럽트
	Enable_INT_Port1;
	
	//Enable_BIT5_RasingEdge_Trig;
	//Enable_BIT5_FallEdge_Trig;
	
	Enable_BIT7_FallEdge_Trig;
	
	set_EPI;
	set_EA;
}

void PinInterrupt_ISR (void) interrupt 7
{
	// Send_Data_To_UART0('+');	
	if(PIF == 0x01)
	{
		PIF = 0x00;
		
		//PIO_button_check_flag = 1;
		
		// log:key1
		// Send_Data_To_UART0('K');	
		// Send_Data_To_UART0('1');
		// Send_Data_To_UART0('\r');
		// Send_Data_To_UART0('\n');
	}
	else if(PIF == 0x04)                  // Power Cable
	{
		////Send_UINT8_To_UART0(PIF);
		PIF = 0x00;
		
		//PIO_button_check_flag = 1;

		// log:key1
		// Send_Data_To_UART0('K');	
		// Send_Data_To_UART0('5');
		// Send_Data_To_UART0('\r');
		// Send_Data_To_UART0('\n');

		//SW_Reset();

		//bongStatus.powerCableInsert = TRUE;
		//lowBattery = FALSE;

				
	}
	else if(PIF == 0x10)
	{
		////Send_UINT8_To_UART0(PIF);
		PIF = 0x00;
		
		//PIO_button_check_flag = 1;
		
		// log:key1
		// Send_Data_To_UART0('K');	
		// Send_Data_To_UART0('6');
		// Send_Data_To_UART0('\r');
		// Send_Data_To_UART0('\n');
	}
	else 	if(PIF == 0x20)                             // P15 꼽히면 발생
	{
		////Send_UINT8_To_UART0(PIF);
		PIF = 0x00;
		
		//PIO_button_check_flag = 1;
		
		// log:key2
		// Send_Data_To_UART0('K');	
		// Send_Data_To_UART0('2');
		// Send_Data_To_UART0('\r');
		// Send_Data_To_UART0('\n');
	}
	else 	if(PIF == 0x40)  
	{
		////Send_UINT8_To_UART0(PIF);
		PIF = 0x00;
		
		//PIO_button_check_flag = 1;
		
		// log:key3
		// Send_Data_To_UART0('K');	
		// Send_Data_To_UART0('3');
		// Send_Data_To_UART0('\r');
		// Send_Data_To_UART0('\n');
	}
	else 	if(PIF == 0x80)  // funnylogic P17은 이곳으로 인터럽트가 들어옴, ke4
	{
		////Send_UINT8_To_UART0(PIF);
		PIF = 0x00;		
		
		// 10s 리셋관련
		if((AUXR1 & SET_BIT7) == SET_BIT7)
		{
			clr_SWRF;
			// Send_Data_To_UART0('R');
			// Send_Data_To_UART0('F');
			// Send_Data_To_UART0('\r');
			// Send_Data_To_UART0('\n');
		}		
		else
		{
			// Send_Data_To_UART0('N');
			// Send_Data_To_UART0('R');
			// Send_Data_To_UART0('F');
			// Send_Data_To_UART0('\r');
			// Send_Data_To_UART0('\n');

			if( bongStatus.showMode == FALSE ) {  // 공연 모드가 아니다.	
				// Send_Data_To_UART0('S');
				// Send_Data_To_UART0('H');
				// Send_Data_To_UART0('O');
				// Send_Data_To_UART0('W');
				// Send_Data_To_UART0('\r');
				// Send_Data_To_UART0('\n');

				if(is5VMode() != BATT_LVL_1){
					PIO_button_check_flag = 1;
				}
			}
			else {    // 공연모드다
				// Send_Data_To_UART0('N');
				// Send_Data_To_UART0('O');
				// Send_Data_To_UART0('S');
				// Send_Data_To_UART0('H');
				// Send_Data_To_UART0('O');
				// Send_Data_To_UART0('W');
				// Send_Data_To_UART0('\r');
				// Send_Data_To_UART0('\n');
			}
			// log:key4
			// Send_Data_To_UART0('K');	
			// Send_Data_To_UART0('4');
			// Send_Data_To_UART0('\r');
			// Send_Data_To_UART0('\n');
		}
		
		// groupInfoCmd.syncTime = SECOND_RECEIVED_CMD;
		// ccData.runSpeed = RUN_SPEED_4;
		// showInteractive( CMD_BTN_WHITE_BLINK_FAST_MODE );
	}
}


void handleButtonDebounce(void)
{
	debounce_counter_clr();

	// log:DEB7
	// Send_Data_To_UART0('D');
	// Send_Data_To_UART0('E');
	// Send_Data_To_UART0('B');
	// Send_Data_To_UART0('7');
	// Send_Data_To_UART0('\r');
	// Send_Data_To_UART0('\n');

    //SHOW_MSG_STR("deb \r\n");

        //debounce_tid = TIMER_INVALID;    
	//PioSetEventMask(BUTTONS_BIT_MASK, pio_event_mode_both);

	//SHOW_MSG_STR("deb 11\r\n");
	
	//BUTTON_DEBOUNCE_TIME 후에 다시 버튼 PIO 상태를 확인하여 실행하므로서 바운싱을 막는다
	
	//다시 버튼 PIO확인
	//if(P15 == FALSE){
		
		//현재 뗀상태라면 롱프레스 타이머 제거
		//if(longPressTimerId != TIMER_INVALID){
		//	TimerDelete(longPressTimerId);
		//	longPressTimerId = TIMER_INVALID;
		//}
	//}
	//else if (P15 == TRUE) {
	//}
	
	//if ((PioGet(SW4_PIO) == FALSE) && (buttonPressStatus == KEY_RELEASED))
	if(P17 == FALSE)
	{
		// log:P17=F
		// Send_Data_To_UART0('P');
		// Send_Data_To_UART0('1');
		// Send_Data_To_UART0('7');
		// Send_Data_To_UART0('=');
		// Send_Data_To_UART0('F');
		// Send_Data_To_UART0('\r');
		// Send_Data_To_UART0('\n');

		//buttonPressStatus = KEY_PRESSED;

		//SHOW_MSG_STR("ppp 11\r\n");
		HandleShortButtonPress();
		
		
		if(click_protect_timer_enable == 0)
		{
			m_is_click_protect = TRUE;
			click_protect_counter_set(300);
		}
		//300ms 다시 눌림 방지
		//if(click_protect_tid == TIMER_INVALID){
		//	m_is_click_protect = TRUE; //클릭방지시작
		//	click_protect_tid = TimerCreate(BUTTON_CLICK_PROTECT, TRUE, handleClickProtect);
		//}
	}
	
	/*
	else if ((PioGet(SW4_PIO) == TRUE) && (buttonPressStatus == KEY_PRESSED))
	{
		buttonPressStatus = KEY_RELEASED;

		SHOW_MSG_STR("ppp 22\r\n");
	}
	
	if ((PioGet(9) == FALSE) && (buttonPressStatus == 7))
	{
		buttonPressStatus = KEY_PRESSED;

		SHOW_MSG_STR("ppp 33");
		
		handleExtraLongButtonPress(debounce_tid);
		handleShortButtonPress(debounce_tid);
	} 
	*/	
}

void handleClickProtect(void)
{
	m_is_click_protect = FALSE; //클릭방지해제
	click_protect_counter_clr();
}


void HandleShortButtonPress(void)
{
	
    // RF제어되고 있을때는 버튼처리를 하지 않는다.
    if( RUN_MODE_RF == g_run_mode ) {
        return;
    }
	

    // 버튼 동작후 300ms 이후에는 클릭방지
    if(m_is_click_protect == TRUE){
        //DEBUG_MSG_STR("btn m_is_click_protect return \r\n");
        return;
    }

	
    // 배터리값이 정상동작이하값이면 버튼 동작을 하지 않는다.
    if(is5VMode() == BATT_LVL_1){
        //DEBUG_MSG_STR("btn BATT_LVL_1 return \r\n");
        return;
    }
	
    
    //패턴 시퀀스 // 화이트 - 화이트 느린 점멸 - 화이트 빠른 점멸 - LED 꺼짐
    switch(switch_counter)
    {
        case 0:         //1번째 버튼 클릭 - 화이트 on      
            ccData.r = 0;
            ccData.g = 0;
            ccData.b = 255;
            ccData.bright = BRIGHT_MAX_VALUE;     		
            
            groupInfoCmd.syncTime = SECOND_RECEIVED_CMD;
            showInteractive( CMD_ALL_ON_COLOR );

        break;
            
        case 1:             // 2번째 버튼 클릭 - 화이트 느린 깜박
						ccData.r = 255;
            ccData.g = 255;
            ccData.b = 255;
            ccData.bright = BRIGHT_MAX_VALUE;  	
				
				ccData.runSpeed = RUN_SPEED_10;
				showInteractive( CMD_ALL_USER_COLOR_BLINK );
        break;                

		case 2:                    // 3번째 버튼 클릭 - 화이트 빠른 깜박
			ccData.r = 255;
            ccData.g = 255;
            ccData.b = 255;
            ccData.bright = BRIGHT_MAX_VALUE;    		
            
            //groupInfoCmd.syncTime = SECOND_RECEIVED_CMD;
            //showInteractive( CMD_ALL_ON_COLOR );
		
		    ccData.runSpeed = RUN_SPEED_20;
				showInteractive( CMD_ALL_USER_COLOR_BLINK );
		
		break;
		
	
       
        case 3:  
            
            groupInfoCmd.syncTime = SECOND_RECEIVED_CMD;
            showInteractive( CMD_ALL_OFF );
			//PWM_change_duty(0,0,0,0);
        break;
        
        default:
        break;
    }
    
    //버튼 패턴 카운터 초기화
    if(switch_counter >= 3)
        switch_counter = 0;
    else
        switch_counter++;
    
    /* 
        bt없이 공연하는 공연일경우 bt신호가 방해되기때문에 제거함
        좌석을 입력하는 공연의 경우 고려필요
    */
    //if( showStatus != SHOW_BT_OFF ) {  
    //    btOn();
    //}

}


void PIO_button_proc(void)
{
	debounce_counter_clr();
	
	debounce_counter_set(20);
}