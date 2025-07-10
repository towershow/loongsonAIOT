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

//等待DHT11回应，返回1：未检测到，返回0成功
uint8_t DHT11_Check(void){
    uint8_t retry=0;
    gpio_set_direction(DHT11_IO,0);//输入状态
    while((gpio_get_pin(DHT11_IO)==1)&&retry<100)//增加重试次数以应对更加严格的检查
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

//DHT11初始化
uint8_t DHT11_Init(void){
    gpio_write_pin(DHT11_IO,1);
    delay_ms(30);
    //初始化引脚状态
    DHT11_RST();//复位，发出起始信号
    return DHT11_Check();//等待DHT11回应
}

//从DHT11读取一个位
uint8_t DHT11_Read_Bit(void){
    uint8_t retry=0;
    while((gpio_get_pin(DHT11_IO)== 1) && retry < 100) //等待变为低电平100
    {
        retry++;
        delay_us(1);
    }
    retry = 0;
    while((gpio_get_pin(DHT11_IO) == 0) && retry < 100) //等待变高电平100
    {
        retry++;
        delay_us(1);
    }
    delay_us(15);//等待40us
    if(gpio_get_pin(DHT11_IO) == 1)       //用于判断高低电平，即数据1或0
        return 1;
    else
        return 0;
}

//从DHT11读取一个字节
//返回值：读到的数据
uint8_t DHT11_Read_Byte(void)
{
    uint8_t i, dat;
    dat = 0;
    for (i = 0; i < 8; i++)
    {
        dat <<= 1;					//左移运算符,dat左移1位
        dat |= DHT11_Read_Bit();	//"|"表示按位或等于
    }
    return dat;
}

//从DHT11读取一次数据
//temp:温度值(范围:0~50°)
//humi:湿度值(范围:20%~90%)
//返回值：0,正常;1,读取失败
uint8_t DHT11_Read_Data(uint8_t *temp, uint8_t *humi)
{
    uint8_t buf[5];
    uint8_t i;
    DHT11_RST();						//DHT11端口复位，发出起始信号
    if(DHT11_Check() == 0)				//等待DHT11回应，0为成功回应
    {
        for(i = 0; i < 5; i++) 			//读取40位数据
        {
            
            buf[i] = DHT11_Read_Byte();	//读出数据
        }
        if((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4] )	//数据校验
        {

            *humi = buf[0];				//将湿度值放入指针humi
            *temp = buf[2];				//将温度值放入指针temp
            return 0;
        }
    }
    return 1;

}
