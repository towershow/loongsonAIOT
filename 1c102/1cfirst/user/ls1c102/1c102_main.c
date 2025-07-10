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


uint8_t Read_Buffer[DATA_LEN];//���ý��ջ�������
uint8_t Read_length;
char lora_tx_data[100];
int flagclear=0;

int main(int arg, char *args[])
{    
    LORA_Init(); 

    Spi_Init(SPI_DIV_2);
    OLED_Init();// ��ʼ�� OLED ģ��
    OLED_Clear();// OLED

    AFIO_RemapConfig(AFIOB, GPIO_Pin_4, 0);
    Adc_powerOn();//adc��Դ����
    Adc_open(ADC_CHANNEL_I5);//��ͨ��0

    uint8_t humi,temp;
    int value=0,ppm=0;
    unsigned short vol=0;

    while(1)
    {
        //��������������
        DHT11_Read_Data(&temp,&humi);

        value=Adc_Measure(ADC_CHANNEL_I5);
        vol = MQ9_Get_Vol();
        ppm=Smog_GetPPM(vol);//ppm���Ʋ��Ŵ�1000��

        sprintf(lora_tx_data,"%d%d%d%d",1,temp,humi,ppm);//�����ںϣ���1��Ϊ�豸��

        OLED_ShowString(0,16,"temp:");
        OLED_ShowNum(40,16,temp,2,16);
        OLED_ShowCHinese( 58, 16, 0);
        OLED_ShowString(0,32,"humi:");
        OLED_ShowNum(40,32,humi,2,16);
        OLED_ShowString(56,32,"%");
        OLED_ShowString(0,48,"ppm:");
        OLED_ShowNum(32,48,ppm,3,16);

        //�ӻ���Ӧ�����ݷ���

        if(Queue_isEmpty(&Circular_queue)==0)//�ж϶����Ƿ�Ϊ�գ����ж��Ƿ��յ�����
        {
            Read_length=Queue_HadUse(&Circular_queue);//���ض��������ݵĳ���
            memset(Read_Buffer, 0, DATA_LEN);//�����ջ�����Ϊ0
            Queue_Read(&Circular_queue, Read_Buffer,Read_length);//��ȡ���л�������ֵ�����ջ�����
            Read_Buffer[Read_length]='\0';//�ַ������ս�����

            if(strncmp(Read_Buffer,"ON",2)==0)
            {
                 lora_transmit(lora_tx_data);
                 //�ӻ�OLED��ʾ����
            }
            if(strncmp(Read_Buffer,"OFF",3)==0)
            {
                OLED_ShowString(0,0,"stop");
            }
        }

        //���ڴӻ��ı�������
        if(temp>32||humi>80||ppm>50000)
        {
            OLED_ShowString(0,0,"DANGEROUS");
            gpio_write_pin(BUZZER,0);//�͵�ƽ����
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

        
        delay_ms(1000);//��������ʱ����ڷ������ݣ�����ѭ��ִ������
    }

    return 0;
}
