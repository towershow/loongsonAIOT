#include "ls1x.h"
#include "Config.h"
#include "ls1x_spi.h"
#include "ls1x_gpio.h"
#include "ls1c102_adc.h"
#include "ls1x_latimer.h"
#include "ls1c102_interrupt.h"
#include "ls1x_printf.h"
#include "ls1x_string.h"
#include "ls1c102_ptimer.h"
#include "queue.h"


#include "oled.h"
#include "dht11.h"
#include "lora.h"
#include "mq9.h"

#define BUZZER 34


uint8_t Read_Buffer[DATA_LEN];//设置接收缓冲数组
uint8_t Read_length;
char lora_tx_data[100];
int flagclear=0;

int main(int arg, char *args[])
{    
    LORA_Init(); 

    Spi_Init(SPI_DIV_2);
    OLED_Init();// 初始化 OLED 模块
    OLED_Clear();// OLED

    AFIO_RemapConfig(AFIOB, GPIO_Pin_4, 0);
    Adc_powerOn();//adc电源开启
    Adc_open(ADC_CHANNEL_I5);//打开通道0

    uint8_t humi,temp;
    int value=0,ppm=0;
    unsigned short vol=0;

    while(1)
    {
        //传感器参数测量
        DHT11_Read_Data(&temp,&humi);

        value=Adc_Measure(ADC_CHANNEL_I5);
        vol = MQ9_Get_Vol();
        ppm=Smog_GetPPM(vol);//ppm估计并放大1000倍

        sprintf(lora_tx_data,"%d%d%d%d",1,temp,humi,ppm);//数据融合，“1”为设备号

        OLED_ShowString(0,16,"temp:");
        OLED_ShowNum(40,16,temp,2,16);
        OLED_ShowCHinese( 58, 16, 0);
        OLED_ShowString(0,32,"humi:");
        OLED_ShowNum(40,32,humi,2,16);
        OLED_ShowString(56,32,"%");
        OLED_ShowString(0,48,"ppm:");
        OLED_ShowNum(32,48,ppm,3,16);

        //从机响应和数据发送

        if(Queue_isEmpty(&Circular_queue)==0)//判断队列是否为空，即判断是否收到数据
        {
            Read_length=Queue_HadUse(&Circular_queue);//返回队列中数据的长度
            memset(Read_Buffer, 0, DATA_LEN);//填充接收缓冲区为0
            Queue_Read(&Circular_queue, Read_Buffer,Read_length);//读取队列缓冲区的值到接收缓冲区
            Read_Buffer[Read_length]='\0';//字符串接收结束符

            if(strncmp(Read_Buffer,"ON",2)==0)
            {
                 lora_transmit(lora_tx_data);
                 //从机OLED显示数据
            }
            if(strncmp(Read_Buffer,"OFF",3)==0)
            {
                OLED_ShowString(0,0,"stop");
            }
        }

        //关于从机的报警机制
        if(temp>32||humi>80||ppm>50000)
        {
            OLED_ShowString(0,0,"DANGEROUS");
            gpio_write_pin(BUZZER,0);//低电平触发
            flagclear=1;
        }
        else 
        {
            if(flagclear == 1)
            {
                OLED_Clear();
                flagclear=0;
            }
            OLED_ShowString(0,0,"SAFETY");
            gpio_write_pin(BUZZER,1);
        }

        
        delay_ms(1000);//处理数据时间大于发送数据，限制循环执行速率
    }

    return 0;
}
