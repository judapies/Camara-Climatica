/*******************************************************************************
* File Name: RTD_Negative.h  
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

#if !defined(CY_PINS_RTD_Negative_H) /* Pins RTD_Negative_H */
#define CY_PINS_RTD_Negative_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "RTD_Negative_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 RTD_Negative__PORT == 15 && ((RTD_Negative__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    RTD_Negative_Write(uint8 value) ;
void    RTD_Negative_SetDriveMode(uint8 mode) ;
uint8   RTD_Negative_ReadDataReg(void) ;
uint8   RTD_Negative_Read(void) ;
uint8   RTD_Negative_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define RTD_Negative_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define RTD_Negative_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define RTD_Negative_DM_RES_UP          PIN_DM_RES_UP
#define RTD_Negative_DM_RES_DWN         PIN_DM_RES_DWN
#define RTD_Negative_DM_OD_LO           PIN_DM_OD_LO
#define RTD_Negative_DM_OD_HI           PIN_DM_OD_HI
#define RTD_Negative_DM_STRONG          PIN_DM_STRONG
#define RTD_Negative_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define RTD_Negative_MASK               RTD_Negative__MASK
#define RTD_Negative_SHIFT              RTD_Negative__SHIFT
#define RTD_Negative_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define RTD_Negative_PS                     (* (reg8 *) RTD_Negative__PS)
/* Data Register */
#define RTD_Negative_DR                     (* (reg8 *) RTD_Negative__DR)
/* Port Number */
#define RTD_Negative_PRT_NUM                (* (reg8 *) RTD_Negative__PRT) 
/* Connect to Analog Globals */                                                  
#define RTD_Negative_AG                     (* (reg8 *) RTD_Negative__AG)                       
/* Analog MUX bux enable */
#define RTD_Negative_AMUX                   (* (reg8 *) RTD_Negative__AMUX) 
/* Bidirectional Enable */                                                        
#define RTD_Negative_BIE                    (* (reg8 *) RTD_Negative__BIE)
/* Bit-mask for Aliased Register Access */
#define RTD_Negative_BIT_MASK               (* (reg8 *) RTD_Negative__BIT_MASK)
/* Bypass Enable */
#define RTD_Negative_BYP                    (* (reg8 *) RTD_Negative__BYP)
/* Port wide control signals */                                                   
#define RTD_Negative_CTL                    (* (reg8 *) RTD_Negative__CTL)
/* Drive Modes */
#define RTD_Negative_DM0                    (* (reg8 *) RTD_Negative__DM0) 
#define RTD_Negative_DM1                    (* (reg8 *) RTD_Negative__DM1)
#define RTD_Negative_DM2                    (* (reg8 *) RTD_Negative__DM2) 
/* Input Buffer Disable Override */
#define RTD_Negative_INP_DIS                (* (reg8 *) RTD_Negative__INP_DIS)
/* LCD Common or Segment Drive */
#define RTD_Negative_LCD_COM_SEG            (* (reg8 *) RTD_Negative__LCD_COM_SEG)
/* Enable Segment LCD */
#define RTD_Negative_LCD_EN                 (* (reg8 *) RTD_Negative__LCD_EN)
/* Slew Rate Control */
#define RTD_Negative_SLW                    (* (reg8 *) RTD_Negative__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define RTD_Negative_PRTDSI__CAPS_SEL       (* (reg8 *) RTD_Negative__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define RTD_Negative_PRTDSI__DBL_SYNC_IN    (* (reg8 *) RTD_Negative__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define RTD_Negative_PRTDSI__OE_SEL0        (* (reg8 *) RTD_Negative__PRTDSI__OE_SEL0) 
#define RTD_Negative_PRTDSI__OE_SEL1        (* (reg8 *) RTD_Negative__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define RTD_Negative_PRTDSI__OUT_SEL0       (* (reg8 *) RTD_Negative__PRTDSI__OUT_SEL0) 
#define RTD_Negative_PRTDSI__OUT_SEL1       (* (reg8 *) RTD_Negative__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define RTD_Negative_PRTDSI__SYNC_OUT       (* (reg8 *) RTD_Negative__PRTDSI__SYNC_OUT) 


#if defined(RTD_Negative__INTSTAT)  /* Interrupt Registers */

    #define RTD_Negative_INTSTAT                (* (reg8 *) RTD_Negative__INTSTAT)
    #define RTD_Negative_SNAP                   (* (reg8 *) RTD_Negative__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_RTD_Negative_H */


/* [] END OF FILE */
