/*******************************************************************************
* File Name: CR6.h  
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

#if !defined(CY_PINS_CR6_H) /* Pins CR6_H */
#define CY_PINS_CR6_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "CR6_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 CR6__PORT == 15 && ((CR6__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    CR6_Write(uint8 value);
void    CR6_SetDriveMode(uint8 mode);
uint8   CR6_ReadDataReg(void);
uint8   CR6_Read(void);
void    CR6_SetInterruptMode(uint16 position, uint16 mode);
uint8   CR6_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the CR6_SetDriveMode() function.
     *  @{
     */
        #define CR6_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define CR6_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define CR6_DM_RES_UP          PIN_DM_RES_UP
        #define CR6_DM_RES_DWN         PIN_DM_RES_DWN
        #define CR6_DM_OD_LO           PIN_DM_OD_LO
        #define CR6_DM_OD_HI           PIN_DM_OD_HI
        #define CR6_DM_STRONG          PIN_DM_STRONG
        #define CR6_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define CR6_MASK               CR6__MASK
#define CR6_SHIFT              CR6__SHIFT
#define CR6_WIDTH              1u

/* Interrupt constants */
#if defined(CR6__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in CR6_SetInterruptMode() function.
     *  @{
     */
        #define CR6_INTR_NONE      (uint16)(0x0000u)
        #define CR6_INTR_RISING    (uint16)(0x0001u)
        #define CR6_INTR_FALLING   (uint16)(0x0002u)
        #define CR6_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define CR6_INTR_MASK      (0x01u) 
#endif /* (CR6__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define CR6_PS                     (* (reg8 *) CR6__PS)
/* Data Register */
#define CR6_DR                     (* (reg8 *) CR6__DR)
/* Port Number */
#define CR6_PRT_NUM                (* (reg8 *) CR6__PRT) 
/* Connect to Analog Globals */                                                  
#define CR6_AG                     (* (reg8 *) CR6__AG)                       
/* Analog MUX bux enable */
#define CR6_AMUX                   (* (reg8 *) CR6__AMUX) 
/* Bidirectional Enable */                                                        
#define CR6_BIE                    (* (reg8 *) CR6__BIE)
/* Bit-mask for Aliased Register Access */
#define CR6_BIT_MASK               (* (reg8 *) CR6__BIT_MASK)
/* Bypass Enable */
#define CR6_BYP                    (* (reg8 *) CR6__BYP)
/* Port wide control signals */                                                   
#define CR6_CTL                    (* (reg8 *) CR6__CTL)
/* Drive Modes */
#define CR6_DM0                    (* (reg8 *) CR6__DM0) 
#define CR6_DM1                    (* (reg8 *) CR6__DM1)
#define CR6_DM2                    (* (reg8 *) CR6__DM2) 
/* Input Buffer Disable Override */
#define CR6_INP_DIS                (* (reg8 *) CR6__INP_DIS)
/* LCD Common or Segment Drive */
#define CR6_LCD_COM_SEG            (* (reg8 *) CR6__LCD_COM_SEG)
/* Enable Segment LCD */
#define CR6_LCD_EN                 (* (reg8 *) CR6__LCD_EN)
/* Slew Rate Control */
#define CR6_SLW                    (* (reg8 *) CR6__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define CR6_PRTDSI__CAPS_SEL       (* (reg8 *) CR6__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define CR6_PRTDSI__DBL_SYNC_IN    (* (reg8 *) CR6__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define CR6_PRTDSI__OE_SEL0        (* (reg8 *) CR6__PRTDSI__OE_SEL0) 
#define CR6_PRTDSI__OE_SEL1        (* (reg8 *) CR6__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define CR6_PRTDSI__OUT_SEL0       (* (reg8 *) CR6__PRTDSI__OUT_SEL0) 
#define CR6_PRTDSI__OUT_SEL1       (* (reg8 *) CR6__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define CR6_PRTDSI__SYNC_OUT       (* (reg8 *) CR6__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(CR6__SIO_CFG)
    #define CR6_SIO_HYST_EN        (* (reg8 *) CR6__SIO_HYST_EN)
    #define CR6_SIO_REG_HIFREQ     (* (reg8 *) CR6__SIO_REG_HIFREQ)
    #define CR6_SIO_CFG            (* (reg8 *) CR6__SIO_CFG)
    #define CR6_SIO_DIFF           (* (reg8 *) CR6__SIO_DIFF)
#endif /* (CR6__SIO_CFG) */

/* Interrupt Registers */
#if defined(CR6__INTSTAT)
    #define CR6_INTSTAT            (* (reg8 *) CR6__INTSTAT)
    #define CR6_SNAP               (* (reg8 *) CR6__SNAP)
    
	#define CR6_0_INTTYPE_REG 		(* (reg8 *) CR6__0__INTTYPE)
#endif /* (CR6__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_CR6_H */


/* [] END OF FILE */
