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
    
#define desvio                          0.5
    
#define m                               0.000625
#define b                               -0.25
    
/* Tiempo de muestreo de acorde a Timer 2, tiempo en milisegundos*/    
#define tmuestreo                       300.0
    
#define aa                              -2.4560
#define bb                              19.801
#define cc                              -73.195
#define dd                              122.55;    

//#define T1Humedad                       6
//#define T2Humedad                       40    
//#define T3Humedad                       2

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

extern uint8 Password[4];
extern uint8 Derivativo,Integral,MediaMovil,Tciclo,Ajuste,Ajusten,dataReady;
extern float Ganancia,GananciaIn;
extern uint16 result;

typedef struct{
   uint8 TemperaturaDec;
   uint8 TemperaturaInt;
   uint8 Horas;
   uint8 Minutos;
   uint8 Humedad;
   float Temperatura;
}Personalizado;
Personalizado rampas[20];

//typedef struct{
   //Personalizado rampas[20];
//}Program;
//Program Programas[5];

void leeEEPROM(void);

uint8 extraeDecimal(float data);

float inttofloat(uint8 decimal);

uint8 convert8(uint16 data,uint8 nible);
uint16 convert16(uint8 nibleh,uint8 niblel);

#endif

/* [] END OF FILE */

/* [] END OF FILE */
