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
#include "funciones.h"
#include "removeOffsetNoise.h"
#include "getResistance.h"
#include "MyServer.h"
#include <stdlib.h>
#include <stdio.h>


int8 Regular[12]={0,3,3,6,1,4,6,2,5,0,3,5};
int8 Bisiesto[12]={0,3,4,0,2,5,0,3,6,1,4,6};

int8 prueba[80];
short Humidificar=0,flagHumedad=0,humidificador=0,desconectoTemperatura=OFF,desconectoHumedad=OFF,flagSobrepasoHumedad=OFF;
uint8 f,T1Humedad,T2Humedad,T3Humedad,T1,T2,T3,tDisplay=0;
uint16 tdehumidificador,thumidificador,tllenado,data=0,data2=0,datacount=0,pwm2=0,tlevel=0,tlevelH=0,tDefrost=0,tCongela=0,tSobrepaso=0,tDisplayHumedad=0;
float RH2,Tam,Humedad,humedadDisplay=0.0,minH=0.0,maxH=0.0;
float promedioH[80];
float promedioTa[80];

int16 offset;
int16 coldJnTemp;
int32 tcColdJnuVolt;
int32 hotJnCount;
int32 filHotJnCount;
int32 tcHotJnuVolt;
int32 tcuVolt;
int32 tcTemp;
char displayStr[100];	
char Str[100];
char Str2[100];
uint8 first = 0;  
uint8_t i,n,PantallaPrincipal,Programa,Opcion2,Derivativo,Integral,MediaMovil,MediaMovil2,Tciclo,Ajuste,Ajusten,Ajuste2,Ajusten2,Modulo,Ajusto;
uint16 ctr,SetpointH,SetpointHAnt,derivativo_ant,contador;
float Setpoint,SetpointAnt;
uint32_t output,tGrafica;
short flagButton0,flagButton1,flagButton2,flagButton3,dibujo,pulsoAumentar,pulsoDisminuir,flagBorrado,estadoSSR,estadoBuzzer,estadoCR1,flagFinalizo;
short estadoCR2,estadoCR3,inicioControlT,inicioControlH;
float valor,pendiente,Ganancia,GananciaIn,vacio,pwmPeltier;
int32  Temperature2;

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
uint16 pwm1=0;
uint8 Dato2,Dato3,Dato_Exitoso,Dato_Exitoso2,Menu,MenuAntt,MenuAnt,Menu2,MenuAntt2,z,TipoClave,Segmentos,CicloLibre,Opcion,h,l;
uint8 Dia,Mes,Year,Hora,Minuto,Segundo,dow;
short Inicio,Inicio2,EstadoPaso,entro,GuardaEEPROM,rampa,Minutos,Horas,flagTemperatura,x1,x10,x100,Succion,flagVacio,Enfria,flagVelocidad,envioUSB,flagRed=0;
uint8 Password[4];
uint8 tiempo_esterilizacion,y,hEst,mEst,hEstp,mEstp,hEstpAnt,mEstpAnt,Segmento,conteo;
uint16 tiemporeset,TiempoControl,tiemporecta,tiemposetpoint,tinicio,tiempocalentamiento,tiempoup,tiempodn;
short mensaje,Lectura2,flag_setpoint,flag_tiempocalentamiento,Lectura,flagup,flagdn,flagRampa,flag_calcula,flagusb,dehumidificador,flag_ajuste,flag_ajuste2;
short Finalizo,conteoPregunta,grafica,imprime,Infinito,flagControlT,flagControlH,Controlando,flag_clear2,flag_ganancia,flag_clear,Pausa;
float X[2]={0.0,0.0};
float Temperatura,Temperatura2,Aumento,PendienteActual,DifPendiente,TempeInicial,Pendiente,TemperaturaIdeal,PotenciaEntregada,Tpulso,penInicial,penMufla=1.0;
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
uint8 l2,h2,k,kk,tEnvioUSB,tmuestreoUSB,tmuestreoUSB_ant,controlH,controlT,controlR;
float promedio2[20];

float promedioT1[90];
float promedioT2[90];
float promediolm35[20];

uint8 extraeDecimal(float data){
   float entero=0;
   float tmp=0.0;
   
   entero=(int8)data;
   tmp=(data-entero)*10;
   return (uint8)tmp;
}

float extraeDecimal2(float data){
   float entero=0;
   float tmp=0.0;
   
   entero=(int8)data;
   tmp=(data-entero);
   return tmp;
}

float inttofloat(uint8 decimal){
   float tmp;
   tmp=decimal;
   return tmp/10;
}

uint8 convert8(uint16 data,uint8 nible){
    uint8 dato=0;
    if(nible==0){
        dato=(uint8)data;   
    }else if(nible==1){
        dato=data>>8;
    }
    return dato;
}

uint16 convert16(uint8 nibleh,uint8 niblel){
    uint16 temp,var;
    temp=(uint16)nibleh;
    var=temp<<8 | (uint16)niblel;
    return var;
}

void LeeEEPROM(){
   mEstp=EEPROM_ReadByte(0);CyDelay(5);  
   hEstp=EEPROM_ReadByte(1);CyDelay(5);  
   
   entero=EEPROM_ReadByte(2);CyDelay(5);  
   Temporal=EEPROM_ReadByte(3);CyDelay(5);  
   Temporal=Temporal/10.0;
   Setpoint=entero+Temporal;  
   
   SetpointH=EEPROM_ReadByte(4);
   CyDelay(5);   

   tDefrost=convert16(EEPROM_ReadByte(6),EEPROM_ReadByte(5));
   CyDelay(5);   
   tCongela=convert16(EEPROM_ReadByte(8),EEPROM_ReadByte(7));
   CyDelay(5);   
   tDisplay=EEPROM_ReadByte(9);CyDelay(5);  
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
   IP[0]=EEPROM_ReadByte(47);CyDelay(5);
   IP[1]=EEPROM_ReadByte(48);CyDelay(5);
   IP[2]=EEPROM_ReadByte(49);CyDelay(5);
   IP[3]=EEPROM_ReadByte(50);CyDelay(5);
   Gateway[0]=EEPROM_ReadByte(51);CyDelay(5);
   Gateway[1]=EEPROM_ReadByte(52);CyDelay(5);
   Gateway[2]=EEPROM_ReadByte(53);CyDelay(5);
   Gateway[3]=EEPROM_ReadByte(54);CyDelay(5);
   Subnet[0]=EEPROM_ReadByte(55);CyDelay(5);
   Subnet[1]=EEPROM_ReadByte(56);CyDelay(5);
   Subnet[2]=EEPROM_ReadByte(57);CyDelay(5);
   Subnet[3]=EEPROM_ReadByte(58);CyDelay(5);
   MAC[0]=EEPROM_ReadByte(59);CyDelay(5);
   MAC[1]=EEPROM_ReadByte(60);CyDelay(5);
   MAC[2]=EEPROM_ReadByte(61);CyDelay(5);
   MAC[3]=EEPROM_ReadByte(62);CyDelay(5);
   MAC[4]=EEPROM_ReadByte(63);CyDelay(5);
   //MAC[5]=EEPROM_ReadByte(64);CyDelay(5);
    for(i=0;i<3;i++){
        calib[i].patron1=EEPROM_ReadByte(64+(i*12))+inttofloat(EEPROM_ReadByte(65+(i*12)));CyDelay(5);
        calib[i].patron2=EEPROM_ReadByte(66+(i*12))+inttofloat(EEPROM_ReadByte(67+(i*12)));CyDelay(5);
        calib[i].patron3=EEPROM_ReadByte(68+(i*12))+inttofloat(EEPROM_ReadByte(69+(i*12)));CyDelay(5);
        calib[i].control1=EEPROM_ReadByte(70+(i*12))+inttofloat(EEPROM_ReadByte(71+(i*12)));CyDelay(5);
        calib[i].control2=EEPROM_ReadByte(72+(i*12))+inttofloat(EEPROM_ReadByte(73+(i*12)));CyDelay(5);
        calib[i].control3=EEPROM_ReadByte(74+(i*12))+inttofloat(EEPROM_ReadByte(75+(i*12)));CyDelay(5);
    }   
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
   if(Setpoint>60.0 || Setpoint<0.0){
      Setpoint=30.0;EEPROM_WriteByte(100,2);CyDelay(20);EEPROM_WriteByte(0,3);CyDelay(20);
   }
   if(SetpointH>95 || SetpointH<10){
      SetpointH=60.0;EEPROM_WriteByte(60,4);CyDelay(20);
   }
   if(tDefrost>120){
      tDefrost=0;EEPROM_WriteByte(0,5);CyDelay(20);EEPROM_WriteByte(0,6);CyDelay(20);
   }
   if(tCongela>120 || tCongela<1){
      tCongela=5;EEPROM_WriteByte(5,7);CyDelay(20);EEPROM_WriteByte(0,8);CyDelay(20);
   }
   if(tDisplay>60 || tDisplay<5){
      tDisplay=10;EEPROM_WriteByte(10,9);CyDelay(20);
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
   if(tmuestreoUSB<1){
      tmuestreoUSB=20;EEPROM_WriteByte(20,40);CyDelay(20);
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
   if(T1Humedad>60 || T1Humedad<1){
      T1Humedad=10;EEPROM_WriteByte(10,44);CyDelay(20);
   }
   if(T2Humedad>60 || T2Humedad<4){
      T2Humedad=30;EEPROM_WriteByte(30,45);CyDelay(20);
   }
   if(T3Humedad>60 || T3Humedad<1){
      T3Humedad=6;EEPROM_WriteByte(6,46);CyDelay(20);
   }
    for(i=0;i<3;i++){
        if(i<2){
            if(calib[i].patron1>25 || calib[i].patron1<1){
                calib[i].patron1=10;EEPROM_WriteByte(10,64+(i*12));CyDelay(20);EEPROM_WriteByte(0,65+(i*12));CyDelay(20);
            }
            if(calib[i].patron2>40 || calib[i].patron2<25){
                calib[i].patron2=30;EEPROM_WriteByte(30,66+(i*12));CyDelay(20);EEPROM_WriteByte(0,67+(i*12));CyDelay(20);
            }
            if(calib[i].patron3>80 || calib[i].patron3<40){
                calib[i].patron3=40;EEPROM_WriteByte(40,68+(i*12));CyDelay(20);EEPROM_WriteByte(0,69+(i*12));CyDelay(20);
            }
            if(calib[i].control1>25 || calib[i].control1<1){
                calib[i].control1=10;EEPROM_WriteByte(10,70+(i*12));CyDelay(20);EEPROM_WriteByte(0,71+(i*12));CyDelay(20);
            }
            if(calib[i].control2>40 || calib[i].control2<25){
                calib[i].control2=30;EEPROM_WriteByte(30,72+(i*12));CyDelay(20);EEPROM_WriteByte(0,73+(i*12));CyDelay(20);
            }
            if(calib[i].control3>80 || calib[i].control3<40){
                calib[i].control3=40;EEPROM_WriteByte(40,74+(i*12));CyDelay(20);EEPROM_WriteByte(0,75+(i*12));CyDelay(20);
            }
        }else{
            if(calib[i].patron1>100 || calib[i].patron1<10){
                calib[i].patron1=10;EEPROM_WriteByte(10,64+(i*12));CyDelay(20);EEPROM_WriteByte(0,65+(i*12));CyDelay(20);
            }
            if(calib[i].patron2>100 || calib[i].patron2<1){
                calib[i].patron2=40;EEPROM_WriteByte(40,66+(i*12));CyDelay(20);EEPROM_WriteByte(0,67+(i*12));CyDelay(20);
            }
            if(calib[i].patron3>100 || calib[i].patron3<1){
                calib[i].patron3=80;EEPROM_WriteByte(80,68+(i*12));CyDelay(20);EEPROM_WriteByte(0,69+(i*12));CyDelay(20);
            }
            if(calib[i].control1>100 || calib[i].control1<1){
                calib[i].control1=10;EEPROM_WriteByte(10,70+(i*12));CyDelay(20);EEPROM_WriteByte(0,71+(i*12));CyDelay(20);
            }
            if(calib[i].control2>100 || calib[i].control2<1){
                calib[i].control2=40;EEPROM_WriteByte(40,72+(i*12));CyDelay(20);EEPROM_WriteByte(0,73+(i*12));CyDelay(20);
            }
            if(calib[i].control3>100 || calib[i].control3<1){
                calib[i].control3=80;EEPROM_WriteByte(80,74+(i*12));CyDelay(20);EEPROM_WriteByte(0,75+(i*12));CyDelay(20);
            }   
        }
        calib[i].m1=(calib[i].control2-calib[i].control1)/(calib[i].patron2-calib[i].patron1);
        calib[i].b1=calib[i].control2-(calib[i].m1*calib[i].patron2);
        calib[i].m2=(calib[i].control3-calib[i].control2)/(calib[i].patron3-calib[i].patron2);
        calib[i].b2=calib[i].control3-(calib[i].m2*calib[i].patron3);
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
      Tpulso=(PotenciaEntregada*Tciclo)/100;  
      if(Enfria==1){
         pwm1=(uint16)(((1/(float)Tciclo)*Tpulso)*60000);
         PWM_WriteCompare1(0);
         PWM_WriteCompare2(pwm1);
      }else{
         pwm1=(uint16)(((1/(float)Tciclo)*Tpulso)*60000);
         PWM_WriteCompare1(pwm1);
         PWM_WriteCompare2(0);
      }
   }else{
               
      if(Enfria==1){//Control Enfriamiento
         //CR1_Write(0);
         //CR2_Write(0);
         //CR3_Write(0);
         //CR4_Write(0);
         error=Setpoint-(desvio-0.2)-Temperatura;      // Cálculo del error ******
         
         if(error<-0.4 && Aumento>-0.2){
            if(Potencia<90.0){
               if(TiempoControl>=Derivativo*2){
                  TiempoControl=0;
                  if(Aumento>0.2){
                     Ganancia+=Integral;
                  }else{
                     if(Aumento>0.0){
                        Ganancia+=Integral*0.5;//1
                     }else{
                        Ganancia+=Integral*0.1;//1
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
         pwm1=(uint16)(((1/(float)Tciclo)*Tpulso)*60000);
         PWM_WriteCompare1(0);
         PWM_WriteCompare2(pwm1);
         //Tp=(Tpulso/0.005);
         //Tc=(Tciclo/0.005);
         Controlando=1;       
      }else{//Control Calentamiento
         //CR1_Write(1);
         //CR2_Write(1);
         //CR3_Write(1);
         //CR4_Write(1);
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
            if(Potencia<80.0){
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
            if(error<=desvio && error>0){   
                if(flag_ajuste==0){
                    TpulsoMax=Tpulso;
                    flag_ajuste=1;  
                    Ajusto=5;
                }
            }
         
            if(error<=desvio-0.1 && error>0){   
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
         pwm1=(uint16)(((1/(float)Tciclo)*Tpulso)*60000);
         PWM_WriteCompare2(0);
         PWM_WriteCompare1(pwm1);
      }   
   }
   //Tp=(Tpulso/0.002);
   //Tc=(Tciclo/0.002);
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

void escribeDisplay2(char Buffer[30]){
    UART_2_PutString(Buffer);
    CyDelayUs(10);
    UART_2_PutChar(0xFF);
    CyDelayUs(10);
    UART_2_PutChar(0xFF);
    CyDelayUs(10);
    UART_2_PutChar(0xFF);
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
      }else if(Rx_Buffer3[3]==0x0b){//0b es Red
         Menu2=10;
         if(MenuAntt2!=Menu2)
            MenuAntt2=Menu2;
      }else if(Rx_Buffer3[3]==0x0c){//0c es Calibracion Temperatura
         Menu2=11;
         if(MenuAntt2!=Menu2)
            MenuAntt2=Menu2;
      }else if(Rx_Buffer3[3]==0x0d){//0c es Calibracion Humedad
         Menu2=12;
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
         //SSR_Write(0);
         PWM_WriteCompare1(0);
         PWM_WriteCompare2(0);
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
         //SSR_Write(0);
        PWM_WriteCompare1(0);
         PWM_WriteCompare2(0);
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
    float Tref;
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
    Prom=(Prom/media)+ajp-ajn;   
    
    if(Prom<=calib[0].control2){
        Tref=(Prom-calib[0].b1)/calib[0].m1;
    }else{
        Tref=(Prom-calib[0].b2)/calib[0].m2;
    }
    return Tref;
}

float leePt1002(uint8 media2){
    float tmp3,ajp2,ajn2;
    float Tref;
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
    Prom2=(Prom2/media2)+ajp2-ajn2;   
    
    if(Prom2<=calib[1].control2){
        Tref=(Prom2-calib[1].b1)/calib[1].m1;
    }else{
        Tref=(Prom2-calib[1].b2)/calib[1].m2;
    }
    return Tref;
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
    float Href;
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
   //if(RH>100.0)
        //RH=100.0;
   //RH =((-0.0000000019206*pow(VH,3))+( 0.000014370*pow(VH,2))+( 0.0034210*VH)-12.4)/(1+(Temperatura-23)*( 0.0024));
   RH2 = 0.0375 * VH - 37.7; //Lineal sin compensacion de Temperatura
   if(n>(media-1))
      n=0;
   promedioH[n]=RH;n++;
         
   for(h=0;h<media;h++)
      PromH+=promedioH[h]; 
   PromH=PromH/media;
    
    if(PromH<=calib[2].control2){
        Href=(PromH-calib[2].b1)/calib[2].m1;
    }else{
        Href=(PromH-calib[2].b2)/calib[2].m2;
    }
   return Href;
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
    //SSR_Write(0);
    PWM_WriteCompare1(0);
    PWM_WriteCompare2(0);
}

void controlaHumedad(){
   if(Humidificar){
      CR4_Write(1);
      dehumidificador=OFF;
      tdehumidificador=0;
      if(Humedad<SetpointH-5.0){
         flagSobrepasoHumedad=OFF;
         tSobrepaso=0;
         escribeDisplay1("statusH.txt=\"Humidificando\"");
         humidificador=ON;
         CR5_Write(0);//Dehumidificador_off;
         if(LEVEL_Read()==0){//Si detecta el nivel de agua en el humidificador
            tllenado=0;
            if(Humedad<SetpointH-10.0){
                CR7_Write(0);//Humidificador_on;
            }else{
                if(thumidificador<=T1){
                    CR7_Write(0);//Humidificador_on;
                }else if(thumidificador>T1&& thumidificador<T2Humedad){
                    CR7_Write(1);//Humidificador_off;    
                }else if(thumidificador>T2Humedad){
                    thumidificador=0;   
                }
            }
         }else{
            CR7_Write(1);//Humidificador_off;
        }
        if(Humedad<SetpointH-10.0)
            flagHumedad=OFF;
      }else if(Humedad<SetpointH-0.3 && Humedad>SetpointH-4.8){
         flagSobrepasoHumedad=OFF;
         tSobrepaso=0;
         escribeDisplay1("statusH.txt=\"Controlando\"");
         humidificador=ON;
         CR5_Write(0);//Dehumidificador_off;
         if(LEVEL_Read()==0){//Si detecta el nivel de agua en el humidificador
            tllenado=0;
            //CR7_Write(0);//Humidificador_on;
            if(thumidificador<=T3){
                CR7_Write(0);//Humidificador_on;
            }else if(thumidificador>T3&& thumidificador<T2Humedad){
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
         flagSobrepasoHumedad=OFF;
         tSobrepaso=0;
         CR7_Write(1);//Humidificador_off;
         CR5_Write(0);//Dehumidificador_off;
         humidificador=OFF;
         thumidificador=0;
         if(!Infinito)   
            flagHumedad=ON;
         else
            flagHumedad=OFF;
      }else if(Humedad>SetpointH+3.0){
         flagSobrepasoHumedad=ON;
         if(tSobrepaso>300)
            Humidificar=OFF; 
      }
   }else{
        CR4_Write(0);
        humidificador=OFF;
        thumidificador=0;
      if(Humedad>SetpointH+2.0){
         escribeDisplay1("statusH.txt=\"Deshumidificando\"");
         dehumidificador=ON;
         if(tdehumidificador<(tCongela*60)){
            CR5_Write(1);//Dehumidificador_on;
         }else if(tdehumidificador>=(tCongela*60) && tdehumidificador<((tDefrost*60)+(tCongela*60))){
            CR5_Write(0);//Dehumidificador_off;
         }else if(tdehumidificador>=((tDefrost*60)+(tCongela*60))){
            tdehumidificador=0;
         }
         CR7_Write(1);//Humidificador_off;
         if(Humedad>SetpointH+10.0)
            flagHumedad=OFF;
      }else if(Humedad>SetpointH && Humedad<SetpointH+2.8){
         escribeDisplay1("statusH.txt=\"Controlando\"");
         if(tdehumidificador<(tCongela*60)){
            CR5_Write(1);//Dehumidificador_on;
         }else if(tdehumidificador>=(tCongela*60) && tdehumidificador<((tCongela*60)+(tDefrost*60))){
            CR5_Write(0);//Dehumidificador_off;
         }else if(tdehumidificador>=((tCongela*60)+(tDefrost*60))){
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

void controlaHumedad2(){
   if(Humidificar){
      CR4_Write(1);
      dehumidificador=OFF;
      tdehumidificador=0;
      if(Humedad<SetpointH-5.0){
         flagSobrepasoHumedad=OFF;
         tSobrepaso=0;
         escribeDisplay1("statusH.txt=\"Humidificando\"");
         humidificador=ON;
         CR5_Write(0);//Dehumidificador_off;
         if(LEVEL_Read()==0){//Si detecta el nivel de agua en el humidificador
            tllenado=0;
            if(Humedad<SetpointH-10.0){
                CR7_Write(0);//Humidificador_on;
                thumidificador=T2Humedad+1;
            }else{
                if(thumidificador<=T1){
                    CR7_Write(0);//Humidificador_on;
                }else if(thumidificador>T1&& thumidificador<T2Humedad){
                    CR7_Write(1);//Humidificador_off;    
                }else if(thumidificador>T2Humedad){
                    thumidificador=0;   
                }
            }
         }else{
            CR7_Write(1);//Humidificador_off;
        }
        if(Humedad<SetpointH-10.0)
            flagHumedad=OFF;
      }else if(Humedad<SetpointH+2.0 && Humedad>SetpointH-5.0){
         flagSobrepasoHumedad=OFF;
         tSobrepaso=0;
         escribeDisplay1("statusH.txt=\"Controlando\"");
         humidificador=ON;
         CR5_Write(0);//Dehumidificador_off;
         if(LEVEL_Read()==0){//Si detecta el nivel de agua en el humidificador
            tllenado=0;
            //CR7_Write(0);//Humidificador_on;
            if(thumidificador<=T3){
                CR7_Write(0);//Humidificador_on;
            }else if(thumidificador>T3&& thumidificador<T2Humedad){
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
      }else if(Humedad>SetpointH+2.0 && Humedad<=SetpointH+3.0){
         escribeDisplay1("statusH.txt=\"Controlando\"");
         flagSobrepasoHumedad=OFF;
         tSobrepaso=0;
         CR7_Write(1);//Humidificador_off;
         CR5_Write(0);//Dehumidificador_off;
         humidificador=OFF;
         thumidificador=T2Humedad+1;
         if(!Infinito)   
            flagHumedad=ON;
         else
            flagHumedad=OFF;
      }else if(Humedad>SetpointH+3.0){
         flagSobrepasoHumedad=ON;
         CR7_Write(1);//Humidificador_off;
         if(tSobrepaso>300)
            Humidificar=OFF; 
      }
   }else{
        CR4_Write(0);
        humidificador=OFF;
        thumidificador=0;
      if(Humedad>SetpointH+2.0){
         escribeDisplay1("statusH.txt=\"Deshumidificando\"");
         dehumidificador=ON;
         if(tdehumidificador<(tCongela*60)){
            CR5_Write(1);//Dehumidificador_on;
         }else if(tdehumidificador>=(tCongela*60) && tdehumidificador<((tDefrost*60)+(tCongela*60))){
            CR5_Write(0);//Dehumidificador_off;
         }else if(tdehumidificador>=((tDefrost*60)+(tCongela*60))){
            tdehumidificador=0;
         }
         CR7_Write(1);//Humidificador_off;
         if(Humedad>SetpointH+10.0)
            flagHumedad=OFF;
      }else if(Humedad>SetpointH && Humedad<SetpointH+2.8){
         escribeDisplay1("statusH.txt=\"Controlando\"");
         if(tdehumidificador<(tCongela*60)){
            CR5_Write(1);//Dehumidificador_on;
         }else if(tdehumidificador>=(tCongela*60) && tdehumidificador<((tCongela*60)+(tDefrost*60))){
            CR5_Write(0);//Dehumidificador_off;
         }else if(tdehumidificador>=((tCongela*60)+(tDefrost*60))){
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
    if(TERM_Read()==1 || Temperatura>80){
      escribeDisplay1("statusT.txt=\"!Termostato!\"");      
      //SSR_Write(0);
      PWM_WriteCompare1(0);
      PWM_WriteCompare2(0);
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
                //SSR_Write(0);
                PWM_WriteCompare1(0);
                PWM_WriteCompare2(0);
                Control=0;
            }else if(Temperatura>Setpoint+0.6){
                flag_tiempocalentamiento=0;
                escribeDisplay1("statusT.txt=\"Enfriando\"");
                //tiempo_esterilizacion=0;
                flagTemperatura=OFF;
                Controla();
            }
        }else{  
            if(Temperatura>=Setpoint-1.0 && Temperatura<=Setpoint+0.5){
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
                if(Temperatura>Setpoint+0.8){
                    Enfria=1;
                    Ganancia=GananciaR*(Setpoint/200);
                }
                //SSR_Write(0);
                PWM_WriteCompare1(0);
                PWM_WriteCompare2(0);
                Control=0;
            }else if(Temperatura<Setpoint-1.0){
                flag_tiempocalentamiento=1;
                escribeDisplay1("statusT.txt=\"Calentando\"");
                //tiempo_esterilizacion=0;
                flagTemperatura=OFF;
                Controla();
            }
        }
    }
}

void actualizaEjeX(){
    for(i=0;i<20;i++){//for dedicado a actualizar el label del eje x
        horaGrafica[i]=graph[0].Hora+((uint16)(tmuestreoUSB*40*(uint16)i)/3600);
        minutoGrafica[i]=graph[0].Minuto+(uint8)(60.0*extraeDecimal2(((float)tmuestreoUSB*40.0*(float)i)/3600.0));
        if(horaGrafica[i]>=24)
            horaGrafica[i]=horaGrafica[i]-24;
        if(minutoGrafica[i]>=60){
            minutoGrafica[i]=minutoGrafica[i]-60;
            horaGrafica[i]++;
        }
        sprintf(displayStr,"t%u.txt=\"%02u:%02u\"",i+11,horaGrafica[i],minutoGrafica[i]); 
        escribeDisplay2(displayStr);
     }   
}

void borraVector(){
    for(data2=0;data2<800;data2++){
        graph[data2].Temperatura=0.0;   
        graph[data2].Humedad=0.0;
        graph[data2].Hora=0.0;
        graph[data2].Minuto=0.0;
    }
    data=0;
    datacount=0;
    graph[0].Minuto=Minuto;
    graph[0].Hora=Hora;
}

void dataUSB(){
    //USB----------------------------------------------------------------
        /* Wait for Device to enumerate */
        if(USBUART_GetConfiguration()){
            if(!flagusb){
                USBUART_CDC_Init();
                flagusb=ON;
            }
        }
        
        if(envioUSB){
            envioUSB=0;
            tDisplayHumedad++;
            if(tDisplayHumedad>=tDisplay){
                humedadDisplay=(minH+maxH)/2;  
                minH=Humedad;
                maxH=Humedad;
                tDisplayHumedad=0;
            }else{
                if(Humedad<minH)
                    minH=Humedad;
                if(Humedad>maxH)
                    maxH=Humedad;
            }
            tEnvioUSB++;
            if(tEnvioUSB>=tmuestreoUSB){
                tEnvioUSB=0;
                grafica=ON;
                graph[data].Temperatura=Temperatura;
                //graph[data].Humedad=Humedad;
                graph[data].Humedad=humedadDisplay;
                graph[data].Hora=Hora;
                graph[data].Minuto=Minuto;
                if(data<760)
                    data++;
                else{
                    if(datacount>=40){
                        actualizaNumeracion=ON;
                        datacount=0;
                    }
                    datacount++;
                    for(data2=0;data2<760;data2++){
                        graph[data2].Temperatura=graph[data2+1].Temperatura;
                        graph[data2].Humedad=graph[data2+1].Humedad;
                        graph[data2].Hora=graph[data2+1].Hora;
                        graph[data2].Minuto=graph[data2+1].Minuto;
                    }
                }
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
//USB----------------------------------------------------------------        
}    
void dataEthernet(){
//Etherneth----------------------------------------------------------------
        socket = ETH0_TcpOpen( 80 );
        if(socket!=0xff){
            ETH0_TcpStartServer( socket );
            // Start a server listening for connection on an open socket 
            for(contador=0;contador<20000;contador++){ 
                if( ETH0_TcpConnected( socket ) ) {
                    CyDelay(50);
                    break;
                }
            }  	               
            bytes = ETH0_SocketRxDataWaiting(socket);		
    
            memset(webbuff, 0,  2000);
            ETH0_TcpReceive(socket,(uint8*)&webbuff[0],bytes);	// put the request from the browser to buffer
    
            webif_data(socket, webbuff );						// Processing browser  requests
            
            ETH0_TcpDisconnect( socket );
            ETH0_SocketClose( socket );
        }
//Etherneth----------------------------------------------------------------      
}
/* [] END OF FILE */
