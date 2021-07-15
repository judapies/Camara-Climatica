/*******************************************************************************
* File Name: ENCODER.h  
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

#if !defined(CY_PINS_ENCODER_H) /* Pins ENCODER_H */
#define CY_PINS_ENCODER_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "ENCODER_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 ENCODER__PORT == 15 && ((ENCODER__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    ENCODER_Write(uint8 value);
void    ENCODER_SetDriveMode(uint8 mode);
uint8   ENCODER_ReadDataReg(void);
uint8   ENCODER_Read(void);
void    ENCODER_SetInterruptMode(uint16 position, uint16 mode);
uint8   ENCODER_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the ENCODER_SetDriveMode() function.
     *  @{
     */
        #define ENCODER_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define ENCODER_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define ENCODER_DM_RES_UP          PIN_DM_RES_UP
        #define ENCODER_DM_RES_DWN         PIN_DM_RES_DWN
        #define ENCODER_DM_OD_LO           PIN_DM_OD_LO
        #define ENCODER_DM_OD_HI           PIN_DM_OD_HI
        #define ENCODER_DM_STRONG          PIN_DM_STRONG
        #define ENCODER_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define ENCODER_MASK               ENCODER__MASK
#define ENCODER_SHIFT              ENCODER__SHIFT
#define ENCODER_WIDTH              1u

/* Interrupt constants */
#if defined(ENCODER__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in ENCODER_SetInterruptMode() function.
     *  @{
     */
        #define ENCODER_INTR_NONE      (uint16)(0x0000u)
        #define ENCODER_INTR_RISING    (uint16)(0x0001u)
        #define ENCODER_INTR_FALLING   (uint16)(0x0002u)
        #define ENCODER_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define ENCODER_INTR_MASK      (0x01u) 
#endif /* (ENCODER__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define ENCODER_PS                     (* (reg8 *) ENCODER__PS)
/* Data Register */
#define ENCODER_DR                     (* (reg8 *) ENCODER__DR)
/* Port Number */
#define ENCODER_PRT_NUM                (* (reg8 *) ENCODER__PRT) 
/* Connect to Analog Globals */                                                  
#define ENCODER_AG                     (* (reg8 *) ENCODER__AG)                       
/* Analog MUX bux enable */
#define ENCODER_AMUX                   (* (reg8 *) ENCODER__AMUX) 
/* Bidirectional Enable */                                                        
#define ENCODER_BIE                    (* (reg8 *) ENCODER__BIE)
/* Bit-mask for Aliased Register Access */
#define ENCODER_BIT_MASK               (* (reg8 *) ENCODER__BIT_MASK)
/* Bypass Enable */
#define ENCODER_BYP                    (* (reg8 *) ENCODER__BYP)
/* Port wide control signals */                                                   
#define ENCODER_CTL                    (* (reg8 *) ENCODER__CTL)
/* Drive Modes */
#define ENCODER_DM0                    (* (reg8 *) ENCODER__DM0) 
#define ENCODER_DM1                    (* (reg8 *) ENCODER__DM1)
#define ENCODER_DM2                    (* (reg8 *) ENCODER__DM2) 
/* Input Buffer Disable Override */
#define ENCODER_INP_DIS                (* (reg8 *) ENCODER__INP_DIS)
/* LCD Common or Segment Drive */
#define ENCODER_LCD_COM_SEG            (* (reg8 *) ENCODER__LCD_COM_SEG)
/* Enable Segment LCD */
#define ENCODER_LCD_EN                 (* (reg8 *) ENCODER__LCD_EN)
/* Slew Rate Control */
#define ENCODER_SLW                    (* (reg8 *) ENCODER__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define ENCODER_PRTDSI__CAPS_SEL       (* (reg8 *) ENCODER__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define ENCODER_PRTDSI__DBL_SYNC_IN    (* (reg8 *) ENCODER__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define ENCODER_PRTDSI__OE_SEL0        (* (reg8 *) ENCODER__PRTDSI__OE_SEL0) 
#define ENCODER_PRTDSI__OE_SEL1        (* (reg8 *) ENCODER__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define ENCODER_PRTDSI__OUT_SEL0       (* (reg8 *) ENCODER__PRTDSI__OUT_SEL0) 
#define ENCODER_PRTDSI__OUT_SEL1       (* (reg8 *) ENCODER__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define ENCODER_PRTDSI__SYNC_OUT       (* (reg8 *) ENCODER__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(ENCODER__SIO_CFG)
    #define ENCODER_SIO_HYST_EN        (* (reg8 *) ENCODER__SIO_HYST_EN)
    #define ENCODER_SIO_REG_HIFREQ     (* (reg8 *) ENCODER__SIO_REG_HIFREQ)
    #define ENCODER_SIO_CFG            (* (reg8 *) ENCODER__SIO_CFG)
    #define ENCODER_SIO_DIFF           (* (reg8 *) ENCODER__SIO_DIFF)
#endif /* (ENCODER__SIO_CFG) */

/* Interrupt Registers */
#if defined(ENCODER__INTSTAT)
    #define ENCODER_INTSTAT            (* (reg8 *) ENCODER__INTSTAT)
    #define ENCODER_SNAP               (* (reg8 *) ENCODER__SNAP)
    
	#define ENCODER_0_INTTYPE_REG 		(* (reg8 *) ENCODER__0__INTTYPE)
#endif /* (ENCODER__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_ENCODER_H */


/* [] END OF FILE */
