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

#define ADC_enable(speed) (ADC_CFG|=ADC_EN|speed)
#define ADC_disable() (ADC_CFG&=0xf7)
#define ADC_enable_interrupt() (IE_ADC=1)
#define ADC_disable_interrupt() (IE_ADC=0)
#define ADC_clear_interrupt_flag() (ADC_IF=0)
#define ADC_start() (ADC_START=1)
#define ADC_get_converting_flag() (ADC_START)
#define ADC_get_value() (ADC_DATA)
#define CMP_enable() (ADC_CFG|=CMP_EN)
#define CMP_disable() (ADC_CFG&=~CMP_EN)
#define ADC_CMP_select_input(channel) (ADC_CTRL=(ADC_CTRL&0xFC)|channel)
#define CMP_select_invert_input(channel) (ADC_CTRL=(ADC_CTRL&0xf7)|(channel&0x2)<<2))
#define CMP_get_convert_done_flag() (CMP_IF)
#define CMP_get_value() (CMP0)