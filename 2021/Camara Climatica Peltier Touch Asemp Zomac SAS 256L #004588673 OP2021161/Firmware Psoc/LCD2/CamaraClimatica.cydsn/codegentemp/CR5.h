/*******************************************************************************
* File Name: CR5.h  
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

#if !defined(CY_PINS_CR5_H) /* Pins CR5_H */
#define CY_PINS_CR5_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "CR5_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 CR5__PORT == 15 && ((CR5__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    CR5_Write(uint8 value);
void    CR5_SetDriveMode(uint8 mode);
uint8   CR5_ReadDataReg(void);
uint8   CR5_Read(void);
void    CR5_SetInterruptMode(uint16 position, uint16 mode);
uint8   CR5_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the CR5_SetDriveMode() function.
     *  @{
     */
        #define CR5_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define CR5_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define CR5_DM_RES_UP          PIN_DM_RES_UP
        #define CR5_DM_RES_DWN         PIN_DM_RES_DWN
        #define CR5_DM_OD_LO           PIN_DM_OD_LO
        #define CR5_DM_OD_HI           PIN_DM_OD_HI
        #define CR5_DM_STRONG          PIN_DM_STRONG
        #define CR5_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define CR5_MASK               CR5__MASK
#define CR5_SHIFT              CR5__SHIFT
#define CR5_WIDTH              1u

/* Interrupt constants */
#if defined(CR5__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in CR5_SetInterruptMode() function.
     *  @{
     */
        #define CR5_INTR_NONE      (uint16)(0x0000u)
        #define CR5_INTR_RISING    (uint16)(0x0001u)
        #define CR5_INTR_FALLING   (uint16)(0x0002u)
        #define CR5_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define CR5_INTR_MASK      (0x01u) 
#endif /* (CR5__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define CR5_PS                     (* (reg8 *) CR5__PS)
/* Data Register */
#define CR5_DR                     (* (reg8 *) CR5__DR)
/* Port Number */
#define CR5_PRT_NUM                (* (reg8 *) CR5__PRT) 
/* Connect to Analog Globals */                                                  
#define CR5_AG                     (* (reg8 *) CR5__AG)                       
/* Analog MUX bux enable */
#define CR5_AMUX                   (* (reg8 *) CR5__AMUX) 
/* Bidirectional Enable */                                                        
#define CR5_BIE                    (* (reg8 *) CR5__BIE)
/* Bit-mask for Aliased Register Access */
#define CR5_BIT_MASK               (* (reg8 *) CR5__BIT_MASK)
/* Bypass Enable */
#define CR5_BYP                    (* (reg8 *) CR5__BYP)
/* Port wide control signals */                                                   
#define CR5_CTL                    (* (reg8 *) CR5__CTL)
/* Drive Modes */
#define CR5_DM0                    (* (reg8 *) CR5__DM0) 
#define CR5_DM1                    (* (reg8 *) CR5__DM1)
#define CR5_DM2                    (* (reg8 *) CR5__DM2) 
/* Input Buffer Disable Override */
#define CR5_INP_DIS                (* (reg8 *) CR5__INP_DIS)
/* LCD Common or Segment Drive */
#define CR5_LCD_COM_SEG            (* (reg8 *) CR5__LCD_COM_SEG)
/* Enable Segment LCD */
#define CR5_LCD_EN                 (* (reg8 *) CR5__LCD_EN)
/* Slew Rate Control */
#define CR5_SLW                    (* (reg8 *) CR5__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define CR5_PRTDSI__CAPS_SEL       (* (reg8 *) CR5__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define CR5_PRTDSI__DBL_SYNC_IN    (* (reg8 *) CR5__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define CR5_PRTDSI__OE_SEL0        (* (reg8 *) CR5__PRTDSI__OE_SEL0) 
#define CR5_PRTDSI__OE_SEL1        (* (reg8 *) CR5__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define CR5_PRTDSI__OUT_SEL0       (* (reg8 *) CR5__PRTDSI__OUT_SEL0) 
#define CR5_PRTDSI__OUT_SEL1       (* (reg8 *) CR5__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define CR5_PRTDSI__SYNC_OUT       (* (reg8 *) CR5__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(CR5__SIO_CFG)
    #define CR5_SIO_HYST_EN        (* (reg8 *) CR5__SIO_HYST_EN)
    #define CR5_SIO_REG_HIFREQ     (* (reg8 *) CR5__SIO_REG_HIFREQ)
    #define CR5_SIO_CFG            (* (reg8 *) CR5__SIO_CFG)
    #define CR5_SIO_DIFF           (* (reg8 *) CR5__SIO_DIFF)
#endif /* (CR5__SIO_CFG) */

/* Interrupt Registers */
#if defined(CR5__INTSTAT)
    #define CR5_INTSTAT            (* (reg8 *) CR5__INTSTAT)
    #define CR5_SNAP               (* (reg8 *) CR5__SNAP)
    
	#define CR5_0_INTTYPE_REG 		(* (reg8 *) CR5__0__INTTYPE)
#endif /* (CR5__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_CR5_H */


/* [] END OF FILE */
