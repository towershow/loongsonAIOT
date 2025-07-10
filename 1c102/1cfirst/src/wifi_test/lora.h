#ifndef _LORA_H
#define _LORA_H

#ifdef __cplusplus
extern "C" {
#endif

#define MASTER_LORA_ADDH 0x02					//主机地址及信道
#define MASTER_LORA_ADDL 0x67
#define SLAVE1_LORA_ADDL 0x01
#define SLAVE2_LORA_ADDL 0x02
#define MASTER_LORA_CHANNEL 0x17



void MD0_Config(void);//MD0初始化
void AUX_Config(void);		
int AUX(void);//返回高低电平					
void lora_check(void);
void lora_set(void);				
void lora_transmit(char* a);
void LORA_Init(void);
					

#ifdef __cplusplus
}
#endif

#endif // _LORA_H

