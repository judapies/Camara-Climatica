/*******************************************************************************
* File Name: RTD_Positive.h  
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

#if !defined(CY_PINS_RTD_Positive_H) /* Pins RTD_Positive_H */
#define CY_PINS_RTD_Positive_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "RTD_Positive_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 RTD_Positive__PORT == 15 && ((RTD_Positive__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    RTD_Positive_Write(uint8 value) ;
void    RTD_Positive_SetDriveMode(uint8 mode) ;
uint8   RTD_Positive_ReadDataReg(void) ;
uint8   RTD_Positive_Read(void) ;
uint8   RTD_Positive_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define RTD_Positive_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define RTD_Positive_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define RTD_Positive_DM_RES_UP          PIN_DM_RES_UP
#define RTD_Positive_DM_RES_DWN         PIN_DM_RES_DWN
#define RTD_Positive_DM_OD_LO           PIN_DM_OD_LO
#define RTD_Positive_DM_OD_HI           PIN_DM_OD_HI
#define RTD_Positive_DM_STRONG          PIN_DM_STRONG
#define RTD_Positive_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define RTD_Positive_MASK               RTD_Positive__MASK
#define RTD_Positive_SHIFT              RTD_Positive__SHIFT
#define RTD_Positive_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define RTD_Positive_PS                     (* (reg8 *) RTD_Positive__PS)
/* Data Register */
#define RTD_Positive_DR                     (* (reg8 *) RTD_Positive__DR)
/* Port Number */
#define RTD_Positive_PRT_NUM                (* (reg8 *) RTD_Positive__PRT) 
/* Connect to Analog Globals */                                                  
#define RTD_Positive_AG                     (* (reg8 *) RTD_Positive__AG)                       
/* Analog MUX bux enable */
#define RTD_Positive_AMUX                   (* (reg8 *) RTD_Positive__AMUX) 
/* Bidirectional Enable */                                                        
#define RTD_Positive_BIE                    (* (reg8 *) RTD_Positive__BIE)
/* Bit-mask for Aliased Register Access */
#define RTD_Positive_BIT_MASK               (* (reg8 *) RTD_Positive__BIT_MASK)
/* Bypass Enable */
#define RTD_Positive_BYP                    (* (reg8 *) RTD_Positive__BYP)
/* Port wide control signals */                                                   
#define RTD_Positive_CTL                    (* (reg8 *) RTD_Positive__CTL)
/* Drive Modes */
#define RTD_Positive_DM0                    (* (reg8 *) RTD_Positive__DM0) 
#define RTD_Positive_DM1                    (* (reg8 *) RTD_Positive__DM1)
#define RTD_Positive_DM2                    (* (reg8 *) RTD_Positive__DM2) 
/* Input Buffer Disable Override */
#define RTD_Positive_INP_DIS                (* (reg8 *) RTD_Positive__INP_DIS)
/* LCD Common or Segment Drive */
#define RTD_Positive_LCD_COM_SEG            (* (reg8 *) RTD_Positive__LCD_COM_SEG)
/* Enable Segment LCD */
#define RTD_Positive_LCD_EN                 (* (reg8 *) RTD_Positive__LCD_EN)
/* Slew Rate Control */
#define RTD_Positive_SLW                    (* (reg8 *) RTD_Positive__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define RTD_Positive_PRTDSI__CAPS_SEL       (* (reg8 *) RTD_Positive__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define RTD_Positive_PRTDSI__DBL_SYNC_IN    (* (reg8 *) RTD_Positive__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define RTD_Positive_PRTDSI__OE_SEL0        (* (reg8 *) RTD_Positive__PRTDSI__OE_SEL0) 
#define RTD_Positive_PRTDSI__OE_SEL1        (* (reg8 *) RTD_Positive__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define RTD_Positive_PRTDSI__OUT_SEL0       (* (reg8 *) RTD_Positive__PRTDSI__OUT_SEL0) 
#define RTD_Positive_PRTDSI__OUT_SEL1       (* (reg8 *) RTD_Positive__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define RTD_Positive_PRTDSI__SYNC_OUT       (* (reg8 *) RTD_Positive__PRTDSI__SYNC_OUT) 


#if defined(RTD_Positive__INTSTAT)  /* Interrupt Registers */

    #define RTD_Positive_INTSTAT                (* (reg8 *) RTD_Positive__INTSTAT)
    #define RTD_Positive_SNAP                   (* (reg8 *) RTD_Positive__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_RTD_Positive_H */


/* [] END OF FILE */
