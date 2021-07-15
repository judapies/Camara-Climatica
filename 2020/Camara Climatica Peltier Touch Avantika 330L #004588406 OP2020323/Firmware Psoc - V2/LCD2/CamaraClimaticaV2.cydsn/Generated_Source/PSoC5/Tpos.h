/*******************************************************************************
* File Name: Tpos.h  
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

#if !defined(CY_PINS_Tpos_H) /* Pins Tpos_H */
#define CY_PINS_Tpos_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Tpos_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Tpos__PORT == 15 && ((Tpos__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Tpos_Write(uint8 value);
void    Tpos_SetDriveMode(uint8 mode);
uint8   Tpos_ReadDataReg(void);
uint8   Tpos_Read(void);
void    Tpos_SetInterruptMode(uint16 position, uint16 mode);
uint8   Tpos_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Tpos_SetDriveMode() function.
     *  @{
     */
        #define Tpos_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Tpos_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Tpos_DM_RES_UP          PIN_DM_RES_UP
        #define Tpos_DM_RES_DWN         PIN_DM_RES_DWN
        #define Tpos_DM_OD_LO           PIN_DM_OD_LO
        #define Tpos_DM_OD_HI           PIN_DM_OD_HI
        #define Tpos_DM_STRONG          PIN_DM_STRONG
        #define Tpos_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Tpos_MASK               Tpos__MASK
#define Tpos_SHIFT              Tpos__SHIFT
#define Tpos_WIDTH              1u

/* Interrupt constants */
#if defined(Tpos__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Tpos_SetInterruptMode() function.
     *  @{
     */
        #define Tpos_INTR_NONE      (uint16)(0x0000u)
        #define Tpos_INTR_RISING    (uint16)(0x0001u)
        #define Tpos_INTR_FALLING   (uint16)(0x0002u)
        #define Tpos_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Tpos_INTR_MASK      (0x01u) 
#endif /* (Tpos__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Tpos_PS                     (* (reg8 *) Tpos__PS)
/* Data Register */
#define Tpos_DR                     (* (reg8 *) Tpos__DR)
/* Port Number */
#define Tpos_PRT_NUM                (* (reg8 *) Tpos__PRT) 
/* Connect to Analog Globals */                                                  
#define Tpos_AG                     (* (reg8 *) Tpos__AG)                       
/* Analog MUX bux enable */
#define Tpos_AMUX                   (* (reg8 *) Tpos__AMUX) 
/* Bidirectional Enable */                                                        
#define Tpos_BIE                    (* (reg8 *) Tpos__BIE)
/* Bit-mask for Aliased Register Access */
#define Tpos_BIT_MASK               (* (reg8 *) Tpos__BIT_MASK)
/* Bypass Enable */
#define Tpos_BYP                    (* (reg8 *) Tpos__BYP)
/* Port wide control signals */                                                   
#define Tpos_CTL                    (* (reg8 *) Tpos__CTL)
/* Drive Modes */
#define Tpos_DM0                    (* (reg8 *) Tpos__DM0) 
#define Tpos_DM1                    (* (reg8 *) Tpos__DM1)
#define Tpos_DM2                    (* (reg8 *) Tpos__DM2) 
/* Input Buffer Disable Override */
#define Tpos_INP_DIS                (* (reg8 *) Tpos__INP_DIS)
/* LCD Common or Segment Drive */
#define Tpos_LCD_COM_SEG            (* (reg8 *) Tpos__LCD_COM_SEG)
/* Enable Segment LCD */
#define Tpos_LCD_EN                 (* (reg8 *) Tpos__LCD_EN)
/* Slew Rate Control */
#define Tpos_SLW                    (* (reg8 *) Tpos__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Tpos_PRTDSI__CAPS_SEL       (* (reg8 *) Tpos__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Tpos_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Tpos__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Tpos_PRTDSI__OE_SEL0        (* (reg8 *) Tpos__PRTDSI__OE_SEL0) 
#define Tpos_PRTDSI__OE_SEL1        (* (reg8 *) Tpos__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Tpos_PRTDSI__OUT_SEL0       (* (reg8 *) Tpos__PRTDSI__OUT_SEL0) 
#define Tpos_PRTDSI__OUT_SEL1       (* (reg8 *) Tpos__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Tpos_PRTDSI__SYNC_OUT       (* (reg8 *) Tpos__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Tpos__SIO_CFG)
    #define Tpos_SIO_HYST_EN        (* (reg8 *) Tpos__SIO_HYST_EN)
    #define Tpos_SIO_REG_HIFREQ     (* (reg8 *) Tpos__SIO_REG_HIFREQ)
    #define Tpos_SIO_CFG            (* (reg8 *) Tpos__SIO_CFG)
    #define Tpos_SIO_DIFF           (* (reg8 *) Tpos__SIO_DIFF)
#endif /* (Tpos__SIO_CFG) */

/* Interrupt Registers */
#if defined(Tpos__INTSTAT)
    #define Tpos_INTSTAT            (* (reg8 *) Tpos__INTSTAT)
    #define Tpos_SNAP               (* (reg8 *) Tpos__SNAP)
    
	#define Tpos_0_INTTYPE_REG 		(* (reg8 *) Tpos__0__INTTYPE)
#endif /* (Tpos__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Tpos_H */


/* [] END OF FILE */
