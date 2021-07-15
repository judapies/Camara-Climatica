/*******************************************************************************
* File Name: VSS_REF.h  
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

#if !defined(CY_PINS_VSS_REF_H) /* Pins VSS_REF_H */
#define CY_PINS_VSS_REF_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "VSS_REF_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 VSS_REF__PORT == 15 && ((VSS_REF__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    VSS_REF_Write(uint8 value);
void    VSS_REF_SetDriveMode(uint8 mode);
uint8   VSS_REF_ReadDataReg(void);
uint8   VSS_REF_Read(void);
void    VSS_REF_SetInterruptMode(uint16 position, uint16 mode);
uint8   VSS_REF_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the VSS_REF_SetDriveMode() function.
     *  @{
     */
        #define VSS_REF_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define VSS_REF_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define VSS_REF_DM_RES_UP          PIN_DM_RES_UP
        #define VSS_REF_DM_RES_DWN         PIN_DM_RES_DWN
        #define VSS_REF_DM_OD_LO           PIN_DM_OD_LO
        #define VSS_REF_DM_OD_HI           PIN_DM_OD_HI
        #define VSS_REF_DM_STRONG          PIN_DM_STRONG
        #define VSS_REF_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define VSS_REF_MASK               VSS_REF__MASK
#define VSS_REF_SHIFT              VSS_REF__SHIFT
#define VSS_REF_WIDTH              1u

/* Interrupt constants */
#if defined(VSS_REF__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in VSS_REF_SetInterruptMode() function.
     *  @{
     */
        #define VSS_REF_INTR_NONE      (uint16)(0x0000u)
        #define VSS_REF_INTR_RISING    (uint16)(0x0001u)
        #define VSS_REF_INTR_FALLING   (uint16)(0x0002u)
        #define VSS_REF_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define VSS_REF_INTR_MASK      (0x01u) 
#endif /* (VSS_REF__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define VSS_REF_PS                     (* (reg8 *) VSS_REF__PS)
/* Data Register */
#define VSS_REF_DR                     (* (reg8 *) VSS_REF__DR)
/* Port Number */
#define VSS_REF_PRT_NUM                (* (reg8 *) VSS_REF__PRT) 
/* Connect to Analog Globals */                                                  
#define VSS_REF_AG                     (* (reg8 *) VSS_REF__AG)                       
/* Analog MUX bux enable */
#define VSS_REF_AMUX                   (* (reg8 *) VSS_REF__AMUX) 
/* Bidirectional Enable */                                                        
#define VSS_REF_BIE                    (* (reg8 *) VSS_REF__BIE)
/* Bit-mask for Aliased Register Access */
#define VSS_REF_BIT_MASK               (* (reg8 *) VSS_REF__BIT_MASK)
/* Bypass Enable */
#define VSS_REF_BYP                    (* (reg8 *) VSS_REF__BYP)
/* Port wide control signals */                                                   
#define VSS_REF_CTL                    (* (reg8 *) VSS_REF__CTL)
/* Drive Modes */
#define VSS_REF_DM0                    (* (reg8 *) VSS_REF__DM0) 
#define VSS_REF_DM1                    (* (reg8 *) VSS_REF__DM1)
#define VSS_REF_DM2                    (* (reg8 *) VSS_REF__DM2) 
/* Input Buffer Disable Override */
#define VSS_REF_INP_DIS                (* (reg8 *) VSS_REF__INP_DIS)
/* LCD Common or Segment Drive */
#define VSS_REF_LCD_COM_SEG            (* (reg8 *) VSS_REF__LCD_COM_SEG)
/* Enable Segment LCD */
#define VSS_REF_LCD_EN                 (* (reg8 *) VSS_REF__LCD_EN)
/* Slew Rate Control */
#define VSS_REF_SLW                    (* (reg8 *) VSS_REF__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define VSS_REF_PRTDSI__CAPS_SEL       (* (reg8 *) VSS_REF__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define VSS_REF_PRTDSI__DBL_SYNC_IN    (* (reg8 *) VSS_REF__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define VSS_REF_PRTDSI__OE_SEL0        (* (reg8 *) VSS_REF__PRTDSI__OE_SEL0) 
#define VSS_REF_PRTDSI__OE_SEL1        (* (reg8 *) VSS_REF__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define VSS_REF_PRTDSI__OUT_SEL0       (* (reg8 *) VSS_REF__PRTDSI__OUT_SEL0) 
#define VSS_REF_PRTDSI__OUT_SEL1       (* (reg8 *) VSS_REF__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define VSS_REF_PRTDSI__SYNC_OUT       (* (reg8 *) VSS_REF__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(VSS_REF__SIO_CFG)
    #define VSS_REF_SIO_HYST_EN        (* (reg8 *) VSS_REF__SIO_HYST_EN)
    #define VSS_REF_SIO_REG_HIFREQ     (* (reg8 *) VSS_REF__SIO_REG_HIFREQ)
    #define VSS_REF_SIO_CFG            (* (reg8 *) VSS_REF__SIO_CFG)
    #define VSS_REF_SIO_DIFF           (* (reg8 *) VSS_REF__SIO_DIFF)
#endif /* (VSS_REF__SIO_CFG) */

/* Interrupt Registers */
#if defined(VSS_REF__INTSTAT)
    #define VSS_REF_INTSTAT            (* (reg8 *) VSS_REF__INTSTAT)
    #define VSS_REF_SNAP               (* (reg8 *) VSS_REF__SNAP)
    
	#define VSS_REF_0_INTTYPE_REG 		(* (reg8 *) VSS_REF__0__INTTYPE)
#endif /* (VSS_REF__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_VSS_REF_H */


/* [] END OF FILE */
