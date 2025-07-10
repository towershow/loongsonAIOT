#include "dht11.h"


#include "ls1x_gpio.h"
#include "ls1x_latimer.h"

#define DHT11_IO 19

void DHT11_RST(void){
    gpio_write_pin(DHT11_IO, 0);
    delay_ms(20);
    gpio_write_pin(DHT11_IO, 1);
    delay_us(10);
}

//�ȴ�DHT11��Ӧ������1��δ��⵽������0�ɹ�
uint8_t DHT11_Check(void){
    uint8_t retry=0;
    gpio_set_direction(DHT11_IO,0);//����״̬
    while((gpio_get_pin(DHT11_IO)==1)&&retry<100)//�������Դ�����Ӧ�Ը����ϸ�ļ��
    {
        retry++;
        delay_us(1);
    }
    if(retry>=100) return 1;
    else retry=0;
    while((gpio_get_pin(DHT11_IO)==0)&&retry<100){
        retry++;
        delay_us(2);
    }
    if(retry>=100) return 1;
    return 0;
}

//DHT11��ʼ��
uint8_t DHT11_Init(void){
    gpio_write_pin(DHT11_IO,1);
    delay_ms(30);
    //��ʼ������״̬
    DHT11_RST();//��λ��������ʼ�ź�
    return DHT11_Check();//�ȴ�DHT11��Ӧ
}

//��DHT11��ȡһ��λ
uint8_t DHT11_Read_Bit(void){
    uint8_t retry=0;
    while((gpio_get_pin(DHT11_IO)== 1) && retry < 100) //�ȴ���Ϊ�͵�ƽ100
    {
        retry++;
        delay_us(1);
    }
    retry = 0;
    while((gpio_get_pin(DHT11_IO) == 0) && retry < 100) //�ȴ���ߵ�ƽ100
    {
        retry++;
        delay_us(1);
    }
    delay_us(15);//�ȴ�40us
    if(gpio_get_pin(DHT11_IO) == 1)       //�����жϸߵ͵�ƽ��������1��0
        return 1;
    else
        return 0;
}

//��DHT11��ȡһ���ֽ�
//����ֵ������������
uint8_t DHT11_Read_Byte(void)
{
    uint8_t i, dat;
    dat = 0;
    for (i = 0; i < 8; i++)
    {
        dat <<= 1;					//���������,dat����1λ
        dat |= DHT11_Read_Bit();	//"|"��ʾ��λ�����
    }
    return dat;
}

//��DHT11��ȡһ������
//temp:�¶�ֵ(��Χ:0~50��)
//humi:ʪ��ֵ(��Χ:20%~90%)
//����ֵ��0,����;1,��ȡʧ��
uint8_t DHT11_Read_Data(uint8_t *temp, uint8_t *humi)
{
    uint8_t buf[5];
    uint8_t i;
    DHT11_RST();						//DHT11�˿ڸ�λ��������ʼ�ź�
    if(DHT11_Check() == 0)				//�ȴ�DHT11��Ӧ��0Ϊ�ɹ���Ӧ
    {
        for(i = 0; i < 5; i++) 			//��ȡ40λ����
        {
            
            buf[i] = DHT11_Read_Byte();	//��������
        }
        if((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4] )	//����У��
        {

            *humi = buf[0];				//��ʪ��ֵ����ָ��humi
            *temp = buf[2];				//���¶�ֵ����ָ��temp
            return 0;
        }
    }
    return 1;

}
