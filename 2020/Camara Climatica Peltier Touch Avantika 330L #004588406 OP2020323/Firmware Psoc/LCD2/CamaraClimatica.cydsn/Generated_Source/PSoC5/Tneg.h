/*******************************************************************************
* File Name: Tneg.h  
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

#if !defined(CY_PINS_Tneg_H) /* Pins Tneg_H */
#define CY_PINS_Tneg_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Tneg_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Tneg__PORT == 15 && ((Tneg__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Tneg_Write(uint8 value);
void    Tneg_SetDriveMode(uint8 mode);
uint8   Tneg_ReadDataReg(void);
uint8   Tneg_Read(void);
void    Tneg_SetInterruptMode(uint16 position, uint16 mode);
uint8   Tneg_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Tneg_SetDriveMode() function.
     *  @{
     */
        #define Tneg_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Tneg_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Tneg_DM_RES_UP          PIN_DM_RES_UP
        #define Tneg_DM_RES_DWN         PIN_DM_RES_DWN
        #define Tneg_DM_OD_LO           PIN_DM_OD_LO
        #define Tneg_DM_OD_HI           PIN_DM_OD_HI
        #define Tneg_DM_STRONG          PIN_DM_STRONG
        #define Tneg_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Tneg_MASK               Tneg__MASK
#define Tneg_SHIFT              Tneg__SHIFT
#define Tneg_WIDTH              1u

/* Interrupt constants */
#if defined(Tneg__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Tneg_SetInterruptMode() function.
     *  @{
     */
        #define Tneg_INTR_NONE      (uint16)(0x0000u)
        #define Tneg_INTR_RISING    (uint16)(0x0001u)
        #define Tneg_INTR_FALLING   (uint16)(0x0002u)
        #define Tneg_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Tneg_INTR_MASK      (0x01u) 
#endif /* (Tneg__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Tneg_PS                     (* (reg8 *) Tneg__PS)
/* Data Register */
#define Tneg_DR                     (* (reg8 *) Tneg__DR)
/* Port Number */
#define Tneg_PRT_NUM                (* (reg8 *) Tneg__PRT) 
/* Connect to Analog Globals */                                                  
#define Tneg_AG                     (* (reg8 *) Tneg__AG)                       
/* Analog MUX bux enable */
#define Tneg_AMUX                   (* (reg8 *) Tneg__AMUX) 
/* Bidirectional Enable */                                                        
#define Tneg_BIE                    (* (reg8 *) Tneg__BIE)
/* Bit-mask for Aliased Register Access */
#define Tneg_BIT_MASK               (* (reg8 *) Tneg__BIT_MASK)
/* Bypass Enable */
#define Tneg_BYP                    (* (reg8 *) Tneg__BYP)
/* Port wide control signals */                                                   
#define Tneg_CTL                    (* (reg8 *) Tneg__CTL)
/* Drive Modes */
#define Tneg_DM0                    (* (reg8 *) Tneg__DM0) 
#define Tneg_DM1                    (* (reg8 *) Tneg__DM1)
#define Tneg_DM2                    (* (reg8 *) Tneg__DM2) 
/* Input Buffer Disable Override */
#define Tneg_INP_DIS                (* (reg8 *) Tneg__INP_DIS)
/* LCD Common or Segment Drive */
#define Tneg_LCD_COM_SEG            (* (reg8 *) Tneg__LCD_COM_SEG)
/* Enable Segment LCD */
#define Tneg_LCD_EN                 (* (reg8 *) Tneg__LCD_EN)
/* Slew Rate Control */
#define Tneg_SLW                    (* (reg8 *) Tneg__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Tneg_PRTDSI__CAPS_SEL       (* (reg8 *) Tneg__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Tneg_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Tneg__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Tneg_PRTDSI__OE_SEL0        (* (reg8 *) Tneg__PRTDSI__OE_SEL0) 
#define Tneg_PRTDSI__OE_SEL1        (* (reg8 *) Tneg__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Tneg_PRTDSI__OUT_SEL0       (* (reg8 *) Tneg__PRTDSI__OUT_SEL0) 
#define Tneg_PRTDSI__OUT_SEL1       (* (reg8 *) Tneg__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Tneg_PRTDSI__SYNC_OUT       (* (reg8 *) Tneg__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Tneg__SIO_CFG)
    #define Tneg_SIO_HYST_EN        (* (reg8 *) Tneg__SIO_HYST_EN)
    #define Tneg_SIO_REG_HIFREQ     (* (reg8 *) Tneg__SIO_REG_HIFREQ)
    #define Tneg_SIO_CFG            (* (reg8 *) Tneg__SIO_CFG)
    #define Tneg_SIO_DIFF           (* (reg8 *) Tneg__SIO_DIFF)
#endif /* (Tneg__SIO_CFG) */

/* Interrupt Registers */
#if defined(Tneg__INTSTAT)
    #define Tneg_INTSTAT            (* (reg8 *) Tneg__INTSTAT)
    #define Tneg_SNAP               (* (reg8 *) Tneg__SNAP)
    
	#define Tneg_0_INTTYPE_REG 		(* (reg8 *) Tneg__0__INTTYPE)
#endif /* (Tneg__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Tneg_H */


/* [] END OF FILE */
