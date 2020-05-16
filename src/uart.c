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

#ifdef UART0_ENABLE_INTERRUPT_DRIVEN_API
// interrupt routine under processing
volatile uint8_t tx_processing = 0;

// indexes for ring buffer
uint8_t tx_buffer_head_index = 0;
uint8_t tx_buffer_data_count = 0;
uint8_t rx_buffer_head_index = 0;
uint8_t rx_buffer_data_count = 0;
uint8_t UART0_TX_buffer[UART0_BUFFER_LENGTH];
uint8_t UART0_RX_buffer[UART0_BUFFER_LENGTH];

// ring buffer UART0_ENABLE_INTERRUPT_DRIVEN_API
uint8_t UART0_put_byte_to_TX_buffer(uint8_t byte);
uint8_t UART0_get_byte_from_TX_buffer(uint8_t* ptr);
uint8_t UART0_put_byte_to_RX_buffer(uint8_t byte);
uint8_t UART0_get_byte_from_RX_buffer(uint8_t* ptr);
void _UART0_TX_done_interrupt_handler();
void _UART0_RX_done_interrupt_handler();

/* private functions */
void _UART0_RX_done_interrupt_handler(){
    UART0_put_byte_to_RX_buffer(SBUF);
}

void _UART0_TX_done_interrupt_handler(){
    uint8_t byte;
    if(UART0_get_byte_from_TX_buffer(&byte)==UART_OK){
        // data to send is available
        SBUF = byte;
    }else{
        // transmit done
        tx_processing = 0;
    }
    return;
}

uint8_t UART0_put_byte_to_TX_buffer(uint8_t byte){
    if(tx_buffer_data_count==UART0_BUFFER_LENGTH){
        // buffer full
        return UART_NG;
    }
    UART0_TX_buffer[tx_buffer_head_index] = byte;
    tx_buffer_head_index++;
    tx_buffer_head_index %= UART0_BUFFER_LENGTH;
    tx_buffer_data_count++;

    return UART_OK;
}

uint8_t UART0_get_byte_from_TX_buffer(uint8_t* ptr){
    uint8_t index;
    if(tx_buffer_data_count==0){
        // buffer empty
        return UART_NG;
    }
    index = tx_buffer_head_index-tx_buffer_data_count;
    index %= UART0_BUFFER_LENGTH;
    *ptr = UART0_TX_buffer[index];
    tx_buffer_data_count--;

    return UART_OK;
}

uint8_t UART0_put_byte_to_RX_buffer(uint8_t byte){
    if(rx_buffer_data_count==UART0_BUFFER_LENGTH){
        // buffer full
        return UART_NG;
    }
    UART0_RX_buffer[rx_buffer_head_index] = byte;
    rx_buffer_head_index++;
    rx_buffer_head_index %= UART0_BUFFER_LENGTH;
    rx_buffer_data_count++;

    return UART_OK;
}

uint8_t UART0_get_byte_from_RX_buffer(uint8_t* ptr){
    uint8_t index;
    if(rx_buffer_data_count==0){
        // buffer empty
        return UART_NG;
    }
    index = rx_buffer_head_index-rx_buffer_data_count;
    index %= UART0_BUFFER_LENGTH;
    *ptr = UART0_RX_buffer[index];
    rx_buffer_data_count--;

    return UART_OK;
}

/* public functions */
// put string to TX buffer
void _UART0_interrupt_handler() __critical{
    if(TI){
        // dransmit done
        TI = 0;
        _UART0_TX_done_interrupt_handler();
    }
    if(RI){
        // receive done
        RI = 0;
        _UART0_RX_done_interrupt_handler();
    }
}

uint8_t UART0_get_bytes_to_read(){
    return rx_buffer_data_count;
}

uint8_t UART0_get_bytes_from_buffer(uint8_t* array, uint8_t length){
    uint8_t index;
    for(index=0;index<length;index++){
        if(UART0_get_bytes_to_read()){
            UART0_get_byte_from_RX_buffer(&array[index]);
        }else{
            break;
        }
    }

    return index;
}

uint8_t UART0_write_string_IT(char* string){
    uint8_t index;
    for(index=0;string[index]!='\0';index++){
        if(UART0_put_byte_to_TX_buffer(string[index])==UART_NG){
            // buffer full
            break;
        }
    }
    if(tx_processing==0){
        tx_processing = 1;
        _UART0_TX_done_interrupt_handler();
    }

    return index;   // bytes to success
}

// put array to TX buffer
uint8_t UART0_write_array_IT(uint8_t* array, uint8_t length){
    uint8_t index;
    for(index=0;index<length;index++){
        if(UART0_put_byte_to_TX_buffer(array[index])==UART_NG){
            // buffer full
            break;
        }
    }
    if(tx_processing==0){
        tx_processing = 1;
        _UART0_TX_done_interrupt_handler();
    }

    return index;
}
#endif