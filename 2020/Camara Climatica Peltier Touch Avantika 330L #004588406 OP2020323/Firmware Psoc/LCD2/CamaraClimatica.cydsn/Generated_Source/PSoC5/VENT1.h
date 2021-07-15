/*******************************************************************************
* File Name: VENT1.h  
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

#if !defined(CY_PINS_VENT1_H) /* Pins VENT1_H */
#define CY_PINS_VENT1_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "VENT1_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 VENT1__PORT == 15 && ((VENT1__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    VENT1_Write(uint8 value);
void    VENT1_SetDriveMode(uint8 mode);
uint8   VENT1_ReadDataReg(void);
uint8   VENT1_Read(void);
void    VENT1_SetInterruptMode(uint16 position, uint16 mode);
uint8   VENT1_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the VENT1_SetDriveMode() function.
     *  @{
     */
        #define VENT1_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define VENT1_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define VENT1_DM_RES_UP          PIN_DM_RES_UP
        #define VENT1_DM_RES_DWN         PIN_DM_RES_DWN
        #define VENT1_DM_OD_LO           PIN_DM_OD_LO
        #define VENT1_DM_OD_HI           PIN_DM_OD_HI
        #define VENT1_DM_STRONG          PIN_DM_STRONG
        #define VENT1_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define VENT1_MASK               VENT1__MASK
#define VENT1_SHIFT              VENT1__SHIFT
#define VENT1_WIDTH              1u

/* Interrupt constants */
#if defined(VENT1__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in VENT1_SetInterruptMode() function.
     *  @{
     */
        #define VENT1_INTR_NONE      (uint16)(0x0000u)
        #define VENT1_INTR_RISING    (uint16)(0x0001u)
        #define VENT1_INTR_FALLING   (uint16)(0x0002u)
        #define VENT1_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define VENT1_INTR_MASK      (0x01u) 
#endif /* (VENT1__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define VENT1_PS                     (* (reg8 *) VENT1__PS)
/* Data Register */
#define VENT1_DR                     (* (reg8 *) VENT1__DR)
/* Port Number */
#define VENT1_PRT_NUM                (* (reg8 *) VENT1__PRT) 
/* Connect to Analog Globals */                                                  
#define VENT1_AG                     (* (reg8 *) VENT1__AG)                       
/* Analog MUX bux enable */
#define VENT1_AMUX                   (* (reg8 *) VENT1__AMUX) 
/* Bidirectional Enable */                                                        
#define VENT1_BIE                    (* (reg8 *) VENT1__BIE)
/* Bit-mask for Aliased Register Access */
#define VENT1_BIT_MASK               (* (reg8 *) VENT1__BIT_MASK)
/* Bypass Enable */
#define VENT1_BYP                    (* (reg8 *) VENT1__BYP)
/* Port wide control signals */                                                   
#define VENT1_CTL                    (* (reg8 *) VENT1__CTL)
/* Drive Modes */
#define VENT1_DM0                    (* (reg8 *) VENT1__DM0) 
#define VENT1_DM1                    (* (reg8 *) VENT1__DM1)
#define VENT1_DM2                    (* (reg8 *) VENT1__DM2) 
/* Input Buffer Disable Override */
#define VENT1_INP_DIS                (* (reg8 *) VENT1__INP_DIS)
/* LCD Common or Segment Drive */
#define VENT1_LCD_COM_SEG            (* (reg8 *) VENT1__LCD_COM_SEG)
/* Enable Segment LCD */
#define VENT1_LCD_EN                 (* (reg8 *) VENT1__LCD_EN)
/* Slew Rate Control */
#define VENT1_SLW                    (* (reg8 *) VENT1__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define VENT1_PRTDSI__CAPS_SEL       (* (reg8 *) VENT1__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define VENT1_PRTDSI__DBL_SYNC_IN    (* (reg8 *) VENT1__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define VENT1_PRTDSI__OE_SEL0        (* (reg8 *) VENT1__PRTDSI__OE_SEL0) 
#define VENT1_PRTDSI__OE_SEL1        (* (reg8 *) VENT1__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define VENT1_PRTDSI__OUT_SEL0       (* (reg8 *) VENT1__PRTDSI__OUT_SEL0) 
#define VENT1_PRTDSI__OUT_SEL1       (* (reg8 *) VENT1__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define VENT1_PRTDSI__SYNC_OUT       (* (reg8 *) VENT1__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(VENT1__SIO_CFG)
    #define VENT1_SIO_HYST_EN        (* (reg8 *) VENT1__SIO_HYST_EN)
    #define VENT1_SIO_REG_HIFREQ     (* (reg8 *) VENT1__SIO_REG_HIFREQ)
    #define VENT1_SIO_CFG            (* (reg8 *) VENT1__SIO_CFG)
    #define VENT1_SIO_DIFF           (* (reg8 *) VENT1__SIO_DIFF)
#endif /* (VENT1__SIO_CFG) */

/* Interrupt Registers */
#if defined(VENT1__INTSTAT)
    #define VENT1_INTSTAT            (* (reg8 *) VENT1__INTSTAT)
    #define VENT1_SNAP               (* (reg8 *) VENT1__SNAP)
    
	#define VENT1_0_INTTYPE_REG 		(* (reg8 *) VENT1__0__INTTYPE)
#endif /* (VENT1__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_VENT1_H */


/* [] END OF FILE */
