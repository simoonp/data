
uint16 cloor_table[8]={BCOLOUR,BCOLOUR,BCOLOUR,BCOLOUR,BCOLOUR,BCOLOUR,BCOLOUR,BCOLOUR};
// typedef enum
// {
    // MAIN_UI  =   0,         //按键按下时对应电平
    // SET_SPEED_UI    =   1,         //按键弹起时对应电平
    // IMAGE_UI,               //长按按键(用于定时按键扫描)
    // SET_IMG_UI,
    // OPEN_IMG_UI,
// } UI_STATUS_e;

void change_pid()
{
	Site_t site;
	uint8 str_buf[STR_BUF_LEN];
    int n;
    n = 1;
    uint8 str_buf[STR_BUF_LEN];
    uint8 char_H=22,key_num;
    Site_t site = {3,5};
    int pid_p = 10;
    int pid_i = 10;
    int pid_d = 10;
    while(1){
        site.y = 5;
        site.x=3; 
        cloor_table[choose_num]=RED;
        LCD_clear(BCOLOUR);
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"Kp:");
        LCD_str(site,str_buf,FCOLOUR,cloor_table[0]);

        site.x += 12*8;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"%d",pid.Kp);
        LCD_str(site,str_buf,FCOLOUR,cloor_table[4]); 
        //----------------------------------------------
        site.y += char_H;
        site.x = 3;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"Ti");
        LCD_str(site,str_buf,FCOLOUR,cloor_table[1]);   //显示8*16字符串
        
        site.x= 12*8;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"%d",pid.Ti);
        LCD_str(site,str_buf,FCOLOUR,cloor_table[5]);   //显示8*16字符串
        //------------------------------------------------
        site.y += char_H;
        site.x = 3;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"Td");
        LCD_str(site,str_buf,FCOLOUR,cloor_table[2]);   //显示8*16字符串
        
        site.x= 12*8;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"%d",pid.Td);
        LCD_str(site,str_buf,FCOLOUR,cloor_table[6]);   //显示8*16字符串
        //-----------------------------------------------
        site.y += char_H;
        site.x = 3;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"n");
        LCD_str(site,str_buf,FCOLOUR,cloor_table[3]);   //显示8*16字符串
        
        site.x= 12*8;
        memset(str_buf,0,STR_BUF_LEN);
        sprintf((char *)str_buf,"%d",n);
        LCD_str(site,str_buf,FCOLOUR,cloor_table[7]);   //显示8*16字符串
        //--------------------------------------
        cloor_table[choose_num]=BCOLOUR;
        key_num=wait_key_down();

        if(key_num==KEY_U)
        {   
            if(choose_num<4)
            {
                choose_num--;
                if(choose_num>8)
                  choose_num=2;
            }
            else  if(choose_num==4)
            {
                pid.Kp += 5;
                if(pid.Kp > 200)
                    pid.Kp=200;
            }
            else  if(choose_num==5)
            {
                pid.Ti+=3;
                if(pid.Ti > 100)
                    pid.Ti=100;
            }
            else  if(choose_num==6)
            {
                pid.Td+=6;
                if(pid.Td > 100)
                    pid.Td=100;
            }
            else  if(choose_num==7)
            {
                n+=1;
                if(n > 5)
                    n=5;
            }
        }
        else if(key_num==KEY_D)
        {   
            if(choose_num<4)
            {
                choose_num++;
                if(choose_num == 4)
                  choose_num=0;
            }
            else  if(choose_num==4)
            {
                pid.Kp-=5;
                if(pid.Kp<0)
                    pid.Kp=0;
            }
            else  if(choose_num==5)
            {
                pid.Ti-=3;
                if(pid.Ti < 0)
                    pid.Ti=0;
            }
            else  if(choose_num==6)
            {
                pid.Td-=6;
                if(pid.Td < 0)
                    pid.Td=0;
            }
            else  if(choose_num==7)
            {
                n -= 1;
                if(n < 0)
                    n=0;
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
//        else if(key_num==KEY_B)
//        {
//            choose_num=0;
//            UI_state=MAIN_UI;
//            break;
//        }
    }
}