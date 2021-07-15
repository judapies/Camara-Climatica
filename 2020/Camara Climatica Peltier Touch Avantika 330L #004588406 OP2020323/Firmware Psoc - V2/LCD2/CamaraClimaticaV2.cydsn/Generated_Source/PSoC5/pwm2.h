/*******************************************************************************
* File Name: pwm2.h  
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

#if !defined(CY_PINS_pwm2_H) /* Pins pwm2_H */
#define CY_PINS_pwm2_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "pwm2_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 pwm2__PORT == 15 && ((pwm2__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    pwm2_Write(uint8 value);
void    pwm2_SetDriveMode(uint8 mode);
uint8   pwm2_ReadDataReg(void);
uint8   pwm2_Read(void);
void    pwm2_SetInterruptMode(uint16 position, uint16 mode);
uint8   pwm2_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the pwm2_SetDriveMode() function.
     *  @{
     */
        #define pwm2_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define pwm2_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define pwm2_DM_RES_UP          PIN_DM_RES_UP
        #define pwm2_DM_RES_DWN         PIN_DM_RES_DWN
        #define pwm2_DM_OD_LO           PIN_DM_OD_LO
        #define pwm2_DM_OD_HI           PIN_DM_OD_HI
        #define pwm2_DM_STRONG          PIN_DM_STRONG
        #define pwm2_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define pwm2_MASK               pwm2__MASK
#define pwm2_SHIFT              pwm2__SHIFT
#define pwm2_WIDTH              1u

/* Interrupt constants */
#if defined(pwm2__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in pwm2_SetInterruptMode() function.
     *  @{
     */
        #define pwm2_INTR_NONE      (uint16)(0x0000u)
        #define pwm2_INTR_RISING    (uint16)(0x0001u)
        #define pwm2_INTR_FALLING   (uint16)(0x0002u)
        #define pwm2_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define pwm2_INTR_MASK      (0x01u) 
#endif /* (pwm2__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define pwm2_PS                     (* (reg8 *) pwm2__PS)
/* Data Register */
#define pwm2_DR                     (* (reg8 *) pwm2__DR)
/* Port Number */
#define pwm2_PRT_NUM                (* (reg8 *) pwm2__PRT) 
/* Connect to Analog Globals */                                                  
#define pwm2_AG                     (* (reg8 *) pwm2__AG)                       
/* Analog MUX bux enable */
#define pwm2_AMUX                   (* (reg8 *) pwm2__AMUX) 
/* Bidirectional Enable */                                                        
#define pwm2_BIE                    (* (reg8 *) pwm2__BIE)
/* Bit-mask for Aliased Register Access */
#define pwm2_BIT_MASK               (* (reg8 *) pwm2__BIT_MASK)
/* Bypass Enable */
#define pwm2_BYP                    (* (reg8 *) pwm2__BYP)
/* Port wide control signals */                                                   
#define pwm2_CTL                    (* (reg8 *) pwm2__CTL)
/* Drive Modes */
#define pwm2_DM0                    (* (reg8 *) pwm2__DM0) 
#define pwm2_DM1                    (* (reg8 *) pwm2__DM1)
#define pwm2_DM2                    (* (reg8 *) pwm2__DM2) 
/* Input Buffer Disable Override */
#define pwm2_INP_DIS                (* (reg8 *) pwm2__INP_DIS)
/* LCD Common or Segment Drive */
#define pwm2_LCD_COM_SEG            (* (reg8 *) pwm2__LCD_COM_SEG)
/* Enable Segment LCD */
#define pwm2_LCD_EN                 (* (reg8 *) pwm2__LCD_EN)
/* Slew Rate Control */
#define pwm2_SLW                    (* (reg8 *) pwm2__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define pwm2_PRTDSI__CAPS_SEL       (* (reg8 *) pwm2__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define pwm2_PRTDSI__DBL_SYNC_IN    (* (reg8 *) pwm2__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define pwm2_PRTDSI__OE_SEL0        (* (reg8 *) pwm2__PRTDSI__OE_SEL0) 
#define pwm2_PRTDSI__OE_SEL1        (* (reg8 *) pwm2__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define pwm2_PRTDSI__OUT_SEL0       (* (reg8 *) pwm2__PRTDSI__OUT_SEL0) 
#define pwm2_PRTDSI__OUT_SEL1       (* (reg8 *) pwm2__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define pwm2_PRTDSI__SYNC_OUT       (* (reg8 *) pwm2__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(pwm2__SIO_CFG)
    #define pwm2_SIO_HYST_EN        (* (reg8 *) pwm2__SIO_HYST_EN)
    #define pwm2_SIO_REG_HIFREQ     (* (reg8 *) pwm2__SIO_REG_HIFREQ)
    #define pwm2_SIO_CFG            (* (reg8 *) pwm2__SIO_CFG)
    #define pwm2_SIO_DIFF           (* (reg8 *) pwm2__SIO_DIFF)
#endif /* (pwm2__SIO_CFG) */

/* Interrupt Registers */
#if defined(pwm2__INTSTAT)
    #define pwm2_INTSTAT            (* (reg8 *) pwm2__INTSTAT)
    #define pwm2_SNAP               (* (reg8 *) pwm2__SNAP)
    
	#define pwm2_0_INTTYPE_REG 		(* (reg8 *) pwm2__0__INTTYPE)
#endif /* (pwm2__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_pwm2_H */


/* [] END OF FILE */
