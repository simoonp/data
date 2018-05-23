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
	#define Selected_color  BRED    //司明朋
}
10、彩屏调整{
	旋转屏幕，VCAN_LCD_ST7735S.h中，将 ST7735S_DIR_DEFAULT 配置为1 
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
	void draw_mid_line(void)
	{
		Site_t site;
		uint16 x;
		uint16 row;

		if(my_debug_flag==1){
			for(row=START_Y; row<=valid_line; row++)
				test_draw_point(LCD_H-row,mid_line[row]-50,BLACK);   //黑色线条
			for(x=10;x<130;x++){
				site.x=x++;
				site.y=LCD_H-valid_line;
				LCD_point(site,RED);
			}
		}
	}
	///////////////////////
	void draw_left_right_line(void)
	{
		uint16 row;
		for(row = START_Y;row <= valid_line;row++){
			test_draw_point(LCD_H-row,left_bundary[row]-50,BLUE);
			test_draw_point(LCD_H-row,right_bundary[row]-50,BLUE);
		}
	}

	//添加参数显示angle、even_diff、valid_line
	void parameter_display(void)
	{    
		Site_t position_char[3] = {{CAMERA_W+1, 5}, {CAMERA_W+1, 15}, {CAMERA_W+1, 25}};
		Site_t position_num[3] = {{CAMERA_W+11, 5}, {CAMERA_W+11, 15}, {CAMERA_W+11, 25}};
			
		if(valid_line > 0)
			LCD_num(position_num[0],(uint32)valid_line, BLACK,WHITE);
		else
			LCD_num(position_num[0],(uint32)valid_line, RED,WHITE);
		LCD_char(position_char[0],'V', BLUE, WHITE);
		
		if(even_diff > 0)
			LCD_num(position_num[1],(uint32)even_diff, BLACK,WHITE);      
		else
			LCD_num(position_num[1],0-even_diff, RED,WHITE);
		LCD_char(position_char[1],'E', BLUE, WHITE);
		
		if(angle > 0)
			LCD_num(position_num[2],(uint32)angle, BLACK,WHITE);
		else
			LCD_num(position_num[2],0 - angle, RED,WHITE);
		LCD_char(position_char[2],'A', BLUE, WHITE);
	}
	//放在open_img_ui()的while循环最后
	
}

11、K66使用摄像头需要在场中断、PCLk引脚加上130欧的下拉电阻，对应是PTA29、PTA27引脚

12、在小彩屏中添加摄像头阈值设置{		//5月17日
	在 my_cfg.c 末尾添加 uint8 my_change; 和 uint8 CNST_change_flag=0;
	在 my_cfg.h 末尾添加 extern uint8 my_change; 和 extern uint8 CNST_change_flag;
	在 VCAN_OV7725_Eagle.c 的245行的 for( i = 0 ; i < ov7725_eagle_cfgnum ; i++ ) 循环中添加 
		if(ov7725_eagle_reg[i].addr == OV7725_CNST)
			ov7725_eagle_reg[i].val = my_change;
	在 VCAN_OV7725_Eagle.c 加入 extern uint8 my_change; 作为全局变量
	在小彩屏中
		修改 change_pid_ui()
			void change_pid_ui()
			{
				Site_t site;
				uint8 str_buf[STR_BUF_LEN];
				//float n = 1.0;
				int n = 0;
				uint8 char_H=20,key_num;
				LCD_init();        //初始化
				while(1){
					site.y = 5;
					site.x=3; 
					cloor_table[choose_num]=Selected_color;        
					LCD_clear(BCOLOUR);
					memset(str_buf,0,STR_BUF_LEN);
					sprintf((char *)str_buf,"Kp:");
					LCD_str(site,str_buf,FCOLOUR,cloor_table[0]);

					site.x += 10*8;
					memset(str_buf,0,STR_BUF_LEN);
					sprintf((char *)str_buf,"%.1f",moto_system_Kp);
					LCD_str(site,str_buf,FCOLOUR,cloor_table[5]); 
					//----------------------------------------------
					site.y += char_H;
					site.x = 3;
					memset(str_buf,0,STR_BUF_LEN);
					sprintf((char *)str_buf,"Ti");
					LCD_str(site,str_buf,FCOLOUR,cloor_table[1]);   //显示8*16字符串
					
					site.x= 10*8;
					memset(str_buf,0,STR_BUF_LEN);
					sprintf((char *)str_buf,"%.1f",moto_system_Ti);
					LCD_str(site,str_buf,FCOLOUR,cloor_table[6]);   //显示8*16字符串
					//------------------------------------------------
					site.y += char_H;
					site.x = 3;
					memset(str_buf,0,STR_BUF_LEN);
					sprintf((char *)str_buf,"Td");
					LCD_str(site,str_buf,FCOLOUR,cloor_table[2]);   //显示8*16字符串
					
					site.x= 10*8;
					memset(str_buf,0,STR_BUF_LEN);
					sprintf((char *)str_buf,"%.1f",moto_system_Td);
					LCD_str(site,str_buf,FCOLOUR,cloor_table[7]);   //显示8*16字符串
					//-----------------------------------------------
					site.y += char_H;
					site.x = 3;
					memset(str_buf,0,STR_BUF_LEN);
					sprintf((char *)str_buf,"n");
					LCD_str(site,str_buf,FCOLOUR,cloor_table[3]);   //显示8*16字符串
					
					site.x= 10*8;
					memset(str_buf,0,STR_BUF_LEN);
					sprintf((char *)str_buf,"%.1f",change_n[n]);
					LCD_str(site,str_buf,FCOLOUR,cloor_table[8]);   //显示8*16字符串
					//--------------------------------------
					site.y += char_H;
					site.y += char_H;
					site.x = 3;
					memset(str_buf,0,STR_BUF_LEN);
					sprintf((char *)str_buf,"CNST");
					LCD_str(site,str_buf,FCOLOUR,cloor_table[4]);   //显示8*16字符串
					
					site.x= 10*8;
					memset(str_buf,0,STR_BUF_LEN);
					sprintf((char *)str_buf,"%d",my_change);
					LCD_str(site,str_buf,FCOLOUR,cloor_table[9]);   //显示8*16字符串
					//--------------------------------------
					cloor_table[choose_num]=BCOLOUR;
					key_num=wait_key_down();

					if(key_num==KEY_U){   
						if(choose_num<5){
							if(choose_num == 0)
							  choose_num=5;
							choose_num--;
						}
						else  if(choose_num==5){
							moto_system_Kp += change_n[n];
							if(moto_system_Kp > 200)
								moto_system_Kp=200;
						}
						else  if(choose_num==6){
							moto_system_Ti+=change_n[n];
							if(moto_system_Ti > 100)
								moto_system_Ti=100;
						}
						else  if(choose_num==7){
							moto_system_Td+=change_n[n];
							if(moto_system_Td > 100)
								moto_system_Td=100;
						}
						else  if(choose_num==8){
							n++;
							if(n > 4)
								n=4;
						}
						else  if(choose_num==9){
							CNST_change_flag=1;
							my_change++;
						}
					}
					else if(key_num==KEY_D){   
						if(choose_num<5){
							choose_num++;
							if(choose_num == 5)
							  choose_num=0;
						}
						else  if(choose_num==5){
							moto_system_Kp-=change_n[n];
							if(moto_system_Kp<0)
								moto_system_Kp=0;
						}
						else  if(choose_num==6){
							moto_system_Ti-=change_n[n];
							if(moto_system_Ti < 0)
								moto_system_Ti=0;
						}
						else  if(choose_num==7){
							moto_system_Td-=change_n[n];
							if(moto_system_Td < 0)
								moto_system_Td=0;
						}
						else  if(choose_num==8){
							n--;
							if(n < 0)
								n=0;
						}
						else  if(choose_num==9){
							CNST_change_flag=1;
							my_change--;
						}
					}
					else if(key_num==KEY_R){
						if(choose_num<5)
						  choose_num+=5;
					}
					else if(key_num==KEY_L){
						if(choose_num<5){
							choose_num=0;
							if(CNST_change_flag==1)
								camera_init(imgbuff);
							CNST_change_flag=0;
							UI_state=MAIN_UI;
							break;
						}
						else
						  choose_num-=5;
					}
					else if(key_num==KEY_B){
						choose_num=0;
						if(CNST_change_flag==1)
								camera_init(imgbuff);
						CNST_change_flag=0;
						UI_state=MAIN_UI;
						break;
					}
				}
			}
		
}
