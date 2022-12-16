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
#include "24512.h"

uint8 I2CWriteBuffer[2]; //Location from which the calibration data is to be read
uint8 status=0;

void write_ext_eeprom(int32 address, uint8 data)
{
    I2C_MasterSendStart(EXTERNAL_EEPROM_ADDRESS,0);
    I2C_MasterWriteByte((uint8)address>>8);
    I2C_MasterWriteByte((uint8)address);
    I2C_MasterWriteByte(data);
    I2C_MasterSendStop();    
}

int8 read_ext_eeprom(int32 address) {
   int8 data;
    I2C_MasterSendStart(EXTERNAL_EEPROM_ADDRESS,0);
    I2C_MasterWriteByte((uint8)address>>8);
    I2C_MasterWriteByte((uint8)address);
    I2C_MasterSendStart(EXTERNAL_EEPROM_ADDRESS,1);
    data=I2C_MasterReadByte(0);
    I2C_MasterSendStop();
    return(data);
}


/* [] END OF FILE */
