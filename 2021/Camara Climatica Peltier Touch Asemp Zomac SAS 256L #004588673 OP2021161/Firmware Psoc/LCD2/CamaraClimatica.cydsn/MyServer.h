/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "device.h"
#include <stdio.h>
#include <math.h>

short LED_state(void );	

void set_param(char *params, char *p);

void display_param(char *buffer);

void web_form(char *buffer, int counter);

void webif_data(char socket, char *Inbuff);
/* [] END OF FILE */
