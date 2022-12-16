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
#ifndef MAIN_H
#define MAIN_H
#include "project.h"
#include <stdio.h>

/* Defines for ADC SAR MUX channels */
#define LM35_CHANNEL               0
#define VACIO_CHANNEL            		1    
     
/* Altura de cuadro en grafica*/
#define ALTURA                          44.0

/* Ancho de cuadro en grafica*/
#define ANCHO                           43.0    

/* Division para ajuste de valores en grafica ALTURA/200*/
#define DIVISION                        0.22
    
#define m                               0.000625
#define b                               -0.25
    
/* Tiempo de muestreo de acorde a Timer 2, tiempo en milisegundos*/    
#define tmuestreo                       300.0

#define tiempoLlenado                   300   
    
/* Function to initialize components */
void InitializeComponents(void);

/* Function to measure cold junction temperature */
int32 MeasureColdJnSensorTemp(void);

/* Funtion to measure IC temperature */
int32 MeasureICTemp(void);

/* Funtion to measure thermistor temperature */
int32 MeasureThermistorTemp(void);


/* Function prototype to measure offset voltage */
int32 MeasureOffset(void);

/*Interrupt handler function*/
CY_ISR_PROTO(switchISRhandler);

#endif

/* [] END OF FILE */

/* [] END OF FILE */
