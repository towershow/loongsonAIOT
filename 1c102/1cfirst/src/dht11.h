#ifndef _DHT11_H
#define _DHT11_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ls1x.h"

void DHT11_RST(void);
uint8_t DHT11_Init(void);//初始化DHT11
uint8_t DHT11_Read_Data(unsigned char *temp,unsigned char *humi);//读取温湿度
uint8_t DHT11_Read_Byte(void);//读出一个字节
uint8_t DHT11_Read_Bit(void);//读出一个位
uint8_t DHT11_Check(void);//检测是否存在DHT11

#ifdef __cplusplus
}
#endif

#endif // _DHT11_H





