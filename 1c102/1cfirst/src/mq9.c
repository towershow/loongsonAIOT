#include "mq9.h"
#include "ls1x_latimer.h"
#include "ls1x_gpio.h"
#include "ls1c102_adc.h"

//MQ9等效模型，近似

#define CAL_PPM  10  // 校准环境中PPM值
#define RL	     10  // RL阻值
#define R0	     26  // R0阻值


//AO-gpio15(adci5),DO-gpio13,DO输出数字信号用于超出阈值后拉高拉低，AO输出模拟信号，利用电压电源将其转换



//读取MQ9传感器的电压值

unsigned short MQ9_Get_Vol(void){
    unsigned short voltage;
    voltage =  3*100*Adc_Measure(ADC_CHANNEL_I5)/4096;//放大了100倍
    return voltage;
}


//计算somg的PPM值，input放大了1000倍
 int Smog_GetPPM(unsigned short vol){
    int RS;
    RS=(3300-vol)/100;
    RS=RS*10/26;
    int ppm = 98*RS/10;
    return ppm;
}

