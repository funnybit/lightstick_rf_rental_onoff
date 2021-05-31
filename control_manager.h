#ifndef __CONTROL_MANAGER_H__
#define __CONTROL_MANAGER_H__

#define TIME_600_MS 600 // 1ms �������� 600ms�� 
#define TIME_400_MS 400 // 1ms �������� 400ms�� 
#define TIME_200_MS 200 // 1ms �������� 200ms�� 
#define TIME_100_MS 100 // 1ms �������� 100ms��

#define TIME_80_MS 80 // 1ms �������� 80ms�� 
#define TIME_60_MS 60 // 1ms �������� 60ms��
#define TIME_40_MS 40 // 1ms �������� 40ms��
#define TIME_20_MS 20 // 1ms �������� 20ms��

#define RUN_SPEED_1_1_S 1000 // 1ms �������� 40ms ��
#define RUN_SPEED_40_MS 40 // 1ms �������� 40ms ��
#define RUN_SPEED_39_MS 39
#define RUN_SPEED_37_MS 37
#define RUN_SPEED_35_MS 35
#define RUN_SPEED_33_MS 33
#define RUN_SPEED_31_MS 31
#define RUN_SPEED_29_MS 29
#define RUN_SPEED_27_MS 27
#define RUN_SPEED_25_MS 25
#define RUN_SPEED_23_MS 23
#define RUN_SPEED_21_MS 21
#define RUN_SPEED_20_MS 20 // 1ms �������� 20ms ��
#define RUN_SPEED_19_MS 19
#define RUN_SPEED_17_MS 17
#define RUN_SPEED_15_MS 15 // 1ms �������� 15ms ��
#define RUN_SPEED_13_MS 13
#define RUN_SPEED_11_MS 11
#define RUN_SPEED_10_MS 10 // 1ms �������� 10ms ��
#define RUN_SPEED_9_MS 9
#define RUN_SPEED_7_MS 7
#define RUN_SPEED_5_MS  5  // 1ms �������� 5ms ��
#define RUN_SPEED_3_MS 3

#define FIRST_RECEIVED_CMD          1
#define SECOND_RECEIVED_CMD         2
#define THIRD_RECEIVED_CMD          3
#define FOURTH_RECEIVED_CMD         4

typedef enum {
	//RUN_MODE_BT,
    RUN_MODE_NOMAL, 
    RUN_MODE_RF
} RUN_MODE_T;

extern RUN_MODE_T g_run_mode;
extern void changeRunMode(RUN_MODE_T mode);
extern void actionModeChange(ActionCommand ac);

#endif
