/*******************************************************************************
* File Name: BOMBA.h  
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

#if !defined(CY_PINS_BOMBA_ALIASES_H) /* Pins BOMBA_ALIASES_H */
#define CY_PINS_BOMBA_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*              Constants        
***************************************/
#define BOMBA_0			(BOMBA__0__PC)
#define BOMBA_0_INTR	((uint16)((uint16)0x0001u << BOMBA__0__SHIFT))

#define BOMBA_INTR_ALL	 ((uint16)(BOMBA_0_INTR))

#endif /* End Pins BOMBA_ALIASES_H */


/* [] END OF FILE */
