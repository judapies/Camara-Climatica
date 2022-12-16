/*******************************************************************************
* File Name: LUZ.h  
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

#if !defined(CY_PINS_LUZ_H) /* Pins LUZ_H */
#define CY_PINS_LUZ_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "LUZ_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 LUZ__PORT == 15 && ((LUZ__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    LUZ_Write(uint8 value);
void    LUZ_SetDriveMode(uint8 mode);
uint8   LUZ_ReadDataReg(void);
uint8   LUZ_Read(void);
void    LUZ_SetInterruptMode(uint16 position, uint16 mode);
uint8   LUZ_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the LUZ_SetDriveMode() function.
     *  @{
     */
        #define LUZ_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define LUZ_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define LUZ_DM_RES_UP          PIN_DM_RES_UP
        #define LUZ_DM_RES_DWN         PIN_DM_RES_DWN
        #define LUZ_DM_OD_LO           PIN_DM_OD_LO
        #define LUZ_DM_OD_HI           PIN_DM_OD_HI
        #define LUZ_DM_STRONG          PIN_DM_STRONG
        #define LUZ_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define LUZ_MASK               LUZ__MASK
#define LUZ_SHIFT              LUZ__SHIFT
#define LUZ_WIDTH              1u

/* Interrupt constants */
#if defined(LUZ__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in LUZ_SetInterruptMode() function.
     *  @{
     */
        #define LUZ_INTR_NONE      (uint16)(0x0000u)
        #define LUZ_INTR_RISING    (uint16)(0x0001u)
        #define LUZ_INTR_FALLING   (uint16)(0x0002u)
        #define LUZ_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define LUZ_INTR_MASK      (0x01u) 
#endif /* (LUZ__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define LUZ_PS                     (* (reg8 *) LUZ__PS)
/* Data Register */
#define LUZ_DR                     (* (reg8 *) LUZ__DR)
/* Port Number */
#define LUZ_PRT_NUM                (* (reg8 *) LUZ__PRT) 
/* Connect to Analog Globals */                                                  
#define LUZ_AG                     (* (reg8 *) LUZ__AG)                       
/* Analog MUX bux enable */
#define LUZ_AMUX                   (* (reg8 *) LUZ__AMUX) 
/* Bidirectional Enable */                                                        
#define LUZ_BIE                    (* (reg8 *) LUZ__BIE)
/* Bit-mask for Aliased Register Access */
#define LUZ_BIT_MASK               (* (reg8 *) LUZ__BIT_MASK)
/* Bypass Enable */
#define LUZ_BYP                    (* (reg8 *) LUZ__BYP)
/* Port wide control signals */                                                   
#define LUZ_CTL                    (* (reg8 *) LUZ__CTL)
/* Drive Modes */
#define LUZ_DM0                    (* (reg8 *) LUZ__DM0) 
#define LUZ_DM1                    (* (reg8 *) LUZ__DM1)
#define LUZ_DM2                    (* (reg8 *) LUZ__DM2) 
/* Input Buffer Disable Override */
#define LUZ_INP_DIS                (* (reg8 *) LUZ__INP_DIS)
/* LCD Common or Segment Drive */
#define LUZ_LCD_COM_SEG            (* (reg8 *) LUZ__LCD_COM_SEG)
/* Enable Segment LCD */
#define LUZ_LCD_EN                 (* (reg8 *) LUZ__LCD_EN)
/* Slew Rate Control */
#define LUZ_SLW                    (* (reg8 *) LUZ__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define LUZ_PRTDSI__CAPS_SEL       (* (reg8 *) LUZ__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define LUZ_PRTDSI__DBL_SYNC_IN    (* (reg8 *) LUZ__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define LUZ_PRTDSI__OE_SEL0        (* (reg8 *) LUZ__PRTDSI__OE_SEL0) 
#define LUZ_PRTDSI__OE_SEL1        (* (reg8 *) LUZ__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define LUZ_PRTDSI__OUT_SEL0       (* (reg8 *) LUZ__PRTDSI__OUT_SEL0) 
#define LUZ_PRTDSI__OUT_SEL1       (* (reg8 *) LUZ__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define LUZ_PRTDSI__SYNC_OUT       (* (reg8 *) LUZ__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(LUZ__SIO_CFG)
    #define LUZ_SIO_HYST_EN        (* (reg8 *) LUZ__SIO_HYST_EN)
    #define LUZ_SIO_REG_HIFREQ     (* (reg8 *) LUZ__SIO_REG_HIFREQ)
    #define LUZ_SIO_CFG            (* (reg8 *) LUZ__SIO_CFG)
    #define LUZ_SIO_DIFF           (* (reg8 *) LUZ__SIO_DIFF)
#endif /* (LUZ__SIO_CFG) */

/* Interrupt Registers */
#if defined(LUZ__INTSTAT)
    #define LUZ_INTSTAT            (* (reg8 *) LUZ__INTSTAT)
    #define LUZ_SNAP               (* (reg8 *) LUZ__SNAP)
    
	#define LUZ_0_INTTYPE_REG 		(* (reg8 *) LUZ__0__INTTYPE)
#endif /* (LUZ__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_LUZ_H */


/* [] END OF FILE */
