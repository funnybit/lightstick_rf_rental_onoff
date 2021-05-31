#ifndef __COLOR_CONTROL_H__
#define __COLOR_CONTROL_H__

//todo ���ʿ�?
//todo IOTLightControlDeviceSetColor
//todo ��ġ�̵�?
//todo �Լ� �ʿ�

#define MAX_RAINBOW_MODE_COLOR      7
#define MAX_DIMMING_REPEAT_COLOR    2
#define MAX_USER_COLOR              28
#define MAX_BTN_WHITE_BLINK_COLOR   2

/* LED DIMMING */
#define BRIGHT_MAX_VALUE                    10	//todo ���ʿ�?

// ������ ����
typedef struct {
	// �������� ǥ���� ������ ���´�.
    uint8 r;
    uint8 g;
    uint8 b;
    uint8 bright;
    // �������� �ٷ����� ǥ���� ������ ���´�.
    uint8 previousRed;
    uint8 previousGreen;
    uint8 previousBlue;
    uint8 previousBright;
	
	  uint8 Color_1_table_index;    //���� ����1 ���̺� �ε���
    uint8 Color_2_table_index;    //���� ����1 ���̺� �ε���

    bool isRun;     // �������� �ش� Ŀ�ǵ忡 �����ؾ��ϴ����� ���� ����. true�� ����, false�� ����
    uint8 runSpeed; // LED ���� �ӵ�
    uint8 g_led_event_state;    //led�� ����������� ������ ����(Ư����嵿�������� ���)
    uint8 g_restore_led_event_state;    //led�̺�Ʈ ���� �����Ǿ�� �ϴ� ���¸� ����(ex �׶��ǵ����� ���ڼ��Ž� �ٽ��׶��Ǹ��)
} BONG_INFO_T; //todo ���ʿ�?

typedef enum {
    DIMMING_REPEAT,
    DIMMING_RISE,
    DIMMING_RAINBOW,
    DIMMING_THERAPY,
    DIMMING_CANDLE,
    DIMMING_SELECT_COLOR_OFF,
    DIMMING_SELECT_COLOR_ON,
    DIMMING_REPEAT_TWO_COLOR
} DIMMING_PATTERN;

typedef enum {
    TYPE_TWO_COLOR_CHANGE,
    TYPE_ALL_USER_COLOR_BLINK,
    TYPE_ALL_EACH_RANDOM_COLOR_BLINK,
    TYPE_ALL_RANDOM_COLOR_BLINK,
	  TYPE_ALL_USER_COLOR_RANDOM_TIME_BLINK,
    //TYPE_BT_CONNECT,                  // BT connect�� �˸�
    //TYPE_BT_CALL_NOTI,                // BT ��ȭ ���Ž� �˸�
    //TYPE_BT_SMS_NOTI,                 // BT ���� ���Ž� �˸�
    //TYPE_BT_PARTY_MODE,               // BT ��Ƽ ���
    //TYPE_BT_THERAPY_MODE,             // BT �׶��� ���
    //TYPE_BT_CANDLE_MODE,              // BT ĵ�� ���
    TYPE_BT_RAINBOW_MODE,             // BT ������ ���
    TYPE_BTN_WHITE_BLINK_SLOW_MODE,   // ��ư ȭ��Ʈ ����ŷ slow ���
    TYPE_BTN_WHITE_BLINK_FAST_MODE,   // ��ư ȭ��Ʈ ����ŷ fast ���
    //TYPE_BTN_RESET_MODE,               // ��ư ���� ���
    //TYPE_FLOOR1_TWO_COLOR_CHANGE,       //1�� �׷� 2�� �ݺ�
    //TYPE_FLOOR2_TWO_COLOR_CHANGE,       //2�� �׷� 2�� �ݺ�
} BLINK_TYPE;

// color dimming���� ����
typedef struct {

    uint8 patten;
    
    bool increaseOrderBase;
    uint16 baseCount;
    uint16 baseCountOrignal;
    uint8 colorSequenc;
    uint16 frameCount;
    uint16 randomCount;
    uint8 repeat_Count;         //�ݺ� Ƚ�� ī����
    uint8 repeat_CountOrignal;  //�ݺ� Ƚ�� ���� �� 0: ���ѹݺ�
    
    uint8 startColorR;
    uint8 startColorG;
    uint8 startColorB;
    
    uint8 endColorR;
    uint8 endColorG;
    uint8 endColorB;
    
    bool increaseOrderR;
    bool increaseOrderG;
    bool increaseOrderB;
    
    uint8 currentR;
    uint8 currentG;
    uint8 currentB;
    uint8 currentBright;
    
    uint8 gapR;
    uint8 gapG;
    uint8 gapB;
    
    uint8 gapR_Temp;
    uint8 gapG_Temp;
    uint8 gapB_Temp;
    
    uint8 randomGapR;
    uint8 randomGapG;
    uint8 randomGapB;
} COLOR_DIMMING_DATA_T;

// blink ���� ����
typedef struct {
    uint8 type; //led�÷��� Ÿ��
    uint16 onCount; //led�� on �Ǵ� ī��Ʈ
    uint16 onCountOrignal; //onCount �ִ�ġ
    uint16 offCount; //led�� off �Ǵ� ī��Ʈ
    uint16 offCountOrignal; //offCount �ִ�ġ
    
    uint8 colorPatternCount; //�÷����� ������
    uint8 colorPatternCountMax; //���ϰ���
    uint8 colorPatternTemp; //�ӽð�
    
    uint8 currentR;
    uint8 currentG;
    uint8 currentB;
    uint8 currentBright;
    
    bool ledState;
    bool colorChange;
} BLINK_DATA_T;

extern BONG_INFO_T ccData;
extern SceneWorkMode sceneMode;

extern void startAction(void);
extern void stopAction(void);


extern void dimmingColorChangeCal(uint16 frameCount, uint8 mode);
extern void setActionBtRainbowMode(uint16 frameCount, uint8 colorSequenc);

extern void setActionLedDimmingRepeat(uint8 persentMin, uint8 persentMax, uint16 frameCount, uint8 colorSequenc, uint8 repeat_CountOrignal);
extern void setActionDimmingColorChange(uint16 frameCount, uint8 mode);
extern void setActionDimmingLedOn(uint16 frameCount, uint8 mode);
extern void setActionLedDimmingRepeatOnOff(uint16 frameCount, uint8 flag_onoff, uint8 repeat_CountOrignal);
//void setActionLedDimmingRepeatOnOff(uint16 frameCount, uint8 flag_onoff, uint8 repeat_CountOrignal);
extern void setActionLedDimmingRepeatTwoColor(uint16 frameCount, uint8 colorSequenc, uint8 repeat_CountOrignal);
//void setActionLedDimmingRepeatOnOff(uint16 frameCount, uint8 flag_onoff, uint8 repeat_CountOrignal);

extern void setActionAllUserColorBlink(uint16 frameCount);
extern void setActionAllEachRandomColorBlink(uint16 frameCount);
extern void setActionAllRandomColorBlink(uint16 frameCount);
extern void setActionBtnWhiteBlinkFastMode(uint16 frameCount);
extern void setActionBtnWhiteBlinkSlowMode(uint16 frameCount);
extern void actionBlink(void);
extern void actionLedOff(void);                 // LED off (��ü)
extern void actionLedOnWithSelectedColor(void); // ���û� �ѱ�(��ü)
//extern void setActionAllUserColorRandomTimeBlink(uint16 frameCount);  // ����� ������ ���� �ð����� �ݺ� ������


void setActionInteractive_Rainbow(uint16 frameCount,uint8 m_nInteractive_frame_counter_init, uint8 m_nmaxPlayShowNum,uint8 m_nFrame_index, bool m_bPattern_Repeat_flag);

extern void setActionLedDimmingRepeatOnOff_CurColor(uint16 frameCount, uint8 flag_onoff, uint8 repeat_CountOrignal);
uint8 action_Increase_On_1_color(void);
uint8 action_Increase_Off_1_color(void);
uint8 actionLedOnByDMX(void);

void setHorColor(uint8 g);

uint8 action_Increase_On_Rainbow(void);

void setActionInteractive_1_color(uint16 frameCount,uint8 m_nInteractive_frame_counter_init, uint8 m_nInteractive_frame_counter_max,uint8 m_nFrame_index);
void setActionShowByDMX(void);

#endif