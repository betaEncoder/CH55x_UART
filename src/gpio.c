/********************************** (C) COPYRIGHT *******************************
* File Name		  : gpio.h
* Author			 : betaEncoder
* Version			: V1.0
* Date			   : 2020/05/16
* Description		: gpio driver
*******************************************************************************/

#include <stdint.h>
#include "../ch554_sdcc/include/ch554.h"

#include "gpio.h"

void GPIO_Init(uint8_t port, uint8_t pin, uint8_t mode){
    uint8_t mask = ~(1<<pin);
    uint8_t tmp_MOD, tmp_DIR;

    // read
    switch (port)
    {
    case GPIO_PORT1:
        tmp_MOD = P1_MOD_OC;
        tmp_DIR = P1_DIR_PU;
        break;
    case GPIO_PORT3:
        tmp_MOD = P3_MOD_OC;
        tmp_DIR = P3_DIR_PU;
        break;
    
    default:
        return;
        break;
    }

    // modify
    switch(mode){
    case GPIO_MODE_HI_Z:
        tmp_MOD &= mask;
        tmp_DIR &= mask;
        break;
    case GPIO_MODE_PUSHPULL:
        tmp_MOD &= mask;
        tmp_DIR |= ~mask;
        break;
    case GPIO_MODE_OC:
        tmp_MOD |= ~mask;
        tmp_DIR &= mask;
        break;
    case GPIO_MODE_INOUT:
        tmp_MOD |= ~mask;
        tmp_DIR |= ~mask;
        break;
    default:
        return;
        break;
    }
    
    // write
    switch (port)
    {
    case GPIO_PORT1:
        P1_MOD_OC = tmp_MOD;
        P1_DIR_PU = tmp_DIR;
        break;
    case GPIO_PORT3:
        P3_MOD_OC = tmp_MOD;
        P3_DIR_PU = tmp_DIR;
        break;
    
    default:
        return;
        break;
    }
}

void GPIO_set_interrupt_mask(uint8_t event){
    GPIO_IE = event;
}

void GPIO_interrupt_enable(){
    IE_GPIO = 1;
}

void GPIO_interrupt_disable(){
    IE_GPIO = 0;
}