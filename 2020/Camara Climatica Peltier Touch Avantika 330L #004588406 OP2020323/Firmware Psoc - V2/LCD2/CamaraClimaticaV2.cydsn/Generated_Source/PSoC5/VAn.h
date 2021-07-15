/*******************************************************************************
* File Name: VAn.h  
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

#if !defined(CY_PINS_VAn_H) /* Pins VAn_H */
#define CY_PINS_VAn_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "VAn_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 VAn__PORT == 15 && ((VAn__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    VAn_Write(uint8 value);
void    VAn_SetDriveMode(uint8 mode);
uint8   VAn_ReadDataReg(void);
uint8   VAn_Read(void);
void    VAn_SetInterruptMode(uint16 position, uint16 mode);
uint8   VAn_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the VAn_SetDriveMode() function.
     *  @{
     */
        #define VAn_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define VAn_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define VAn_DM_RES_UP          PIN_DM_RES_UP
        #define VAn_DM_RES_DWN         PIN_DM_RES_DWN
        #define VAn_DM_OD_LO           PIN_DM_OD_LO
        #define VAn_DM_OD_HI           PIN_DM_OD_HI
        #define VAn_DM_STRONG          PIN_DM_STRONG
        #define VAn_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define VAn_MASK               VAn__MASK
#define VAn_SHIFT              VAn__SHIFT
#define VAn_WIDTH              1u

/* Interrupt constants */
#if defined(VAn__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in VAn_SetInterruptMode() function.
     *  @{
     */
        #define VAn_INTR_NONE      (uint16)(0x0000u)
        #define VAn_INTR_RISING    (uint16)(0x0001u)
        #define VAn_INTR_FALLING   (uint16)(0x0002u)
        #define VAn_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define VAn_INTR_MASK      (0x01u) 
#endif /* (VAn__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define VAn_PS                     (* (reg8 *) VAn__PS)
/* Data Register */
#define VAn_DR                     (* (reg8 *) VAn__DR)
/* Port Number */
#define VAn_PRT_NUM                (* (reg8 *) VAn__PRT) 
/* Connect to Analog Globals */                                                  
#define VAn_AG                     (* (reg8 *) VAn__AG)                       
/* Analog MUX bux enable */
#define VAn_AMUX                   (* (reg8 *) VAn__AMUX) 
/* Bidirectional Enable */                                                        
#define VAn_BIE                    (* (reg8 *) VAn__BIE)
/* Bit-mask for Aliased Register Access */
#define VAn_BIT_MASK               (* (reg8 *) VAn__BIT_MASK)
/* Bypass Enable */
#define VAn_BYP                    (* (reg8 *) VAn__BYP)
/* Port wide control signals */                                                   
#define VAn_CTL                    (* (reg8 *) VAn__CTL)
/* Drive Modes */
#define VAn_DM0                    (* (reg8 *) VAn__DM0) 
#define VAn_DM1                    (* (reg8 *) VAn__DM1)
#define VAn_DM2                    (* (reg8 *) VAn__DM2) 
/* Input Buffer Disable Override */
#define VAn_INP_DIS                (* (reg8 *) VAn__INP_DIS)
/* LCD Common or Segment Drive */
#define VAn_LCD_COM_SEG            (* (reg8 *) VAn__LCD_COM_SEG)
/* Enable Segment LCD */
#define VAn_LCD_EN                 (* (reg8 *) VAn__LCD_EN)
/* Slew Rate Control */
#define VAn_SLW                    (* (reg8 *) VAn__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define VAn_PRTDSI__CAPS_SEL       (* (reg8 *) VAn__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define VAn_PRTDSI__DBL_SYNC_IN    (* (reg8 *) VAn__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define VAn_PRTDSI__OE_SEL0        (* (reg8 *) VAn__PRTDSI__OE_SEL0) 
#define VAn_PRTDSI__OE_SEL1        (* (reg8 *) VAn__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define VAn_PRTDSI__OUT_SEL0       (* (reg8 *) VAn__PRTDSI__OUT_SEL0) 
#define VAn_PRTDSI__OUT_SEL1       (* (reg8 *) VAn__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define VAn_PRTDSI__SYNC_OUT       (* (reg8 *) VAn__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(VAn__SIO_CFG)
    #define VAn_SIO_HYST_EN        (* (reg8 *) VAn__SIO_HYST_EN)
    #define VAn_SIO_REG_HIFREQ     (* (reg8 *) VAn__SIO_REG_HIFREQ)
    #define VAn_SIO_CFG            (* (reg8 *) VAn__SIO_CFG)
    #define VAn_SIO_DIFF           (* (reg8 *) VAn__SIO_DIFF)
#endif /* (VAn__SIO_CFG) */

/* Interrupt Registers */
#if defined(VAn__INTSTAT)
    #define VAn_INTSTAT            (* (reg8 *) VAn__INTSTAT)
    #define VAn_SNAP               (* (reg8 *) VAn__SNAP)
    
	#define VAn_0_INTTYPE_REG 		(* (reg8 *) VAn__0__INTTYPE)
#endif /* (VAn__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_VAn_H */


/* [] END OF FILE */
