#ifndef __CHEERBONG_INFO_H__
#define __CHEERBONG_INFO_H__

#include "Common.h"

typedef struct {
    bool runActionWithSync;
    bool runActionContinue;
    bool syncCalculer;
    bool continueRunCalculer;
    UINT16 syncCounter;
    UINT16 continueCounter;
    UINT8 powerCheckPassCount;
    bool showMode;
} BONG_STATUS_T;

extern BONG_STATUS_T bongStatus;

typedef enum {    
    MODE_OFF,                         // 전체 끄기
    MODE_SELECTED_COLOR,              // 선택색 켜기
    MODE_DIMMING_REPEAT_0_100,        // 디밍 (0 ~ 100까지 반복) MODE_DIMMING_REPEAT_0_100, MODE_DIMMING_REPEAT_20_100 동일하게 사용 CMD의 경우는 따로사용
    MODE_DIMMING_REPEAT_20_100,       // 디밍 (20 ~ 100까지 반복)
    MODE_DIMMING_REPEAT_OFF,          // 선택색 부드럽게 OFF 반복
    MODE_DIMMING_REPEAT_ON,           // 선택색 부드럽게 ON 반복
    MODE_DIMMING_OFF_CUR_COLOR,       // 현재 컬러 부드럽게 OFF
    MODE_DIMMING_REPEAT_SELECT_TWO_COLOR, // 선택 2컬러 부드럽게 변환 반복
    MODE_DIMMING_REPEAT_TWO_COLOR,    // 2컬러 부드럽게 변환 반복 (빨-녹)
    MODE_DIMMING_REPEAT_TWO_COLOR_COUNT,  // 2컬러 부드럽게 변환 반복 횟수 지정
    MODE_DIMMING_COLOR_CHANGE,        // 디밍으로 색상변경(A색에서 B색으로 변경)
    MODE_DIMMING_LED_ON,              // 디밍으로 LED 켜기(선택색)
    MODE_ALL_RANDOM_COLOR_BLINK,      // 전체 점멸(전체 동일컬러로 랜덤)
    MODE_ALL_USER_COLOR_BLINK,        // 전체 점멸(선택색)
    MODE_ALL_EACH_RANDOM_COLOR_BLINK, // 전체 점멸(전체 개별 랜덤컬러)
//    MODE_ALL_USER_COLOR_RANDOM_TIME_BLINK, // 전체 점멸(선택색 랜덤시간)
    MODE_TWO_COLOR_CHANGE,            // 전체 2컬러 반복(고정색)
    MODE_SELECT_TWO_COLOR_CHANGE,    // 전체 2컬러 반복(선택색)

    MODE_BT_CONNECT,                  // BT connect시 알림
    MODE_BT_CALL_NOTI,                // BT 전화 수신시 알림
    MODE_BT_SMS_NOTI,                 // BT 문자 수신시 알림
    MODE_BT_PARTY_MODE,               // BT 파티 모드
    MODE_BT_THERAPY_MODE,             // BT 테라피 모드
    MODE_BT_CANDLE_MODE,              // BT 캔들 모드
    MODE_BT_RAINBOW_MODE,             // BT 무지개 모드
    MODE_BT_PAIRING_COLOR,            // BT 페어링 컬러 표현 상태
    
    MODE_BTN_WHITE_BLINK_SLOW_MODE,   // 버튼 화이트 블링킹 slow
    MODE_BTN_WHITE_BLINK_FAST_MODE,   // 버튼 화이트 블링킹 slow
    MODE_BTN_RESET_MODE,              // 버튼 리셋  
    
    MODE_INTERACTIVE_VERTICAL_UP_LED_ON_7STEP,         // 세로(아래->위) 순차 켜지기
    MODE_INTERACTIVE_VERTICAL_UP_LED_OFF_7STEP,        // 세로(아래->위) 순차 꺼지기
    MODE_INTERACTIVE_VERTICAL_DOWN_LED_ON_7STEP,       // 세로(위->아래) 순차 켜지기
    MODE_INTERACTIVE_VERTICAL_DOWN_LED_OFF_7STEP,      // 세로(위->아래) 순차 꺼지기
    
    MODE_INTERACTIVE_HORIZONTAL_UP_LED_ON_7STEP,       // 가로(아래->위) 순차 켜지기
    MODE_INTERACTIVE_HORIZONTAL_UP_LED_OFF_7STEP,      // 가로(아래->위) 순차 꺼지기
    MODE_INTERACTIVE_HORIZONTAL_DOWN_LED_ON_7STEP,     // 가로(위->아래) 순차 켜지기
    MODE_INTERACTIVE_HORIZONTAL_DOWN_LED_OFF_7STEP,    // 가로(위->아래) 순차 꺼지기
    
    MODE_SHOW_VERTICAL_DOWN_UP_DOWN_LED_ON_OFF,        // 세로(아래->위->아래) 순차 켜지고 꺼지기 1회
    MODE_SHOW_HORIZONTAL_DOWN_UP_DOWN_LED_ON_OFF,      // 가로(아래->위->아래) 순차 켜지고 꺼지기 1회
    MODE_SHOW_VERTICAL_DOWN_UP_DOWN_LED_ON_OFF_REPEAT,        // 세로(아래->위->아래) 순차 켜지고 꺼지기 반복
    MODE_SHOW_HORIZONTAL_DOWN_UP_DOWN_LED_ON_OFF_REPEAT,      // 가로(아래->위->아래) 순차 켜지고 꺼지기 반복 
    
    MODE_SHOW_VERTICAL_RAINBOW_TYPE1,                  // 세로 전체 무지개 패턴 한번에 보이기
    MODE_SHOW_HORIZONTAL_RAINBOW_TYPE1,                // 가로 전체 무지개 패턴 한번에 보이기
    
    MODE_INTERACTIVE_HORIZONTAL_OUT_LED_ON_3STEP,      // 무대 반으로 갈라서 센터에서 밖으로 켜기(순차, 가로)
    MODE_INTERACTIVE_HORIZONTAL_OUT_LED_OFF_3STEP,     // 무대 반으로 갈라서 센터에서 밖으로 끄기(순차, 가로)
    MODE_INTERACTIVE_HORIZONTAL_IN_LED_ON_3STEP,       // 무대 반으로 갈라서 밖에서 센터로 켜기(순차, 가로)
    MODE_INTERACTIVE_HORIZONTAL_IN_LED_OFF_3STEP,      // 무대 반으로 갈라서 밖에서 센터로 끄기(순차, 가로)
        
    MODE_INTERACTIVE_VERTICAL_OUT_LED_ON_3STEP,        // 무대 반으로 갈라서 센터에서 밖으로 켜기(순차, 세로)
    MODE_INTERACTIVE_VERTICAL_OUT_LED_OFF_3STEP,       // 무대 반으로 갈라서 센터에서 밖으로 끄기(순차, 세로)
    MODE_INTERACTIVE_VERTICAL_IN_LED_ON_3STEP,         // 무대 반으로 갈라서 밖에서 센터로 켜기(순차, 세로)
    MODE_INTERACTIVE_VERTICAL_IN_LED_OFF_3STEP,        // 무대 반으로 갈라서 밖에서 센터로 끄기(순차, 세로)
     
    MODE_SHOW_H_CENTER_TO_OUT_GRA_2BAR_REPEAT,         // 가로기준 그라데이션2바 센터에서 밖으로 이동 반복
    MODE_SHOW_SPEED_FLASH,                             // 전체 번쩍이기
    MODE_SHOW_H_LEFT_TO_RIGHT_2BAR_REPEAT,             // 가로기준 그라데이션2바 왼쪽에서 오른쪽으로 이동 반복
    MODE_SHOW_H_LEFT_TO_RIGHT_2BAR_RAINBOW_REPEAT,     // 가로기준 그라데이션2바 왼쪽에서 오른쪽으로 이동, 무지개 반복
	MODE_SHOW_H_CENTER_TO_OUT_GRA_2BAR_RAINBOW_REPEAT, // 가로기준 그라데이션2바 센터에서 밖으로 이동, 무지개 반복
    MODE_SHOW_H_LEFT_TO_RIGHT_DRAW_RAINBOW_REPEAT,     // 가로기준 왼쪽에서 오른쪽으로 무지개 색깔 칠하면서 이동 반복
    MODE_SHOW_LR_ROTATION_BLINK,                       // 가로기준, 왼쪽 오른쪽 번갈아 색상 보이기
    
    MODE_INTERACTIVE_VERTICAL_UP_RAINBOW_6STEP,         // 세로(아래->위) 무지개 반복 - 6단계
    MODE_INTERACTIVE_VERTICAL_UP_RAINBOW_ONCE_6STEP,         // 세로(아래->위) 무지개 1회 - 6단계
    MODE_INTERACTIVE_VERTICAL_DOWN_RAINBOW_6STEP,         // 세로(위->아래) 무지개 반복 - 6단계
    MODE_INTERACTIVE_VERTICAL_DOWN_RAINBOW_ONCE_6STEP,         // 세로(위->아래) 무지개 1회 - 6단계
    
    MODE_INTERACTIVE_HORIZONTAL_UP_RAINBOW_6STEP,         // 가로(아래->위) 무지개 반복 - 6단계
    MODE_INTERACTIVE_HORIZONTAL_UP_RAINBOW_ONCE_6STEP,         // 가로(아래->위) 무지개 1회 - 6단계
    MODE_INTERACTIVE_HORIZONTAL_DOWN_RAINBOW_6STEP,         // 가로(위->아래) 무지개 반복 - 6단계
    MODE_INTERACTIVE_HORIZONTAL_DOWN_RAINBOW_ONCE_6STEP,         // 가로(위->아래) 무지개 1회 - 6단계
    
    MODE_INTERACTIVE_HORIZONTAL_UP_3SPOT_6STEP,         // 가로(아래->위) 3점 흐르기 - 6단계
    MODE_INTERACTIVE_HORIZONTAL_UP_3SPOT_ONCE_6STEP,         // 가로(아래->위) 3점 흐르기 1회- 6단계
    
    MODE_INTERACTIVE_HORIZONTAL_DOWN_3SPOT_6STEP,         // 가로(위->아래) 3점 흐르기 - 6단계
    MODE_INTERACTIVE_HORIZONTAL_DOWN_3SPOT_ONCE_6STEP,         // 가로(위->아래) 3점 흐르기 1회- 6단계
    
    MODE_INTERACTIVE_HORIZONTAL_UP_RAINBOW_3SPOT_6STEP,         // 가로(아래->위) 3점 무지개 흐르기 - 6단계
    MODE_INTERACTIVE_HORIZONTAL_UP_RAINBOW_3SPOT_ONCE_6STEP,         // 가로(아래->위) 3점 무지개 흐르기 1회- 6단계
    
    MODE_INTERACTIVE_HORIZONTAL_DOWN_RAINBOW_3SPOT_6STEP,         // 가로(위->아래) 3점 무지개 흐르기 - 6단계
    MODE_INTERACTIVE_HORIZONTAL_DOWN_RAINBOW_3SPOT_ONCE_6STEP,         // 가로(위->아래) 3점 무지개 흐르기 1회- 6단계
    
    MODE_INTERACTIVE_LINE_UP_LED_ON , // 라인 선택색 (아래->위) 순차 켜기
    MODE_INTERACTIVE_SHAPE_UP_LED_ON , // 모양 선택색 (아래->위) 순차 켜기
    MODE_INTERACTIVE_SHAPE_UP_LED_OFF , // 모양 선택색 (아래->위) 순차 끄기
    
    MODE_SHOW_SHAPE_LOG_SHOW,            // 글자보이기
    MODE_SHOW_SHAPE_LOGO_SHOW_BACK_OFF,  // 글자보이기(배경은 오프)
    MODE_SHOW_SHAPE_LOGO_3,              // 글자3 보이기
    
    MODE_DMX_CONTROL_STAY,
    MODE_DMX_CONTROL_RUN,
    
    MODE_INTERACTIVE_VERTICAL_NUM,            // 세로 연출 번호
    MODE_INTERACTIVE_HORIZONTAL_NUM,            // 가로 연출 번호
    MODE_INTERACTIVE_FLOOR_NUM,            // 층 연출 번호
    MODE_INTERACTIVE_LINE_NUM,            // 라인 연출 번호
    MODE_INTERACTIVE_SHAPE_NUM,            // 모양 연출 번호
    MODE_CHANGE_FREQUENCY_1,
    MODE_CHANGE_FREQUENCY_2,
    MODE_CHANGE_FREQUENCY_3,
    MODE_CHANGE_FREQUENCY_4,
    MODE_CHANGE_FREQUENCY_5,
    MODE_CHANGE_FREQUENCY_6,
    MODE_BTN_RAINBOW
} SceneWorkMode;

typedef enum {
    CMD_ALL_OFF,                     // 전체 끄기
    CMD_ALL_ON_COLOR,                // 선택색 켜기
    CMD_DIMMING_REPEAT_0_100,        // 디밍 (0 ~ 100까지 반복)
    CMD_DIMMING_REPEAT_20_100,       // 디밍 (20 ~ 100까지 반복) 
    CMD_DIMMING_REPEAT_OFF,          // 선택색 부드럽게 OFF 반복
    CMD_DIMMING_REPEAT_ON,           // 선택색 부드럽게 ON 반복
    CMD_DIMMING_OFF_CUR_COLOR,       // 현재컬러 부드럽게 OFF
    CMD_DIMMING_REPEAT_SELECT_TWO_COLOR,    // 선택 2컬러 부드럽게 변환 반복 (선택 2색)
    CMD_DIMMING_REPEAT_TWO_COLOR,    // 2컬러 부드럽게 변환 반복 (빨-녹)
    CMD_DIMMING_REPEAT_TWO_COLOR_COUNT,     // 2컬러 부드럽게 변환 반복 횟수 지정
    CMD_DIMMING_COLOR_CHANGE,        // 디밍으로 색상변경(A색에서 B색으로 변경)
    CMD_DIMMING_LED_ON,              // 디밍으로 LED 켜기(선택색) 
    CMD_ALL_RANDOM_COLOR_BLINK,      // 전체 점멸(전체 동일컬러로 랜덤)
    CMD_ALL_USER_COLOR_BLINK,        // 전체 점멸(선택색)
    CMD_ALL_EACH_RANDOM_COLOR_BLINK, // 전체 점멸(전체 개별 랜덤컬러)
    //CMD_ALL_USER_COLOR_RANDOM_TIME_BLINK,        // 전체 점멸(선택색 랜덤시간)
    CMD_TWO_COLOR_CHANGE,            // 전체 2컬러 반복(고정색)
    CMD_SELECT_TWO_COLOR_CHANGE,            // 전체 2컬러 반복(선택 2색)
    
    CMD_BT_CONNECT,                  // BT connect시 알림
    CMD_BT_CALL_NOTI,                // BT 전화 수신시 알림
    CMD_BT_SMS_NOTI,                 // BT 문자 수신시 알림
    CMD_BT_PARTY_MODE,               // BT 파티 모드
    CMD_BT_THERAPY_MODE,             // BT 테라피 모드
    CMD_BT_CANDLE_MODE,              // BT 캔들 모드
    CMD_BT_RAINBOW_MODE,             // BT 무지개 모드
    CMD_BT_PAIRING_COLOR,            // BT 페어링 컬러 표현 상태
    
    CMD_BTN_WHITE_BLINK_FAST_MODE,   // 버튼 화이트 블링킹 fast
    CMD_BTN_WHITE_BLINK_SLOW_MODE,   // 버튼 화이트 블링킹 slow
    CMD_BTN_RESET_MODE,   // reset    
    
    CMD_SHOW_VERTICAL_UP_LED_ON,                // 세로(아래->위) 순차 켜지기
    CMD_SHOW_VERTICAL_UP_LED_OFF,               // 세로(아래->위) 순차 꺼지기 - 7단계
    CMD_SHOW_VERTICAL_DOWN_LED_ON,              // 세로(위->아래) 순차 켜지기 - 7단계
    CMD_SHOW_VERTICAL_DOWN_LED_OFF,             // 세로(위->아래) 순차 꺼지기 - 7단계    
    
    CMD_SHOW_HORIZONTAL_UP_LED_ON,           // 가로(좌->우) 순차 켜지기
    CMD_SHOW_HORIZONTAL_UP_LED_OFF,          // 가로(좌->우) 순차 꺼지기
    CMD_SHOW_HORIZONTAL_DOWN_LED_ON,         // 가로(우->좌) 순차 켜지기
    CMD_SHOW_HORIZONTAL_DOWN_LED_OFF,        // 가로(우->좌) 순차 꺼지기
    
    CMD_SHOW_VERTICAL_DOWN_UP_DOWN_LED_ON_OFF,    // 세로(아래->위->아래) 순차 켜지고 꺼지기 1회
    CMD_SHOW_HORIZONTAL_DOWN_UP_DOWN_LED_ON_OFF,  // 가로(아래->위->아래) 순차 켜지고 꺼지기 1회  
    CMD_SHOW_VERTICAL_DOWN_UP_DOWN_LED_ON_OFF_REPEAT,    // 세로(아래->위->아래) 순차 켜지고 꺼지기 반복
    CMD_SHOW_HORIZONTAL_DOWN_UP_DOWN_LED_ON_OFF_REPEAT,  // 가로(아래->위->아래) 순차 켜지고 꺼지기 반복
    
    CMD_SHOW_VERTICAL_RAINBOW_TYPE1,              // 세로 전체 무지개 패턴 한번에 보이기
    CMD_SHOW_HORIZONTAL_RAINBOW_TYPE1,            // 가로 전체 무지개 패턴 한번에 보이기
    
    CMD_INTERACTIVE_HORIZONTAL_OUT_LED_ON_3STEP,         // 무대 반으로 갈라서 센터에서 밖으로 켜기(순차, 가로)
    CMD_INTERACTIVE_HORIZONTAL_OUT_LED_OFF_3STEP,         // 무대 반으로 갈라서 센터에서 밖으로 끄기(순차, 가로)
    CMD_INTERACTIVE_HORIZONTAL_IN_LED_ON_3STEP,         // 무대 반으로 갈라서 밖에서 센터로 켜기(순차, 가로)
    CMD_INTERACTIVE_HORIZONTAL_IN_LED_OFF_3STEP,         // 무대 반으로 갈라서 밖에서 센터로 끄기(순차, 가로)
    
    CMD_INTERACTIVE_VERTICAL_OUT_LED_ON_3STEP,         // 무대 반으로 갈라서 센터에서 밖으로 켜기(순차, 세로)
    CMD_INTERACTIVE_VERTICAL_OUT_LED_OFF_3STEP,         // 무대 반으로 갈라서 센터에서 밖으로 끄기(순차, 세로)
    CMD_INTERACTIVE_VERTICAL_IN_LED_ON_3STEP,         // 무대 반으로 갈라서 밖에서 센터로 켜기(순차, 세로)
    CMD_INTERACTIVE_VERTICAL_IN_LED_OFF_3STEP,         // 무대 반으로 갈라서 밖에서 센터로 끄기(순차, 세로)
    
    // 무지개(세로)
    CMD_INTERACTIVE_VERTICAL_UP_RAINBOW_6STEP,              // 세로(아래->위) 무지개 반복
    CMD_INTERACTIVE_VERTICAL_UP_RAINBOW_ONCE_6STEP,         // 세로(아래->위) 무지개 1회
    CMD_INTERACTIVE_VERTICAL_DOWN_RAINBOW_6STEP,            // 세로(위->아래) 무지개 반복 
    CMD_INTERACTIVE_VERTICAL_DOWN_RAINBOW_ONCE_6STEP,       // 세로(위->아래) 무지개 1회
    
    // 무지개(가로)
    CMD_INTERACTIVE_HORIZONTAL_UP_RAINBOW_6STEP,              // 가로(좌->우) 무지개 반복
    CMD_INTERACTIVE_HORIZONTAL_UP_RAINBOW_ONCE_6STEP,         // 가로(좌->우) 무지개 1회
    CMD_INTERACTIVE_HORIZONTAL_DOWN_RAINBOW_6STEP,            // 가로(우->좌) 무지개 반복 
    CMD_INTERACTIVE_HORIZONTAL_DOWN_RAINBOW_ONCE_6STEP,       // 가로(우->좌) 무지개 1회
    
    // 좌우 각각 ~
    CMD_INTERACTIVE_HORIZONTAL_LR_LED_ON_2_COLOR_6STEP,                         // 좌우 각기 다른 선택색 켜기
    CMD_INTERACTIVE_HORIZONTAL_LR_DIMMING_REPEAT_0_100_2_COLOR_6STEP,           // 좌우 각기 다른 색 디밍(0->100), 반복
    CMD_INTERACTIVE_HORIZONTAL_LR_DIMMING_REPEAT_20_100_2_COLOR_6STEP,          // 좌우 각기 다른 색 디밍(20->100), 반복
    CMD_INTERACTIVE_HORIZONTAL_LR_DIMMING_REPEAT_0_100_2_COLOR_ONCE_6STEP,      // 좌우 각기 다른 색 디밍(0->100), 1회
    CMD_INTERACTIVE_HORIZONTAL_LR_DIMMING_REPEAT_20_100_2_COLOR_ONCE_6STEP,     // 좌우 각기 다른 색 디밍(20->100), 1회
    CMD_INTERACTIVE_HORIZONTAL_LR_DIMMING_LED_ON_2_COLOR_6STEP,                 // 좌우 각기 다른 색 부드럽게 켜기
    CMD_INTERACTIVE_HORIZONTAL_LR_DIMMING_LED_OFF_2_COLOR_6STEP,                // 좌우 각기 다른 색 부드럽게 끄기
    CMD_INTERACTIVE_HORIZONTAL_LR_BLINK_2_COLOR_6STEP,                          // 좌우 각기 다른 색 점멸, 반복
    
    CMD_INTERACTIVE_VERTICAL_LR_ODD_EVEN_CHANGE_BLINK_2_COLOR_6STEP,         // 세로기준 홀,짝 색상 바꾸기 반복 - 6단계
    CMD_INTERACTIVE_HORIZONTAL_LR_ODD_EVEN_CHANGE_BLINK_2_COLOR_6STEP,         // 가로기준 홀,짝 색상 바꾸기 반복 - 6단계
    
    CMD_SHOW_H_CENTER_TO_OUT_GRA_2BAR_REPEAT,         // 가로기준 그라데이션2바 센터에서 밖으로 이동 반복
    CMD_SHOW_H_CENTER_TO_OUT_GRA_2BAR_RAINBOW_REPEAT, // 가로기준 그라데이션2바 센터에서 밖으로 이동, 무지개 반복
    CMD_SHOW_SPEED_FLASH,                             // 전체 번쩍이기
    CMD_SHOW_H_LEFT_TO_RIGHT_2BAR_REPEAT,             // 가로기준 그라데이션2바 왼쪽에서 오른쪽으로 이동 반복
    CMD_SHOW_H_LEFT_TO_RIGHT_2BAR_RAINBOW_REPEAT,     // 가로기준 그라데이션2바 왼쪽에서 오른쪽으로 이동, 무지개 반복
    CMD_SHOW_H_LEFT_TO_RIGHT_DRAW_RAINBOW_REPEAT,     // 가로기준 왼쪽에서 오른쪽으로 무지개 색깔 칠하면서 이동 반복
    CMD_SHOW_LR_ROTATION_BLINK,                       // 가로기준, 왼쪽 오른쪽 번갈아 색상 보이기 
    
    CMD_INTERACTIVE_FLOOR_SELECT_LED_ON_STANDING_FLOOR, // 층 선택색 켜기 - 스탠딩
    CMD_INTERACTIVE_FLOOR_SELECT_LED_ON_1_FLOOR, // 층 선택색 켜기 - 1층
    CMD_INTERACTIVE_FLOOR_SELECT_LED_ON_2_FLOOR, // 층 선택색 켜기 - 2층
    CMD_INTERACTIVE_FLOOR_SELECT_LED_ON_3_FLOOR, // 층 선택색 켜기 - 3층
    CMD_INTERACTIVE_FLOOR_SELECT_LED_ON_ETC_FLOOR, // 층 선택색 켜기 - 그 외층
    CMD_INTERACTIVE_FLOOR_SELECT_DIMMING_LED_ON_STANDING_FLOOR, // 층 선택색 부드럽게 켜기 - 스탠딩
    CMD_INTERACTIVE_FLOOR_SELECT_DIMMING_LED_ON_1_FLOOR, // 층 선택색 부드럽게 켜기 - 1층
    CMD_INTERACTIVE_FLOOR_SELECT_DIMMING_LED_ON_2_FLOOR, // 층 선택색 부드럽게 켜기 - 2층
    CMD_INTERACTIVE_FLOOR_SELECT_DIMMING_LED_ON_3_FLOOR, // 층 선택색 부드럽게 켜기 - 3층
    CMD_INTERACTIVE_FLOOR_SELECT_DIMMING_LED_ON_ETC_FLOOR, // 층 선택색 부드럽게 켜기 - 그 외층
    CMD_INTERACTIVE_FLOOR_SELECT_COLOR_BLINK_STANDING_FLOOR, // 층 선택색 점멸 - 스탠딩
    CMD_INTERACTIVE_FLOOR_SELECT_COLOR_BLINK_1_FLOOR, // 층 선택색 점멸 - 1층
    CMD_INTERACTIVE_FLOOR_SELECT_COLOR_BLINK_2_FLOOR, // 층 선택색 점멸 - 2층
    CMD_INTERACTIVE_FLOOR_SELECT_COLOR_BLINK_3_FLOOR, // 층 선택색 점멸 - 3층
    CMD_INTERACTIVE_FLOOR_SELECT_COLOR_BLINK_ETC_FLOOR, // 층 선택색 점멸 - 그 외층
    CMD_INTERACTIVE_FLOOR_SELECT_TWO_COLOR_CHANGE_STANDING_FLOOR, // 층 선택색 점멸 - 스탠딩
    CMD_INTERACTIVE_FLOOR_SELECT_TWO_COLOR_CHANGE_1_FLOOR, // 층 선택색 점멸 - 1층
    CMD_INTERACTIVE_FLOOR_SELECT_TWO_COLOR_CHANGE_2_FLOOR, // 층 선택색 점멸 - 2층
    CMD_INTERACTIVE_FLOOR_SELECT_TWO_COLOR_CHANGE_3_FLOOR, // 층 선택색 점멸 - 3층
    CMD_INTERACTIVE_FLOOR_SELECT_TWO_COLOR_CHANGE_ETC_FLOOR, // 층 선택색 점멸 - 그 외층
    CMD_INTERACTIVE_FLOOR_DIMMING_REPEAT_0_100_STANDING_FLOOR, // 층 디밍(0->100) 반복 - 스탠딩
    CMD_INTERACTIVE_FLOOR_DIMMING_REPEAT_0_100_1_FLOOR, // 층 디밍(0->100) 반복 - 1층
    CMD_INTERACTIVE_FLOOR_DIMMING_REPEAT_0_100_2_FLOOR, // 층 디밍(0->100) 반복 - 2층
    CMD_INTERACTIVE_FLOOR_DIMMING_REPEAT_0_100_3_FLOOR, // 층 디밍(0->100) 반복 - 3층
    CMD_INTERACTIVE_FLOOR_DIMMING_REPEAT_0_100_ETC_FLOOR, // 층 디밍(0->100) 반복 - 그 외층
    CMD_INTERACTIVE_FLOOR_SELECT_LED_ON_1_2_3_FLOOR, // 층 선택색 켜기 - 1, 2, 3층
    CMD_INTERACTIVE_FLOOR_SELECT_LED_ON_STANDING_ETC_FLOOR, // 층 선택색 켜기 - 스탠딩, 그외층
    CMD_INTERACTIVE_FLOOR_SELECT_DIMMING_LED_ON_STANDING_ETC_FLOOR, // 층 선택색 부드럽게 켜기 - 스탠딩, 그외층
    CMD_INTERACTIVE_FLOOR_SELECT_COLOR_BLINK_STANDING_ETC_FLOOR, // 층 선택색 점멸 - 스탠딩, 그외층
    CMD_INTERACTIVE_FLOOR_SELECT_TWO_COLOR_CHANGE_STANDING_ETC_FLOOR, // 층 선택색 점멸(2색) - 스탠딩, 그외층
    
    CMD_INTERACTIVE_LINE_SELECT_LED_ON_10STEP , // 라인 선택색 켜기 - 10단계
    CMD_INTERACTIVE_LINE_SELECT_LED_ON_15STEP , // 라인 선택색 켜기 - 15단계
    CMD_INTERACTIVE_LINE_SELECT_LED_ON_20STEP , // 라인 선택색 켜기 - 20단계
    CMD_INTERACTIVE_LINE_SELECT_LED_ON_25STEP , // 라인 선택색 켜기 - 25단계
    CMD_INTERACTIVE_LINE_SELECT_LED_ON_30STEP , // 라인 선택색 켜기 - 30단계
    
    CMD_INTERACTIVE_SHAPE_SELECT_UP_LED_ON_5STEP , // 모양 선택색 켜기(아래->위) - 5단계
    CMD_INTERACTIVE_SHAPE_SELECT_UP_LED_ON_7STEP , // 모양 선택색 켜기(아래->위) - 7단계
    CMD_INTERACTIVE_SHAPE_SELECT_UP_LED_ON_10STEP , // 모양 선택색 켜기(아래->위) - 10단계
    CMD_INTERACTIVE_SHAPE_SELECT_UP_LED_ON_15STEP , // 모양 선택색 켜기(아래->위) - 15단계
    CMD_INTERACTIVE_SHAPE_SELECT_UP_LED_ON_20STEP , // 모양 선택색 켜기(아래->위) - 20단계
    CMD_INTERACTIVE_SHAPE_SELECT_UP_LED_OFF_5STEP , // 모양 선택색 끄기(아래->위) - 5단계
    CMD_INTERACTIVE_SHAPE_SELECT_UP_LED_OFF_7STEP , // 모양 선택색 끄기(아래->위) - 7단계
    CMD_INTERACTIVE_SHAPE_SELECT_UP_LED_OFF_10STEP , // 모양 선택색 끄기(아래->위) - 10단계
    CMD_INTERACTIVE_SHAPE_SELECT_UP_LED_OFF_15STEP , // 모양 선택색 끄기(아래->위) - 15단계
    CMD_INTERACTIVE_SHAPE_SELECT_UP_LED_OFF_20STEP , // 모양 선택색 끄기(아래->위) - 20단계
    
    CMD_SHOW_SHAPE_LOGO_SHOW,                        // 글자 보이기(선택 2색)
    CMD_SHOW_SHAPE_LOGO_SHOW_BACK_OFF,               // 글자 보이기(배경은 끄기)
    CMD_SHOW_SHAPE_LOGO_3,                           // 글자 보이기(3)
    
    CMD_INTERACTIVE_VERTICAL_NUM,            // 세로 연출 번호
    CMD_INTERACTIVE_HORIZONTAL_NUM,            // 가로 연출 번호
    CMD_INTERACTIVE_FLOOR_NUM,            // 층 연출 번호
    CMD_INTERACTIVE_LINE_NUM,            // 라인 연출 번호
    CMD_INTERACTIVE_SHAPE_NUM,            // 모양 연출 번호
    
    CMD_DMX_CONTROL_STAY,
    CMD_DMX_CONTROL_RUN,
    

    // 이하 필요한지 검토
    //CMD_ON_PIXEL,
    //CMD_ON_ROW,
    //CMD_ON_COL,
    //CMD_SCREEN_RAINBOW_FLOW_DE_R,
    //CMD_SCREEN_RAINBOW_FLOW_DE_L,
    //CMD_STOP,
    
    CMD_SHOW_RAINBOW_COLOR,    
    CMD_CHANGE_FREQUENCY_1,
    CMD_CHANGE_FREQUENCY_2,
    CMD_CHANGE_FREQUENCY_3,
    CMD_CHANGE_FREQUENCY_4,
    CMD_CHANGE_FREQUENCY_5,
    CMD_CHANGE_FREQUENCY_6,
    CMD_BTN_RAINBOW
} ActionCommand;

// 컨트롤러로부터 들어온 제어 명령에 대한 값들
typedef struct {
    uint16 group;
    uint8 row;
    uint8 col;
    uint16 id;
    uint16 group_1;
    uint16 group_2;
    uint16 row_1;
    uint16 row_2;
    uint16 row_3;    
    uint16 col_1;
    uint16 col_2;
    uint16 col_3;  
    uint16 udSpeed;
    uint16 udSpeed_1;
    uint16 udSpeed_2;
    uint8 syncTime;
	uint8 product_group;
    uint8 company;
    uint8 artist;
    uint8 hallWidth;    // 공연장 가로 크기
    uint8 hallHeight;   // 공연장 세로 크기
    uint8 hallLine;     // 공연장 라인 크기
    uint8 dmxR;
    uint8 dmxG;
    uint8 dmxB;
    uint8 dmxD;
    uint8 dmxS;
    uint8 dmxRN;
    uint8 dmxGN;
    uint8 dmxBN;
    uint16 dmxRTemp;
    uint16 dmxGTemp;
    uint16 dmxBTemp;
    uint8 dmxDPercent;
    uint8 dmxShowDirection;
} GROUP_INFO_CONTROL_T;

typedef struct {
    uint16 group;
    uint8 row;
    uint8 col;
	uint8 vertical;
    uint8 horizontal;
    uint8 floor;
    uint8 line;
    uint8 shape;
    uint8 verticalHalf;
    uint8 horizontalHalf;
    uint8 hallWidthHalf;
    uint8 hallHeightHalf;
    uint8 myGroup;          // 개별제어시 사용되는 조명번호
    uint8 logo1;            // 글자1에 사용되는 번호
    uint8 logo2;            // 글자2에 사용되는 번호
    uint8 logo3;            // 글자3에 사용되는 번호
    uint8 showVersion;      // 공연번호
    uint8 showVersionDay;   // 공연번호, 날짜
		uint8 dummy;
	
		uint8 myGroup2;
		uint8 myGroup3;
		uint8 myGroup4;
		uint8 myGroup5;
		uint8 myGroup6;
		uint8 myGroup7;
		uint8 myGroup8;
		uint8 myGroup9;
		uint8 myGroup10;
		uint8 myGroup11;
		uint8 myGroup12;
		uint8 myGroup13;
		uint8 myGroup14;
		uint8 myGroup15;
		uint8 myGroup16;
		uint8 myGroup17;
		uint8 myGroup18;
		uint8 myGroup19;
		uint8 myGroup20;
		uint8 myGroup21;
    
    uint8 colorIndexForDeviceReg;  // 기기등록시 띄운 색상값
} GROUP_INFO_T;

enum BONG_RUN_SPEED {
    RUN_SPEED_1 = 0,   // very slow         / 40ms
    RUN_SPEED_2,       // slow             / 20ms
    RUN_SPEED_3,       // normal          / 15ms
    RUN_SPEED_4,       // fast           / 10ms
    RUN_SPEED_5,       //very fast      / 5ms
	  RUN_SPEED_6,
	  RUN_SPEED_7,
	  RUN_SPEED_8,
	  RUN_SPEED_9,
	  RUN_SPEED_10,
	  RUN_SPEED_11,
	  RUN_SPEED_12,
	  RUN_SPEED_13,
	  RUN_SPEED_14,
	  RUN_SPEED_15,
	  RUN_SPEED_16,
	  RUN_SPEED_17,
	  RUN_SPEED_18,
	  RUN_SPEED_19,
	  RUN_SPEED_20,	
    RUN_SPEED_NOT
};

/* 기존 프로토콜
enum RF_PACKET_PAYLOAD {
	RF_PKT_CMD_TYPE = 0,
	RF_PKT_COMPANY_CODE, // 1
	RF_PKT_SINGER_CODE,  // 2
	RF_PKT_GROUP,        // 3
	RF_PKT_CMD,          // 4
	RF_PKT_LED_R,
	RF_PKT_LED_G,
	RF_PKT_LED_B,
	RF_PKT_SPEED,
	RF_PKT_SYNC,
	RF_PKT_STOP_1,
	RF_PKT_STOP_2
};
*/

// 프로토콜, 위치조정
/* 2018.10.29
enum RF_PACKET_PAYLOAD {
	RF_PKT_GROUP = 0,        // 3
    RF_PKT_CMD,          // 4
    RF_PKT_SPEED,
	RF_PKT_SYNC,
    RF_PKT_LED_R,
	RF_PKT_LED_G,
	RF_PKT_LED_B,
    RF_PKT_CMD_TYPE,
	RF_PKT_COMPANY_CODE, // 1
	RF_PKT_SINGER_CODE,  // 2
	RF_PKT_STOP_1,
	RF_PKT_STOP_2
};
*/

/* 2018.10.30
typedef enum {
	//PKT_PRE = 0,   // 1
	//PKT_STA1,      // 2
	//PKT_STA2,      // 3
	PKT_CMD_TYPE = 0,   // 4, 구분
	PKT_CMD,       // 5, 기능
	PKT_COLOR1,    // 6, 색상1
	PKT_COLOR2,    // 7, 색상2
	PKT_SYNC,      // 8, 동기
	PKT_SPEED,     // 9, 속도
	PKT_ADD1,      // 10, 추가1
	PKT_ADD2,      // 11, 추가2
	PKT_ADD3,      // 12, 추가3
	PKT_GROUP1,    // 13
	PKT_GROUP2,    // 14
	PKT_GROUP3,    // 15
	PKT_GROUP4,    // 16
	PKT_FLOOR,     // 17
	PKT_STOP1,     // 18
	PKT_STOP2,      // 19
	PKT_R,               // R
    PKT_G,               // G
    PKT_B,               // B
	PKT_GROUP,
	PKT_LED_R,
	PKT_LED_G,
	PKT_LED_B
	
} PKT_DEF;
*/

// 2020.12.10일까지 사용하던 버전
///*
enum RF_PACKET_PAYLOAD {
	RF_PKT_PRODUCT_TYPE_1 = 0,   // 4, 제품구분1
	RF_PKT_PRODUCT_TYPE_2,       // 5, 제품구분2
	RF_PKT_CMD_TYPE,             // 6, 구분
	RF_PKT_CMD,                  // 7, 기능
	RF_PKT_COLOR_1,              // 8, 색상1
	RF_PKT_COLOR_2,              // 9, 색상2
    RF_PKT_SYNC,                 // 10, 동기
	RF_PKT_SPEED,                // 11, 속도
    RF_PKT_BRIGHT,               // 12, 밝기
    RF_PKT_ADD1,                 // 13, 추가1
    RF_PKT_ADD2,                 // 14, 추가2 
    RF_PKT_ADD3,                 // 15, 추가3
    RF_PKT_DIMMER_01_R,          // 16, 추가2 //RF_PKT_ADD4,                 // 16, 추가4
    RF_PKT_DIMMER_01_G,          // 17, 추가2 //RF_PKT_ADD55,                // 17, 추가2 
    RF_PKT_DIMMER_01_B,          // 18, 추가2 //RF_PKT_ADD56,                // 18, 추가2
    RF_PKT_DIMMER_01_S,          // 19, 추가2 //RF_PKT_ADD57,                // 19, 추가2
    RF_PKT_DIMMER_01_D,          // 20, RF_PKT_ADD58,                // 20, 추가2
    RF_PKT_DIMMER_02_R,          // 21, 추가2  //RF_PKT_ADD59,                // 21, 추가2
    RF_PKT_DIMMER_02_G,          // 22, 추가2  //RF_PKT_ADD60,                // 22, 추가2
    RF_PKT_DIMMER_02_B,          // 23, 추가2  //RF_PKT_ADD61,                // 23, 추가2   
    RF_PKT_DIMMER_02_S,          // 24, 추가2  //RF_PKT_ADD62,                // 24, 추가2
    RF_PKT_DIMMER_02_D,          // 25, 추가2
    RF_PKT_DIMMER_03_R,          // 26, 추가2
    RF_PKT_DIMMER_03_G,          // 27, 추가2
    RF_PKT_DIMMER_03_B,          // 28, 추가2
    RF_PKT_DIMMER_03_S,          // 29, 추가2
    RF_PKT_DIMMER_03_D,          // 30, 추가2
    RF_PKT_DIMMER_04_R,          // 31, 추가2
    RF_PKT_DIMMER_04_G,          // 32, 추가2
    RF_PKT_DIMMER_04_B,          // 33, 추가2
    RF_PKT_DIMMER_04_S,          // 34, 추가2
    RF_PKT_DIMMER_04_D,          // 35, 추가2
    RF_PKT_DIMMER_05_R,          // 36, 추가2
    RF_PKT_DIMMER_05_G,          // 37, 추가2
    RF_PKT_DIMMER_05_B,          // 38, 추가2
    RF_PKT_DIMMER_05_S,          // 39, 추가2
    RF_PKT_DIMMER_05_D,          // 40, 추가2
    RF_PKT_DIMMER_06_R,          // 41, 추가2
    RF_PKT_DIMMER_06_G,          // 42, 추가2
    RF_PKT_DIMMER_06_B,          // 43, 추가2    
    RF_PKT_DIMMER_06_S,          // 44, 추가2
    RF_PKT_DIMMER_06_D,          // 45, 추가2
    RF_PKT_DIMMER_07_R,          // 46, 추가2
    RF_PKT_DIMMER_07_G,          // 47, 추가2
    RF_PKT_DIMMER_07_B,          // 48, 추가2
    RF_PKT_DIMMER_07_S,          // 49, 추가2
    RF_PKT_DIMMER_07_D,          // 50, 추가2
    RF_PKT_DIMMER_08_R,          // 51, 추가2
    RF_PKT_DIMMER_08_G,          // 52, 추가2
    RF_PKT_DIMMER_08_B,          // 53, 추가2
    RF_PKT_DIMMER_08_S,          // 54, 추가2
    RF_PKT_DIMMER_08_D,          // 55, 추가2
    RF_PKT_DIMMER_09_R,          // 56, 추가2
    RF_PKT_DIMMER_09_G,          // 57, 추가2
    RF_PKT_DIMMER_09_B,          // 58, 추가2
    RF_PKT_DIMMER_09_S,          // 59, 추가2
    RF_PKT_DIMMER_09_D,          // 60, 추가2
    RF_PKT_DIMMER_10_R,          // 61, 추가2
    RF_PKT_DIMMER_10_G,          // 62, 추가2
    RF_PKT_DIMMER_10_B,          // 63, 추가2    
    RF_PKT_DIMMER_10_S,          // 64, 추가2
    RF_PKT_DIMMER_10_D,          // 65, 추가2
    RF_PKT_DIMMER_11_R,          // 66, 추가2
    RF_PKT_DIMMER_11_G,          // 67, 추가2
    RF_PKT_DIMMER_11_B,          // 68, 추가2    
    RF_PKT_DIMMER_11_S,          // 69, 추가2
    RF_PKT_DIMMER_11_D,          // 70, 추가2
    RF_PKT_DIMMER_12_R,          // 71, 추가2
    RF_PKT_DIMMER_12_G,          // 72, 추가2
    RF_PKT_DIMMER_12_B,          // 73, 추가2    
    RF_PKT_DIMMER_12_S,          // 74, 추가2
    RF_PKT_DIMMER_12_D,          // 75, 추가2
    RF_PKT_DIMMER_13_R,          // 76, 추가2
    RF_PKT_DIMMER_13_G,          // 77, 추가2
    RF_PKT_DIMMER_13_B,          // 78, 추가2    
    RF_PKT_DIMMER_13_S,          // 79, 추가2
    RF_PKT_DIMMER_13_D,          // 80, 추가2
    RF_PKT_DIMMER_14_R,          // 81, 추가2
    RF_PKT_DIMMER_14_G,          // 82, 추가2
    RF_PKT_DIMMER_14_B,          // 83, 추가2    
    RF_PKT_DIMMER_14_S,          // 84, 추가2
    RF_PKT_DIMMER_14_D,          // 85, 추가2
    RF_PKT_DIMMER_15_R,          // 86, 추가2
    RF_PKT_DIMMER_15_G,          // 87, 추가2
    RF_PKT_DIMMER_15_B,          // 88, 추가2    
    RF_PKT_DIMMER_15_S,          // 89, 추가2
    RF_PKT_DIMMER_15_D,          // 90, 추가2
    RF_PKT_DIMMER_16_R,          // 91, 추가2
    RF_PKT_DIMMER_16_G,          // 92, 추가2
    RF_PKT_DIMMER_16_B,          // 93, 추가2    
    RF_PKT_DIMMER_16_S,          // 94, 추가2
    RF_PKT_DIMMER_16_D,          // 95, 추가2
    RF_PKT_DIMMER_17_R,          // 96, 추가2
    RF_PKT_DIMMER_17_G,          // 97, 추가2
    RF_PKT_DIMMER_17_B,          // 98, 추가2    
    RF_PKT_DIMMER_17_S,          // 99, 추가2
    RF_PKT_DIMMER_17_D,          // 100, 추가2
    RF_PKT_DIMMER_18_R,          // 101, 추가2
    RF_PKT_DIMMER_18_G,          // 102, 추가2
    RF_PKT_DIMMER_18_B,          // 103, 추가2    
    RF_PKT_DIMMER_18_S,          // 104, 추가2
    RF_PKT_DIMMER_18_D,          // 105, 추가2
    RF_PKT_DIMMER_19_R,          // 106, 추가2
    RF_PKT_DIMMER_19_G,          // 107, 추가2
    RF_PKT_DIMMER_19_B,          // 108, 추가2    
    RF_PKT_DIMMER_19_S,          // 109, 추가2
    RF_PKT_DIMMER_19_D,          // 110, 추가2
    RF_PKT_DIMMER_20_R,          // 111, 추가2
    RF_PKT_DIMMER_20_G,          // 112, 추가2
    RF_PKT_DIMMER_20_B,          // 113, 추가2    
    RF_PKT_DIMMER_20_S,          // 114, 추가2
    RF_PKT_DIMMER_20_D,          // 115, 추가2
    RF_PKT_DIMMER_21_R,          // 116, 추가2
    RF_PKT_DIMMER_21_G,          // 117, 추가2
    RF_PKT_DIMMER_21_B,          // 118, 추가2    
    RF_PKT_DIMMER_21_S,          // 119, 추가2
    RF_PKT_DIMMER_21_D,          // 120, 추가2
    RF_PKT_DMX_COLOR1,           // 121, 추가2
    RF_PKT_DMX_SPEED,            // 122, 추가2   
    RF_PKT_DMX_LOGO,             // 123, 추가2
    RF_PKT_GROUP1,               // 124, 추가2
    RF_PKT_GROUP2,               // 125, 추가2
    RF_PKT_GROUP3,               // 126, 추가2
    RF_PKT_GROUP4,               // 127, 추가2
    RF_PKT_FLOOR,                // 128, 추가2
	  RF_PKT_STOP,                 // 129, stop
    RF_PKT_CRC                   // 130, crc, 28개(스타트 신호 3바이트 빠짐)
};
//*/

// 2020.12.10 이후 수정본
/*
enum RF_PACKET_PAYLOAD {
	RF_PKT_PRODUCT_TYPE_1 = 0,   // 4, 제품구분1     
	RF_PKT_PRODUCT_TYPE_2,       // 5, 제품구분2       
	RF_PKT_CMD_TYPE,             // 6, 구분                    
	RF_PKT_CMD,                  // 7, 기능                      
	RF_PKT_COLOR_1,              // 8, 색상1              
	RF_PKT_COLOR_2,              // 9, 색상2                 
    RF_PKT_SYNC,                 // 10, 동기                  
	RF_PKT_SPEED,                // 11, 속도                     
    RF_PKT_BRIGHT,               // 12, 밝기                    
    RF_PKT_ADD1,                 // 13, 추가1                     
    RF_PKT_ADD2,                 // 14, 추가2                     
    RF_PKT_ADD3,                 // 15, 추가3                     
    RF_PKT_DIMMER_01_R,          // 16, 추가2 //RF_PKT_ADD4,                 // 16, 추가4
    RF_PKT_DIMMER_01_G,          // 17, 추가2 //RF_PKT_ADD55,                // 17, 추가2 
    RF_PKT_DIMMER_01_B,          // 18, 추가2 //RF_PKT_ADD56,                // 18, 추가2	
    RF_PKT_DIMMER_01_S,          // 19, 추가2 //RF_PKT_ADD57,                // 19, 추가2
	
    RF_PKT_DIMMER_02_R,          // 20, 추가2  //RF_PKT_ADD59,                // 21, 추가2
    RF_PKT_DIMMER_02_G,          // 21, 추가2  //RF_PKT_ADD60,                // 22, 추가2
    RF_PKT_DIMMER_02_B,          // 22, 추가2  //RF_PKT_ADD61,                // 23, 추가2   
    RF_PKT_DIMMER_02_S,          // 23, 추가2  //RF_PKT_ADD62,                // 24, 추가2
		
    RF_PKT_DIMMER_03_R,          // 24, 추가2
    RF_PKT_DIMMER_03_G,          // 25, 추가2
    RF_PKT_DIMMER_03_B,          // 26, 추가2
    RF_PKT_DIMMER_03_S,          // 27, 추가2
		
    RF_PKT_DIMMER_04_R,          // 28, 추가2
    RF_PKT_DIMMER_04_G,          // 29, 추가2
    RF_PKT_DIMMER_04_B,          // 30, 추가2
    RF_PKT_DIMMER_04_S,          // 31, 추가2
		
    RF_PKT_DIMMER_05_R,          // 32, 추가2
    RF_PKT_DIMMER_05_G,          // 33, 추가2
    RF_PKT_DIMMER_05_B,          // 34, 추가2
    RF_PKT_DIMMER_05_S,          // 35, 추가2
		
    RF_PKT_DIMMER_06_R,          // 36, 추가2
    RF_PKT_DIMMER_06_G,          // 37, 추가2
    RF_PKT_DIMMER_06_B,          // 38, 추가2    
    RF_PKT_DIMMER_06_S,          // 39, 추가2
		
    RF_PKT_DIMMER_07_R,          // 40, 추가2
    RF_PKT_DIMMER_07_G,          // 41, 추가2
    RF_PKT_DIMMER_07_B,          // 42, 추가2
    RF_PKT_DIMMER_07_S,          // 43, 추가2
    
    RF_PKT_DIMMER_08_R,          // 44, 추가2
    RF_PKT_DIMMER_08_G,          // 45, 추가2
    RF_PKT_DIMMER_08_B,          // 46, 추가2
    RF_PKT_DIMMER_08_S,          // 47, 추가2

		RF_PKT_DIMMER_09_R,          // 48, 추가2
    RF_PKT_DIMMER_09_G,          // 49, 추가2
    RF_PKT_DIMMER_09_B,          // 50, 추가2
    RF_PKT_DIMMER_09_S,          // 51, 추가2

    RF_PKT_DIMMER_10_R,          // 52, 추가2
    RF_PKT_DIMMER_10_G,          // 53, 추가2
    RF_PKT_DIMMER_10_B,          // 54, 추가2    
    RF_PKT_DIMMER_10_S,          // 55, 추가2

    RF_PKT_DIMMER_11_R,          // 56, 추가2
    RF_PKT_DIMMER_11_G,          // 57, 추가2
    RF_PKT_DIMMER_11_B,          // 58, 추가2    
    RF_PKT_DIMMER_11_S,          // 59, 추가2

    RF_PKT_DIMMER_12_R,          // 60, 추가2
    RF_PKT_DIMMER_12_G,          // 61, 추가2
    RF_PKT_DIMMER_12_B,          // 62, 추가2    
    RF_PKT_DIMMER_12_S,          // 63, 추가2

    RF_PKT_DIMMER_13_R,          // 64, 추가2
    RF_PKT_DIMMER_13_G,          // 65, 추가2
    RF_PKT_DIMMER_13_B,          // 66, 추가2    
    RF_PKT_DIMMER_13_S,          // 67, 추가2

    RF_PKT_DIMMER_14_R,          // 68, 추가2
    RF_PKT_DIMMER_14_G,          // 69, 추가2
    RF_PKT_DIMMER_14_B,          // 70, 추가2    
    RF_PKT_DIMMER_14_S,          // 71, 추가2

    RF_PKT_DIMMER_15_R,          // 72, 추가2
    RF_PKT_DIMMER_15_G,          // 73, 추가2
    RF_PKT_DIMMER_15_B,          // 74, 추가2    
    RF_PKT_DIMMER_15_S,          // 75, 추가2

    RF_PKT_DIMMER_16_R,          // 76, 추가2
    RF_PKT_DIMMER_16_G,          // 77, 추가2
    RF_PKT_DIMMER_16_B,          // 78, 추가2    
    RF_PKT_DIMMER_16_S,          // 79, 추가2

    RF_PKT_DIMMER_17_R,          // 80, 추가2
    RF_PKT_DIMMER_17_G,          // 81, 추가2
    RF_PKT_DIMMER_17_B,          // 82, 추가2    
    RF_PKT_DIMMER_17_S,          // 83, 추가2

    RF_PKT_DIMMER_18_R,          // 84, 추가2
    RF_PKT_DIMMER_18_G,          // 85, 추가2
    RF_PKT_DIMMER_18_B,          // 86, 추가2    
    RF_PKT_DIMMER_18_S,          // 87, 추가2

    RF_PKT_DIMMER_19_R,          // 88, 추가2
    RF_PKT_DIMMER_19_G,          // 89, 추가2
    RF_PKT_DIMMER_19_B,          // 90, 추가2    
    RF_PKT_DIMMER_19_S,          // 91, 추가2
    
    RF_PKT_DIMMER_20_R,          // 92, 추가2
    RF_PKT_DIMMER_20_G,          // 93, 추가2
    RF_PKT_DIMMER_20_B,          // 94, 추가2    
    RF_PKT_DIMMER_20_S,          // 95, 추가2

    RF_PKT_DIMMER_21_R,          // 96, 추가2
    RF_PKT_DIMMER_21_G,          // 97, 추가2
    RF_PKT_DIMMER_21_B,          // 98, 추가2    
    RF_PKT_DIMMER_21_S,          // 99, 추가2

    RF_PKT_DMX_COLOR1,           // 100, 추가2
    RF_PKT_DMX_SPEED,            // 101, 추가2   
    RF_PKT_DMX_LOGO,             // 102, 추가2
    RF_PKT_GROUP1,               // 103, 추가2
    RF_PKT_GROUP2,               // 104, 추가2
    RF_PKT_GROUP3,               // 105, 추가2
    RF_PKT_GROUP4,               // 106, 추가2
    RF_PKT_FLOOR,                // 107, 추가2
	  RF_PKT_STOP,                 // 108, stop
    RF_PKT_CRC                   // 109, crc, 28개(스타트 신호 3바이트 빠짐)
};
*/

/*
enum RF_PACKET_PAYLOAD {
	RF_PKT_CMD_TYPE = 0,    // 구분
	RF_PKT_CMD,             // 기능
	RF_PKT_COLOR_1,         // 인덱스 색상1
	RF_PKT_COLOR_2,         // 인덱스 색상2
    RF_PKT_R,               // R
    RF_PKT_G,               // G
    RF_PKT_B,               // B
	RF_PKT_SPEED,           // 속도
	RF_PKT_SYNC,            // 동기
    RF_PKT_PRODUCT_TYPE_1,  // 제품구분1
	RF_PKT_PRODUCT_TYPE_2,  // 제품구분2
    RF_PKT_STOP,            // stop
    RF_PKT_CRC,             // checksum
};
*/

extern GROUP_INFO_CONTROL_T groupInfoCmd;
extern GROUP_INFO_T groupInfo;

#endif