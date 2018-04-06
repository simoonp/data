//#include "my_UI.h"
//#include "control.h"
//#include "speed_new.h"
//#include "VCAN_LCD.h"
//#include "path.h"
//#include "my_cfg.h"
//#include "VCAN_key.h"
#include "common.h"
#include "include.h"
//开机 界面有几种速度选项 及图像还有图像阈值选项
//交互：向左返回上一菜单，右键跟近下一个，长按中间的按键返回主菜单。
//******主菜单第一项是起跑**** 中间件确认 长按0.3秒
//如果显示的字符背景色错乱，检查头文件LCD.h中LCD_EN_W定义的字符宽度与LCD.c中LCD_str()函数中site.x是否相等
uint8 key_state;
uint8 UI_state=MAIN_UI;
uint8 choose_num=0;
uint16 cloor_table[8]={BCOLOUR,BCOLOUR,BCOLOUR,BCOLOUR,BCOLOUR,BCOLOUR,BCOLOUR,BCOLOUR};
void draw_mark_line();
float change_n[5]={0.1, 1, 2, 5, 10};
uint8 wait_key_down()
{
    uint8 ret_key=0xff;
    uint16 key_time=0;
    while(1)
    {
      if(key_check(KEY_U)==KEY_DOWN)
          ret_key=KEY_U;
      if(key_check(KEY_D)==KEY_DOWN)
          ret_key=KEY_D;
      if(key_check(KEY_L)==KEY_DOWN)
          ret_key=KEY_L;
      if(key_check(KEY_R)==KEY_DOWN)
          ret_key=KEY_R;
      if(key_check(KEY_A)==KEY_DOWN)
          ret_key=KEY_A;
      // if(key_check(KEY_START)==KEY_DOWN)
          // ret_key=KEY_START;
      if(key_check(KEY_B)==KEY_DOWN)
          ret_key=KEY_B;
      // if(key_check(KEY_STOP)==KEY_DOWN)
          // ret_key=KEY_STOP;
      if(ret_key!=0xff)
      {
          key_state=KEY_DOWN;
          while(key_get(ret_key)==KEY_DOWN)
          {
              DELAY_MS(1);
              key_time++;
          }
          if(key_time>300)
          {
              key_state=KEY_HOLD;
          }
          break;
      }
      camera_get_img();
//      ctrl_main();
    }
    return ret_key;
}

void draw_mark_line()
{
    uint16 x=0,y=0;
    Site_t site;
    for(y=0;y<IMG_H;y+=10)
    {
        for(x=0;x<IMG_W;x++)
        {
            site.x=x;
            site.y=y;
            LCD_point(site,RED);
        }
    }
    for(y=0;y<IMG_H;y++)
    {
        for(x=0;x<IMG_W;x+=10)
        {
            site.x=x;
            site.y=y;
            LCD_point(site,RED);
        }
    }
}

void main_ui()
{
    uint8 str_buf[STR_BUF_LEN];
    uint8 char_H=16,key_num=0xff;
    Site_t site;
    while(1)
    {
        site.y = 5;
        site.x = 3;
        cloor_table[choose_num]=Selected_color;
        LCD_clear(BCOLOUR);
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"0 run       ");
        LCD_str(site,str_buf,FCOLOUR,cloor_table[0]);   //显示8*16字符串
        site.y += char_H;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"1 speed_1   ");
        LCD_str(site,str_buf,FCOLOUR,cloor_table[1]);   //显示8*16字符串
        site.y += char_H;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"2 speed_2   ");
        LCD_str(site,str_buf,FCOLOUR,cloor_table[2]);   //显示8*16字符串
        site.y += char_H;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"3 speed_3   ");
        LCD_str(site,str_buf,FCOLOUR,cloor_table[3]);   //显示8*16字符串
        site.y += char_H;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"4 set speed ");
        LCD_str(site,str_buf,FCOLOUR,cloor_table[4]);   //显示8*16字符串
        site.y += char_H;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"5 open img  ");
        LCD_str(site,str_buf,FCOLOUR,cloor_table[5]);   //显示8*16字符串
        site.y += char_H;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"6 change pid");
        LCD_str(site,str_buf,FCOLOUR,cloor_table[6]);   //显示8*16字符串
        
        cloor_table[choose_num]=BCOLOUR;
        key_num=wait_key_down();
        if(key_num==KEY_U)
        {           
            choose_num--;
            if(choose_num>7)
              choose_num=6;
        }
        else if(key_num==KEY_D)
        {
            choose_num++;
            if(choose_num==7)
              choose_num=0;
        }
        else if(key_num==KEY_R)
        {
            break;
        }
        else if(key_num==KEY_B)
        {
            choose_num=0;
            UI_state=MAIN_UI;
            break;
        }
        
    }
    switch(choose_num)
    {
    case 0:UI_state=0xff;break;	//speed_table[][2], 修改为curve_speed[][2]，在my_cfg.c中
    case 1:zhidao_speed=curve_speed[0][0];CD_speed=curve_speed[0][1];UI_state=0xff;break;
    case 2:zhidao_speed=curve_speed[1][0];CD_speed=curve_speed[1][1];UI_state=0xff;break;
    case 3:zhidao_speed=curve_speed[2][0];CD_speed=curve_speed[2][1];UI_state=0xff;break;
    case 4:UI_state=SET_SPEED_UI;break;
    case 5:UI_state=OPEN_IMG_UI;break;
    case 6:UI_state=CHANGE_PID_UI;break;		//在my_UI.h（第9行）中把SET_IMG_UI改为CHANGE_PID_UI
    }
    choose_num=0;
}
void set_speed_ui()
{
    uint8 str_buf[STR_BUF_LEN];
    uint8 char_H=22,key_num;
    Site_t site = {3,5};
    float kpv=kp_val;
    while(1)
    {
      //ring_road_w
        
        site.y = 5;
        site.x=3; 
        cloor_table[choose_num]=Selected_color;
        LCD_clear(BCOLOUR);
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"max speed   ");
        LCD_str(site,str_buf,FCOLOUR,cloor_table[0]);   //显示8*16字符串
        
        site.x += 12*8;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"%d",zhidao_speed);
        LCD_str(site,str_buf,FCOLOUR,cloor_table[3]);   //显示8*16字符串

        site.y += char_H;
        site.x = 3;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"min speed   ");
        LCD_str(site,str_buf,FCOLOUR,cloor_table[1]);   //显示8*16字符串
        site.x= 12*8;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"%d",CD_speed);
        LCD_str(site,str_buf,FCOLOUR,cloor_table[4]);   //显示8*16字符串
        site.y += char_H;
        site.x = 3;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"kp");
        LCD_str(site,str_buf,FCOLOUR,cloor_table[2]);   //显示8*16字符串
        site.x= 12*8;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"%.1f",kpv);
        LCD_str(site,str_buf,FCOLOUR,cloor_table[5]);   //显示8*16字符串
        cloor_table[choose_num]=BCOLOUR;
        key_num=wait_key_down();
        if(key_num==KEY_D)
        {   
            if(choose_num<3)
            {
                choose_num++;
                if(choose_num == 3)
                  choose_num=0;
            }
            else  if(choose_num==3)
            {
                zhidao_speed-=2;
                if(zhidao_speed<CD_speed)
                  zhidao_speed=CD_speed;
            }
            else  if(choose_num==4)
            {
                CD_speed-=2;
                if(CD_speed>250)
                  CD_speed=0;
            }
            else  if(choose_num==5)
            {
                kpv-=1;
                kp_val=kpv;
            }
        }
        else if(key_num==KEY_U)
        {
            if(choose_num<3)
            {
                if(choose_num == 0)
                  choose_num=0;
                choose_num--;
            }
            else  if(choose_num==3)
            {
                zhidao_speed+=2;
                if(zhidao_speed>250)
                  zhidao_speed=250;
            }
            else  if(choose_num==4)
            {
                CD_speed+=2;
                if(CD_speed>zhidao_speed)
                  CD_speed=zhidao_speed;
            }
              else  if(choose_num==5)
            {
                kpv+=1;
                kp_val=kpv;
            }
        }
        else if(key_num==KEY_R)
        {
            if(choose_num<3)
              choose_num+=3;
        }
        else if(key_num==KEY_L)
        {
            if(choose_num<3)
            {
                choose_num=0;
                UI_state=MAIN_UI;
                break;
            }
            else
              choose_num=0;
        }
        else if(key_num==KEY_B)
        {
            choose_num=0;
            UI_state=MAIN_UI;
            break;
        }
        
    }
}
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
        LCD_str(site,str_buf,FCOLOUR,cloor_table[4]); 
        //----------------------------------------------
        site.y += char_H;
        site.x = 3;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"Ti");
        LCD_str(site,str_buf,FCOLOUR,cloor_table[1]);   //显示8*16字符串
        
        site.x= 10*8;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"%.1f",moto_system_Ti);
        LCD_str(site,str_buf,FCOLOUR,cloor_table[5]);   //显示8*16字符串
        //------------------------------------------------
        site.y += char_H;
        site.x = 3;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"Td");
        LCD_str(site,str_buf,FCOLOUR,cloor_table[2]);   //显示8*16字符串
        
        site.x= 10*8;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"%.1f",moto_system_Td);
        LCD_str(site,str_buf,FCOLOUR,cloor_table[6]);   //显示8*16字符串
        //-----------------------------------------------
        site.y += char_H;
        site.x = 3;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"n");
        LCD_str(site,str_buf,FCOLOUR,cloor_table[3]);   //显示8*16字符串
        
        site.x= 10*8;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"%.1f",change_n[n]);
        LCD_str(site,str_buf,FCOLOUR,cloor_table[7]);   //显示8*16字符串
        //--------------------------------------
        cloor_table[choose_num]=BCOLOUR;
        key_num=wait_key_down();

        if(key_num==KEY_U){   
            if(choose_num<4){
                if(choose_num == 0)
                  choose_num=4;
                choose_num--;
            }
            else  if(choose_num==4){
                moto_system_Kp += change_n[n];
                if(moto_system_Kp > 200)
                    moto_system_Kp=200;
            }
            else  if(choose_num==5){
                moto_system_Ti+=change_n[n];
                if(moto_system_Ti > 100)
                    moto_system_Ti=100;
            }
            else  if(choose_num==6){
                moto_system_Td+=change_n[n];
                if(moto_system_Td > 100)
                    moto_system_Td=100;
            }
            else  if(choose_num==7){
		n++;
                if(n > 4)
                    n=4;
            }
        }
        else if(key_num==KEY_D){   
            if(choose_num<4){
                choose_num++;
                if(choose_num == 4)
                  choose_num=0;
            }
            else  if(choose_num==4){
                moto_system_Kp-=change_n[n];
                if(moto_system_Kp<0)
                    moto_system_Kp=0;
            }
            else  if(choose_num==5){
                moto_system_Ti-=change_n[n];
                if(moto_system_Ti < 0)
                    moto_system_Ti=0;
            }
            else  if(choose_num==6){
                moto_system_Td-=change_n[n];
                if(moto_system_Td < 0)
                    moto_system_Td=0;
            }
            else  if(choose_num==7){
		n--;
                if(n < 0)
                    n=0;
            }
        }
        else if(key_num==KEY_R){
            if(choose_num<4)
              choose_num+=4;
        }
        else if(key_num==KEY_L){
            if(choose_num<4){
                choose_num=0;
                UI_state=MAIN_UI;
                break;
            }
            else
              choose_num-=4;
        }
        else if(key_num==KEY_B){
            choose_num=0;
            UI_state=MAIN_UI;
            break;
        }
    }
}

void open_img_ui()
{
    Site_t site     = {0, 0}; 
    Size_t size;
    size.H= CAMERA_H;
    size.W = CAMERA_W;
    Size_t imgsize  = {CAMERA_W, CAMERA_H};             //图像大小
    my_debug_flag=1;
    while(key_check(KEY_L)!=KEY_DOWN&&key_check(KEY_B)!=KEY_DOWN)
    {
        camera_get_img();                                   //摄像头获取图像 
        LCD_rectangle(site, size, BCOLOUR);     //初始化背景
        LCD_Img_Binary_Z(site, size, imgbuff, imgsize);//显示二值化图像(可缩放)
        draw_mark_line();//在图像上面划红线（便于看图像）
//        ctrl_main();//控制主函数
        
    }
    UI_state=MAIN_UI;
    my_debug_flag=0;
}
void my_UI()
{
    while(UI_state<5)
    {
        printf("%f\n%f\n%f\n",moto_system_Td,moto_system_Ti,moto_system_Kp);
        switch(UI_state)
        {
        case MAIN_UI:main_ui();break;
        case SET_SPEED_UI:set_speed_ui();break;
        case CHANGE_PID_UI:change_pid_ui();break;
        case OPEN_IMG_UI:open_img_ui();break;
        }
    }
    LCD_clear(RED);
}
