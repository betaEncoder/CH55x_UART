/**
 * @file adc.c
 * @brief ADC driver for CH55x
 * @author betaEncoder
 * @date 2020/05/18
 */

#include <stdint.h>
#include "../ch554_sdcc/include/ch554.h"

#define ADC_SPEED_LOW 0
#define ADC_SPEED_HIGH 1

#define ADC_AIN0 0
#define ADC_AIN1 1
#define ADC_AIN2 2
#define ADC_AIN3 3

#define CMP_EN 1<<2
#define ADC_EN 1<<3

/**
 * @brief ADC enable macro
 * @param speed clock speed for conversion
 * @return none
 */
#define ADC_enable(speed) (ADC_CFG|=ADC_EN|speed)

/**
 * @brief ADC disable macro
 * @param none
 * @return none
 */
#define ADC_disable() (ADC_CFG&=0xf7)

/**
 * @brief ADC enable interrupt macro
 * @param none
 * @return none
 */
#define ADC_enable_interrupt() (IE_ADC=1)

/**
 * @brief ADC disable interrupt macro
 * @param none
 * @return none
 */
#define ADC_disable_interrupt() (IE_ADC=0)

/**
 * @brief clear ADC interrupt flag macro
 * @param none
 * @return none
 * @detail Interrupt flag must be clear at ISR
 */
#define ADC_clear_interrupt_flag() (ADC_IF=0)

/**
 * @brief ADC conversion start macro
 * @param none
 * @return none
 */
#define ADC_start() (ADC_START=1)

/**
 * @brief get ADC is under converting flag macro
 * @param none
 * @return under converting flag
 */
#define ADC_get_converting_flag() (ADC_START)

/**
 * @brief get last converted value macro
 * @param none
 * @return adc conversion result
 */
#define ADC_get_value() (ADC_DATA)

/**
 * @brief enable comperetor macro
 * @param none
 * @return none
 */
#define CMP_enable() (ADC_CFG|=CMP_EN)

/**
 * @brief disable comperetor macro
 * @param none
 * @return none
 */
#define CMP_disable() (ADC_CFG&=~CMP_EN)

/**
 * @brief select ADC input and comperetor non-inverting input macro
 * @param none
 * @return none
 */
#define ADC_CMP_select_input(channel) (ADC_CTRL=(ADC_CTRL&0xFC)|channel)

/**
 * @brief select inverting comperetor inverting input macro
 * @param none
 * @return none
 */
#define CMP_select_invert_input(channel) (ADC_CTRL=(ADC_CTRL&0xf7)|(channel&0x2)<<2))

/**
 * @brief get conperetor conversion done flag macro
 * @param none
 * @return conperetor conversion done
 */
#define CMP_get_convert_done_flag() (CMP_IF)

/**
 * @brief get comperetor value macro
 * @param none
 * @return compere result
 */
#define CMP_get_value() (CMP0)
