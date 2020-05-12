/********************************** (C) COPYRIGHT *******************************
* File Name		  : uart.c
* Author			 : betaEncoder
* Version			: V1.0
* Date			   : 2020/05/11
* Description		: uart driver
*******************************************************************************/
#include <stdint.h>
#include "../ch554_sdcc/include/ch554.h"

#include "uart.h"

void UART0_init(){
    // setup io mode
#ifndef UART0_ALT_PIN   // use default pin
    PIN_FUNC  &= ~bUART0_PIN_X;    // UART0 use P3.0/3.1 as RX/TX
/*    P3_DIR_PU |= 0b00000011;    // P3.1: output, P3.0 input
    P3_MOD_OC &= 0b11111101;    // P3.1: push-pull
    P3_MOD_OC |= 0b00000001;    // P3.0: input, pull-up
    RXD = 1;                    // RXD pull-up enable*/
#else
    PIN_FUNC  |= bUART0_PIN_X;    //UART0 use P1.2/1.3 as RX/TX
/*    P1_DIR_PU |= 0b00001100;    //P1.3: output, P1.2 input
    P1_MOD_OC &= 0b11110111;    //P1.3: push-pull
    P1_MOD_OC |= 0b00000100;    //P1.2: input, pull-up
    RXD_ = 1;                    // RXD_ pull-up enable*/
#endif

#ifdef UART0_MODE0
    SM0 = 0;
    SM1 = 0;
#endif
#ifdef UART0_MODE1
    SM0 = 0;
    SM1 = 1;
    TMOD = TMOD & ~ bT1_GATE & ~ bT1_CT & ~ MASK_T1_MOD | bT1_M1;
#endif
#ifdef UART0_MODE2
    SM0 = 1;
    SM1 = 0;
    TMOD = TMOD & ~ bT1_GATE & ~ bT1_CT;
#endif
#ifdef UART0_MODE3
    SM0 = 1;
    SM1 = 1;
    TMOD = TMOD & ~ bT1_GATE & ~ bT1_CT & ~ MASK_T1_MOD | bT1_M1;
#endif

#ifdef UART0_MULTIMACHINE_OR_RX_WITH_STOPBIT
    SM2 = 1;
#else
    SM2 = 0;
#endif

    //divider setup
#if TIMER_SMOD==1
    PCON |= SMOD;
#else
    PCON &= ~SMOD;
#endif
#if TIMER_bTMR_CLK==1
    T2MOD |= bTMR_CLK;
#else
    T2MOD &= ~bTMR_CLK;
#endif

#ifdef UART0_USE_TIMER1
    RCLK = 0;
    TCLK = 0;
    #if TIMER1_bT1_CLK==1
    T2MOD |= bT1_CLK;
    #else
    T2MOD &= ~bT1_CLK;
    #endif
    TH1 = UART0_TIMER1_TH1;
    TR1 = 1;    // Timer1 start

#endif
#ifdef UART0_USE_TIMER2
    RCLK = 1;
    TCLK = 1;
    #if TIMER2_bT2_CLK==1
    T2MOD |= bT2_CLK;
    #else
    T2MOD &= ~bT2_CLK;
    #endif
    RCAP2 = UART0_TIMER2_RCAP2;
#endif

    REN = 1;    // UART0 Receive enable

    return;
}

inline void UART0_interrupt_enable(){
    ES = 1;

    return;
}

inline void UART0_interrupt_disable(){
    ES = 0;

    return;
}

inline void UART0_clear_RX_interrupt_flag(){
    RI = 0;

    return;
}

inline void UART0_increase_interrupt_priority(){
    PS = 1;

    return;
}

inline void UART0_decrease_interrupt_priority(){
    PS = 0;

    return;
}

// blocking read byte
uint8_t UART0_read_byte(){
    while(!RI); // wait for receive
    RI = 0;
    return SBUF;
}

// blocking write byte
inline void UART0_write_byte(uint8_t byte){
    SBUF = byte;
    while(TI ==0);  // wait for done
    TI = 0;

    return;
}

// blocking write string(null terminated array) 
void UART0_write_string(char* string){
    while(*string!='\0'){
        UART0_write_byte(*string);
        string++;
    }

    return;
}