#include "common.h"
#include "include.h"
int handler_flag;
void uart4_handler(void)
{
    char ch;
    UARTn_e uratn = UART4;

    if(UART_S1_REG(UARTN[uratn]) & UART_S1_RDRF_MASK){   //接收数据寄存器满
        uart_getchar (UART4,&ch);   
        if (ch == 'y'){
            handler_flag=0;
            led (LED1,LED_ON);
            led (LED0,LED_ON);
            ftm_pwm_duty(FTM0, FTM_CH3,00);
            ftm_pwm_duty(FTM0, FTM_CH2,0);
        }else if (ch == 'n'){
            handler_flag=1;
            ftm_pwm_duty(FTM0, FTM_CH3,0);
            ftm_pwm_duty(FTM0, FTM_CH2,0);
            led (LED1,LED_OFF);
            led (LED0,LED_OFF);
        }else if( ch == 'a'){
            printf("\n**中断中**\n"); 
            handler_flag=0;
            led (LED0,LED_ON);
            ftm_pwm_duty(FTM0, FTM_CH3,00);
            ftm_pwm_duty(FTM0, FTM_CH2,0);
        } else if( ch == 'b'){
            handler_flag=0;
            ftm_pwm_duty(FTM0, FTM_CH3,0);
            ftm_pwm_duty(FTM0, FTM_CH2,0);
            led (LED0,LED_OFF);
        } else if( ch == 'c'){
            handler_flag=0;
            led (LED1,LED_ON);
            ftm_pwm_duty(FTM0, FTM_CH3,0);
            ftm_pwm_duty(FTM0, FTM_CH2,0);
        } else if( ch == 'd'){
            handler_flag=0;
            led (LED1,LED_OFF);
            ftm_pwm_duty(FTM0, FTM_CH3,0);
            ftm_pwm_duty(FTM0, FTM_CH2,0);
        }
     }
}
void main (void)
{	
	led_init (LED0);
	led_init (LED1);
	led_init (LED3);
    uart_init(UART4,38400);     

    uart_putstr   (UART4 ,"\n\n\n接收中断测试：");           //发送字符串
    
    NVIC_SetPriorityGrouping(2); 
    NVIC_SetPriority(UART4_RX_TX_IRQn,0);         //配置优先级

    set_vector_handler(UART4_RX_TX_VECTORn,uart4_handler);   // 设置中断服务函数到中断向量表里
    enable_irq(UART4_RX_TX_IRQn);
    uart_rx_irq_en (UART4); 
    
    printf("\n***** 测试*****\n");

    ftm_pwm_init(FTM0, FTM_CH3,10*1000,0);          //初始化 FTM PWM ，使用 FTM0_CH3，频率为10k ，占空比为 100 / FTM0_PRECISON
                                                    // FTM0_PRECISON 配置 为 100 ，即占空比 为 100%
                                                    // port_cfg.h 里 配置 FTM0_CH3 对应为 PTA6
    gpio_init(PTD15,GPO,0);                         // 使能端 输入为 0
    
    ftm_pwm_init(FTM0, FTM_CH2,10*1000,0);


    while(1)
    {
		led_turn (LED3);
        DELAY_MS(500);
		printf("\n***** 测试中****\n");
		while(handler_flag)
            ;
    }
}
