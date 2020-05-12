/********************************** (C) COPYRIGHT *******************************
* File Name		  : uart.c
* Author			 : betaEncoder
* Version			: V1.0
* Date			   : 2020/05/11
* Description		: uart driver
*******************************************************************************/
// define for debug
#define FREQ_SYS 24000000

/* configration for UART0 */

/* Uncomment if use alternate pins for UART */
//#define UART0_ALT_PIN     // use RX_ and TX_ pin for UART0

/* Set baudrate */
#define UART0_BAUDRATE 115200   // It should be considered for the counter will not overflow.

/* Select one of UART mode */
//#define UART0_MODE0         // shiftregistor mode; baudrate is Fsys/12
#define UART0_MODE1         // 8bit asyncronous mode; baudrate is provided Timer1 or Timer2
//#define UART0_MODE2         // 9bit asyncronous mode; baudrate is Fsys/128 (SMOD=0) or Fsys/32 (SMOD=1)
//#define UART0_MODE3         // 9bit asyncronous mode; baudrate is provided Timer1 or Timer2

/* Uncomment if use situetion of multimachine communication or stopbit controll */
//#define UART0_MULTIMACHINE_OR_RX_WITH_STOPBIT   // SM2 bit 

/* Select one of timer 1 or 2 if use any timer for generating baudrate. */
#define UART0_USE_TIMER1
//#define UART0_USE_TIMER2

// divider configration
/* Configure carefully to set timer. "bTMR_CLK" effects all Timers. */
//#define TIMER_SMOD 0            // T1 baudrate trigger divided by 2
#define TIMER_SMOD 1            // T1 baudrate trigger divider disable
//#define TIMER1_bT1_CLK 0        // T1 is driven normal clock Fsys/12
#define TIMER1_bT1_CLK 1        // T1 is driven fast clock. cf:bTMR_CLK
//#define TIMER2_bT2_CLK 0        // T2 is driven normal clock Fsys/4
//#define TIMER2_bT2_CLK 1        // T2 is driven fast clock. cf:bTMR_CLK
//#define TIMER_bTMR_CLK 0        // fast clock frequency is Fsys/4.
#define TIMER_bTMR_CLK 1        // fast clock frequency is Fsys.

#ifdef UART0_USE_TIMER1
    #if TIMER_SMOD==1 && TIMER_bTMR_CLK==1 && TIMER1_bT1_CLK==1
        #define UART0_BAUDRATE_DIVIDER 16
    #elif TIMER_SMOD==0 && TIMER_bTMR_CLK==1 && TIMER1_bT1_CLK==1
        #define UART0_BAUDRATE_DIVIDER 32
    #elif TIMER_SMOD==1 && TIMER_bTMR_CLK==0 && TIMER1_bT1_CLK==1
        #define UART0_BAUDRATE_DIVIDER 64
    #elif TIMER_SMOD==0 && TIMER_bTMR_CLK==0 && TIMER1_bT1_CLK==1
        #define UART0_BAUDRATE_DIVIDER 128
    #elif TIMER_SMOD==1 && TIMER1_bT1_CLK==0
        #define UART0_BAUDRATE_DIVIDER 192
    #elif TIMER_SMOD==0 && TIMER1_bT1_CLK==0
        #define UART0_BAUDRATE_DIVIDER 384
    #endif
#endif
#ifdef UART0_USE_TIMER2
    #if TIMER_bTMR_CLK==1 && TIMER2_bT2_CLK==1
        #define UART0_BAUDRATE_DIVIDER 16
    #elif TIMER_bTMR_CLK==0 && TIMER2_bT2_CLK==1
        #define UART0_BAUDRATE_DIVIDER 32
    #elif TIMER2_bT2_CLK==0
        #define UART0_BAUDRATE_DIVIDER 64
    #endif
    #define UART0_TIMER2_DIVIDED16      // bTMR_CLK=1, bT2_CLK=1
    //#define UART0_TIMER2_DIVIDED32      // bTMR_CLK=0, bT2_CLK=1
    //#define UART0_TIMER2_DIVIDED64      // bT2_CLK=0
#endif

/* baudrate generator counterperiod calcration
   notice: TH1 is 8bit width
           RCAP2 is 16bit width */
#ifdef UART0_USE_TIMER1
    #define UART0_TIMER1_TH1 ((256-(FREQ_SYS+(UART0_BAUDRATE_DIVIDER/UART0_BAUDRATE/2))/UART0_BAUDRATE_DIVIDER/UART0_BAUDRATE)) // divide with rounding
#endif
#ifdef UART0_USE_TIMER2
    #define UART0_TIMER2_RCAP2 ((65536-(FREQ_SYS+(UART0_BAUDRATE_DIVIDER/UART0_BAUDRATE/2))/UART0_BAUDRATE_DIVIDER/UART0_BAUDRATE)) // divide with rounding
#endif

void UART0_init();
inline void UART0_interrupt_enable();
inline void UART0_interrupt_disable();
inline void UART0_clear_RX_interrupt_flag();
inline void UART0_increase_interrupt_priority();
inline void UART0_decrease_interrupt_priority();
uint8_t UART0_read_byte();
void UART0_write_byte(uint8_t byte);
void UART0_write_string(char* string);