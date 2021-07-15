/*******************************************************************************
* File Name: LCD1.h  
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

#if !defined(CY_PINS_LCD1_H) /* Pins LCD1_H */
#define CY_PINS_LCD1_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "LCD1_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 LCD1__PORT == 15 && ((LCD1__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    LCD1_Write(uint8 value);
void    LCD1_SetDriveMode(uint8 mode);
uint8   LCD1_ReadDataReg(void);
uint8   LCD1_Read(void);
void    LCD1_SetInterruptMode(uint16 position, uint16 mode);
uint8   LCD1_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the LCD1_SetDriveMode() function.
     *  @{
     */
        #define LCD1_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define LCD1_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define LCD1_DM_RES_UP          PIN_DM_RES_UP
        #define LCD1_DM_RES_DWN         PIN_DM_RES_DWN
        #define LCD1_DM_OD_LO           PIN_DM_OD_LO
        #define LCD1_DM_OD_HI           PIN_DM_OD_HI
        #define LCD1_DM_STRONG          PIN_DM_STRONG
        #define LCD1_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define LCD1_MASK               LCD1__MASK
#define LCD1_SHIFT              LCD1__SHIFT
#define LCD1_WIDTH              1u

/* Interrupt constants */
#if defined(LCD1__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in LCD1_SetInterruptMode() function.
     *  @{
     */
        #define LCD1_INTR_NONE      (uint16)(0x0000u)
        #define LCD1_INTR_RISING    (uint16)(0x0001u)
        #define LCD1_INTR_FALLING   (uint16)(0x0002u)
        #define LCD1_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define LCD1_INTR_MASK      (0x01u) 
#endif /* (LCD1__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define LCD1_PS                     (* (reg8 *) LCD1__PS)
/* Data Register */
#define LCD1_DR                     (* (reg8 *) LCD1__DR)
/* Port Number */
#define LCD1_PRT_NUM                (* (reg8 *) LCD1__PRT) 
/* Connect to Analog Globals */                                                  
#define LCD1_AG                     (* (reg8 *) LCD1__AG)                       
/* Analog MUX bux enable */
#define LCD1_AMUX                   (* (reg8 *) LCD1__AMUX) 
/* Bidirectional Enable */                                                        
#define LCD1_BIE                    (* (reg8 *) LCD1__BIE)
/* Bit-mask for Aliased Register Access */
#define LCD1_BIT_MASK               (* (reg8 *) LCD1__BIT_MASK)
/* Bypass Enable */
#define LCD1_BYP                    (* (reg8 *) LCD1__BYP)
/* Port wide control signals */                                                   
#define LCD1_CTL                    (* (reg8 *) LCD1__CTL)
/* Drive Modes */
#define LCD1_DM0                    (* (reg8 *) LCD1__DM0) 
#define LCD1_DM1                    (* (reg8 *) LCD1__DM1)
#define LCD1_DM2                    (* (reg8 *) LCD1__DM2) 
/* Input Buffer Disable Override */
#define LCD1_INP_DIS                (* (reg8 *) LCD1__INP_DIS)
/* LCD Common or Segment Drive */
#define LCD1_LCD_COM_SEG            (* (reg8 *) LCD1__LCD_COM_SEG)
/* Enable Segment LCD */
#define LCD1_LCD_EN                 (* (reg8 *) LCD1__LCD_EN)
/* Slew Rate Control */
#define LCD1_SLW                    (* (reg8 *) LCD1__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define LCD1_PRTDSI__CAPS_SEL       (* (reg8 *) LCD1__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define LCD1_PRTDSI__DBL_SYNC_IN    (* (reg8 *) LCD1__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define LCD1_PRTDSI__OE_SEL0        (* (reg8 *) LCD1__PRTDSI__OE_SEL0) 
#define LCD1_PRTDSI__OE_SEL1        (* (reg8 *) LCD1__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define LCD1_PRTDSI__OUT_SEL0       (* (reg8 *) LCD1__PRTDSI__OUT_SEL0) 
#define LCD1_PRTDSI__OUT_SEL1       (* (reg8 *) LCD1__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define LCD1_PRTDSI__SYNC_OUT       (* (reg8 *) LCD1__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(LCD1__SIO_CFG)
    #define LCD1_SIO_HYST_EN        (* (reg8 *) LCD1__SIO_HYST_EN)
    #define LCD1_SIO_REG_HIFREQ     (* (reg8 *) LCD1__SIO_REG_HIFREQ)
    #define LCD1_SIO_CFG            (* (reg8 *) LCD1__SIO_CFG)
    #define LCD1_SIO_DIFF           (* (reg8 *) LCD1__SIO_DIFF)
#endif /* (LCD1__SIO_CFG) */

/* Interrupt Registers */
#if defined(LCD1__INTSTAT)
    #define LCD1_INTSTAT            (* (reg8 *) LCD1__INTSTAT)
    #define LCD1_SNAP               (* (reg8 *) LCD1__SNAP)
    
	#define LCD1_0_INTTYPE_REG 		(* (reg8 *) LCD1__0__INTTYPE)
#endif /* (LCD1__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_LCD1_H */


/* [] END OF FILE */
