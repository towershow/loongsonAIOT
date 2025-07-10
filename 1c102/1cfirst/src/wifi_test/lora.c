#include "lora.h"
#include "ls1x_uart.h"
#include "ls1x_printf.h"
#include "ls1X_gpio.h"
#include "Config.h"
#include "uart0.h"
#include "ls1x_latimer.h"

void MD0_Config(void){
    gpio_write_pin(35,0);
}


//AUX接37
void AUX_Config(void){
    gpio_set_direction(37,0);
}

//激活lora模块
void MD0(int a){
    if(a){
        gpio_write_pin(35,1);
    }
    else gpio_write_pin(35,0);
}


//模块是否空闲
int AUX(void){
    if(gpio_get_pin(39)==1){
        return 1;
    }
    else
        return 0;
}

//模块检测并发送AT
void lora_check(void){
    int OK_state=0;
    while(AUX()){
        printf("Lora is busy");
        delay_ms(500);
    }
    MD0(1);
    delay_ms(40);
    printf("lora is examing");
    while(!OK_state){
        USART0TX("AT");
        USART0Test(&OK_state);
    }
    printf("lora is good");
}

void lora_set(void){
    /*################################设置地址###############################*/
    int OK_state = 0;
	char addr[20] = {0};int t1 = 0,t2 = 0;
	unsigned char i = 0;
	while(AUX())//等待模块空闲
	{
		 printf("Lora is busy two");
	}
	MD0(1);
	delay_ms(40);
	while(!OK_state)//模块检测成功
	{
		USART0TX("AT+ADDR=02,65");//设置地址
		USART0Test(&OK_state);
		printf("地址设置中......");
	}
	printf("地址为：");
	USART0TX("AT+ADDR?");
	while(1)                       //检验设置是否正确
	{
			USART0GetByte(&i);
			if(i==':')
			{
				while(1)
				{
					if(USART0GetByte(&i) == 1 && i!='O')
					{
						addr[t1++]=i;
					}
					else if(i=='O')
						break;
				}
				while(1)
				{
						printf("$s",addr[t2++]);
						if(t1 == t2)
							break;
				}
				break;
			}
	}
	printf("  ");

    /*##############################设置信道和速率################################*/
	OK_state = 0;
	t1 = 0;
	t2 = 0;

	while(!OK_state)//模块检测成功
	{
		USART0TX("AT+WLRATE=23,5");
		USART0Test(&OK_state);
		printf("信道速率设置中......");
	}
	printf("信道，速率为：");
	USART0TX("AT+WLRATE?");
	while(1)
	{
			USART0GetByte(&i);
			if(i==':')
			{
				while(1)
				{
					if(USART0GetByte(&i) == 1 && i!='O')
					{
						addr[t1++]=i;
					}
					else if(i=='O')
						break;
				}
				while(1)
				{
						printf("$s",addr[t2++]);
						if(t1 == t2)
							break;
				}
				break;
			}
	}
	printf("  ");
	
	/*##############################发射功率################################*/
	OK_state = 0;
	t1 = 0;
	t2 = 0;

	while(!OK_state)//模块检测成功
	{
		USART0TX("AT+TPOWER=3");
		USART0Test(&OK_state);
		printf("发射功率设置中......");
	}
	printf("发射功率为：");
	USART0TX("AT+TPOWER?");
	while(1)
	{
			USART0GetByte(&i);
			if(i==':')
			{
				while(1)
				{
					if(USART0GetByte(&i) == 1 && i!='O')
					{
						addr[t1++]=i;
					}
					else if(i=='O')
						break;
				}
				while(1)
				{
						printf("$s",addr[t2++]);
						if(t1 == t2)
							break;
				}
				break;
			}
	}
	printf("  ");
	/*##############################工作模式################################*/
	OK_state = 0;
	t1 = 0;
	t2 = 0;

	while(!OK_state)//模块检测成功
	{
		USART0TX("AT+CWMODE=1");
		USART0Test(&OK_state);
		printf("工作模式设置中......");
	}
	printf("工作模式为：");
	USART0TX("AT+CWMODE?");
	while(1)
	{
			USART0GetByte(&i);
			if(i==':')
			{
				while(1)
				{
					if(USART0GetByte(&i) == 1 && i!='O')
					{
						addr[t1++]=i;
					}
					else if(i=='O')
						break;
				}
				while(1)
				{
						printf("$s",addr[t2++]);
						if(t1 == t2)
							break;
				}
				break;
			}
	}
	printf("  ");
	/*##############################发送状态################################*/
	OK_state = 0;
	t1 = 0;
	t2 = 0;

	while(!OK_state)//模块检测成功
	{
		USART0TX("AT+TMODE=0");//0：透明传输，1：定向传输
		USART0Test(&OK_state);
		printf("发送状态设置中......");
	}
	printf("发送状态为：");
	USART0TX("AT+TMODE?");
	while(1)
	{
			USART0GetByte(&i);
			if(i==':')
			{
				while(1)
				{
					if(USART0GetByte(&i) == 1 && i!='O')
					{
						addr[t1++]=i;
					}
					else if(i=='O')
						break;
				}
				while(1)
				{
						printf("$s",addr[t2++]);
						if(t1 == t2)
							break;
				}
				break;
			}
	}
	printf("  ");
	/*##############################睡眠时间################################*/
	OK_state = 0;
	t1 = 0;
	t2 = 0;

	while(!OK_state)//模块检测成功
	{
		USART0TX("AT+WLTIME=0");
		USART0Test(&OK_state);
		printf("睡眠时间设置中......");
	}
	printf("睡眠时间为：");
	USART0TX("AT+WLTIME?");
	while(1)
	{
			USART0GetByte(&i);
			if(i==':')
			{
				while(1)
				{
					if(USART0GetByte(&i) == 1 && i!='O')
					{
						addr[t1++]=i;
					}
					else if(i=='O')
						break;
				}
				while(1)
				{
						printf("$s",addr[t2++]);
						if(t1 == t2)
							break;
				}
				break;
			}
	}
	printf("  ");

	/*##############################波特率，数据校验位################################*/
	OK_state = 0;
	t1 = 0;
	t2 = 0;

	while(!OK_state)//模块检测成功
	{
		USART0TX("AT+UART=3,0");
		USART0Test(&OK_state);
		printf("波特率，数据校验位设置中......");
	}
	printf("波特率，数据校验位为：");
	USART0TX("AT+UART?");
	while(1)
	{
			USART0GetByte(&i);
			if(i==':')
			{
				while(1)
				{
					if(USART0GetByte(&i) == 1 && i!='O')
					{
						addr[t1++]=i;
					}
					else if(i=='O')
						break;
				}
				while(1)
				{
						printf("$s",addr[t2++]);
						if(t1 == t2)
							break;
				}
				break;
			}
	}
	printf("  ");

	MD0(0);
	delay_ms(40);
	while(AUX());



	printf("LORA配置完成");
}






void lora_transmit(char*a){
    int i=0;
    Uart0_send(MASTER_LORA_ADDH);
    Uart0_send(MASTER_LORA_ADDL);
	Uart0_send(MASTER_LORA_CHANNEL);
	//Uart0_send('#');//起始符

	while(a[i] != '\0')
		{
				Uart0_send(a[i]);
				i++;
		}
		//USART1SendByte('2');//节点2标志，用来终端区分
		//Uart0_send('$');//结束符
		//USART1SendByte('1');//节点1标志，用来终端区分
		//USART1SendByte('2');//节点2标志，用来终端区分
		//Uart0_send('\r');
		//Uart0_send('\n');
}


void LORA_Init(void)
{
		MD0_Config(); //初始化MD0 
		AUX_Config();
		lora_check();
		lora_set();   //配置LORA


}
