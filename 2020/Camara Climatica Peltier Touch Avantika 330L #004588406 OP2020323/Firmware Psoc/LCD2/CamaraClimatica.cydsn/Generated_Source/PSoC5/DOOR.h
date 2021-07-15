/*******************************************************************************
* File Name: DOOR.h  
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

#if !defined(CY_PINS_DOOR_H) /* Pins DOOR_H */
#define CY_PINS_DOOR_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "DOOR_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 DOOR__PORT == 15 && ((DOOR__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    DOOR_Write(uint8 value);
void    DOOR_SetDriveMode(uint8 mode);
uint8   DOOR_ReadDataReg(void);
uint8   DOOR_Read(void);
void    DOOR_SetInterruptMode(uint16 position, uint16 mode);
uint8   DOOR_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the DOOR_SetDriveMode() function.
     *  @{
     */
        #define DOOR_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define DOOR_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define DOOR_DM_RES_UP          PIN_DM_RES_UP
        #define DOOR_DM_RES_DWN         PIN_DM_RES_DWN
        #define DOOR_DM_OD_LO           PIN_DM_OD_LO
        #define DOOR_DM_OD_HI           PIN_DM_OD_HI
        #define DOOR_DM_STRONG          PIN_DM_STRONG
        #define DOOR_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define DOOR_MASK               DOOR__MASK
#define DOOR_SHIFT              DOOR__SHIFT
#define DOOR_WIDTH              1u

/* Interrupt constants */
#if defined(DOOR__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in DOOR_SetInterruptMode() function.
     *  @{
     */
        #define DOOR_INTR_NONE      (uint16)(0x0000u)
        #define DOOR_INTR_RISING    (uint16)(0x0001u)
        #define DOOR_INTR_FALLING   (uint16)(0x0002u)
        #define DOOR_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define DOOR_INTR_MASK      (0x01u) 
#endif /* (DOOR__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define DOOR_PS                     (* (reg8 *) DOOR__PS)
/* Data Register */
#define DOOR_DR                     (* (reg8 *) DOOR__DR)
/* Port Number */
#define DOOR_PRT_NUM                (* (reg8 *) DOOR__PRT) 
/* Connect to Analog Globals */                                                  
#define DOOR_AG                     (* (reg8 *) DOOR__AG)                       
/* Analog MUX bux enable */
#define DOOR_AMUX                   (* (reg8 *) DOOR__AMUX) 
/* Bidirectional Enable */                                                        
#define DOOR_BIE                    (* (reg8 *) DOOR__BIE)
/* Bit-mask for Aliased Register Access */
#define DOOR_BIT_MASK               (* (reg8 *) DOOR__BIT_MASK)
/* Bypass Enable */
#define DOOR_BYP                    (* (reg8 *) DOOR__BYP)
/* Port wide control signals */                                                   
#define DOOR_CTL                    (* (reg8 *) DOOR__CTL)
/* Drive Modes */
#define DOOR_DM0                    (* (reg8 *) DOOR__DM0) 
#define DOOR_DM1                    (* (reg8 *) DOOR__DM1)
#define DOOR_DM2                    (* (reg8 *) DOOR__DM2) 
/* Input Buffer Disable Override */
#define DOOR_INP_DIS                (* (reg8 *) DOOR__INP_DIS)
/* LCD Common or Segment Drive */
#define DOOR_LCD_COM_SEG            (* (reg8 *) DOOR__LCD_COM_SEG)
/* Enable Segment LCD */
#define DOOR_LCD_EN                 (* (reg8 *) DOOR__LCD_EN)
/* Slew Rate Control */
#define DOOR_SLW                    (* (reg8 *) DOOR__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define DOOR_PRTDSI__CAPS_SEL       (* (reg8 *) DOOR__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define DOOR_PRTDSI__DBL_SYNC_IN    (* (reg8 *) DOOR__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define DOOR_PRTDSI__OE_SEL0        (* (reg8 *) DOOR__PRTDSI__OE_SEL0) 
#define DOOR_PRTDSI__OE_SEL1        (* (reg8 *) DOOR__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define DOOR_PRTDSI__OUT_SEL0       (* (reg8 *) DOOR__PRTDSI__OUT_SEL0) 
#define DOOR_PRTDSI__OUT_SEL1       (* (reg8 *) DOOR__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define DOOR_PRTDSI__SYNC_OUT       (* (reg8 *) DOOR__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(DOOR__SIO_CFG)
    #define DOOR_SIO_HYST_EN        (* (reg8 *) DOOR__SIO_HYST_EN)
    #define DOOR_SIO_REG_HIFREQ     (* (reg8 *) DOOR__SIO_REG_HIFREQ)
    #define DOOR_SIO_CFG            (* (reg8 *) DOOR__SIO_CFG)
    #define DOOR_SIO_DIFF           (* (reg8 *) DOOR__SIO_DIFF)
#endif /* (DOOR__SIO_CFG) */

/* Interrupt Registers */
#if defined(DOOR__INTSTAT)
    #define DOOR_INTSTAT            (* (reg8 *) DOOR__INTSTAT)
    #define DOOR_SNAP               (* (reg8 *) DOOR__SNAP)
    
	#define DOOR_0_INTTYPE_REG 		(* (reg8 *) DOOR__0__INTTYPE)
#endif /* (DOOR__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_DOOR_H */


/* [] END OF FILE */
