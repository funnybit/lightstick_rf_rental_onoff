#ifndef __ADC_H__
#define __ADC_H__

// typedef enum {
//     BATT_LVL_1,  // ~ 2.8v (low 배터리, 꺼버리자)
//     BATT_LVL_2,  // 2.9v ~ 4.5V (배터리 정상동작)          
//     BATT_LVL_3   // 4.5v 이상 (크래들 장착)
// } BATT_LVL_T;

typedef enum {
    BATT_LVL_1, // < 3.3 (low 배터리, 꺼버리자)
    BATT_LVL_2, // normal
    BATT_LVL_3  // 완충
} BATT_LVL_T;

extern bool g_bADC_first_check_flag;

extern bool g_bADC_complete_flag;
extern void ADC_Init(void);
extern void refreshBattValue(void);
extern BATT_LVL_T is5VMode(void);
extern void three_time_ADC_read(void);

#endif