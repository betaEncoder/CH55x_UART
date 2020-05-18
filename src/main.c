/**
 * @file main.c
 * @brief example for uart driver
 * @author betaEncoder
 * @date 2020/05/11
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../ch554_sdcc/include/ch554.h"
#include "../ch554_sdcc/include/debug.h"

#include "gpio.h"
#include "uart.h"
#include "adc.h"

#define LED_PIN GPIO_PIN5
#define LED_PORT GPIO_PORT1
#define LED P1_5

int main(){
    CfgFsys();
    uint8_t c, str[8];

    GPIO_Init(LED_PORT, LED_PIN, GPIO_MODE_PUSHPULL);
    ADC_enable(ADC_SPEED_HIGH);
    ADC_CMP_select_input(ADC_AIN0);
    ADC_start();

    UART0_init();
    UART0_interrupt_enable();
    EA = 1;
    UART0_write_string_IT("Hello CH552T\r\n");


    while(1){
        if(UART0_get_bytes_to_read()){
    
            UART0_get_bytes_from_buffer(&c, 1);
            if(c=='1'){
                LED = 1;
            }else if(c=='a'){
                c = ADC_get_value();
                ADC_start();
                sprintf(str, "%d\r\n", c);
                UART0_write_string_IT(str);
                continue;
            }else{
                LED = 0;
            }
            UART0_write_array_IT(&c, 1);
        }
    }

    return 0;
}

// uart0 interrupt function
void UART0_isr(void) __interrupt INT_NO_UART0 {
    _UART0_interrupt_handler();
}