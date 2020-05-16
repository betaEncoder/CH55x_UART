/********************************** (C) COPYRIGHT *******************************
* File Name		  : gpio.h
* Author			 : betaEncoder
* Version			: V1.0
* Date			   : 2020/05/16
* Description		: gpio driver
*******************************************************************************/

#define GPIO_PORT1 1
#define GPIO_PORT3 3

#define GPIO_PIN0 1<<0
#define GPIO_PIN1 1<<1
#define GPIO_PIN2 1<<2
#define GPIO_PIN3 1<<3
#define GPIO_PIN4 1<<4
#define GPIO_PIN5 1<<5
#define GPIO_PIN6 1<<6
#define GPIO_PIN7 1<<7

#define GPIO_MODE_HI_Z 0
#define GPIO_MODE_PUSHPULL 1
#define GPIO_MODE_OC 2
#define GPIO_MODE_INOUT 3

#define GPIO_INTERRUPT_MODE_EDGE 1<<7
#define GPIO_INTERRUPT_RXD1 1<<6
#define GPIO_INTERRUPT_P1_5 1<<5
#define GPIO_INTERRUPT_P1_4 1<<4
#define GPIO_INTERRUPT_P1_3 1<<3
#define GPIO_INTERRUPT_RST 1<<2
#define GPIO_INTERRUPT_P3_1 1<<1
#define GPIO_INTERRUPT_RXD0 1<<0

SBIT(P1_0, 0x90, 0);
SBIT(P1_1, 0x90, 1);
SBIT(P1_2, 0x90, 2);
SBIT(P1_3, 0x90, 3);
SBIT(P1_4, 0x90, 4);
SBIT(P1_5, 0x90, 5);
SBIT(P1_6, 0x90, 6);
SBIT(P1_7, 0x90, 7);

SBIT(P3_0, 0xB0, 0);
SBIT(P3_1, 0xB0, 1);
SBIT(P3_2, 0xB0, 2);
SBIT(P3_3, 0xB0, 3);
SBIT(P3_4, 0xB0, 4);
SBIT(P3_5, 0xB0, 5);
SBIT(P3_6, 0xB0, 6);
SBIT(P3_7, 0xB0, 7);

void GPIO_Init(uint8_t port, uint8_t pin, uint8_t mode);
void GPIO_set_interrupt_mask(uint8_t event);
void GPIO_interrupt_enable();
void GPIO_interrupt_disable();