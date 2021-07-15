/*******************************************************************************
* File Name: CR7.h  
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

#if !defined(CY_PINS_CR7_H) /* Pins CR7_H */
#define CY_PINS_CR7_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "CR7_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 CR7__PORT == 15 && ((CR7__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    CR7_Write(uint8 value);
void    CR7_SetDriveMode(uint8 mode);
uint8   CR7_ReadDataReg(void);
uint8   CR7_Read(void);
void    CR7_SetInterruptMode(uint16 position, uint16 mode);
uint8   CR7_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the CR7_SetDriveMode() function.
     *  @{
     */
        #define CR7_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define CR7_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define CR7_DM_RES_UP          PIN_DM_RES_UP
        #define CR7_DM_RES_DWN         PIN_DM_RES_DWN
        #define CR7_DM_OD_LO           PIN_DM_OD_LO
        #define CR7_DM_OD_HI           PIN_DM_OD_HI
        #define CR7_DM_STRONG          PIN_DM_STRONG
        #define CR7_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define CR7_MASK               CR7__MASK
#define CR7_SHIFT              CR7__SHIFT
#define CR7_WIDTH              1u

/* Interrupt constants */
#if defined(CR7__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in CR7_SetInterruptMode() function.
     *  @{
     */
        #define CR7_INTR_NONE      (uint16)(0x0000u)
        #define CR7_INTR_RISING    (uint16)(0x0001u)
        #define CR7_INTR_FALLING   (uint16)(0x0002u)
        #define CR7_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define CR7_INTR_MASK      (0x01u) 
#endif /* (CR7__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define CR7_PS                     (* (reg8 *) CR7__PS)
/* Data Register */
#define CR7_DR                     (* (reg8 *) CR7__DR)
/* Port Number */
#define CR7_PRT_NUM                (* (reg8 *) CR7__PRT) 
/* Connect to Analog Globals */                                                  
#define CR7_AG                     (* (reg8 *) CR7__AG)                       
/* Analog MUX bux enable */
#define CR7_AMUX                   (* (reg8 *) CR7__AMUX) 
/* Bidirectional Enable */                                                        
#define CR7_BIE                    (* (reg8 *) CR7__BIE)
/* Bit-mask for Aliased Register Access */
#define CR7_BIT_MASK               (* (reg8 *) CR7__BIT_MASK)
/* Bypass Enable */
#define CR7_BYP                    (* (reg8 *) CR7__BYP)
/* Port wide control signals */                                                   
#define CR7_CTL                    (* (reg8 *) CR7__CTL)
/* Drive Modes */
#define CR7_DM0                    (* (reg8 *) CR7__DM0) 
#define CR7_DM1                    (* (reg8 *) CR7__DM1)
#define CR7_DM2                    (* (reg8 *) CR7__DM2) 
/* Input Buffer Disable Override */
#define CR7_INP_DIS                (* (reg8 *) CR7__INP_DIS)
/* LCD Common or Segment Drive */
#define CR7_LCD_COM_SEG            (* (reg8 *) CR7__LCD_COM_SEG)
/* Enable Segment LCD */
#define CR7_LCD_EN                 (* (reg8 *) CR7__LCD_EN)
/* Slew Rate Control */
#define CR7_SLW                    (* (reg8 *) CR7__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define CR7_PRTDSI__CAPS_SEL       (* (reg8 *) CR7__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define CR7_PRTDSI__DBL_SYNC_IN    (* (reg8 *) CR7__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define CR7_PRTDSI__OE_SEL0        (* (reg8 *) CR7__PRTDSI__OE_SEL0) 
#define CR7_PRTDSI__OE_SEL1        (* (reg8 *) CR7__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define CR7_PRTDSI__OUT_SEL0       (* (reg8 *) CR7__PRTDSI__OUT_SEL0) 
#define CR7_PRTDSI__OUT_SEL1       (* (reg8 *) CR7__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define CR7_PRTDSI__SYNC_OUT       (* (reg8 *) CR7__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(CR7__SIO_CFG)
    #define CR7_SIO_HYST_EN        (* (reg8 *) CR7__SIO_HYST_EN)
    #define CR7_SIO_REG_HIFREQ     (* (reg8 *) CR7__SIO_REG_HIFREQ)
    #define CR7_SIO_CFG            (* (reg8 *) CR7__SIO_CFG)
    #define CR7_SIO_DIFF           (* (reg8 *) CR7__SIO_DIFF)
#endif /* (CR7__SIO_CFG) */

/* Interrupt Registers */
#if defined(CR7__INTSTAT)
    #define CR7_INTSTAT            (* (reg8 *) CR7__INTSTAT)
    #define CR7_SNAP               (* (reg8 *) CR7__SNAP)
    
	#define CR7_0_INTTYPE_REG 		(* (reg8 *) CR7__0__INTTYPE)
#endif /* (CR7__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_CR7_H */


/* [] END OF FILE */
