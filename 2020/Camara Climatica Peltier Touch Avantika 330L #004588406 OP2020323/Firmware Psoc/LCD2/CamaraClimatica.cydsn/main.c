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
#include <BMP180.h>
#include "removeOffsetNoise.h"
#include "getResistance.h"
#include <stdio.h>
#include <main.h>
#include <math.h>
#include <stdlib.h>
#include <DS1302.h>

#if defined (__GNUC__)
    /* Add an explicit reference to the floating point printf library */
    /* to allow the usage of floating point conversion specifiers. */
    /* This is not linked in by default with the newlib-nano library. */
    asm (".global _printf_float");
#endif

#define OFF 0
#define ON  1
#define false 0
#define true  1
#define LOW 0
#define HIGH  1

int8 Regular[12]={0,3,3,6,1,4,6,2,5,0,3,5};
int8 Bisiesto[12]={0,3,4,0,2,5,0,3,6,1,4,6};

int16 offset;
int16 coldJnTemp;
int32 tcColdJnuVolt;
int32 hotJnCount;
int32 filHotJnCount;
int32 tcHotJnuVolt;
int32 tcuVolt;
int32 tcTemp;
char displayStr[100];	
uint8 first = 0;
extern uint8 const CYCODE LCD_customFonts[];  
uint8_t i,pwm1,pwm2,n,PantallaPrincipal,Programa,Opcion2,Derivativo,Integral,MediaMovil,MediaMovil2,Tciclo,Ajuste,Ajusten,Ajuste2,Ajusten2,Modulo,Ajusto;
uint16 ctr,SetpointH,SetpointHAnt,derivativo_ant;
float Setpoint,SetpointAnt;
uint32_t output,tGrafica;
short flagButton0,flagButton1,flagButton2,flagButton3,dibujo,pulsoAumentar,pulsoDisminuir,flagBorrado,Control,estadoSSR,estadoBuzzer,estadoCR1,flagFinalizo;
short estadoCR2,estadoCR3,inicioControlT,inicioControlH;
float valor,pendiente,Ganancia,GananciaIn,vacio,pwmPeltier;

#define RX_BUFFER_SIZE  10
char Rx_Buffer[RX_BUFFER_SIZE+1];
char Rx_Buffer2[RX_BUFFER_SIZE+1];
uint8 RX_Wr_Index=0;
uint8 RX_Rd_Index=0;
uint8 RX_Counter=0;

char Rx_Buffer3[RX_BUFFER_SIZE+1];
char Rx_Buffer4[RX_BUFFER_SIZE+1];
uint8 RX_Wr_Index2=0;
uint8 RX_Rd_Index2=0;
uint8 RX_Counter2=0;

uint8 i2cbuf[4];
uint8 iconoGrande[15]={75,76,77,78,79,27,45};
uint8 iconoPeque[15]={80,81,82,83,84,37,16};
char const texto[9][20]={"PROGRAMA 1","PROGRAMA 2","PROGRAMA 3","PROGRAMA 4","PROGRAMA 5","TEST","CLAVE"};

uint8 Dato2,Dato3,Dato_Exitoso,Dato_Exitoso2,Menu,MenuAntt,MenuAnt,Menu2,MenuAntt2,z,TipoClave,Segmentos,CicloLibre,Opcion,h,l;
uint8 Dia,Mes,Year,Hora,Minuto,Segundo,dow;
short Inicio,Inicio2,EstadoPaso,entro,GuardaEEPROM,rampa,Minutos,Horas,flagTemperatura,Test,x1,x10,x100,Succion,flagVacio,Enfria,flagVelocidad,envioUSB;
uint8 Password[4];
uint8 tiempo_esterilizacion,y,hEst,mEst,hEstp,mEstp,hEstpAnt,mEstpAnt,Segmento,conteo;
uint16 tiempos,tiempos3,tiempoctrl,Tc,Tp,tiemporeset,TiempoControl,tiemporecta,tiemposetpoint,tinicio,tiempocalentamiento,tiempoup,tiempodn;
short mensaje,Lectura2,flag_setpoint,flag_tiempocalentamiento,Lectura,flagup,flagdn,flagRampa,flag_calcula,flagusb,dehumidificador,flag_ajuste,flag_ajuste2;
short Finalizo,conteoPregunta,grafica,imprime,Infinito,flagControlT,flagControlH,Controlando,flag_clear2,flag_ganancia,flag_clear,Pausa;
float X[2]={0.0,0.0};
float Temperatura,Temperatura2,Aumento,PendienteActual,DifPendiente,TempeInicial,Pendiente,TemperaturaIdeal,PotenciaEntregada,Tpulso,penInicial,penMufla=3.0;
float error,Potencia,TpulsoMax,TpulsoMax2,Tpulsomin,Tpulsomin2;
uint16 TiempoActual,MinutosActual,HorasActual,Tiempo,TiempoRampa,tPregunta;
uint8 dataReady = 0u,GananciaR;
uint16 result;
float Prom,Prom2,V0,V1,Temperature;
uint8 YearTx=0,MesTx=0,DiaTx=0,HoraTx=0,MinutoTx=0,dowTx=0;

int32 rtdRes[2];
int32 rtdTemp[2];
int32 vRef,filt;
uint8 i,l,h,entero;
uint8 ll,hh;
int32 decTemp[2];
uint16 decTemplf,decTemphf;
int32 fracTemp[2];
uint16 adcReading,presmV;
uint32 lm35,tmp;
float presion,Temporal,presionHorno,Tam;
uint8 i;
uint8 l2,h2,k,kk,tEnvioUSB,tmuestreoUSB,controlH,controlT,controlR;
float promedio2[20];

float promedioT1[90];
float promedioT2[90];
float promediolm35[20];

/*-------Variables Humedad-----------*/
short Humidificar,flagHumedad,humidificador;
uint8 f,T1Humedad,T2Humedad,T3Humedad;
uint16 tdehumidificador,thumidificador,tllenado;
float RH2,Tam,Humedad;
float promedioH[80];
float promedioTa[80];
/*-------------------------------------*/

void LeeEEPROM(){
   mEstp=EEPROM_ReadByte(0);CyDelay(5);  
   hEstp=EEPROM_ReadByte(1);CyDelay(5);  
   
   entero=EEPROM_ReadByte(80);CyDelay(5);  
   Temporal=EEPROM_ReadByte(81);CyDelay(5);  
   Temporal=Temporal/10.0;
   Setpoint=entero+Temporal;  
   
   SetpointH=EEPROM_ReadByte(82);
   CyDelay(5);   

   Password[0]=EEPROM_ReadByte(10);CyDelay(5);  
   Password[1]=EEPROM_ReadByte(11);CyDelay(5);  
   Password[2]=EEPROM_ReadByte(12);CyDelay(5);  
   Password[3]=EEPROM_ReadByte(13);CyDelay(5);  
   
   Ganancia=EEPROM_ReadByte(30);CyDelay(5);  
   Derivativo=EEPROM_ReadByte(31);CyDelay(5);  
   Integral=EEPROM_ReadByte(32);CyDelay(5);  
   Tciclo=EEPROM_ReadByte(33);CyDelay(5);  
   Ajuste=EEPROM_ReadByte(34);CyDelay(5);  
   Ajusten=EEPROM_ReadByte(35);CyDelay(5);  
   MediaMovil=EEPROM_ReadByte(36);CyDelay(5);  
   Ajuste2=EEPROM_ReadByte(37);CyDelay(5);
   Ajusten2=EEPROM_ReadByte(38);CyDelay(5);
   MediaMovil2=EEPROM_ReadByte(39);CyDelay(5);  
   tmuestreoUSB=EEPROM_ReadByte(40);CyDelay(5);
   controlT=EEPROM_ReadByte(41);CyDelay(5);
   controlH=EEPROM_ReadByte(42);CyDelay(5);
   controlR=EEPROM_ReadByte(43);CyDelay(5);
   T1Humedad=EEPROM_ReadByte(44);CyDelay(5);
   T2Humedad=EEPROM_ReadByte(45);CyDelay(5);
   T3Humedad=EEPROM_ReadByte(46);CyDelay(5);
   //Coeficiente=EEPROM_ReadByte(37);CyDelay(5);  
   //Saturacion=EEPROM_ReadByte(38);CyDelay(5);  
   GananciaR=Ganancia;
   
   for(i=0;i<20;i++){//Lee valores almacenados de ciclos personalizados
      rampas[i].TemperaturaInt=EEPROM_ReadByte(100+(i*6)); 
      rampas[i].TemperaturaDec=EEPROM_ReadByte(101+(i*6)); 
      rampas[i].Humedad=EEPROM_ReadByte(102+(i*6)); 
      rampas[i].Horas=EEPROM_ReadByte(103+(i*6)); 
      rampas[i].Minutos=EEPROM_ReadByte(104+(i*6)); 
      
      rampas[i].Temperatura=rampas[i].TemperaturaInt+inttofloat(rampas[i].TemperaturaDec);
      if(rampas[i].Temperatura>60.0 || rampas[i].Temperatura<0.1)rampas[i].Temperatura=30.0;
      if(rampas[i].Humedad>95 || rampas[i].Humedad<10)rampas[i].Humedad=60.0;
      if(rampas[i].Horas>99)rampas[i].Horas=10;
      if(rampas[i].Minutos>59)rampas[i].Minutos=10;
   }
}

void LimitaValores(){
   if(mEstp>59){
      mEstp=10;EEPROM_WriteByte(10,0);CyDelay(20);
   }
   if(hEstp>99){
      hEstp=10;EEPROM_WriteByte(10,1);CyDelay(20);
   }
   if(Password[0]>9 || Password[0]<0){
      Password[0]=0;EEPROM_WriteByte(0,10);CyDelay(20);
   }
   if(Password[1]>9 || Password[1]<0){
      Password[1]=0;EEPROM_WriteByte(0,11);CyDelay(20);
   }
   if(Password[2]>9 || Password[2]<0){
      Password[2]=0;EEPROM_WriteByte(0,12);CyDelay(20);
   }
   if(Password[3]>9 || Password[3]<0){
      Password[3]=0;EEPROM_WriteByte(0,13);CyDelay(20);
   }
   if(Setpoint>60.0 || Setpoint<0.0){
      Setpoint=30.0;EEPROM_WriteByte(100,80);CyDelay(20);EEPROM_WriteByte(0,81);CyDelay(20);
   }
   if(SetpointH>95 || SetpointH<10){
      SetpointH=60.0;EEPROM_WriteByte(60,82);CyDelay(20);
   }
   if(Ganancia>20 || Ganancia<1){
      Ganancia=2;EEPROM_WriteByte(2,30);CyDelay(20);
   }
   if(Derivativo>80 || Derivativo<5){
      Derivativo=80;EEPROM_WriteByte(80,31);CyDelay(20);
   }
   if(Integral>4 || Integral<1){
      Integral=1;EEPROM_WriteByte(1,32);CyDelay(20);
   }
   if(Tciclo>20 || Tciclo<5){
      Tciclo=5;EEPROM_WriteByte(5,33);CyDelay(20);
   }
   if(Ajuste>100){
      Ajuste=0;EEPROM_WriteByte(0,34);CyDelay(20);
   }
   if(Ajusten>100){
      Ajusten=0;EEPROM_WriteByte(0,35);CyDelay(20);
   }
   if(MediaMovil>80 || MediaMovil<10){
      MediaMovil=80;EEPROM_WriteByte(80,36);CyDelay(20);
   }
   if(Ajuste2>100){
      Ajuste2=0;EEPROM_WriteByte(0,37);CyDelay(20);
   }
   if(Ajusten2>100){
      Ajusten2=0;EEPROM_WriteByte(0,38);CyDelay(20);
   }
   if(MediaMovil2>80 || MediaMovil2<10){
      MediaMovil2=40;EEPROM_WriteByte(40,39);CyDelay(20);
   }
   if(controlT>1){
      controlT=0;EEPROM_WriteByte(0,41);CyDelay(20);
   }
   if(controlH>1){
      controlH=0;EEPROM_WriteByte(0,42);CyDelay(20);
   }
   if(controlR>1){
      controlR=0;EEPROM_WriteByte(0,43);CyDelay(20);
   }
   if(T1Humedad>60 || T1Humedad<4){
      T1Humedad=10;EEPROM_WriteByte(10,44);CyDelay(20);
   }
   if(T2Humedad>60 || T2Humedad<4){
      T2Humedad=30;EEPROM_WriteByte(30,45);CyDelay(20);
   }
   if(T3Humedad>60 || T3Humedad<4){
      T3Humedad=6;EEPROM_WriteByte(6,46);CyDelay(20);
   }
}

void ActualizaRecta(){
   X[y]=Temperatura;
   y++;
   if(y>=2){
      y=0;
      Aumento=X[1]-X[0];
      HorasActual=hEst;
      MinutosActual=mEst;
      TiempoActual=Tiempo-((HorasActual*3600)+(MinutosActual*60)+tinicio);
      PendienteActual=((Setpoint-Temperatura)/TiempoActual);
      DifPendiente=(PendienteActual*20)-Aumento;
      DifPendiente=DifPendiente*5;
   }
}

void Controla(){
//-------------------------------Control de Calentamiento------------------------------------------------------------//         
   float tmp=0.0;
     if(!flagRampa){
        if(tiemporecta>=Derivativo){
            ActualizaRecta();
            tiemporecta=0;
        } 
    }else{
        if(tiemporecta>=10){
            ActualizaRecta();
            tiemporecta=0;
        }
    }
    
    CR6_Write(0); // activa Ventiladores de Celda de Peltier
    
    if(flagRampa){
      if(Segmento==0){
         if(flag_calcula==0){
            Tiempo=(rampas[0].Horas*3600)+(rampas[0].Minutos*60);         
            Setpoint=rampas[0].Temperatura;
            SetpointH=rampas[Segmento].Humedad;
            Pendiente=(((float)Setpoint-TempeInicial)/(float)Tiempo);
            hEstp=rampas[0].Horas;
            mEstp=rampas[0].Minutos;
            flag_calcula=1;
            Ganancia=GananciaR*(Setpoint/200);
         }
      }else{
         if(flag_calcula==0){
            Tiempo=(rampas[Segmento].Horas*3600)+(rampas[Segmento].Minutos*60);         
            Setpoint=rampas[Segmento].Temperatura;
            SetpointH=rampas[Segmento].Humedad;
            Pendiente=(((float)rampas[Segmento].Temperatura-(float)rampas[Segmento-1].Temperatura)/(float)Tiempo);
            hEstp=rampas[Segmento].Horas;
            mEstp=rampas[Segmento].Minutos;
            flag_calcula=1;
            Ganancia=GananciaR*(Setpoint/200);
         }
      }
    }
   
  
   if(Pendiente!=0 && flagRampa){// Si Esta realizando una Rampa debe realizar el control de acuerdo a la pendiente de Temperatura. Aprox 10°C * min Maximo Valor de Pendiente.
      
      TiempoRampa=((hEst*3600)+(mEst*60)+tinicio);
      if(Segmento==0){
         TemperaturaIdeal=(Pendiente*TiempoRampa)+TempeInicial;
      }else{
         TemperaturaIdeal=(Pendiente*TiempoRampa)+rampas[Segmento-1].Temperatura;
      }
      PotenciaEntregada=((TemperaturaIdeal-Temperatura)*100)/penMufla;
// Pendiente determinar si el comportamiento de la mufla es lineal 10°C/min o si tiene un comportamiento diferente.         
      if(Pendiente<0){
         if(PotenciaEntregada<0 && PotenciaEntregada>-100.0){
            if(PotenciaEntregada<0)
                PotenciaEntregada=PotenciaEntregada*-1;
         }else if(PotenciaEntregada<-100.0){
            PotenciaEntregada=100.0;
         }else if(PotenciaEntregada>0){
            PotenciaEntregada=0; 
         }
      }else{
         if(PotenciaEntregada>100.0)
            PotenciaEntregada=100.0;
     
         if(PotenciaEntregada<0)
            PotenciaEntregada=0; 
      }
      
      if(Enfria==1){
         CR1_Write(0);
         CR2_Write(0);
         CR3_Write(0);
         CR4_Write(0);
      }else{
         CR1_Write(1);
         CR2_Write(1);
         CR3_Write(1);
         CR4_Write(1);
      }
      Tpulso=(PotenciaEntregada*Tciclo)/100;  
   }else{
               
      if(Enfria==1){
         CR1_Write(0);
         CR2_Write(0);
         CR3_Write(0);
         CR4_Write(0);
         error=Setpoint-desvio-Temperatura;      // Cálculo del error ******
         
         if(error<-0.4 && Aumento>-0.2){
            if(Potencia<90.0){
               if(TiempoControl>=Derivativo*2){
                  TiempoControl=0;
                  if(Aumento>0.2){
                     Ganancia+=Integral;
                  }else{
                     if(Aumento>0.0){
                        Ganancia+=Integral-0.4;
                     }else{
                        Ganancia+=Integral-0.7;
                     }
                  }
               }
            }
         }
      
         if(error>0.0)     // Anti Wind_DOWN
            error=0.0;
         
         error=error*-1.0;
         
         if(Ganancia<0.1)      
            Ganancia=0.1;
      
         tmp=error*Ganancia;  // Control Proporcional.
               
         if(tmp>Setpoint)   // Anti Wind-UP      
            tmp=Setpoint;
                     
         if(tmp<0.0)     // Anti Wind_DOWN    
            tmp=0.0;
         
         // Tpulso(t)= Tciclo*(y(t)-ymin)/(ymax - ymin); calculo de ciclo util para control de resistencia.ymax=140.ymin=0;
         // Tpulso es float, hay que pasar ese ciclo a una salida de un puerto usando el TMR1.
         Tpulso=(tmp/Setpoint)*Tciclo;  
         
         Tp=(Tpulso/0.005);
         Tc=(Tciclo/0.005);
         Controlando=1;       
      }else{
         CR1_Write(1);
         CR2_Write(1);
         CR3_Write(1);
         CR4_Write(1);
         error=desvio+Setpoint-Temperatura;      // Cálculo del error ******
      
         if(error>=20.0 && Aumento<=0.7){
            derivativo_ant=Derivativo*4;
            flag_ajuste=0;
            flag_ajuste2=0;
            Ajusto=10;
            if(Potencia<90.0){
                if(TiempoControl>=derivativo_ant){
                    TiempoControl=0;
                    if(Aumento<-0.5){
                        Ganancia+=Integral;//2
                    }else{
                        if(Aumento<-0.1)
                            Ganancia+=Integral*0.5;//1
                        else
                            Ganancia+=Integral*0.3;//0.5
                    }
                }
            }
         }else if(error>=6.0 && error<20 && Aumento<=0.5){
            derivativo_ant=Derivativo*3;
            flag_ajuste=0;
            flag_ajuste2=0;
            Ajusto=10;
            if(Potencia<70.0){
                if(TiempoControl>=derivativo_ant){//80
                    TiempoControl=0;
                    if(Aumento<-0.5){
                        Ganancia+=Integral;//2
                    }else{
                        if(Aumento<-0.1)
                            Ganancia+=Integral*0.3;//1
                        else
                            Ganancia+=Integral*0.1;//0.5
                    }
                }
            }
         }else if(error>=desvio+0.5 && error<10 && Aumento<=0.3){
            derivativo_ant=Derivativo*2;
            flag_ajuste=0;
            flag_ajuste2=0;
            Ajusto=10;
            if(Potencia<50.0){
                if(TiempoControl>=derivativo_ant){//80
                    TiempoControl=0;
                    if(Aumento<-0.5){
                        Ganancia+=Integral;//2
                    }else{
                        if(Aumento<-0.1)
                            Ganancia+=Integral*0.3;//1
                        else
                            Ganancia+=Integral*0.1;//0.5
                    }
                }
             }
         }
         
         if(error<0.0){
            Ganancia=GananciaR*(Setpoint/200);
            //flag_ajuste=0;
            //flag_ajuste2=0;
         }
         
         if(Ganancia<0.1){
            Ganancia=0.1;
         }
      
         if(error<0.0)     // Anti Wind_DOWN     
            error=0.0;
         
         //tmp=(Setpoint-Prom)*Ganancia;  // Control Proporcional.
         tmp=error*Ganancia;  // Control Proporcional.
               
         if(tmp>Setpoint)   // Anti Wind-UP      
            tmp=Setpoint;
                     
         if(tmp<0.0)     // Anti Wind_DOWN    
            tmp=0.0;
      
         Tpulso=(tmp/Setpoint)*Tciclo; 
         if(error<desvio+0.5 && error>0){
            if(error<=desvio+0.1 && error>0){   
                if(flag_ajuste==0){
                    TpulsoMax=Tpulso;
                    flag_ajuste=1;  
                    Ajusto=5;
                }
            }
         
            if(error<=desvio-0.4 && error>0){   
                if(flag_ajuste2==0){
                    Tpulsomin=Tpulso;
                    flag_ajuste2=1;           
                    Ajusto=5;
                }
            }
         
            if(flag_ajuste2==1){
                if(Tpulso<Tpulsomin)
                    Tpulso=Tpulsomin;
            }
            if(flag_ajuste==1){   
                if(Tpulso>TpulsoMax)
                    Tpulso=TpulsoMax;
            }
         }
      }   
   }
   Tp=(Tpulso/0.002);
   Tc=(Tciclo/0.002);
   Control=1;     
//--------------------------------------------------------------------------------------------------------------------//   
  
}

void CuentaTiempo(){
      if(tinicio>=60){
         tinicio=0;
         mEst+=1.0;
         if(mEst>59){
            mEst=0;
            hEst+=1.0;
         }
      }
}

void iniciaConteo(){
   if(flagRampa && Pendiente!=0){
      tiempo_esterilizacion=1;
   }else if(flagRampa && Pendiente==0){
      if(flagTemperatura)
         tiempo_esterilizacion=1;
      else
         tiempo_esterilizacion=0;
   }else{
      if(!flagControlT && !flagControlH){
         tiempo_esterilizacion=0;
       }else if(flagControlT && !flagControlH){
          if(flagTemperatura)
             tiempo_esterilizacion=1;
          else
             tiempo_esterilizacion=0;
       }else if(!flagControlT && flagControlH){
         if(flagHumedad)
            tiempo_esterilizacion=1;
         else
            tiempo_esterilizacion=0;
      }else if(flagControlT && flagControlH){
         if(flagTemperatura && flagHumedad)
            tiempo_esterilizacion=1;
         else
            tiempo_esterilizacion=0;
      }
   }
}

void escribeDisplay1(char Buffer[30]){
    UART_1_PutString(Buffer);
    CyDelayUs(10);
    UART_1_PutChar(0xFF);
    CyDelayUs(10);
    UART_1_PutChar(0xFF);
    CyDelayUs(10);
    UART_1_PutChar(0xFF);
    CyDelayUs(10);
}

void LeeDisplay(void){    
    if(Dato_Exitoso==5){
         Rx_Buffer[0]=Rx_Buffer2[0];
         Rx_Buffer[1]=Rx_Buffer2[1];
         Rx_Buffer[2]=Rx_Buffer2[2];
         Rx_Buffer[3]=Rx_Buffer2[3];
         Rx_Buffer[4]=Rx_Buffer2[4];
         Rx_Buffer[5]=Rx_Buffer2[5];
         Rx_Buffer[6]=Rx_Buffer2[6];
         Rx_Buffer[7]=Rx_Buffer2[7];
         Rx_Buffer[8]=Rx_Buffer2[8];
         Rx_Buffer[9]=Rx_Buffer2[9];
         
      if(Rx_Buffer[3]==0x01){// Pregunta por la pagina en la que esta el display,01 es Contraseña de Acceso
         Menu=0;
         if(MenuAntt!=Menu)
            MenuAntt=Menu;
      }else if(Rx_Buffer[3]==0x02){//02 es Menu Principal con PantallaPriincipal=0, y con PantallaPrincipal=1 es Ciclo Libre
         Menu=1;
         if(MenuAntt!=Menu)
            MenuAntt=Menu;
      }else if(Rx_Buffer[3]==0x04){//03 es Tiempo Esterilizacion
         Menu=2;
         if(MenuAntt!=Menu)
            MenuAntt=Menu;
      }else if(Rx_Buffer[3]==0x05){//05 es Temperatura
         Menu=3;
         if(MenuAntt!=Menu)
            MenuAntt=Menu;
      }else if(Rx_Buffer[3]==0x06){//06 es Humedad
         Menu=4;
         if(MenuAntt!=Menu)
            MenuAntt=Menu;
      }else if(Rx_Buffer[3]==0x07){//07 es Test de Componentes
         Menu=6;
         if(MenuAntt!=Menu)
            MenuAntt=Menu;
      }else if(Rx_Buffer[3]==0x08){//08 es Fecha y hora
         Menu=7;
         if(MenuAntt!=Menu)
            MenuAntt=Menu;
      }else if(Rx_Buffer[3]==0x09){//09 es Rampas
         Menu=8;
         if(MenuAntt!=Menu)
            MenuAntt=Menu;
      }else if(Rx_Buffer[3]==0x0a){//0a es Pulsos de Vacio
         Menu=9;
         if(MenuAntt!=Menu)
            MenuAntt=Menu;
      }else if(Rx_Buffer[3]==0x0f){//0f es Recibe caracteres de contraseña desde display
         
      }else if(Rx_Buffer[3]==0x1a){//1a es Menu de Funcionamiento
         Menu=20;
         if(MenuAntt!=Menu)
            MenuAntt=Menu;
      }else if(Rx_Buffer[3]==0x1b){//1b es Menu de clave correcta
         Menu=15;
         if(MenuAntt!=Menu)
            MenuAntt=Menu;
      }else if(Rx_Buffer[3]==0x1c){//1c es Menu de clave incorrecta
         Menu=16;
         if(MenuAntt!=Menu)
            MenuAntt=Menu;
      }else if(Rx_Buffer[3]==0x1d){//1d es Menu de Eliminado Programa
         Menu=17;
         if(MenuAntt!=Menu)
            MenuAntt=Menu;
      }else if(Rx_Buffer[3]==0x1e){//1d es Menu de Pregunta
         Menu=18;
         if(MenuAntt!=Menu)
            MenuAntt=Menu;
      }else if(Rx_Buffer[3]==0x2c){//2c es Menu de Configuracion de Parametros
         Menu=100;
         if(MenuAntt!=Menu)
            MenuAntt=Menu;
      }else if(Rx_Buffer[3]==0xcc){//cc es Menu de Bienvenida
        if(controlT==0 && controlH==0 && controlR==0)
            Menu=120;
        else
            Menu=20;
         //if(MenuAntt!=120)
            //reset_cpu();
      }
   }else{
      for(z=0;z<RX_BUFFER_SIZE;z++){
            //Rx_Buffer[z]=0;
            //Rx_Buffer2[z]=0;
         }
   }

       if(Dato_Exitoso2==5){
         Rx_Buffer3[0]=Rx_Buffer4[0];
         Rx_Buffer3[1]=Rx_Buffer4[1];
         Rx_Buffer3[2]=Rx_Buffer4[2];
         Rx_Buffer3[3]=Rx_Buffer4[3];
         Rx_Buffer3[4]=Rx_Buffer4[4];
         Rx_Buffer3[5]=Rx_Buffer4[5];
         Rx_Buffer3[6]=Rx_Buffer4[6];
         Rx_Buffer3[7]=Rx_Buffer4[7];
         Rx_Buffer3[8]=Rx_Buffer4[8];
         Rx_Buffer3[9]=Rx_Buffer4[9];
         
      if(Rx_Buffer3[3]==0x01){// Pregunta por la pagina en la que esta el display,01 es Contraseña de Acceso
         Menu2=0;
         if(MenuAntt2!=Menu2)
            MenuAntt2=Menu2;
      }else if(Rx_Buffer3[3]==0x02){//02 es Menu Principal con PantallaPriincipal=0, y con PantallaPrincipal=1 es Ciclo Libre
         Menu2=1;
         if(MenuAntt2!=Menu2)
            MenuAntt2=Menu2;
      }else if(Rx_Buffer3[3]==0x04){//03 es Tiempo Esterilizacion
         Menu2=2;
         if(MenuAntt2!=Menu2)
            MenuAntt2=Menu2;
      }else if(Rx_Buffer3[3]==0x05){//05 es Temperatura
         Menu2=3;
         if(MenuAntt2!=Menu2)
            MenuAntt2=Menu2;
      }else if(Rx_Buffer3[3]==0x06){//06 es Nivel
         Menu2=5;
         if(MenuAntt2!=Menu2)
            MenuAntt2=Menu2;
      }else if(Rx_Buffer3[3]==0x07){//07 es Test de Componentes
         Menu2=6;
         if(MenuAntt2!=Menu2)
            MenuAntt2=Menu2;
      }else if(Rx_Buffer3[3]==0x08){//08 es Fecha y hora
         Menu2=7;
         if(MenuAntt2!=Menu2)
            MenuAntt2=Menu2;
      }else if(Rx_Buffer3[3]==0x09){//09 es Rampas
         Menu2=8;
         if(MenuAntt2!=Menu2)
            MenuAntt2=Menu2;
      }else if(Rx_Buffer3[3]==0x0a){//0a es Grafica
         Menu2=9;
         if(MenuAntt2!=Menu2)
            MenuAntt2=Menu2;
      }else if(Rx_Buffer3[3]==0x0f){//0f es Recibe caracteres de contraseña desde display
         
      }else if(Rx_Buffer3[3]==0x1a){//1a es Menu de Funcionamiento
         Menu2=20;
         if(MenuAntt2!=Menu2)
            MenuAntt2=Menu2;
      }else if(Rx_Buffer3[3]==0x1b){//1b es Menu de clave correcta
         Menu2=15;
         if(MenuAntt2!=Menu2)
            MenuAntt2=Menu2;
      }else if(Rx_Buffer3[3]==0x1c){//1c es Menu de clave incorrecta
         Menu2=16;
         if(MenuAntt2!=Menu2)
            MenuAntt2=Menu2;
      }else if(Rx_Buffer3[3]==0x2c){//2c es Menu de Configuracion de Parametros
         Menu2=100;
         if(MenuAntt2!=Menu2)
            MenuAntt2=Menu2;
      }else if(Rx_Buffer3[3]==0xcc){//cc es Menu de Bienvenida
         Menu2=120;
         //if(MenuAntt!=120)
            //reset_cpu();
      }
   }else{
         for(z=0;z<RX_BUFFER_SIZE;z++){
            //Rx_Buffer[z]=0;
            //Rx_Buffer2[z]=0;
         }
   }
}

void controlaRampa(){
      if(Enfria==1){
      if(Temperatura<=Setpoint+0.5 && Temperatura>=Setpoint-0.5){
         if(Pendiente!=0){
            escribeDisplay1("statusT.txt=\"Controlando\"");
            flagTemperatura=ON;            
         }
         
         if(Pendiente==0){
            escribeDisplay1("statusT.txt=\"Meseta\"");
            flagTemperatura=ON;
         }
         Controla();
         flag_tiempocalentamiento=0;
      }else if(Temperatura<Setpoint-0.5){
         Enfria=0;
         Ganancia=GananciaR*(Setpoint/200);
         //Gananciamax=(0.21)*Setpoint;
         SSR_Write(0);
         Control=0;
      }else if(Temperatura>Setpoint+0.6){
         if(Pendiente!=0){
            escribeDisplay1("statusT.txt=\"Rampa -\"");
            flagTemperatura=ON;            
         }
         if(Pendiente==0){
            escribeDisplay1("statusT.txt=\"Enfriando\"");
            flagTemperatura=OFF;            
         }
         Controla();
      }
   }else{  
      if(Temperatura>=Setpoint-0.5 && Temperatura<=Setpoint+0.5){
         if(Pendiente!=0){
            escribeDisplay1("statusT.txt=\"Controlando\"");
            flagTemperatura=ON;            
         }
         
         if(Pendiente==0){
            escribeDisplay1("statusT.txt=\"Meseta\"");
            flagTemperatura=ON;
         }
         Controla();
         flag_tiempocalentamiento=0;
      }else if(Temperatura>Setpoint+0.5){
         Enfria=1;
         Ganancia=GananciaR*(Setpoint/200);
         SSR_Write(0);
         Control=0;
      }else if(Temperatura<Setpoint-0.6){
         if(Pendiente!=0){
            escribeDisplay1("statusT.txt=\"Rampa +\"");
            flagTemperatura=ON;            
         }
         if(Pendiente==0){
            escribeDisplay1("statusT.txt=\"Calentando\"");
            flagTemperatura=OFF;            
         }
         Controla();
      }
   }
}

float leePt100(uint8 media){
    float tmp2,ajp,ajn;
    Prom=0.0;    
    tmp2=0;
    
    /* Measure the voltage across reference resistance */
    vRef = MeasureRefResVoltage();
	/* Find the RTD resistance */
	rtdRes[0] = GetRTDRes(vRef,0);
	/* Calculate RTD temperature from resistance */
	rtdTemp[0] = RTD_GetTemperature(rtdRes[0]);
    /*Determine decimal portion of temperature by dividing temperature by 100*/
    decTemp[0] = rtdTemp[0]/100;
    /*Determine fraction portion of temperature by subtracting decimal portion from result*/
    fracTemp[0] = rtdTemp[0] - (decTemp[0] * 100);
    /*If fractional temperature is negative make it positive*/
    if(fracTemp[0] < 0){
        fracTemp[0] *= -1;
    }
    tmp2=(float)fracTemp[0];    
    V0=(float)decTemp[0]+(tmp2/100.0);
    
    if(l>(media-1))
         l=0;
    
    promedioT1[l]=V0;l++;
             
    for(h=0;h<media;h++){
        Prom+=promedioT1[h];
    } 
    
    ajp=(float)Ajuste;
    ajp=ajp/10.0;
    ajn=(float)Ajusten;
    ajn=ajn/10.0;
    return (Prom/media)+ajp-ajn;   
}

float leePt1002(uint8 media2){
    float tmp3,ajp2,ajn2;
    Prom2=0.0;
    tmp3=0;
    
    /* Measure the voltage across reference resistance */
    vRef = MeasureRefResVoltage();
	/* Find the RTD resistance */
	rtdRes[1] = GetRTDRes(vRef,1);
	/* Calculate RTD temperature from resistance */
	rtdTemp[1] = RTD_GetTemperature(rtdRes[1]);
    /*Determine decimal portion of temperature by dividing temperature by 100*/
    decTemp[1] = rtdTemp[1]/100;
    /*Determine fraction portion of temperature by subtracting decimal portion from result*/
    fracTemp[1] = rtdTemp[1] - (decTemp[1] * 100);
    /*If fractional temperature is negative make it positive*/
    if(fracTemp[1] < 0){
        fracTemp[1] *= -1;
    }
    tmp3=(float)fracTemp[1];    
    V1=(float)decTemp[1]+(tmp3/100.0);
    
    if(ll>(media2-1))
        ll=0;

    promedioT2[ll]=V1;ll++;
             
    for(hh=0;hh<media2;hh++){
        Prom2+=promedioT2[hh];
    }
    ajp2=(float)Ajuste2;
    ajp2=ajp2/10.0;
    ajn2=(float)Ajusten2;
    ajn2=ajn2/10.0;
    return (Prom2/media2)+ajp2-ajn2;
}

int32 LowPassFilter(int32 input){
    int32 k;
    input <<= 8;
    filt = filt + (((input-filt) >> 8) * 40);
    k = (filt>>8) + ((filt & 0x00000080) >> 7);
    return k;
}

int32 leeLM35(){ //Falta hacerle promedio
    float Promlm35;
    Promlm35=0.0;
    ADC_SAR_IsEndConversion(ADC_SAR_WAIT_FOR_RESULT);
    tmp=ADC_SAR_GetResult16(0u);
    lm35=LowPassFilter(ADC_SAR_CountsTo_mVolts(tmp));
    if(k>(4))
      k=0;
    promediolm35[k]=lm35;k++;
         
    for(kk=0;kk<5;kk++)
      Promlm35+=promediolm35[kk]; 
    Promlm35=Promlm35/5;
    return Promlm35*10;
}

float leeHumedad(int8 media){
   float VH=0.0,RH=0.0,Ta=0.0,VT=0.0;
   float PromH,PromTa;
   PromH=0.0;
   PromTa=0.0;
   ADC_SAR_IsEndConversion(ADC_SAR_WAIT_FOR_RESULT);
   //VH=ADC_SAR_CountsTo_Volts(LowPassFilter(tmp));
   VH=ADC_SAR_CountsTo_Volts(ADC_SAR_GetResult16(1u));
   VT=ADC_SAR_CountsTo_Volts(ADC_SAR_GetResult16(2u));
   VH=VH*1000;
   //VT=VT*1000;
   Ta=(pow(VT,3)*aa)+(pow(VT,2)*bb)+(VT*cc)+dd;

   if(f>(media-1))
      f=0;
   promedioTa[f]=Ta;f++;
         
   for(h=0;h<media;h++)
      PromTa+=promedioTa[h]; 
   PromTa=PromTa/media;
   
   Tam=PromTa;
   RH =((-0.0000000019206*pow(VH,3))+( 0.000014370*pow(VH,2))+( 0.0034210*VH)-12.4)/(1+(PromTa-23)*( 0.0024));
   if(RH<0)
        RH=0.0;
   if(RH>100.0)
        RH=100.0;
   //RH =((-0.0000000019206*pow(VH,3))+( 0.000014370*pow(VH,2))+( 0.0034210*VH)-12.4)/(1+(Temperatura-23)*( 0.0024));
   RH2 = 0.0375 * VH - 37.7; //Lineal sin compensacion de Temperatura
   if(n>(media-1))
      n=0;
   promedioH[n]=RH;n++;
         
   for(h=0;h<media;h++)
      PromH+=promedioH[h]; 
   PromH=PromH/media;
   return PromH;
}

short esBisiesto(uint8 year) {
     return ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0);
}

void LimitaDia(){
   if(MesTx>12)
      MesTx=10;
   if(DiaTx>31)
      DiaTx=16;
   if(YearTx>99)
      YearTx=18;   
   if(HoraTx>24)
      HoraTx=11;      
   if(MinutoTx>59)
      MinutoTx=30;      
   
      if(MesTx==2){
         if(esBisiesto(YearTx)){
            if(DiaTx>29){
               DiaTx=29;
               //glcd_fillScreen(OFF);
            }
         }else{
            if(DiaTx>28){
               DiaTx=28;
               //glcd_fillScreen(OFF);
            }
         }
      }else{
         if(MesTx<=7){
            if(MesTx % 2 ==0){
               if(DiaTx>30){
                  DiaTx=30; 
                  //glcd_fillScreen(OFF);
               }
            }
         }else{
            if(MesTx % 2 ==1){
               if(DiaTx>30){
                  DiaTx=30; 
                  //glcd_fillScreen(OFF);
               }
            }
         }
      }
}

void ApagaSalidas(){
    BUZZER_Write(0);
    CR1_Write(0);
    CR2_Write(0);
    CR3_Write(0);
    CR4_Write(0);
    CR5_Write(0);
    CR6_Write(1);
    CR7_Write(1);
    CR8_Write(0);
    SSR_Write(0);
}

void controlaHumedad(){
   if(Humidificar){
      dehumidificador=OFF;
      tdehumidificador=0;
      if(Humedad<SetpointH-3.0){
         escribeDisplay1("statusH.txt=\"Humidificando\"");
         humidificador=ON;
         CR5_Write(0);//Dehumidificador_off;
         if(LEVEL_Read()==0){
            tllenado=0;
            if(Humedad<SetpointH-10.0){
                CR7_Write(0);//Humidificador_on;
            }else{
                if(thumidificador<=T1Humedad){
                    CR7_Write(0);//Humidificador_on;
                }else if(thumidificador>T1Humedad && thumidificador<T2Humedad){
                    CR7_Write(1);//Humidificador_off;    
                }else if(thumidificador>T2Humedad){
                    thumidificador=0;   
                }
            }
         }else{
            CR7_Write(1);//Humidificador_off;
        }
        if(Humedad<SetpointH-5.0)
            flagHumedad=OFF;
      }else if(Humedad<SetpointH-0.3 && Humedad>SetpointH-2.8){
         escribeDisplay1("statusH.txt=\"Controlando\"");
         humidificador=ON;
         CR5_Write(0);//Dehumidificador_off;
         if(LEVEL_Read()==0){
            tllenado=0;
            //CR7_Write(0);//Humidificador_on;
            if(thumidificador<=T3Humedad){
                CR7_Write(0);//Humidificador_on;
            }else if(thumidificador>T3Humedad && thumidificador<T2Humedad){
                CR7_Write(1);//Humidificador_off;    
            }else if(thumidificador>T2Humedad){
                thumidificador=0;   
            }
         }else{
            CR7_Write(1);//Humidificador_off;
         }
         if(!Infinito)   
            flagHumedad=ON;
         else
            flagHumedad=OFF;
      }else if(Humedad>SetpointH && Humedad<SetpointH+3.0){
         CR7_Write(1);//Humidificador_off;
         CR5_Write(0);//Dehumidificador_off;
         humidificador=OFF;
         thumidificador=0;
         if(!Infinito)   
            flagHumedad=ON;
         else
            flagHumedad=OFF;
      }else if(Humedad>SetpointH+3.0){
         Humidificar=OFF; 
      }
   }else{
        humidificador=OFF;
        thumidificador=0;
      if(Humedad>SetpointH+2.0){
         escribeDisplay1("statusH.txt=\"Deshumidificando\"");
         dehumidificador=ON;
         if(tdehumidificador<300){
            CR5_Write(1);//Dehumidificador_on;
         }else if(tdehumidificador>=300 && tdehumidificador<360){
            CR5_Write(0);//Dehumidificador_off;
         }else if(tdehumidificador>=360){
            tdehumidificador=0;
         }
         CR7_Write(1);//Humidificador_off;
         if(Humedad>SetpointH+5.0)
            flagHumedad=OFF;
      }else if(Humedad>SetpointH && Humedad<SetpointH+2.8){
         escribeDisplay1("statusH.txt=\"Controlando\"");
         if(tdehumidificador<300){
            CR5_Write(1);//Dehumidificador_on;
         }else if(tdehumidificador>=300 && tdehumidificador<360){
            CR5_Write(0);//Dehumidificador_off;
         }else if(tdehumidificador>=360){
            tdehumidificador=0;
         }
         CR7_Write(1);//Humidificador_off;
         if(!Infinito)   
            flagHumedad=ON;
         else
            flagHumedad=OFF;
      }else if(Humedad<SetpointH-0.1 && Humedad>SetpointH-2.0){
         dehumidificador=OFF;
         tdehumidificador=0;
         CR7_Write(1);//Humidificador_off;
         CR5_Write(0);//Dehumidificador_off;
         if(!Infinito)   
            flagHumedad=ON;
         else
            flagHumedad=OFF;
      }else if(Humedad<SetpointH-3.0){
         Humidificar=ON; 
      }
   }
}

void controlaTemperatura(){
    if(TERM_Read()==1 || Temperatura>60){
      escribeDisplay1("statusT.txt=\"!Termostato!\"");      
      SSR_Write(0);
      Control=0;
      Ganancia=GananciaR*(Setpoint/200);
      BUZZER_Write(1);
      CyDelay(300);
      BUZZER_Write(0);
      CyDelay(300);
    }else{
        if(Enfria==1){
            if(Temperatura<=Setpoint+0.5 && Temperatura>=Setpoint-0.5){
                escribeDisplay1("statusT.txt=\"Controlando -\"");
                if(!Infinito)
                    flagTemperatura=ON;
                //tiempo_esterilizacion=1;
            else
                flagTemperatura=OFF;
                //tiempo_esterilizacion=0;
                Controla();
                flag_tiempocalentamiento=0;
            }else if(Temperatura<Setpoint-0.5){
                Enfria=0;
                Ganancia=GananciaR*(Setpoint/200);
                //Gananciamax=(0.21)*Setpoint;
                SSR_Write(0);
                Control=0;
            }else if(Temperatura>Setpoint+0.6){
                flag_tiempocalentamiento=0;
                escribeDisplay1("statusT.txt=\"Enfriando\"");
                //tiempo_esterilizacion=0;
                flagTemperatura=OFF;
                Controla();
            }
        }else{  
            if(Temperatura>=Setpoint-0.5 && Temperatura<=Setpoint+0.5){
                escribeDisplay1("statusT.txt=\"Controlando +\"");
                if(!Infinito)
                    //tiempo_esterilizacion=1;
                    flagTemperatura=ON;
                else
                    //tiempo_esterilizacion=0;   
                    flagTemperatura=OFF;
                Controla();
                flag_tiempocalentamiento=0;
            }else if(Temperatura>Setpoint+0.5){
                Enfria=1;
                Ganancia=GananciaR*(Setpoint/200);
                SSR_Write(0);
                Control=0;
            }else if(Temperatura<Setpoint-0.6){
                flag_tiempocalentamiento=1;
                escribeDisplay1("statusT.txt=\"Calentando\"");
                //tiempo_esterilizacion=0;
                flagTemperatura=OFF;
                Controla();
            }
        }
    }
}

CY_ISR(Reloj2){ 
    //CapSense_DisplayState();
    tiempos++;
    tiempos3++;
    tiempoctrl++;
    
    if(Control==1){// && tcHotJnuVolt > BROKEN_TC_THRESHOLD){
        if(tiempoctrl>=Tc) // Si el tiempo de control es mayor al tiempo de ciclo se reinicia tiempo de control
            tiempoctrl=0;
        // tiempoctrl incrementa una unidad cada 5ms, por eso se dividen los tiempos por 0.005.
      
        if(tiempoctrl<=Tp) // Si el tiempo es menor o igual al tiempo de control se enciende la resistencia de lo contrario se apaga.
            SSR_Write(1);
        else         
            SSR_Write(0);
    }else{
        if(!Test)
            SSR_Write(0);
    }

    if(tiempos3>=200){
        Lectura2=1;
        if(flagup)
            tiempoup++;
        if(flagdn)
            tiempodn++;   
        tiempos3=0;
    }
    
    if(tiempos>=500){  // Cuenta cada 2ms   
        tiempos=0;Lectura=1;tiemporeset++;TiempoControl++;tiemporecta++;mensaje=!mensaje;envioUSB=1;
        
        if(LEVEL_Read()==1){
            if(tllenado<600)
                tllenado++;
        }
        
        if(dehumidificador){
            tdehumidificador++;   
        }
        
        if(humidificador){
            thumidificador++;   
        }
        
        if(conteoPregunta)tPregunta++;
        
        if(flag_setpoint==1)
            tiemposetpoint++;
      
        if(tiempo_esterilizacion==1 || tiempo_esterilizacion==2){               
            tinicio++;
            tGrafica++;
        }
             
        if(flag_tiempocalentamiento==1)
            tiempocalentamiento++;
    }     
}

CY_ISR(inteRX){
    Dato2=UART_1_GetByte();
   if(Dato2==0x65){//Inicio Comunicacion
      Inicio=1;
      RX_Wr_Index =0;
   }
   if(Inicio==1){
      Rx_Buffer2[RX_Wr_Index] = Dato2;
      RX_Wr_Index++;
   }
   if(RX_Wr_Index >= RX_BUFFER_SIZE){
      RX_Wr_Index =0;
      Inicio=0;
   }
   if(RX_Wr_Index==0){
      if(Rx_Buffer2[0]==0x65 && Rx_Buffer2[1]==0xff && Rx_Buffer2[2]==0xff && Rx_Buffer2[8]==0x00 && Rx_Buffer2[9]==0xff )
         Dato_Exitoso=5;
      else
         Dato_Exitoso=10;
   }
   //if(Menu==20)
//     Buzzer_on;
}


int main(void)
{ 
    CyGlobalIntEnable; /* Enable global interrupts. */    

    Reloj_StartEx(Reloj2);
    UART_1_Start();    
    RX2_StartEx(inteRX); 
    Timer_Start();  
    Timer2_Start();  
    EEPROM_Start(); 
    ADC_Start();    
    ADC_SAR_Start();
    ADC_SAR_StartConvert();
    I2C_Start();
    IDAC_Start();
    CurrentMux_Start();
    ADCMux_Start();
    USBUART_Start(0, USBUART_5V_OPERATION);
    
    LCD2_Write(1);
    LCD1_Write(1);
    CyDelay(100);
    
    penInicial=penMufla;
    Opcion=1;Control=0;
    
    BUZZER_Write(1);
    LeeEEPROM();
    LimitaValores();
    BUZZER_Write(0);
    
    for(i=0;i<80;i++){
        Temperatura=leePt100(MediaMovil);
        Temperatura2=leePt1002(MediaMovil);
        Humedad=leeHumedad(MediaMovil2);
    }

    for(i=0;i<20;i++){
         if(rampas[i].Horas==0 && rampas[i].Minutos==0){
            Segmentos=i;  
            break;
         }else{
            Segmentos=i+1;  
         }
    } 
    
    if(controlT==0 && controlH==0 && controlR==0){
        Menu=120;Menu2=120;
        flagControlT=OFF;
        flagControlH=OFF;      
        flagRampa=OFF;
        
        escribeDisplay1("page Bienvenida");
        escribeDisplay1("bkcmd=0");
        CyDelay(10);
    }else{
        if(controlR==1)
            flagRampa=ON;
        else
            flagRampa=OFF;
        
        if(controlT==1)
            flagControlT=ON;
        else
            flagControlT=OFF;
        
        if(controlH==1)
            flagControlH=ON;
        else
            flagControlH=OFF;
        
        escribeDisplay1("page Principal");
        escribeDisplay1("bkcmd=0");
        CyDelay(10);
        Menu=20;Menu2=20;
    }
    
    DS1302_write (DS1302_ENABLE, 0);
    DS1302_write (DS1302_TRICKLE, 0x00);
    
    for(;;)
    {
        /* Wait for Device to enumerate */
        if(!USBUART_GetConfiguration()){
            
        }else{
            if(!flagusb){
                USBUART_CDC_Init();
                flagusb=ON;
            }
        }
        
        rtc_get_date(&Dia,&Mes,&Year,&dow);
        rtc_get_time(&Hora,&Minuto,&Segundo);
      
        if(Lectura2){
            grafica=ON;
            Lectura2=0;
            Temperatura=leePt100(MediaMovil);
            Temperatura2=leePt1002(MediaMovil);
            Temperature2=leeLM35();
            Humedad=leeHumedad(MediaMovil2);
            //ReadSensorCalibrationData();
            //ReadUncompensatedTemperature();
            //ReadAndCalculateSensorData();
        }
        
        if(envioUSB){
            envioUSB=0;
            tEnvioUSB++;
            if(tEnvioUSB>=tmuestreoUSB){
                tEnvioUSB=0;
                if(flagusb){
                    //fecha.txt=\"Fecha:%02u/%02u/20%02u    Hora: %02u:%02u:%02u\"",Dia,Mes,Year,Hora,Minuto,Segundo
                    if(rtdRes[0]>80000 && rtdRes[1]>80000){
                        sprintf(displayStr, "T:%1.1f°C -- T2:%1.1f°C -- HR:%1.1f %02u/%02u/20%02u %02u:%02u:%02u\n\r", Temperatura,Temperatura2,Humedad,Dia,Mes,Year,Hora,Minuto,Segundo); 
                    }else if(rtdRes[0]<80000 && rtdRes[1]>80000){
                        sprintf(displayStr, "T:----- T2:%1.1f°C -- HR:%1.1f %02u/%02u/20%02u %02u:%02u:%02u\n\r",Temperatura2,Humedad,Dia,Mes,Year,Hora,Minuto,Segundo); 
                    }else if(rtdRes[0]>80000 && rtdRes[1]<80000){
                        sprintf(displayStr, "T:%1.1f°C T2:---- -- HR:%1.1f %02u/%02u/20%02u %02u:%02u:%02u\n\r",Temperatura,Humedad,Dia,Mes,Year,Hora,Minuto,Segundo); 
                    }else if(rtdRes[0]<80000 && rtdRes[1]<80000){
                        sprintf(displayStr, "T:---- T2:----  -- HR:%1.1f  %02u/%02u/20%02u %02u:%02u:%02u\n\r",Humedad,Dia,Mes,Year,Hora,Minuto,Segundo); 
                    }
                    USBUART_PutString(displayStr);
                }
            }
        }
        LeeDisplay();
        
//_--------------------------------------------------------------------------------------------------------------------------------------------------------
        if(Menu == 120){ //Menu de Bienvenida.
            escribeDisplay1("bkcmd=0");
            CyDelay(800);
            escribeDisplay1("page Clave");
        }
//_--------------------------------------------------------------------------------------------------------------------------------------------------------
        
//_--------------------------------------------------------------------------------------------------------------------------------------------------------
    if(Menu==0){ //Menu de Contraseña de acceso.
      //ApagaSalidas();
      entro=0;
      if(Rx_Buffer[4]==0x11){//11
         if(TipoClave!=0){
            escribeDisplay1("page Principal");
         }
      }
      
      if(TipoClave==3){
         escribeDisplay1("titulo.txt=\"Clave Nueva\"");
         escribeDisplay1("doevents");
      }else if(TipoClave==2){
         escribeDisplay1("titulo.txt=\"Clave Actual\"");         
         escribeDisplay1("doevents");
      }else if(TipoClave==1 || TipoClave==4){
         escribeDisplay1("titulo.txt=\"Clave Tecnico\"");         
         escribeDisplay1("doevents");         
      }else if(TipoClave==0){
        sprintf(displayStr,"titulo.txt=\"Ingrese Clave\"");
        escribeDisplay1(displayStr);
      }
      
      if(Rx_Buffer[3]==0x0f){//0f, recibe caracteres ingresados desde el Display
         if(TipoClave==0){
            if(Rx_Buffer[4]>=0x30 && Rx_Buffer[5]>=0x30 && Rx_Buffer[6]>=0x30 && Rx_Buffer[7]>=0x30
            && Rx_Buffer[4]<=0x39 && Rx_Buffer[5]<=0x39 && Rx_Buffer[6]<=0x39 && Rx_Buffer[7]<=0x39){
            if(Rx_Buffer[4]==0x33&&Rx_Buffer[5]==0x38&&Rx_Buffer[6]==0x39&&Rx_Buffer[7]==0x32){ // Si Ingresa clave para reset general del sistema.
               EEPROM_WriteByte(0,10);CyDelay(20);EEPROM_WriteByte(0,11);CyDelay(20);// Reestablece a contraseña de Fabrica y reinicia Programa.
               EEPROM_WriteByte(0,12);CyDelay(20);EEPROM_WriteByte(0,13);CyDelay(20);
               CySoftwareReset();
            }
            }
            
            if(Rx_Buffer[4]>=0x30 && Rx_Buffer[5]>=0x30 && Rx_Buffer[6]>=0x30 && Rx_Buffer[7]>=0x30
            && Rx_Buffer[4]<=0x39 && Rx_Buffer[5]<=0x39 && Rx_Buffer[6]<=0x39 && Rx_Buffer[7]<=0x39){
            if((Rx_Buffer[4]==Password[0]+0x30)&&(Rx_Buffer[5]==Password[1]+0x30)&&(Rx_Buffer[6]==Password[2]+0x30)&&(Rx_Buffer[7]==Password[3]+0x30)){
                  escribeDisplay1("page ClaveCorrecta");
                  dibujo=OFF;
                  //RX_Buffer[3]=0x00;RX_Buffer2[3]=0x00;  
               }else{
                  escribeDisplay1("page ClaveBad");
                  //RX_Buffer[3]=0x00;RX_Buffer2[3]=0x00;  
               } 
            }
         }else if(TipoClave==1){
            if(Rx_Buffer[4]>=0x30 && Rx_Buffer[5]>=0x30 && Rx_Buffer[6]>=0x30 && Rx_Buffer[7]>=0x30
            && Rx_Buffer[4]<=0x39 && Rx_Buffer[5]<=0x39 && Rx_Buffer[6]<=0x39 && Rx_Buffer[7]<=0x39){
            if(Rx_Buffer[4]==0x34&&Rx_Buffer[5]==0x34&&Rx_Buffer[6]==0x34&&Rx_Buffer[7]==0x34){ // Si Ingresa clave de Servicio Tecnico
               escribeDisplay1("page Config");
            }else{
               escribeDisplay1("page Principal");
            }
            }
         }else if(TipoClave==2){
            if(Rx_Buffer[4]>=0x30 && Rx_Buffer[5]>=0x30 && Rx_Buffer[6]>=0x30 && Rx_Buffer[7]>=0x30
            && Rx_Buffer[4]<=0x39 && Rx_Buffer[5]<=0x39 && Rx_Buffer[6]<=0x39 && Rx_Buffer[7]<=0x39){
            if((Rx_Buffer[4]==Password[0]+0x30)&&(Rx_Buffer[5]==Password[1]+0x30)&&(Rx_Buffer[6]==Password[2]+0x30)&&(Rx_Buffer[7]==Password[3]+0x30)){
               escribeDisplay1("page Clave");
               escribeDisplay1("titulo.txt=\"Clave Nueva\"");
               TipoClave=3;
               Rx_Buffer[3]=0x00;
               Rx_Buffer2[3]=0x00;
            }else{
               escribeDisplay1("page Principal");
               //RX_Buffer[3]=0x00;
               //RX_Buffer2[3]=0x00;
            } 
            }
         }else if(TipoClave==3){
            escribeDisplay1("titulo.txt=\"Clave Nueva\"");
            escribeDisplay1("page Principal");
            if(!GuardaEEPROM){
               //write_eeprom(10,Rx_Buffer[4]-0x30);delay_ms(20);write_eeprom(11,Rx_Buffer[5]-0x30);delay_ms(20);
               EEPROM_WriteByte(Rx_Buffer[4]-0x30,10);CyDelay(20);
               EEPROM_WriteByte(Rx_Buffer[5]-0x30,11);CyDelay(20);
               EEPROM_WriteByte(Rx_Buffer[6]-0x30,12);CyDelay(20);
               EEPROM_WriteByte(Rx_Buffer[7]-0x30,13);CyDelay(20);
               GuardaEEPROM=ON;
            }
            Password[0]=Rx_Buffer[4]-0x30;Password[1]=Rx_Buffer[5]-0x30;Password[2]=Rx_Buffer[6]-0x30;Password[3]=Rx_Buffer[7]-0x30;
            //RX_Buffer[3]=0x00;
            //RX_Buffer2[3]=0x00;
         }else if(TipoClave==4){
            if(Rx_Buffer[4]==0x34&&Rx_Buffer[5]==0x34&&Rx_Buffer[6]==0x34&&Rx_Buffer[7]==0x34){ // Si Ingresa clave de Servicio Tecnico
               escribeDisplay1("page Test");
            }else{
               escribeDisplay1("page Principal");
            }
            //RX_Buffer[3]=0x00;
            //RX_Buffer2[3]=0x00;
         }
      }
    }
//_--------------------------------------------------------------------------------------------------------------------------------------------------------       

//_--------------------------------------------------------------------------------------------------------------------------------------------------------
    if(Menu==1){ //Menu Principal.
      GuardaEEPROM=OFF;
      Test=OFF;
      dibujo=OFF;
      if(Rx_Buffer[3]==0x0f){//0f, recibe caracteres ingresados desde el Display
         CyDelay(5);
         if(Rx_Buffer[3]==0x0f){
            if(Rx_Buffer[4]==0x37){//Ajustes y pasa a solicitud de password de tecnico
               escribeDisplay1("page Clave");
               MenuAnt=Menu;
               TipoClave=1;
               Rx_Buffer[3]=0x00;
               Rx_Buffer2[3]=0x00;
               Opcion=0;
            } 
            if(Opcion==7){// Cambio de Clave
               escribeDisplay1("page Clave");
               MenuAnt=Menu;
               TipoClave=2;
               Rx_Buffer[3]=0x00;
               Rx_Buffer2[3]=0x00;
               Opcion=0;
            }else if(Opcion==6){//Test
               escribeDisplay1("page Test");
               PantallaPrincipal=0;
               MenuAnt=Menu;
            }else if(Opcion==5){//Programa 5
               escribeDisplay1("page Rampas");
               MenuAnt=Menu;
               Programa=5;
            }else if(Opcion==4){//Programa 4
               escribeDisplay1("page Rampas");
               MenuAnt=2;
               Programa=4;
            }else if(Opcion==1){//Programa 1
               escribeDisplay1("page Rampas");
               MenuAnt=2;
               Programa=1;
            }else if(Opcion==3){//Programa 3
               escribeDisplay1("page Rampas");
               MenuAnt=Menu;               
               Programa=3;
            }else if(Opcion==2){//Programa 2
               escribeDisplay1("page Rampas");
               MenuAnt=Menu;
               Programa=2;
            }
         }
      }else {
         if(Opcion>7)
            Opcion=1;
            
         if(Opcion<1)
            Opcion=7;
         
         if(Opcion==1){
            sprintf(displayStr,"bc.pic=%u",iconoGrande[Opcion-1]);
            escribeDisplay1(displayStr);
            sprintf(displayStr,"bd.pic=%u",iconoPeque[Opcion]);
            escribeDisplay1(displayStr);
            sprintf(displayStr,"bi.pic=%u",iconoPeque[6]);
            escribeDisplay1(displayStr);
            sprintf(displayStr,"bc.pic2=%u",iconoGrande[Opcion-1]);
            escribeDisplay1(displayStr);
            sprintf(displayStr,"bd.pic2=%u",iconoPeque[Opcion]);
            escribeDisplay1(displayStr);
            sprintf(displayStr,"bi.pic2=%u",iconoPeque[6]);
            escribeDisplay1(displayStr);
            sprintf(displayStr,"textop.txt=\"%s\"",texto[Opcion-1]);
            escribeDisplay1(displayStr);
         }else if(Opcion==7){
            sprintf(displayStr,"bc.pic=%u",iconoGrande[Opcion-1]);
            escribeDisplay1(displayStr);
            sprintf(displayStr,"bd.pic=%u",iconoPeque[0]);
            escribeDisplay1(displayStr);
            sprintf(displayStr,"bi.pic=%u",iconoPeque[Opcion-2]);
            escribeDisplay1(displayStr);
            sprintf(displayStr,"bc.pic2=%u",iconoGrande[Opcion-1]);
            escribeDisplay1(displayStr);
            sprintf(displayStr,"bd.pic2=%u",iconoPeque[0]);
            escribeDisplay1(displayStr);
            sprintf(displayStr,"bi.pic2=%u",iconoPeque[Opcion-2]);
            escribeDisplay1(displayStr);
            sprintf(displayStr,"textop.txt=\"%s\"",texto[Opcion-1]);
            escribeDisplay1(displayStr);
         }else{
            sprintf(displayStr,"bc.pic=%u",iconoGrande[Opcion-1]);
            escribeDisplay1(displayStr);
            sprintf(displayStr,"bd.pic=%u",iconoPeque[Opcion]);
            escribeDisplay1(displayStr);
            sprintf(displayStr,"bi.pic=%u",iconoPeque[Opcion-2]);
            escribeDisplay1(displayStr);
            sprintf(displayStr,"bc.pic2=%u",iconoGrande[Opcion-1]);
            escribeDisplay1(displayStr);
            sprintf(displayStr,"bd.pic2=%u",iconoPeque[Opcion]);
            escribeDisplay1(displayStr);
            sprintf(displayStr,"bi.pic2=%u",iconoPeque[Opcion-2]);
            escribeDisplay1(displayStr);
            sprintf(displayStr,"textop.txt=\"%s\"",texto[Opcion-1]);
            escribeDisplay1(displayStr);
         }
                  
         if(Rx_Buffer[4]==0x0d || Rx_Buffer[4]==0x0c){//Tecla Derecha Oprimida
            Opcion++;
            Rx_Buffer[4]=0x00;
            Rx_Buffer2[4]=0x00;
         }
         
         if(Rx_Buffer[4]==0x0e || Rx_Buffer[4]==0x0f){//Tecla Izquierda Oprimida
            Opcion--;
            Rx_Buffer[4]=0x00;
            Rx_Buffer2[4]=0x00;
         }
                  
      }
   }
//_--------------------------------------------------------------------------------------------------------------------------------------------------------       
    
//_--------------------------------------------------------------------------------------------------------------------------------------------------------
    if(Menu==2){ //Menu de Tiempo de Duraciòn
        dibujo=OFF;
        imprime=OFF;
        sprintf(displayStr,"fecha.txt=\"Fecha:%02u/%02u/20%02u    Hora: %02u:%02u:%02u\"",Dia,Mes,Year,Hora,Minuto,Segundo);
        escribeDisplay1(displayStr);
        
         if(!rampa){
            if(Rx_Buffer[4]==0x0a){//Selecciono Minutos
               Minutos=OFF;
               Horas=ON;
               pulsoAumentar=OFF;
               pulsoDisminuir=OFF;
            }
            
            if(Rx_Buffer[4]==0x0b){//Selecciono Segundos
               Minutos=ON;
               Horas=OFF;
               pulsoAumentar=OFF;
               pulsoDisminuir=OFF;
            }           
            
            if(Rx_Buffer[4]==0x11){//Selecciono Regresar
               CyDelay(200);
               if(Rx_Buffer[4]==0x11){
                  Minutos=OFF;
                  Horas=OFF;
                  escribeDisplay1("page Principal");
                  if(!GuardaEEPROM){
                     EEPROM_WriteByte(mEstp,0);
                     CyDelay(10);
                     EEPROM_WriteByte(hEstp,1);
                     CyDelay(10);
                     //mEst=mEstp;
                     //hEst=hEstp;
                     if((mEstp==0)&&(hEstp==0))
                        Infinito=1;
                     else
                        Infinito=0;
                     GuardaEEPROM=ON;
                     pulsoAumentar=OFF;
                     pulsoDisminuir=OFF;
                  }
               }
            }
                        
            if(pulsoAumentar){//Tecla arriba Oprimida
               if(Minutos){
                  mEstp++;
               }
               if(Horas){
                  hEstp++;
               }  
               CyDelay(100);
            }
            
            if(pulsoDisminuir){//Tecla abajo oprimida
               if(Minutos && mEstp>0){
                  mEstp--;
               }
               if(Horas  && hEstp>0){
                  hEstp--;
               }
               CyDelay(100);
            }
            
            if(hEstp>99)hEstp=1;
            if(mEstp>59)mEstp=0;
               
            sprintf(displayStr,"tminsec.txt=\"%02u\"",hEstp);
            escribeDisplay1(displayStr);
            sprintf(displayStr,"tsecsec.txt=\"%02u\"",mEstp);
            escribeDisplay1(displayStr);
         }else{
            if(Rx_Buffer[4]==0x0a){//Selecciono Minutos
               Minutos=OFF;
               Horas=ON;
            }
            
            if(Rx_Buffer[4]==0x0b){//Selecciono Segundos
               Minutos=ON;
               Horas=OFF;
            }           
            
            if(Rx_Buffer[4]==0x11){//Selecciono Regresar
               CyDelay(200);
               if(Rx_Buffer[4]==0x11){
                  Minutos=OFF;
                  Horas=OFF;
                  escribeDisplay1("page Rampas");
                  if(!GuardaEEPROM){
                     EEPROM_WriteByte((int8)rampas[CicloLibre-1].Horas,103+((CicloLibre-1)*5));
                     CyDelay(10);
                     EEPROM_WriteByte((int8)rampas[CicloLibre-1].Minutos,104+((CicloLibre-1)*5));
                     CyDelay(10);
                     GuardaEEPROM=ON;
                  }
                  pulsoAumentar=OFF;
                  pulsoDisminuir=OFF;
               }
            }
                      
            if(pulsoAumentar){//Tecla arriba Oprimida
               if(Minutos){
                  rampas[CicloLibre-1].Minutos+=1.0;
               }
               if(Horas){
                  rampas[CicloLibre-1].Horas+=1.0;
               }  
               CyDelay(100);
            }
            
            if(pulsoDisminuir){//Tecla abajo oprimida
               if(Minutos && rampas[CicloLibre-1].Minutos>0){
                  rampas[CicloLibre-1].Minutos-=1.0;
               }
               if(Horas  && rampas[CicloLibre-1].Horas>0){
                  rampas[CicloLibre-1].Horas-=1.0;
               }
               CyDelay(100);
            }
            
            if(rampas[CicloLibre-1].Horas>99)rampas[CicloLibre-1].Horas=1;
            if(rampas[CicloLibre-1].Minutos>59)rampas[CicloLibre-1].Minutos=0;
               
            sprintf(displayStr,"tminsec.txt=\"%02u\"",rampas[CicloLibre-1].Horas);
            escribeDisplay1(displayStr);
            sprintf(displayStr,"tsecsec.txt=\"%02u\"",rampas[CicloLibre-1].Minutos);
            escribeDisplay1(displayStr);
         }
        
        if(Rx_Buffer[4]==0x0d){//Tecla Arriba Oprimida
           pulsoAumentar=ON;
           pulsoDisminuir=OFF;
           Rx_Buffer[4]=0x00;  
           Rx_Buffer2[4]=0x00;  
        }else if(Rx_Buffer[4]==0x0e){//Tecla Arriba Suelta
           pulsoAumentar=OFF;
           pulsoDisminuir=OFF;
           Rx_Buffer[4]=0x00;  
           Rx_Buffer2[4]=0x00;            
        }else if(Rx_Buffer[4]==0x0c){//Tecla Abajo Oprimida
           pulsoDisminuir=ON;
            pulsoAumentar=OFF;
           Rx_Buffer[4]=0x00;
           Rx_Buffer2[4]=0x00;                        
        }else if(Rx_Buffer[4]==0x0b){//Tecla Abajo Oprimida
           pulsoAumentar=OFF;
           pulsoDisminuir=OFF;
           Rx_Buffer[4]=0x00;
           Rx_Buffer2[4]=0x00;                        
        }else{//if(Rx_Buffer[4]!=0x00){
           pulsoAumentar=OFF;
           pulsoDisminuir=OFF; 
        }
    }
//_--------------------------------------------------------------------------------------------------------------------------------------------------------       

//_--------------------------------------------------------------------------------------------------------------------------------------------------------
    if(Menu==3){ //Menu de Temperatura
      dibujo=OFF;
      imprime=OFF;
      
        if(Rx_Buffer[4]==0x0d){//Tecla Arriba Oprimida
           pulsoAumentar=ON;
           pulsoDisminuir=OFF;
           Rx_Buffer[4]=0x00;  
           Rx_Buffer2[4]=0x00;  
           CyDelay(100);
        }else if(Rx_Buffer[4]==0x0e){//Tecla Arriba Suelta
           pulsoAumentar=OFF;
           pulsoDisminuir=OFF;
           Rx_Buffer[4]=0x00;  
           Rx_Buffer2[4]=0x00;            
           CyDelay(100);
        }else if(Rx_Buffer[4]==0x0c){//Tecla Abajo Oprimida
           pulsoDisminuir=ON;
            pulsoAumentar=OFF;
           Rx_Buffer[4]=0x00;
           Rx_Buffer2[4]=0x00;                        
           CyDelay(100);
        }else if(Rx_Buffer[4]==0x0b){//Tecla Abajo Oprimida
           pulsoAumentar=OFF;
           pulsoDisminuir=OFF;
           Rx_Buffer[4]=0x00;
           Rx_Buffer2[4]=0x00;                        
           CyDelay(100);
        }else if(Rx_Buffer[4]==0x20){//Selecciono x1
            x1=ON;
            x10=OFF;
            x100=OFF;
            escribeDisplay1("c0.val=1");
            escribeDisplay1("c1.val=0");
            escribeDisplay1("c2.val=0");
            Rx_Buffer[4]=0x00;  
            Rx_Buffer2[4]=0x00;     
        }else if(Rx_Buffer[4]==0x21){//Selecciono x10
            x1=OFF;
            x10=ON;
            x100=OFF;
            escribeDisplay1("c0.val=0");
            escribeDisplay1("c1.val=1");
            escribeDisplay1("c2.val=0");
            Rx_Buffer[4]=0x00;  
            Rx_Buffer2[4]=0x00;     
        }else if(Rx_Buffer[4]==0x22){//Selecciono x100
            x1=OFF;
            x10=OFF;
            x100=ON;
            escribeDisplay1("c0.val=0");
            escribeDisplay1("c1.val=0");
            escribeDisplay1("c2.val=1");
            Rx_Buffer[4]=0x00;  
            Rx_Buffer2[4]=0x00;     
        }else{//if(Rx_Buffer[4]!=0x00){
           pulsoAumentar=OFF;
           pulsoDisminuir=OFF; 
        }
        
      sprintf(displayStr,"fecha.txt=\"Fecha:%02u/%02u/20%02u    Hora: %02u:%02u:%02u\"",Dia,Mes,Year,Hora,Minuto,Segundo);
      escribeDisplay1(displayStr);
      
      if(!rampa){
         sprintf(displayStr,"tsettem.txt=\"%2.1f\"",Setpoint);
         escribeDisplay1(displayStr);
         
         if(Rx_Buffer[4]==0x11){//Selecciono Regresar
            Horas=OFF;
            Minutos=OFF;
            escribeDisplay1("page Principal");  
            if(!GuardaEEPROM){
               EEPROM_WriteByte((int8)Setpoint,80);
               EEPROM_WriteByte(extraeDecimal(Setpoint),81);
               CyDelay(10);
               GuardaEEPROM=ON;
            }
            pulsoAumentar=OFF;
            pulsoDisminuir=OFF;            
            if(Setpoint<Temperatura){
               Enfria=1;
               Ganancia=GananciaR*(Setpoint/200);               
            }else{
               Enfria=0;
               Ganancia=GananciaR*(Setpoint/200);
            }
         }
         
        if(pulsoAumentar){
            if(x1)
                Setpoint+=0.1;    
            else if(x10)    
                Setpoint+=1;    
            else if(x100)    
                Setpoint+=10; 
            CyDelay(100);    
        }
        
        if(pulsoDisminuir){
            if(x1)
                Setpoint-=0.1;    
            else if(x10)    
                Setpoint-=1;    
            else if(x100)    
                Setpoint-=10;
            CyDelay(100);
        }
         
        if(Setpoint<0.1)
            Setpoint=60.0;
         if(Setpoint>60.0)
            Setpoint=0.1;
      }else{
         sprintf(displayStr,"tsettem.txt=\"%2.1f\"",rampas[CicloLibre-1].Temperatura);
         escribeDisplay1(displayStr);
                    
         if(Rx_Buffer[4]==0x11){//Selecciono Regresar
            n=0;
            conteo=0;
            Horas=OFF;
            Minutos=OFF;
            escribeDisplay1("page Rampas");
            if(!GuardaEEPROM){
               EEPROM_WriteByte((uint8)rampas[CicloLibre-1].Temperatura,100+((CicloLibre-1)*5));
               CyDelay(10);
               EEPROM_WriteByte(extraeDecimal(rampas[CicloLibre-1].Temperatura),101+((CicloLibre-1)*5));
               CyDelay(10);
               GuardaEEPROM=ON;
            }
            pulsoAumentar=OFF;
            pulsoDisminuir=OFF;
            if(Setpoint<Temperatura){
               Enfria=1;
               Ganancia=GananciaR*(Setpoint/200);               
            }else{
               Enfria=0;
               Ganancia=GananciaR*(Setpoint/200);
            }
         }
        
        if(pulsoAumentar){
            if(x1)
                rampas[CicloLibre-1].Temperatura+=0.1;    
            else if(x10)    
                rampas[CicloLibre-1].Temperatura+=1;    
            else if(x100)    
                rampas[CicloLibre-1].Temperatura+=10;
                
            CyDelay(100);    
        }
        
        if(pulsoDisminuir){
            if(x1)
                rampas[CicloLibre-1].Temperatura-=0.1;    
            else if(x10)    
                rampas[CicloLibre-1].Temperatura-=1;    
            else if(x100)    
                rampas[CicloLibre-1].Temperatura-=10;
                
            CyDelay(100);
        }   
        
        if(rampas[CicloLibre-1].Temperatura<0.1)
            rampas[CicloLibre-1].Temperatura=60.0;
         if(rampas[CicloLibre-1].Temperatura>60.0)
            rampas[CicloLibre-1].Temperatura=0.1;
      }
         
    }
//_--------------------------------------------------------------------------------------------------------------------------------------------------------       

//_--------------------------------------------------------------------------------------------------------------------------------------------------------
    if(Menu==4){ //Menu de Humedad
      sprintf(displayStr,"fecha.txt=\"Fecha:%02u/%02u/20%02u    Hora: %02u:%02u:%02u\"",Dia,Mes,Year,Hora,Minuto,Segundo);
      escribeDisplay1(displayStr);
      
        if(Rx_Buffer[4]==0x0d){//Tecla Arriba Oprimida
           pulsoAumentar=ON;
           pulsoDisminuir=OFF;
           Rx_Buffer[4]=0x00;  
           Rx_Buffer2[4]=0x00;  
           CyDelay(100);
        }else if(Rx_Buffer[4]==0x0e){//Tecla Arriba Suelta
           pulsoAumentar=OFF;
           pulsoDisminuir=OFF;
           Rx_Buffer[4]=0x00;  
           Rx_Buffer2[4]=0x00;            
           CyDelay(100);
        }else if(Rx_Buffer[4]==0x0c){//Tecla Abajo Oprimida
           pulsoDisminuir=ON;
            pulsoAumentar=OFF;
           Rx_Buffer[4]=0x00;
           Rx_Buffer2[4]=0x00;                        
           CyDelay(100);
        }else if(Rx_Buffer[4]==0x0b){//Tecla Abajo Oprimida
           pulsoAumentar=OFF;
           pulsoDisminuir=OFF;
           Rx_Buffer[4]=0x00;
           Rx_Buffer2[4]=0x00;                        
           CyDelay(100);
        }else if(Rx_Buffer[4]==0x20){//Selecciono x1
            x1=ON;
            x10=OFF;
            x100=OFF;
            escribeDisplay1("c0.val=1");
            escribeDisplay1("c1.val=0");
            escribeDisplay1("c2.val=0");
            Rx_Buffer[4]=0x00;  
            Rx_Buffer2[4]=0x00;     
        }else if(Rx_Buffer[4]==0x21){//Selecciono x10
            x1=OFF;
            x10=ON;
            x100=OFF;
            escribeDisplay1("c0.val=0");
            escribeDisplay1("c1.val=1");
            escribeDisplay1("c2.val=0");
            Rx_Buffer[4]=0x00;  
            Rx_Buffer2[4]=0x00;     
        }else{// if(Rx_Buffer[4]!=0x00){
           pulsoAumentar=OFF;
           pulsoDisminuir=OFF; 
        }
        
      if(!rampa){
         if(Rx_Buffer[4]==0x11){//Selecciono Regresar
            Horas=OFF;
            Minutos=OFF;
            escribeDisplay1("page Principal");
            if(!GuardaEEPROM){
               EEPROM_WriteByte(SetpointH,82);
               CyDelay(10);
               GuardaEEPROM=ON;
            }
            pulsoAumentar=OFF;
            pulsoDisminuir=OFF;
         }
         
        if(pulsoAumentar){
            if(x1)
                SetpointH+=1;    
            else if(x10)    
                SetpointH+=10;    
            CyDelay(100);    
        }
        
        if(pulsoDisminuir){
            if(x1)
                SetpointH-=1;    
            else if(x10)    
                SetpointH-=10;    
            CyDelay(100);
        }
        
        if(SetpointH<10)
            SetpointH=95;
         if(SetpointH>95)
            SetpointH=10;
        
         sprintf(displayStr,"tsettem.txt=\"%02u\"",SetpointH);
         escribeDisplay1(displayStr);
      }else{
         
         if(Rx_Buffer[4]==0x11){//Selecciono Regresar
            Horas=OFF;
            Minutos=OFF;
            escribeDisplay1("page Rampas");
            if(!GuardaEEPROM){
               EEPROM_WriteByte(rampas[CicloLibre-1].Humedad,102+((CicloLibre-1)*5));
               CyDelay(10);
               GuardaEEPROM=ON;
            }
            pulsoAumentar=OFF;
            pulsoDisminuir=OFF;
         }
         
         if(pulsoAumentar){
            if(x1)
                rampas[CicloLibre-1].Humedad+=1;    
            else if(x10)    
                rampas[CicloLibre-1].Humedad+=10;    
            CyDelay(100);    
        }
        
        if(pulsoDisminuir){
            if(x1)
                rampas[CicloLibre-1].Humedad-=1;    
            else if(x10)    
                rampas[CicloLibre-1].Humedad-=10;    
            CyDelay(100);
        }
        
        if(rampas[CicloLibre-1].Humedad<10)
            rampas[CicloLibre-1].Humedad=95;
         if(rampas[CicloLibre-1].Humedad>95)
            rampas[CicloLibre-1].Humedad=10;
            
         sprintf(displayStr,"tsettem.txt=\"%02u\"",rampas[CicloLibre-1].Humedad);
         escribeDisplay1(displayStr);
      }
    }
//_--------------------------------------------------------------------------------------------------------------------------------------------------------       
    
//_--------------------------------------------------------------------------------------------------------------------------------------------------------      
    if(Menu==6){//Menu de Test
        Test=ON;
        Control=0;
        if(Rx_Buffer[4]==0x21){
            escribeDisplay1("page Principal");
            Test=OFF;
            Rx_Buffer[4]=0x00;
            Rx_Buffer2[4]=0x00;   
        }
        
        if(Rx_Buffer[4]==0x08){
            estadoSSR=!estadoSSR;
            Rx_Buffer[4]=0x00;
            Rx_Buffer2[4]=0x00;   
        }
        
        if(Rx_Buffer[4]==0x13){
            BUZZER_Write(!BUZZER_Read());
            Rx_Buffer[4]=0x00;
            Rx_Buffer2[4]=0x00;   
        }
        
        if(Rx_Buffer[4]==0x01){
            CR1_Write(!CR1_Read());
            Rx_Buffer[4]=0x00;
            Rx_Buffer2[4]=0x00;   
        }
        
        if(Rx_Buffer[4]==0x02){
            CR2_Write(!CR2_Read());
            Rx_Buffer[4]=0x00;
            Rx_Buffer2[4]=0x00;   
        }
        
        if(Rx_Buffer[4]==0x03){
            CR3_Write(!CR3_Read());
            Rx_Buffer[4]=0x00;
            Rx_Buffer2[4]=0x00;   
        }
        
        if(Rx_Buffer[4]==0x04){
            CR4_Write(!CR4_Read());
            Rx_Buffer[4]=0x00;
            Rx_Buffer2[4]=0x00;   
        }
        
        if(Rx_Buffer[4]==0x05){
            CR5_Write(!CR5_Read());
            Rx_Buffer[4]=0x00;
            Rx_Buffer2[4]=0x00;   
        }
        
        if(Rx_Buffer[4]==0x06){
            CR6_Write(!CR6_Read());
            Rx_Buffer[4]=0x00;
            Rx_Buffer2[4]=0x00;   
        }
        
        if(Rx_Buffer[4]==0x07){
            CR7_Write(!CR7_Read());
            Rx_Buffer[4]=0x00;
            Rx_Buffer2[4]=0x00;   
        }
        
        if(Rx_Buffer[4]==0x12){
            CR8_Write(!CR8_Read());
            Rx_Buffer[4]=0x00;
            Rx_Buffer2[4]=0x00;   
        }
        
        if(estadoSSR){
            SSR_Write(1);
            escribeDisplay1("b7.bco=1024");
        }else{
            SSR_Write(0);
            escribeDisplay1("b7.bco=63488");
        }
        
        if(BUZZER_Read()==1){
            escribeDisplay1("b12.bco=1024");
        }else{
            escribeDisplay1("b12.bco=63488");
        }
        
        if(CR1_Read()==1){
            escribeDisplay1("b0.bco=1024");
        }else{
            escribeDisplay1("b0.bco=63488");
        }
        
        if(CR2_Read()==1){
            escribeDisplay1("b1.bco=1024");
        }else{            
            escribeDisplay1("b1.bco=63488");
        }
        
        if(CR3_Read()==1){            
            escribeDisplay1("b2.bco=1024");
        }else{            
            escribeDisplay1("b2.bco=63488");
        }
        
        if(CR4_Read()==1){            
            escribeDisplay1("b3.bco=1024");
        }else{            
            escribeDisplay1("b3.bco=63488");
        }
        
        if(CR5_Read()==1){            
            escribeDisplay1("b4.bco=1024");
        }else{            
            escribeDisplay1("b4.bco=63488");
        }
        
        if(CR6_Read()==0){            
            escribeDisplay1("b5.bco=1024");
        }else{            
            escribeDisplay1("b5.bco=63488");
        }
        
        if(CR7_Read()==0){            
            escribeDisplay1("b6.bco=1024");
        }else{            
            escribeDisplay1("b6.bco=63488");
        }
        
        if(CR8_Read()==1){            
            escribeDisplay1("b11.bco=1024");
        }else{            
            escribeDisplay1("b11.bco=63488");
        }
        
        if(DOOR_Read()==0)
            escribeDisplay1("b8.bco=1024");
        else
            escribeDisplay1("b8.bco=63488");
        
        if(TERM_Read()==0)
            escribeDisplay1("b9.bco=1024");
        else
            escribeDisplay1("b9.bco=63488");    
        
        if(LEVEL_Read()==0){
            escribeDisplay1("b10.bco=1024");
            CR8_Write(0);
        }else{
            escribeDisplay1("b10.bco=63488");        
        }
            
      sprintf(displayStr,"b13.txt=\"RTD0:%ld \"",rtdRes[0]);
      escribeDisplay1(displayStr);
      sprintf(displayStr,"b14.txt=\"RTD1:%ld \"",rtdRes[1]);
      escribeDisplay1(displayStr);
      sprintf(displayStr,"b15.txt=\"T1:%1.2f \"",Temperatura);
      escribeDisplay1(displayStr);
      sprintf(displayStr,"b18.txt=\"T35:%ld\"",leeLM35());
      escribeDisplay1(displayStr);
      sprintf(displayStr,"b19.txt=\"T2:%1.2f\"",Temperatura2);
      escribeDisplay1(displayStr);
      sprintf(displayStr,"b20.txt=\"Thum:%1.2f\"",Tam);
      escribeDisplay1(displayStr);
   }
//_--------------------------------------------------------------------------------------------------------------------------------------------------------          
    
//_--------------------------------------------------------------------------------------------------------------------------------------------------------      
    if(Menu==7){ //Menu de Configuración de Fecha y Hora
         if(Rx_Buffer[4]==0x0a){//Selecciono Hora
            Opcion=4;
         }
         
         if(Rx_Buffer[4]==0x0b){//Selecciono Minuto
            Opcion=5;
         }
         
         if(Rx_Buffer[4]==0x0c){//Selecciono Dia
            Opcion=1;
         }
         
         if(Rx_Buffer[4]==0x0d){//Selecciono Hora
            Opcion=2;
         }
         
         if(Rx_Buffer[4]==0x0e){//Selecciono Year
            Opcion=3;
         }
         
         if(Opcion>5)
            Opcion=1;
         if(Opcion<1)
            Opcion=5;
            
         if(Rx_Buffer[4]==0x11){//Selecciono Regresar
            escribeDisplay1("page Principal");
            
            if(esBisiesto(YearTx))
               Modulo=Bisiesto[MesTx];
            else
               Modulo=Regular[MesTx];
         
            dowTx=((YearTx-1)%7+((YearTx-1)/4-3*((YearTx-1)/100+1)/4)%7+Modulo+DiaTx%7)%7;
            rtc_set_datetime(DiaTx,MesTx,YearTx,dowTx,HoraTx,MinutoTx);
         }
         
         if(Rx_Buffer[4]==0x2a){//Tecla Arriba Oprimida
               if(Opcion==1){
                  if(MesTx==2){
                     if(esBisiesto(YearTx)){
                        if(DiaTx<29)
                           DiaTx++;
                        else
                           DiaTx=1;   
                     }else{
                        if(DiaTx<28)
                           DiaTx++;
                        else
                           DiaTx=1;   
                     }
                  }else{
                     if(MesTx<=7){
                        if(MesTx % 2 ==0){
                           if(DiaTx<30)
                              DiaTx++;    
                           else
                              DiaTx=1;   
                        }else{
                           if(DiaTx<31)
                              DiaTx++;    
                           else
                              DiaTx=1;   
                        }    
                     }else{
                        if(MesTx % 2 ==0){
                           if(DiaTx<31)
                              DiaTx++;  
                           else
                              DiaTx=1;
                        }else{
                           if(DiaTx<30)
                              DiaTx++;    
                           else
                              DiaTx=1;
                        }    
                     }
                  }
               }else if(Opcion==2){
                  if(MesTx<12)
                     MesTx++;
                  else
                     MesTx=1;
               }else if(Opcion==3){
                  if(YearTx<99)
                     YearTx++;
                  else 
                     YearTx=0;
               }else if(Opcion==4){
                  if(HoraTx<24)
                     HoraTx++;
                  else
                     HoraTx=0;
               }else if(Opcion==5){
                  if(MinutoTx<59)
                     MinutoTx++;
                  else
                     MinutoTx=0;
               }
               Rx_Buffer[4]=0x00;
               Rx_Buffer2[4]=0x00;
            
         }
         
         if(Rx_Buffer[4]==0x2b){//Tecla Abajo Oprimida
               if(Opcion==1){
                  if(DiaTx>0)
                     DiaTx--;
               }else if(Opcion==2){
                  if(MesTx>1)
                     MesTx--;
                  else
                     MesTx=12;
               }else if(Opcion==3){
                  if(YearTx>0)
                     YearTx--;
                  else
                     YearTx=99;
               }else if(Opcion==4){
                  if(HoraTx>0)
                     HoraTx--;
                  else
                     HoraTx=23;
               }else if(Opcion==5){
                  if(MinutoTx>0)
                     MinutoTx--;
                  else
                     MinutoTx=59;
               }
               Rx_Buffer[4]=0x00;
               Rx_Buffer2[4]=0x00;
         }
      
      sprintf(displayStr,"thora.txt=\"%02u\"",HoraTx);
      escribeDisplay1(displayStr);
      sprintf(displayStr,"tminutos.txt=\"%02u\"",MinutoTx);
      escribeDisplay1(displayStr);
      sprintf(displayStr,"tdia.txt=\"%02u\"",DiaTx);
      escribeDisplay1(displayStr);
      sprintf(displayStr,"tmes.txt=\"%02u\"",MesTx);
      escribeDisplay1(displayStr);
      sprintf(displayStr,"tyear.txt=\"%02u\"",YearTx);
      escribeDisplay1(displayStr);
      LimitaDia();  
    }
//_--------------------------------------------------------------------------------------------------------------------------------------------------------       
        
//_--------------------------------------------------------------------------------------------------------------------------------------------------------
    if(Menu==8){ //Menu de Rampas
      flagBorrado=OFF;
      sprintf(displayStr,"fecha.txt=\"Fecha:%02u/%02u/20%02u Hora: %02u:%02u:%02u\"",Dia,Mes,Year,Hora,Minuto,Segundo);
      escribeDisplay1(displayStr);
      x1=ON;x10=OFF;x100=OFF;
    
      for(i=0;i<20;i++){
         if(rampas[i].Horas==0 && rampas[i].Minutos==0){
            Segmentos=i;  
            break;
         }else{
            Segmentos=i+1;  
         }
      }
      
      sprintf(displayStr,"t3.txt=\"Pasos Programados %02u\"",Segmentos);
      escribeDisplay1(displayStr);
      GuardaEEPROM=OFF;
      
      if(Rx_Buffer[4]>0x00 && Rx_Buffer[4]<21){//Personalizados
         sprintf(displayStr,"ciclo.txt=\"Paso %02u\"",Rx_Buffer[4]);
         escribeDisplay1(displayStr);
         sprintf(displayStr,"temp.txt=\"Temperatura: %2.1f°C\"",rampas[Rx_Buffer[4]-1].Temperatura);
         escribeDisplay1(displayStr);
         sprintf(displayStr,"hum.txt=\"Humedad: %03u \"",rampas[Rx_Buffer[4]-1].Humedad);
         escribeDisplay1(displayStr);
         sprintf(displayStr,"tiempo.txt=\"Tiempo: %02u:%02u \"",rampas[Rx_Buffer[4]-1].Horas,rampas[Rx_Buffer[4]-1].Minutos);
         escribeDisplay1(displayStr);
         CicloLibre=Rx_Buffer[4];
      }else if(CicloLibre==0x00){
         escribeDisplay1("ciclo.txt=\"Paso --\"");
         escribeDisplay1("temp.txt=\"Temperatura: --\"");
         escribeDisplay1("hum.txt=\"Humedad: --\"");
         escribeDisplay1("tiempo.txt=\"Tiempo: --:--\"");
      }else if(CicloLibre>0 && CicloLibre<21){
         sprintf(displayStr,"ciclo.txt=\"Paso %02u\"",CicloLibre);
         escribeDisplay1(displayStr);         
         sprintf(displayStr,"temp.txt=\"Temperatura: %2.1f°C\"",rampas[CicloLibre-1].Temperatura);
         escribeDisplay1(displayStr);
         sprintf(displayStr,"hum.txt=\"Humedad: %03u \"",rampas[CicloLibre-1].Humedad);
         escribeDisplay1(displayStr);
         sprintf(displayStr,"tiempo.txt=\"Tiempo: %02u:%02u \"",rampas[CicloLibre-1].Horas,rampas[CicloLibre-1].Minutos);
         escribeDisplay1(displayStr);
      }
      
      if(Rx_Buffer[4]==0x20 && CicloLibre>0){//20, Temperatura
         escribeDisplay1("page Temperatura");
         rampa=ON;
      }
      
      if(Rx_Buffer[4]==0x30 && CicloLibre>0){//30, Humedad
         escribeDisplay1("page Humedad");
         rampa=ON;
      }
      
      if(Rx_Buffer[4]==0x40 && CicloLibre>0){//40, Tiempo
         escribeDisplay1("page Tiempo");
         rampa=ON;
      }
      
      if(Rx_Buffer[4]==0x70){//70, Regresar
         escribeDisplay1("page Principal");
         rampa=OFF;
      }
   }
//_--------------------------------------------------------------------------------------------------------------------------------------------------------       
        
//_--------------------------------------------------------------------------------------------------------------------------------------------------------      
    if(Menu==15){//Menu de Clave Correcta
      if(!entro){
         CyDelay(2000);
         entro=ON;
      }
      escribeDisplay1("page Principal");
   }
//_--------------------------------------------------------------------------------------------------------------------------------------------------------      

//_--------------------------------------------------------------------------------------------------------------------------------------------------------      
    if(Menu==16){//Menu de Clave InCorrecta
      if(!entro){
         CyDelay(2000);
         entro=ON;
      }
      escribeDisplay1("page Clave");
   }
//_--------------------------------------------------------------------------------------------------------------------------------------------------------      
    
//_--------------------------------------------------------------------------------------------------------------------------------------------------------      
    if(Menu==17){//Menu de Programa Eliminado
      if(!flagBorrado){
         CyDelay(2000);
         flagBorrado=ON;
      }
      escribeDisplay1("page Principal");
   }
//_--------------------------------------------------------------------------------------------------------------------------------------------------------          

//_--------------------------------------------------------------------------------------------------------------------------------------------------------      
    if(Menu==18){//Menu de Pegunta
        conteoPregunta=ON;
        if(tPregunta>10){
            escribeDisplay1("page Funcionamiento");
        }
        
      if(Rx_Buffer[4]==0x01){//Si
         escribeDisplay1("page MenuPrincipal");
         mEst=0;
         hEst=0;
         CicloLibre=0;
         SSR_Write(0);         
         Control=0;
         Tpulso=0.0;
         tiempo_esterilizacion=0;
         flag_tiempocalentamiento=0;
         Segmento=0;
         Ganancia=GananciaIn*(Setpoint/100);
         tinicio=0;
         Finalizo=OFF;
         flag_calcula=OFF;
         flagTemperatura=OFF;
         flagRampa=OFF;
         flagFinalizo=OFF;
         Rx_Buffer[4]=0x00;
         Rx_Buffer2[4]=0x00;
      }
    
      if(Rx_Buffer[4]==0x02){//No
         escribeDisplay1("page Funcionamiento");
         Rx_Buffer[4]=0x00;
         Rx_Buffer2[4]=0x00;
      }
   }
//_--------------------------------------------------------------------------------------------------------------------------------------------------------          

//_--------------------------------------------------------------------------------------------------------------------------------------------------------
    if(Menu==20){ //Menu de Funcionamiento
      GuardaEEPROM=OFF;
      dibujo=OFF;
      conteoPregunta=OFF;
      tPregunta=0;
      sprintf(displayStr,"t7.txt=\"Camara Climatica JP Inglobal \"");
      escribeDisplay1(displayStr);
      sprintf(displayStr,"fecha.txt=\"Fecha:%02u/%02u/20%02u    Hora: %02u:%02u:%02u\"",Dia,Mes,Year,Hora,Minuto,Segundo);
      escribeDisplay1(displayStr);
      x1=ON;x10=OFF;x100=OFF;

      if(Rx_Buffer[4]==0x01){//Temperatura
         escribeDisplay1("page Temperatura");
      }
      
      if(Rx_Buffer[4]==0x02){//Humedad
         escribeDisplay1("page Humedad");
      }
      
      if(Rx_Buffer[4]==0x03){//Tiempo
         escribeDisplay1("page Tiempo");
      }
      
      if(Rx_Buffer[4]==0x04){//Test de componentes
         escribeDisplay1("page Clave");
         TipoClave=4;
      }
      
      if(Rx_Buffer[4]==0x05){//Cambio Clave
         escribeDisplay1("page Clave");
         TipoClave=2;
      }
      
      if(Rx_Buffer[4]==0x06){//Fecha y Hora
         escribeDisplay1("page Fecha");
         DiaTx=Dia;
         MesTx=Mes;
         YearTx=Year;
         HoraTx=Hora;
         MinutoTx=Minuto;
      }
      
      if(Rx_Buffer[4]==0x07){//Configuracion Parametros
         escribeDisplay1("page Clave");
         TipoClave=1;
      }
      
      if(Rx_Buffer[4]==0x0a){//Configuracion Parametros
         escribeDisplay1("page Rampas");
      }
      
      if(Finalizo==0){
            if(DOOR_Read()==0){
                if(Rx_Buffer[4]==0x08){//Inicia Control Temperatura
                    flagControlT=!flagControlT;
                    if(flagControlT)
                        inicioControlT=ON;
                    else
                        inicioControlT=OFF;
                    Rx_Buffer[4]=0x00;
                    Rx_Buffer2[4]=0x00;
                    Ganancia=GananciaR*(Setpoint/200);
                    if((mEstp==0)&&(hEstp==0))
                        Infinito=1;
                    else
                        Infinito=0;
                    EEPROM_WriteByte((uint8)flagControlT,41);CyDelay(20);
                }
              
                if(Rx_Buffer[4]==0x09){//Inicia Control Humedad
                    flagControlH=!flagControlH;
                    if(flagControlH)
                        inicioControlH=ON;
                    else
                        inicioControlH=OFF;
                    Rx_Buffer[4]=0x00;
                    Rx_Buffer2[4]=0x00;
                    if((mEstp==0)&&(hEstp==0))
                        Infinito=1;
                    else
                        Infinito=0;
                    EEPROM_WriteByte((uint8)flagControlH,42);CyDelay(20);
                }
                
                if(Rx_Buffer[4]==0x0b){//Inicia Control Rampa
                    if(Segmentos>0 && !inicioControlH && !inicioControlT){
                        flagRampa=!flagRampa;
                        if(flagRampa){
                            // Configura parametros para inicio de rampa  
                            SetpointAnt=Setpoint;
                            SetpointHAnt=SetpointH;
                            hEstpAnt=hEstp;
                            mEstpAnt=mEstp;
                            TempeInicial=Temperatura;
                            Tiempo=(rampas[Segmento].Horas*3600)+(rampas[Segmento].Minutos*60);         
                            Setpoint=rampas[Segmento].Temperatura;
                            SetpointH=rampas[Segmento].Humedad;
                            Pendiente=((Setpoint-TempeInicial)/Tiempo);
                            hEstp=rampas[Segmento].Horas;
                            mEstp=rampas[Segmento].Minutos;
                            flagControlH=ON;
                            flagControlT=ON;
                        }else{
                            Setpoint=SetpointAnt;
                            SetpointH=SetpointHAnt;
                            hEstp=hEstpAnt;
                            mEstp=mEstpAnt;
                            mEst=0;
                            hEst=0;
                            flagControlH=OFF;
                            flagControlT=OFF;
                        }
                    }
                    Rx_Buffer[4]=0x00;
                    Rx_Buffer2[4]=0x00;
                }
                if(flagRampa){
                    iniciaConteo(); 
                    escribeDisplay1("b4.pic=85");
                    escribeDisplay1("b5.pic=85");
                    escribeDisplay1("b6.pic=85");
                    if(LEVEL_Read()==1){
                        if(tllenado<tiempoLlenado){
                            escribeDisplay1("status.txt=\"Estado:Llenando\"");
                            CR7_Write(1);//Humidificador_off;
                            CR8_Write(1);//Bomba ON;
                            BUZZER_Write(1);
                            CyDelay(300);
                            BUZZER_Write(0);
                            CyDelay(300);
                        }else{
                            escribeDisplay1("status.txt=\"Estado:!Verifique Agua!\"");
                            escribeDisplay1("statusH.txt=\"Error Llenado\"");
                            CR7_Write(1);//Humidificador_off;
                            CR8_Write(0);//Bomba OFF;
                            BUZZER_Write(1);
                            CyDelay(800);
                            BUZZER_Write(0);
                            CyDelay(800);
                        }
                    }else{
                        escribeDisplay1("status.txt=\"Puerta Cerrada\"");  
                        CR8_Write(0);//Bomba OFF;
                        controlaHumedad();
                        tllenado=0;
                    }
                    if(TERM_Read()==1 || Temperatura>60){
                        escribeDisplay1("statusT.txt=\"!Termostato!\"");      
                        SSR_Write(0);
                        Control=0;
                        Ganancia=GananciaR*(Setpoint/200);
                        BUZZER_Write(1);
                        CyDelay(300);
                        BUZZER_Write(0);
                        CyDelay(300);
                    }else{
                        controlaRampa();
                    }
                    sprintf(displayStr,"t8.txt=\"Paso %02u\"",Segmento+1);
                    //printf("t8.txt=\"%2.0f\"",Tiempo);
                    escribeDisplay1(displayStr);
                }else{
                    escribeDisplay1("t8.txt=\"Info\"");
                    escribeDisplay1("b6.pic=84");
                    iniciaConteo();
                    
                    if(flagControlH){ 
                        if(LEVEL_Read()==1){
                            if(tllenado<tiempoLlenado){
                                escribeDisplay1("status.txt=\"Estado:Llenando\"");
                                CR7_Write(1);//Humidificador_off;
                                CR8_Write(1);//Bomba ON;
                                BUZZER_Write(1);
                                CyDelay(300);
                                BUZZER_Write(0);
                                CyDelay(300);
                            }else{
                                escribeDisplay1("status.txt=\"Estado:!Verifique Agua!\"");
                                escribeDisplay1("statusH.txt=\"Error Llenado\"");
                                CR7_Write(1);//Humidificador_off;
                                CR8_Write(0);//Bomba OFF;
                                BUZZER_Write(1);
                                CyDelay(800);
                                BUZZER_Write(0);
                                CyDelay(800);
                            }
                        }else{
                            escribeDisplay1("status.txt=\"Puerta Cerrada\"");  
                            CR8_Write(0);//Bomba OFF;
                            controlaHumedad();
                            tllenado=0;
                        }
                        escribeDisplay1("b5.pic=85");
                    }else{
                        escribeDisplay1("statusH.txt=\"Detenido\"");
                        escribeDisplay1("status.txt=\"Puerta Cerrada\"");  
                        escribeDisplay1("b5.pic=84");
                        CR7_Write(1);//Humidificador_off;
                        CR5_Write(0);//Dehumidificador_off;
                        CR8_Write(0);//Bomba OFF;
                    }
            
                    if(flagControlT){
                        escribeDisplay1("b4.pic=85");
                        controlaTemperatura();
                    }else{
                        escribeDisplay1("statusT.txt=\"Detenido\"");
                        escribeDisplay1("b4.pic=84");
                        SSR_Write(0);
                        Control=0;
                        Tpulso=0.0;
                        CR6_Write(1); // apaga Ventiladores de Celda de Peltier
                    }
                }
            }else if(DOOR_Read()==1){
               CR7_Write(1);//Humidificador_off;
               CR5_Write(0);//Dehumidificador_off;
               CR6_Write(1); // apaga Ventiladores de Celda de Peltier
               CR8_Write(0);//Bomba OFF;
               humidificador=OFF;
               thumidificador=0;
               dehumidificador=OFF;
               tdehumidificador=0;
               escribeDisplay1("b4.pic=84");
               escribeDisplay1("b5.pic=84");
               escribeDisplay1("b6.pic=84");
               Control=0;
               flag_tiempocalentamiento=0;
               tiempocalentamiento=0;
               Ganancia=GananciaR*(Setpoint/200);
               tiempo_esterilizacion=0;
               SSR_Write(0);
               Tpulso=0.0;
               flag_clear2=1;
               flag_ganancia=0;
               escribeDisplay1("status.txt=\"Puerta Abierta\"");
               escribeDisplay1("statusH.txt=\"-----\"");
               escribeDisplay1("statusT.txt=\"-----\"");
            }
      }else{
         CR7_Write(1);//Humidificador_off;
         CR5_Write(0);//Dehumidificador_off;
         CR1_Write(0);
         CR2_Write(0);
         CR3_Write(0);
         CR4_Write(0);
         CR6_Write(1);
         CR8_Write(0);//Bomba OFF;
         escribeDisplay1("t8.txt=\"Paso --\"");
         escribeDisplay1("status.txt=\"Reinicie el equipo\"");
         SSR_Write(0);
         Control=0;
         Tpulso=0.0;
         tiempo_esterilizacion=0;
         flagRampa=OFF;
         flagControlT=OFF;
         flagControlH=OFF;
         ApagaSalidas();
         escribeDisplay1("b4.pic=84");
         escribeDisplay1("b5.pic=84");
         escribeDisplay1("b6.pic=84");
         escribeDisplay1("statusT.txt=\"Finalizado\"");
         escribeDisplay1("statusH.txt=\"Finalizado\"");
      }
      
      if(Lectura==1)// Utilizado para visualizar la lectura de la temperatura
      {                   
         Lectura=0;
         Potencia=(100.0/Tciclo)*Tpulso; 
 //----------------Muestra Temperatura--------------------------
         if(rtdRes[0]>80000){
             sprintf(displayStr, "temperatura.txt=\"T1:%1.1f°C\"", Temperatura); 
         }else{
             sprintf(displayStr, "temperatura.txt=\"T1:-----\""); 
         }
         escribeDisplay1(displayStr);
        
         if(rtdRes[1]>80000){
             sprintf(displayStr, "temperatura2.txt=\"T2:%1.1f°C\"", Temperatura2); 
         }else{
             sprintf(displayStr, "temperatura2.txt=\"T2:-----\""); 
         }
         escribeDisplay1(displayStr);
        
         //printf("statusH.txt=\"%2.1f°C %2.1f%% %2.1f\"",Tam,RH2,sensores(3));
         //SendDataDisplay();
         sprintf(displayStr,"stemperatura.txt=\"SET:%1.1f°C\"",Setpoint);
         escribeDisplay1(displayStr);
         sprintf(displayStr,"shumedad.txt=\"SET:%03u%% \"",SetpointH);
         escribeDisplay1(displayStr);
         if(Humedad>0.0){
            sprintf(displayStr,"humedad.txt=\"HR:%03.1f%%\"",Humedad);
         }else{
            sprintf(displayStr,"humedad.txt=\"HR:----\"");
         }
         escribeDisplay1(displayStr);
         sprintf(displayStr,"stiempo.txt=\"SET:%02u:%02u (HH:MM)\"",hEstp,mEstp);
         escribeDisplay1(displayStr);
         sprintf(displayStr,"tiempo.txt=\"%02u:%02u \"",hEst,mEst);
         escribeDisplay1(displayStr);
         sprintf(displayStr,"potT.txt=\"Pot. T:%2.0f %%\"",Potencia);
         //printf("potT.txt=\"Pot:%2.0f%% %2.1f %2.4f\"",Potencia,TemperaturaIdeal,Pendiente);
         escribeDisplay1(displayStr);
      } 
      
      // Si cumplio el tiempo de esterilización, Muestre ciclo finalizado y detenga control de temperatura.
      if((mEstp>0)||(hEstp>0)){
         CuentaTiempo();  
         if(flagRampa){
            if((mEst>=mEstp)&&(hEst>=hEstp)&& Finalizo==0){
               flag_calcula=0;
               if(Segmento<Segmentos-1){
                  Segmento++;
                  hEst=0;
                  mEst=0;
               }else{
                  Finalizo=1;
               }
               for(i=0;i<2;i++){
                  BUZZER_Write(1);
                  CyDelay(1000);
                  BUZZER_Write(0);
                  CyDelay(1000);
               }
            }
         }else{
            if((mEst>=mEstp)&&(hEst>=hEstp)&& tiempo_esterilizacion==1){
               if(flag_clear==0){
                  tiempo_esterilizacion=0;
                  Pausa=2;
                  flag_clear=1;
                  SSR_Write(0);
                  Control=0;
                  Tpulso=0.0;
                  flag_ganancia=0;
                  Finalizo=1;
               }
               escribeDisplay1("status.txt=\"Finalizado\"");
               ApagaSalidas();
               for(i=0;i<2;i++){
                  BUZZER_Write(1);
                  CyDelay(1000);
                  BUZZER_Write(0);
                  CyDelay(1000);
               }
            }
         }
      }
    }
//_--------------------------------------------------------------------------------------------------------------------------------------------------------       
     
//_--------------------------------------------------------------------------------------------------------------------------------------------------------
     if(Menu==100){ //Menu de Configuración de Parametros de Control
     
      if(Opcion2>14)
         Opcion2=1;
      if(Opcion2<1)
         Opcion2=14;
      
      if(Opcion2==1){
         escribeDisplay1("Config.t2c.txt=\"Proporcional\"");      
         sprintf(displayStr,"t3c.txt=\"%u\"",(uint8)GananciaR);
         escribeDisplay1(displayStr);
         
         if(Rx_Buffer[4]==0x0a){//Tecla Arriba Oprimida
            CyDelay(20);
            if(Rx_Buffer[4]==0x0a){
               GananciaR+=1.0;
               Rx_Buffer[4]=0x00;  
               Rx_Buffer2[4]=0x00;            
            }
         }
            
         if(Rx_Buffer[4]==0x0b){//Tecla Abajo Oprimida
            CyDelay(20);
            if(Rx_Buffer[4]==0x0b){
               GananciaR-=1.0;
               Rx_Buffer[4]=0x00;
               Rx_Buffer2[4]=0x00;            
            }
         }
         
         if(GananciaR>20)
            GananciaR=1;
         if(GananciaR<1)
            GananciaR=20;
      }else if(Opcion2==2){
         escribeDisplay1("Config.t2c.txt=\"Derivativo\"");
         sprintf(displayStr,"t3c.txt=\"%u\"",Derivativo);
         escribeDisplay1(displayStr); 
        
         if(Rx_Buffer[4]==0x0a){//Tecla Arriba Oprimida
            CyDelay(20);
            if(Rx_Buffer[4]==0x0a){
               Derivativo++;
               Rx_Buffer[4]=0x00;  
               Rx_Buffer2[4]=0x00;                           
            }
         }
            
         if(Rx_Buffer[4]==0x0b){//Tecla Abajo Oprimida
            CyDelay(20);
            if(Rx_Buffer[4]==0x0b){
               Derivativo--;
               Rx_Buffer[4]=0x00;
               Rx_Buffer2[4]=0x00;                           
            }
         }
         
         if(Derivativo>80)
            Derivativo=1;
         if(Derivativo<1)
            Derivativo=80;
      }else if(Opcion2==3){
         escribeDisplay1("Config.t2c.txt=\"Integral\"");
         sprintf(displayStr,"t3c.txt=\"%u\"",Integral);
         escribeDisplay1(displayStr); 
         if(Rx_Buffer[4]==0x0a){//Tecla Arriba Oprimida
            CyDelay(20);
            if(Rx_Buffer[4]==0x0a){
               Integral++;
               Rx_Buffer[4]=0x00;  
               Rx_Buffer2[4]=0x00;            
            }
         }
            
         if(Rx_Buffer[4]==0x0b){//Tecla Abajo Oprimida
            CyDelay(20);
            if(Rx_Buffer[4]==0x0b){
               Integral--;
               Rx_Buffer[4]=0x00;
               Rx_Buffer2[4]=0x00;                           
            }
         }
         
         if(Integral>4)
            Integral=1;
         if(Integral<1)
            Integral=4;
      }else if(Opcion2==4){
         escribeDisplay1("Config.t2c.txt=\"Tciclo\"");
         sprintf(displayStr,"t3c.txt=\"%u\"",Tciclo);
         escribeDisplay1(displayStr); 
         if(Rx_Buffer[4]==0x0a){//Tecla Arriba Oprimida
            CyDelay(20);
            if(Rx_Buffer[4]==0x0a){
               Tciclo+=1.0;
               Rx_Buffer[4]=0x00;  
               Rx_Buffer2[4]=0x00;            
            }
         }
            
         if(Rx_Buffer[4]==0x0b){//Tecla Abajo Oprimida
            CyDelay(20);
            if(Rx_Buffer[4]==0x0b){
               Tciclo-=1.0;
               Rx_Buffer[4]=0x00;
               Rx_Buffer2[4]=0x00;            
            }
         }
         
         if(Tciclo>20)
            Tciclo=5;
         if(Tciclo<5)
            Tciclo=20;
      }else if(Opcion2==5){
         escribeDisplay1("Config.t2c.txt=\"Ajuste +\"");  
         sprintf(displayStr,"t3c.txt=\"%u\"",Ajuste);
         escribeDisplay1(displayStr); 
         if(Rx_Buffer[4]==0x0a){//Tecla Arriba Oprimida
            CyDelay(20);
            if(Rx_Buffer[4]==0x0a){
               if(Ajuste<100)
                  Ajuste++;
               Rx_Buffer[4]=0x00;  
               Rx_Buffer2[4]=0x00;            
            }
         }
            
         if(Rx_Buffer[4]==0x0b){//Tecla Abajo Oprimida
            CyDelay(20);
            if(Rx_Buffer[4]==0x0b){
               if(Ajuste>0)
                  Ajuste--;
               Rx_Buffer[4]=0x00;
               Rx_Buffer2[4]=0x00;            
            }
         }
      }else if(Opcion2==6){
         escribeDisplay1("Config.t2c.txt=\"Ajuste -\"");
         sprintf(displayStr,"t3c.txt=\"%u\"",Ajusten);
         escribeDisplay1(displayStr); 
         if(Rx_Buffer[4]==0x0a){//Tecla Arriba Oprimida
            CyDelay(20);
            if(Rx_Buffer[4]==0x0a){
               if(Ajusten<100)
                  Ajusten++;
               Rx_Buffer[4]=0x00;  
               Rx_Buffer2[4]=0x00;            
            }         
         }
            
         if(Rx_Buffer[4]==0x0b){//Tecla Abajo Oprimida
            CyDelay(20);
            if(Rx_Buffer[4]==0x0b){
               if(Ajusten>0)
                  Ajusten--;
               Rx_Buffer[4]=0x00;
               Rx_Buffer2[4]=0x00;            
            }
         }
      }else if(Opcion2==7){
         escribeDisplay1("Config.t2c.txt=\"Media\"");    
         sprintf(displayStr,"t3c.txt=\"%u\"",MediaMovil);
         escribeDisplay1(displayStr);
         if(Rx_Buffer[4]==0x0a){//Tecla Arriba Oprimida
            CyDelay(20);
            if(Rx_Buffer[4]==0x0a){
               if(MediaMovil<80)
                  MediaMovil++;
               Rx_Buffer[4]=0x00;  
               Rx_Buffer2[4]=0x00;            
            }
         }
            
         if(Rx_Buffer[4]==0x0b){//Tecla Abajo Oprimida
            CyDelay(20);
            if(Rx_Buffer[4]==0x0b){
               if(MediaMovil>10)
                  MediaMovil--;
               Rx_Buffer[4]=0x00;
               Rx_Buffer2[4]=0x00;            
            }            
         }
      }else if(Opcion2==8){
         escribeDisplay1("Config.t2c.txt=\"Media Hum\"");    
         sprintf(displayStr,"t3c.txt=\"%u\"",MediaMovil2);
         escribeDisplay1(displayStr);
         if(Rx_Buffer[4]==0x0a){//Tecla Arriba Oprimida
            CyDelay(20);
            if(Rx_Buffer[4]==0x0a){
               if(MediaMovil2<80)
                  MediaMovil2++;
               Rx_Buffer[4]=0x00;  
               Rx_Buffer2[4]=0x00;            
            }
         }
            
         if(Rx_Buffer[4]==0x0b){//Tecla Abajo Oprimida
            CyDelay(20);
            if(Rx_Buffer[4]==0x0b){
               if(MediaMovil2>10)
                  MediaMovil2--;
               Rx_Buffer[4]=0x00;
               Rx_Buffer2[4]=0x00;            
            }            
         }
      }else if(Opcion2==9){
         escribeDisplay1("Config.t2c.txt=\"Ajuste2 +\"");  
         sprintf(displayStr,"t3c.txt=\"%u\"",Ajuste2);
         escribeDisplay1(displayStr); 
         if(Rx_Buffer[4]==0x0a){//Tecla Arriba Oprimida
            CyDelay(20);
            if(Rx_Buffer[4]==0x0a){
               if(Ajuste2<100)
                  Ajuste2++;
               Rx_Buffer[4]=0x00;  
               Rx_Buffer2[4]=0x00;            
            }
         }
            
         if(Rx_Buffer[4]==0x0b){//Tecla Abajo Oprimida
            CyDelay(20);
            if(Rx_Buffer[4]==0x0b){
               if(Ajuste2>0)
                  Ajuste2--;
               Rx_Buffer[4]=0x00;
               Rx_Buffer2[4]=0x00;            
            }
         }
      }else if(Opcion2==10){
         escribeDisplay1("Config.t2c.txt=\"Ajuste2 -\"");
         sprintf(displayStr,"t3c.txt=\"%u\"",Ajusten2);
         escribeDisplay1(displayStr); 
         if(Rx_Buffer[4]==0x0a){//Tecla Arriba Oprimida
            CyDelay(20);
            if(Rx_Buffer[4]==0x0a){
               if(Ajusten2<100)
                  Ajusten2++;
               Rx_Buffer[4]=0x00;  
               Rx_Buffer2[4]=0x00;            
            }         
         }
            
         if(Rx_Buffer[4]==0x0b){//Tecla Abajo Oprimida
            CyDelay(20);
            if(Rx_Buffer[4]==0x0b){
               if(Ajusten2>0)
                  Ajusten2--;
               Rx_Buffer[4]=0x00;
               Rx_Buffer2[4]=0x00;            
            }
         }
      }else if(Opcion2==11){
         escribeDisplay1("Config.t2c.txt=\"Muestreo\"");
         sprintf(displayStr,"t3c.txt=\"%03u\"",tmuestreoUSB);
         escribeDisplay1(displayStr); 
         if(Rx_Buffer[4]==0x0a){//Tecla Arriba Oprimida
            CyDelay(20);
            if(Rx_Buffer[4]==0x0a){               
                  tmuestreoUSB++;
               Rx_Buffer[4]=0x00;  
               Rx_Buffer2[4]=0x00;            
            }         
         }
            
         if(Rx_Buffer[4]==0x0b){//Tecla Abajo Oprimida
            CyDelay(20);
            if(Rx_Buffer[4]==0x0b){
                  tmuestreoUSB--;
               Rx_Buffer[4]=0x00;
               Rx_Buffer2[4]=0x00;            
            }
         }
      }else if(Opcion2==12){
         escribeDisplay1("Config.t2c.txt=\"T1 Humedad\"");
         sprintf(displayStr,"t3c.txt=\"%02u\"",T1Humedad);
         escribeDisplay1(displayStr); 
         if(T1Humedad>60 || T1Humedad<4)
            T1Humedad=4;
         if(Rx_Buffer[4]==0x0a){//Tecla Arriba Oprimida
            CyDelay(20);
            if(Rx_Buffer[4]==0x0a){               
                  T1Humedad++;
               Rx_Buffer[4]=0x00;  
               Rx_Buffer2[4]=0x00;            
            }         
         }
            
         if(Rx_Buffer[4]==0x0b){//Tecla Abajo Oprimida
            CyDelay(20);
            if(Rx_Buffer[4]==0x0b){
                  T1Humedad--;
               Rx_Buffer[4]=0x00;
               Rx_Buffer2[4]=0x00;            
            }
         }
      }else if(Opcion2==13){
         escribeDisplay1("Config.t2c.txt=\"T2 Humedad\"");
         sprintf(displayStr,"t3c.txt=\"%02u\"",T2Humedad);
         escribeDisplay1(displayStr); 
         if(T2Humedad>60 || T2Humedad<4)
            T2Humedad=4;
         if(Rx_Buffer[4]==0x0a){//Tecla Arriba Oprimida
            CyDelay(20);
            if(Rx_Buffer[4]==0x0a){               
                  T2Humedad++;
               Rx_Buffer[4]=0x00;  
               Rx_Buffer2[4]=0x00;            
            }         
         }
            
         if(Rx_Buffer[4]==0x0b){//Tecla Abajo Oprimida
            CyDelay(20);
            if(Rx_Buffer[4]==0x0b){
                  T2Humedad--;
               Rx_Buffer[4]=0x00;
               Rx_Buffer2[4]=0x00;            
            }
         }
      } else if(Opcion2==14){
         escribeDisplay1("Config.t2c.txt=\"T3 Humedad\"");
         sprintf(displayStr,"t3c.txt=\"%02u\"",T3Humedad);
         escribeDisplay1(displayStr); 
         if(T3Humedad>60 || T3Humedad<4)
            T3Humedad=4;
         if(Rx_Buffer[4]==0x0a){//Tecla Arriba Oprimida
            CyDelay(20);
            if(Rx_Buffer[4]==0x0a){               
                  T3Humedad++;
               Rx_Buffer[4]=0x00;  
               Rx_Buffer2[4]=0x00;            
            }         
         }
            
         if(Rx_Buffer[4]==0x0b){//Tecla Abajo Oprimida
            CyDelay(20);
            if(Rx_Buffer[4]==0x0b){
                  T3Humedad--;
               Rx_Buffer[4]=0x00;
               Rx_Buffer2[4]=0x00;            
            }
         }
      }         
      
      if(Rx_Buffer[4]==0x0c){//Tecla Izquierda Oprimida
         CyDelay(20);
         if(Rx_Buffer[4]==0x0c){
            Opcion2++;
            Rx_Buffer[4]=0x00;  
            Rx_Buffer2[4]=0x00;            
         }
      }
         
      if(Rx_Buffer[4]==0x0d){//Tecla Derecha Oprimida
         CyDelay(20);
         if(Rx_Buffer[4]==0x0d){
            Opcion2--;
            Rx_Buffer[4]=0x00;
            Rx_Buffer2[4]=0x00;            
         }
      }
      
      if(Rx_Buffer[4]==0x11){//Tecla Regresar
         CyDelay(20);
         if(Rx_Buffer[4]==0x11){
            if(!GuardaEEPROM){
               EEPROM_WriteByte(Integral,32);
               CyDelay(10);
               EEPROM_WriteByte(Derivativo,31);
               CyDelay(10);
               EEPROM_WriteByte((uint8)GananciaR,30);
               CyDelay(10);
               EEPROM_WriteByte(Ajuste,34);
               CyDelay(10);
               EEPROM_WriteByte(Ajusten,35);
               CyDelay(10);
               EEPROM_WriteByte(MediaMovil,36);
               CyDelay(10);
               EEPROM_WriteByte(Tciclo,33);
               CyDelay(10);
               EEPROM_WriteByte(Ajuste2,37);
               CyDelay(10);
               EEPROM_WriteByte(Ajusten2,38);
               CyDelay(10);
               EEPROM_WriteByte(MediaMovil2,39);
               CyDelay(10);
               Ganancia=GananciaR*(Setpoint/200);
               EEPROM_WriteByte(tmuestreoUSB,40);
               CyDelay(10);
               EEPROM_WriteByte(T1Humedad,44);
               CyDelay(10);
               EEPROM_WriteByte(T2Humedad,45);
               CyDelay(10);
               EEPROM_WriteByte(T3Humedad,46);
               CyDelay(10);
               GuardaEEPROM=ON;
            }
            escribeDisplay1("page Principal");
         }
      }
    }
//_--------------------------------------------------------------------------------------------------------------------------------------------------------           
    
    }
}
