
#ifndef _USART1_H
#define _USART1_H

#ifdef __cplusplus
extern "C" {
#endif



void USART0SendByte(unsigned char SendData);
unsigned char USART0GetByte(unsigned char* GetData);
void USART0TX(char a[100]);
void USART0Test(int *a);

#ifdef __cplusplus
}
#endif

#endif // _USART1_H
