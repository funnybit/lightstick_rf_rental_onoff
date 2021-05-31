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

// ���͸� �� ��ճ��� ���� ����
#define BATT_AVERAGE_COUNT 3      // ��հ��� ���� ���� ���ø� Ƚ��. ���ø� Ƚ���� �ø����� �� ���� �ٲٸ� �ȴ�. 
                                  // ex. 3(3�� ���ø��� ��հ� ����), 5(5�� ���ø��� ��հ��� ����)

bool g_bADC_complete_flag=FALSE;
bool lowBattery = FALSE;
UINT16 readBatteryValue = 0;
//uint8 fastCheckCount = 0;
UINT16 currentBatteryValue = 0;      // ���͸� ��հ� ������
UINT16 tempCurrentBatteryValue = 0;  // ���͸� ��հ��� ���� ���ؼ� ���� ���� �δ� ��.
uint8 averCount = 0;
BATT_LVL_T g_batt_lvl;
bool g_bADC_first_check_flag=FALSE;

void RFSleep(void);

extern void set_Internal_OSC(void);
extern void set_External_OSC(void);
extern void BOD_Init(void);

void ADC_Init(void)
{
	// funnylogic p17 ����ġ��ǲ���� ���� ���ؼ� �ּ�ó����
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
	
	g_batt_lvl = BATT_LVL_2; //�ʱ� ���͸� ���� ������ LV2, 
	g_bADC_first_check_flag = TRUE;//���͸� üũ �ֱ� ���� ù �б� �ÿ� 3ȸ �ݺ� �б��ϵ��� ���� //three_time_ADC_read();
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
				
				//����üũ �ڵ�
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
				
				// �о���� ���͸������� ��հ��� ���Ѵ�.
				if( averCount >= BATT_AVERAGE_COUNT ) {
					averCount = 0;
					tempCurrentBatteryValue /= BATT_AVERAGE_COUNT;
					//SHOW_MSG_STR("data mod\r\n");
					currentBatteryValue = tempCurrentBatteryValue;
					tempCurrentBatteryValue = 0;
					
					if( currentBatteryValue < 0x0a83 ) {   // 3.3����.. LOW battery
						g_batt_lvl = BATT_LVL_1;
						lowBattery = TRUE;

						// Send_Data_To_UART0('b');
						// Send_Data_To_UART0('1');
						// Send_Data_To_UART0('1');
						// Send_Data_To_UART0('\r');
						// Send_Data_To_UART0('\n');
							
						// RF�� ������ ������ ���¸� �ʱ�ȭ�ؼ� �ٽ� ������ �ʵ��� �Ѵ�.
						//LED ����
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
					
					//����üũ �ڵ�
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
	// 	//LED�� �����־����Ƿ� �ٽ� �б� �õ��Ѵ�
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
	
	//low ���͸��� ������ �� �Ŀ��� ���̻� üũ���� �ʵ��� ���

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
		
        //readBatteryValue = AioRead( BATTERY_AIO );    //���͸� �ܷ� �б�
		readBatteryValue = (0x0fff & (ADCRH<< 4)) | (0x000f & ADCRL);
		clr_ADCEN;
		
		//if((sceneMode == MODE_OFF) && (ccData.r == 0) && (ccData.g == 0) && (ccData.b==0))
		//if(sceneMode == MODE_OFF)
		//{
			
		
			///*
			//����üũ �ڵ�
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
			// ���͸��� �ƴ� ��쿡 ���� ���� ���� �ʿ���. funnylogic
			/*
			  0x460�� �ǹ�
			  usb��꿡 �Ű� Ȯ���غ��� ���� ���ϱ�� �ϴµ�(��, 0x477 ~ 0x46a���� �Դٰ�����.) ���� 
			  ������ �ּ� 0x460�� ������������ ��Ҵ�. 
			*/  
			//if( (readBatteryValue >= 0xea8 ) && ( fastCheckCount <= 0 ) ) {
			//    fastCheckCount = 3;
				//fastBatteryCheck = TRUE;        
				//SHOW_MSG_STR("batt gap over 20\r\n");
			//}
				
			/*
			// TODO 
			// ���͸��� ���� ��Ȯ�� ���� ��츦 ã�ƾ��Ѵ�. ������ <-> usb �� ��ȯ�� ��������� ���̰� ������,
			// 20�̶�� ���� ���� ��������� ���������� �����ؾ���. 
			// �̿� ���� 20�̶�� ���� �ٲ� �� ����.
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
					
					// ���� �����Ŀ��� ���͸����� �������� �ʴ´�. ���� ���� ��� �������� ����ǹǷ� 
					// ���� �����Ŀ��� ù��° ���� ���� �ٷ� �־��ش�.
					// TODO funnylogic ���� ���� ���� ū ������ ������ �ִٸ�? 
					currentBatteryValue = readBatteryValue;  
					tempCurrentBatteryValue = readBatteryValue;
					//SHOW_MSG_STR("add data - first\r\n");
					
					
					//����üũ �ڵ�
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
						
						// �о���� ���͸������� ��հ��� ���Ѵ�.
						if( averCount >= BATT_AVERAGE_COUNT ) {
							averCount = 0;
							tempCurrentBatteryValue /= BATT_AVERAGE_COUNT;
							//SHOW_MSG_STR("data mod\r\n");
							currentBatteryValue = tempCurrentBatteryValue;
							tempCurrentBatteryValue = 0;
						}
						
						//����üũ �ڵ�
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
				
				// ����ڵ�
				// Ȥ�� averCount�� ���س��� ��ġ�� �Ѿ����� �������� ���� ����.
				if( averCount > BATT_AVERAGE_COUNT ) {
					averCount = 0;
				}
				
				// 2018.12.06 
				// ���������� �����Ͽ� ���� ī������ ������
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
				
		}      // LED ����������
		else   // LED ����������
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