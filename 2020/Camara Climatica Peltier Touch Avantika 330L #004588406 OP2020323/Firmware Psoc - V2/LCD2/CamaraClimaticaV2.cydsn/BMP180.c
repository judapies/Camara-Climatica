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

#include <BMP180.h>


/*************************Variables Declaration*************************************************************************/
uint8 SensorCalibrationData[0x16]; //Calibration data is stored in this array
long X1, X2, X3, B3, B5, B6, UT, UP; //Variables to store calibration coefficients
unsigned long B4, B7; //Variables to store calibration coefficients
short AC1, AC2, AC3, B1, B2, MC, MD; //Variables to store calibration coefficients
unsigned short AC4, AC5, AC6; //Variables to store calibration coefficients
long UT, UP; //Variables to store uncompensated temperature and pressure data
double PressureAtSeaLevel = 101325.0; //Pressure at sea level in Pascal
uint8 CalibrationDataAvailable = FALSE; //This flag is to whether the calibration data is available
/***********************************************************************************************************************/
char8 str[12];    

/*************************************************************************************************************************
* Function Name: ReadSensorCalibrationData
**************************************************************************************************************************
* Summary: This function reads the sensor calibration data over I2C and
* assigns it to the appropriate sensor coefficient variables.
*
* Parameters:
*  void
*
* Return:
*  void
*
*************************************************************************************************************************/
void ReadSensorCalibrationData(void)
{
    static uint8 I2CWriteBuffer[0x01] = {0xAA}; //Location from which the calibration data is to be read
    
    /* Do a write operation with the memory address bytes to get calibration data */
	I2C_MasterWriteBuf(I2CSlaveAddress, (uint8 *) I2CWriteBuffer, sizeof(I2CWriteBuffer), I2C_MODE_COMPLETE_XFER);
    while(!(I2C_MasterStatus() & I2C_MSTAT_WR_CMPLT)); //Wait till the master completes writing
    I2C_MasterClearStatus(); //Clear I2C master status
    
    /* Read calibration from the sensor EEPROM and store it in a buffer */
	I2C_MasterReadBuf(I2CSlaveAddress, (uint8 *) SensorCalibrationData, sizeof(SensorCalibrationData), I2C_MODE_COMPLETE_XFER);
    while(!(I2C_MasterStatus() & I2C_MSTAT_RD_CMPLT)); //Wait till the master completes reading
    I2C_MasterClearStatus(); //Clear I2C master status
    
    CalibrationDataAvailable = TRUE; //Set sensor calibration data available flag
    
    /* Assign calibration data to coefficient variables as described in sensor datasheet */
    AC1 = (SensorCalibrationData[0x00] << 0x08) | SensorCalibrationData[0x01];
    AC2 = (SensorCalibrationData[0x02] << 0x08) | SensorCalibrationData[0x03];
    AC3 = (SensorCalibrationData[0x04] << 0x08) | SensorCalibrationData[0x05];
    AC4 = (SensorCalibrationData[0x06] << 0x08) | SensorCalibrationData[0x07];
    AC5 = (SensorCalibrationData[0x08] << 0x08) | SensorCalibrationData[0x09];
    AC6 = (SensorCalibrationData[0x0A] << 0x08) | SensorCalibrationData[0x0B];
    B1 = (SensorCalibrationData[0x0C] << 0x08) | SensorCalibrationData[0x0D];
    B2 = (SensorCalibrationData[0x0E] << 0x08) | SensorCalibrationData[0x0F];
    MC = (SensorCalibrationData[0x012] << 0x08) | SensorCalibrationData[0x013];
    MD = (SensorCalibrationData[0x014] << 0x08) | SensorCalibrationData[0x015];
}
/***********************************************************************************************************************/

/*************************************************************************************************************************
* Function Name: ReadUncompensatedTemperature
**************************************************************************************************************************
* Summary: This function writes the temperature conversion command to the sensor and
* waits for conversion to complete, and then reads the converted data (uncompensated temperature) and stores it in a variable.
*
* Parameters:
*  void
*
* Return:
*  void
*
*************************************************************************************************************************/
void ReadUncompensatedTemperature(void)
{
    static uint8 I2CReadBuffer[0x02]; //The uncompensated temperature read from the sensor is stored here
    static uint8 I2CWriteBuffer[0x02] = {0xF4, 0x2E}; //Command for temperature conversion - Register address and value
    static uint8 SetReadAddressBuffer[0x01] = {0xF6}; //Register address from which the data is read
    
    /* Do a write operation with the memory address bytes to get temperature data */
	I2C_MasterWriteBuf(I2CSlaveAddress, (uint8 *) I2CWriteBuffer, sizeof(I2CWriteBuffer), I2C_MODE_COMPLETE_XFER);
    while(!(I2C_MasterStatus() & I2C_MSTAT_WR_CMPLT)); //Wait till the master completes writing
    I2C_MasterClearStatus(); //Clear I2C master status
    
    CyDelay(5); //Wait for 5ms temperature conversion
    
    /* In order to do a read from a specific address, do a write operation with the memory address bytes alone */
	I2C_MasterWriteBuf(I2CSlaveAddress, (uint8 *) SetReadAddressBuffer, sizeof(SetReadAddressBuffer), I2C_MODE_COMPLETE_XFER);
    while(!(I2C_MasterStatus() & I2C_MSTAT_WR_CMPLT)); //Wait till the master completes writing
    I2C_MasterClearStatus(); //Clear I2C master status
    
    /* Read temperature from the sensor and store it in a buffer */
	I2C_MasterReadBuf(I2CSlaveAddress, (uint8 *) I2CReadBuffer, sizeof(I2CReadBuffer), I2C_MODE_COMPLETE_XFER);
    while(!(I2C_MasterStatus() & I2C_MSTAT_RD_CMPLT)); //Wait till the master completes reading
    I2C_MasterClearStatus(); //Clear I2C master status
    
	UT = (I2CReadBuffer[0x00] << 0x08) | I2CReadBuffer[0x01]; //Return uncompensated temperature value
}
/***********************************************************************************************************************/

/*************************************************************************************************************************
* Function Name: ReadUncompensatedPressure
**************************************************************************************************************************
* Summary: This function writes the pressure conversion command to the sensor and
* waits for conversion to complete, and then reads the converted data (uncompensated pressure) and stores it in a variable.
*
* Parameters:
*  void
*
* Return:
*  void
*
*************************************************************************************************************************/
void ReadUncompensatedPressure(void)
{
    static uint8 I2CReadBuffer[0x03]; //The uncompensated pressure read from the sensor is stored here
    static uint8 I2CWriteBuffer[0x02] = {0xF4, 0x34 + (OSS << 0x06)}; //Command for pressure conversion - Register address and value
    static uint8 SetReadAddressBuffer[0x01] = {0xF6}; //Register address from which the data is read
    
    /* Do a write operation with the memory address bytes to get pressure data */
	I2C_MasterWriteBuf(I2CSlaveAddress, (uint8 *) I2CWriteBuffer, sizeof(I2CWriteBuffer), I2C_MODE_COMPLETE_XFER);
    while(!(I2C_MasterStatus() & I2C_MSTAT_WR_CMPLT)); //Wait till the master completes writing
    I2C_MasterClearStatus(); //Clear I2C master status
    
    CyDelay(26); //Wait for 26ms pressure conversion
    
    /* In order to do a read from a specific address, do a write operation with the memory address bytes alone */
	I2C_MasterWriteBuf(I2CSlaveAddress, (uint8 *) SetReadAddressBuffer, sizeof(SetReadAddressBuffer), I2C_MODE_COMPLETE_XFER);
    while(!(I2C_MasterStatus() & I2C_MSTAT_WR_CMPLT)); //Wait till the master completes writing
    I2C_MasterClearStatus(); //Clear I2C master status
    
    /* Read pressure from the sensor and store it in a buffer */
	I2C_MasterReadBuf(I2CSlaveAddress, (uint8 *) I2CReadBuffer, sizeof(I2CReadBuffer), I2C_MODE_COMPLETE_XFER);
    while(!(I2C_MasterStatus() & I2C_MSTAT_RD_CMPLT)); //Wait till the master completes reading
    I2C_MasterClearStatus(); //Clear I2C master status
    
	UP = ((I2CReadBuffer[0x00] << 0x10) | (I2CReadBuffer[0x01] << 0x08) | I2CReadBuffer[0x02]) >> (0x08 - OSS); //Return uncompensated pressure value
}
/***********************************************************************************************************************/

/*************************************************************************************************************************
* Function Name: ReadCalculateSensorData
**************************************************************************************************************************
* Summary: This function reads the uncompensated temperature and pressure value and converts it
* into true temperature, pressure and altitude using the algorithm given in the sensor datasheet.
*
* Parameters:
*  void
*
* Return:
*  void
*
*************************************************************************************************************************/
void ReadAndCalculateSensorData(void)
{
    /* Read the sensor calibration data if not already available */
	if(!CalibrationDataAvailable)
    {
        ReadSensorCalibrationData(); //Read sensor calibration data
    }
    
    ReadUncompensatedTemperature(); //Read the uncompensated temperature value
    ReadUncompensatedPressure(); //Read the uncompensated pressure value
    
    /* Calculate the true temperature as described in sensor datasheet */
    X1 = (UT - AC6) * AC5 / pow(2, 15);
    X2 = MC * pow(2, 11) / (X1 + MD);
    B5 = X1 + X2;
    Temperature2 = (B5 + 8) / pow(2, 4);
    
    /* Calculate the true barometric pressure as described in sensor datasheet */
    B6 = B5 - 4000;
    X1 = (B2 * (B6 * B6 / pow(2, 12))) / pow(2, 11);
    X2 = AC2 * B6 / pow(2, 11);
    X3 = X1 + X2;
    B3 = (((AC1 * 4 + X3) << OSS) + 2) / 4;
    X1 = AC3 * B6 / pow(2, 13);
    X2 = (B1 * (B6 * B6 / pow(2, 12))) / pow(2, 16);
    X3 = ((X1 + X2) + 2) / pow(2, 2);
    B4 = (AC4 * (unsigned long)(X3 + 32768)) / pow(2, 15);
    B7 = ((unsigned long)UP - B3) * (50000 >> OSS);
    if(B7 < 0x80000000)
    {
        Pressure = (B7 *2) / B4;
    }
    else 
    {
        Pressure = (B7 / B4) *2;
    }
    X1 = (Pressure / pow(2, 8)) * (Pressure / pow(2, 8));
    X1 = (X1 * 3038) / pow(2, 16);
    X2 = (-7357 * Pressure) / pow(2, 16);
    Pressure = Pressure + (X1 + X2 + 3791) / pow(2, 4);
    
    /* Calculate the true altitude based on pressure at sea level as described in sensor datasheet */
    //Altitude = 44330 * (1 - (pow(Pressure/PressureAtSeaLevel, 1/5.255)));
}
/***********************************************************************************************************************/


/* [] END OF FILE */
