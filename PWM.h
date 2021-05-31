#ifndef __PWM_H__
#define __PWM_H__

#define BRIGHT_5V_OFFSET_PERCENT    7

/* 2016.09.27 HW팀 요청으로 white에 대한 calibration은 하지 않습니다. */
#define WHITE_LED_CALIBRATION_VALUE         0xD8//0xA0 //85% 밝기 확정


extern UINT8 g_last_LED_R;
extern UINT8 g_last_LED_G;
extern UINT8 g_last_LED_B;
extern UINT8 g_last_LED_W;

extern uint8 g_nCur_r;
extern uint8 g_nCur_g;
extern uint8 g_nCur_b;

extern void PWM_Init(void);
//extern void PWM_change_duty(UINT8 R_duty, UINT8 G_duty, UINT8 B_duty, UINT8 Y_duty);
extern void PWM_change_duty(UINT8 R_duty, UINT8 G_duty, UINT8 B_duty);

#endif