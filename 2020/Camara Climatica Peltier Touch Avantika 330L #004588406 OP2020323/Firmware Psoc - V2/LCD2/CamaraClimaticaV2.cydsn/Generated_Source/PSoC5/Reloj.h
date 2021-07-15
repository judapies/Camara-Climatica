/*******************************************************************************
* File Name: Reloj.h
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
#if !defined(CY_ISR_Reloj_H)
#define CY_ISR_Reloj_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void Reloj_Start(void);
void Reloj_StartEx(cyisraddress address);
void Reloj_Stop(void);

CY_ISR_PROTO(Reloj_Interrupt);

void Reloj_SetVector(cyisraddress address);
cyisraddress Reloj_GetVector(void);

void Reloj_SetPriority(uint8 priority);
uint8 Reloj_GetPriority(void);

void Reloj_Enable(void);
uint8 Reloj_GetState(void);
void Reloj_Disable(void);

void Reloj_SetPending(void);
void Reloj_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the Reloj ISR. */
#define Reloj_INTC_VECTOR            ((reg32 *) Reloj__INTC_VECT)

/* Address of the Reloj ISR priority. */
#define Reloj_INTC_PRIOR             ((reg8 *) Reloj__INTC_PRIOR_REG)

/* Priority of the Reloj interrupt. */
#define Reloj_INTC_PRIOR_NUMBER      Reloj__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable Reloj interrupt. */
#define Reloj_INTC_SET_EN            ((reg32 *) Reloj__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the Reloj interrupt. */
#define Reloj_INTC_CLR_EN            ((reg32 *) Reloj__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the Reloj interrupt state to pending. */
#define Reloj_INTC_SET_PD            ((reg32 *) Reloj__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the Reloj interrupt. */
#define Reloj_INTC_CLR_PD            ((reg32 *) Reloj__INTC_CLR_PD_REG)


#endif /* CY_ISR_Reloj_H */


/* [] END OF FILE */
