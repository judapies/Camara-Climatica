/*******************************************************************************
* File Name: TRANSMISOR.h  
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

#if !defined(CY_PINS_TRANSMISOR_H) /* Pins TRANSMISOR_H */
#define CY_PINS_TRANSMISOR_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "TRANSMISOR_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 TRANSMISOR__PORT == 15 && ((TRANSMISOR__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    TRANSMISOR_Write(uint8 value);
void    TRANSMISOR_SetDriveMode(uint8 mode);
uint8   TRANSMISOR_ReadDataReg(void);
uint8   TRANSMISOR_Read(void);
void    TRANSMISOR_SetInterruptMode(uint16 position, uint16 mode);
uint8   TRANSMISOR_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the TRANSMISOR_SetDriveMode() function.
     *  @{
     */
        #define TRANSMISOR_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define TRANSMISOR_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define TRANSMISOR_DM_RES_UP          PIN_DM_RES_UP
        #define TRANSMISOR_DM_RES_DWN         PIN_DM_RES_DWN
        #define TRANSMISOR_DM_OD_LO           PIN_DM_OD_LO
        #define TRANSMISOR_DM_OD_HI           PIN_DM_OD_HI
        #define TRANSMISOR_DM_STRONG          PIN_DM_STRONG
        #define TRANSMISOR_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define TRANSMISOR_MASK               TRANSMISOR__MASK
#define TRANSMISOR_SHIFT              TRANSMISOR__SHIFT
#define TRANSMISOR_WIDTH              1u

/* Interrupt constants */
#if defined(TRANSMISOR__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in TRANSMISOR_SetInterruptMode() function.
     *  @{
     */
        #define TRANSMISOR_INTR_NONE      (uint16)(0x0000u)
        #define TRANSMISOR_INTR_RISING    (uint16)(0x0001u)
        #define TRANSMISOR_INTR_FALLING   (uint16)(0x0002u)
        #define TRANSMISOR_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define TRANSMISOR_INTR_MASK      (0x01u) 
#endif /* (TRANSMISOR__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define TRANSMISOR_PS                     (* (reg8 *) TRANSMISOR__PS)
/* Data Register */
#define TRANSMISOR_DR                     (* (reg8 *) TRANSMISOR__DR)
/* Port Number */
#define TRANSMISOR_PRT_NUM                (* (reg8 *) TRANSMISOR__PRT) 
/* Connect to Analog Globals */                                                  
#define TRANSMISOR_AG                     (* (reg8 *) TRANSMISOR__AG)                       
/* Analog MUX bux enable */
#define TRANSMISOR_AMUX                   (* (reg8 *) TRANSMISOR__AMUX) 
/* Bidirectional Enable */                                                        
#define TRANSMISOR_BIE                    (* (reg8 *) TRANSMISOR__BIE)
/* Bit-mask for Aliased Register Access */
#define TRANSMISOR_BIT_MASK               (* (reg8 *) TRANSMISOR__BIT_MASK)
/* Bypass Enable */
#define TRANSMISOR_BYP                    (* (reg8 *) TRANSMISOR__BYP)
/* Port wide control signals */                                                   
#define TRANSMISOR_CTL                    (* (reg8 *) TRANSMISOR__CTL)
/* Drive Modes */
#define TRANSMISOR_DM0                    (* (reg8 *) TRANSMISOR__DM0) 
#define TRANSMISOR_DM1                    (* (reg8 *) TRANSMISOR__DM1)
#define TRANSMISOR_DM2                    (* (reg8 *) TRANSMISOR__DM2) 
/* Input Buffer Disable Override */
#define TRANSMISOR_INP_DIS                (* (reg8 *) TRANSMISOR__INP_DIS)
/* LCD Common or Segment Drive */
#define TRANSMISOR_LCD_COM_SEG            (* (reg8 *) TRANSMISOR__LCD_COM_SEG)
/* Enable Segment LCD */
#define TRANSMISOR_LCD_EN                 (* (reg8 *) TRANSMISOR__LCD_EN)
/* Slew Rate Control */
#define TRANSMISOR_SLW                    (* (reg8 *) TRANSMISOR__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define TRANSMISOR_PRTDSI__CAPS_SEL       (* (reg8 *) TRANSMISOR__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define TRANSMISOR_PRTDSI__DBL_SYNC_IN    (* (reg8 *) TRANSMISOR__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define TRANSMISOR_PRTDSI__OE_SEL0        (* (reg8 *) TRANSMISOR__PRTDSI__OE_SEL0) 
#define TRANSMISOR_PRTDSI__OE_SEL1        (* (reg8 *) TRANSMISOR__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define TRANSMISOR_PRTDSI__OUT_SEL0       (* (reg8 *) TRANSMISOR__PRTDSI__OUT_SEL0) 
#define TRANSMISOR_PRTDSI__OUT_SEL1       (* (reg8 *) TRANSMISOR__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define TRANSMISOR_PRTDSI__SYNC_OUT       (* (reg8 *) TRANSMISOR__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(TRANSMISOR__SIO_CFG)
    #define TRANSMISOR_SIO_HYST_EN        (* (reg8 *) TRANSMISOR__SIO_HYST_EN)
    #define TRANSMISOR_SIO_REG_HIFREQ     (* (reg8 *) TRANSMISOR__SIO_REG_HIFREQ)
    #define TRANSMISOR_SIO_CFG            (* (reg8 *) TRANSMISOR__SIO_CFG)
    #define TRANSMISOR_SIO_DIFF           (* (reg8 *) TRANSMISOR__SIO_DIFF)
#endif /* (TRANSMISOR__SIO_CFG) */

/* Interrupt Registers */
#if defined(TRANSMISOR__INTSTAT)
    #define TRANSMISOR_INTSTAT            (* (reg8 *) TRANSMISOR__INTSTAT)
    #define TRANSMISOR_SNAP               (* (reg8 *) TRANSMISOR__SNAP)
    
	#define TRANSMISOR_0_INTTYPE_REG 		(* (reg8 *) TRANSMISOR__0__INTTYPE)
#endif /* (TRANSMISOR__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_TRANSMISOR_H */


/* [] END OF FILE */
