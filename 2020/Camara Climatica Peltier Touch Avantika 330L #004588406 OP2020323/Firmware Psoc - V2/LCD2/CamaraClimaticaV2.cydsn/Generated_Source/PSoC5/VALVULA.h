/*******************************************************************************
* File Name: VALVULA.h  
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

#if !defined(CY_PINS_VALVULA_H) /* Pins VALVULA_H */
#define CY_PINS_VALVULA_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "VALVULA_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 VALVULA__PORT == 15 && ((VALVULA__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    VALVULA_Write(uint8 value);
void    VALVULA_SetDriveMode(uint8 mode);
uint8   VALVULA_ReadDataReg(void);
uint8   VALVULA_Read(void);
void    VALVULA_SetInterruptMode(uint16 position, uint16 mode);
uint8   VALVULA_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the VALVULA_SetDriveMode() function.
     *  @{
     */
        #define VALVULA_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define VALVULA_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define VALVULA_DM_RES_UP          PIN_DM_RES_UP
        #define VALVULA_DM_RES_DWN         PIN_DM_RES_DWN
        #define VALVULA_DM_OD_LO           PIN_DM_OD_LO
        #define VALVULA_DM_OD_HI           PIN_DM_OD_HI
        #define VALVULA_DM_STRONG          PIN_DM_STRONG
        #define VALVULA_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define VALVULA_MASK               VALVULA__MASK
#define VALVULA_SHIFT              VALVULA__SHIFT
#define VALVULA_WIDTH              1u

/* Interrupt constants */
#if defined(VALVULA__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in VALVULA_SetInterruptMode() function.
     *  @{
     */
        #define VALVULA_INTR_NONE      (uint16)(0x0000u)
        #define VALVULA_INTR_RISING    (uint16)(0x0001u)
        #define VALVULA_INTR_FALLING   (uint16)(0x0002u)
        #define VALVULA_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define VALVULA_INTR_MASK      (0x01u) 
#endif /* (VALVULA__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define VALVULA_PS                     (* (reg8 *) VALVULA__PS)
/* Data Register */
#define VALVULA_DR                     (* (reg8 *) VALVULA__DR)
/* Port Number */
#define VALVULA_PRT_NUM                (* (reg8 *) VALVULA__PRT) 
/* Connect to Analog Globals */                                                  
#define VALVULA_AG                     (* (reg8 *) VALVULA__AG)                       
/* Analog MUX bux enable */
#define VALVULA_AMUX                   (* (reg8 *) VALVULA__AMUX) 
/* Bidirectional Enable */                                                        
#define VALVULA_BIE                    (* (reg8 *) VALVULA__BIE)
/* Bit-mask for Aliased Register Access */
#define VALVULA_BIT_MASK               (* (reg8 *) VALVULA__BIT_MASK)
/* Bypass Enable */
#define VALVULA_BYP                    (* (reg8 *) VALVULA__BYP)
/* Port wide control signals */                                                   
#define VALVULA_CTL                    (* (reg8 *) VALVULA__CTL)
/* Drive Modes */
#define VALVULA_DM0                    (* (reg8 *) VALVULA__DM0) 
#define VALVULA_DM1                    (* (reg8 *) VALVULA__DM1)
#define VALVULA_DM2                    (* (reg8 *) VALVULA__DM2) 
/* Input Buffer Disable Override */
#define VALVULA_INP_DIS                (* (reg8 *) VALVULA__INP_DIS)
/* LCD Common or Segment Drive */
#define VALVULA_LCD_COM_SEG            (* (reg8 *) VALVULA__LCD_COM_SEG)
/* Enable Segment LCD */
#define VALVULA_LCD_EN                 (* (reg8 *) VALVULA__LCD_EN)
/* Slew Rate Control */
#define VALVULA_SLW                    (* (reg8 *) VALVULA__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define VALVULA_PRTDSI__CAPS_SEL       (* (reg8 *) VALVULA__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define VALVULA_PRTDSI__DBL_SYNC_IN    (* (reg8 *) VALVULA__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define VALVULA_PRTDSI__OE_SEL0        (* (reg8 *) VALVULA__PRTDSI__OE_SEL0) 
#define VALVULA_PRTDSI__OE_SEL1        (* (reg8 *) VALVULA__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define VALVULA_PRTDSI__OUT_SEL0       (* (reg8 *) VALVULA__PRTDSI__OUT_SEL0) 
#define VALVULA_PRTDSI__OUT_SEL1       (* (reg8 *) VALVULA__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define VALVULA_PRTDSI__SYNC_OUT       (* (reg8 *) VALVULA__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(VALVULA__SIO_CFG)
    #define VALVULA_SIO_HYST_EN        (* (reg8 *) VALVULA__SIO_HYST_EN)
    #define VALVULA_SIO_REG_HIFREQ     (* (reg8 *) VALVULA__SIO_REG_HIFREQ)
    #define VALVULA_SIO_CFG            (* (reg8 *) VALVULA__SIO_CFG)
    #define VALVULA_SIO_DIFF           (* (reg8 *) VALVULA__SIO_DIFF)
#endif /* (VALVULA__SIO_CFG) */

/* Interrupt Registers */
#if defined(VALVULA__INTSTAT)
    #define VALVULA_INTSTAT            (* (reg8 *) VALVULA__INTSTAT)
    #define VALVULA_SNAP               (* (reg8 *) VALVULA__SNAP)
    
	#define VALVULA_0_INTTYPE_REG 		(* (reg8 *) VALVULA__0__INTTYPE)
#endif /* (VALVULA__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_VALVULA_H */


/* [] END OF FILE */
