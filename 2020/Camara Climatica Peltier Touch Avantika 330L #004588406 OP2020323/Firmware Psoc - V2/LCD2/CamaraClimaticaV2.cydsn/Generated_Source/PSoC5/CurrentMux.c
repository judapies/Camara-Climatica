/*******************************************************************************
* File Name: CurrentMux.c
* Version 1.80
*
*  Description:
*    This file contains all functions required for the analog multiplexer
*    AMux User Module.
*
*   Note:
*
*******************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "CurrentMux.h"

static uint8 CurrentMux_lastChannel = CurrentMux_NULL_CHANNEL;


/*******************************************************************************
* Function Name: CurrentMux_Start
********************************************************************************
* Summary:
*  Disconnect all channels.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void CurrentMux_Start(void) 
{
    uint8 chan;

    for(chan = 0u; chan < CurrentMux_CHANNELS ; chan++)
    {
#if (CurrentMux_MUXTYPE == CurrentMux_MUX_SINGLE)
        CurrentMux_Unset(chan);
#else
        CurrentMux_CYAMUXSIDE_A_Unset(chan);
        CurrentMux_CYAMUXSIDE_B_Unset(chan);
#endif
    }

    CurrentMux_lastChannel = CurrentMux_NULL_CHANNEL;
}


#if (!CurrentMux_ATMOSTONE)
/*******************************************************************************
* Function Name: CurrentMux_Select
********************************************************************************
* Summary:
*  This functions first disconnects all channels then connects the given
*  channel.
*
* Parameters:
*  channel:  The channel to connect to the common terminal.
*
* Return:
*  void
*
*******************************************************************************/
void CurrentMux_Select(uint8 channel) 
{
    CurrentMux_DisconnectAll();        /* Disconnect all previous connections */
    CurrentMux_Connect(channel);       /* Make the given selection */
    CurrentMux_lastChannel = channel;  /* Update last channel */
}
#endif


/*******************************************************************************
* Function Name: CurrentMux_FastSelect
********************************************************************************
* Summary:
*  This function first disconnects the last connection made with FastSelect or
*  Select, then connects the given channel. The FastSelect function is similar
*  to the Select function, except it is faster since it only disconnects the
*  last channel selected rather than all channels.
*
* Parameters:
*  channel:  The channel to connect to the common terminal.
*
* Return:
*  void
*
*******************************************************************************/
void CurrentMux_FastSelect(uint8 channel) 
{
    /* Disconnect the last valid channel */
    if( CurrentMux_lastChannel != CurrentMux_NULL_CHANNEL)
    {
        CurrentMux_Disconnect(CurrentMux_lastChannel);
    }

    /* Make the new channel connection */
#if (CurrentMux_MUXTYPE == CurrentMux_MUX_SINGLE)
    CurrentMux_Set(channel);
#else
    CurrentMux_CYAMUXSIDE_A_Set(channel);
    CurrentMux_CYAMUXSIDE_B_Set(channel);
#endif


    CurrentMux_lastChannel = channel;   /* Update last channel */
}


#if (CurrentMux_MUXTYPE == CurrentMux_MUX_DIFF)
#if (!CurrentMux_ATMOSTONE)
/*******************************************************************************
* Function Name: CurrentMux_Connect
********************************************************************************
* Summary:
*  This function connects the given channel without affecting other connections.
*
* Parameters:
*  channel:  The channel to connect to the common terminal.
*
* Return:
*  void
*
*******************************************************************************/
void CurrentMux_Connect(uint8 channel) 
{
    CurrentMux_CYAMUXSIDE_A_Set(channel);
    CurrentMux_CYAMUXSIDE_B_Set(channel);
}
#endif

/*******************************************************************************
* Function Name: CurrentMux_Disconnect
********************************************************************************
* Summary:
*  This function disconnects the given channel from the common or output
*  terminal without affecting other connections.
*
* Parameters:
*  channel:  The channel to disconnect from the common terminal.
*
* Return:
*  void
*
*******************************************************************************/
void CurrentMux_Disconnect(uint8 channel) 
{
    CurrentMux_CYAMUXSIDE_A_Unset(channel);
    CurrentMux_CYAMUXSIDE_B_Unset(channel);
}
#endif

#if (CurrentMux_ATMOSTONE)
/*******************************************************************************
* Function Name: CurrentMux_DisconnectAll
********************************************************************************
* Summary:
*  This function disconnects all channels.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void CurrentMux_DisconnectAll(void) 
{
    if(CurrentMux_lastChannel != CurrentMux_NULL_CHANNEL) 
    {
        CurrentMux_Disconnect(CurrentMux_lastChannel);
        CurrentMux_lastChannel = CurrentMux_NULL_CHANNEL;
    }
}
#endif

/* [] END OF FILE */
