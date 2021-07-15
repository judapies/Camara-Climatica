/*******************************************************************************
* File Name: CurrentMux.h
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

#if !defined(CY_AMUX_CurrentMux_H)
#define CY_AMUX_CurrentMux_H

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

void CurrentMux_Start(void) ;
#define CurrentMux_Init() CurrentMux_Start()
void CurrentMux_FastSelect(uint8 channel) ;
/* The Stop, Select, Connect, Disconnect and DisconnectAll functions are declared elsewhere */
/* void CurrentMux_Stop(void); */
/* void CurrentMux_Select(uint8 channel); */
/* void CurrentMux_Connect(uint8 channel); */
/* void CurrentMux_Disconnect(uint8 channel); */
/* void CurrentMux_DisconnectAll(void) */


/***************************************
*         Parameter Constants
***************************************/

#define CurrentMux_CHANNELS  3u
#define CurrentMux_MUXTYPE   1
#define CurrentMux_ATMOSTONE 0

/***************************************
*             API Constants
***************************************/

#define CurrentMux_NULL_CHANNEL 0xFFu
#define CurrentMux_MUX_SINGLE   1
#define CurrentMux_MUX_DIFF     2


/***************************************
*        Conditional Functions
***************************************/

#if CurrentMux_MUXTYPE == CurrentMux_MUX_SINGLE
# if !CurrentMux_ATMOSTONE
#  define CurrentMux_Connect(channel) CurrentMux_Set(channel)
# endif
# define CurrentMux_Disconnect(channel) CurrentMux_Unset(channel)
#else
# if !CurrentMux_ATMOSTONE
void CurrentMux_Connect(uint8 channel) ;
# endif
void CurrentMux_Disconnect(uint8 channel) ;
#endif

#if CurrentMux_ATMOSTONE
# define CurrentMux_Stop() CurrentMux_DisconnectAll()
# define CurrentMux_Select(channel) CurrentMux_FastSelect(channel)
void CurrentMux_DisconnectAll(void) ;
#else
# define CurrentMux_Stop() CurrentMux_Start()
void CurrentMux_Select(uint8 channel) ;
# define CurrentMux_DisconnectAll() CurrentMux_Start()
#endif

#endif /* CY_AMUX_CurrentMux_H */


/* [] END OF FILE */
