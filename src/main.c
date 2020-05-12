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

#include "uart.h"

int main(){
    CfgFsys();

    UART0_init();
    UART0_write_string("Hello CH552T\r\n");


    while(1){
        UART0_write_byte(UART0_read_byte());    // echo back

    }

    return 0;
}
