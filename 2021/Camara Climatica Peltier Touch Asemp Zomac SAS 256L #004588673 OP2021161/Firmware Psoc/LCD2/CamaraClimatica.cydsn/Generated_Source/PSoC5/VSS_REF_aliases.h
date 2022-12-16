/*******************************************************************************
* File Name: VSS_REF.h  
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

#if !defined(CY_PINS_VSS_REF_ALIASES_H) /* Pins VSS_REF_ALIASES_H */
#define CY_PINS_VSS_REF_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*              Constants        
***************************************/
#define VSS_REF_0			(VSS_REF__0__PC)
#define VSS_REF_0_INTR	((uint16)((uint16)0x0001u << VSS_REF__0__SHIFT))

#define VSS_REF_INTR_ALL	 ((uint16)(VSS_REF_0_INTR))

#endif /* End Pins VSS_REF_ALIASES_H */


/* [] END OF FILE */
