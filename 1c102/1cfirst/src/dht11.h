#ifndef _DHT11_H
#define _DHT11_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ls1x.h"

void DHT11_RST(void);
uint8_t DHT11_Init(void);//��ʼ��DHT11
uint8_t DHT11_Read_Data(unsigned char *temp,unsigned char *humi);//��ȡ��ʪ��
uint8_t DHT11_Read_Byte(void);//����һ���ֽ�
uint8_t DHT11_Read_Bit(void);//����һ��λ
uint8_t DHT11_Check(void);//����Ƿ����DHT11

#ifdef __cplusplus
}
#endif

#endif // _DHT11_H





