/*******************************************************************************
* File Name: ADC_SAR_Mux.h
* Version 1.80
*
*  Description:
*    This file contains the constants and function prototypes for the Analog
*    Multiplexer User Module AMux.
*
*   Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_AMUX_ADC_SAR_Mux_H)
#define CY_AMUX_ADC_SAR_Mux_H

#include "cyfitter.h"
#include "cyfitter_cfg.h"

#if ((CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC3) || \
         (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC4) || \
         (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC5))    
    #include "cytypes.h"
#else
    #include "syslib/cy_syslib.h"
#endif /* ((CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC3) */


/***************************************
*        Function Prototypes
***************************************/

void ADC_SAR_Mux_Start(void) ;
#define ADC_SAR_Mux_Init() ADC_SAR_Mux_Start()
void ADC_SAR_Mux_FastSelect(uint8 channel) ;
/* The Stop, Select, Connect, Disconnect and DisconnectAll functions are declared elsewhere */
/* void ADC_SAR_Mux_Stop(void); */
/* void ADC_SAR_Mux_Select(uint8 channel); */
/* void ADC_SAR_Mux_Connect(uint8 channel); */
/* void ADC_SAR_Mux_Disconnect(uint8 channel); */
/* void ADC_SAR_Mux_DisconnectAll(void) */


/***************************************
*         Parameter Constants
***************************************/

#define ADC_SAR_Mux_CHANNELS  3u
#define ADC_SAR_Mux_MUXTYPE   1
#define ADC_SAR_Mux_ATMOSTONE 0

/***************************************
*             API Constants
***************************************/

#define ADC_SAR_Mux_NULL_CHANNEL 0xFFu
#define ADC_SAR_Mux_MUX_SINGLE   1
#define ADC_SAR_Mux_MUX_DIFF     2


/***************************************
*        Conditional Functions
***************************************/

#if ADC_SAR_Mux_MUXTYPE == ADC_SAR_Mux_MUX_SINGLE
# if !ADC_SAR_Mux_ATMOSTONE
#  define ADC_SAR_Mux_Connect(channel) ADC_SAR_Mux_Set(channel)
# endif
# define ADC_SAR_Mux_Disconnect(channel) ADC_SAR_Mux_Unset(channel)
#else
# if !ADC_SAR_Mux_ATMOSTONE
void ADC_SAR_Mux_Connect(uint8 channel) ;
# endif
void ADC_SAR_Mux_Disconnect(uint8 channel) ;
#endif

#if ADC_SAR_Mux_ATMOSTONE
# define ADC_SAR_Mux_Stop() ADC_SAR_Mux_DisconnectAll()
# define ADC_SAR_Mux_Select(channel) ADC_SAR_Mux_FastSelect(channel)
void ADC_SAR_Mux_DisconnectAll(void) ;
#else
# define ADC_SAR_Mux_Stop() ADC_SAR_Mux_Start()
void ADC_SAR_Mux_Select(uint8 channel) ;
# define ADC_SAR_Mux_DisconnectAll() ADC_SAR_Mux_Start()
#endif

#endif /* CY_AMUX_ADC_SAR_Mux_H */


/* [] END OF FILE */
