/*******************************************************************************
* File Name: TERM.h  
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

#if !defined(CY_PINS_TERM_H) /* Pins TERM_H */
#define CY_PINS_TERM_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "TERM_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 TERM__PORT == 15 && ((TERM__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    TERM_Write(uint8 value);
void    TERM_SetDriveMode(uint8 mode);
uint8   TERM_ReadDataReg(void);
uint8   TERM_Read(void);
void    TERM_SetInterruptMode(uint16 position, uint16 mode);
uint8   TERM_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the TERM_SetDriveMode() function.
     *  @{
     */
        #define TERM_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define TERM_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define TERM_DM_RES_UP          PIN_DM_RES_UP
        #define TERM_DM_RES_DWN         PIN_DM_RES_DWN
        #define TERM_DM_OD_LO           PIN_DM_OD_LO
        #define TERM_DM_OD_HI           PIN_DM_OD_HI
        #define TERM_DM_STRONG          PIN_DM_STRONG
        #define TERM_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define TERM_MASK               TERM__MASK
#define TERM_SHIFT              TERM__SHIFT
#define TERM_WIDTH              1u

/* Interrupt constants */
#if defined(TERM__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in TERM_SetInterruptMode() function.
     *  @{
     */
        #define TERM_INTR_NONE      (uint16)(0x0000u)
        #define TERM_INTR_RISING    (uint16)(0x0001u)
        #define TERM_INTR_FALLING   (uint16)(0x0002u)
        #define TERM_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define TERM_INTR_MASK      (0x01u) 
#endif /* (TERM__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define TERM_PS                     (* (reg8 *) TERM__PS)
/* Data Register */
#define TERM_DR                     (* (reg8 *) TERM__DR)
/* Port Number */
#define TERM_PRT_NUM                (* (reg8 *) TERM__PRT) 
/* Connect to Analog Globals */                                                  
#define TERM_AG                     (* (reg8 *) TERM__AG)                       
/* Analog MUX bux enable */
#define TERM_AMUX                   (* (reg8 *) TERM__AMUX) 
/* Bidirectional Enable */                                                        
#define TERM_BIE                    (* (reg8 *) TERM__BIE)
/* Bit-mask for Aliased Register Access */
#define TERM_BIT_MASK               (* (reg8 *) TERM__BIT_MASK)
/* Bypass Enable */
#define TERM_BYP                    (* (reg8 *) TERM__BYP)
/* Port wide control signals */                                                   
#define TERM_CTL                    (* (reg8 *) TERM__CTL)
/* Drive Modes */
#define TERM_DM0                    (* (reg8 *) TERM__DM0) 
#define TERM_DM1                    (* (reg8 *) TERM__DM1)
#define TERM_DM2                    (* (reg8 *) TERM__DM2) 
/* Input Buffer Disable Override */
#define TERM_INP_DIS                (* (reg8 *) TERM__INP_DIS)
/* LCD Common or Segment Drive */
#define TERM_LCD_COM_SEG            (* (reg8 *) TERM__LCD_COM_SEG)
/* Enable Segment LCD */
#define TERM_LCD_EN                 (* (reg8 *) TERM__LCD_EN)
/* Slew Rate Control */
#define TERM_SLW                    (* (reg8 *) TERM__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define TERM_PRTDSI__CAPS_SEL       (* (reg8 *) TERM__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define TERM_PRTDSI__DBL_SYNC_IN    (* (reg8 *) TERM__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define TERM_PRTDSI__OE_SEL0        (* (reg8 *) TERM__PRTDSI__OE_SEL0) 
#define TERM_PRTDSI__OE_SEL1        (* (reg8 *) TERM__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define TERM_PRTDSI__OUT_SEL0       (* (reg8 *) TERM__PRTDSI__OUT_SEL0) 
#define TERM_PRTDSI__OUT_SEL1       (* (reg8 *) TERM__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define TERM_PRTDSI__SYNC_OUT       (* (reg8 *) TERM__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(TERM__SIO_CFG)
    #define TERM_SIO_HYST_EN        (* (reg8 *) TERM__SIO_HYST_EN)
    #define TERM_SIO_REG_HIFREQ     (* (reg8 *) TERM__SIO_REG_HIFREQ)
    #define TERM_SIO_CFG            (* (reg8 *) TERM__SIO_CFG)
    #define TERM_SIO_DIFF           (* (reg8 *) TERM__SIO_DIFF)
#endif /* (TERM__SIO_CFG) */

/* Interrupt Registers */
#if defined(TERM__INTSTAT)
    #define TERM_INTSTAT            (* (reg8 *) TERM__INTSTAT)
    #define TERM_SNAP               (* (reg8 *) TERM__SNAP)
    
	#define TERM_0_INTTYPE_REG 		(* (reg8 *) TERM__0__INTTYPE)
#endif /* (TERM__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_TERM_H */


/* [] END OF FILE */
