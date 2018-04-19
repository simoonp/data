1、include.h{
	添加
​	include  "my_cfg.h"
	#include  "my_cfg.h"
	#include  "init.h"
	#include  "duoji.h"
	#include  "image.h"
	#include  "moto.h"
	#include  "my_UI.h"
	#include  "recognization.h"
	//#include  "roundabout.h"
	#include "filter.h"
}
2、PORT_cfg.h{
	注释
	/**********************************  TPM    ***************************************/
                                     // ALT6   ALT7  ALT6
	#define TPM1_CH0_PIN    PTA12        //PTA8 、PTA12、PTB0
	#define TPM1_CH1_PIN    PTA13       //PTA9 、PTA13、PTB1
										// ALT6
	#define TPM2_CH0_PIN    PTA10       //PTA10、PTB18
	#define TPM2_CH1_PIN    PTA11       //PTA11、PTB19

	#define TPM_CLKIN0_PIN  PTB16       //PTA18 、PTB16  、PTC12
	#define TPM_CLKIN1_PIN  PTB17       //PTA19  、PTB17  、PTC13
	修改56、57、78、79行的端口 为 PTA8、PTA9、PTA8、PTA9
}
3、k66版本的enter.h文件有大量的改动
4、k66版本的管脚复用有改动！！！！！！！！！！！！！！！！！
5、option -> C/C++compiler -> Preprocesseor 修改为{
	$PROJ_DIR$\..\..\App
	$PROJ_DIR$\..\..\App\Inc
	$PROJ_DIR$\..\..\Board\Inc
	$PROJ_DIR$\..\..\Chip\inc
	$PROJ_DIR$\..\..\Chip\inc\IAR
	$PROJ_DIR$\..\..\Chip\inc\kinetis
	$PROJ_DIR$\..\..\Lib\CMSIS\Inc
	$PROJ_DIR$\..\..\Lib\FatFs
	$PROJ_DIR$\..\..\Lib\USB
	$PROJ_DIR$\..\..\App\zhaozishuo_h
}
6、init.c{
	第13行修改为set_vector_handler(DMA0_DMA16_VECTORn , DMA0_IRQHandler);     //设置 DMA0 的中断服务函数为 PORTA_IRQHandler
}
7、init.c中舵机初始化{
	22行     ftm_pwm_init(DUO_JI, DUO_JI_CH,300,DJ_CENTER,FTM3_CH0_PIN);         // 初始化 FTM PWM    
}
8、moto.c{
	电机初始化
		ftm_pwm_init(FTM0, FTM_CH2,10 * 1000,0, FTM0_CH2_PIN);      //初始化 电机 PWM
		ftm_pwm_init(FTM0, FTM_CH3,10 * 1000,0, FTM0_CH3_PIN);      //初始化 电机 PWM
	正交解码初始化
		   ftm_quad_init(FTM1,FTM_PS_1,FTM1_QDPHA_PIN,FTM1_QDPHB_PIN);                                    //FTM1 正交解码初始化
}
9、enter.h{
	添加选中字体颜色
	#define Selected_color  BRED    // 
}
10、彩屏调整{
	在彩屏初始化函数 LCD_init() 中取消LCD_SET_DIR(LCD_DIR+1)的注释以旋转屏幕，在K66的程序中，要在VCAN_LCD_ST7735S.h中第25行将 ST7735S_DIR_DEFAULT 配置为1 
	VCAN_key.h中，按键枚举
		typedef enum
		{
			//////////////////默认
		//    KEY_U,  //上
		//    KEY_D,  //下
		//
		//    KEY_L,  //左
		//    KEY_R,  //右
			/////////////////////////顺时针旋转90度后
			KEY_L,  //左
			KEY_R,  //右
			KEY_D,  //下   
			KEY_U,  //上
			
			KEY_A,  //取消
			KEY_B,  //选择
			//KEY_START,  //开始
			//KEY_STOP,   //停止
			KEY_MAX,
		} KEY_e;
		
	filter.c中
	////////////////彩屏画点//////////////////////////////////////
	int test_draw_point(uint16 line,uint16 x,uint16 color)
	{
		Site_t site;   
		site.x=x;
		site.y=line;
		LCD_point(site,color);
		return 0;
	}
	////////////////////////////////////////////
	在void fitting_mid_line()末尾添加
	/////////彩屏画线/// ////////////////
    if(my_debug_flag == 1){
        if(my_debug_flag==1){
            for(row=START_Y; row<=valid_line; row++)
            test_draw_point(LCD_H-row,mid_line[row]-50,0X0000);   //黑色线条
        }
        for(x=40;x<80;x++){
            site.x=x+50;
            site.y=LCD_H-valid_line;
            LCD_point(site,0XFFE0);
        }
	}
    ///////////////////////////////////
	在void tixing_jiaozheng()末尾添加
	////////小彩屏画线/////////  /////////////////
	if(my_debug_flag==1)
	{
		test_draw_point(LCD_H-row,left_bundary[row]-50,0X001F);
		test_draw_point(LCD_H-row,right_bundary[row]-50,0X001F);
	}
	//////////////////////////////////
}
11、K66使用摄像头需要在场中断、PCLk引脚加上130欧的下拉电阻，对应是PTA29、PTA27引脚
