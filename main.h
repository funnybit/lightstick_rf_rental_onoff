#ifndef __MAIN_H__
#define __MAIN_H__

#include "cheerbong_info.h"

/*
    공연관련
*/
typedef enum {
    SHOW_BT_OFF,
    SHOW_ING,
    SHOW_END,
} SHOW_STATUS;

extern UINT8 switch_counter;

extern UINT8 click_protect_timer_enable;
extern int debounce_counter_set(UINT32 time_ms);
extern void debounce_counter_clr(void);
extern int click_protect_counter_set(UINT32 time_ms);
extern void click_protect_counter_clr(void);

extern void showTimeStatus(uint8 show);

extern UINT16 RF_check_counter;
extern void showInteractive(ActionCommand ac);

//extern UINT8 rcvDataStage;
#endif