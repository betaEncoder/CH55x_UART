/********************************** (C) COPYRIGHT *******************************
* File Name		  : main.c
* Author			 : betaEncoder
* Version			: V1.0
* Date			   : 2020/05/11
* Description		: uart sample
*******************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../ch554_sdcc/include/ch554.h"
#include "../ch554_sdcc/include/debug.h"

#include "gpio.h"
#include "uart.h"

#define LED_PIN GPIO_PIN5
#define LED_PORT GPIO_PORT1
#define LED P1_5

int main(){
    CfgFsys();
    uint8_t c;

    GPIO_Init(LED_PORT, LED_PIN, GPIO_MODE_PUSHPULL);

    UART0_init();
    UART0_interrupt_enable();
    EA = 1;
    UART0_write_string_IT("Hello CH552T\r\n");


    while(1){
        if(UART0_get_bytes_to_read()){
    
            UART0_get_bytes_from_buffer(&c, 1);
            if(c=='1'){
                LED = 1;
            }else{
                LED = 0;
            }
            UART0_write_array_IT(&c, 1);
        }
    }

    return 0;
}

void UART0_TX_done_isr(void) __interrupt INT_NO_UART0{
    _UART0_interrupt_handler();
}