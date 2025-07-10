#include "uart0.h"
#include "ls1x_uart.h"



//发送数据
void USART0SendByte(unsigned char SendData){
    Uart0_send(SendData);
}

//接受数据
unsigned char USART0GetByte(unsigned char* GetData){
    if(UART_GetFlagStatus(UART0,UART_LSR_FLAG_RXNE)==RESET){
        return 0;
    }
    *GetData=UART_ReceiveData(UART0);
    return 1;
}
//检查是否为OK
void USART0Test(int *a){
    int state = 0;
    unsigned char i=0;
    while(1){
        while(USART0GetByte(&i)){
            USART0SendByte(i);
        }
        if(i=='O'){
            state = 1;
        }
        if(i=='K'&&state==1)       
        {
            *a=1;
            break;
        }
    }
}

//发送字符
void USART0TX(char a[100])
{
		int i = 0;
		while(a[i] != '\0')
		{
				USART0SendByte(a[i]);
				i++;
		}
		USART0SendByte('\r');
		USART0SendByte('\n');
}