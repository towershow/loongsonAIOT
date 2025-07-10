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


//AUX��37
void AUX_Config(void){
    gpio_set_direction(37,0);
}

//����loraģ��
void MD0(int a){
    if(a){
        gpio_write_pin(35,1);
    }
    else gpio_write_pin(35,0);
}


//ģ���Ƿ����
int AUX(void){
    if(gpio_get_pin(39)==1){
        return 1;
    }
    else
        return 0;
}

//ģ���Ⲣ����AT
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
    /*################################���õ�ַ###############################*/
    int OK_state = 0;
	char addr[20] = {0};int t1 = 0,t2 = 0;
	unsigned char i = 0;
	while(AUX())//�ȴ�ģ�����
	{
		 printf("Lora is busy two");
	}
	MD0(1);
	delay_ms(40);
	while(!OK_state)//ģ����ɹ�
	{
		USART0TX("AT+ADDR=02,65");//���õ�ַ
		USART0Test(&OK_state);
		printf("��ַ������......");
	}
	printf("��ַΪ��");
	USART0TX("AT+ADDR?");
	while(1)                       //���������Ƿ���ȷ
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

    /*##############################�����ŵ�������################################*/
	OK_state = 0;
	t1 = 0;
	t2 = 0;

	while(!OK_state)//ģ����ɹ�
	{
		USART0TX("AT+WLRATE=23,5");
		USART0Test(&OK_state);
		printf("�ŵ�����������......");
	}
	printf("�ŵ�������Ϊ��");
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
	
	/*##############################���书��################################*/
	OK_state = 0;
	t1 = 0;
	t2 = 0;

	while(!OK_state)//ģ����ɹ�
	{
		USART0TX("AT+TPOWER=3");
		USART0Test(&OK_state);
		printf("���书��������......");
	}
	printf("���书��Ϊ��");
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
	/*##############################����ģʽ################################*/
	OK_state = 0;
	t1 = 0;
	t2 = 0;

	while(!OK_state)//ģ����ɹ�
	{
		USART0TX("AT+CWMODE=1");
		USART0Test(&OK_state);
		printf("����ģʽ������......");
	}
	printf("����ģʽΪ��");
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
	/*##############################����״̬################################*/
	OK_state = 0;
	t1 = 0;
	t2 = 0;

	while(!OK_state)//ģ����ɹ�
	{
		USART0TX("AT+TMODE=0");//0��͸�����䣬1��������
		USART0Test(&OK_state);
		printf("����״̬������......");
	}
	printf("����״̬Ϊ��");
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
	/*##############################˯��ʱ��################################*/
	OK_state = 0;
	t1 = 0;
	t2 = 0;

	while(!OK_state)//ģ����ɹ�
	{
		USART0TX("AT+WLTIME=0");
		USART0Test(&OK_state);
		printf("˯��ʱ��������......");
	}
	printf("˯��ʱ��Ϊ��");
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

	/*##############################�����ʣ�����У��λ################################*/
	OK_state = 0;
	t1 = 0;
	t2 = 0;

	while(!OK_state)//ģ����ɹ�
	{
		USART0TX("AT+UART=3,0");
		USART0Test(&OK_state);
		printf("�����ʣ�����У��λ������......");
	}
	printf("�����ʣ�����У��λΪ��");
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



	printf("LORA�������");
}






void lora_transmit(char*a){
    int i=0;
    Uart0_send(MASTER_LORA_ADDH);
    Uart0_send(MASTER_LORA_ADDL);
	Uart0_send(MASTER_LORA_CHANNEL);
	//Uart0_send('#');//��ʼ��

	while(a[i] != '\0')
		{
				Uart0_send(a[i]);
				i++;
		}
		//USART1SendByte('2');//�ڵ�2��־�������ն�����
		//Uart0_send('$');//������
		//USART1SendByte('1');//�ڵ�1��־�������ն�����
		//USART1SendByte('2');//�ڵ�2��־�������ն�����
		//Uart0_send('\r');
		//Uart0_send('\n');
}


void LORA_Init(void)
{
		MD0_Config(); //��ʼ��MD0 
		AUX_Config();
		lora_check();
		lora_set();   //����LORA


}
