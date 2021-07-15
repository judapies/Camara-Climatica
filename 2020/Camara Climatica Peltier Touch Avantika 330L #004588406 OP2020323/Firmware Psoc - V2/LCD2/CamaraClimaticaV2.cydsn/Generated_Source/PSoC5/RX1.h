/*******************************************************************************
* File Name: RX1.h
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
#if !defined(CY_ISR_RX1_H)
#define CY_ISR_RX1_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void RX1_Start(void);
void RX1_StartEx(cyisraddress address);
void RX1_Stop(void);

CY_ISR_PROTO(RX1_Interrupt);

void RX1_SetVector(cyisraddress address);
cyisraddress RX1_GetVector(void);

void RX1_SetPriority(uint8 priority);
uint8 RX1_GetPriority(void);

void RX1_Enable(void);
uint8 RX1_GetState(void);
void RX1_Disable(void);

void RX1_SetPending(void);
void RX1_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the RX1 ISR. */
#define RX1_INTC_VECTOR            ((reg32 *) RX1__INTC_VECT)

/* Address of the RX1 ISR priority. */
#define RX1_INTC_PRIOR             ((reg8 *) RX1__INTC_PRIOR_REG)

/* Priority of the RX1 interrupt. */
#define RX1_INTC_PRIOR_NUMBER      RX1__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable RX1 interrupt. */
#define RX1_INTC_SET_EN            ((reg32 *) RX1__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the RX1 interrupt. */
#define RX1_INTC_CLR_EN            ((reg32 *) RX1__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the RX1 interrupt state to pending. */
#define RX1_INTC_SET_PD            ((reg32 *) RX1__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the RX1 interrupt. */
#define RX1_INTC_CLR_PD            ((reg32 *) RX1__INTC_CLR_PD_REG)


#endif /* CY_ISR_RX1_H */


/* [] END OF FILE */
