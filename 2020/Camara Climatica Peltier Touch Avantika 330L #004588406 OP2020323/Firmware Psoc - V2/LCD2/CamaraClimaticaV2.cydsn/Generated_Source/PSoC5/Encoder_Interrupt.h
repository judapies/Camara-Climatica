/*******************************************************************************
* File Name: Encoder_Interrupt.h
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
#if !defined(CY_ISR_Encoder_Interrupt_H)
#define CY_ISR_Encoder_Interrupt_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void Encoder_Interrupt_Start(void);
void Encoder_Interrupt_StartEx(cyisraddress address);
void Encoder_Interrupt_Stop(void);

CY_ISR_PROTO(Encoder_Interrupt_Interrupt);

void Encoder_Interrupt_SetVector(cyisraddress address);
cyisraddress Encoder_Interrupt_GetVector(void);

void Encoder_Interrupt_SetPriority(uint8 priority);
uint8 Encoder_Interrupt_GetPriority(void);

void Encoder_Interrupt_Enable(void);
uint8 Encoder_Interrupt_GetState(void);
void Encoder_Interrupt_Disable(void);

void Encoder_Interrupt_SetPending(void);
void Encoder_Interrupt_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the Encoder_Interrupt ISR. */
#define Encoder_Interrupt_INTC_VECTOR            ((reg32 *) Encoder_Interrupt__INTC_VECT)

/* Address of the Encoder_Interrupt ISR priority. */
#define Encoder_Interrupt_INTC_PRIOR             ((reg8 *) Encoder_Interrupt__INTC_PRIOR_REG)

/* Priority of the Encoder_Interrupt interrupt. */
#define Encoder_Interrupt_INTC_PRIOR_NUMBER      Encoder_Interrupt__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable Encoder_Interrupt interrupt. */
#define Encoder_Interrupt_INTC_SET_EN            ((reg32 *) Encoder_Interrupt__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the Encoder_Interrupt interrupt. */
#define Encoder_Interrupt_INTC_CLR_EN            ((reg32 *) Encoder_Interrupt__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the Encoder_Interrupt interrupt state to pending. */
#define Encoder_Interrupt_INTC_SET_PD            ((reg32 *) Encoder_Interrupt__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the Encoder_Interrupt interrupt. */
#define Encoder_Interrupt_INTC_CLR_PD            ((reg32 *) Encoder_Interrupt__INTC_CLR_PD_REG)


#endif /* CY_ISR_Encoder_Interrupt_H */


/* [] END OF FILE */
