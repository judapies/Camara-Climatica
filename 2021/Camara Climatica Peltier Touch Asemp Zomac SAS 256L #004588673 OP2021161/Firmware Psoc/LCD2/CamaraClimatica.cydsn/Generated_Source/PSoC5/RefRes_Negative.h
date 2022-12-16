/*******************************************************************************
* File Name: RefRes_Negative.h  
* Version 2.10
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_RefRes_Negative_H) /* Pins RefRes_Negative_H */
#define CY_PINS_RefRes_Negative_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "RefRes_Negative_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 RefRes_Negative__PORT == 15 && ((RefRes_Negative__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    RefRes_Negative_Write(uint8 value) ;
void    RefRes_Negative_SetDriveMode(uint8 mode) ;
uint8   RefRes_Negative_ReadDataReg(void) ;
uint8   RefRes_Negative_Read(void) ;
uint8   RefRes_Negative_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define RefRes_Negative_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define RefRes_Negative_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define RefRes_Negative_DM_RES_UP          PIN_DM_RES_UP
#define RefRes_Negative_DM_RES_DWN         PIN_DM_RES_DWN
#define RefRes_Negative_DM_OD_LO           PIN_DM_OD_LO
#define RefRes_Negative_DM_OD_HI           PIN_DM_OD_HI
#define RefRes_Negative_DM_STRONG          PIN_DM_STRONG
#define RefRes_Negative_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define RefRes_Negative_MASK               RefRes_Negative__MASK
#define RefRes_Negative_SHIFT              RefRes_Negative__SHIFT
#define RefRes_Negative_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define RefRes_Negative_PS                     (* (reg8 *) RefRes_Negative__PS)
/* Data Register */
#define RefRes_Negative_DR                     (* (reg8 *) RefRes_Negative__DR)
/* Port Number */
#define RefRes_Negative_PRT_NUM                (* (reg8 *) RefRes_Negative__PRT) 
/* Connect to Analog Globals */                                                  
#define RefRes_Negative_AG                     (* (reg8 *) RefRes_Negative__AG)                       
/* Analog MUX bux enable */
#define RefRes_Negative_AMUX                   (* (reg8 *) RefRes_Negative__AMUX) 
/* Bidirectional Enable */                                                        
#define RefRes_Negative_BIE                    (* (reg8 *) RefRes_Negative__BIE)
/* Bit-mask for Aliased Register Access */
#define RefRes_Negative_BIT_MASK               (* (reg8 *) RefRes_Negative__BIT_MASK)
/* Bypass Enable */
#define RefRes_Negative_BYP                    (* (reg8 *) RefRes_Negative__BYP)
/* Port wide control signals */                                                   
#define RefRes_Negative_CTL                    (* (reg8 *) RefRes_Negative__CTL)
/* Drive Modes */
#define RefRes_Negative_DM0                    (* (reg8 *) RefRes_Negative__DM0) 
#define RefRes_Negative_DM1                    (* (reg8 *) RefRes_Negative__DM1)
#define RefRes_Negative_DM2                    (* (reg8 *) RefRes_Negative__DM2) 
/* Input Buffer Disable Override */
#define RefRes_Negative_INP_DIS                (* (reg8 *) RefRes_Negative__INP_DIS)
/* LCD Common or Segment Drive */
#define RefRes_Negative_LCD_COM_SEG            (* (reg8 *) RefRes_Negative__LCD_COM_SEG)
/* Enable Segment LCD */
#define RefRes_Negative_LCD_EN                 (* (reg8 *) RefRes_Negative__LCD_EN)
/* Slew Rate Control */
#define RefRes_Negative_SLW                    (* (reg8 *) RefRes_Negative__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define RefRes_Negative_PRTDSI__CAPS_SEL       (* (reg8 *) RefRes_Negative__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define RefRes_Negative_PRTDSI__DBL_SYNC_IN    (* (reg8 *) RefRes_Negative__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define RefRes_Negative_PRTDSI__OE_SEL0        (* (reg8 *) RefRes_Negative__PRTDSI__OE_SEL0) 
#define RefRes_Negative_PRTDSI__OE_SEL1        (* (reg8 *) RefRes_Negative__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define RefRes_Negative_PRTDSI__OUT_SEL0       (* (reg8 *) RefRes_Negative__PRTDSI__OUT_SEL0) 
#define RefRes_Negative_PRTDSI__OUT_SEL1       (* (reg8 *) RefRes_Negative__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define RefRes_Negative_PRTDSI__SYNC_OUT       (* (reg8 *) RefRes_Negative__PRTDSI__SYNC_OUT) 


#if defined(RefRes_Negative__INTSTAT)  /* Interrupt Registers */

    #define RefRes_Negative_INTSTAT                (* (reg8 *) RefRes_Negative__INTSTAT)
    #define RefRes_Negative_SNAP                   (* (reg8 *) RefRes_Negative__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_RefRes_Negative_H */


/* [] END OF FILE */
