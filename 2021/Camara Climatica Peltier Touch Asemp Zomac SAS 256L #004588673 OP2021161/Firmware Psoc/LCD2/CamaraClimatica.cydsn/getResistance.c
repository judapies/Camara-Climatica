/******************************************************************************
* File Name: getResistance.c
*  Version 2.0
*
* Description:
*  This file contains two functions that are used to find the RTD resistance.
* 
* Note:
*
********************************************************************************
* Copyright (2015), Cypress Semiconductor Corporation. All Rights Reserved.
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
#include "getResistance.h"
#include "removeOffsetNoise.h"
#include "main.h"

uint8 i;
uint8 l2,h2;

/*******************************************************************************
*  Function Name: MeasureRefResVoltage
********************************************************************************
* Summary:
* This functions returns the voltage across the reference resistor. 
*
* Parameters:
* none
*
* Return: 
* reference voltage in ADC counts

* Theory:
* A current is passed through the reference resistor and the voltage across it 
* is measured. Offset is measured and subtracted from this voltage. An IIR filter 
* is also applied on this voltage.
*
* Side Effects:
* None
*******************************************************************************/
int32 MeasureRefResVoltage()
{
	/* Voltage read across reference resistor in ADC counts */
	int32 rtdRefResult = 0;
	
	
	/* Filtered value for reference voltage measurement */
	int32 filRtdRefResult = 0;
	
	/* Offset counts */
	int32 zeroCurrentOffset = 0;
	
	/* Select the right current Mux and ADC Mux Channels */
	CurrentMux_FastSelect(REF_IDAC_CHANNEL);
	ADCMux_FastSelect(REF_CHANNEL);
	
	/* Configure the IDAC to source 1 mA */
	IDAC_SetValue(125);
		
	/* Read voltage across reference resistor */
	ADC_StartConvert();

	ADC_IsEndConversion(ADC_WAIT_FOR_RESULT);
	rtdRefResult = ADC_GetResult32();
		

	
	/* Get the offset voltage */
	zeroCurrentOffset = GetOffset();
		
	/* Get CDS difference */
	rtdRefResult = rtdRefResult - zeroCurrentOffset; 

	/* Filter signal */
	filRtdRefResult = FilterSignal(rtdRefResult, REF_RES);
	
	return filRtdRefResult;
}
/*******************************************************************************
*  Function Name: GetRTDRes
********************************************************************************
* Summary:
* This function measures the voltage across the RTD resistance.   
*
* Parameters:
* int32 vRef - reference voltage in ADC counts
*
* Return: 
* int32 rtdRes - RTD resistance in milliohms

* Theory:
* A current is passed through the reference resistor and the voltage across it 
* is measured. Offset is measured and subtracted from this voltage. An IIR filter 
* is also applied on this voltage.
*
* Side Effects:
* None
*******************************************************************************/
int32 GetRTDRes(int32 vRef,uint8 channel)
{
	/* Voltage read across RTD sensor in ADC counts */
	int32 rtdResult = 0;
	
	/* Filtered value for RTD sensor voltage measurement */
	int32 filRtdResult;
	
	/* RTD resistance */
	int32 rtdRes = 0;
	
	/* Offset counts */
	int32 zeroCurrentOffset = 0;
    
	
	/* Set the IDAC to pass 1 mA */
	IDAC_SetValue(125);
	
    if(channel==0){
	/* Select the right current Mux and ADC Mux Channels */
	CurrentMux_FastSelect(RTD_IDAC_CHANNEL);
	ADCMux_FastSelect(RTD_CHANNEL);
		
	/* Read voltage across RTD */
	ADC_StartConvert();
	ADC_IsEndConversion(ADC_WAIT_FOR_RESULT);
	rtdResult = ADC_GetResult32();
    }else if(channel==1){
    /* Select the right current Mux and ADC Mux Channels */
	CurrentMux_FastSelect(RTD2_IDAC_CHANNEL);
	ADCMux_FastSelect(RTD2_CHANNEL);
		
	/* Read voltage across RTD */
	ADC_StartConvert();
	ADC_IsEndConversion(ADC_WAIT_FOR_RESULT);
	rtdResult = ADC_GetResult32();    
    }

		
	/* Read zero current voltage across RTD sensor resistor */
	zeroCurrentOffset = GetOffset();
	
	/* Get CDS difference */
	rtdResult = rtdResult - zeroCurrentOffset; 
	
	/* Filter signal */
	filRtdResult = FilterSignal(rtdResult, RTD);
	
	/* Find RTD resistance in milliohms */
    /* To save precision we need to cast the numerator to a float*/
	rtdRes =  (int32)(((float)filRtdResult / vRef) * REF_RESISTOR);
	
	return rtdRes;
}

/* [] END OF FILE */
