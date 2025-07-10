// ================================================ ��Դ���� ================================================ //
//   VCC         ��          DC 5V/3.3V      // OLED ����Դ��
//   GND         ��          GND             // OLED ����Դ��
// ================================================ Һ���������߽��� ================================================ //
// ��ģ��Ĭ��������������Ϊ4����SPI
//   OLED ģ��
//   D0          ��          GPIO53          // OLED �� SPI ʱ���ź�           // Ӳ�� SPI CLK
//   D1/MOSI     ��          GPIO55          // OLED �� SPI д�ź�             // Ӳ�� SPI MOSI
//   CS          ��          GPIO56          // OLED ��Ƭѡ�����ź�            // Ӳ�� SPI CSN0
//   RES         ��          GPIO40          // OLED ����λ�����źš�          // ��ͨ GPIO
//   DC          ��          GPIO39          // OLED ������/����ѡ������ź�   // ��ͨ GPIO

#ifndef __OLED_H
#define __OLED_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ls1x.h"
#include "ls1x_gpio.h"
#include "ls1x_spi.h"

//--------------OLED��������---------------------
#define PAGE_SIZE    8
#define XLevelL		 0x00
#define XLevelH		 0x10
#define YLevel       0xB0
#define	Brightness	 0xFF
#define WIDTH 	     128
#define HEIGHT 	     64

// ---------------- д��������ݶ��� ----------------
#define OLED_CMD     0	// д����
#define OLED_DATA    1	// д����
   						  
// ---------------- OLED�˿ڶ��� ----------------
#define OLED_CS   GPIO_PIN_56   // Ƭѡ�ź�          // spi_csn[1]
#define OLED_DC   GPIO_PIN_39   // ����/��������ź� // ��ͨ GPIO
#define OLED_RST  GPIO_PIN_40   // ��λ�ź�          // ��ͨ GPIO

// ---------------- OLED�˿ڲ������� ----------------
#define OLED_CS_Clr()  Spi_Cs_Down()
#define OLED_CS_Set()  Spi_Cs_Up()

#define OLED_DC_Clr()  gpio_write_pin(OLED_DC, 0)
#define OLED_DC_Set()  gpio_write_pin(OLED_DC, 1)
 					   
#define OLED_RST_Clr()  gpio_write_pin(OLED_RST, 0)
#define OLED_RST_Set()  gpio_write_pin(OLED_RST, 1)

// ͨ�� SPI �� OLED �������ݺ���
void OLED_WR_Byte(uint8_t dat, uint8_t cmd);

// OLED ��ʼ������
void OLED_GPIO_Init(void);
void OLED_Reset(void);
void OLED_Init(void);

// OLED ��ʾ����
// x, y : �������
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_FULL(void);
void OLED_Clear(void);
uint32_t oled_pow(uint8_t m,uint8_t n);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_Set_Pixel(unsigned char x, unsigned char y,unsigned char color);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr, uint8_t Char_Size);                                 // ��ʾ�ַ� 16 * 8   chr�� ʾ����'A'       �鿴:oledfont.h ���� ASCII ���Ƿ��ж�Ӧ���ַ�
void OLED_ShowString(uint8_t x, uint8_t y, char *chr);                            // ��ʾ�ַ���        *chr��ʾ����"ABCDE"   �鿴:oledfont.h ���� ASCII ���Ƿ��ж�Ӧ���ַ�
void OLED_ShowCHinese(uint8_t x, uint8_t y, uint8_t num);                            // ��ʾ���� 16*16  num :oledfont.h   Chi_Cha_Font_16x16�ж�Ӧ����  �������������Ҫ�ĺ���
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size);    // ��ʾ���� num : ��ֵ(0~4294967295); len : ���ֵ�λ��  size: ��д16



#ifdef __cplusplus
}
#endif

#endif