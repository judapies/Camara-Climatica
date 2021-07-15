/*******************************************************************************
* File Name: BOMBA.h  
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

#if !defined(CY_PINS_BOMBA_H) /* Pins BOMBA_H */
#define CY_PINS_BOMBA_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "BOMBA_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 BOMBA__PORT == 15 && ((BOMBA__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    BOMBA_Write(uint8 value);
void    BOMBA_SetDriveMode(uint8 mode);
uint8   BOMBA_ReadDataReg(void);
uint8   BOMBA_Read(void);
void    BOMBA_SetInterruptMode(uint16 position, uint16 mode);
uint8   BOMBA_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the BOMBA_SetDriveMode() function.
     *  @{
     */
        #define BOMBA_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define BOMBA_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define BOMBA_DM_RES_UP          PIN_DM_RES_UP
        #define BOMBA_DM_RES_DWN         PIN_DM_RES_DWN
        #define BOMBA_DM_OD_LO           PIN_DM_OD_LO
        #define BOMBA_DM_OD_HI           PIN_DM_OD_HI
        #define BOMBA_DM_STRONG          PIN_DM_STRONG
        #define BOMBA_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define BOMBA_MASK               BOMBA__MASK
#define BOMBA_SHIFT              BOMBA__SHIFT
#define BOMBA_WIDTH              1u

/* Interrupt constants */
#if defined(BOMBA__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in BOMBA_SetInterruptMode() function.
     *  @{
     */
        #define BOMBA_INTR_NONE      (uint16)(0x0000u)
        #define BOMBA_INTR_RISING    (uint16)(0x0001u)
        #define BOMBA_INTR_FALLING   (uint16)(0x0002u)
        #define BOMBA_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define BOMBA_INTR_MASK      (0x01u) 
#endif /* (BOMBA__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define BOMBA_PS                     (* (reg8 *) BOMBA__PS)
/* Data Register */
#define BOMBA_DR                     (* (reg8 *) BOMBA__DR)
/* Port Number */
#define BOMBA_PRT_NUM                (* (reg8 *) BOMBA__PRT) 
/* Connect to Analog Globals */                                                  
#define BOMBA_AG                     (* (reg8 *) BOMBA__AG)                       
/* Analog MUX bux enable */
#define BOMBA_AMUX                   (* (reg8 *) BOMBA__AMUX) 
/* Bidirectional Enable */                                                        
#define BOMBA_BIE                    (* (reg8 *) BOMBA__BIE)
/* Bit-mask for Aliased Register Access */
#define BOMBA_BIT_MASK               (* (reg8 *) BOMBA__BIT_MASK)
/* Bypass Enable */
#define BOMBA_BYP                    (* (reg8 *) BOMBA__BYP)
/* Port wide control signals */                                                   
#define BOMBA_CTL                    (* (reg8 *) BOMBA__CTL)
/* Drive Modes */
#define BOMBA_DM0                    (* (reg8 *) BOMBA__DM0) 
#define BOMBA_DM1                    (* (reg8 *) BOMBA__DM1)
#define BOMBA_DM2                    (* (reg8 *) BOMBA__DM2) 
/* Input Buffer Disable Override */
#define BOMBA_INP_DIS                (* (reg8 *) BOMBA__INP_DIS)
/* LCD Common or Segment Drive */
#define BOMBA_LCD_COM_SEG            (* (reg8 *) BOMBA__LCD_COM_SEG)
/* Enable Segment LCD */
#define BOMBA_LCD_EN                 (* (reg8 *) BOMBA__LCD_EN)
/* Slew Rate Control */
#define BOMBA_SLW                    (* (reg8 *) BOMBA__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define BOMBA_PRTDSI__CAPS_SEL       (* (reg8 *) BOMBA__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define BOMBA_PRTDSI__DBL_SYNC_IN    (* (reg8 *) BOMBA__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define BOMBA_PRTDSI__OE_SEL0        (* (reg8 *) BOMBA__PRTDSI__OE_SEL0) 
#define BOMBA_PRTDSI__OE_SEL1        (* (reg8 *) BOMBA__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define BOMBA_PRTDSI__OUT_SEL0       (* (reg8 *) BOMBA__PRTDSI__OUT_SEL0) 
#define BOMBA_PRTDSI__OUT_SEL1       (* (reg8 *) BOMBA__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define BOMBA_PRTDSI__SYNC_OUT       (* (reg8 *) BOMBA__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(BOMBA__SIO_CFG)
    #define BOMBA_SIO_HYST_EN        (* (reg8 *) BOMBA__SIO_HYST_EN)
    #define BOMBA_SIO_REG_HIFREQ     (* (reg8 *) BOMBA__SIO_REG_HIFREQ)
    #define BOMBA_SIO_CFG            (* (reg8 *) BOMBA__SIO_CFG)
    #define BOMBA_SIO_DIFF           (* (reg8 *) BOMBA__SIO_DIFF)
#endif /* (BOMBA__SIO_CFG) */

/* Interrupt Registers */
#if defined(BOMBA__INTSTAT)
    #define BOMBA_INTSTAT            (* (reg8 *) BOMBA__INTSTAT)
    #define BOMBA_SNAP               (* (reg8 *) BOMBA__SNAP)
    
	#define BOMBA_0_INTTYPE_REG 		(* (reg8 *) BOMBA__0__INTTYPE)
#endif /* (BOMBA__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_BOMBA_H */


/* [] END OF FILE */
