/*******************************************************************************
* File Name: ClockPWM.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_ClockPWM_H)
#define CY_CLOCK_ClockPWM_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
* Conditional Compilation Parameters
***************************************/

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component cy_clock_v2_20 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */


/***************************************
*        Function Prototypes
***************************************/

void ClockPWM_Start(void) ;
void ClockPWM_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void ClockPWM_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void ClockPWM_StandbyPower(uint8 state) ;
void ClockPWM_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 ClockPWM_GetDividerRegister(void) ;
void ClockPWM_SetModeRegister(uint8 modeBitMask) ;
void ClockPWM_ClearModeRegister(uint8 modeBitMask) ;
uint8 ClockPWM_GetModeRegister(void) ;
void ClockPWM_SetSourceRegister(uint8 clkSource) ;
uint8 ClockPWM_GetSourceRegister(void) ;
#if defined(ClockPWM__CFG3)
void ClockPWM_SetPhaseRegister(uint8 clkPhase) ;
uint8 ClockPWM_GetPhaseRegister(void) ;
#endif /* defined(ClockPWM__CFG3) */

#define ClockPWM_Enable()                       ClockPWM_Start()
#define ClockPWM_Disable()                      ClockPWM_Stop()
#define ClockPWM_SetDivider(clkDivider)         ClockPWM_SetDividerRegister(clkDivider, 1u)
#define ClockPWM_SetDividerValue(clkDivider)    ClockPWM_SetDividerRegister((clkDivider) - 1u, 1u)
#define ClockPWM_SetMode(clkMode)               ClockPWM_SetModeRegister(clkMode)
#define ClockPWM_SetSource(clkSource)           ClockPWM_SetSourceRegister(clkSource)
#if defined(ClockPWM__CFG3)
#define ClockPWM_SetPhase(clkPhase)             ClockPWM_SetPhaseRegister(clkPhase)
#define ClockPWM_SetPhaseValue(clkPhase)        ClockPWM_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(ClockPWM__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define ClockPWM_CLKEN              (* (reg8 *) ClockPWM__PM_ACT_CFG)
#define ClockPWM_CLKEN_PTR          ((reg8 *) ClockPWM__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define ClockPWM_CLKSTBY            (* (reg8 *) ClockPWM__PM_STBY_CFG)
#define ClockPWM_CLKSTBY_PTR        ((reg8 *) ClockPWM__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define ClockPWM_DIV_LSB            (* (reg8 *) ClockPWM__CFG0)
#define ClockPWM_DIV_LSB_PTR        ((reg8 *) ClockPWM__CFG0)
#define ClockPWM_DIV_PTR            ((reg16 *) ClockPWM__CFG0)

/* Clock MSB divider configuration register. */
#define ClockPWM_DIV_MSB            (* (reg8 *) ClockPWM__CFG1)
#define ClockPWM_DIV_MSB_PTR        ((reg8 *) ClockPWM__CFG1)

/* Mode and source configuration register */
#define ClockPWM_MOD_SRC            (* (reg8 *) ClockPWM__CFG2)
#define ClockPWM_MOD_SRC_PTR        ((reg8 *) ClockPWM__CFG2)

#if defined(ClockPWM__CFG3)
/* Analog clock phase configuration register */
#define ClockPWM_PHASE              (* (reg8 *) ClockPWM__CFG3)
#define ClockPWM_PHASE_PTR          ((reg8 *) ClockPWM__CFG3)
#endif /* defined(ClockPWM__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define ClockPWM_CLKEN_MASK         ClockPWM__PM_ACT_MSK
#define ClockPWM_CLKSTBY_MASK       ClockPWM__PM_STBY_MSK

/* CFG2 field masks */
#define ClockPWM_SRC_SEL_MSK        ClockPWM__CFG2_SRC_SEL_MASK
#define ClockPWM_MODE_MASK          (~(ClockPWM_SRC_SEL_MSK))

#if defined(ClockPWM__CFG3)
/* CFG3 phase mask */
#define ClockPWM_PHASE_MASK         ClockPWM__CFG3_PHASE_DLY_MASK
#endif /* defined(ClockPWM__CFG3) */

#endif /* CY_CLOCK_ClockPWM_H */


/* [] END OF FILE */
