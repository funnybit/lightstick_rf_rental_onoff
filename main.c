//=============================
// 박지훈 히스토리
//
// ver 0.1 (2019.10.11)
// 1. 버튼 동작 변경
//
// ver 0.2 (2019.10.14)
// 1. 그룹2, 그룹3 연출번호 저장 및 불러오기 기능 추가
// 2. DMX로 그룹2, 그룹3 지정하여 제어가능하도록 기능 추가
// 3. DMX로 제어시 버튼 잠금 기능 추가
// 
// ver 1.3 (2019.12.03)
// 1. P12 인터럽트 disable처리(노이즈가 타고 들어와서 버튼 동작(p17)을 막는 경우가 있다.
//
// ver 1.4 (2019.12.04)
// 1. 제어코드 추가
//
//
// 더 보이즈(20.02.19)
// 위 사양을 그대로 이어감.
// 
// ver 1.0 (20.02.19)
// 1. 버튼씬 수정(white on - white flick - white blink - rainbow dimming - off)
//
// ver 1.1 (20.02.19)
// 1. 버튼 무지개 패턴 변경(빨 - 노 - 파)

// 조승연
// 위 더보이즈에서 이어옴
// 버전 1.0s
// 버튼동작 
// - 화이트 on - 느린점멸 - 빠른 점멸 - off
//=============================

#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "Delay.h"
#include "evb.h"
#include "UART.h"
#include "PIO.h"
#include "PWM.h"
#include "ADC.h"
#include "main.h"
#include "control_manager.h"
#include "cheerbong_info.h"
#include "color_control.h"
#include "action_service.h"

#include "UserDefine.h"
#include "UserDataFlash.h"
System_t    System;

#define Enable_ADC_AIN0			ADCCON0&=0xF0;P17_Input_Mode;AINDIDS=0x00;AINDIDS|=SET_BIT0;ADCCON1|=SET_BIT0									//P17

// I2C Master function
#define I2C_CLOCK               13
#define EEPROM_SLA              0xA4
#define EEPROM_WR               0
#define EEPROM_RD               1
#define ERROR_CODE              0x78
#define PAGE_SIZE               32

#define PRODUCT_COMPANY 17    // 회사 : 코코아
#define PRODUCT_TYPE         1    // 제품종류 : 응원봉
#define PRODUCT_STICK        1    // 가수 : 렌탈봉

uint8 iLoopA;
uint8 iLoopB;

uint16 product_type;

bool runContinue;

bool anotherBong;
uint8 rainbowWhere;

uint8 half;
uint8 groupSelect;
uint8 autoSave;

#define T0_PERIOD_US 1000	//1000us

#define TH1_INIT        HIBYTE(TIMER_DIV12_VALUE_10ms)//0xE0 //2.5ms@XTAL=12MHz, Period = (5.425/2) ms@XTAL=22.1184MHz
#define TL1_INIT        LOBYTE(TIMER_DIV12_VALUE_10ms)//0x00
//#define TH0_INIT	253		//64869 >> 8 = 253, TIMER_DIV12_VALUE_500us			64869
//#define TL0_INIT    101    	//64869 & 0xFF = 101, TIMER_DIV12_VALUE_500us			64869
//#define TH0_INIT	250		//64202 >> 8 = 250, TIMER_DIV12_VALUE_1ms			64202
//#define TL0_INIT    202    	//64202 & 0xFF = 202, TIMER_DIV12_VALUE_1ms			64202
#define TH0_INIT	0xfc		// 12MHz
#define TL0_INIT    0x18    	// 12MHz

bit isParsingData = FALSE;


//UINT8 RF_check_timer_enable = 0;
bit RF_check_timer_enable = 0;
UINT16 RF_check_counter = 0;
UINT16 RF_check_cnt_limit = 0;
UINT8 RF_check_flag = 0;


UINT8 PIO_button_check_flag = 0;

UINT8 debounce_timer_enable = 0;
UINT16 debounce_counter = 0;
UINT16 debounce_cnt_limit = 0;
UINT8 debounce_flag = 0;

UINT8 click_protect_timer_enable = 0;
UINT16 click_protect_counter = 0;
UINT16 click_protect_cnt_limit = 0;
UINT8 click_protect_flag = 0;

//UINT8 showing_alive_timer_enable = 0;
bit showing_alive_timer_enable = 0;
UINT32 showing_alive_counter = 0;
UINT32 showing_alive_cnt_limit = 0;
UINT8 showing_alive_flag = 0;


// 롱키 리셋관련 변수추가
UINT8 long_key_timer_enable = 0;
UINT32 long_key_counter = 0;
UINT32 long_key_cnt_limit = 0;
UINT8 long_key_flag = 0;
	
UINT8 g_TI=0;

bool m_is_rf_reinit = FALSE;
extern BONG_INFO_T ccData;

//bool parsingPassByReset = FALSE;
bit parsingPassByReset = FALSE;

UINT8 switch_counter =0;

SHOW_STATUS showStatus;
//UINT8 rcvDataStage = 0;

extern UINT8 dataCheckCount;
extern UINT8 dataBodyCheckCount;

extern UINT16 rcvBufferIndex;
extern UINT8 rcvBuffer[130];

int showing_alive_counter_set(UINT32 time_ms);
void showing_alive_counter_clr(void);

BONG_STATUS_T bongStatus;
GROUP_INFO_CONTROL_T groupInfoCmd;
GROUP_INFO_T groupInfo;

UINT8 g_batt_low_LED_off_counter =0;

extern BATT_LVL_T g_batt_lvl;

void BOD_Init(void);

extern bool lowBattery;

void set_Internal_OSC(void);
void set_External_OSC(void);
void initBong(void);
extern void setColor(uint8 color);
void processAll_control(char *buffer);
void processInteractive_Vertical_control(char *buffer);
void processInteractive_Horizontal_control(char *buffer);
void save_interactive_num(char *buffer);
void readSaveData(void);
void process_DMX_control(char *buffer);
void processInteractive_Shape_control(char *buffer);

void delayForWrite(void);


enum BONG_ACTION {
    ACTION_OFF = 1,                                   // 전체끄기                          
    ACTION_ON_SELECT_COLOR,                           // 선택색 켜기               
    ACTION_DIMMING_ALL_REPEAT_0_100,                  // 전체 디밍 반복(0-100)
    ACTION_DIMMING_ALL_REPEAT_20_100,                 // 전체 디밍 반복(20-100)
    ACTION_ALL_FLICKER_SELECT_COLOR,                  // 전체 점멸(선택색)
    ACTION_ALL_FLICKER_SAME_COLOR_RANDOM,             // 전체 점멸(전체 동일컬러 랜덤)
    ACTION_ALL_FLICKER_EACH_COLOR_RANDOM,             // 전체 점멸(전체 개별 랜덤)
    ACTION_ALL_EACH_TIME_RANDOM_SELECT_COLOR,         // 개별 랜덤 시간 점멸(선택색)
    ACTION_ALL_2COLOR_SMOOTH_TURN_SELECT_COLOR,       // 전체 선택 2색 부드럽게 반복
    ACTION_ALL_2COLOR_TURN_SELECT_COLOR,              // 전체 2컬러 반복(선택색 2색)
    ACTION_ALL_RAINBOW,                               // 전체 무지개
    ACTION_ALL_DIMMING_ON_SELECT_COLOR,               // 전체 디밍으로 LED 켜기
    ACTION_ALL_DIMMING_OFF,                           // 전체 부드럽게 끄기
    ACTION_ALL_COLOR_SMOOTH_TURN_TO_SELECT_COLOR,     // 전체 부드럽게 컬러 변경
    ACTION_ALL_SMOOTH_ON_REPEAT_SELECT_COLOR,         // 선택색 부드럽게 ON 반복
    ACTION_ALL_SMOOTH_OFF_REPEAT_SELECT_COLOR,        // 선택색 부드럽게 OFF 반복    
    ACTION_ALL_DISABLE1,
    ACTION_ALL_DISABLE2,
    ACTION_FUNC_INPUT_VERTICAL_NUM,                   // 세로 연출번호 입력
	ACTION_FUNC_INPUT_HORIZON_NUM,                    // 가로 연출번호 입력
	ACTION_FUNC_INPUT_FLOOR_NUM,                      // 층 연출번호 입력
	ACTION_FUNC_INPUT_LINE_NUM,                       // 라인 연출번호 입력
	ACTION_FUNC_INPUT_SPECIAL_NUM,                    // 모양 연출번호 입력
	ACTION_RUNMODE_NORMAL,                            // 일반모드(BT on, 버튼 enable)
	ACTION_RUNMODE_SHOW,                              // 공연모드(BT off, 버튼 disable)
    ACTION_ALL_SPEED_FLASH,                            // 전체 빠르게 번쩍
    ACTION_FUNC_GO_SLEEP,                             // 슬립으로 진입(30초후)
    ACTION_FUNC_SHOW_NUMBER_CHECK,                     // 연출번호 입력 체크
	ACTION_FUNC_SHOW_VER_CHECK,                        // 공연버전 체크
    ACTION_FUNC_FREQ_CHANGE,                            // 주파수 변경
    ACTION_BTN_RAINBOW
};

enum {
	CONTROL_MODE_ALL = 1,   // 전체 제어
	CONTROL_MODE_VERTICAL,  // 세로 제어
	CONTROL_MODE_HORIZON,   // 가로 제어
	CONTROL_MODE_FLOOR,     // 층 제어
	CONTROL_MODE_LINE,      // 라인 제어
	CONTROL_MODE_SHAPE,     // 모양 제어
    CONTROL_MODE_DMX
};

#define FLAG_1 0x01
#define FLAG_2 0x02
#define FLAG_3 0x04
#define FLAG_4 0x08
#define FLAG_5 0x10
#define FLAG_6 0x20
#define FLAG_7 0x40
#define FLAG_8 0x80

#if 0
void Init_I2C(void)
{
//    /* Set I2C clock rate */
    I2CLK = I2C_CLOCK; 

    /* Enable I2C */
    set_I2CEN;                                   
}
#endif

void I2C_Error(void)
{
//    P3 = I2STAT;
//    P3 = ERROR_CODE;
    while (1);    
}
//========================================================================================================
void I2C_Process(UINT8 u8DAT)
{
    UINT32 u32Count;

//--------------------------------------------------------------------------------------------
//----  Page Write----------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
    /* Step1 */
    set_STA;                                    /* Send Start bit to I2C EEPROM */
    clr_SI;
    while (!SI);                                //Check SI set or not
    if (I2STAT != 0x08)                         //Check status value after every step
        I2C_Error();
    
    /* Step2 */
    clr_STA;                                    //STA=0
    I2DAT = (EEPROM_SLA | EEPROM_WR);
    clr_SI;
    while (!SI);                                //Check SI set or not
    if (I2STAT != 0x18)              
        I2C_Error();

    /* Step3 */
    I2DAT = 0x00;                               //address high for I2C EEPROM
    clr_SI;
    while (!SI);                                //Check SI set or not
    if (I2STAT != 0x28)              
        I2C_Error();
                    
    /* Step4 */
    I2DAT = 0x00;                               //address low for I2C EEPROM
    clr_SI;
    while (!SI);                                //Check SI set or not
    if (I2STAT != 0x28)              
        I2C_Error();

    /* Step5 */
    for (u32Count = 0; u32Count < PAGE_SIZE; u32Count++)
    {
        I2DAT = u8DAT;
        clr_SI;
        while (!SI);                            //Check SI set or not
        if (I2STAT != 0x28)              
            I2C_Error();

        u8DAT = ~u8DAT;        
    }

//--------------------------------------------------------------------------------------------
//----  Waitting the ready for I2C write------------------------------------------------------
//--------------------------------------------------------------------------------------------
    /* Step6 */
    do{
        set_STO;
        clr_SI;
        
        set_STA;                                //Check if no ACK is returned by EEPROM, it is under timed-write cycle
        clr_SI;
        while (!SI);                            //Check SI set or not
        if (I2STAT != 0x08)                     //Check status value after every step
            I2C_Error();

        clr_STA;
        I2DAT = (EEPROM_SLA | EEPROM_WR);
        clr_SI;
        while (!SI);                            //Check SI set or not
    }while (I2STAT != 0x18);
    
    /* Step7 */
    set_STO;
    clr_SI;
    while (STO);                                /* Check STOP signal */
//--------------------------------------------------------------------------------------------
//----  Page Read ----------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
    /* Step8 */
    set_STA;
    clr_SI;          
    while (!SI);                                //Check SI set or not
    if (I2STAT != 0x08)                         //Check status value after every step
        I2C_Error();

    /* Step9 */
    I2DAT = (EEPROM_SLA | EEPROM_WR);
    clr_STA;
    clr_SI;
    while (!SI);                                //Check SI set or not
    if (I2STAT != 0x18)              
        I2C_Error();

    /* Step10 */
    I2DAT = 0x00;                               //address high for I2C EEPROM
    clr_SI;
    while (!SI);                                //Check SI set or not
    if (I2STAT != 0x28)              
        I2C_Error();

    /* Step11 */
    I2DAT = 0x00;                               //address low for I2C EEPROM
    clr_SI;
    while (!SI);                                //Check SI set or not
    if (I2STAT != 0x28)              
        I2C_Error();

    /* Step12 */
    /* Repeated START */
    set_STA;                       
    clr_SI;
    while (!SI);                                //Check SI set or not
    if (I2STAT != 0x10)                         //Check status value after every step
        I2C_Error();
    
    /* Step13 */
    clr_STA;                                    //STA needs to be cleared after START codition is generated
    I2DAT = (EEPROM_SLA | EEPROM_RD);
    clr_SI;
    while (!SI);                                //Check SI set or not
    if (I2STAT != 0x40)              
        I2C_Error();
    
    /* Step14 */
    for (u32Count = 0; u32Count <PAGE_SIZE-1; u32Count++)
    {
        set_AA;
        clr_SI;        
        while (!SI);                            //Check SI set or not

        if (I2STAT != 0x50)              
            I2C_Error();
        
        if (I2DAT != u8DAT)             
            I2C_Error();
        u8DAT = ~u8DAT; 
    } 
    
    /* Step15 */
    clr_AA;
    clr_SI;
    while (!SI);                                //Check SI set or not
    if (I2STAT != 0x58)              
        I2C_Error();

    /* Step16 */
    set_STO;
    clr_SI;
    while (STO);                                /* Check STOP signal */ 
}
 
 void showTimeStatus(uint8 show) {
	  showStatus = show;
    
    if( showStatus == SHOW_ING ) {                 // 공연중(버튼 동작 막음)
        //showing_alive_counter = 0;
		
		showing_alive_counter_clr();
		
		//60 minute counter
		showing_alive_counter_set(3600000);
        
        changeRunMode( RUN_MODE_RF );   //버튼동작불가
        
		//todo setEventLedState
        //setEventLedState(LED_EVENT_STATE_IDLE);
		
    }
    else if( showStatus == SHOW_END ) {            // 공연 종료
		
        changeRunMode( RUN_MODE_NOMAL );   //버튼동작가능

        showing_alive_counter_clr();
		
    }
    //else if( showStatus == SHOW_BT_OFF ) {         // BT OFF
    //    btAllOff();
    //}
 }
 

/************************************************************************************************************
*    TIMER 0 interrupt subroutine
************************************************************************************************************/
void Timer0_ISR (void) interrupt 1          //interrupt address is 0x000B
{
	// 아래값으로 계산하는데, xtal 12Mhz로 계산을 하는데, 
	// 아래값이 1ms로 맞춰져있음
    TH0 = TH0_INIT;
	TL0 = TL0_INIT;    

	if( bongStatus.syncCalculer == TRUE ) {
		++bongStatus.syncCounter;

		if( groupInfoCmd.syncTime == FIRST_RECEIVED_CMD ) {
			if( bongStatus.syncCounter >= TIME_80_MS ) {
				bongStatus.syncCalculer = FALSE;
				bongStatus.syncCounter = 0;

				////Send_Data_To_UART0('\n');
				////Send_Data_To_UART0('7');
				////Send_Data_To_UART0('3');

				bongStatus.runActionWithSync = TRUE;
			}
			//DEBUG_MSG_STR("in s-3\r\n");
		   // actionCreateTimer(ACTION_TIME_200);
		}       
		else if( groupInfoCmd.syncTime == SECOND_RECEIVED_CMD ) {
			if( bongStatus.syncCounter >= TIME_60_MS ) {
				bongStatus.syncCalculer = FALSE;
				bongStatus.syncCounter = 0;

				////Send_Data_To_UART0('\n');
				////Send_Data_To_UART0('7');
				////Send_Data_To_UART0('4');

				bongStatus.runActionWithSync = TRUE;
			}
			//DEBUG_MSG_STR("in s-4\r\n"); 
			//actionCreateTimer(ACTION_TIME_100);
		}   
		else if( groupInfoCmd.syncTime == THIRD_RECEIVED_CMD ) {
			if( bongStatus.syncCounter >= TIME_40_MS ) {
				bongStatus.syncCalculer = FALSE;
				bongStatus.syncCounter = 0;

				////Send_Data_To_UART0('\n');
				////Send_Data_To_UART0('7');
				////Send_Data_To_UART0('4');

				bongStatus.runActionWithSync = TRUE;
			}
			//DEBUG_MSG_STR("in s-4\r\n"); 
			//actionCreateTimer(ACTION_TIME_100);
		}   
		else if( groupInfoCmd.syncTime == FOURTH_RECEIVED_CMD ) {
			if( bongStatus.syncCounter >= TIME_20_MS ) {
				bongStatus.syncCalculer = FALSE;
				bongStatus.syncCounter = 0;

				////Send_Data_To_UART0('\n');
				////Send_Data_To_UART0('7');
				////Send_Data_To_UART0('4');

				bongStatus.runActionWithSync = TRUE;
			}
			//DEBUG_MSG_STR("in s-4\r\n"); 
			//actionCreateTimer(ACTION_TIME_100);
		}   
		else {
			//DEBUG_MSG_STR("run err\r\n");
		}    
	}

	if( bongStatus.continueRunCalculer == TRUE ) {
		++bongStatus.continueCounter;
		
		runContinue = FALSE;


		if( ccData.runSpeed == RUN_SPEED_1 ) {       
			if( bongStatus.continueCounter >= RUN_SPEED_40_MS ) {
				runContinue = TRUE;
			}
		}
		else if( ccData.runSpeed == RUN_SPEED_2 ) {  
			if( bongStatus.continueCounter >= RUN_SPEED_39_MS ) {
				runContinue = TRUE;
			}
		}
		else if( ccData.runSpeed == RUN_SPEED_3 ) { 
			if( bongStatus.continueCounter >= RUN_SPEED_37_MS ) {
				runContinue = TRUE;
			}
		}
		else if( ccData.runSpeed == RUN_SPEED_4 ) {  
			if( bongStatus.continueCounter >= RUN_SPEED_35_MS ) {
				runContinue = TRUE;
			}
		}
		else if( ccData.runSpeed == RUN_SPEED_5 ) {  
			if( bongStatus.continueCounter >= RUN_SPEED_33_MS ) {
				runContinue = TRUE;
			}
		}	
		else if( ccData.runSpeed == RUN_SPEED_6 ) {  
			if( bongStatus.continueCounter >= RUN_SPEED_31_MS ) {
				runContinue = TRUE;
			}
		}	
		else if( ccData.runSpeed == RUN_SPEED_7 ) {  
			if( bongStatus.continueCounter >= RUN_SPEED_29_MS ) {
				runContinue = TRUE;
			}
		}	
		else if( ccData.runSpeed == RUN_SPEED_8 ) {  
			if( bongStatus.continueCounter >= RUN_SPEED_27_MS ) {
				runContinue = TRUE;
			}
		}	
		else if( ccData.runSpeed == RUN_SPEED_9 ) {  
			if( bongStatus.continueCounter >= RUN_SPEED_25_MS ) {
				runContinue = TRUE;
			}
		}	
		else if( ccData.runSpeed == RUN_SPEED_10 ) {  
			if( bongStatus.continueCounter >= RUN_SPEED_23_MS ) {
				runContinue = TRUE;
			}
		}
		else if( ccData.runSpeed == RUN_SPEED_11 ) {  
			if( bongStatus.continueCounter >= RUN_SPEED_21_MS ) {
				runContinue = TRUE;
			}
		}
		else if( ccData.runSpeed == RUN_SPEED_12 ) {  
			if( bongStatus.continueCounter >= RUN_SPEED_19_MS ) {
				runContinue = TRUE;
			}
		}
		else if( ccData.runSpeed == RUN_SPEED_13 ) {  
			if( bongStatus.continueCounter >= RUN_SPEED_17_MS ) {
				runContinue = TRUE;
			}
		}
		else if( ccData.runSpeed == RUN_SPEED_14 ) {  
			if( bongStatus.continueCounter >= RUN_SPEED_15_MS ) {
				runContinue = TRUE;
			}
		}
		else if( ccData.runSpeed == RUN_SPEED_15 ) {  
			if( bongStatus.continueCounter >= RUN_SPEED_13_MS ) {
				runContinue = TRUE;
			}
		}
		else if( ccData.runSpeed == RUN_SPEED_16 ) {  
			if( bongStatus.continueCounter >= RUN_SPEED_11_MS ) {
				runContinue = TRUE;
			}
		}
		else if( ccData.runSpeed == RUN_SPEED_17 ) {  
			if( bongStatus.continueCounter >= RUN_SPEED_9_MS ) {
				runContinue = TRUE;
			}
		}
		else if( ccData.runSpeed == RUN_SPEED_18 ) {  
			if( bongStatus.continueCounter >= RUN_SPEED_7_MS ) {
				runContinue = TRUE;
			}
		}
		else if( ccData.runSpeed == RUN_SPEED_19 ) {  
			if( bongStatus.continueCounter >= RUN_SPEED_5_MS ) {
				runContinue = TRUE;
			}
		}
		else if( ccData.runSpeed == RUN_SPEED_20 ) {  
			if( bongStatus.continueCounter >= RUN_SPEED_3_MS ) {
				runContinue = TRUE;
			}
		}
		
		if( runContinue == TRUE ) {
			runContinue = FALSE;
			bongStatus.continueCounter = 0;
			actionHandler();
		}
	}
	
	if(RF_check_timer_enable == 1)
	{
		RF_check_counter++;
		
		if(RF_check_counter >= RF_check_cnt_limit)
		{
			RF_check_counter = 0;
			RF_check_flag = 1;
		}
	}
	
	if(debounce_timer_enable == 1)
	{
		// log:DEB1
		// Send_Data_To_UART0('D');
		// Send_Data_To_UART0('E');
		// Send_Data_To_UART0('B');
		// Send_Data_To_UART0('1');
		// Send_Data_To_UART0('\r');
		// Send_Data_To_UART0('\n');

		debounce_counter++;
		
		if(debounce_counter >= debounce_cnt_limit)
		{
			// log:DEB1-1
			// Send_Data_To_UART0('D');
			// Send_Data_To_UART0('E');
			// Send_Data_To_UART0('B');
			// Send_Data_To_UART0('1');
			// Send_Data_To_UART0('-');
			// Send_Data_To_UART0('1');
			// Send_Data_To_UART0('\r');
			// Send_Data_To_UART0('\n');
			debounce_counter = 0;
			debounce_flag = 1;
		}
	}
	
	if(click_protect_timer_enable == 1)
	{
		click_protect_counter++;
		
		if(click_protect_counter >= click_protect_cnt_limit)
		{
			click_protect_counter = 0;
			click_protect_flag = 1;
		}
	}
	
	if(showing_alive_timer_enable == 1)
	{
		showing_alive_counter++;
		
		if(showing_alive_counter >= showing_alive_cnt_limit)
		{
			showing_alive_counter = 0;
			showing_alive_flag = 1;
		}
	}
	
	// 롱키 리셋관련
	if(long_key_timer_enable == 1)
	{
		if(P17 == 1)
		{
			long_key_counter++;
			
			if(long_key_counter >= long_key_cnt_limit)
			{
				long_key_counter = 0;
				long_key_flag = 1;
			}
		}
		else
		{
			long_key_counter=0;
		}
	}
	
}

int RF_check_counter_set(UINT32 time_ms)
{
	int ret=0;
	
	if(RF_check_timer_enable == 0)
	{
		RF_check_counter = 0;
		RF_check_cnt_limit = (time_ms * 1000) / T0_PERIOD_US;
		RF_check_timer_enable = 1;
	}
	else
		ret = -1;
	
	return ret;
}

#if 0
void RF_check_counter_clr(void)
{
	RF_check_timer_enable = 0;
	RF_check_counter = 0;
	RF_check_cnt_limit = 0;
	RF_check_flag = 0;
}
#endif

int debounce_counter_set(UINT32 time_ms)
{
	int ret=0;
	
	if(debounce_timer_enable == 0)
	{
		// log:DEB0
		// Send_Data_To_UART0('D');
		// Send_Data_To_UART0('E');
		// Send_Data_To_UART0('B');
		// Send_Data_To_UART0('0');
		// Send_Data_To_UART0('\r');
		// Send_Data_To_UART0('\n');
		debounce_counter = 0;
		debounce_cnt_limit = (time_ms * 1000) / T0_PERIOD_US;
		debounce_timer_enable = 1;
	}
	else
		ret = -1;
	
	return ret;
}

void debounce_counter_clr(void)
{
	debounce_timer_enable = 0;
	debounce_counter = 0;
	debounce_cnt_limit = 0;
	debounce_flag = 0;
}

int click_protect_counter_set(UINT32 time_ms)
{
	int ret=0;
	
	if(click_protect_timer_enable == 0)
	{
		click_protect_counter = 0;
		click_protect_cnt_limit = (time_ms * 1000) / T0_PERIOD_US;
		click_protect_timer_enable = 1;
	}
	else
		ret = -1;
	
	return ret;
}

int showing_alive_counter_set(UINT32 time_ms)
{
	int ret=0;
	
	if(showing_alive_timer_enable == 0)
	{
		showing_alive_counter = 0;
		showing_alive_cnt_limit = (time_ms * 1000) / T0_PERIOD_US;
		showing_alive_timer_enable = 1;
	}
	else
		ret = -1;
	
	return ret;
}

void showing_alive_counter_clr(void)
{
	showing_alive_timer_enable = 0;
	showing_alive_counter = 0;
	showing_alive_cnt_limit = 0;
	showing_alive_flag = 0;
}

// 롱키 리셋관련
int long_key_counter_set(UINT32 time_ms)
{
	int ret=0;
	
	if(long_key_timer_enable == 0)
	{
		long_key_counter = 0;
		long_key_cnt_limit = (time_ms * 1000) / T0_PERIOD_US;
		long_key_timer_enable = 1;
	}
	else
		ret = -1;
	
	return ret;
}

// 롱키 리셋관련
void long_key_counter_clr(void)
{
	long_key_timer_enable = 0;
	long_key_counter = 0;
	long_key_cnt_limit = 0;
	long_key_flag = 0;
}

void click_protect_counter_clr(void)
{
	click_protect_timer_enable = 0;
	click_protect_counter = 0;
	click_protect_cnt_limit = 0;
	click_protect_flag = 0;
}

void Interrupt_Priorities_Init(void)
{
	//level 3~0 높을수록 우선순위가 높음
	
	//UART lv2
	//Timer0 lv1
	//Pin lv1
	//ADC lv1
	//WDT lv1
	
	//그외 셋팅하지 않을시 lv0
	//lv3 H 1 L1
	//lv2 H 1 L0
	//lv1 H 0 L1
	//lv0 H 0 L0
	
	//UART lv2
	set_PSH;
	clr_PS;
	
	//Timer0 lv1
	clr_PT0H;
	set_PT0;
	
	//Pin lv1
	clr_PPIH;
	set_PPI;
	
	//ADC lv1
	clr_PADCH;
	set_PADC;
	
	//WDT lv1
	clr_PWDTH;
	set_PWDT;

	////Send_Data_To_UART0('I');	
	//IPH / EIPH / EIPH1 /IP / EIP / EIP2
	////Send_UINT8_To_UART0(IPH);
	////Send_UINT8_To_UART0(EIPH);
	////Send_UINT8_To_UART0(EIPH1);
	////Send_UINT8_To_UART0(IP);
	////Send_UINT8_To_UART0(EIP);
	////Send_UINT8_To_UART0(EIP1);
}

void Main_Timer_Init(void)
{
	TIMER0_MODE1_ENABLE;
	clr_T0M;
	
	TH0 = TH0_INIT;
	TL0 = TL0_INIT;
	set_ET0;          // ET0 enable timer 0
	set_EA;
	
	set_TR0;          // timer 0 run
	
	//RF 레지스터 체크 카운터 set
	RF_check_counter_set(5000);
	
	// 롱키 리셋관련
	long_key_counter_set(10000);
}


void BOD_Init(void)
{	
	clr_BODEN;
	clr_BORST;
	clr_BOF;
	clr_BORF;
	
	set_BOV0;
	set_BOV1;
	
	set_BORST;
	set_BODEN;
}

/************************************************************************************************************
*    WDT interrupt sub-routine
************************************************************************************************************/
void WDT_ISR (void)   interrupt 10
{
	clr_WDTF;
	set_WDCLR;
	set_WDTR;
}


void WatchDog_Init(void)
{
	//CONFIG4 set enabled and stop running in Idle/Power-down mode
	set_IAPEN;
    IAPAL = 0x04;
    IAPAH = 0x00;
    IAPFD = 0x50;
    IAPCN = 0xE1;
    set_CFUEN;
    set_IAPGO;                                  //trigger IAP
		while((CHPCON&SET_BIT6)==SET_BIT6);          //check IAPFF (CHPCON.6)
    clr_CFUEN;
    clr_IAPEN;
	
	
	
	//WDT disable
	clr_WDTEN;
	
	//WDT 주기 설정
	set_WPS2;
	set_WPS1;
	set_WPS0;
	
	//WDT flag clear
	clr_WDTF;
	
	//clr WIDPD WDT stops running during Idle or Power-down mode.
	BIT_TMP=EA;EA=0;TA=0xAA;TA=0x55;WDCON&=~SET_BIT4;EA=BIT_TMP;
	
	//WDT interrupt_set
	set_EWDT;
	set_EA;
	
	//Run WDT
	set_WDTR;
}


void main(void)
{
//	UINT8 buf[100] = {0};
	//UINT8 temp=0;
	//int buf_count=0;
	//int test_on=0;
	
	//인터럽트 우선순위 초기화
	Interrupt_Priorities_Init();
	
	//WatchDog 초기화
	WatchDog_Init();
	
	//URAT 초기화
	//InitUART();

	//DEBUGCHRIS
	//UART_Hal_Init76E003(57600);
//UART_Hal_Init76E003(38400);
//UART_Hal_Init76E003(9600);
//UART_Hal_Init76E003(9600);
UART_Hal_Init76E003(57600);
	
	//start UART MSG
	//Send_Data_To_UART0('S');
	//Send_Data_To_UART0('t');
	//Send_Data_To_UART0('a');
	//Send_Data_To_UART0('r');
	//Send_Data_To_UART0('t');
	//Send_Data_To_UART0('\r');
	//Send_Data_To_UART0('\n');
	
	//external osc로 변경
	set_External_OSC();
	
	//UART baud rate다시 계산 (ext osc 기준)
	calc_UART_baud_rate();
	
	//start UART MSG
	//Send_Data_To_UART0('E');
	//Send_Data_To_UART0('X');
	//Send_Data_To_UART0('T');
	//Send_Data_To_UART0(' ');
	//Send_Data_To_UART0('O');
	//Send_Data_To_UART0('S');
	//Send_Data_To_UART0('C');
	//Send_Data_To_UART0('\r');
	//Send_Data_To_UART0('\n');
	
	//Brown-Out detect 초기화
	BOD_Init();
	
	//RF 칩 Pio, 주파수, 레지스터 관련 초기 설정
	m_is_rf_reinit = FALSE;
    rf_init();
	
	//버튼 Key PIO 초기화
	PIO_Init();
	
	set_P12;
	//P17_Input_Mode; // funnylogic P17을 스위치 입력으로 사용하기 위함
	
	//PWM 초기화
	PWM_Init();
	
	//ADC 초기화
	ADC_Init();
	
	//메인타이머 초기화 및 시작
	Main_Timer_Init();

	Send_Data_To_UART0('V');
	Send_Data_To_UART0('1');
	//Send_Data_To_UART0('.');
	//Send_Data_To_UART0('1');
	//Send_Data_To_UART0('\r');
	//Send_Data_To_UART0('\n');

	initBong();
	
	readSaveData();
				
				/*
				Send_Data_To_UART0('r');
				Send_Data_To_UART0('1');
				Send_Data_To_UART0(' ');
				Send_Data_To_UART0((uint8_t)(System.u16CalLowside&0x00ff));
				Send_Data_To_UART0((uint8_t)(System.u16CalLowside>>8));
				Send_Data_To_UART0('\r');
				Send_Data_To_UART0('\n');
    
				Send_Data_To_UART0('r');
				Send_Data_To_UART0('2');
				Send_Data_To_UART0(' ');
				Send_Data_To_UART0((uint8_t)(System.u16CalHighside&0x00ff));
				Send_Data_To_UART0((uint8_t)(System.u16CalHighside>>8));
				Send_Data_To_UART0('\r');
				Send_Data_To_UART0('\n');
				*/

	// zzz
	
	groupInfoCmd.product_group = PRODUCT_TYPE;     // 응원봉
  groupInfoCmd.company       = PRODUCT_COMPANY;  // 회사 : 마루기획
  groupInfoCmd.artist        = PRODUCT_STICK;    // 옹성우
  rainbowWhere = 1;
	
	//main Loop
	while(1)
	{
		//WDT Clear and Run
		set_WDCLR;
		set_WDTR;
		
		if( bongStatus.runActionWithSync == TRUE )	{
			bongStatus.runActionWithSync = FALSE;
			actionHandler();
		}

		//rcv UART data(RF data) parsing
		if( isParsingData == TRUE )
		{
			isParsingData = FALSE;			

			parsingPassByReset = FALSE;
			
			// 중지커맨드가 들어올때는 syncTime을 초기화하여 새로운 커맨드를 받아들일 준비를 한다.
			if( (rcvBuffer[ RF_PKT_CMD_TYPE ] == 0x1) && (rcvBuffer[ RF_PKT_CMD ] == 0x8e) ) {   
				groupInfoCmd.syncTime = 0;
				//DEBUG_MSG_STR("RST!!\r\n");
				// Send_Data_To_UART0('\r');
				// Send_Data_To_UART0('\n');
				//Send_Data_To_UART0('R');
				// Send_Data_To_UART0('!');
				//Send_Data_To_UART0('!');

				ccData.runSpeed = RUN_SPEED_NOT;
				bongStatus.continueRunCalculer = FALSE;

				//return;
				parsingPassByReset = TRUE;
			}
			
			// 연출번호를 저장하는 기능이다.
			if( (rcvBuffer[ RF_PKT_CMD_TYPE ] == 250) || (rcvBuffer[ RF_PKT_CMD_TYPE ] == 251)) {
				if( rcvBuffer[ RF_PKT_CMD_TYPE ] == 251) {
					autoSave = TRUE;
				}
				else {
					autoSave = FALSE;
				}
				
				if( (rcvBuffer[ RF_PKT_SYNC ] == 0x61) || (rcvBuffer[ RF_PKT_SYNC ] == 0x62) || (rcvBuffer[ RF_PKT_SYNC ] == 0x63) || (rcvBuffer[ RF_PKT_SYNC ] == 0x64)) {
					if( groupInfoCmd.syncTime == 0 ) {
						if( rcvBuffer[ RF_PKT_SYNC ] == 0x61 ) {        // 200ms
							groupInfoCmd.syncTime = FIRST_RECEIVED_CMD;
							save_interactive_num(rcvBuffer);
						}
						else if( rcvBuffer[ RF_PKT_SYNC ] == 0x62 ) {   // 100ms
							groupInfoCmd.syncTime = SECOND_RECEIVED_CMD;
							save_interactive_num(rcvBuffer);
						}
						else if( rcvBuffer[ RF_PKT_SYNC ] == 0x63 ) {   // 100ms
							groupInfoCmd.syncTime = THIRD_RECEIVED_CMD;
							save_interactive_num(rcvBuffer);
						}
						else if( rcvBuffer[ RF_PKT_SYNC ] == 0x64 ) {   // 100ms
							groupInfoCmd.syncTime = FOURTH_RECEIVED_CMD;
							save_interactive_num(rcvBuffer);
						}
					
						//save_interactive_num(rcvBuffer);
					}
				}
				//Send_Data_To_UART0('!');
            
				parsingPassByReset = TRUE;
			}
			
			if( parsingPassByReset == FALSE ) {					
				parsingPassByReset = TRUE;
			
				// syncTime에 대한 설정
				if( (rcvBuffer[ RF_PKT_SYNC ] == 0x61) || (rcvBuffer[ RF_PKT_SYNC ] == 0x62) || 
					(rcvBuffer[ RF_PKT_SYNC ] == 0x63) || (rcvBuffer[ RF_PKT_SYNC ] == 0x64) ) {
					if( groupInfoCmd.syncTime == 0 ) {
						parsingPassByReset = FALSE;
						
						if( rcvBuffer[ RF_PKT_SYNC ] == 0x61 ) {        // 80ms
							//DEBUG_MSG_STR("sync1\r\n");
							groupInfoCmd.syncTime = FIRST_RECEIVED_CMD;
							ccData.isRun = TRUE;  	
							parsingPassByReset = FALSE;
							//Send_Data_To_UART0('\r');
							//Send_Data_To_UART0('\n');
							//Send_Data_To_UART0('S');
							//Send_Data_To_UART0('1');
						}
						else if( rcvBuffer[ RF_PKT_SYNC ] == 0x62 ) {   // 60ms
							//DEBUG_MSG_STR("sync2\r\n");
							groupInfoCmd.syncTime = SECOND_RECEIVED_CMD;
							ccData.isRun = TRUE;    
							parsingPassByReset = FALSE;
							//Send_Data_To_UART0('\r');
							//Send_Data_To_UART0('\n');
							//Send_Data_To_UART0('S');
							//Send_Data_To_UART0('2');
						}
						else if( rcvBuffer[ RF_PKT_SYNC ] == 0x63 ) {        // 40ms
							//DEBUG_MSG_STR("sync1\r\n");
							groupInfoCmd.syncTime = THIRD_RECEIVED_CMD;
							ccData.isRun = TRUE;    
							parsingPassByReset = FALSE;
							//Send_Data_To_UART0('\r');
							//Send_Data_To_UART0('\n');
							//Send_Data_To_UART0('S');
							//Send_Data_To_UART0('1');
						}
						else if( rcvBuffer[ RF_PKT_SYNC ] == 0x64 ) {        // 20ms
							//DEBUG_MSG_STR("sync1\r\n");
							groupInfoCmd.syncTime = FOURTH_RECEIVED_CMD;
							ccData.isRun = TRUE;    
							parsingPassByReset = FALSE;
							//Send_Data_To_UART0('\r');
							//Send_Data_To_UART0('\n');
							//Send_Data_To_UART0('S');
							//Send_Data_To_UART0('1');
						}
					}
					else {
						//Send_Data_To_UART0('\r');
						//Send_Data_To_UART0('\n');
						//Send_Data_To_UART0('S');
						//Send_Data_To_UART0('3');
						ccData.isRun = FALSE;  
						parsingPassByReset = TRUE;						
					}
				}
			}
			

						// 제어 대상 제품구분 판단
        //2byte 변수에 패킷데이터를 담는다.
        product_type = 0xff00 & (rcvBuffer[ RF_PKT_PRODUCT_TYPE_1 ]<<8);
        product_type = product_type | (0x00ff & rcvBuffer[ RF_PKT_PRODUCT_TYPE_2 ]);
			
			anotherBong = FALSE;
        
        //2byte 데이터를 분리하여 비교
        //제품군 0x0007(0b111), 회사 0x003F(0b111111), 가수 0x007F(0b1111111) 는 전체 제어를 뜻하기때문에 Run 될 수 있음
        if( (( (product_type>>13) == groupInfoCmd.product_group ) || ( (product_type>>13) == 0x0007 ))&& 
            (( ( (product_type>>7) & 0x003F ) == groupInfoCmd.company ) || ( ( (product_type>>7) & 0x003F ) == 0x003F )) &&
            (( ( product_type & 0x007F ) == groupInfoCmd.artist ) || ( ( product_type & 0x007F ) == 0x007F ))) {   
							
        }
        else
        {
						anotherBong = TRUE;
				//	ccData.isRun = TRUE;
        }
			
			
						
			//Send_Data_To_UART0('z');		
			if( parsingPassByReset == FALSE ) {				
	
				//Send_Data_To_UART0('n');
				if( ccData.isRun == TRUE ) {
					//Send_Data_To_UART0('m');
					//DEBUG_MSG_STR("run cmd\r\n");
					// 현재 색상을 저장하기 전에 이전색상을 저장해둔다.
					ccData.previousRed = ccData.r;
					ccData.previousGreen = ccData.g;
					ccData.previousBlue = ccData.b;
					// 커맨드를 실행하기에 앞서 LED가 표현할 색상을 취함.
					//ccData.r = (uint8)rcvBuffer[PKT_LED_R];                    
					//ccData.g = (uint8)rcvBuffer[PKT_LED_G];
					//ccData.b = (uint8)rcvBuffer[PKT_LED_B];
					
					ccData.Color_1_table_index = (uint8)rcvBuffer[RF_PKT_COLOR_1];       //필요없을수도 있으나 일단 현재의 컬러테이블 인덱스를 저장해놓자
					ccData.Color_2_table_index = (uint8)rcvBuffer[RF_PKT_COLOR_2];       //2컬러 함수의 경우 다른곳에서 이 컬러를 쓰도록 컬러테이블 인덱스를 저장해놓음
					
					setColor(ccData.Color_1_table_index);					
					
					//Send_UINT8_To_UART0 (rcvBuffer[ RF_PKT_SPEED]);
					// 커맨드를 실행하기에 앞서 동작 속도를 결정한다.
					if( (0x61 <= rcvBuffer[ RF_PKT_SPEED]) && (rcvBuffer[ RF_PKT_SPEED] <= 0x74)) {
						ccData.runSpeed = rcvBuffer[ RF_PKT_SPEED] - 0x61;
					}					
					
					//제어 단위를 구분하여 함수 호출
					if( rcvBuffer[ RF_PKT_CMD_TYPE ] == CONTROL_MODE_ALL ) {           // 전체 제어 명령이다. 
						if( anotherBong == FALSE ) {
							processAll_control(rcvBuffer);
						}
					}   
					else if(rcvBuffer[ RF_PKT_CMD_TYPE ] == CONTROL_MODE_VERTICAL) {        // 세로 연출 커맨드
						groupInfoCmd.hallWidth = rcvBuffer[ RF_PKT_ADD1 ];
						groupInfoCmd.hallHeight = rcvBuffer[ RF_PKT_ADD2 ];
						
						// 값 검증용 로그
						//DebugWriteString("s_v1 :");
						//DebugWriteUint8(half);
						//DebugWriteString(",");
						//DebugWriteUint8(groupInfo.vertical);
						//DebugWriteString(",");
						//DebugWriteUint8(groupInfoCmd.hallHeight);
						//DebugWriteString("..\r\n");
						
						// 단, 세로번호가 짝수여야 한다.    
						half = groupInfoCmd.hallHeight / 2;
						if( (groupInfo.vertical >= 1) && (groupInfo.vertical <= half) ) {
							groupInfo.verticalHalf = ( half + 1 ) - groupInfo.vertical; 
						}
						else {
							groupInfo.verticalHalf = groupInfo.vertical - half;
						}
						
						groupInfo.hallHeightHalf = half;
						
						// 값 검증용 로그
						//DebugWriteString("s_v2 :");
						//DebugWriteUint8(half);
						//DebugWriteString(",");
						//DebugWriteUint8(groupInfo.verticalHalf);
						//DebugWriteString("..\r\n");    
						
						if( anotherBong == FALSE ) {
							processInteractive_Vertical_control(rcvBuffer);
						}
					}	
					else if(rcvBuffer[ RF_PKT_CMD_TYPE ] == CONTROL_MODE_HORIZON)             // 가로 연출 커맨드
					{
						groupInfoCmd.hallWidth = rcvBuffer[ RF_PKT_ADD1 ];
						groupInfoCmd.hallHeight = rcvBuffer[ RF_PKT_ADD2 ];

						/*
						DebugWriteString("hall:");
						DebugWriteUint8(groupInfoCmd.hallWidth);
						DebugWriteString(",");
						DebugWriteUint8(groupInfoCmd.hallHeight);
						DebugWriteString("..\r\n");
						*/
						
						// 값 검증용 로그                
						//DebugWriteString("s_v3 :");
						//DebugWriteUint8(half);
						//DebugWriteString(",");
						//DebugWriteUint8(groupInfo.horizontal);
						//DebugWriteString(",");
						//DebugWriteUint8(groupInfoCmd.hallWidth);
						//DebugWriteString("..\r\n");
						
						// 가로 연출중 반반나뉘어서 하는 동작에 필요한 반반의 좌표를 만드는 곳이다. 
						// 가로 6의 크기면 왼쪽 3개와 오른쪽 3개 각각 동일한 좌표로 반반 나뉘어야 한다.
						// 단, 가로번호가 짝수여야 한다.    
						half = groupInfoCmd.hallWidth / 2;
						if( (groupInfo.horizontal >= 1) && (groupInfo.horizontal <= half) ) {
							groupInfo.horizontalHalf = ( half + 1 ) - groupInfo.horizontal; 
						}
						else {
							groupInfo.horizontalHalf = groupInfo.horizontal - half;
						}
						
						groupInfo.hallWidthHalf = half;
						
						// 값 검증용 로그
						//DebugWriteString("s_v4 :");
						//DebugWriteUint8(half);
						//DebugWriteString(",");
						//DebugWriteUint8(groupInfo.horizontalHalf);
						//DebugWriteString("..\r\n");

						if( anotherBong == FALSE ) {
							processInteractive_Horizontal_control(rcvBuffer); 
						}
					}
					else if(rcvBuffer[ RF_PKT_CMD_TYPE ] == CONTROL_MODE_DMX)         // DMX 제어
					{
							if( anotherBong == FALSE ) {
								process_DMX_control(rcvBuffer);    
							}
					}	
					else if(rcvBuffer[ RF_PKT_CMD_TYPE ] == CONTROL_MODE_SHAPE)         //모양 연출 커맨드
					{
							if( anotherBong == FALSE ) {
							processInteractive_Shape_control(rcvBuffer);    
							}
					}					
				}	
				else {
					//Send_Data_To_UART0(',');
				}
			}
		}
		
		//RF check
		if(RF_check_flag == 1)
		{
			// log:y1
			// Send_Data_To_UART0('\r');
			// Send_Data_To_UART0('\n');
			// Send_Data_To_UART0('y');
			// Send_Data_To_UART0('1');
			// Send_Data_To_UART0('\r');
			// Send_Data_To_UART0('\n');

			RF_check_flag = 0;
			////Send_Data_To_UART0('C');
			
			RF_Check_proc();
			
			if(g_bADC_first_check_flag == TRUE)
			{
				// log:y2
				// Send_Data_To_UART0('y');
				// Send_Data_To_UART0('2');
				// Send_Data_To_UART0('\r');
				// Send_Data_To_UART0('\n');
				g_bADC_first_check_flag = FALSE;
				
				three_time_ADC_read();
			}
			else
			{
				// log:y3
				// Send_Data_To_UART0('y');
				// Send_Data_To_UART0('3');
				// Send_Data_To_UART0('\r');
				// Send_Data_To_UART0('\n');

				set_ADCEN;
				set_ADCS;	//adc start
			}
		}
		
		//ADC refresh
		if(g_bADC_complete_flag == TRUE)
		{
			// log:T001
			// Send_Data_To_UART0('T');
			// Send_Data_To_UART0('0');
			// Send_Data_To_UART0('0');
			// Send_Data_To_UART0('1');
			// Send_Data_To_UART0('\r');
			// Send_Data_To_UART0('\n');
			
			g_bADC_complete_flag = FALSE;
			refreshBattValue();
		}
		
		//low Battery LED Off
		if(is5VMode() == BATT_LVL_1)
		{
			g_batt_low_LED_off_counter++;
			
			if(g_batt_low_LED_off_counter >= 100)
			{
				g_batt_low_LED_off_counter = 0;
				PWM_change_duty(0,0,0);
			}
		}
		
		
		//key button pressed (falling)
		if(PIO_button_check_flag == 1)
		{
		//Send_UINT8_To_UART0(rcvBufferIndex>>8);
		//Send_UINT8_To_UART0(rcvBufferIndex & 0x00ff);

		// bongStatus.sleepCount = 0;
		// Send_Data_To_UART0('z');
		// Send_Data_To_UART0('2');
		// Send_Data_To_UART0('0');
		// Send_Data_To_UART0('\r');
		// Send_Data_To_UART0('\n');

		// if( dataCheckCount == 0 ) {
		// 	Send_Data_To_UART0('\r');
		// 	Send_Data_To_UART0('\n');
		// 	Send_Data_To_UART0('0');
		// 	Send_Data_To_UART0('-');
		// }
		// else {
		// 	Send_Data_To_UART0('\r');
		// 	Send_Data_To_UART0('\n');
		// 	Send_Data_To_UART0('z');
		// 	Send_Data_To_UART0('-');
		// }

		// if( dataBodyCheckCount == 0 ) {
		// 	Send_Data_To_UART0('\r');
		// 	Send_Data_To_UART0('\n');
		// 	Send_Data_To_UART0('0');
		// 	Send_Data_To_UART0('-');
		// }
		// else {
		// 	Send_Data_To_UART0('\r');
		// 	Send_Data_To_UART0('\n');
		// 	Send_Data_To_UART0('b');
		// 	Send_Data_To_UART0('\r');
		// 	Send_Data_To_UART0('\n');
		// }


		// Send_Data_To_UART0('/');
		// Send_Data_To_UART0(dataCheckCount);
		// Send_Data_To_UART0('/');
		// Send_Data_To_UART0(dataBodyCheckCount);
		// Send_Data_To_UART0('\r');
		// Send_Data_To_UART0('\n');

			PIO_button_check_flag = 0;
			////Send_Data_To_UART0('B');
			PIO_button_proc();
		}
		
		//key button debounce
		if(debounce_flag == 1)
		{
			debounce_flag = 0;
			////Send_Data_To_UART0('D');
			handleButtonDebounce();
		}
		
		//key re press protect
		if(click_protect_flag == 1)
		{
			click_protect_flag = 0;
			////Send_Data_To_UART0('P');
			handleClickProtect();
		}
		
		
		//showing alive
		if(showing_alive_flag == 1)
		{
			showing_alive_flag = 0;
			//Send_Data_To_UART0('\r');
			//Send_Data_To_UART0('\n');
			//Send_Data_To_UART0('E');
			//Send_Data_To_UART0('N');
			//Send_Data_To_UART0('D');
			//Send_Data_To_UART0('\r');
			//Send_Data_To_UART0('\n');
			showTimeStatus(SHOW_END);
		}
		
		// 롱키 리셋관련
		//Long key event
		if(long_key_flag == 1)
		{
			
			long_key_flag = 0;
			//Soft reset
			set_SWRST;
		}	
	}
}

void initBong(void) {
	bongStatus.runActionWithSync = FALSE;
	bongStatus.runActionContinue = FALSE;
	bongStatus.syncCalculer = FALSE;
	bongStatus.continueRunCalculer = FALSE;
	bongStatus.syncCounter = 0;
	bongStatus.powerCheckPassCount = 0;
	bongStatus.showMode = FALSE;
}

extern void showInteractive(ActionCommand ac) {
	//    DEBUG_MSG_STR("in s\r\n");	
	startAction();
	actionModeChange(ac);
	
	//bongStatus.syncCalculer = FALSE;	
    bongStatus.syncCounter = 0;
    bongStatus.continueRunCalculer = FALSE;
    bongStatus.continueCounter = 0;
    bongStatus.runActionWithSync = FALSE;
	
	bongStatus.syncCalculer = TRUE;	
	
	/*
	// LED EVENT 상태 체크 동작 여부 판단
	if(eventAvailableCMDCheck(ac) == FALSE){
		DEBUG_MSG_STR("eventAvailableCMDCheck return \r\n");
		return;
	}
	
	startAction();
	
	if( groupInfoCmd.syncTime == FIRST_RECEIVED_CMD ) {
		//DEBUG_MSG_STR("in s-3\r\n");
		actionCreateTimer(ACTION_TIME_200);
	}       
	else if( groupInfoCmd.syncTime == SECOND_RECEIVED_CMD ) {
		//DEBUG_MSG_STR("in s-4\r\n");
		actionCreateTimer(ACTION_TIME_100);
	}   
	else {
		DEBUG_MSG_STR("run err\r\n");
	}    
	*/
}


void set_Internal_OSC(void)
{
	/*
	clr_EXTEN1;
	clr_EXTEN0;												//step1: enable extclk clock source run		
	set_HIRCEN;												//step1: enable HIRC clock source run
	while((CKSWT&SET_BIT5)==0);								//step2: check ready
	clr_OSC1;												//step3: switching system clock source if needed
	clr_OSC0;
	while((CKEN&SET_BIT0)==1);								//step4: check system clock switching OK or NG		


	P15_PushPull_Mode;	
	P15 = 0;
	*/
}

void set_External_OSC(void)
{
	// 파워 다운관련 동작 추가
	P15_PushPull_Mode;	
	P15 = 1;	

	//external clock enable
	set_EXTEN1;
	set_EXTEN0;
	
	/*
	while(1) {
		if((CKSWT & 0x08) == 0x08)
			break;
	}
	*/
	while((CKSWT&SET_BIT3)==0);
	
	// 파워 다운관련 동작 추가
	clr_HIRCEN;												//step1: enable HIRC clock source run
	
	//clock switching
	clr_OSC1;
	set_OSC0;
	
	//wait clock switching
	/*
	while(1) {
		if( (CKEN & 0x01) == 0x00)
			break;
	}
	*/
	while((CKEN&SET_BIT0)==1);								//step4: check system clock switching OK or NG	
}

void setColor(uint8 color) {
	  //Send_Data_To_UART0('p');
	
		if( color == 1 ) {            ccData.r = 0x00; ccData.g = 0x00;  ccData.b = 0xff;	}
		else if( color == 2 )  {	  ccData.r = 0x00;	ccData.g = 0x92; 	ccData.b = 0xff;	}
		else if( color == 3 )  {	  ccData.r = 0x00;	ccData.g = 0xff; 	ccData.b = 0xff;	}
		else if( color == 4 )  {	  ccData.r = 0x0a;	ccData.g = 0x2e; 	ccData.b = 0xff;	}
		else if( color == 5 )  {	  ccData.r = 0x0a;	ccData.g = 0x19; 	ccData.b = 0xff;	}
		else if( color == 6 )  {	  ccData.r = 0x00;	ccData.g = 0x50; 	ccData.b = 0xff;	}
		else if( color == 7 )  {	  ccData.r = 0x1e;	ccData.g = 0xc8; 	ccData.b = 0xff;	}
		else if( color == 8 )  {	  ccData.r = 0xff;	ccData.g = 0x00; 	ccData.b = 0x00;	}
		else if( color == 9 )  {	  ccData.r = 0xff;	ccData.g = 0x50; 	ccData.b = 0x00;	}
		else if( color == 10 ) {	ccData.r = 0x2d;	ccData.g = 0xdd; 	ccData.b = 0x00;	}
		else if( color == 11 ) {	ccData.r = 0xff;	ccData.g = 0x00; 	ccData.b = 0x86;	}
		else if( color == 12 ) {	ccData.r = 0xff;	ccData.g = 0x00; 	ccData.b = 0xff;	}
		else if( color == 13 ) {	ccData.r = 0xa7;	ccData.g = 0x00; 	ccData.b = 0xff;	}
		else if( color == 14 ) {	ccData.r = 0x2d;	ccData.g = 0xdd; 	ccData.b = 0x99;	}
		else if( color == 15 ) {	ccData.r = 0xff;	ccData.g = 0xbb; 	ccData.b = 0x55;	}
		else if( color == 16 ) {	ccData.r = 0x0a;	ccData.g = 0xff; 	ccData.b = 0x2e;	}
		else if( color == 17 ) {	ccData.r = 0x00;	ccData.g = 0xff; 	ccData.b = 0x00;	}
		else if( color == 18 ) {	ccData.r = 0x7d;	ccData.g = 0xff; 	ccData.b = 0x00;	}
		else if( color == 19 ) {	ccData.r = 0xff;	ccData.g = 0xff; 	ccData.b = 0xff;	}
		else if( color == 20 ) {	ccData.r = 0xff;	ccData.g = 0xff; 	ccData.b = 0x00;	}
	  else {
			//Send_Data_To_UART0('o');
		}
		
		//Send_Data_To_UART0('i');
}

// 전체 제어 동작 커맨드 처리
void processAll_control(char *buffer) {
		// as 부스나, 생산시 r, g, b, w제어를 할때 잠기면 안되니까. 일단 여기는 막지 말자.
	  //changeRunMode( RUN_MODE_RF );   // 무선을 받으면 일단 버튼동작불가
	/*
	Send_Data_To_UART0('a');
	Send_Data_To_UART0('\:');
	Send_Data_To_UART0(buffer[ RF_PKT_CMD ]);
	Send_Data_To_UART0('\r');
	Send_Data_To_UART0('\n');
	*/
	
    switch( buffer[ RF_PKT_CMD ] ) {               // 전체 끄기
        case ACTION_OFF :				
			/*
				System.u16CalLowside = (read_APROM_BYTE(FLASH_VERTICAL)<<8) + read_APROM_BYTE(FLASH_HORIZONTAL);
			   System.u16CalHighside = (read_APROM_BYTE(FLASH_FLOOR)<<8) + read_APROM_BYTE(FLASH_LINE);
			
			Send_Data_To_UART0('r');
			Send_Data_To_UART0('1');
			Send_Data_To_UART0(' ');
			Send_Data_To_UART0((uint8_t)(System.u16CalLowside&0x00ff));
			Send_Data_To_UART0((uint8_t)(System.u16CalLowside>>8));
			Send_Data_To_UART0('\r');
			Send_Data_To_UART0('\n');

			Send_Data_To_UART0('r');
			Send_Data_To_UART0('2');
			Send_Data_To_UART0(' ');
			Send_Data_To_UART0((uint8_t)(System.u16CalHighside&0x00ff));
			Send_Data_To_UART0((uint8_t)(System.u16CalHighside>>8));
			Send_Data_To_UART0('\r');
			Send_Data_To_UART0('\n');
			*/
								
            showInteractive( CMD_ALL_OFF );
            break;
        case ACTION_ON_SELECT_COLOR :
			/*
			Send_Data_To_UART0('\r');
			Send_Data_To_UART0('\n');
			Send_Data_To_UART0('W');
			Write_Dataflash_Byte(FLASH_VERTICAL,0x37);
			Write_Dataflash_Byte(FLASH_HORIZONTAL,0x38);
			Write_Dataflash_Byte(FLASH_FLOOR,0x31);
			Write_Dataflash_Byte(FLASH_LINE,0x33);	
			Send_Data_To_UART0('\r');
			Send_Data_To_UART0('\n');		
			*/
				
            showInteractive( CMD_ALL_ON_COLOR );   // 선택색 켜기
            break;
		case ACTION_DIMMING_ALL_REPEAT_0_100 :     // 전체 디밍 반복(0 - 100)
            showInteractive( CMD_DIMMING_REPEAT_0_100 );
            break;
        case ACTION_DIMMING_ALL_REPEAT_20_100 :     // 전체 디밍 반복(20 - 100)
            showInteractive( CMD_DIMMING_REPEAT_20_100 );
            break;
        case ACTION_ALL_FLICKER_SELECT_COLOR :     // 전체 점멸(선택색)
            showInteractive( CMD_ALL_USER_COLOR_BLINK );
            break;
		case ACTION_ALL_FLICKER_SAME_COLOR_RANDOM :     // 전체 점멸(전체 동일컬러로 랜덤)
            showInteractive( CMD_ALL_RANDOM_COLOR_BLINK );
            break;
        case ACTION_ALL_FLICKER_EACH_COLOR_RANDOM :     // 전체 점멸(전체 개별 랜덤컬러)
            showInteractive( CMD_ALL_EACH_RANDOM_COLOR_BLINK );
            break;    
		case ACTION_ALL_RAINBOW :     // ��ü ������
						rainbowWhere = 1;
            showInteractive( CMD_BT_RAINBOW_MODE );
            break;
		case ACTION_BTN_RAINBOW :     // ��ü ������
						rainbowWhere = 2;		
            showInteractive( CMD_BT_RAINBOW_MODE );
            break;
		case ACTION_ALL_DIMMING_ON_SELECT_COLOR :     // 전체 디밍으로 LED 켜기
            showInteractive( CMD_DIMMING_LED_ON );
            break;
		case ACTION_ALL_DIMMING_OFF :     // 전체 부드럽게 끄기
            showInteractive( CMD_DIMMING_OFF_CUR_COLOR );
            break;
		case ACTION_ALL_COLOR_SMOOTH_TURN_TO_SELECT_COLOR :     // 전체 부드럽게 컬러 변경
            showInteractive( CMD_DIMMING_COLOR_CHANGE );
            break;
		//case ACTION_ALL_EACH_TIME_RANDOM_SELECT_COLOR :     // 개별 랜덤 시간 점멸(선택색)		
        //    showInteractive( CMD_ALL_USER_COLOR_RANDOM_TIME_BLINK );
        //    break;
		case ACTION_RUNMODE_NORMAL :                                   // 일반모드(BT on, 버튼 enable)
            showTimeStatus(SHOW_END);
            break;
        case ACTION_RUNMODE_SHOW :                                     // 공연모드(BT off, 버튼 disable)
            showTimeStatus(SHOW_ING);
            break;
		case ACTION_FUNC_SHOW_NUMBER_CHECK :                    // 연출번호가 등록되어있는지 체크
						
            setHorColor(1);
            showInteractive( CMD_ALL_ON_COLOR );   // 선택색 켜기
        break;
		case 49 :                    // 그룹2 체크
          setHorColor(2);
         showInteractive( CMD_ALL_ON_COLOR );   // 선택색 켜기
        break;
		case 50 :                    // 그룹3 체크
          setHorColor(3);
         showInteractive( CMD_ALL_ON_COLOR );   // 선택색 켜기
        break;
		case 51 :                    // 그룹4 체크
          setHorColor(4);
         showInteractive( CMD_ALL_ON_COLOR );   // 선택색 켜기
        break;
		case 52 :                    // 그룹5 체크
          setHorColor(5);
         showInteractive( CMD_ALL_ON_COLOR );   // 선택색 켜기
        break;
		case 53 :                    // 그룹6 체크
          setHorColor(6);
         showInteractive( CMD_ALL_ON_COLOR );   // 선택색 켜기
        break;
		case 54 :                    // 그룹7 체크
          setHorColor(7);
         showInteractive( CMD_ALL_ON_COLOR );   // 선택색 켜기
        break;
		case 55 :                    // 그룹8 체크
          setHorColor(8);
         showInteractive( CMD_ALL_ON_COLOR );   // 선택색 켜기
        break;
		case 56 :                    // 그룹9 체크
          setHorColor(9);
         showInteractive( CMD_ALL_ON_COLOR );   // 선택색 켜기
        break;
		case 57 :                    // 그룹10 체크
          setHorColor(10);
         showInteractive( CMD_ALL_ON_COLOR );   // 선택색 켜기
        break;
		case 58 :                    // 그룹11 체크
          setHorColor(11);
         showInteractive( CMD_ALL_ON_COLOR );   // 선택색 켜기
        break;
		case 59 :                    // 그룹12 체크
          setHorColor(12);
         showInteractive( CMD_ALL_ON_COLOR );   // 선택색 켜기
        break;
		case 60 :                    // 그룹13 체크
          setHorColor(13);
         showInteractive( CMD_ALL_ON_COLOR );   // 선택색 켜기
        break;
		case 61 :                    // 그룹14 체크
          setHorColor(14);
         showInteractive( CMD_ALL_ON_COLOR );   // 선택색 켜기
        break;
		case 62 :                    // 그룹15 체크
          setHorColor(15);
         showInteractive( CMD_ALL_ON_COLOR );   // 선택색 켜기
        break;
		case 63 :                    // 그룹16 체크
          setHorColor(16);
         showInteractive( CMD_ALL_ON_COLOR );   // 선택색 켜기
        break;
		case 64 :                    // 그룹17 체크
          setHorColor(17);
         showInteractive( CMD_ALL_ON_COLOR );   // 선택색 켜기
        break;
		case 65 :                    // 그룹18 체크
          setHorColor(18);
         showInteractive( CMD_ALL_ON_COLOR );   // 선택색 켜기
        break;
		case 66 :                    // 그룹19 체크
          setHorColor(19);
         showInteractive( CMD_ALL_ON_COLOR );   // 선택색 켜기
        break;
		case 67 :                    // 그룹20 체크
          setHorColor(20);
         showInteractive( CMD_ALL_ON_COLOR );   // 선택색 켜기
        break;
		case 68 :                    // 그룹21 체크
          setHorColor(21);
         showInteractive( CMD_ALL_ON_COLOR );   // 선택색 켜기
        break;
		case ACTION_FUNC_SHOW_VER_CHECK :                    // 공연버전 체크
			/*
			//Send_Data_To_UART0('1');
			if( groupInfo.showVersion == rcvBuffer[ RF_PKT_ADD1 ] ) {
				//Send_Data_To_UART0('2');
				if( groupInfo.showVersionDay == rcvBuffer[ RF_PKT_ADD2 ] ) {
					//Send_Data_To_UART0('3');
					ccData.r = 0x00;
					ccData.g = 0xff;
					ccData.b = 0x00;
				}
				else {
					ccData.r = 0x00;
					ccData.g = 0x00;
					ccData.b = 0xff;
				}
			}
			else {
				//Send_Data_To_UART0('4');
				ccData.r = 0xff;
				ccData.g = 0x00;
				ccData.b = 0x00;
			}
            showInteractive( CMD_ALL_ON_COLOR );   // 선택색 켜기
			*/
			if( (groupInfo.showVersion == rcvBuffer[ RF_PKT_ADD1 ]) && ( groupInfo.showVersionDay == rcvBuffer[ RF_PKT_ADD2 ] ) ) {
				setHorColor(1);
			}
			else {
				//Send_Data_To_UART0('4');
				ccData.r = 0xff;
				ccData.g = 0xff;
				ccData.b = 0xff;
			}
			
            showInteractive( CMD_ALL_ON_COLOR );   // 선택색 켜기
        break;					
	}		
}

// 구역제어 세로 제어 동작 처리
void processInteractive_Vertical_control(char *buffer) {			
    // 순차  켜기, 순차끄기
    if( buffer[ RF_PKT_CMD ] == (0x00 + 0x01) ) {            // 세로(아래->위) 순차 켜지기
		//Send_Data_To_UART0('1');
        showInteractive( CMD_SHOW_VERTICAL_UP_LED_ON );
    }
    else if( buffer[ RF_PKT_CMD ] == (0x03 + 0x01) ) {       // 세로(아래->위) 순차 꺼지기
		//Send_Data_To_UART0('2');
        showInteractive( CMD_SHOW_VERTICAL_UP_LED_OFF );
    }     
}

void save_interactive_num(char *buffer)
{
//		Send_Data_To_UART0('W');
	//Send_Data_To_UART0('1');
	
	
    // 연출번호를 저장할때 반씩 잘라서 연출할때 필요한 공연장 사이즈를 저장하도록 하자.
    // 이 값을 이용하여 NVM에 저장할때 공연장 사이즈의 반을 자른 값을 추출한다.
    groupInfoCmd.hallWidth = buffer[ RF_PKT_ADD1 ];
    groupInfoCmd.hallHeight = buffer[ RF_PKT_ADD2 ];
                
    if( (P17 == TRUE) || (autoSave == TRUE) )
    {		
				autoSave = FALSE;
		// 커맨드를 실행하기에 앞서 동작 속도를 결정한다.
		if( (0x61 <= buffer[ RF_PKT_SPEED]) && (buffer[ RF_PKT_SPEED] <= 0x74))
		{
			ccData.runSpeed = buffer[ RF_PKT_SPEED] - 0x61;
		}
		else
		{
			ccData.runSpeed = RUN_SPEED_17;
		}
		
		//============================
		//세로 번호 입력
		//============================
		if(buffer[ RF_PKT_PRODUCT_TYPE_1 ] == 255)  //0값을 입력하기 위해서 255값을 받았을때는 0으로 입력하도록함
			groupInfo.vertical = 0;
		else
			groupInfo.vertical = buffer[ RF_PKT_PRODUCT_TYPE_1 ];        
			
				Write_Dataflash_Byte(FLASH_VERTICAL, groupInfo.vertical);
		
		delayForWrite();
		
		//============================
		//가로 번호 입력
		//============================
		if(buffer[ RF_PKT_PRODUCT_TYPE_2 ] == 255)
			groupInfo.horizontal = 0;
		else
			groupInfo.horizontal = buffer[ RF_PKT_PRODUCT_TYPE_2 ];       
				
			Write_Dataflash_Byte(FLASH_HORIZONTAL, groupInfo.horizontal);
				
			//Timer0_Delay1ms_12MHz(3);
		delayForWrite();
		
		//============================
		//층 번호 입력
		//============================
		if(buffer[ RF_PKT_CMD ] == 255)
			groupInfo.floor = 0;
		else
			groupInfo.floor = buffer[ RF_PKT_CMD ];
				
				Write_Dataflash_Byte(FLASH_FLOOR, groupInfo.floor);    

		delayForWrite();
		
		//============================
		//라인 번호 입력
		//============================
		if(buffer[ RF_PKT_COLOR_1 ] == 255)
			groupInfo.line = 0;
		else
			groupInfo.line = buffer[ RF_PKT_COLOR_1 ];
		
			  Write_Dataflash_Byte(FLASH_LINE, groupInfo.line);   
				
			//Timer0_Delay1ms_12MHz(3);
		
		delayForWrite();
		
		//============================
		//모양 번호 입력
		//============================
		if(buffer[ RF_PKT_COLOR_2 ] == 255)
			groupInfo.shape = 0;
		else
			groupInfo.shape = buffer[ RF_PKT_COLOR_2 ];
		
				Write_Dataflash_Byte(FLASH_SHAPE, groupInfo.shape);   
		
		delayForWrite();
		
		//============================
		//그룹 번호 입력
		//============================
		/*
		Send_Data_To_UART0('s');
	Send_Data_To_UART0(':');
	Send_Data_To_UART0(groupInfo.myGroup);
		Send_Data_To_UART0('\r');
	Send_Data_To_UART0('\n');
		*/
		
		// add1, add2는 홀의 가로, 세로 크기로 사용중이므로 add3에 넣자.
		if(buffer[ RF_PKT_ADD3 ] == 255)
			groupInfo.myGroup = 0;
		else
			groupInfo.myGroup = buffer[ RF_PKT_ADD3 ];
		
				Write_Dataflash_Byte(FLASH_MYGROUP, groupInfo.myGroup);   
		
		delayForWrite();
				
				//Timer0_Delay1ms_12MHz(3);
		
		//============================
		//그룹1 번호 입력
		//============================
		if(buffer[ RF_PKT_DIMMER_01_R ] == 255)//if(buffer[ RF_PKT_ADD4 ] == 255)
			groupInfo.logo1 = 0;
		else
			groupInfo.logo1 = buffer[ RF_PKT_DIMMER_01_R ];//groupInfo.logo1 = buffer[ RF_PKT_ADD4 ];
		
		Write_Dataflash_Byte(FLASH_LOGO1, groupInfo.logo1);   
		
		delayForWrite();
		
		//============================
		// 공연버전 입력
		//============================
				
		if(buffer[ RF_PKT_DIMMER_01_G ] == 255)//if(buffer[ RF_PKT_ADD55 ] == 255)
			groupInfo.showVersion = 0;
		else
			groupInfo.showVersion = buffer[ RF_PKT_DIMMER_01_G ];//groupInfo.showVersion = buffer[ RF_PKT_ADD55 ];
		//setNVMShowVersion(groupInfo.showVersion);
		Write_Dataflash_Byte(FLASH_SHOW_VER, groupInfo.showVersion);   
		
		delayForWrite();
			
		
		//============================
		//글자2 번호 입력
		//============================
		if(buffer[ RF_PKT_DIMMER_01_B ] == 255)//if(buffer[ RF_PKT_ADD56 ] == 255)
			groupInfo.logo2 = 0;
		else
			groupInfo.logo2 = buffer[ RF_PKT_DIMMER_01_B ];//groupInfo.logo2 = buffer[ RF_PKT_ADD56 ];
		
				Write_Dataflash_Byte(FLASH_LOGO2, groupInfo.logo2);   
		
		delayForWrite();
		
		//============================
		//글자3 번호 입력
		//============================
		if(buffer[ RF_PKT_DIMMER_01_S ] == 255)//if(buffer[ RF_PKT_ADD57 ] == 255)
			groupInfo.logo3 = 0;
		else
			groupInfo.logo3 = buffer[ RF_PKT_DIMMER_01_S ];//groupInfo.logo3 = buffer[ RF_PKT_ADD57 ];
		
				Write_Dataflash_Byte(FLASH_LOGO3, groupInfo.logo3); 
		
		delayForWrite();
		
		//============================
		//공연버전_날짜 입력
		//============================
		/*
		if(buffer[ RF_PKT_DIMMER_01_D ] == 255)//if(buffer[ RF_PKT_ADD57 ] == 255)
			groupInfo.showVersionDay = 0;
		else
			groupInfo.showVersionDay = buffer[ RF_PKT_DIMMER_01_D ];//groupInfo.logo3 = buffer[ RF_PKT_ADD57 ];
		
		Write_Dataflash_Byte(FLASH_SHOW_VER_DAY, groupInfo.showVersionDay); 
		
		delayForWrite();
		*/
		
		
		//============================
		//그룹2 번호 입력
		//============================
		// add1, add2는 홀의 가로, 세로 크기로 사용중이므로 add3에 넣자.
		if(buffer[ RF_PKT_DIMMER_03_R ] == 255)
			groupInfo.myGroup2 = 0;
		else
			groupInfo.myGroup2 = buffer[ RF_PKT_DIMMER_03_R ];
		
				Write_Dataflash_Byte(FLASH_GROUP2, groupInfo.myGroup2);   
		
		delayForWrite();
		
		//============================
		//그룹3 번호 입력
		//============================
		/*
		Send_Data_To_UART0('f');
	Send_Data_To_UART0(':');
		Send_Data_To_UART0(groupInfo.myGroup3);
	Send_Data_To_UART0('\r');
	Send_Data_To_UART0('\n');
		*/
		// add1, add2는 홀의 가로, 세로 크기로 사용중이므로 add3에 넣자.
		if(buffer[ RF_PKT_DIMMER_03_G ] == 255)
			groupInfo.myGroup3 = 0;
		else
			groupInfo.myGroup3 = buffer[ RF_PKT_DIMMER_03_G ];
		
				Write_Dataflash_Byte(FLASH_GROUP3, groupInfo.myGroup3);   
		
		delayForWrite();
		
		//============================
		//그룹4 번호 입력
		//============================
		// add1, add2는 홀의 가로, 세로 크기로 사용중이므로 add3에 넣자.
		if(buffer[ RF_PKT_DIMMER_03_B ] == 255)
			groupInfo.myGroup4 = 0;
		else
			groupInfo.myGroup4 = buffer[ RF_PKT_DIMMER_03_B ];
		
				Write_Dataflash_Byte(FLASH_GROUP4, groupInfo.myGroup4);   
		
		delayForWrite();
		
		//============================
		//그룹5 번호 입력
		//============================
		// add1, add2는 홀의 가로, 세로 크기로 사용중이므로 add3에 넣자.
		if(buffer[ RF_PKT_DIMMER_03_S ] == 255)
			groupInfo.myGroup5 = 0;
		else
			groupInfo.myGroup5 = buffer[ RF_PKT_DIMMER_03_S ];
		
				Write_Dataflash_Byte(FLASH_GROUP5, groupInfo.myGroup5);   
		
		delayForWrite();
		
		//============================
		//그룹6 번호 입력
		//============================
		// add1, add2는 홀의 가로, 세로 크기로 사용중이므로 add3에 넣자.
		if(buffer[ RF_PKT_DIMMER_04_R ] == 255)
			groupInfo.myGroup6 = 0;
		else
			groupInfo.myGroup6 = buffer[ RF_PKT_DIMMER_04_R ];
		
				Write_Dataflash_Byte(FLASH_GROUP6, groupInfo.myGroup6);   
		
		delayForWrite();
		
		//============================
		//그룹7 번호 입력
		//============================
		// add1, add2는 홀의 가로, 세로 크기로 사용중이므로 add3에 넣자.
		if(buffer[ RF_PKT_DIMMER_04_G ] == 255)
			groupInfo.myGroup7 = 0;
		else
			groupInfo.myGroup7 = buffer[ RF_PKT_DIMMER_04_G ];
		
				Write_Dataflash_Byte(FLASH_GROUP7, groupInfo.myGroup7);   
		
		delayForWrite();
		
		//============================
		//그룹8 번호 입력
		//============================
		// add1, add2는 홀의 가로, 세로 크기로 사용중이므로 add3에 넣자.
		if(buffer[ RF_PKT_DIMMER_04_B ] == 255)
			groupInfo.myGroup8 = 0;
		else
			groupInfo.myGroup8 = buffer[ RF_PKT_DIMMER_04_B ];
		
				Write_Dataflash_Byte(FLASH_GROUP8, groupInfo.myGroup8);   
		
		delayForWrite();
		
		//============================
		//그룹9 번호 입력
		//============================
		// add1, add2는 홀의 가로, 세로 크기로 사용중이므로 add3에 넣자.
		if(buffer[ RF_PKT_DIMMER_04_S ] == 255)
			groupInfo.myGroup9 = 0;
		else
			groupInfo.myGroup9 = buffer[ RF_PKT_DIMMER_04_S ];
		
				Write_Dataflash_Byte(FLASH_GROUP9, groupInfo.myGroup9);   
		
		delayForWrite();
		
		//============================
		//그룹10 번호 입력
		//============================
		// add1, add2는 홀의 가로, 세로 크기로 사용중이므로 add3에 넣자.
		if(buffer[ RF_PKT_DIMMER_05_R ] == 255)
			groupInfo.myGroup10 = 0;
		else
			groupInfo.myGroup10 = buffer[ RF_PKT_DIMMER_05_R ];
		
				Write_Dataflash_Byte(FLASH_GROUP10, groupInfo.myGroup10);   
		
		delayForWrite();
		
		//============================
		//그룹11 번호 입력
		//============================
		// add1, add2는 홀의 가로, 세로 크기로 사용중이므로 add3에 넣자.
		if(buffer[ RF_PKT_DIMMER_05_G ] == 255)
			groupInfo.myGroup11 = 0;
		else
			groupInfo.myGroup11 = buffer[ RF_PKT_DIMMER_05_G ];
		
				Write_Dataflash_Byte(FLASH_GROUP11, groupInfo.myGroup11);   
		
		delayForWrite();
		
		//============================
		//그룹12 번호 입력
		//============================
		// add1, add2는 홀의 가로, 세로 크기로 사용중이므로 add3에 넣자.
		if(buffer[ RF_PKT_DIMMER_05_B ] == 255)
			groupInfo.myGroup12 = 0;
		else
			groupInfo.myGroup12 = buffer[ RF_PKT_DIMMER_05_B ];
		
				Write_Dataflash_Byte(FLASH_GROUP12, groupInfo.myGroup12);   
		
		delayForWrite();
		
		//============================
		//그룹13 번호 입력
		//============================
		// add1, add2는 홀의 가로, 세로 크기로 사용중이므로 add3에 넣자.
		if(buffer[ RF_PKT_DIMMER_05_S ] == 255)
			groupInfo.myGroup13 = 0;
		else
			groupInfo.myGroup13 = buffer[ RF_PKT_DIMMER_05_S ];
		
				Write_Dataflash_Byte(FLASH_GROUP13, groupInfo.myGroup13);   
		
		delayForWrite();
		
		//============================
		//그룹14 번호 입력
		//============================
		// add1, add2는 홀의 가로, 세로 크기로 사용중이므로 add3에 넣자.
		if(buffer[ RF_PKT_DIMMER_06_R ] == 255)
			groupInfo.myGroup14 = 0;
		else
			groupInfo.myGroup14 = buffer[ RF_PKT_DIMMER_06_R ];
		
				Write_Dataflash_Byte(FLASH_GROUP14, groupInfo.myGroup14);   
		
		delayForWrite();
		
		//============================
		//그룹15 번호 입력
		//============================
		// add1, add2는 홀의 가로, 세로 크기로 사용중이므로 add3에 넣자.
		if(buffer[ RF_PKT_DIMMER_06_G ] == 255)
			groupInfo.myGroup15 = 0;
		else
			groupInfo.myGroup15 = buffer[ RF_PKT_DIMMER_06_G ];
		
				Write_Dataflash_Byte(FLASH_GROUP15, groupInfo.myGroup15);   
		
		delayForWrite();
		
		//============================
		//그룹16 번호 입력
		//============================
		// add1, add2는 홀의 가로, 세로 크기로 사용중이므로 add3에 넣자.
		if(buffer[ RF_PKT_DIMMER_06_B ] == 255)
			groupInfo.myGroup16 = 0;
		else
			groupInfo.myGroup16 = buffer[ RF_PKT_DIMMER_06_B ];
		
				Write_Dataflash_Byte(FLASH_GROUP16, groupInfo.myGroup16);   
		
		delayForWrite();
		
		//============================
		//그룹17 번호 입력
		//============================
		// add1, add2는 홀의 가로, 세로 크기로 사용중이므로 add3에 넣자.
		if(buffer[ RF_PKT_DIMMER_06_S ] == 255)
			groupInfo.myGroup17 = 0;
		else
			groupInfo.myGroup17 = buffer[ RF_PKT_DIMMER_06_S ];
		
				Write_Dataflash_Byte(FLASH_GROUP17, groupInfo.myGroup17);   
		
		delayForWrite();
		
		//============================
		//그룹18 번호 입력
		//============================
		// add1, add2는 홀의 가로, 세로 크기로 사용중이므로 add3에 넣자.
		if(buffer[ RF_PKT_DIMMER_07_R ] == 255)
			groupInfo.myGroup18 = 0;
		else
			groupInfo.myGroup18 = buffer[ RF_PKT_DIMMER_07_R ];
		
				Write_Dataflash_Byte(FLASH_GROUP18, groupInfo.myGroup18);   
		
		delayForWrite();
		
		//============================
		//그룹19 번호 입력
		//============================
		// add1, add2는 홀의 가로, 세로 크기로 사용중이므로 add3에 넣자.
		if(buffer[ RF_PKT_DIMMER_07_G ] == 255)
			groupInfo.myGroup19 = 0;
		else
			groupInfo.myGroup19 = buffer[ RF_PKT_DIMMER_07_G ];
		
				Write_Dataflash_Byte(FLASH_GROUP19, groupInfo.myGroup19);   
		
		delayForWrite();
		
		//============================
		//그룹20 번호 입력
		//============================
		// add1, add2는 홀의 가로, 세로 크기로 사용중이므로 add3에 넣자.
		if(buffer[ RF_PKT_DIMMER_07_B ] == 255)
			groupInfo.myGroup20 = 0;
		else
			groupInfo.myGroup20 = buffer[ RF_PKT_DIMMER_07_B ];
		
				Write_Dataflash_Byte(FLASH_GROUP20, groupInfo.myGroup20);   
		
		delayForWrite();
		
		//============================
		//그룹21 번호 입력
		//============================
		// add1, add2는 홀의 가로, 세로 크기로 사용중이므로 add3에 넣자.
		if(buffer[ RF_PKT_DIMMER_07_S ] == 255)
			groupInfo.myGroup21 = 0;
		else
			groupInfo.myGroup21 = buffer[ RF_PKT_DIMMER_07_S ];
		
				Write_Dataflash_Byte(FLASH_GROUP21, groupInfo.myGroup21);   
		
		delayForWrite();
		
		//============================
		//dummy 입력
		//============================       
		Write_Dataflash_Byte(FLASH_DUMMY, 99); 		
		delayForWrite();
	
		//Send_Data_To_UART0('W');
//		    Send_Data_To_UART0('2');
		
		//showInteractive( CMD_INTERACTIVE_VERTICAL_NUM );
		//switch_counter = 3;
		
		PWM_change_duty(0, 255, 0);
    }
}

void readSaveData(void) {
	System.u16CalLowside = (read_APROM_BYTE(FLASH_VERTICAL)<<8) + read_APROM_BYTE(FLASH_HORIZONTAL);
	System.u16CalHighside = (read_APROM_BYTE(FLASH_FLOOR)<<8) + read_APROM_BYTE(FLASH_LINE);
	
	groupInfo.vertical = (uint8_t)(System.u16CalLowside>>8);
	groupInfo.horizontal = (uint8_t)(System.u16CalLowside&0x00ff);
	groupInfo.floor = (uint8_t)(System.u16CalHighside>>8);
	groupInfo.line = (uint8_t)(System.u16CalHighside&0x00ff);
	
	
	System.u16CalLowside = (read_APROM_BYTE(FLASH_SHAPE)<<8) + read_APROM_BYTE(FLASH_MYGROUP);
	System.u16CalHighside = (read_APROM_BYTE(FLASH_LOGO1)<<8) + read_APROM_BYTE(FLASH_LOGO2);

	groupInfo.shape = (uint8_t)(System.u16CalLowside>>8);
	groupInfo.myGroup = (uint8_t)(System.u16CalLowside&0x00ff);
	groupInfo.logo1 = (uint8_t)(System.u16CalHighside>>8);
	groupInfo.logo2 = (uint8_t)(System.u16CalHighside&0x00ff);
	
	System.u16CalLowside = (read_APROM_BYTE(FLASH_LOGO3)<<8) + read_APROM_BYTE(FLASH_SHOW_VER);	
	System.u16CalHighside = (read_APROM_BYTE(FLASH_SHOW_VER_DAY)<<8) + read_APROM_BYTE(FLASH_GROUP2);
	groupInfo.logo3 = (uint8_t)(System.u16CalLowside>>8);	
	groupInfo.showVersion = (uint8_t)(System.u16CalLowside&0x00ff);
	groupInfo.showVersionDay = (uint8_t)(System.u16CalHighside>>8);
	groupInfo.myGroup2 = (uint8_t)(System.u16CalHighside&0x00ff);
	
	//////////
	System.u16CalLowside = (read_APROM_BYTE(FLASH_GROUP3)<<8) + read_APROM_BYTE(FLASH_GROUP4);
	System.u16CalHighside = (read_APROM_BYTE(FLASH_GROUP5)<<8) + read_APROM_BYTE(FLASH_GROUP6);
	
	groupInfo.myGroup3 = (uint8_t)(System.u16CalLowside>>8);
	groupInfo.myGroup4 = (uint8_t)(System.u16CalLowside&0x00ff);
	groupInfo.myGroup5 = (uint8_t)(System.u16CalHighside>>8);
	groupInfo.myGroup6 = (uint8_t)(System.u16CalHighside&0x00ff);
	
	//////////
	System.u16CalLowside = (read_APROM_BYTE(FLASH_GROUP7)<<8) + read_APROM_BYTE(FLASH_GROUP8);
	System.u16CalHighside = (read_APROM_BYTE(FLASH_GROUP9)<<8) + read_APROM_BYTE(FLASH_GROUP10);
	
	groupInfo.myGroup7 = (uint8_t)(System.u16CalLowside>>8);
	groupInfo.myGroup8 = (uint8_t)(System.u16CalLowside&0x00ff);
	groupInfo.myGroup9 = (uint8_t)(System.u16CalHighside>>8);                      
	groupInfo.myGroup10 = (uint8_t)(System.u16CalHighside&0x00ff);
	
	//////////
	System.u16CalLowside = (read_APROM_BYTE(FLASH_GROUP11)<<8) + read_APROM_BYTE(FLASH_GROUP12);
	System.u16CalHighside = (read_APROM_BYTE(FLASH_GROUP13)<<8) + read_APROM_BYTE(FLASH_GROUP14);
	
	groupInfo.myGroup11 = (uint8_t)(System.u16CalLowside>>8);
	groupInfo.myGroup12 = (uint8_t)(System.u16CalLowside&0x00ff);
	groupInfo.myGroup13 = (uint8_t)(System.u16CalHighside>>8);
	groupInfo.myGroup14 = (uint8_t)(System.u16CalHighside&0x00ff);
	
	//////////
	System.u16CalLowside = (read_APROM_BYTE(FLASH_GROUP15)<<8) + read_APROM_BYTE(FLASH_GROUP16);
	System.u16CalHighside = (read_APROM_BYTE(FLASH_GROUP17)<<8) + read_APROM_BYTE(FLASH_GROUP18);
	
	groupInfo.myGroup15 = (uint8_t)(System.u16CalLowside>>8);
	groupInfo.myGroup16 = (uint8_t)(System.u16CalLowside&0x00ff);
	groupInfo.myGroup17 = (uint8_t)(System.u16CalHighside>>8);
	groupInfo.myGroup18 = (uint8_t)(System.u16CalHighside&0x00ff);
	
	//////////
	System.u16CalLowside = (read_APROM_BYTE(FLASH_GROUP19)<<8) + read_APROM_BYTE(FLASH_GROUP20);
	System.u16CalHighside = (read_APROM_BYTE(FLASH_GROUP21)<<8) + read_APROM_BYTE(FLASH_GROUP21);
	
	groupInfo.myGroup19 = (uint8_t)(System.u16CalLowside>>8);
	groupInfo.myGroup20 = (uint8_t)(System.u16CalLowside&0x00ff);
	groupInfo.myGroup21 = (uint8_t)(System.u16CalHighside>>8);
	groupInfo.dummy = (uint8_t)(System.u16CalHighside&0x00ff);
	/*
	DebugWriteString("load:");
	DebugWriteUint8(groupInfoCmd.myGroup2);
	DebugWriteString(","); 
	DebugWriteUint8(groupInfoCmd.myGroup21);
	DebugWriteString("..\r\n");  
	*/
	/*
	Send_Data_To_UART0('r');
	Send_Data_To_UART0('1');
	Send_Data_To_UART0(' ');
	Send_Data_To_UART0((uint8_t)(groupInfo.myGroup));
	Send_Data_To_UART0((uint8_t)(groupInfo.myGroup21));
	Send_Data_To_UART0('\r');
	Send_Data_To_UART0('\n');
	*/
	Send_Data_To_UART0('r');
	Send_Data_To_UART0(':');
	Send_Data_To_UART0(groupInfo.myGroup);
	Send_Data_To_UART0('/');
	Send_Data_To_UART0(groupInfo.myGroup3);
	Send_Data_To_UART0('\r');
	Send_Data_To_UART0('\n');
}

// DMX 제어 처리
void process_DMX_control(char *buffer) {
	//Send_Data_To_UART0('d');
	
	groupSelect = groupInfo.myGroup;
	
	if( buffer[ RF_PKT_DMX_COLOR1 ] == 5 ) {            // 그룹1 제어 코드      
	}
	else if( buffer[ RF_PKT_DMX_COLOR1 ] == 10 ) {        // 그룹2 제어 코드       
			groupSelect = groupInfo.myGroup2;
	} 
	else if( buffer[ RF_PKT_DMX_COLOR1 ] == 15 ) {        // 그룹3 제어 코드 			
			groupSelect = groupInfo.myGroup3;
	}
	else if( buffer[ RF_PKT_DMX_COLOR1 ] == 20 ) {        // 그룹4 제어 코드 			
			groupSelect = groupInfo.myGroup4;
	}
	else if( buffer[ RF_PKT_DMX_COLOR1 ] == 25 ) {        // 그룹5 제어 코드 			
			groupSelect = groupInfo.myGroup5;
	}
	else if( buffer[ RF_PKT_DMX_COLOR1 ] == 30 ) {        // 그룹6 제어 코드 			
			groupSelect = groupInfo.myGroup6;
	}
	else if( buffer[ RF_PKT_DMX_COLOR1 ] == 35 ) {        // 그룹7 제어 코드 			
			groupSelect = groupInfo.myGroup7;
	}
	else if( buffer[ RF_PKT_DMX_COLOR1 ] == 40 ) {        // 그룹8 제어 코드 			
			groupSelect = groupInfo.myGroup8;
	}
	else if( buffer[ RF_PKT_DMX_COLOR1 ] == 45 ) {        // 그룹9 제어 코드 			
			groupSelect = groupInfo.myGroup9;
	}
	else if( buffer[ RF_PKT_DMX_COLOR1 ] == 50 ) {        // 그룹10 제어 코드 			
			groupSelect = groupInfo.myGroup10;
	}
	else if( buffer[ RF_PKT_DMX_COLOR1 ] == 55 ) {        // 그룹11 제어 코드 			
			groupSelect = groupInfo.myGroup11;
	}
	else if( buffer[ RF_PKT_DMX_COLOR1 ] == 60 ) {        // 그룹12 제어 코드 			
			groupSelect = groupInfo.myGroup12;
	}
	else if( buffer[ RF_PKT_DMX_COLOR1 ] == 65 ) {        // 그룹13 제어 코드 			
			groupSelect = groupInfo.myGroup13; 
	}
	else if( buffer[ RF_PKT_DMX_COLOR1 ] == 70 ) {        // 그룹14 제어 코드 			
			groupSelect = groupInfo.myGroup14;
	}
	else if( buffer[ RF_PKT_DMX_COLOR1 ] == 75 ) {        // 그룹15 제어 코드 			
			groupSelect = groupInfo.myGroup15;
	}
	else if( buffer[ RF_PKT_DMX_COLOR1 ] == 80 ) {        // 그룹16 제어 코드 			
			groupSelect = groupInfo.myGroup16;
	}
	else if( buffer[ RF_PKT_DMX_COLOR1 ] == 85 ) {        // 그룹17 제어 코드 			
			groupSelect = groupInfo.myGroup17;
	}
	else if( buffer[ RF_PKT_DMX_COLOR1 ] == 90 ) {        // 그룹18 제어 코드 			
			groupSelect = groupInfo.myGroup18;
	}
	else if( buffer[ RF_PKT_DMX_COLOR1 ] == 95 ) {        // 그룹19 제어 코드 			
			groupSelect = groupInfo.myGroup19;
	}
	else if( buffer[ RF_PKT_DMX_COLOR1 ] == 100 ) {        // 그룹20 제어 코드 			
			groupSelect = groupInfo.myGroup20;
	}
	else if( buffer[ RF_PKT_DMX_COLOR1 ] == 105 ) {        // 그룹21 제어 코드 			
			groupSelect = groupInfo.myGroup21;
	}
	
	
	if( (groupSelect >= 1) && (groupSelect <= 21) ) {
		//Send_Data_To_UART0('i');
		//Send_Data_To_UART0('n');
		groupInfoCmd.dmxR = buffer[ (groupSelect * 5 ) + 7 ];
		groupInfoCmd.dmxG = buffer[ ( groupSelect * 5 ) + 7 + 1 ];
		groupInfoCmd.dmxB = buffer[ ( groupSelect * 5 ) + 7 + 2 ];
		groupInfoCmd.dmxS = buffer[ ( groupSelect * 5 ) + 7 + 3 ];
		groupInfoCmd.dmxD = buffer[ ( groupSelect * 5 ) + 7 + 4 ];
		
		
		//DebugWriteString("dmx:");
		//DebugWriteUint8(groupInfoCmd.dmxR);
		//DebugWriteString(",");    
		//DebugWriteUint8(groupInfoCmd.dmxG);
		//DebugWriteString(",");    
		//DebugWriteUint8(groupInfoCmd.dmxB);
		//DebugWriteString(",");    
		//DebugWriteUint8(groupInfoCmd.dmxD);
		//DebugWriteString(",");    
		//DebugWriteUint8(groupInfoCmd.dmxS);
		//DebugWriteString("..\r\n");  

		if( groupInfoCmd.dmxS == 1 ) {
			ccData.runSpeed = RUN_SPEED_1;
			groupInfoCmd.dmxS = 0;
	//        DebugWriteString("s_no\r\n");
			//Send_Data_To_UART0('1');
		}
		else if( (groupInfoCmd.dmxS > 1) && (groupInfoCmd.dmxS < 12) ) {
			ccData.runSpeed = RUN_SPEED_1;
	  //      DebugWriteString("s_1\r\n");
			//Send_Data_To_UART0('2');
		}
		else if( (groupInfoCmd.dmxS >= 12) && (groupInfoCmd.dmxS < 24) ) {
			ccData.runSpeed = RUN_SPEED_2;
	  //      DebugWriteString("s_2\r\n");
			//Send_Data_To_UART0('3');
		}
		else if( (groupInfoCmd.dmxS >= 24) && (groupInfoCmd.dmxS < 36) ) {
			ccData.runSpeed = RUN_SPEED_3;
	   //     DebugWriteString("s_3\r\n");
			//Send_Data_To_UART0('4');
		}
		else if( (groupInfoCmd.dmxS >= 36) && (groupInfoCmd.dmxS < 48) ) {
			ccData.runSpeed = RUN_SPEED_4;
	   //     DebugWriteString("s_4\r\n");
			//Send_Data_To_UART0('5');
		}
		else if( (groupInfoCmd.dmxS >= 48) && (groupInfoCmd.dmxS < 60) ) {
			ccData.runSpeed = RUN_SPEED_5;
	  //      DebugWriteString("s_5\r\n");
			//Send_Data_To_UART0('6');
		}
		else if( (groupInfoCmd.dmxS >= 60) && (groupInfoCmd.dmxS < 72) ) {
			ccData.runSpeed = RUN_SPEED_6;
	  //      DebugWriteString("s_6\r\n");
		}
		else if( (groupInfoCmd.dmxS >= 72) && (groupInfoCmd.dmxS < 84) ) {
			ccData.runSpeed = RUN_SPEED_7;
	  //      DebugWriteString("s_7\r\n");
			//Send_Data_To_UART0('7');
		}
		else if( (groupInfoCmd.dmxS >= 84) && (groupInfoCmd.dmxS < 96) ) {
			ccData.runSpeed = RUN_SPEED_8;
	  //      DebugWriteString("s_8\r\n");
			//Send_Data_To_UART0('8');
		}
		else if( (groupInfoCmd.dmxS >= 96) && (groupInfoCmd.dmxS < 108) ) {
			ccData.runSpeed = RUN_SPEED_9;
	  //      DebugWriteString("s_9\r\n");
			//Send_Data_To_UART0('9');
		}
		else if( (groupInfoCmd.dmxS >= 108) && (groupInfoCmd.dmxS < 120) ) {
			ccData.runSpeed = RUN_SPEED_10;
			//DebugWriteString("s_10\r\n");
			//Send_Data_To_UART0('a');
		}
		else if( (groupInfoCmd.dmxS >= 120) && (groupInfoCmd.dmxS < 132) ) {
			ccData.runSpeed = RUN_SPEED_11;
	//        DebugWriteString("s_11\r\n");
			//Send_Data_To_UART0('b');
		}
		else if( (groupInfoCmd.dmxS >= 132) && (groupInfoCmd.dmxS < 144) ) {
			ccData.runSpeed = RUN_SPEED_12;
	  //      DebugWriteString("s_12\r\n");
			//Send_Data_To_UART0('c');
		}
		else if( (groupInfoCmd.dmxS >= 144) && (groupInfoCmd.dmxS < 156) ) {
			ccData.runSpeed = RUN_SPEED_13;
		//    DebugWriteString("s_13\r\n");
			//Send_Data_To_UART0('d');
		}
		else if( (groupInfoCmd.dmxS >= 156) && (groupInfoCmd.dmxS < 168) ) {
			ccData.runSpeed = RUN_SPEED_14;
	//        DebugWriteString("s_14\r\n");
			//Send_Data_To_UART0('e');
		}
		else if( (groupInfoCmd.dmxS >= 168) && (groupInfoCmd.dmxS < 180) ) {
			ccData.runSpeed = RUN_SPEED_15;
	//        DebugWriteString("s_15\r\n");
			//Send_Data_To_UART0('f');
		}
		else if( (groupInfoCmd.dmxS >= 180) && (groupInfoCmd.dmxS < 192) ) {
			ccData.runSpeed = RUN_SPEED_16;
	//        DebugWriteString("s_16\r\n");
			//Send_Data_To_UART0('g');
		}
		else if( (groupInfoCmd.dmxS >= 192) && (groupInfoCmd.dmxS < 204) ) {
			ccData.runSpeed = RUN_SPEED_17;
	//        DebugWriteString("s_17\r\n");
			//Send_Data_To_UART0('h');
		}
		else if( (groupInfoCmd.dmxS >= 204) && (groupInfoCmd.dmxS < 216) ) {
			ccData.runSpeed = RUN_SPEED_18;
	//        DebugWriteString("s_18\r\n");
			//Send_Data_To_UART0('i');
		}
		else if( (groupInfoCmd.dmxS >= 216) && (groupInfoCmd.dmxS < 228) ) {
			ccData.runSpeed = RUN_SPEED_19;
	//        DebugWriteString("s_19\r\n");
			//Send_Data_To_UART0('j');
		}
		else if( (groupInfoCmd.dmxS >= 228) && (groupInfoCmd.dmxS <= 255) ) {
			ccData.runSpeed = RUN_SPEED_20;
	//        DebugWriteString("s_20\r\n");
			//Send_Data_To_UART0('k');
		}
		
		showInteractive( CMD_DMX_CONTROL_STAY );
	}
}

void delayForWrite(void) {
	for( iLoopA = 0; iLoopA < 170; iLoopA++ ) {
		for( iLoopB = 0; iLoopB < 250; iLoopB++ ) {
		}
	}
}

// 구역제어 모양 동작 처리
void processInteractive_Shape_control(char *buffer) {
}

// 구역제어 가로 제어 동작 처리
void processInteractive_Horizontal_control(char *buffer) {    
    // 좌우 제어
	if( buffer[ RF_PKT_CMD ] == (0x18 + 0x01) ) {       // 가로(좌->우) 무지개 반복
        showInteractive( CMD_INTERACTIVE_HORIZONTAL_UP_RAINBOW_6STEP );
    }
}