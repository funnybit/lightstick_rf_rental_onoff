#include "Common.h"
#include "N76E003.h"
#include "Function_define.h"
#include "SFR_Macro.h"

#include "PWM.h"
#include "ADC.h"

UINT8 g_last_LED_R=0;
UINT8 g_last_LED_G=0;
UINT8 g_last_LED_B=0;
UINT8 g_last_LED_W=0;

//현재 입력된 RGB 값을 이용가능하도록 저장
uint8 g_nCur_r = 0;
uint8 g_nCur_g = 0;
uint8 g_nCur_b = 0;

uint8 white = 0;

	UINT32 total_period=0;
	UINT16 R_On_duty_period=0;
	UINT16 G_On_duty_period=0;
	UINT16 B_On_duty_period=0;
	UINT16 Y_On_duty_period=0;


void PWM_Init(void)
{
	// PWM2, PWM3, PWM4, PWM5
	P00_Quasi_Mode;
    P01_Quasi_Mode;
    P03_Quasi_Mode;
    P05_Quasi_Mode;
 
    PWM3_P00_OUTPUT_ENABLE;	//YELLOW
    PWM4_P01_OUTPUT_ENABLE;	//BLUE
    PWM5_P03_OUTPUT_ENABLE;	//GREEN
    PWM2_P05_OUTPUT_ENABLE;	//RED
	
	PWM_IMDEPENDENT_MODE;
	PWM_CLOCK_DIV_8;
	
	PWMPH = 0x07;
	PWMPL = 0xCF;
		
/**********************************************************************
	PWM frequency = Fpwm/((PWMPH,PWMPL) + 1) <Fpwm = Fsys/PWM_CLOCK_DIV> 
								= (16MHz/8)/(0x7CF + 1)
								= 1KHz (1ms)
***********************************************************************/
	
	PWM2H = 0;						
	PWM2L = 0;
	PWM3H = 0;					
	PWM3L = 0;


	set_SFRPAGE;						//PWM4 and PWM5 duty seting is in SFP page 1
	PWM4H = 0;						
	PWM4L = 0;
	PWM5H = 0;						
	PWM5L = 0;
	clr_SFRPAGE;	
	
	set_LOAD;
	set_PWMRUN;
}


//void PWM_change_duty(UINT8 R_duty, UINT8 G_duty, UINT8 B_duty, UINT8 Y_duty)
void PWM_change_duty(UINT8 R_duty, UINT8 G_duty, UINT8 B_duty)
{
	 total_period=0;
	 R_On_duty_period=0;
	 G_On_duty_period=0;
	 B_On_duty_period=0;
	 Y_On_duty_period=0;
	
	
	/*
    uint8 white = 0;
        
	//white, 배터리 밝기 적용
    if(R_duty == 0xff && G_duty == 0xff && B_duty == 0xff){ // white case 
        //DEBUG_MSG_STR("white case \r\n");
        R_duty = 0;
        G_duty = 0;
        B_duty = 0;
        white = WHITE_LED_CALIBRATION_VALUE;
        
        //if(is5VMode() == BATT_LVL_3){ // 5v일 경우 70% 밝기로 동작
        //    white = (white * BRIGHT_5V_OFFSET_PERCENT) / 10;
        //}
    } //else {    // color case
        //if(is5VMode() == BATT_LVL_3){ // 5v일 경우 70% 밝기로 동작
        //    R_duty = (R_duty * BRIGHT_5V_OFFSET_PERCENT) / 10;
        //    G_duty = (G_duty * BRIGHT_5V_OFFSET_PERCENT) / 10;
        //    B_duty = (B_duty * BRIGHT_5V_OFFSET_PERCENT) / 10;
        //}
    //}
		*/
		
		white = 0;
		
		//white, 배터리 밝기 적용
    if(R_duty == 0xff && G_duty == 0xff && B_duty == 0xff){ // white case 
        //DEBUG_MSG_STR("white case \r\n");
        R_duty = 0;
        G_duty = 0;
        B_duty = 0;
        white = WHITE_LED_CALIBRATION_VALUE;
        
        //if(is5VMode() == BATT_LVL_3){ // 5v일 경우 70% 밝기로 동작
        //    white = (white * BRIGHT_5V_OFFSET_PERCENT) / 10;
        //}
    } //else {    // color case
        //if(is5VMode() == BATT_LVL_3){ // 5v일 경우 70% 밝기로 동작
        //    R_duty = (R_duty * BRIGHT_5V_OFFSET_PERCENT) / 10;
        //    G_duty = (G_duty * BRIGHT_5V_OFFSET_PERCENT) / 10;
        //    B_duty = (B_duty * BRIGHT_5V_OFFSET_PERCENT) / 10;
        //}
    //}
		
	
	g_last_LED_R = R_duty;
	g_last_LED_G = G_duty;
	g_last_LED_B = B_duty;
	g_last_LED_W = white;
		
		//현재 입력된 RGB 값을 이용가능하도록 저장
    g_nCur_r = R_duty;
    g_nCur_g = G_duty;
    g_nCur_b = B_duty;
	
	total_period = (0xff00 & (PWMPH << 8)) | (0x00ff && PWMPL);
	
	R_On_duty_period = 0x0000ffff & ((total_period * R_duty) / 255);
	G_On_duty_period = 0x0000ffff & ((total_period * G_duty) / 255);
	B_On_duty_period = 0x0000ffff & ((total_period * B_duty) / 255);
	Y_On_duty_period = 0x0000ffff & ((total_period * white) / 255);

	clr_LOAD;
	
	PWM2H = 0x00ff & (R_On_duty_period>>8);						
	PWM2L = 0x00ff & R_On_duty_period;

	PWM3H = 0x00ff & (Y_On_duty_period>>8);						
	PWM3L = 0x00ff & Y_On_duty_period;


	set_SFRPAGE;						//PWM4 and PWM5 duty seting is in SFP page 1
	PWM4H = 0x00ff & (B_On_duty_period>>8);						
	PWM4L = 0x00ff & B_On_duty_period;
	PWM5H = 0x00ff & (G_On_duty_period>>8);						
	PWM5L = 0x00ff & G_On_duty_period;
	clr_SFRPAGE;
	
	set_LOAD;
}