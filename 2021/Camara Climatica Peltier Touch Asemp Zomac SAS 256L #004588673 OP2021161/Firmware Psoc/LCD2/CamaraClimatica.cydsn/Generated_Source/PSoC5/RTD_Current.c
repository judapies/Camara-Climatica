/*******************************************************************************
* File Name: RTD_Current.c  
* Version 2.10
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "RTD_Current.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 RTD_Current__PORT == 15 && ((RTD_Current__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: RTD_Current_Write
********************************************************************************
*
* Summary:
*  Assign a new value to the digital port's data output register.  
*
* Parameters:  
*  prtValue:  The value to be assigned to the Digital Port. 
*
* Return: 
*  None
*  
*******************************************************************************/
void RTD_Current_Write(uint8 value) 
{
    uint8 staticBits = (RTD_Current_DR & (uint8)(~RTD_Current_MASK));
    RTD_Current_DR = staticBits | ((uint8)(value << RTD_Current_SHIFT) & RTD_Current_MASK);
}


/*******************************************************************************
* Function Name: RTD_Current_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  RTD_Current_DM_STRONG     Strong Drive 
*  RTD_Current_DM_OD_HI      Open Drain, Drives High 
*  RTD_Current_DM_OD_LO      Open Drain, Drives Low 
*  RTD_Current_DM_RES_UP     Resistive Pull Up 
*  RTD_Current_DM_RES_DWN    Resistive Pull Down 
*  RTD_Current_DM_RES_UPDWN  Resistive Pull Up/Down 
*  RTD_Current_DM_DIG_HIZ    High Impedance Digital 
*  RTD_Current_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void RTD_Current_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(RTD_Current_0, mode);
}


/*******************************************************************************
* Function Name: RTD_Current_Read
********************************************************************************
*
* Summary:
*  Read the current value on the pins of the Digital Port in right justified 
*  form.
*
* Parameters:  
*  None
*
* Return: 
*  Returns the current value of the Digital Port as a right justified number
*  
* Note:
*  Macro RTD_Current_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 RTD_Current_Read(void) 
{
    return (RTD_Current_PS & RTD_Current_MASK) >> RTD_Current_SHIFT;
}


/*******************************************************************************
* Function Name: RTD_Current_ReadDataReg
********************************************************************************
*
* Summary:
*  Read the current value assigned to a Digital Port's data output register
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value assigned to the Digital Port's data output register
*  
*******************************************************************************/
uint8 RTD_Current_ReadDataReg(void) 
{
    return (RTD_Current_DR & RTD_Current_MASK) >> RTD_Current_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(RTD_Current_INTSTAT) 

    /*******************************************************************************
    * Function Name: RTD_Current_ClearInterrupt
    ********************************************************************************
    * Summary:
    *  Clears any active interrupts attached to port and returns the value of the 
    *  interrupt status register.
    *
    * Parameters:  
    *  None 
    *
    * Return: 
    *  Returns the value of the interrupt status register
    *  
    *******************************************************************************/
    uint8 RTD_Current_ClearInterrupt(void) 
    {
        return (RTD_Current_INTSTAT & RTD_Current_MASK) >> RTD_Current_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
