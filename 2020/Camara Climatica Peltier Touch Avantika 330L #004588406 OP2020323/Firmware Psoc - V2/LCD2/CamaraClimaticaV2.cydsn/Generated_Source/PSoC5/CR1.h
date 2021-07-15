/*******************************************************************************
* File Name: CR1.h  
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

#if !defined(CY_PINS_CR1_H) /* Pins CR1_H */
#define CY_PINS_CR1_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "CR1_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 CR1__PORT == 15 && ((CR1__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    CR1_Write(uint8 value);
void    CR1_SetDriveMode(uint8 mode);
uint8   CR1_ReadDataReg(void);
uint8   CR1_Read(void);
void    CR1_SetInterruptMode(uint16 position, uint16 mode);
uint8   CR1_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the CR1_SetDriveMode() function.
     *  @{
     */
        #define CR1_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define CR1_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define CR1_DM_RES_UP          PIN_DM_RES_UP
        #define CR1_DM_RES_DWN         PIN_DM_RES_DWN
        #define CR1_DM_OD_LO           PIN_DM_OD_LO
        #define CR1_DM_OD_HI           PIN_DM_OD_HI
        #define CR1_DM_STRONG          PIN_DM_STRONG
        #define CR1_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define CR1_MASK               CR1__MASK
#define CR1_SHIFT              CR1__SHIFT
#define CR1_WIDTH              1u

/* Interrupt constants */
#if defined(CR1__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in CR1_SetInterruptMode() function.
     *  @{
     */
        #define CR1_INTR_NONE      (uint16)(0x0000u)
        #define CR1_INTR_RISING    (uint16)(0x0001u)
        #define CR1_INTR_FALLING   (uint16)(0x0002u)
        #define CR1_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define CR1_INTR_MASK      (0x01u) 
#endif /* (CR1__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define CR1_PS                     (* (reg8 *) CR1__PS)
/* Data Register */
#define CR1_DR                     (* (reg8 *) CR1__DR)
/* Port Number */
#define CR1_PRT_NUM                (* (reg8 *) CR1__PRT) 
/* Connect to Analog Globals */                                                  
#define CR1_AG                     (* (reg8 *) CR1__AG)                       
/* Analog MUX bux enable */
#define CR1_AMUX                   (* (reg8 *) CR1__AMUX) 
/* Bidirectional Enable */                                                        
#define CR1_BIE                    (* (reg8 *) CR1__BIE)
/* Bit-mask for Aliased Register Access */
#define CR1_BIT_MASK               (* (reg8 *) CR1__BIT_MASK)
/* Bypass Enable */
#define CR1_BYP                    (* (reg8 *) CR1__BYP)
/* Port wide control signals */                                                   
#define CR1_CTL                    (* (reg8 *) CR1__CTL)
/* Drive Modes */
#define CR1_DM0                    (* (reg8 *) CR1__DM0) 
#define CR1_DM1                    (* (reg8 *) CR1__DM1)
#define CR1_DM2                    (* (reg8 *) CR1__DM2) 
/* Input Buffer Disable Override */
#define CR1_INP_DIS                (* (reg8 *) CR1__INP_DIS)
/* LCD Common or Segment Drive */
#define CR1_LCD_COM_SEG            (* (reg8 *) CR1__LCD_COM_SEG)
/* Enable Segment LCD */
#define CR1_LCD_EN                 (* (reg8 *) CR1__LCD_EN)
/* Slew Rate Control */
#define CR1_SLW                    (* (reg8 *) CR1__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define CR1_PRTDSI__CAPS_SEL       (* (reg8 *) CR1__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define CR1_PRTDSI__DBL_SYNC_IN    (* (reg8 *) CR1__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define CR1_PRTDSI__OE_SEL0        (* (reg8 *) CR1__PRTDSI__OE_SEL0) 
#define CR1_PRTDSI__OE_SEL1        (* (reg8 *) CR1__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define CR1_PRTDSI__OUT_SEL0       (* (reg8 *) CR1__PRTDSI__OUT_SEL0) 
#define CR1_PRTDSI__OUT_SEL1       (* (reg8 *) CR1__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define CR1_PRTDSI__SYNC_OUT       (* (reg8 *) CR1__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(CR1__SIO_CFG)
    #define CR1_SIO_HYST_EN        (* (reg8 *) CR1__SIO_HYST_EN)
    #define CR1_SIO_REG_HIFREQ     (* (reg8 *) CR1__SIO_REG_HIFREQ)
    #define CR1_SIO_CFG            (* (reg8 *) CR1__SIO_CFG)
    #define CR1_SIO_DIFF           (* (reg8 *) CR1__SIO_DIFF)
#endif /* (CR1__SIO_CFG) */

/* Interrupt Registers */
#if defined(CR1__INTSTAT)
    #define CR1_INTSTAT            (* (reg8 *) CR1__INTSTAT)
    #define CR1_SNAP               (* (reg8 *) CR1__SNAP)
    
	#define CR1_0_INTTYPE_REG 		(* (reg8 *) CR1__0__INTTYPE)
#endif /* (CR1__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_CR1_H */


/* [] END OF FILE */
