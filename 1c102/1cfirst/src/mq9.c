#include "mq9.h"
#include "ls1x_latimer.h"
#include "ls1x_gpio.h"
#include "ls1c102_adc.h"

//MQ9��Чģ�ͣ�����

#define CAL_PPM  10  // У׼������PPMֵ
#define RL	     10  // RL��ֵ
#define R0	     26  // R0��ֵ


//AO-gpio15(adci5),DO-gpio13,DO��������ź����ڳ�����ֵ���������ͣ�AO���ģ���źţ����õ�ѹ��Դ����ת��



//��ȡMQ9�������ĵ�ѹֵ

unsigned short MQ9_Get_Vol(void){
    unsigned short voltage;
    voltage =  3*100*Adc_Measure(ADC_CHANNEL_I5)/4096;//�Ŵ���100��
    return voltage;
}


//����somg��PPMֵ��input�Ŵ���1000��
 int Smog_GetPPM(unsigned short vol){
    int RS;
    RS=(3300-vol)/100;
    RS=RS*10/26;
    int ppm = 98*RS/10;
    return ppm;
}

