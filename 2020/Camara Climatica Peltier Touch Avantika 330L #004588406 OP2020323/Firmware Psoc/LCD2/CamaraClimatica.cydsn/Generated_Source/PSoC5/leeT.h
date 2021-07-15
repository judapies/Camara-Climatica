/*******************************************************************************
* File Name: leeT.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_leeT_H)
#define CY_ISR_leeT_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void leeT_Start(void);
void leeT_StartEx(cyisraddress address);
void leeT_Stop(void);

CY_ISR_PROTO(leeT_Interrupt);

void leeT_SetVector(cyisraddress address);
cyisraddress leeT_GetVector(void);

void leeT_SetPriority(uint8 priority);
uint8 leeT_GetPriority(void);

void leeT_Enable(void);
uint8 leeT_GetState(void);
void leeT_Disable(void);

void leeT_SetPending(void);
void leeT_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the leeT ISR. */
#define leeT_INTC_VECTOR            ((reg32 *) leeT__INTC_VECT)

/* Address of the leeT ISR priority. */
#define leeT_INTC_PRIOR             ((reg8 *) leeT__INTC_PRIOR_REG)

/* Priority of the leeT interrupt. */
#define leeT_INTC_PRIOR_NUMBER      leeT__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable leeT interrupt. */
#define leeT_INTC_SET_EN            ((reg32 *) leeT__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the leeT interrupt. */
#define leeT_INTC_CLR_EN            ((reg32 *) leeT__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the leeT interrupt state to pending. */
#define leeT_INTC_SET_PD            ((reg32 *) leeT__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the leeT interrupt. */
#define leeT_INTC_CLR_PD            ((reg32 *) leeT__INTC_CLR_PD_REG)


#endif /* CY_ISR_leeT_H */


/* [] END OF FILE */
