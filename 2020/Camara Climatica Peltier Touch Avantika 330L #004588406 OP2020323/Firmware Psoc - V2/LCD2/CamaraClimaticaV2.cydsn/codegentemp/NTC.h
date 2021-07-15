/*******************************************************************************
* File Name: NTC.h  
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

#if !defined(CY_PINS_NTC_H) /* Pins NTC_H */
#define CY_PINS_NTC_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "NTC_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 NTC__PORT == 15 && ((NTC__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    NTC_Write(uint8 value);
void    NTC_SetDriveMode(uint8 mode);
uint8   NTC_ReadDataReg(void);
uint8   NTC_Read(void);
void    NTC_SetInterruptMode(uint16 position, uint16 mode);
uint8   NTC_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the NTC_SetDriveMode() function.
     *  @{
     */
        #define NTC_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define NTC_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define NTC_DM_RES_UP          PIN_DM_RES_UP
        #define NTC_DM_RES_DWN         PIN_DM_RES_DWN
        #define NTC_DM_OD_LO           PIN_DM_OD_LO
        #define NTC_DM_OD_HI           PIN_DM_OD_HI
        #define NTC_DM_STRONG          PIN_DM_STRONG
        #define NTC_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define NTC_MASK               NTC__MASK
#define NTC_SHIFT              NTC__SHIFT
#define NTC_WIDTH              1u

/* Interrupt constants */
#if defined(NTC__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in NTC_SetInterruptMode() function.
     *  @{
     */
        #define NTC_INTR_NONE      (uint16)(0x0000u)
        #define NTC_INTR_RISING    (uint16)(0x0001u)
        #define NTC_INTR_FALLING   (uint16)(0x0002u)
        #define NTC_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define NTC_INTR_MASK      (0x01u) 
#endif /* (NTC__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define NTC_PS                     (* (reg8 *) NTC__PS)
/* Data Register */
#define NTC_DR                     (* (reg8 *) NTC__DR)
/* Port Number */
#define NTC_PRT_NUM                (* (reg8 *) NTC__PRT) 
/* Connect to Analog Globals */                                                  
#define NTC_AG                     (* (reg8 *) NTC__AG)                       
/* Analog MUX bux enable */
#define NTC_AMUX                   (* (reg8 *) NTC__AMUX) 
/* Bidirectional Enable */                                                        
#define NTC_BIE                    (* (reg8 *) NTC__BIE)
/* Bit-mask for Aliased Register Access */
#define NTC_BIT_MASK               (* (reg8 *) NTC__BIT_MASK)
/* Bypass Enable */
#define NTC_BYP                    (* (reg8 *) NTC__BYP)
/* Port wide control signals */                                                   
#define NTC_CTL                    (* (reg8 *) NTC__CTL)
/* Drive Modes */
#define NTC_DM0                    (* (reg8 *) NTC__DM0) 
#define NTC_DM1                    (* (reg8 *) NTC__DM1)
#define NTC_DM2                    (* (reg8 *) NTC__DM2) 
/* Input Buffer Disable Override */
#define NTC_INP_DIS                (* (reg8 *) NTC__INP_DIS)
/* LCD Common or Segment Drive */
#define NTC_LCD_COM_SEG            (* (reg8 *) NTC__LCD_COM_SEG)
/* Enable Segment LCD */
#define NTC_LCD_EN                 (* (reg8 *) NTC__LCD_EN)
/* Slew Rate Control */
#define NTC_SLW                    (* (reg8 *) NTC__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define NTC_PRTDSI__CAPS_SEL       (* (reg8 *) NTC__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define NTC_PRTDSI__DBL_SYNC_IN    (* (reg8 *) NTC__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define NTC_PRTDSI__OE_SEL0        (* (reg8 *) NTC__PRTDSI__OE_SEL0) 
#define NTC_PRTDSI__OE_SEL1        (* (reg8 *) NTC__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define NTC_PRTDSI__OUT_SEL0       (* (reg8 *) NTC__PRTDSI__OUT_SEL0) 
#define NTC_PRTDSI__OUT_SEL1       (* (reg8 *) NTC__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define NTC_PRTDSI__SYNC_OUT       (* (reg8 *) NTC__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(NTC__SIO_CFG)
    #define NTC_SIO_HYST_EN        (* (reg8 *) NTC__SIO_HYST_EN)
    #define NTC_SIO_REG_HIFREQ     (* (reg8 *) NTC__SIO_REG_HIFREQ)
    #define NTC_SIO_CFG            (* (reg8 *) NTC__SIO_CFG)
    #define NTC_SIO_DIFF           (* (reg8 *) NTC__SIO_DIFF)
#endif /* (NTC__SIO_CFG) */

/* Interrupt Registers */
#if defined(NTC__INTSTAT)
    #define NTC_INTSTAT            (* (reg8 *) NTC__INTSTAT)
    #define NTC_SNAP               (* (reg8 *) NTC__SNAP)
    
	#define NTC_0_INTTYPE_REG 		(* (reg8 *) NTC__0__INTTYPE)
#endif /* (NTC__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_NTC_H */


/* [] END OF FILE */
