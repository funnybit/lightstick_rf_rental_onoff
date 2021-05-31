#include "Common.h"
#include "N76E003.h"
#include "Function_define.h"
#include "SFR_Macro.h"
#include "delay.h"

#include "ADC.h"
#include "UART.h"
#include "cheerbong_info.h"
#include "color_control.h"
#include "control_manager.h"
#include "main.h"
#include "PWM.h"
#include "evb.h"

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

// 배터리 값 평균내기 관련 변수
#define BATT_AVERAGE_COUNT 3      // 평균값을 내기 위한 샘플링 횟수. 샘플링 횟수를 늘리려면 이 값을 바꾸면 된다. 
                                  // ex. 3(3번 샘플링후 평균값 구함), 5(5번 샘플링후 평균값을 구함)

bool g_bADC_complete_flag=FALSE;
bool lowBattery = FALSE;
UINT16 readBatteryValue = 0;
//uint8 fastCheckCount = 0;
UINT16 currentBatteryValue = 0;      // 배터리 평균값 최종값
UINT16 tempCurrentBatteryValue = 0;  // 배터리 평균값을 내기 위해서 값을 더해 두는 곳.
uint8 averCount = 0;
BATT_LVL_T g_batt_lvl;
bool g_bADC_first_check_flag=FALSE;

void RFSleep(void);

extern void set_Internal_OSC(void);
extern void set_External_OSC(void);
extern void BOD_Init(void);

void ADC_Init(void)
{
	// funnylogic p17 스위치인풋으로 쓰기 위해서 주석처리함
	/*
	P17_Quasi_Mode;
	P17 = 0;
	
	Enable_ADC_AIN0;						// Enable AIN0 P1.7 as ADC input, Find in "Function_define.h" - "ADC INIT"
	set_EADC;
	set_EA;
	*/
	
	P11_Quasi_Mode;
	P11 = 0;
	
	Enable_ADC_AIN7;						// Enable AIN0 P1.7 as ADC input, Find in "Function_define.h" - "ADC INIT"
	set_EADC;
	set_EA;
	
	g_batt_lvl = BATT_LVL_2; //초기 배터리 레벨 셋팅은 LV2, 
	g_bADC_first_check_flag = TRUE;//배터리 체크 주기 이후 첫 읽기 시에 3회 반복 읽기하도록 변경 //three_time_ADC_read();
}

/******************************************************************************
 * FUNCTION_PURPOSE: ADC interrupt Service Routine
 ******************************************************************************/
void ADC_ISR (void) interrupt 11
{
    clr_ADCF;                               //clear ADC interrupt flag
		//printf ("\n Value = 0x%bx",ADCRH);
	g_bADC_complete_flag=TRUE;
	
	//Send_Data_To_UART0('K');	
	
}


void three_time_ADC_read(void)
{
	// if((sceneMode == MODE_OFF) && (g_last_LED_R == 0) && (g_last_LED_G == 0) &&
	// 	(g_last_LED_B == 0) && (g_last_LED_W == 0))
	// {
		clr_ADCF;
		g_bADC_complete_flag = FALSE;
		set_ADCEN;
		//Timer0_Delay1ms_12MHz(3);
		set_ADCS;	//adc start
		
		while(1)
		{ 
			//ADC refresh
			if(g_bADC_complete_flag == TRUE)
			{
				g_bADC_complete_flag = FALSE;
				//refreshBattValue();
				
				// T002
				//Send_Data_To_UART0('T');
				//Send_Data_To_UART0('0');
				//Send_Data_To_UART0('0');
				//Send_Data_To_UART0('2');
				//Send_Data_To_UART0('\r');
				//Send_Data_To_UART0('\n');
				
				readBatteryValue = (0x0fff & (ADCRH<< 4)) | (0x000f & ADCRL);
				clr_ADCEN;
				
				//전압체크 코드
				////Send_UINT8_To_UART0(readBatteryValue>>8);
				////Send_UINT8_To_UART0(readBatteryValue & 0x00ff);
				////Send_Data_To_UART0('\r');
				////Send_Data_To_UART0('\n');
				
				++averCount;
				
				tempCurrentBatteryValue += readBatteryValue;

				// Send_Data_To_UART0('B');
				// Send_Data_To_UART0('2');
				// Send_Data_To_UART0(':');
				// Send_UINT8_To_UART0(readBatteryValue>>8);
				// Send_UINT8_To_UART0(readBatteryValue & 0x00ff);
				// Send_Data_To_UART0('\r');
				// Send_Data_To_UART0('\n');
				
				// 읽어들인 배터리값으로 평균값을 구한다.
				if( averCount >= BATT_AVERAGE_COUNT ) {
					averCount = 0;
					tempCurrentBatteryValue /= BATT_AVERAGE_COUNT;
					//SHOW_MSG_STR("data mod\r\n");
					currentBatteryValue = tempCurrentBatteryValue;
					tempCurrentBatteryValue = 0;
					
					if( currentBatteryValue < 0x0a83 ) {   // 3.3이하.. LOW battery
						g_batt_lvl = BATT_LVL_1;
						lowBattery = TRUE;

						// Send_Data_To_UART0('b');
						// Send_Data_To_UART0('1');
						// Send_Data_To_UART0('1');
						// Send_Data_To_UART0('\r');
						// Send_Data_To_UART0('\n');
							
						// RF로 켜졌던 마지막 상태를 초기화해서 다시 켜지지 않도록 한다.
						//LED 꺼짐
						groupInfoCmd.syncTime = SECOND_RECEIVED_CMD;
						showInteractive( CMD_ALL_OFF );
					}
					else if( (currentBatteryValue >= 0x0a83) && (currentBatteryValue < 0x0d35) ) {
						g_batt_lvl = BATT_LVL_2;
						// Send_Data_To_UART0('N');
						// Send_Data_To_UART0('\r');
						// Send_Data_To_UART0('\n');

						// b11, b22
						// Send_Data_To_UART0('b');
						// Send_Data_To_UART0('2');
						// Send_Data_To_UART0('2');
						// Send_Data_To_UART0('\r');
						// Send_Data_To_UART0('\n');
					}
					else {
						g_batt_lvl = BATT_LVL_3;
						// log:b31
						// Send_Data_To_UART0('b');
						// Send_Data_To_UART0('3');
						// Send_Data_To_UART0('1');
						// Send_Data_To_UART0('\r');
						// Send_Data_To_UART0('\n');	
					}	
					
					//전압체크 코드
					// log:ba
					// Send_Data_To_UART0('b');
					// Send_Data_To_UART0('a');
					// Send_Data_To_UART0(':');
					// Send_UINT8_To_UART0(currentBatteryValue>>8);
					// Send_UINT8_To_UART0(currentBatteryValue & 0x00ff);
					// Send_Data_To_UART0('\r');
					// Send_Data_To_UART0('\n');
					break;
				}
				else
				{
					set_ADCEN;
					//Timer0_Delay1ms_12MHz(3);
					set_ADCS;	//adc start
				}
			}
		}
	// }
	// else
	// {
	// 	//LED가 켜져있었으므로 다시 읽기 시도한다
	// 	// f11
	// 	// Send_Data_To_UART0('F');
	// 	// Send_Data_To_UART0('1');
	// 	// Send_Data_To_UART0('1');
	// 	// Send_Data_To_UART0('\r');
	// 	// Send_Data_To_UART0('\n');

	// 	g_bADC_first_check_flag = TRUE;
	// 	// log:a1
	// 	Send_Data_To_UART0('a');
	// 	Send_Data_To_UART0('1');
	// 	Send_Data_To_UART0('\r');
	// 	Send_Data_To_UART0('\n');
	// }
}

void refreshBattValue(void)
{
	// ++bongStatus.sleepCount;

	// Send_Data_To_UART0('\r');
	// Send_Data_To_UART0('\n');
	// Send_Data_To_UART0('-');
	// Send_Data_To_UART0('-');
	// Send_Data_To_UART0('\r');
	// Send_Data_To_UART0('\n');

	// log:sle
	// Send_Data_To_UART0('S');
	// Send_Data_To_UART0('L');
	// Send_Data_To_UART0('E');	
	// Send_UINT8_To_UART0(bongStatus.sleepCount>>8);
	// Send_UINT8_To_UART0(bongStatus.sleepCount & 0x00ff);
	// Send_Data_To_UART0('\r');
	// Send_Data_To_UART0('\n');	
	
	//low 배터리로 판정이 난 후에는 더이상 체크하지 않도록 방어

	// if( lowBattery == FALSE ) {
	// 	Send_Data_To_UART0('N');
	// 	Send_Data_To_UART0('L');
	// 	Send_Data_To_UART0('B');
	// 	Send_Data_To_UART0('\r');
	// 	Send_Data_To_UART0('\n');
	// }
	// else {
	// 	Send_Data_To_UART0('L');
	// 	Send_Data_To_UART0('B');
	// 	Send_Data_To_UART0('\r');
	// 	Send_Data_To_UART0('\n');
	// }

	// pit
	// if( bongStatus.powerCableInsert == FALSE ) {
	// 	Send_Data_To_UART0('P');
	// 	Send_Data_To_UART0('I');
	// 	Send_Data_To_UART0('T');
	// 	Send_Data_To_UART0('\r');
	// 	Send_Data_To_UART0('\n');
	// }
	// else {
	// 	Send_Data_To_UART0('P');
	// 	Send_Data_To_UART0('I');
	// 	Send_Data_To_UART0('F');
	// 	Send_Data_To_UART0('\r');
	// 	Send_Data_To_UART0('\n');
	// }

    if(lowBattery == FALSE) {
		
        //readBatteryValue = AioRead( BATTERY_AIO );    //배터리 잔량 읽기
		readBatteryValue = (0x0fff & (ADCRH<< 4)) | (0x000f & ADCRL);
		clr_ADCEN;
		
		//if((sceneMode == MODE_OFF) && (ccData.r == 0) && (ccData.g == 0) && (ccData.b==0))
		//if(sceneMode == MODE_OFF)
		//{
			
		
			///*
			//전압체크 코드
			// log:BAT:
			// Send_Data_To_UART0('B');
			// Send_Data_To_UART0('1');
			// Send_Data_To_UART0(':');
			// Send_UINT8_To_UART0(readBatteryValue>>8);
			// Send_UINT8_To_UART0(readBatteryValue & 0x00ff);
			// Send_Data_To_UART0('\r');
			// Send_Data_To_UART0('\n');

			//*/
		
			
			
			// TODO
			// 배터리가 아닌 경우에 대한 기준 전압 필요함. funnylogic
			/*
			  0x460의 의미
			  usb허브에 꼽고 확인해보면 값이 변하기는 하는데(예, 0x477 ~ 0x46a등을 왔다갔다함.) 좀더 
			  마진을 둬서 0x460을 기준전압으로 잡았다. 
			*/  
			//if( (readBatteryValue >= 0xea8 ) && ( fastCheckCount <= 0 ) ) {
			//    fastCheckCount = 3;
				//fastBatteryCheck = TRUE;        
				//SHOW_MSG_STR("batt gap over 20\r\n");
			//}
				
			/*
			// TODO 
			// 배터리값 차가 명확히 나는 경우를 찾아야한다. 건전지 <-> usb 간 전환시 어느정도의 차이가 나는지,
			// 20이라는 값은 실제 어느정도의 차이인지를 검토해야함. 
			// 이에 따라서 20이라는 값은 바뀔 수 있음.
			if( battGap >= 20 ) {
				SHOW_MSG_STR("batt gap over 20\r\n");
				fastCheckCount = 3;
				fastBatteryCheck = TRUE;
			}   
			*/
			if((sceneMode == MODE_OFF) && (g_last_LED_R == 0) && (g_last_LED_G == 0) &&
				(g_last_LED_B == 0) && (g_last_LED_W == 0))
			{
				// c11
				// Send_Data_To_UART0('C');
				// Send_Data_To_UART0('1');
				// Send_Data_To_UART0('1');
				// Send_Data_To_UART0('\r');
				// Send_Data_To_UART0('\n');

				if( currentBatteryValue <= 0 ) {
					averCount = 0;
					++averCount;
					
					///*
					//SHOW_MSG_STR("arc : ");
					//SHOW_MSG_UINT16( averCount );
					//SHOW_MSG_STR("\r\n");
					//*/
					
					// 최초 부팅후에는 배터리값이 존재하지 않는다. 값이 없는 경우 오동작이 예상되므로 
					// 최초 부팅후에는 첫번째 읽은 값을 바로 넣어준다.
					// TODO funnylogic 최초 읽은 값이 큰 오차를 가지고 있다면? 
					currentBatteryValue = readBatteryValue;  
					tempCurrentBatteryValue = readBatteryValue;
					//SHOW_MSG_STR("add data - first\r\n");
					
					
					//전압체크 코드
					// log:CUR1:
					// Send_Data_To_UART0('C');
					// Send_Data_To_UART0('U');
					// Send_Data_To_UART0('R');
					// Send_Data_To_UART0(':');
					// Send_UINT8_To_UART0(currentBatteryValue>>8);
					// Send_UINT8_To_UART0(currentBatteryValue & 0x00ff);
					// Send_Data_To_UART0('\r');
					// Send_Data_To_UART0('\n');
					
					
				}
				else {
					// Send_Data_To_UART0('u');
					// Send_UINT8_To_UART0(bongStatus.powerCheckPassCount);
					// Send_Data_To_UART0('\r');
					// Send_Data_To_UART0('\n');

					if( bongStatus.powerCheckPassCount > 0 ) {
						--bongStatus.powerCheckPassCount;
						// pass
						// Send_Data_To_UART0('P');
						// Send_Data_To_UART0('A');
						// Send_Data_To_UART0('S');
						// Send_Data_To_UART0('S');
						// Send_Data_To_UART0('\r');
						// Send_Data_To_UART0('\n');
					}
					else {
						++averCount;
						
						/*
						SHOW_MSG_STR("arc : ");
						SHOW_MSG_UINT16( averCount );
						SHOW_MSG_STR("\r\n");
						*/
					
						tempCurrentBatteryValue += readBatteryValue;              
						//SHOW_MSG_STR("add data\r\n");
						
						// 읽어들인 배터리값으로 평균값을 구한다.
						if( averCount >= BATT_AVERAGE_COUNT ) {
							averCount = 0;
							tempCurrentBatteryValue /= BATT_AVERAGE_COUNT;
							//SHOW_MSG_STR("data mod\r\n");
							currentBatteryValue = tempCurrentBatteryValue;
							tempCurrentBatteryValue = 0;
						}
						
						//전압체크 코드
						// log:CUR2:
						// Send_Data_To_UART0('C');
						// Send_Data_To_UART0('U');
						// Send_Data_To_UART0('R');
						// Send_Data_To_UART0('2');
						// Send_Data_To_UART0(':');
						// Send_UINT8_To_UART0(currentBatteryValue>>8);
						// Send_UINT8_To_UART0(currentBatteryValue & 0x00ff);
						// Send_Data_To_UART0('\r');
						// Send_Data_To_UART0('\n');
					}
				}        
				
				// 방어코드
				// 혹여 averCount가 정해놓은 수치를 넘었을때 오동작을 막기 위함.
				if( averCount > BATT_AVERAGE_COUNT ) {
					averCount = 0;
				}
				
				// 2018.12.06 
				// 선빈팀장이 측정하여 값을 카톡으로 보내줌
				// low : 2732
				// max : 3690
				//if( currentBatteryValue < 0x0a83 ) {  // LOW, 3.3
				if( currentBatteryValue < 0x0aac ) {  // LOW, 3.3
					g_batt_lvl = BATT_LVL_1;
					lowBattery = TRUE;
					groupInfoCmd.syncTime = SECOND_RECEIVED_CMD;
					showInteractive( CMD_ALL_OFF );

					// b11, b12
					// Send_Data_To_UART0('b');
					// Send_Data_To_UART0('1');
					// Send_Data_To_UART0('2');
					// Send_Data_To_UART0('\r');
					// Send_Data_To_UART0('\n');
					
					 //Send_Data_To_UART0('L');
					 //Send_Data_To_UART0('\r');
					 //Send_Data_To_UART0('\n');
				}
				//else if( (currentBatteryValue >= 0x0aac) && (currentBatteryValue < 0x0d35) ) {
				else if( (currentBatteryValue >= 0x0aac) && (currentBatteryValue < 0x0e6a) ) {
					g_batt_lvl = BATT_LVL_2;
					 //Send_Data_To_UART0('N');
					 //Send_Data_To_UART0('\r');
					 //Send_Data_To_UART0('\n');

					// b11, b22
					// Send_Data_To_UART0('b');
					// Send_Data_To_UART0('2');
					// Send_Data_To_UART0('2');
					// Send_Data_To_UART0('\r');
					// Send_Data_To_UART0('\n');
				}
				else {
					g_batt_lvl = BATT_LVL_3;
					// log:b31
					// Send_Data_To_UART0('b');
					// Send_Data_To_UART0('3');
					// Send_Data_To_UART0('1');
					// Send_Data_To_UART0('\r');
					// Send_Data_To_UART0('\n');	
					//Send_Data_To_UART0('H');
					//Send_Data_To_UART0('\r');
					//Send_Data_To_UART0('\n');
				}			
				
		}      // LED 꺼져있을때
		else   // LED 켜져있을때
		{
			// c21
			// Send_Data_To_UART0('C');
			// Send_Data_To_UART0('2');
			// Send_Data_To_UART0('1');
			// Send_Data_To_UART0('\r');
			// Send_Data_To_UART0('\n');
		}
    }    
    else {
		clr_ADCEN;
    }
}

BATT_LVL_T is5VMode(void){
    return g_batt_lvl;
}