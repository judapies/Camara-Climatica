/******************************************************************************
* File Name: coldJnTemp.c
*  Version 1.0
*
* Description:
*  This file contains the code to measure cold junction temperature
*
* Note:
*
********************************************************************************
* Copyright (2018), Cypress Semiconductor Corporation. All Rights Reserved.
********************************************************************************
* This software is owned by Cypress Semiconductor Corporation (Cypress)
* and is protected by and subject to worldwide patent protection (United
* States and foreign), United States copyright laws and international treaty
* provisions. Cypress hereby grants to licensee a personal, non-exclusive,
* non-transferable license to copy, use, modify, create derivative works of,
* and compile the Cypress Source Code and derivative works for the sole
* purpose of creating custom software in support of licensee product to be
* used only in conjunction with a Cypress integrated circuit as specified in
* the applicable agreement. Any reproduction, modification, translation,
* compilation, or representation of this software except as specified above 
* is prohibited without the express written permission of Cypress.
*
* Disclaimer: CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH 
* REGARD TO THIS MATERIAL, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
* Cypress reserves the right to make changes without further notice to the 
* materials described herein. Cypress does not assume any liability arising out 
* of the application or use of any product or circuit described herein. Cypress 
* does not authorize its products for use as critical components in life-support 
* systems where a malfunction or failure may reasonably be expected to result in 
* significant injury to the user. The inclusion of Cypress' product in a life-
* support systems application implies that the manufacturer assumes all risk of 
* such use and in doing so indemnifies Cypress against all charges. 
*
* Use of this Software may be limited by and subject to the applicable Cypress
* software license agreement. 
*******************************************************************************/
#include <main.h>

extern volatile uint8 CJSelectFlag;
uint8 i;
uint8 l2,h2;
uint16 Prom;
uint16 promedio2[20];

uint8 extraeDecimal(float data){
   float entero=0;
   float tmp=0.0;
   
   entero=(int8)data;
   tmp=(data-entero)*10;
   return (int8)tmp;
}

float inttofloat(uint8 decimal){
   float tmp;
   tmp=decimal;
   return tmp/10;
}

uint8 convert8(uint16 data,uint8 nible){
    uint8 dato=0;
    if(nible==0){
        dato=(uint8)data;   
    }else if(nible==1){
        dato=data>>8;
    }
    return dato;
}

uint16 convert16(uint8 nibleh,uint8 niblel){
    uint16 temp,var;
    temp=(uint16)nibleh;
    var=temp<<8 | (uint16)niblel;
    return var;
}

void leeEEPROM(){
    uint8 n;
    Password[0]=EEPROM_ReadByte(0);CyDelay(5);  
    Password[1]=EEPROM_ReadByte(1);CyDelay(5);
    Password[2]=EEPROM_ReadByte(2);CyDelay(5);
    Password[3]=EEPROM_ReadByte(3);CyDelay(5);
    
    Ganancia=EEPROM_ReadByte(30);CyDelay(5);
    Derivativo=EEPROM_ReadByte(31);CyDelay(5);
    Integral=EEPROM_ReadByte(32);CyDelay(5);
    Tciclo=EEPROM_ReadByte(33);CyDelay(5);
    Ajuste=EEPROM_ReadByte(34);CyDelay(5);
    Ajusten=EEPROM_ReadByte(35);CyDelay(5);
    MediaMovil=EEPROM_ReadByte(36);CyDelay(5);
    
    if(Password[0]>9){
        Password[0]=0;
        EEPROM_WriteByte(0,0);CyDelay(20);
    }
    if(Password[1]>9){
        Password[1]=0;
        EEPROM_WriteByte(0,1);CyDelay(20);
    }
    if(Password[2]>9){
        Password[2]=0;
        EEPROM_WriteByte(0,2);CyDelay(20);
    }
    if(Password[3]>9){
        Password[3]=0;
        EEPROM_WriteByte(0,3);CyDelay(20);
    }
    if(Ganancia>10 || Ganancia<1){
        Ganancia=3;    
        EEPROM_WriteByte(3,30);CyDelay(20);
    }
    
    if(Derivativo>80 || Derivativo<5){
        Derivativo=5;    
        EEPROM_WriteByte(5,31);CyDelay(20);
    }
    if(Integral>4 || Integral<1){
        Integral=1;    
        EEPROM_WriteByte(1,32);CyDelay(20);
    }
    if(Tciclo>10 || Tciclo<1){
        Tciclo=10;    
        EEPROM_WriteByte(10,33);CyDelay(20);
    }
    if(Ajuste>50){
        Ajuste=0;    
        EEPROM_WriteByte(0,34);CyDelay(20);
    }
    if(Ajusten>50){
        Ajusten=0;    
        EEPROM_WriteByte(0,35);CyDelay(20);
    }
    if(MediaMovil>40 || MediaMovil<1){
        MediaMovil=10;    
        EEPROM_WriteByte(10,36);CyDelay(20);
    }
    GananciaIn=Ganancia;
    for(n=0;n<5;n++){
        for(i=0;i<10;i++){
            Programas[n].rampas[i].TemperaturaInt=EEPROM_ReadByte(100+(n*50)+(i*5)); 
            Programas[n].rampas[i].TemperaturaDec=EEPROM_ReadByte(101+(n*50)+(i*5)); 
            Programas[n].rampas[i].Horas=EEPROM_ReadByte(103+(n*50)+(i*5)); 
            Programas[n].rampas[i].Minutos=EEPROM_ReadByte(104+(n*50)+(i*5)); 
               
            Programas[n].rampas[i].Temperatura=convert16(Programas[n].rampas[i].TemperaturaDec,Programas[n].rampas[i].TemperaturaInt);
            if(Programas[n].rampas[i].Temperatura>1200)Programas[n].rampas[i].Temperatura=10;
            if(Programas[n].rampas[i].Horas>99.0)Programas[n].rampas[i].Horas=10.0;
            if(Programas[n].rampas[i].Minutos>59.0)Programas[n].rampas[i].Minutos=10.0;
        }
    }
}

/*******************************************************************************
*  Function Name: MeasureColdJnSensorTemp
********************************************************************************
* Summary:
*  This function is used to select cold junction temperature sensor which is used 
*  as cold junction compensation for thermocouple. The selection is made based on 
*  the value of CJSelectFlag
*
* Parameters:  
* void:
*
* Return: 
* Temperature in °C
*
* Theory:
*
* Side Effects:
* None
*******************************************************************************/
int32 MeasureColdJnSensorTemp(void)
{
    /* Cold Jn temperature is measured using either DS600 IC or Thermistor SW2
        controls the selection*/
    uint16 lm35;
        /* Show ADC2 result on LCD*/
        lm35=0;
        if(dataReady != 0u)
        {
            Prom=ADC_SAR_CountsTo_mVolts(result);
            if(l2>(10-1))
               l2=0;

            promedio2[l2]=Prom;l2++;
                 
            for(h2=0;h2<10;h2++){
                lm35+=promedio2[h2];
            } 
            lm35=lm35/10; 
            dataReady = 0u;
        }
    
	return lm35*10;
}

/*******************************************************************************
*  Function Name: MeasureICTemp
********************************************************************************
* Summary:
*  This function is used to measure the DS600 IC temperature which is used as
*  cold junction compensation for thermocouple.
*
* Parameters:  
* void:
*
* Return: 
* int32: Temperature in °C
*
* Theory:
* Cold Junction IC DS600 has temperature coefficient of 6.45mV/°C with 
* 509mv offset at 25°C.
* Analog voltage output from IC is converted to temperature as follows:
* Temp = (Vout - 509mV)/6.45mV
*
* Side Effects:
* None
*******************************************************************************/
int32 MeasureICTemp(void)
{
  	/* Cold junction IC ADC count measurement*/
    int32 ICResult;
   
    /* Cold Junction micro volts converted from ADC reading */
    int32 ICUVolts;
    
    /* Cold Jn temperature measured using DS600 */
    int32 ICTemp;
	
	/* Variable used to calculate VSSA ground reference voltage */
    int32 offset;
    
    
    /* Measure cold junction temperature in Celsius from parameters from the DS600 
	 * Temp Sensor 509 mV at 0 degrees C, divided by 6.45 mV per degree C. The macros
	 * (SCALING, VOLTAGE_PER_DEGREE and ZERO_DEGREE_OFFSET_VOLTAGE have been 
	 * manipulated such that the resultant temperature is in 100th of a degree Celsius*/
    
	ICTemp = ((ICUVolts - ZERO_DEGREE_OFFSET_VOLTAGE) * SCALING) / VOLTAGE_PER_DEGREE;

    return ICTemp;
}

/*******************************************************************************
*  Function Name: MeasureThermistorTemp
********************************************************************************
* Summary:
*  This function is used to measure the thermistor temperature which is used as
*  cold junction compensation for thermocouple.
*
* Parameters:  
* void:
*
* Return: 
* int32: Temperature in 1/100th °C
*
* Theory:
* The thermistor resistance is measured and converted into temperature. See 
* 'AN66477 - Temperature Measurement with thermistor' to understand the resistance 
* measurement technique and the conversion of resistance to temperature using the 
* thermistor component
*
* Side Effects:
* None
*******************************************************************************/

/* [] END OF FILE */

