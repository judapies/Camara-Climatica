/*******************************************************************************
* File Name: CONTACTOR.h  
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

#if !defined(CY_PINS_CONTACTOR_H) /* Pins CONTACTOR_H */
#define CY_PINS_CONTACTOR_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "CONTACTOR_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 CONTACTOR__PORT == 15 && ((CONTACTOR__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    CONTACTOR_Write(uint8 value);
void    CONTACTOR_SetDriveMode(uint8 mode);
uint8   CONTACTOR_ReadDataReg(void);
uint8   CONTACTOR_Read(void);
void    CONTACTOR_SetInterruptMode(uint16 position, uint16 mode);
uint8   CONTACTOR_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the CONTACTOR_SetDriveMode() function.
     *  @{
     */
        #define CONTACTOR_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define CONTACTOR_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define CONTACTOR_DM_RES_UP          PIN_DM_RES_UP
        #define CONTACTOR_DM_RES_DWN         PIN_DM_RES_DWN
        #define CONTACTOR_DM_OD_LO           PIN_DM_OD_LO
        #define CONTACTOR_DM_OD_HI           PIN_DM_OD_HI
        #define CONTACTOR_DM_STRONG          PIN_DM_STRONG
        #define CONTACTOR_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define CONTACTOR_MASK               CONTACTOR__MASK
#define CONTACTOR_SHIFT              CONTACTOR__SHIFT
#define CONTACTOR_WIDTH              1u

/* Interrupt constants */
#if defined(CONTACTOR__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in CONTACTOR_SetInterruptMode() function.
     *  @{
     */
        #define CONTACTOR_INTR_NONE      (uint16)(0x0000u)
        #define CONTACTOR_INTR_RISING    (uint16)(0x0001u)
        #define CONTACTOR_INTR_FALLING   (uint16)(0x0002u)
        #define CONTACTOR_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define CONTACTOR_INTR_MASK      (0x01u) 
#endif /* (CONTACTOR__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define CONTACTOR_PS                     (* (reg8 *) CONTACTOR__PS)
/* Data Register */
#define CONTACTOR_DR                     (* (reg8 *) CONTACTOR__DR)
/* Port Number */
#define CONTACTOR_PRT_NUM                (* (reg8 *) CONTACTOR__PRT) 
/* Connect to Analog Globals */                                                  
#define CONTACTOR_AG                     (* (reg8 *) CONTACTOR__AG)                       
/* Analog MUX bux enable */
#define CONTACTOR_AMUX                   (* (reg8 *) CONTACTOR__AMUX) 
/* Bidirectional Enable */                                                        
#define CONTACTOR_BIE                    (* (reg8 *) CONTACTOR__BIE)
/* Bit-mask for Aliased Register Access */
#define CONTACTOR_BIT_MASK               (* (reg8 *) CONTACTOR__BIT_MASK)
/* Bypass Enable */
#define CONTACTOR_BYP                    (* (reg8 *) CONTACTOR__BYP)
/* Port wide control signals */                                                   
#define CONTACTOR_CTL                    (* (reg8 *) CONTACTOR__CTL)
/* Drive Modes */
#define CONTACTOR_DM0                    (* (reg8 *) CONTACTOR__DM0) 
#define CONTACTOR_DM1                    (* (reg8 *) CONTACTOR__DM1)
#define CONTACTOR_DM2                    (* (reg8 *) CONTACTOR__DM2) 
/* Input Buffer Disable Override */
#define CONTACTOR_INP_DIS                (* (reg8 *) CONTACTOR__INP_DIS)
/* LCD Common or Segment Drive */
#define CONTACTOR_LCD_COM_SEG            (* (reg8 *) CONTACTOR__LCD_COM_SEG)
/* Enable Segment LCD */
#define CONTACTOR_LCD_EN                 (* (reg8 *) CONTACTOR__LCD_EN)
/* Slew Rate Control */
#define CONTACTOR_SLW                    (* (reg8 *) CONTACTOR__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define CONTACTOR_PRTDSI__CAPS_SEL       (* (reg8 *) CONTACTOR__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define CONTACTOR_PRTDSI__DBL_SYNC_IN    (* (reg8 *) CONTACTOR__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define CONTACTOR_PRTDSI__OE_SEL0        (* (reg8 *) CONTACTOR__PRTDSI__OE_SEL0) 
#define CONTACTOR_PRTDSI__OE_SEL1        (* (reg8 *) CONTACTOR__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define CONTACTOR_PRTDSI__OUT_SEL0       (* (reg8 *) CONTACTOR__PRTDSI__OUT_SEL0) 
#define CONTACTOR_PRTDSI__OUT_SEL1       (* (reg8 *) CONTACTOR__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define CONTACTOR_PRTDSI__SYNC_OUT       (* (reg8 *) CONTACTOR__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(CONTACTOR__SIO_CFG)
    #define CONTACTOR_SIO_HYST_EN        (* (reg8 *) CONTACTOR__SIO_HYST_EN)
    #define CONTACTOR_SIO_REG_HIFREQ     (* (reg8 *) CONTACTOR__SIO_REG_HIFREQ)
    #define CONTACTOR_SIO_CFG            (* (reg8 *) CONTACTOR__SIO_CFG)
    #define CONTACTOR_SIO_DIFF           (* (reg8 *) CONTACTOR__SIO_DIFF)
#endif /* (CONTACTOR__SIO_CFG) */

/* Interrupt Registers */
#if defined(CONTACTOR__INTSTAT)
    #define CONTACTOR_INTSTAT            (* (reg8 *) CONTACTOR__INTSTAT)
    #define CONTACTOR_SNAP               (* (reg8 *) CONTACTOR__SNAP)
    
	#define CONTACTOR_0_INTTYPE_REG 		(* (reg8 *) CONTACTOR__0__INTTYPE)
#endif /* (CONTACTOR__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_CONTACTOR_H */


/* [] END OF FILE */
