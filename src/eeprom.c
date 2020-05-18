/**
 * @file eeprom.c
 * @brief EEPROM driver for CH55x
 * @author betaEncoder
 * @date 2020/05/18
 */

#include <stdint.h>
#include "../ch554_sdcc/include/ch554.h"
#include "EEPROM.h"

uint8_t EEPROM_write(uint8_t* array, uint8_t length) __critical{
    uint8_t i;

    // into safe mode
    SAFE_MOD = 0x55;
    SAFE_MOD = 0xAA;

    EEPROM_write_enable();

    for(i=0;i<length;i++){
        ROM_ADDR = DATA_FLASH_ADDR + (i<<1);
        ROM_DATA_L = array[i];
        ROM_CTRL = 0x9a;    // EEPROM write
        if(ROM_STATUS!=bROM_ADDR_OK){
            // failed
            return EEPROM_NG;
        }
    }
    
    // into safe mode
    SAFE_MOD = 0x55;
    SAFE_MOD = 0xAA;

    EEPROM_write_disable();

    return EEPROM_OK;
}

uint8_t EEPROM_read(uint8_t* array, uint8_t length) __critical{
    uint8_t i;

    for(i=0;i<length;i++){
        ROM_ADDR = DATA_FLASH_ADDR + (i<<1);
        ROM_CTRL = 0x8e;    // EEPROM read
        if(ROM_STATUS!=EEPROM_OK){
            // failed
            return EEPROM_NG;
        }
        array[i] = ROM_DATA_L;
    }

    return EEPROM_OK;
}