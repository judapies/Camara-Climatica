/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "device.h"
#include "funciones.h"
#define EXTERNAL_EEPROM_ADDRESS 0xa0
#define EXTERNAL_EEPROM_SIZE   65535

extern uint8 I2CWriteBuffer[2]; //Location from which the calibration data is to be read
extern uint8 status;

void write_ext_eeprom(int32 address, uint8 data);
int8 read_ext_eeprom(int32 address);
/* [] END OF FILE */
