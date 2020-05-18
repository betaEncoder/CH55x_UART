/**
 * @file pwm.c
 * @brief PWM driver for CH55x
 * @author betaEncoder
 * @date 2020/05/18
 */

#include <stdint.h>
#include "../ch554_sdcc/include/ch554.h"

#define PWM_EMPTY_INTERRUPT_ENABLE 1<<7
#define PWM2_OUTPUT_INVERT 1<<6
#define PWM1_OUTPUT_INVERT 1<<5
#define PWM_SYCLE_END_INTERRUPT_FLAG 1<<4
#define PWM2_OUTPUT_ENABLE 1<<3
#define PWM1_OUTPUT_ENABLE 1<<2
#define PWM_CLR_ALL 1<<1

#define PWM_output_enable(channel) (PWM_CTRL=(PWM_CTRL&~bPWM_CLR_ALL)|channel)
#define PWM_output_disable(channel) (PWM_CTRL&=~channel)
#define PWM_interrupt_enable() (IE_EX|=IE_PWMX)
#define PWM_interrupt_disable() (IE_EX&=~IE_PWMX)
#define PWM_use_alternate_pin(pin) (PIN_FUNC=(PIN_FUNC&0xf3)|pin)
#define PWM1_set_compare(value) (PWM_DATA1=value)
#define PWM2_set_compare(value) (PWM_DATA2=value)
#define PWM_set_prescaler(value) (PWM_CK_SE=value)