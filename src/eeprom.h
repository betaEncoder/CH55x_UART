/**
 * @file eeprom.h
 * @brief EEPROM driver for CH55x
 * @author betaEncoder
 * @date 2020/05/18
 */

#include <stdint.h>
#include "../ch554_sdcc/include/ch554.h"

#define EEPROM_OK 0
#define EEPROM_NG 1

#define EEPROM_write_enable() (GLOBAL_CFG|=bCODE_WE|bDATA_WE)
#define EEPROM_write_disable() (GLOBAL_CFG&=~(bCODE_WE|bDATA_WE))
uint8_t EEPROM_write(uint8_t* array, uint8_t length);
uint8_t EEPROM_read(uint8_t* array, uint8_t length);