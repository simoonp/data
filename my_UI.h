#ifndef MY_UI_H_
#define MY_UI_H_

#include "include.h"

#define STR_BUF_LEN 30

extern int curve_speed_max;
extern int curve_speed_min;
extern uint8 my_debug_flag;
//extern int  dj_change;

/*************彩屏修改****/
extern uint8 my_change; 
extern uint8 CNST_change_flag;

extern uint8 curve_start_change; 
extern uint8 curve_end_change;

extern uint8 image_midvalue_change;
/**************/
///////////////////
typedef enum
{
    MAIN_UI  =   0,         //按键按下时对应电平
    SET_SPEED_UI   =  1,         //按键弹起时对应电平
    IMAGE_UI,               //长按按键(用于定时按键扫描)
    CHANGE_PID_UI,
    OPEN_IMG_UI,
    //5月1日起调试
    EXPECT_SPEED,
    /////////////////
} UI_STATUS_e;

extern void my_UI();


#endif


}
