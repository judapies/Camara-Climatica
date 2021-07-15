/*******************************************************************************
* File Name: CR8.h  
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

#if !defined(CY_PINS_CR8_H) /* Pins CR8_H */
#define CY_PINS_CR8_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "CR8_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 CR8__PORT == 15 && ((CR8__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    CR8_Write(uint8 value);
void    CR8_SetDriveMode(uint8 mode);
uint8   CR8_ReadDataReg(void);
uint8   CR8_Read(void);
void    CR8_SetInterruptMode(uint16 position, uint16 mode);
uint8   CR8_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the CR8_SetDriveMode() function.
     *  @{
     */
        #define CR8_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define CR8_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define CR8_DM_RES_UP          PIN_DM_RES_UP
        #define CR8_DM_RES_DWN         PIN_DM_RES_DWN
        #define CR8_DM_OD_LO           PIN_DM_OD_LO
        #define CR8_DM_OD_HI           PIN_DM_OD_HI
        #define CR8_DM_STRONG          PIN_DM_STRONG
        #define CR8_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define CR8_MASK               CR8__MASK
#define CR8_SHIFT              CR8__SHIFT
#define CR8_WIDTH              1u

/* Interrupt constants */
#if defined(CR8__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in CR8_SetInterruptMode() function.
     *  @{
     */
        #define CR8_INTR_NONE      (uint16)(0x0000u)
        #define CR8_INTR_RISING    (uint16)(0x0001u)
        #define CR8_INTR_FALLING   (uint16)(0x0002u)
        #define CR8_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define CR8_INTR_MASK      (0x01u) 
#endif /* (CR8__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define CR8_PS                     (* (reg8 *) CR8__PS)
/* Data Register */
#define CR8_DR                     (* (reg8 *) CR8__DR)
/* Port Number */
#define CR8_PRT_NUM                (* (reg8 *) CR8__PRT) 
/* Connect to Analog Globals */                                                  
#define CR8_AG                     (* (reg8 *) CR8__AG)                       
/* Analog MUX bux enable */
#define CR8_AMUX                   (* (reg8 *) CR8__AMUX) 
/* Bidirectional Enable */                                                        
#define CR8_BIE                    (* (reg8 *) CR8__BIE)
/* Bit-mask for Aliased Register Access */
#define CR8_BIT_MASK               (* (reg8 *) CR8__BIT_MASK)
/* Bypass Enable */
#define CR8_BYP                    (* (reg8 *) CR8__BYP)
/* Port wide control signals */                                                   
#define CR8_CTL                    (* (reg8 *) CR8__CTL)
/* Drive Modes */
#define CR8_DM0                    (* (reg8 *) CR8__DM0) 
#define CR8_DM1                    (* (reg8 *) CR8__DM1)
#define CR8_DM2                    (* (reg8 *) CR8__DM2) 
/* Input Buffer Disable Override */
#define CR8_INP_DIS                (* (reg8 *) CR8__INP_DIS)
/* LCD Common or Segment Drive */
#define CR8_LCD_COM_SEG            (* (reg8 *) CR8__LCD_COM_SEG)
/* Enable Segment LCD */
#define CR8_LCD_EN                 (* (reg8 *) CR8__LCD_EN)
/* Slew Rate Control */
#define CR8_SLW                    (* (reg8 *) CR8__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define CR8_PRTDSI__CAPS_SEL       (* (reg8 *) CR8__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define CR8_PRTDSI__DBL_SYNC_IN    (* (reg8 *) CR8__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define CR8_PRTDSI__OE_SEL0        (* (reg8 *) CR8__PRTDSI__OE_SEL0) 
#define CR8_PRTDSI__OE_SEL1        (* (reg8 *) CR8__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define CR8_PRTDSI__OUT_SEL0       (* (reg8 *) CR8__PRTDSI__OUT_SEL0) 
#define CR8_PRTDSI__OUT_SEL1       (* (reg8 *) CR8__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define CR8_PRTDSI__SYNC_OUT       (* (reg8 *) CR8__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(CR8__SIO_CFG)
    #define CR8_SIO_HYST_EN        (* (reg8 *) CR8__SIO_HYST_EN)
    #define CR8_SIO_REG_HIFREQ     (* (reg8 *) CR8__SIO_REG_HIFREQ)
    #define CR8_SIO_CFG            (* (reg8 *) CR8__SIO_CFG)
    #define CR8_SIO_DIFF           (* (reg8 *) CR8__SIO_DIFF)
#endif /* (CR8__SIO_CFG) */

/* Interrupt Registers */
#if defined(CR8__INTSTAT)
    #define CR8_INTSTAT            (* (reg8 *) CR8__INTSTAT)
    #define CR8_SNAP               (* (reg8 *) CR8__SNAP)
    
	#define CR8_0_INTTYPE_REG 		(* (reg8 *) CR8__0__INTTYPE)
#endif /* (CR8__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_CR8_H */


/* [] END OF FILE */
