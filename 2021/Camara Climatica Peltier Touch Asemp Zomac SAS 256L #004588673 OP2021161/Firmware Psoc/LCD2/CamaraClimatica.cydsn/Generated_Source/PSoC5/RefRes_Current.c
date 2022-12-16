/*******************************************************************************
* File Name: RefRes_Current.c  
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
#include "RefRes_Current.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 RefRes_Current__PORT == 15 && ((RefRes_Current__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: RefRes_Current_Write
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
void RefRes_Current_Write(uint8 value) 
{
    uint8 staticBits = (RefRes_Current_DR & (uint8)(~RefRes_Current_MASK));
    RefRes_Current_DR = staticBits | ((uint8)(value << RefRes_Current_SHIFT) & RefRes_Current_MASK);
}


/*******************************************************************************
* Function Name: RefRes_Current_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  RefRes_Current_DM_STRONG     Strong Drive 
*  RefRes_Current_DM_OD_HI      Open Drain, Drives High 
*  RefRes_Current_DM_OD_LO      Open Drain, Drives Low 
*  RefRes_Current_DM_RES_UP     Resistive Pull Up 
*  RefRes_Current_DM_RES_DWN    Resistive Pull Down 
*  RefRes_Current_DM_RES_UPDWN  Resistive Pull Up/Down 
*  RefRes_Current_DM_DIG_HIZ    High Impedance Digital 
*  RefRes_Current_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void RefRes_Current_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(RefRes_Current_0, mode);
}


/*******************************************************************************
* Function Name: RefRes_Current_Read
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
*  Macro RefRes_Current_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 RefRes_Current_Read(void) 
{
    return (RefRes_Current_PS & RefRes_Current_MASK) >> RefRes_Current_SHIFT;
}


/*******************************************************************************
* Function Name: RefRes_Current_ReadDataReg
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
uint8 RefRes_Current_ReadDataReg(void) 
{
    return (RefRes_Current_DR & RefRes_Current_MASK) >> RefRes_Current_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(RefRes_Current_INTSTAT) 

    /*******************************************************************************
    * Function Name: RefRes_Current_ClearInterrupt
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
    uint8 RefRes_Current_ClearInterrupt(void) 
    {
        return (RefRes_Current_INTSTAT & RefRes_Current_MASK) >> RefRes_Current_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
