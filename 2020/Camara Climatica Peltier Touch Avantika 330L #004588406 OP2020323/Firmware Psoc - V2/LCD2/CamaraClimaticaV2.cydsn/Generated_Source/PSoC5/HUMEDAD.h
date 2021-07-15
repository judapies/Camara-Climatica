/*******************************************************************************
* File Name: HUMEDAD.h  
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

#if !defined(CY_PINS_HUMEDAD_H) /* Pins HUMEDAD_H */
#define CY_PINS_HUMEDAD_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "HUMEDAD_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 HUMEDAD__PORT == 15 && ((HUMEDAD__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    HUMEDAD_Write(uint8 value);
void    HUMEDAD_SetDriveMode(uint8 mode);
uint8   HUMEDAD_ReadDataReg(void);
uint8   HUMEDAD_Read(void);
void    HUMEDAD_SetInterruptMode(uint16 position, uint16 mode);
uint8   HUMEDAD_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the HUMEDAD_SetDriveMode() function.
     *  @{
     */
        #define HUMEDAD_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define HUMEDAD_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define HUMEDAD_DM_RES_UP          PIN_DM_RES_UP
        #define HUMEDAD_DM_RES_DWN         PIN_DM_RES_DWN
        #define HUMEDAD_DM_OD_LO           PIN_DM_OD_LO
        #define HUMEDAD_DM_OD_HI           PIN_DM_OD_HI
        #define HUMEDAD_DM_STRONG          PIN_DM_STRONG
        #define HUMEDAD_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define HUMEDAD_MASK               HUMEDAD__MASK
#define HUMEDAD_SHIFT              HUMEDAD__SHIFT
#define HUMEDAD_WIDTH              1u

/* Interrupt constants */
#if defined(HUMEDAD__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in HUMEDAD_SetInterruptMode() function.
     *  @{
     */
        #define HUMEDAD_INTR_NONE      (uint16)(0x0000u)
        #define HUMEDAD_INTR_RISING    (uint16)(0x0001u)
        #define HUMEDAD_INTR_FALLING   (uint16)(0x0002u)
        #define HUMEDAD_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define HUMEDAD_INTR_MASK      (0x01u) 
#endif /* (HUMEDAD__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define HUMEDAD_PS                     (* (reg8 *) HUMEDAD__PS)
/* Data Register */
#define HUMEDAD_DR                     (* (reg8 *) HUMEDAD__DR)
/* Port Number */
#define HUMEDAD_PRT_NUM                (* (reg8 *) HUMEDAD__PRT) 
/* Connect to Analog Globals */                                                  
#define HUMEDAD_AG                     (* (reg8 *) HUMEDAD__AG)                       
/* Analog MUX bux enable */
#define HUMEDAD_AMUX                   (* (reg8 *) HUMEDAD__AMUX) 
/* Bidirectional Enable */                                                        
#define HUMEDAD_BIE                    (* (reg8 *) HUMEDAD__BIE)
/* Bit-mask for Aliased Register Access */
#define HUMEDAD_BIT_MASK               (* (reg8 *) HUMEDAD__BIT_MASK)
/* Bypass Enable */
#define HUMEDAD_BYP                    (* (reg8 *) HUMEDAD__BYP)
/* Port wide control signals */                                                   
#define HUMEDAD_CTL                    (* (reg8 *) HUMEDAD__CTL)
/* Drive Modes */
#define HUMEDAD_DM0                    (* (reg8 *) HUMEDAD__DM0) 
#define HUMEDAD_DM1                    (* (reg8 *) HUMEDAD__DM1)
#define HUMEDAD_DM2                    (* (reg8 *) HUMEDAD__DM2) 
/* Input Buffer Disable Override */
#define HUMEDAD_INP_DIS                (* (reg8 *) HUMEDAD__INP_DIS)
/* LCD Common or Segment Drive */
#define HUMEDAD_LCD_COM_SEG            (* (reg8 *) HUMEDAD__LCD_COM_SEG)
/* Enable Segment LCD */
#define HUMEDAD_LCD_EN                 (* (reg8 *) HUMEDAD__LCD_EN)
/* Slew Rate Control */
#define HUMEDAD_SLW                    (* (reg8 *) HUMEDAD__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define HUMEDAD_PRTDSI__CAPS_SEL       (* (reg8 *) HUMEDAD__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define HUMEDAD_PRTDSI__DBL_SYNC_IN    (* (reg8 *) HUMEDAD__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define HUMEDAD_PRTDSI__OE_SEL0        (* (reg8 *) HUMEDAD__PRTDSI__OE_SEL0) 
#define HUMEDAD_PRTDSI__OE_SEL1        (* (reg8 *) HUMEDAD__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define HUMEDAD_PRTDSI__OUT_SEL0       (* (reg8 *) HUMEDAD__PRTDSI__OUT_SEL0) 
#define HUMEDAD_PRTDSI__OUT_SEL1       (* (reg8 *) HUMEDAD__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define HUMEDAD_PRTDSI__SYNC_OUT       (* (reg8 *) HUMEDAD__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(HUMEDAD__SIO_CFG)
    #define HUMEDAD_SIO_HYST_EN        (* (reg8 *) HUMEDAD__SIO_HYST_EN)
    #define HUMEDAD_SIO_REG_HIFREQ     (* (reg8 *) HUMEDAD__SIO_REG_HIFREQ)
    #define HUMEDAD_SIO_CFG            (* (reg8 *) HUMEDAD__SIO_CFG)
    #define HUMEDAD_SIO_DIFF           (* (reg8 *) HUMEDAD__SIO_DIFF)
#endif /* (HUMEDAD__SIO_CFG) */

/* Interrupt Registers */
#if defined(HUMEDAD__INTSTAT)
    #define HUMEDAD_INTSTAT            (* (reg8 *) HUMEDAD__INTSTAT)
    #define HUMEDAD_SNAP               (* (reg8 *) HUMEDAD__SNAP)
    
	#define HUMEDAD_0_INTTYPE_REG 		(* (reg8 *) HUMEDAD__0__INTTYPE)
#endif /* (HUMEDAD__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_HUMEDAD_H */


/* [] END OF FILE */
