/*******************************************************************************
* File Name: CR4.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_CR4_H) /* Pins CR4_H */
#define CY_PINS_CR4_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "CR4_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 CR4__PORT == 15 && ((CR4__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    CR4_Write(uint8 value);
void    CR4_SetDriveMode(uint8 mode);
uint8   CR4_ReadDataReg(void);
uint8   CR4_Read(void);
void    CR4_SetInterruptMode(uint16 position, uint16 mode);
uint8   CR4_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the CR4_SetDriveMode() function.
     *  @{
     */
        #define CR4_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define CR4_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define CR4_DM_RES_UP          PIN_DM_RES_UP
        #define CR4_DM_RES_DWN         PIN_DM_RES_DWN
        #define CR4_DM_OD_LO           PIN_DM_OD_LO
        #define CR4_DM_OD_HI           PIN_DM_OD_HI
        #define CR4_DM_STRONG          PIN_DM_STRONG
        #define CR4_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define CR4_MASK               CR4__MASK
#define CR4_SHIFT              CR4__SHIFT
#define CR4_WIDTH              1u

/* Interrupt constants */
#if defined(CR4__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in CR4_SetInterruptMode() function.
     *  @{
     */
        #define CR4_INTR_NONE      (uint16)(0x0000u)
        #define CR4_INTR_RISING    (uint16)(0x0001u)
        #define CR4_INTR_FALLING   (uint16)(0x0002u)
        #define CR4_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define CR4_INTR_MASK      (0x01u) 
#endif /* (CR4__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define CR4_PS                     (* (reg8 *) CR4__PS)
/* Data Register */
#define CR4_DR                     (* (reg8 *) CR4__DR)
/* Port Number */
#define CR4_PRT_NUM                (* (reg8 *) CR4__PRT) 
/* Connect to Analog Globals */                                                  
#define CR4_AG                     (* (reg8 *) CR4__AG)                       
/* Analog MUX bux enable */
#define CR4_AMUX                   (* (reg8 *) CR4__AMUX) 
/* Bidirectional Enable */                                                        
#define CR4_BIE                    (* (reg8 *) CR4__BIE)
/* Bit-mask for Aliased Register Access */
#define CR4_BIT_MASK               (* (reg8 *) CR4__BIT_MASK)
/* Bypass Enable */
#define CR4_BYP                    (* (reg8 *) CR4__BYP)
/* Port wide control signals */                                                   
#define CR4_CTL                    (* (reg8 *) CR4__CTL)
/* Drive Modes */
#define CR4_DM0                    (* (reg8 *) CR4__DM0) 
#define CR4_DM1                    (* (reg8 *) CR4__DM1)
#define CR4_DM2                    (* (reg8 *) CR4__DM2) 
/* Input Buffer Disable Override */
#define CR4_INP_DIS                (* (reg8 *) CR4__INP_DIS)
/* LCD Common or Segment Drive */
#define CR4_LCD_COM_SEG            (* (reg8 *) CR4__LCD_COM_SEG)
/* Enable Segment LCD */
#define CR4_LCD_EN                 (* (reg8 *) CR4__LCD_EN)
/* Slew Rate Control */
#define CR4_SLW                    (* (reg8 *) CR4__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define CR4_PRTDSI__CAPS_SEL       (* (reg8 *) CR4__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define CR4_PRTDSI__DBL_SYNC_IN    (* (reg8 *) CR4__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define CR4_PRTDSI__OE_SEL0        (* (reg8 *) CR4__PRTDSI__OE_SEL0) 
#define CR4_PRTDSI__OE_SEL1        (* (reg8 *) CR4__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define CR4_PRTDSI__OUT_SEL0       (* (reg8 *) CR4__PRTDSI__OUT_SEL0) 
#define CR4_PRTDSI__OUT_SEL1       (* (reg8 *) CR4__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define CR4_PRTDSI__SYNC_OUT       (* (reg8 *) CR4__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(CR4__SIO_CFG)
    #define CR4_SIO_HYST_EN        (* (reg8 *) CR4__SIO_HYST_EN)
    #define CR4_SIO_REG_HIFREQ     (* (reg8 *) CR4__SIO_REG_HIFREQ)
    #define CR4_SIO_CFG            (* (reg8 *) CR4__SIO_CFG)
    #define CR4_SIO_DIFF           (* (reg8 *) CR4__SIO_DIFF)
#endif /* (CR4__SIO_CFG) */

/* Interrupt Registers */
#if defined(CR4__INTSTAT)
    #define CR4_INTSTAT            (* (reg8 *) CR4__INTSTAT)
    #define CR4_SNAP               (* (reg8 *) CR4__SNAP)
    
	#define CR4_0_INTTYPE_REG 		(* (reg8 *) CR4__0__INTTYPE)
#endif /* (CR4__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_CR4_H */


/* [] END OF FILE */
