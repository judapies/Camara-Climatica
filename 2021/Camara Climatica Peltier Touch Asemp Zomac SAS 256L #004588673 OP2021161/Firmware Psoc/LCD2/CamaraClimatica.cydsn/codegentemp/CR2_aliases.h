/*******************************************************************************
* File Name: CR2.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_CR2_ALIASES_H) /* Pins CR2_ALIASES_H */
#define CY_PINS_CR2_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*              Constants        
***************************************/
#define CR2_0			(CR2__0__PC)
#define CR2_0_INTR	((uint16)((uint16)0x0001u << CR2__0__SHIFT))

#define CR2_INTR_ALL	 ((uint16)(CR2_0_INTR))

#endif /* End Pins CR2_ALIASES_H */


/* [] END OF FILE */
