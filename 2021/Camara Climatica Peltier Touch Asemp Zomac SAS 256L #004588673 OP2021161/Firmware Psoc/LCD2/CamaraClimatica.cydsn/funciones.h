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
#if !defined(funciones_H)
#define funciones_H
#include "device.h"
#include <math.h>

/* Defines for ADC MUX channels */
#define LM35                     0
#define HUMEDAD                  1
#define NTC                      2
    
#define OFF 0
#define ON  1
#define false 0
#define true  1
#define LOW 0
#define HIGH  1

#define desvio                          0.5
#define desvioH                         2.0
#define aa                              -2.4560
#define bb                              19.801
#define cc                              -73.195
#define dd                              122.55;    
#define tiempoNivel                     3

typedef struct{
   uint8 TemperaturaDec;
   uint8 TemperaturaInt;
   uint8 Horas;
   uint8 Minutos;
   uint8 Humedad;
   float Temperatura;
}Personalizado;
Personalizado rampas[20];

typedef struct{
   float Temperatura;
   float Humedad;
   uint8 Hora;
   uint8 Minuto;
}Grafica;
Grafica graph[800];

typedef struct{
   float control1,control2,control3,m1,m2,m3,patron1,patron2,patron3,b1,b2,b3;
}Calibracion;
Calibracion calib[3];

extern uint16 pwm1,pwm2,data,data2,datacount,tDisplayHumedad;
extern int bytes;
extern char webbuff[2000];	// After processing the received data is cleared and used to send data to the client
extern uint8 socket;
extern int8 prueba[80];
extern uint8 horaGrafica[20],minutoGrafica[20];
extern short ip1,ip2,ip3,ip4,s1,s2,s3,s4,g1,g2,g3,g4,m1,m2,m3,m4,m5,m6,flagRed,entroGrafica,actualizaNumeracion,config,desconectoTemperatura,desconectoHumedad;
//---------Declaracion de Variables---------------------
extern int8 Regular[12];
extern int8 Bisiesto[12];

/*-------Variables Humedad-----------*/
extern short Humidificar,flagHumedad,humidificador,flagSobrepasoHumedad;
extern uint8 f,T1Humedad,T2Humedad,T3Humedad,T1,T2,T3,tDisplay;
extern uint16 tdehumidificador,thumidificador,tllenado,tlevel,tlevelH,tDefrost,tCongela,tSobrepaso;
extern float RH2,Tam,Humedad,humedadDisplay,minH,maxH;
extern float promedioH[80];
extern float promedioTa[80];
/*-------------------------------------*/

extern uint8 IP[4];
extern uint8 Gateway[4];
extern uint8 Subnet[4];
extern uint8 MAC[6];

extern int16 offset;
extern int16 coldJnTemp;
extern int32 tcColdJnuVolt;
extern int32 hotJnCount;
extern int32 filHotJnCount;
extern int32 tcHotJnuVolt;
extern int32 tcuVolt;
extern int32 tcTemp;
extern char displayStr[100];	
extern char Str[100];	
extern char Str2[100];	
extern uint8 first;  
extern uint8_t i,n,PantallaPrincipal,Programa,Opcion2,Derivativo,Integral,MediaMovil,MediaMovil2,Tciclo,Ajuste,Ajusten,Ajuste2,Ajusten2,Modulo,Ajusto;
extern uint16 ctr,SetpointH,SetpointHAnt,derivativo_ant,contador;
extern float Setpoint,SetpointAnt;
extern uint32_t output,tGrafica;
extern short flagButton0,flagButton1,flagButton2,flagButton3,dibujo,pulsoAumentar,pulsoDisminuir,flagBorrado,Control,estadoSSR,estadoBuzzer,estadoCR1,flagFinalizo;
extern short estadoCR2,estadoCR3,inicioControlT,inicioControlH;
extern float valor,pendiente,Ganancia,GananciaIn,vacio,pwmPeltier;

#define RX_BUFFER_SIZE  10
extern char Rx_Buffer[RX_BUFFER_SIZE+1];
extern char Rx_Buffer2[RX_BUFFER_SIZE+1];
extern uint8 RX_Wr_Index;
extern uint8 RX_Rd_Index;
extern uint8 RX_Counter;

extern char Rx_Buffer3[RX_BUFFER_SIZE+1];
extern char Rx_Buffer4[RX_BUFFER_SIZE+1];
extern uint8 RX_Wr_Index2;
extern uint8 RX_Rd_Index2;
extern uint8 RX_Counter2;

extern uint8 i2cbuf[4];

extern uint8 Dato2,Dato3,Dato_Exitoso,Dato_Exitoso2,Menu,MenuAntt,MenuAnt,Menu2,MenuAntt2,z,TipoClave,Segmentos,CicloLibre,Opcion,h,l;
extern uint8 Dia,Mes,Year,Hora,Minuto,Segundo,dow;
extern short Inicio,Inicio2,EstadoPaso,entro,GuardaEEPROM,rampa,Minutos,Horas,flagTemperatura,Test,x1,x10,x100,Succion,flagVacio,Enfria,flagVelocidad,envioUSB;
extern uint8 Password[4];
extern uint8 tiempo_esterilizacion,y,hEst,mEst,hEstp,mEstp,hEstpAnt,mEstpAnt,Segmento,conteo;
extern uint16 tiempos,tiempos3,tiempoctrl,Tc,Tp,tiemporeset,TiempoControl,tiemporecta,tiemposetpoint,tinicio,tiempocalentamiento,tiempoup,tiempodn;
extern short mensaje,Lectura2,flag_setpoint,flag_tiempocalentamiento,Lectura,flagup,flagdn,flagRampa,flag_calcula,flagusb,dehumidificador,flag_ajuste,flag_ajuste2;
extern short Finalizo,conteoPregunta,grafica,imprime,Infinito,flagControlT,flagControlH,Controlando,flag_clear2,flag_ganancia,flag_clear,Pausa;
extern float X[2];
extern float Temperatura,Temperatura2,Aumento,PendienteActual,DifPendiente,TempeInicial,Pendiente,TemperaturaIdeal,PotenciaEntregada,Tpulso,penInicial,penMufla;
extern float error,Potencia,TpulsoMax,TpulsoMax2,Tpulsomin,Tpulsomin2;
extern uint16 TiempoActual,MinutosActual,HorasActual,Tiempo,TiempoRampa,tPregunta;
extern uint8 dataReady,GananciaR;
extern uint16 result;
extern float Prom,Prom2,V0,V1,Temperature;
extern uint8 YearTx,MesTx,DiaTx,HoraTx,MinutoTx,dowTx;

extern int32 rtdRes[2];
extern int32 rtdTemp[2];
extern int32 vRef,filt;
extern uint8 i,l,h,entero;
extern uint8 ll,hh;
extern int32 decTemp[2];
extern uint16 decTemplf,decTemphf;
extern int32 fracTemp[2];
extern uint16 adcReading,presmV;
extern uint32 lm35,tmp;
extern float presion,Temporal,presionHorno,Tam;
extern uint8 i;
extern uint8 l2,h2,k,kk,tEnvioUSB,tmuestreoUSB,tmuestreoUSB_ant,controlH,controlT,controlR;
extern float promedio2[20];

extern float promedioT1[90];
extern float promedioT2[90];
extern float promediolm35[20];


//---------Declaracion de Funciones----------------------    
extern uint8 Password[4];
extern uint8 Derivativo,Integral,MediaMovil,Tciclo,Ajuste,Ajusten;
extern float Ganancia,GananciaIn;
extern uint16 result;
extern float Temperatura,Humedad,Tam,Temperatura2;
extern int32  Temperature2;

uint8 extraeDecimal(float data);
float extraeDecimal2(float data);
float inttofloat(uint8 decimal);
uint8 convert8(uint16 data,uint8 nible);
uint16 convert16(uint8 nibleh,uint8 niblel);
void LeeEEPROM();
void LimitaValores();
void ActualizaRecta();
void Controla();
void CuentaTiempo();
void iniciaConteo();
void escribeDisplay1(char Buffer[30]);
void escribeDisplay2(char Buffer[30]);
void LeeDisplay(void);
void controlaRampa();
float leePt100(uint8 media);
float leePt1002(uint8 media2);
int32 LowPassFilter(int32 input);
int32 leeLM35();
float leeHumedad(int8 media);
short esBisiesto(uint8 year);
void LimitaDia();
void ApagaSalidas();
void controlaHumedad();
void controlaHumedad2();
void controlaTemperatura();
void actualizaEjeX();
void borraVector();
void dataUSB();
void dataEthernet();
    
#endif    

/* [] END OF FILE */
