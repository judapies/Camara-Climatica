
/* ========================================
 *
 * Copyright JP Bioingenieria SAS, 2021
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * Para calibrar la temperatura de la mufla de acuerdo a un patron de temperatura se debe realizar lo siguiente:
 * Se mide con el patron la temperatura en dos puntos y se registran las diferencias con respecto al control
 * Se dibuja una grafica XY, donde en el eje Y este la temperatura del control y en el eje X la temperatura del patron
 * Se traza la recta con los valores obtenidos y se calcula la pendiente m y el punto de corte b, estos valores se añadiran
 * a  la lectura de la termocupla de acuerdo a la siguiente ecuacion: T=(Tcontrol-b)/m;
 *
 * Este programa activa el extractor de una campana externa. El extractor estara activado durante toda la prueba una vez se encuentre en el menu de
 * funcionamiento (Menu==20), una vez termine todas las rampas, comenzara a correr el tiempo configurado, este tiempo esta en Horas y minutos, una
 * vez acabe el tiempo apagara automaticamente el extractor.
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
#include <24512.h>
#include "MyServer.h"
#include "funciones.h"

#if defined (__GNUC__)
    /* Add an explicit reference to the floating point printf library */
    /* to allow the usage of floating point conversion specifiers. */
    /* This is not linked in by default with the newlib-nano library. */
    asm (".global _printf_float");
#endif

int bytes = 0;
char webbuff[2000];	// After processing the received data is cleared and used to send data to the client
uint8 socket =0;
uint8 IP[4]={192,168,0,101};
uint8 Gateway[4]={192,168,0,1};
uint8 Subnet[4]={255,255,255,0};
uint8 MAC[6]={0,222,173,190,239,0};
uint16 tiempos=0,tiempos3=0,tiempoctrl=0,Tc=0,Tp=0,tiempoup=0,tiempodn=0;
short Control=0,Test=0,Lectura2=OFF,flagup=OFF,flagdn=OFF;
short ip1=0,ip2=0,ip3=0,ip4=0,s1=0,s2=0,s3=0,s4=0,g1=0,g2=0,g3=0,g4=0,m1=0,m2=0,m3=0,m4=0,m5=0,m6=0,entroGrafica=OFF,actualizaNumeracion=OFF,config=OFF;
uint8 horaGrafica[20],minutoGrafica[20];

CY_ISR(Reloj2){ 
    //CapSense_DisplayState();
    tiempos++;
    tiempos3++;
    tiempoctrl++;
   /*
    if(Control==1){// && tcHotJnuVolt > BROKEN_TC_THRESHOLD){
        if(tiempoctrl>=Tc) // Si el tiempo de control es mayor al tiempo de ciclo se reinicia tiempo de control
            tiempoctrl=0;
        // tiempoctrl incrementa una unidad cada 5ms, por eso se dividen los tiempos por 0.005.
      
        if(tiempoctrl<=Tp) // Si el tiempo es menor o igual al tiempo de control se enciende la resistencia de lo contrario se apaga.
            SSR_Write(1);
        else         
            SSR_Write(0);
            //PWM_WriteCompare(0);
    }else{
        if(!Test)
            SSR_Write(0);
            //PWM_WriteCompare(0);
    }*/
    
    if(tiempos3>=400){
        Lectura2=1;
        if(flagup)
            tiempoup++;
        if(flagdn)
            tiempodn++;   
        tiempos3=0;
    }
    
    if(tiempos>=1000){  // Cuenta cada 1ms   
        tiempos=0;Lectura=1;tiemporeset++;TiempoControl++;tiemporecta++;mensaje=!mensaje;envioUSB=1;
        
        if(LEVEL_Read()==1){//Si no detecta el nivel de agua en el humidificador
            tlevelH=0;  
            if(tlevel<20)
                tlevel++;   
            
            if(tllenado<600)
                tllenado++;
        }else{
            if(tlevelH<20)
                tlevelH++;
            tlevel=0;       
        }
        
        if(flagSobrepasoHumedad)
            tSobrepaso++;
        
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
}

CY_ISR(inteRX2){
    Dato3=UART_2_GetByte();
   if(Dato3==0x65){//Inicio Comunicacion
      Inicio2=1;
      RX_Wr_Index2 =0;
   }
   if(Inicio2==1){
      Rx_Buffer4[RX_Wr_Index2] = Dato3;
      RX_Wr_Index2++;
   }
   if(RX_Wr_Index2 >= RX_BUFFER_SIZE){
      RX_Wr_Index2 =0;
      Inicio2=0;
   }
   if(RX_Wr_Index2==0){
      if(Rx_Buffer4[0]==0x65 && Rx_Buffer4[1]==0xff && Rx_Buffer4[2]==0xff && Rx_Buffer4[8]==0x00 && Rx_Buffer4[9]==0xff )
         Dato_Exitoso2=5;
      else
         Dato_Exitoso2=10;
   }
}

int main(void)
{ 
    CyGlobalIntEnable; /* Enable global interrupts. */    
    SPI0_Start();  ETH0_Start();
    Reloj_StartEx(Reloj2);
    UART_1_Start();    
    UART_2_Start();    
    RX1_StartEx(inteRX); 
    RX2_StartEx(inteRX2);
    Timer_Start();  
    EEPROM_Start(); 
    ADC_Start();    
    ADC_SAR_Start();
    ADC_SAR_StartConvert();
    I2C_Start();
    IDAC_Start();
    CurrentMux_Start();
    ADCMux_Start();
    PWM_Start();
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
    humedadDisplay=Humedad;
    minH=Humedad;
    maxH=Humedad;
    CyDelay(1);
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
        escribeDisplay2("page Bienvenida");
        escribeDisplay2("bkcmd=0");
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
        
        T1=(uint8)(((float)T1Humedad)*((float)SetpointH/80.0));
        T3=(uint8)(((float)T3Humedad)*((float)SetpointH/80.0));
        Ganancia=GananciaR*(Setpoint/200);
        
        if((mEstp==0)&&(hEstp==0))
            Infinito=1;
        else
            Infinito=0;
        
        escribeDisplay1("page Principal");
        escribeDisplay1("bkcmd=0");
        escribeDisplay2("page Ajustes");
        escribeDisplay2("bkcmd=0");
        CyDelay(10);
        Menu=20;Menu2=1;
    }
    //write_ext_eeprom(78,160);
    DS1302_write (DS1302_ENABLE, 0);
    DS1302_write (DS1302_TRICKLE, 0x00);
    // Sete la IP
    sprintf(Str,"%d.%d.%d.%d",IP[0],IP[1],IP[2],IP[3]); 
    ETH0_SetIP(ETH0_ParseIP(Str));
    sprintf(displayStr,"%d.%d.%d.%d",Gateway[0],Gateway[1],Gateway[2],Gateway[3]); 
    sprintf(Str2,"%d.%d.%d.%d",Subnet[0],Subnet[1],Subnet[2],Subnet[3]); 
    ETH0_Init(MAC,ETH0_ParseIP(Str),ETH0_ParseIP(Str2),ETH0_ParseIP(displayStr));
    //Captura la hora y minuto para la grafica
    rtc_get_date(&Dia,&Mes,&Year,&dow);
    rtc_get_time(&Hora,&Minuto,&Segundo);
    graph[0].Minuto=Minuto;
    graph[0].Hora=Hora;
    tmuestreoUSB_ant=tmuestreoUSB;
    
    for(;;)
    {        
        if(Lectura2){
            Lectura2=0;
            Temperatura=leePt100(MediaMovil);
            Temperatura2=leePt1002(MediaMovil);
            Temperature2=leeLM35();
            Humedad=leeHumedad(MediaMovil2);
            ReadSensorCalibrationData();
            ReadUncompensatedTemperature();
            ReadAndCalculateSensorData();
        }
        
        dataEthernet();
        dataUSB();
        rtc_get_date(&Dia,&Mes,&Year,&dow);
        rtc_get_time(&Hora,&Minuto,&Segundo);
        LeeDisplay();
        
//_--------------------------------------------------------------------------------------------------------------------------------------------------------
        if(Menu == 120){ //Menu de Bienvenida.
            escribeDisplay1("bkcmd=0");
            CyDelay(800);
            escribeDisplay1("page Clave");
        }
//_--------------------------------------------------------------------------------------------------------------------------------------------------------
     
//_-------------------------------------------------------Display 1----------------------------------------------------------------------------------------        
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
                  escribeDisplay2("page Ajustes");
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
    }//Menu de Contraseña de acceso.
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
            }
            
            if(pulsoDisminuir){//Tecla abajo oprimida
               if(Minutos && mEstp>0){
                  mEstp--;
               }
               if(Horas  && hEstp>0){
                  hEstp--;
               }
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
            }
            
            if(pulsoDisminuir){//Tecla abajo oprimida
               if(Minutos && rampas[CicloLibre-1].Minutos>0){
                  rampas[CicloLibre-1].Minutos-=1.0;
               }
               if(Horas  && rampas[CicloLibre-1].Horas>0){
                  rampas[CicloLibre-1].Horas-=1.0;
               }
            }
            
            if(rampas[CicloLibre-1].Horas>99)rampas[CicloLibre-1].Horas=1;
            if(rampas[CicloLibre-1].Minutos>59)rampas[CicloLibre-1].Minutos=0;
               
            sprintf(displayStr,"tminsec.txt=\"%02u\"",rampas[CicloLibre-1].Horas);
            escribeDisplay1(displayStr);
            sprintf(displayStr,"tsecsec.txt=\"%02u\"",rampas[CicloLibre-1].Minutos);
            escribeDisplay1(displayStr);
         }
        
        if(Rx_Buffer[4]==0x0c || Rx_Buffer[4]==0x10){//Tecla Abajo Oprimida
            if(Rx_Buffer[4]==0x10){
                pulsoAumentar=OFF;
                pulsoDisminuir=OFF; 
                Rx_Buffer[4]=0x00;
                Rx_Buffer2[4]=0x00;
            }else{
                pulsoDisminuir=ON; 
            }
        }
        
        if(Rx_Buffer[4]==0x0d || Rx_Buffer[4]==0x0e){//Tecla Abajo Oprimida
            if(Rx_Buffer[4]==0x0e){
                pulsoAumentar=OFF;
                pulsoDisminuir=OFF; 
                Rx_Buffer[4]=0x00;
                Rx_Buffer2[4]=0x00;
            }else{
                pulsoAumentar=ON; 
            }
        }
        
        
    }//Menu de Tiempo de Duraciòn
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
               EEPROM_WriteByte((int8)Setpoint,2);
               EEPROM_WriteByte(extraeDecimal(Setpoint),3);
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
        }
        
        if(pulsoDisminuir){
            if(x1)
                Setpoint-=0.1;    
            else if(x10)    
                Setpoint-=1;    
            else if(x100)    
                Setpoint-=10;            
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
                
        }
        
        if(pulsoDisminuir){
            if(x1)
                rampas[CicloLibre-1].Temperatura-=0.1;    
            else if(x10)    
                rampas[CicloLibre-1].Temperatura-=1;    
            else if(x100)    
                rampas[CicloLibre-1].Temperatura-=10;
                
        }   
        
        if(rampas[CicloLibre-1].Temperatura<0.1)
            rampas[CicloLibre-1].Temperatura=60.0;
         if(rampas[CicloLibre-1].Temperatura>60.0)
            rampas[CicloLibre-1].Temperatura=0.1;
      }
         
    }//Menu de Temperatura
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
               EEPROM_WriteByte(SetpointH,4);
               CyDelay(10);
               GuardaEEPROM=ON;
            }
            T1=(uint8)(((float)T1Humedad)*((float)SetpointH/80.0));
            T3=(uint8)(((float)T3Humedad)*((float)SetpointH/80.0));
            pulsoAumentar=OFF;
            pulsoDisminuir=OFF;
         }
         
        if(pulsoAumentar){
            if(x1)
                SetpointH+=1;    
            else if(x10)    
                SetpointH+=10;    
        }
        
        if(pulsoDisminuir){
            if(x1)
                SetpointH-=1;    
            else if(x10)    
                SetpointH-=10;    
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
        }
        
        if(pulsoDisminuir){
            if(x1)
                rampas[CicloLibre-1].Humedad-=1;    
            else if(x10)    
                rampas[CicloLibre-1].Humedad-=10;                
        }
        
        if(rampas[CicloLibre-1].Humedad<10)
            rampas[CicloLibre-1].Humedad=95;
         if(rampas[CicloLibre-1].Humedad>95)
            rampas[CicloLibre-1].Humedad=10;
            
         sprintf(displayStr,"tsettem.txt=\"%02u\"",rampas[CicloLibre-1].Humedad);
         escribeDisplay1(displayStr);
      }
    }//Menu de Humedad
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
   }//Menu de Rampas
//_--------------------------------------------------------------------------------------------------------------------------------------------------------       
        
//_--------------------------------------------------------------------------------------------------------------------------------------------------------      
    if(Menu==15){//Menu de Clave Correcta
      if(!entro){
         CyDelay(800);
         entro=ON;
      }
      escribeDisplay1("page Principal");
   }//Menu de Clave Correcta
//_--------------------------------------------------------------------------------------------------------------------------------------------------------      

//_--------------------------------------------------------------------------------------------------------------------------------------------------------      
    if(Menu==16){//Menu de Clave InCorrecta
      if(!entro){
         CyDelay(800);         
         entro=ON;
      }
      escribeDisplay1("page Clave");
   }//Menu de Clave InCorrecta
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
    
      if(desconectoHumedad){
        inicioControlH=OFF;
        flagControlH=OFF;
        flagRampa=OFF;
      }
        
      if(desconectoTemperatura){
        inicioControlT=OFF;
        flagControlT=OFF;
        flagRampa=OFF;
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
                    T1=(uint8)(((float)T1Humedad)*((float)SetpointH/80.0));
                    T3=(uint8)(((float)T3Humedad)*((float)SetpointH/80.0));
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
                    if(tlevel>tiempoNivel){
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
                            CR4_Write(0);
                            BUZZER_Write(1);
                            CyDelay(600);
                            BUZZER_Write(0);
                            CyDelay(600);
                        }
                    }else if(tlevelH>tiempoNivel){
                        escribeDisplay1("status.txt=\"Puerta Cerrada\"");  
                        CR8_Write(0);//Bomba OFF;
                        controlaHumedad2();
                        tllenado=0;
                    }
                    if(TERM_Read()==1 || Temperatura>60){
                        escribeDisplay1("statusT.txt=\"!Termostato!\"");      
                        PWM_WriteCompare1(0);
                        PWM_WriteCompare2(0);
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
                        if(tlevel>tiempoNivel){
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
                                CR4_Write(0);
                                BUZZER_Write(1);
                                CyDelay(600);
                                BUZZER_Write(0);
                                CyDelay(600);
                            }
                        }else if(tlevelH>tiempoNivel){
                            escribeDisplay1("status.txt=\"Puerta Cerrada\"");  
                            CR8_Write(0);//Bomba OFF;
                            controlaHumedad2();
                            tllenado=0;
                        }
                        escribeDisplay1("b5.pic=85");
                    }else{
                        escribeDisplay1("statusH.txt=\"Detenido\"");
                        escribeDisplay1("status.txt=\"Puerta Cerrada\"");  
                        escribeDisplay1("b5.pic=84");
                        if(!Test){
                            CR7_Write(1);//Humidificador_off;
                            CR5_Write(0);//Dehumidificador_off;
                            CR8_Write(0);//Bomba OFF;
                            CR4_Write(0);
                        }
                        tllenado=0;
                    }
            
                    if(flagControlT){
                        escribeDisplay1("b4.pic=85");
                        controlaTemperatura();
                    }else{
                        escribeDisplay1("statusT.txt=\"Detenido\"");
                        escribeDisplay1("b4.pic=84");
                        if(!Test){
                            PWM_WriteCompare1(0);
                            PWM_WriteCompare2(0);
                            CR6_Write(1); // apaga Ventiladores de Celda de Peltier
                        }
                        Control=0;
                        Tpulso=0.0;
                    }
                }
            }else if(DOOR_Read()==1){
               if(!Test){
                CR7_Write(1);//Humidificador_off;
                CR5_Write(0);//Dehumidificador_off;
                CR6_Write(1); // apaga Ventiladores de Celda de Peltier
                CR8_Write(0);//Bomba OFF;
                CR4_Write(0);
                PWM_WriteCompare1(0);
                PWM_WriteCompare2(0);
               }
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
         PWM_WriteCompare1(0);
         PWM_WriteCompare2(0);
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
            desconectoTemperatura=OFF;
         }else{
             sprintf(displayStr, "temperatura.txt=\"T1:-----\""); 
             desconectoTemperatura=ON;
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
         if(Humedad>0.0 && Humedad<=110){
            //sprintf(displayStr,"humedad.txt=\"HR:%03.1f%%\"",Humedad);
            sprintf(displayStr,"humedad.txt=\"HR:%03.1f%%\"",humedadDisplay);
            desconectoHumedad=OFF;
         }else if(Humedad>110 || Humedad<=0.0){
            if(Humedad>=100 && Humedad<110)
                sprintf(displayStr,"humedad.txt=\"HR:100%%\"");    
            else    
                sprintf(displayStr,"humedad.txt=\"HR:----\"");
            desconectoHumedad=ON;
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
                  CyDelay(600);
                  //CyWdtClear();
                  BUZZER_Write(0);
                  CyDelay(600);
                  //CyWdtClear();
               }
            }
         }else{
            if((mEst>=mEstp)&&(hEst>=hEstp)&& tiempo_esterilizacion==1){
               if(flag_clear==0){
                  tiempo_esterilizacion=0;
                  Pausa=2;
                  flag_clear=1;
                  PWM_WriteCompare1(0);
                  PWM_WriteCompare2(0);
                  Control=0;
                  Tpulso=0.0;
                  flag_ganancia=0;
                  Finalizo=1;
               }
               escribeDisplay1("status.txt=\"Finalizado\"");
               ApagaSalidas();
               for(i=0;i<2;i++){
                  BUZZER_Write(1);
                  CyDelay(600);
                  //CyWdtClear();
                  BUZZER_Write(0);
                  CyDelay(600);
                  //CyWdtClear();
               }
            }
         }
      }
    }//Menu de Funcionamiento
//_--------------------------------------------------------------------------------------------------------------------------------------------------------       
//_-------------------------------------------------------Display 1----------------------------------------------------------------------------------------        
    

//_-------------------------------------------------------Display 2----------------------------------------------------------------------------------------            
//_--------------------------------------------------------------------------------------------------------------------------------------------------------
    if(Menu2==0){ //Menu de Contraseña de acceso.      
      if(Rx_Buffer3[4]==0x11){//11
         if(TipoClave!=0){
            escribeDisplay1("page Ajustes");
         }
      }
      
      if(TipoClave==3){
         escribeDisplay2("titulo.txt=\"Clave Nueva\"");
         escribeDisplay2("doevents");
      }else if(TipoClave==2){
         escribeDisplay2("titulo.txt=\"Clave Actual\"");         
         escribeDisplay2("doevents");
      }else if(TipoClave==1 || TipoClave==4){
         escribeDisplay2("titulo.txt=\"Clave Tecnico\"");         
         escribeDisplay2("doevents");         
      }else if(TipoClave==0){
        sprintf(displayStr,"titulo.txt=\"Ingrese Clave\"");
        escribeDisplay2(displayStr);
      }
      
      if(Rx_Buffer3[3]==0x0f){//0f, recibe caracteres ingresados desde el Display
         if(TipoClave==0){
            if(Rx_Buffer3[4]>=0x30 && Rx_Buffer3[5]>=0x30 && Rx_Buffer3[6]>=0x30 && Rx_Buffer3[7]>=0x30
            && Rx_Buffer3[4]<=0x39 && Rx_Buffer3[5]<=0x39 && Rx_Buffer3[6]<=0x39 && Rx_Buffer3[7]<=0x39){
            if(Rx_Buffer3[4]==0x33&&Rx_Buffer3[5]==0x38&&Rx_Buffer3[6]==0x39&&Rx_Buffer3[7]==0x32){ // Si Ingresa clave para reset general del sistema.
               EEPROM_WriteByte(0,10);CyDelay(20);EEPROM_WriteByte(0,11);CyDelay(20);// Reestablece a contraseña de Fabrica y reinicia Programa.
               EEPROM_WriteByte(0,12);CyDelay(20);EEPROM_WriteByte(0,13);CyDelay(20);
               CySoftwareReset();
            }
            }
            
            if(Rx_Buffer3[4]>=0x30 && Rx_Buffer3[5]>=0x30 && Rx_Buffer3[6]>=0x30 && Rx_Buffer3[7]>=0x30
            && Rx_Buffer3[4]<=0x39 && Rx_Buffer3[5]<=0x39 && Rx_Buffer3[6]<=0x39 && Rx_Buffer3[7]<=0x39){
            if((Rx_Buffer3[4]==Password[0]+0x30)&&(Rx_Buffer3[5]==Password[1]+0x30)&&(Rx_Buffer3[6]==Password[2]+0x30)&&(Rx_Buffer3[7]==Password[3]+0x30)){
                  escribeDisplay2("page ClaveCorrecta");
                  dibujo=OFF;
                  //RX_Buffer[3]=0x00;RX_Buffer2[3]=0x00;  
               }else{
                  escribeDisplay2("page ClaveBad");
                  //RX_Buffer[3]=0x00;RX_Buffer2[3]=0x00;  
               } 
            }
         }else if(TipoClave==1){
            if(Rx_Buffer3[4]>=0x30 && Rx_Buffer3[5]>=0x30 && Rx_Buffer3[6]>=0x30 && Rx_Buffer3[7]>=0x30
            && Rx_Buffer3[4]<=0x39 && Rx_Buffer3[5]<=0x39 && Rx_Buffer3[6]<=0x39 && Rx_Buffer3[7]<=0x39){
            if(Rx_Buffer3[4]==0x34&&Rx_Buffer3[5]==0x34&&Rx_Buffer3[6]==0x34&&Rx_Buffer3[7]==0x34){ // Si Ingresa clave de Servicio Tecnico
               escribeDisplay2("page Config");
            }else{
               escribeDisplay2("page Ajustes");
            }
            }
         }else if(TipoClave==2){
            if(Rx_Buffer3[4]>=0x30 && Rx_Buffer3[5]>=0x30 && Rx_Buffer3[6]>=0x30 && Rx_Buffer3[7]>=0x30
            && Rx_Buffer3[4]<=0x39 && Rx_Buffer3[5]<=0x39 && Rx_Buffer3[6]<=0x39 && Rx_Buffer3[7]<=0x39){
            if((Rx_Buffer3[4]==Password[0]+0x30)&&(Rx_Buffer3[5]==Password[1]+0x30)&&(Rx_Buffer3[6]==Password[2]+0x30)&&(Rx_Buffer3[7]==Password[3]+0x30)){
               escribeDisplay2("page Clave");
               escribeDisplay2("titulo.txt=\"Clave Nueva\"");
               TipoClave=3;
               Rx_Buffer3[3]=0x00;
               Rx_Buffer4[3]=0x00;
            }else{
               escribeDisplay2("page Ajustes");
               //RX_Buffer[3]=0x00;
               //RX_Buffer2[3]=0x00;
            } 
            }
         }else if(TipoClave==3){
            escribeDisplay2("titulo.txt=\"Clave Nueva\"");
            escribeDisplay2("page Ajustes");
            if(!GuardaEEPROM){
               //write_eeprom(10,Rx_Buffer[4]-0x30);delay_ms(20);write_eeprom(11,Rx_Buffer[5]-0x30);delay_ms(20);
               EEPROM_WriteByte(Rx_Buffer3[4]-0x30,10);CyDelay(20);
               EEPROM_WriteByte(Rx_Buffer3[5]-0x30,11);CyDelay(20);
               EEPROM_WriteByte(Rx_Buffer3[6]-0x30,12);CyDelay(20);
               EEPROM_WriteByte(Rx_Buffer3[7]-0x30,13);CyDelay(20);
               GuardaEEPROM=ON;
            }
            Password[0]=Rx_Buffer3[4]-0x30;Password[1]=Rx_Buffer3[5]-0x30;Password[2]=Rx_Buffer3[6]-0x30;Password[3]=Rx_Buffer3[7]-0x30;
            //RX_Buffer[3]=0x00;
            //RX_Buffer2[3]=0x00;
         }else if(TipoClave==4){
            if(Rx_Buffer3[4]==0x34&&Rx_Buffer3[5]==0x34&&Rx_Buffer3[6]==0x34&&Rx_Buffer3[7]==0x34){ // Si Ingresa clave de Servicio Tecnico
                if(!config)
                    escribeDisplay2("page Test");
                else
                    escribeDisplay2("page Config");
            }else{
               escribeDisplay2("page Ajustes");
            }
            //RX_Buffer[3]=0x00;
            //RX_Buffer2[3]=0x00;
         }
      }
    }//Menu de Contraseña de acceso.
//_--------------------------------------------------------------------------------------------------------------------------------------------------------       

//_--------------------------------------------------------------------------------------------------------------------------------------------------------      
    if(Menu2==1){//Menu Principal
        ETH0_StringIP(Str);
        sprintf(displayStr, "fecha.txt=\"IP:%s \"",Str); 
        escribeDisplay2(displayStr);
        if(Rx_Buffer3[4]==0x01){
            escribeDisplay2("page Red");
            flagRed=0;
            ip1=0;ip2=0;ip3=0;ip4=0;g1=0;g2=0;g3=0;g4=0;s1=0;s2=0;s3=0;s4=0;m1=0;m2=0;m3=0;m4=0;m5=0;m6=0;
            Rx_Buffer3[4]=0x00;
            Rx_Buffer4[4]=0x00;   
            CyDelay(200);
        } 
    
        if(Rx_Buffer3[4]==0x05){
            escribeDisplay2("page Fecha");
            Rx_Buffer3[4]=0x00;
            Rx_Buffer4[4]=0x00;   
            DiaTx=Dia;
            MesTx=Mes;
            YearTx=Year;
            HoraTx=Hora;
            MinutoTx=Minuto;
        } 
        
        if(Rx_Buffer3[4]==0x06){
            escribeDisplay2("page Clave");
            Rx_Buffer3[4]=0x00;
            Rx_Buffer4[4]=0x00; 
            TipoClave=2;
        } 
        
        if(Rx_Buffer3[4]==0x04){
            escribeDisplay2("page Test");
            Rx_Buffer3[4]=0x00;
            Rx_Buffer4[4]=0x00;   
            TipoClave=4;
        } 
        
        if(Rx_Buffer3[4]==0xa0){
            escribeDisplay2("page Clave");
            Rx_Buffer3[4]=0x00;
            Rx_Buffer4[4]=0x00;   
            TipoClave=4;
            Opcion2=1;
            config=ON;
        } 
        
        if(Rx_Buffer3[4]==0x07){
            escribeDisplay2("page Grafica");
            Rx_Buffer3[4]=0x00;
            Rx_Buffer4[4]=0x00;   
            entroGrafica=ON;
            TipoClave=1;
        } 
        
        if(Rx_Buffer3[4]==0x08){
            escribeDisplay2("page Calib");
            Rx_Buffer3[4]=0x00;
            Rx_Buffer4[4]=0x00;   
            Opcion=1;
        } 
        
        if(Rx_Buffer3[4]==0x09){
            escribeDisplay2("page CalibHumedad");
            Rx_Buffer3[4]=0x00;
            Rx_Buffer4[4]=0x00;   
            Opcion=1;
        } 
    }//Menu Principal
//_--------------------------------------------------------------------------------------------------------------------------------------------------------          

//_--------------------------------------------------------------------------------------------------------------------------------------------------------      
    if(Menu2==9){//Menu de Grafica
        if(Rx_Buffer3[4]==0x11){
            escribeDisplay2("page Ajustes");
            Rx_Buffer3[4]=0x00;
            Rx_Buffer4[4]=0x00; 
        }
        
        if(tmuestreoUSB!=tmuestreoUSB_ant){
            borraVector();
            tmuestreoUSB_ant=tmuestreoUSB;
        }
        
        if(actualizaNumeracion){
            actualizaEjeX();
            actualizaNumeracion=OFF;
        }
        
        if(entroGrafica){//Si entro al menu de grafica
            actualizaEjeX();
            for(data2=0;data2<data;data2++){//for dedicado a actualizar la grafica 
                sprintf(displayStr,"add 1,0,%1.0f",graph[data2].Temperatura*2); 
                escribeDisplay2(displayStr);
                sprintf(displayStr,"add 1,1,%1.0f",graph[data2].Humedad*2); 
                escribeDisplay2(displayStr);
            }   
            entroGrafica=OFF;
        }
        
        if(flagControlT && !flagControlH && !flagRampa){
            escribeDisplay2("t0.txt=\"Control de Temperatura Activado\"");
        }else if(!flagControlT && flagControlH && !flagRampa){
            escribeDisplay2("t0.txt=\"Control de Humedad Activado\"");
        }else if(flagControlT && flagControlH && !flagRampa){
            escribeDisplay2("t0.txt=\"Control de Temperatura y Humedad Activado\"");
        }else if(flagRampa){
            escribeDisplay2("t0.txt=\"Control por Rampas Activado\"");
        }else{
            escribeDisplay2("t0.txt=\"Controles Desactivados\"");
        }
        
        if(grafica){
            grafica=OFF;
            sprintf(displayStr,"add 1,0,%1.0f",Temperatura*2); 
            escribeDisplay2(displayStr);
            //sprintf(displayStr,"add 1,1,%1.0f",Humedad*2); 
            sprintf(displayStr,"add 1,1,%1.0f",humedadDisplay*2); 
            escribeDisplay2(displayStr);
        }
         
    }//Menu de Grafica
//_--------------------------------------------------------------------------------------------------------------------------------------------------------          
        
//_--------------------------------------------------------------------------------------------------------------------------------------------------------      
    if(Menu2==10){//Menu de RED
        
        if(!flagRed){
            sprintf(displayStr, "ip1.txt=\"%u\"",IP[0]); 
            escribeDisplay2(displayStr);
            sprintf(displayStr, "ip2.txt=\"%u\"",IP[1]); 
            escribeDisplay2(displayStr);
            sprintf(displayStr, "ip3.txt=\"%u\"",IP[2]); 
            escribeDisplay2(displayStr);
            sprintf(displayStr, "ip4.txt=\"%u\"",IP[3]); 
            escribeDisplay2(displayStr);
        
            sprintf(displayStr, "g1.txt=\"%u\"",Gateway[0]); 
            escribeDisplay2(displayStr);
            sprintf(displayStr, "g2.txt=\"%u\"",Gateway[1]); 
            escribeDisplay2(displayStr);
            sprintf(displayStr, "g3.txt=\"%u\"",Gateway[2]); 
            escribeDisplay2(displayStr);
            sprintf(displayStr, "g4.txt=\"%u\"",Gateway[3]); 
            escribeDisplay2(displayStr);
        
            sprintf(displayStr, "s1.txt=\"%u\"",Subnet[0]); 
            escribeDisplay2(displayStr);
            sprintf(displayStr, "s2.txt=\"%u\"",Subnet[1]); 
            escribeDisplay2(displayStr);
            sprintf(displayStr, "s3.txt=\"%u\"",Subnet[2]); 
            escribeDisplay2(displayStr);
            sprintf(displayStr, "s4.txt=\"%u\"",Subnet[3]); 
            escribeDisplay2(displayStr);
            
            sprintf(displayStr, "m1.txt=\"%u\"",MAC[0]); 
            escribeDisplay2(displayStr);
            sprintf(displayStr, "m2.txt=\"%u\"",MAC[1]); 
            escribeDisplay2(displayStr);
            sprintf(displayStr, "m3.txt=\"%u\"",MAC[2]); 
            escribeDisplay2(displayStr);
            sprintf(displayStr, "m4.txt=\"%u\"",MAC[3]); 
            escribeDisplay2(displayStr);
            sprintf(displayStr, "m5.txt=\"%u\"",MAC[4]); 
            escribeDisplay2(displayStr);
            sprintf(displayStr, "m6.txt=\"%u\"",MAC[5]); 
            escribeDisplay2(displayStr);
        }
        
        if(Rx_Buffer3[4]==0x11){
            escribeDisplay2("page Ajustes");
            sprintf(Str,"%d.%d.%d.%d",IP[0],IP[1],IP[2],IP[3]); 
            ETH0_SetIP(ETH0_ParseIP(Str));
            sprintf(displayStr,"%d.%d.%d.%d",Gateway[0],Gateway[1],Gateway[2],Gateway[3]); 
            sprintf(Str2,"%d.%d.%d.%d",Subnet[0],Subnet[1],Subnet[2],Subnet[3]); 
            ETH0_Init(MAC,ETH0_ParseIP(Str),ETH0_ParseIP(Str2),ETH0_ParseIP(displayStr));
            EEPROM_WriteByte(IP[0],47);
            CyDelay(10);
            EEPROM_WriteByte(IP[1],48);
            CyDelay(10); 
            EEPROM_WriteByte(IP[2],49);
            CyDelay(10); 
            EEPROM_WriteByte(IP[3],50);
            CyDelay(10); 
            EEPROM_WriteByte(Gateway[0],51);
            CyDelay(10); 
            EEPROM_WriteByte(Gateway[1],52);
            CyDelay(10); 
            EEPROM_WriteByte(Gateway[2],53);
            CyDelay(10); 
            EEPROM_WriteByte(Gateway[3],54);
            CyDelay(10); 
            EEPROM_WriteByte(Subnet[0],55);
            CyDelay(10); 
            EEPROM_WriteByte(Subnet[1],56);
            CyDelay(10); 
            EEPROM_WriteByte(Subnet[2],57);
            CyDelay(10);
            EEPROM_WriteByte(Subnet[3],58);
            CyDelay(10); 
            EEPROM_WriteByte(MAC[0],59);
            CyDelay(10); 
            EEPROM_WriteByte(MAC[1],60);
            CyDelay(10); 
            EEPROM_WriteByte(MAC[2],61);
            CyDelay(10); 
            EEPROM_WriteByte(MAC[3],62);
            CyDelay(10); 
            EEPROM_WriteByte(MAC[4],63);
            CyDelay(10); 
            //EEPROM_WriteByte(MAC[5],64);
            //CyDelay(10); 
            Rx_Buffer3[4]=0x00;
            Rx_Buffer4[4]=0x00;   
            flagRed=0;
        } 
        
        if(Rx_Buffer3[4]==0x00){
            if(ip1){
                IP[0]=Rx_Buffer3[5];
            }else if(ip2){
                IP[1]=Rx_Buffer3[5];
            }else if(ip3){
                IP[2]=Rx_Buffer3[5];
            }else if(ip4){
                IP[3]=Rx_Buffer3[5];
            }else if(g1){
                Gateway[0]=Rx_Buffer3[5];
            }else if(g2){
                Gateway[1]=Rx_Buffer3[5];
            }else if(g3){
                Gateway[2]=Rx_Buffer3[5];
            }else if(g4){
                Gateway[3]=Rx_Buffer3[5];
            }else if(s1){
                Subnet[0]=Rx_Buffer3[5];
            }else if(s2){
                Subnet[1]=Rx_Buffer3[5];
            }else if(s3){
                Subnet[2]=Rx_Buffer3[5];
            }else if(s4){
                Subnet[3]=Rx_Buffer3[5];
            }else if(m1){
                MAC[0]=Rx_Buffer3[5];
            }else if(m2){
                MAC[1]=Rx_Buffer3[5];
            }else if(m3){
                MAC[2]=Rx_Buffer3[5];
            }else if(m4){
                MAC[3]=Rx_Buffer3[5];
            }else if(m5){
                MAC[4]=Rx_Buffer3[5];
            }else if(m6){
                MAC[5]=Rx_Buffer3[5];
            }
        }else if(Rx_Buffer3[4]==0x01){//Oprimio IP1
            ip1=1;ip2=0;ip3=0;ip4=0;g1=0;g2=0;g3=0;g4=0;s1=0;s2=0;s3=0;s4=0;m1=0;m2=0;m3=0;m4=0;m5=0;m6=0;
            Rx_Buffer3[4]=0x00;Rx_Buffer4[4]=0x00;flagRed=1;   
        }else if(Rx_Buffer3[4]==0x02){//Oprimio IP2
            ip1=0;ip2=1;ip3=0;ip4=0;g1=0;g2=0;g3=0;g4=0;s1=0;s2=0;s3=0;s4=0;m1=0;m2=0;m3=0;m4=0;m5=0;m6=0;
            Rx_Buffer3[4]=0x00;Rx_Buffer4[4]=0x00;flagRed=1;   
        }else if(Rx_Buffer3[4]==0x03){//Oprimio IP3
            ip1=0;ip2=0;ip3=1;ip4=0;g1=0;g2=0;g3=0;g4=0;s1=0;s2=0;s3=0;s4=0;m1=0;m2=0;m3=0;m4=0;m5=0;m6=0;
            Rx_Buffer3[4]=0x00;Rx_Buffer4[4]=0x00;flagRed=1;   
        }else if(Rx_Buffer3[4]==0x04){//Oprimio IP4
            ip1=0;ip2=0;ip3=0;ip4=1;g1=0;g2=0;g3=0;g4=0;s1=0;s2=0;s3=0;s4=0;m1=0;m2=0;m3=0;m4=0;m5=0;m6=0;
            Rx_Buffer3[4]=0x00;Rx_Buffer4[4]=0x00;flagRed=1;   
        }else if(Rx_Buffer3[4]==0x05){//Oprimio Gateway1
            ip1=0;ip2=0;ip3=0;ip4=0;g1=1;g2=0;g3=0;g4=0;s1=0;s2=0;s3=0;s4=0;m1=0;m2=0;m3=0;m4=0;m5=0;m6=0;
            Rx_Buffer3[4]=0x00;Rx_Buffer4[4]=0x00;flagRed=1;   
        }else if(Rx_Buffer3[4]==0x06){//Oprimio Gateway2
            ip1=0;ip2=0;ip3=0;ip4=0;g1=0;g2=1;g3=0;g4=0;s1=0;s2=0;s3=0;s4=0;m1=0;m2=0;m3=0;m4=0;m5=0;m6=0;
            Rx_Buffer3[4]=0x00;Rx_Buffer4[4]=0x00;flagRed=1;   
        }else if(Rx_Buffer3[4]==0x07){//Oprimio Gateway3
            ip1=0;ip2=0;ip3=0;ip4=0;g1=0;g2=0;g3=1;g4=0;s1=0;s2=0;s3=0;s4=0;m1=0;m2=0;m3=0;m4=0;m5=0;m6=0;
            Rx_Buffer3[4]=0x00;Rx_Buffer4[4]=0x00;flagRed=1;   
        }else if(Rx_Buffer3[4]==0x08){//Oprimio Gateway4
            ip1=0;ip2=0;ip3=0;ip4=0;g1=0;g2=0;g3=0;g4=1;s1=0;s2=0;s3=0;s4=0;m1=0;m2=0;m3=0;m4=0;m5=0;m6=0;
            Rx_Buffer3[4]=0x00;Rx_Buffer4[4]=0x00;flagRed=1;   
        }else if(Rx_Buffer3[4]==0x09){//Oprimio Subnet1
            ip1=0;ip2=0;ip3=0;ip4=0;g1=0;g2=0;g3=0;g4=0;s1=1;s2=0;s3=0;s4=0;m1=0;m2=0;m3=0;m4=0;m5=0;m6=0;
            Rx_Buffer3[4]=0x00;Rx_Buffer4[4]=0x00;flagRed=1;   
        }else if(Rx_Buffer3[4]==0x0a){//Oprimio Subnet2
            ip1=0;ip2=0;ip3=0;ip4=0;g1=0;g2=0;g3=0;g4=0;s1=0;s2=1;s3=0;s4=0;m1=0;m2=0;m3=0;m4=0;m5=0;m6=0;
            Rx_Buffer3[4]=0x00;Rx_Buffer4[4]=0x00;flagRed=1;   
        }else if(Rx_Buffer3[4]==0x0b){//Oprimio Subnet3
            ip1=0;ip2=0;ip3=0;ip4=0;g1=0;g2=0;g3=0;g4=0;s1=0;s2=0;s3=1;s4=0;m1=0;m2=0;m3=0;m4=0;m5=0;m6=0;
            Rx_Buffer3[4]=0x00;Rx_Buffer4[4]=0x00;flagRed=1;   
        }else if(Rx_Buffer3[4]==0x0c){//Oprimio Subnet4
            ip1=0;ip2=0;ip3=0;ip4=0;g1=0;g2=0;g3=0;g4=0;s1=0;s2=0;s3=0;s4=1;m1=0;m2=0;m3=0;m4=0;m5=0;m6=0;
            Rx_Buffer3[4]=0x00;Rx_Buffer4[4]=0x00;flagRed=1;   
        }else if(Rx_Buffer3[4]==0x0d){//Oprimio Subnet1
            ip1=0;ip2=0;ip3=0;ip4=0;g1=0;g2=0;g3=0;g4=0;s1=0;s2=0;s3=0;s4=0;m1=1;m2=0;m3=0;m4=0;m5=0;m6=0;
            Rx_Buffer3[4]=0x00;Rx_Buffer4[4]=0x00;flagRed=1;   
        }else if(Rx_Buffer3[4]==0x0e){
            ip1=0;ip2=0;ip3=0;ip4=0;g1=0;g2=0;g3=0;g4=0;s1=0;s2=0;s3=0;s4=0;m1=0;m2=1;m3=0;m4=0;m5=0;m6=0;
            Rx_Buffer3[4]=0x00;Rx_Buffer4[4]=0x00;flagRed=1;   
        }else if(Rx_Buffer3[4]==0x0f){
            ip1=0;ip2=0;ip3=0;ip4=0;g1=0;g2=0;g3=0;g4=0;s1=0;s2=0;s3=0;s4=0;m1=0;m2=0;m3=1;m4=0;m5=0;m6=0;
            Rx_Buffer3[4]=0x00;Rx_Buffer4[4]=0x00;flagRed=1;   
        }else if(Rx_Buffer3[4]==0x10){
            ip1=0;ip2=0;ip3=0;ip4=0;g1=0;g2=0;g3=0;g4=0;s1=0;s2=0;s3=0;s4=0;m1=0;m2=0;m3=0;m4=1;m5=0;m6=0;
            Rx_Buffer3[4]=0x00;Rx_Buffer4[4]=0x00;flagRed=1;   
        }else if(Rx_Buffer3[4]==0x12){
            ip1=0;ip2=0;ip3=0;ip4=0;g1=0;g2=0;g3=0;g4=0;s1=0;s2=0;s3=0;s4=0;m1=0;m2=0;m3=0;m4=0;m5=1;m6=0;
            Rx_Buffer3[4]=0x00;Rx_Buffer4[4]=0x00;flagRed=1;   
        }else if(Rx_Buffer3[4]==0x13){
            ip1=0;ip2=0;ip3=0;ip4=0;g1=0;g2=0;g3=0;g4=0;s1=0;s2=0;s3=0;s4=0;m1=0;m2=0;m3=0;m4=0;m5=0;m6=1;
            Rx_Buffer3[4]=0x00;Rx_Buffer4[4]=0x00;flagRed=1;   
        }
    }//Menu de RED
//_--------------------------------------------------------------------------------------------------------------------------------------------------------          

//_--------------------------------------------------------------------------------------------------------------------------------------------------------      
    if(Menu2==11){ //Menu de Calibracion de Temperatura
         if(Rx_Buffer3[4]==0x01){// Sensor 1
            Opcion=1;            
         }else if(Rx_Buffer3[4]==0x02){//Patron 1
            Opcion=2;
         }else if(Rx_Buffer3[4]==0x03){//Sensor 2
            Opcion=3;
         }else if(Rx_Buffer3[4]==0x04){//Patron 2
            Opcion=4;
         }else if(Rx_Buffer3[4]==0x05){//Sensor 3
            Opcion=5;
         }else if(Rx_Buffer3[4]==0x06){//Patron 3
            Opcion=6;
         }else if(Rx_Buffer3[4]==0x07){//Sensor 1, Temperatura 2
            Opcion=7;
         }else if(Rx_Buffer3[4]==0x08){//Patron 1, Temperatura 2
            Opcion=8;
         }else if(Rx_Buffer3[4]==0x09){//Sensor 2, Temperatura 2
            Opcion=9;
         }else if(Rx_Buffer3[4]==0x0a){//Patron 2, Temperatura 2
            Opcion=10;
         }else if(Rx_Buffer3[4]==0x0b){//Sensor 3, Temperatura 2
            Opcion=11;
         }else if(Rx_Buffer3[4]==0x0c){//Patron 3, Temperatura 2
            Opcion=12;
         }
         
         if(Opcion>12)
            Opcion=1;
         if(Opcion<1)
            Opcion=12;
         
         for(i=0;i<12;i++){
            if(i==Opcion-1){
                 sprintf(displayStr,"t%u.bco=27501",i);
                 escribeDisplay2(displayStr); 
                 sprintf(displayStr,"t%u.pco=65535",i);
                 escribeDisplay2(displayStr); 
             }else{
                 sprintf(displayStr,"t%u.bco=65535",i);
                 escribeDisplay2(displayStr); 
                 sprintf(displayStr,"t%u.pco=0",i);
                 escribeDisplay2(displayStr); 
             }
         }
        
         if(Rx_Buffer3[4]==0x11){//Selecciono Regresar
            escribeDisplay2("page Ajustes");
            EEPROM_WriteByte(calib[0].patron1,64);CyDelay(20);
            EEPROM_WriteByte(extraeDecimal(calib[0].patron1),65);CyDelay(20);
            EEPROM_WriteByte(calib[0].patron2,66);CyDelay(20);
            EEPROM_WriteByte(extraeDecimal(calib[0].patron2),67);CyDelay(20);
            EEPROM_WriteByte(calib[0].patron3,68);CyDelay(20);
            EEPROM_WriteByte(extraeDecimal(calib[0].patron3),69);CyDelay(20);
            EEPROM_WriteByte(calib[0].control1,70);CyDelay(20);
            EEPROM_WriteByte(extraeDecimal(calib[0].control1),71);CyDelay(20);
            EEPROM_WriteByte(calib[0].control2,72);CyDelay(20);
            EEPROM_WriteByte(extraeDecimal(calib[0].control2),73);CyDelay(20);
            EEPROM_WriteByte(calib[0].control3,74);CyDelay(20);
            EEPROM_WriteByte(extraeDecimal(calib[0].control3),75);CyDelay(20);
            
            EEPROM_WriteByte(calib[1].patron1,76);CyDelay(20);
            EEPROM_WriteByte(extraeDecimal(calib[1].patron1),77);CyDelay(20);
            EEPROM_WriteByte(calib[1].patron2,78);CyDelay(20);
            EEPROM_WriteByte(extraeDecimal(calib[1].patron2),79);CyDelay(20);
            EEPROM_WriteByte(calib[1].patron3,80);CyDelay(20);
            EEPROM_WriteByte(extraeDecimal(calib[1].patron3),81);CyDelay(20);
            EEPROM_WriteByte(calib[1].control1,82);CyDelay(20);
            EEPROM_WriteByte(extraeDecimal(calib[1].control1),83);CyDelay(20);
            EEPROM_WriteByte(calib[1].control2,84);CyDelay(20);
            EEPROM_WriteByte(extraeDecimal(calib[1].control2),85);CyDelay(20);
            EEPROM_WriteByte(calib[1].control3,86);CyDelay(20);
            EEPROM_WriteByte(extraeDecimal(calib[1].control3),87);CyDelay(20);
         }
        
        for(i=0;i<2;i++){
            calib[i].m1=(calib[i].control2-calib[i].control1)/(calib[i].patron2-calib[i].patron1);
            calib[i].b1=calib[i].control2-(calib[i].m1*calib[i].patron2);
            calib[i].m2=(calib[i].control3-calib[i].control2)/(calib[i].patron3-calib[i].patron2);
            calib[i].b2=calib[i].control3-(calib[i].m2*calib[i].patron3);
            sprintf(displayStr,"t%u.txt=\"m1:%2.2f b1:%2.2f m2:%2.2f b2:%2.2f\"",i+20,calib[i].m1,calib[i].b1,calib[i].m2,calib[i].b2);
            escribeDisplay2(displayStr); 
        }
        
         if(Rx_Buffer3[4]==0x2a || Rx_Buffer3[4]==0x2c){//Tecla Arriba Oprimida
            if(Opcion==1){
               calib[0].control1+=0.1;
            }else if(Opcion==2){
               calib[0].patron1+=0.1; 
            }else if(Opcion==3){
               calib[0].control2+=0.1; 
            }else if(Opcion==4){
               calib[0].patron2+=0.1;  
            }else if(Opcion==5){
                calib[0].control3+=0.1;
            }else if(Opcion==6){
                calib[0].patron3+=0.1; 
            }else if(Opcion==7){
                calib[1].control1+=0.1;
            }else if(Opcion==8){
                calib[1].patron1+=0.1; 
            }else if(Opcion==9){
                calib[1].control2+=0.1;
            }else if(Opcion==10){
                calib[1].patron2+=0.1; 
            }else if(Opcion==11){
                calib[1].control3+=0.1;
            }else if(Opcion==12){
                calib[1].patron3+=0.1; 
            }
            if(Rx_Buffer3[4]==0x2c){
                Rx_Buffer3[4]=0x00;
                Rx_Buffer4[4]=0x00;  
                CyDelay(100);
            }
         }
         
         if(Rx_Buffer3[4]==0x2b || Rx_Buffer3[4]==0x2d){//Tecla Abajo Oprimida
            if(Opcion==1){
               calib[0].control1-=0.1;
            }else if(Opcion==2){
               calib[0].patron1-=0.1; 
            }else if(Opcion==3){
               calib[0].control2-=0.1; 
            }else if(Opcion==4){
               calib[0].patron2-=0.1;  
            }else if(Opcion==5){
                calib[0].control3-=0.1;
            }else if(Opcion==6){
                calib[0].patron3-=0.1; 
            }else if(Opcion==7){
                calib[1].control1-=0.1;
            }else if(Opcion==8){
                calib[1].patron1-=0.1; 
            }else if(Opcion==9){
                calib[1].control2-=0.1;
            }else if(Opcion==10){
                calib[1].patron2-=0.1; 
            }else if(Opcion==11){
                calib[1].control3-=0.1;
            }else if(Opcion==12){
                calib[1].patron3-=0.1; 
            }
            if(Rx_Buffer3[4]==0x2d){
                Rx_Buffer3[4]=0x00;
                Rx_Buffer4[4]=0x00;
                CyDelay(100);
            }
         }
        
        sprintf(displayStr,"t0.txt=\"%2.1f°C\"",calib[0].control1);
        escribeDisplay2(displayStr);
        sprintf(displayStr,"t1.txt=\"%2.1f°C\"",calib[0].patron1);
        escribeDisplay2(displayStr);
        sprintf(displayStr,"t2.txt=\"%2.1f°C\"",calib[0].control2);
        escribeDisplay2(displayStr);
        sprintf(displayStr,"t3.txt=\"%2.1f°C\"",calib[0].patron2);
        escribeDisplay2(displayStr);
        sprintf(displayStr,"t4.txt=\"%2.1f°C\"",calib[0].control3);
        escribeDisplay2(displayStr);
        sprintf(displayStr,"t5.txt=\"%2.1f°C\"",calib[0].patron3);
        escribeDisplay2(displayStr);
        sprintf(displayStr,"t6.txt=\"%2.1f°C\"",calib[1].control1);
        escribeDisplay2(displayStr);
        sprintf(displayStr,"t7.txt=\"%2.1f°C\"",calib[1].patron1);
        escribeDisplay2(displayStr);
        sprintf(displayStr,"t8.txt=\"%2.1f°C\"",calib[1].control2);
        escribeDisplay2(displayStr);
        sprintf(displayStr,"t9.txt=\"%2.1f°C\"",calib[1].patron2);
        escribeDisplay2(displayStr);
        sprintf(displayStr,"t10.txt=\"%2.1f°C\"",calib[1].control3);
        escribeDisplay2(displayStr);
        sprintf(displayStr,"t11.txt=\"%2.1f°C\"",calib[1].patron3);
        escribeDisplay2(displayStr);
      
    }//Menu de Calibracion de Temperatura
//_--------------------------------------------------------------------------------------------------------------------------------------------------------       
      
//_--------------------------------------------------------------------------------------------------------------------------------------------------------      
    if(Menu2==12){ //Menu de Calibracion de Humedad
         if(Rx_Buffer3[4]==0x01){// Sensor 1
            Opcion=1;            
         }else if(Rx_Buffer3[4]==0x02){//Patron 1
            Opcion=2;
         }else if(Rx_Buffer3[4]==0x03){//Sensor 2
            Opcion=3;
         }else if(Rx_Buffer3[4]==0x04){//Patron 2
            Opcion=4;
         }else if(Rx_Buffer3[4]==0x05){//Sensor 3
            Opcion=5;
         }else if(Rx_Buffer3[4]==0x06){//Patron 3
            Opcion=6;
         }else if(Rx_Buffer3[4]==0x07){//Sensor 1, Temperatura 2
            Opcion=7;
         }else if(Rx_Buffer3[4]==0x08){//Patron 1, Temperatura 2
            Opcion=8;
         }else if(Rx_Buffer3[4]==0x09){//Sensor 2, Temperatura 2
            Opcion=9;
         }else if(Rx_Buffer3[4]==0x0a){//Patron 2, Temperatura 2
            Opcion=10;
         }else if(Rx_Buffer3[4]==0x0b){//Sensor 3, Temperatura 2
            Opcion=11;
         }else if(Rx_Buffer3[4]==0x0c){//Patron 3, Temperatura 2
            Opcion=12;
         }
         
         if(Opcion>12)
            Opcion=1;
         if(Opcion<1)
            Opcion=12;
         
         for(i=0;i<12;i++){
            if(i==Opcion-1){
                 sprintf(displayStr,"t%u.bco=27501",i);
                 escribeDisplay2(displayStr); 
                 sprintf(displayStr,"t%u.pco=65535",i);
                 escribeDisplay2(displayStr); 
             }else{
                 sprintf(displayStr,"t%u.bco=65535",i);
                 escribeDisplay2(displayStr); 
                 sprintf(displayStr,"t%u.pco=0",i);
                 escribeDisplay2(displayStr); 
             }
         }
        
         if(Rx_Buffer3[4]==0x11){//Selecciono Regresar
            escribeDisplay2("page Ajustes");            
            EEPROM_WriteByte(calib[2].patron1,88);CyDelay(20);
            EEPROM_WriteByte(extraeDecimal(calib[2].patron1),89);CyDelay(20);
            EEPROM_WriteByte(calib[2].patron2,90);CyDelay(20);
            EEPROM_WriteByte(extraeDecimal(calib[2].patron2),91);CyDelay(20);
            EEPROM_WriteByte(calib[2].patron3,92);CyDelay(20);
            EEPROM_WriteByte(extraeDecimal(calib[2].patron3),93);CyDelay(20);
            EEPROM_WriteByte(calib[2].control1,94);CyDelay(20);
            EEPROM_WriteByte(extraeDecimal(calib[2].control1),95);CyDelay(20);
            EEPROM_WriteByte(calib[2].control2,96);CyDelay(20);
            EEPROM_WriteByte(extraeDecimal(calib[2].control2),97);CyDelay(20);
            EEPROM_WriteByte(calib[2].control3,98);CyDelay(20);
            EEPROM_WriteByte(extraeDecimal(calib[2].control3),99);CyDelay(20);
         }
        
        //for(i=0;i<2;i++){
            calib[2].m1=(calib[2].control2-calib[2].control1)/(calib[2].patron2-calib[2].patron1);
            calib[2].b1=calib[2].control2-(calib[2].m1*calib[2].patron2);
            calib[2].m2=(calib[2].control3-calib[2].control2)/(calib[2].patron3-calib[2].patron2);
            calib[2].b2=calib[2].control3-(calib[2].m2*calib[2].patron3);
            sprintf(displayStr,"t20.txt=\"m1:%2.2f b1:%2.2f m2:%2.2f b2:%2.2f\"",calib[2].m1,calib[2].b1,calib[2].m2,calib[2].b2);
            escribeDisplay2(displayStr); 
        //}
        
         if(Rx_Buffer3[4]==0x2a || Rx_Buffer3[4]==0x2c){//Tecla Arriba Oprimida
            if(Opcion==1){
               calib[2].control1+=0.1;
            }else if(Opcion==2){
               calib[2].patron1+=0.1; 
            }else if(Opcion==3){
               calib[2].control2+=0.1; 
            }else if(Opcion==4){
               calib[2].patron2+=0.1;  
            }else if(Opcion==5){
                calib[2].control3+=0.1;
            }else if(Opcion==6){
                calib[2].patron3+=0.1; 
            }/*else if(Opcion==7){
                calib[1].control1+=0.1;
            }else if(Opcion==8){
                calib[1].patron1+=0.1; 
            }else if(Opcion==9){
                calib[1].control2+=0.1;
            }else if(Opcion==10){
                calib[1].patron2+=0.1; 
            }else if(Opcion==11){
                calib[1].control3+=0.1;
            }else if(Opcion==12){
                calib[1].patron3+=0.1; 
            }*/
            if(Rx_Buffer3[4]==0x2c){
                Rx_Buffer3[4]=0x00;
                Rx_Buffer4[4]=0x00;  
                CyDelay(100);
            }
         }
         
         if(Rx_Buffer3[4]==0x2b || Rx_Buffer3[4]==0x2d){//Tecla Abajo Oprimida
            if(Opcion==1){
               calib[2].control1-=0.1;
            }else if(Opcion==2){
               calib[2].patron1-=0.1; 
            }else if(Opcion==3){
               calib[2].control2-=0.1; 
            }else if(Opcion==4){
               calib[2].patron2-=0.1;  
            }else if(Opcion==5){
                calib[2].control3-=0.1;
            }else if(Opcion==6){
                calib[2].patron3-=0.1; 
            }/*else if(Opcion==7){
                calib[1].control1-=0.1;
            }else if(Opcion==8){
                calib[1].patron1-=0.1; 
            }else if(Opcion==9){
                calib[1].control2-=0.1;
            }else if(Opcion==10){
                calib[1].patron2-=0.1; 
            }else if(Opcion==11){
                calib[1].control3-=0.1;
            }else if(Opcion==12){
                calib[1].patron3-=0.1; 
            }*/
            if(Rx_Buffer3[4]==0x2d){
                Rx_Buffer3[4]=0x00;
                Rx_Buffer4[4]=0x00;
                CyDelay(100);
            }
         }
        
        sprintf(displayStr,"t0.txt=\"%2.1f°C\"",calib[2].control1);
        escribeDisplay2(displayStr);
        sprintf(displayStr,"t1.txt=\"%2.1f°C\"",calib[2].patron1);
        escribeDisplay2(displayStr);
        sprintf(displayStr,"t2.txt=\"%2.1f°C\"",calib[2].control2);
        escribeDisplay2(displayStr);
        sprintf(displayStr,"t3.txt=\"%2.1f°C\"",calib[2].patron2);
        escribeDisplay2(displayStr);
        sprintf(displayStr,"t4.txt=\"%2.1f°C\"",calib[2].control3);
        escribeDisplay2(displayStr);
        sprintf(displayStr,"t5.txt=\"%2.1f°C\"",calib[2].patron3);
        escribeDisplay2(displayStr);
        /*sprintf(displayStr,"t6.txt=\"%2.1f°C\"",calib[1].control1);
        escribeDisplay2(displayStr);
        sprintf(displayStr,"t7.txt=\"%2.1f°C\"",calib[1].patron1);
        escribeDisplay2(displayStr);
        sprintf(displayStr,"t8.txt=\"%2.1f°C\"",calib[1].control2);
        escribeDisplay2(displayStr);
        sprintf(displayStr,"t9.txt=\"%2.1f°C\"",calib[1].patron2);
        escribeDisplay2(displayStr);
        sprintf(displayStr,"t10.txt=\"%2.1f°C\"",calib[1].control3);
        escribeDisplay2(displayStr);
        sprintf(displayStr,"t11.txt=\"%2.1f°C\"",calib[1].patron3);
        escribeDisplay2(displayStr);*/
      
    }//Menu de Calibracion de Humedad
//_--------------------------------------------------------------------------------------------------------------------------------------------------------          
    
//_--------------------------------------------------------------------------------------------------------------------------------------------------------      
    if(Menu2==6){//Menu de Test
        Test=ON;
        Control=0;
        if(Rx_Buffer3[4]==0x21){
            escribeDisplay2("page Ajustes");
            Test=OFF;
            Rx_Buffer3[4]=0x00;
            Rx_Buffer4[4]=0x00;   
        }
        
        if(Rx_Buffer3[4]==0x08){
            estadoSSR=!estadoSSR;
            Rx_Buffer3[4]=0x00;
            Rx_Buffer4[4]=0x00;   
        }
        
        if(Rx_Buffer3[4]==0x13){
            BUZZER_Write(!BUZZER_Read());
            Rx_Buffer3[4]=0x00;
            Rx_Buffer4[4]=0x00;   
        }
        
        if(Rx_Buffer3[4]==0x01){
            CR1_Write(!CR1_Read());
            Rx_Buffer3[4]=0x00;
            Rx_Buffer4[4]=0x00;   
        }
        
        if(Rx_Buffer3[4]==0x02){
            CR2_Write(!CR2_Read());
            Rx_Buffer3[4]=0x00;
            Rx_Buffer4[4]=0x00;   
        }
        
        if(Rx_Buffer3[4]==0x03){
            CR3_Write(!CR3_Read());
            Rx_Buffer3[4]=0x00;
            Rx_Buffer4[4]=0x00;   
        }
        
        if(Rx_Buffer3[4]==0x04){
            CR4_Write(!CR4_Read());
            Rx_Buffer3[4]=0x00;
            Rx_Buffer4[4]=0x00;   
        }
        
        if(Rx_Buffer3[4]==0x05){
            CR5_Write(!CR5_Read());
            Rx_Buffer3[4]=0x00;
            Rx_Buffer4[4]=0x00;   
        }
        
        if(Rx_Buffer3[4]==0x06){
            CR6_Write(!CR6_Read());
            Rx_Buffer3[4]=0x00;
            Rx_Buffer4[4]=0x00;   
        }
        
        if(Rx_Buffer3[4]==0x07){
            CR7_Write(!CR7_Read());
            Rx_Buffer3[4]=0x00;
            Rx_Buffer4[4]=0x00;   
        }
        
        if(Rx_Buffer3[4]==0x12){
            CR8_Write(!CR8_Read());
            Rx_Buffer3[4]=0x00;
            Rx_Buffer4[4]=0x00;   
        }
        
        if(!flagControlT){
            if(estadoSSR){
                PWM_WriteCompare1(35000);
                PWM_WriteCompare2(0);
                escribeDisplay2("b7.bco=1024");
            }else{
                PWM_WriteCompare1(0);
                PWM_WriteCompare2(0);
                escribeDisplay2("b7.bco=63488");
            }
        }
        
        if(BUZZER_Read()==1){
            escribeDisplay2("b12.bco=1024");
        }else{
            escribeDisplay2("b12.bco=63488");
        }
        
        if(CR1_Read()==1){
            escribeDisplay2("b0.bco=1024");
        }else{
            escribeDisplay2("b0.bco=63488");
        }
        
        if(CR2_Read()==1){
            escribeDisplay2("b1.bco=1024");
        }else{            
            escribeDisplay2("b1.bco=63488");
        }
        
        if(CR3_Read()==1){            
            escribeDisplay2("b2.bco=1024");
        }else{            
            escribeDisplay2("b2.bco=63488");
        }
        
        if(CR4_Read()==1){            
            escribeDisplay2("b3.bco=1024");
        }else{            
            escribeDisplay2("b3.bco=63488");
        }
        
        if(CR5_Read()==1){            
            escribeDisplay2("b4.bco=1024");
        }else{            
            escribeDisplay2("b4.bco=63488");
        }
        
        if(CR6_Read()==0){            
            escribeDisplay2("b5.bco=1024");
        }else{            
            escribeDisplay2("b5.bco=63488");
        }
        
        if(CR7_Read()==0){            
            escribeDisplay2("b6.bco=1024");
        }else{            
            escribeDisplay2("b6.bco=63488");
        }
        
        if(CR8_Read()==1){            
            escribeDisplay2("b11.bco=1024");
        }else{            
            escribeDisplay2("b11.bco=63488");
        }
        
        if(DOOR_Read()==0)
            escribeDisplay2("b8.bco=1024");
        else
            escribeDisplay2("b8.bco=63488");
        
        if(TERM_Read()==0)
            escribeDisplay2("b9.bco=1024");
        else
            escribeDisplay2("b9.bco=63488");    
        
        if(LEVEL_Read()==0){
            escribeDisplay2("b10.bco=1024");
            CR8_Write(0);
        }else{
            escribeDisplay2("b10.bco=63488");        
        }
            
      sprintf(displayStr,"b0.txt=\"T3:%ld \"",Temperature3);
      escribeDisplay2(displayStr);
      sprintf(displayStr,"b1.txt=\"P:%ld \"",Pressure);
      escribeDisplay2(displayStr);
      sprintf(displayStr,"b2.txt=\"H:%2.2f \"",Humedad);
      escribeDisplay2(displayStr);
      sprintf(displayStr,"b13.txt=\"RTD0:%ld \"",rtdRes[0]);
      escribeDisplay2(displayStr);
      sprintf(displayStr,"b14.txt=\"RTD1:%ld \"",rtdRes[1]);
      escribeDisplay2(displayStr);
      sprintf(displayStr,"b15.txt=\"T1:%1.2f \"",Temperatura);
      escribeDisplay2(displayStr);
      sprintf(displayStr,"b18.txt=\"T35:%ld\"",leeLM35());
      escribeDisplay2(displayStr);
      sprintf(displayStr,"b19.txt=\"T2:%1.2f\"",Temperatura2);
      escribeDisplay2(displayStr);
      sprintf(displayStr,"b20.txt=\"Thum:%1.2f\"",Tam);
      escribeDisplay2(displayStr);
   }//Menu de Test
//_--------------------------------------------------------------------------------------------------------------------------------------------------------          
    
//_--------------------------------------------------------------------------------------------------------------------------------------------------------      
    if(Menu2==7){ //Menu de Configuración de Fecha y Hora
         if(Rx_Buffer3[4]==0x0a){//Selecciono Hora
            Opcion=4;
         }
         
         if(Rx_Buffer3[4]==0x0b){//Selecciono Minuto
            Opcion=5;
         }
         
         if(Rx_Buffer3[4]==0x0c){//Selecciono Dia
            Opcion=1;
         }
         
         if(Rx_Buffer3[4]==0x0d){//Selecciono Hora
            Opcion=2;
         }
         
         if(Rx_Buffer3[4]==0x0e){//Selecciono Year
            Opcion=3;
         }
         
         if(Opcion>5)
            Opcion=1;
         if(Opcion<1)
            Opcion=5;
            
         if(Rx_Buffer3[4]==0x11){//Selecciono Regresar
            escribeDisplay2("page Ajustes");
            
            if(esBisiesto(YearTx))
               Modulo=Bisiesto[MesTx];
            else
               Modulo=Regular[MesTx];
         
            dowTx=((YearTx-1)%7+((YearTx-1)/4-3*((YearTx-1)/100+1)/4)%7+Modulo+DiaTx%7)%7;
            rtc_set_datetime(DiaTx,MesTx,YearTx,dowTx,HoraTx,MinutoTx);
         }
         
         if(Rx_Buffer3[4]==0x2a){//Tecla Arriba Oprimida
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
               Rx_Buffer3[4]=0x00;
               Rx_Buffer4[4]=0x00;
            
         }
         
         if(Rx_Buffer3[4]==0x2b){//Tecla Abajo Oprimida
               if(Opcion==1){
                  if(DiaTx>1)
                     DiaTx--;
                  else{
                    DiaTx=31;
                    LimitaDia();
                  }
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
               Rx_Buffer3[4]=0x00;
               Rx_Buffer4[4]=0x00;
         }
      LimitaDia(); 
      sprintf(displayStr,"thora.txt=\"%02u\"",HoraTx);
      escribeDisplay2(displayStr);
      sprintf(displayStr,"tminutos.txt=\"%02u\"",MinutoTx);
      escribeDisplay2(displayStr);
      sprintf(displayStr,"tdia.txt=\"%02u\"",DiaTx);
      escribeDisplay2(displayStr);
      sprintf(displayStr,"tmes.txt=\"%02u\"",MesTx);
      escribeDisplay2(displayStr);
      sprintf(displayStr,"tyear.txt=\"%02u\"",YearTx);
      escribeDisplay2(displayStr); 
    }//Menu de Configuración de Fecha y Hora
//_--------------------------------------------------------------------------------------------------------------------------------------------------------       
    
//_--------------------------------------------------------------------------------------------------------------------------------------------------------
     if(Menu2==100){ //Menu de Configuración de Parametros de Control
      config=OFF;
      if(Opcion2>17)
         Opcion2=1;
      if(Opcion2<1)
         Opcion2=17;
      
      if(Opcion2==1){
         escribeDisplay2("Config.t2c.txt=\"Proporcional\"");      
         sprintf(displayStr,"t3c.txt=\"%u\"",(uint8)GananciaR);
         escribeDisplay2(displayStr);
         
         if(Rx_Buffer3[4]==0x0a){//Tecla Arriba Oprimida
            if(Rx_Buffer3[4]==0x0a){
               GananciaR+=1.0;
               Rx_Buffer3[4]=0x00;  
               Rx_Buffer4[4]=0x00;            
            }
         }
            
         if(Rx_Buffer3[4]==0x0b){//Tecla Abajo Oprimida
            if(Rx_Buffer3[4]==0x0b){
               GananciaR-=1.0;
               Rx_Buffer3[4]=0x00;
               Rx_Buffer4[4]=0x00;            
            }
         }
         
         if(GananciaR>20)
            GananciaR=1;
         if(GananciaR<1)
            GananciaR=20;
      }else if(Opcion2==2){
         escribeDisplay2("Config.t2c.txt=\"Derivativo\"");
         sprintf(displayStr,"t3c.txt=\"%u\"",Derivativo);
         escribeDisplay2(displayStr); 
        
         if(Rx_Buffer3[4]==0x0a){//Tecla Arriba Oprimida            
            if(Rx_Buffer3[4]==0x0a){
               Derivativo++;
               Rx_Buffer3[4]=0x00;  
               Rx_Buffer4[4]=0x00;                           
            }
         }
            
         if(Rx_Buffer3[4]==0x0b){//Tecla Abajo Oprimida
            if(Rx_Buffer3[4]==0x0b){
               Derivativo--;
               Rx_Buffer3[4]=0x00;
               Rx_Buffer4[4]=0x00;                           
            }
         }
         
         if(Derivativo>80)
            Derivativo=1;
         if(Derivativo<1)
            Derivativo=80;
      }else if(Opcion2==3){
         escribeDisplay2("Config.t2c.txt=\"Integral\"");
         sprintf(displayStr,"t3c.txt=\"%u\"",Integral);
         escribeDisplay2(displayStr); 
         if(Rx_Buffer3[4]==0x0a){//Tecla Arriba Oprimida
            if(Rx_Buffer3[4]==0x0a){
               Integral++;
               Rx_Buffer3[4]=0x00;  
               Rx_Buffer4[4]=0x00;            
            }
         }
            
         if(Rx_Buffer3[4]==0x0b){//Tecla Abajo Oprimida            
            if(Rx_Buffer3[4]==0x0b){
               Integral--;
               Rx_Buffer3[4]=0x00;
               Rx_Buffer4[4]=0x00;                           
            }
         }
         
         if(Integral>4)
            Integral=1;
         if(Integral<1)
            Integral=4;
      }else if(Opcion2==4){
         escribeDisplay2("Config.t2c.txt=\"Tciclo\"");
         sprintf(displayStr,"t3c.txt=\"%u\"",Tciclo);
         escribeDisplay2(displayStr); 
         if(Rx_Buffer3[4]==0x0a){//Tecla Arriba Oprimida            
            if(Rx_Buffer3[4]==0x0a){
               Tciclo+=1.0;
               Rx_Buffer3[4]=0x00;  
               Rx_Buffer4[4]=0x00;            
            }
         }
            
         if(Rx_Buffer3[4]==0x0b){//Tecla Abajo Oprimida
            if(Rx_Buffer3[4]==0x0b){
               Tciclo-=1.0;
               Rx_Buffer3[4]=0x00;
               Rx_Buffer4[4]=0x00;            
            }
         }
         
         if(Tciclo>20)
            Tciclo=5;
         if(Tciclo<5)
            Tciclo=20;
      }else if(Opcion2==5){
         escribeDisplay2("Config.t2c.txt=\"Ajuste +\"");  
         sprintf(displayStr,"t3c.txt=\"%u\"",Ajuste);
         escribeDisplay2(displayStr); 
         if(Rx_Buffer3[4]==0x0a){//Tecla Arriba Oprimida            
            if(Rx_Buffer3[4]==0x0a){
               if(Ajuste<100)
                  Ajuste++;
               Rx_Buffer3[4]=0x00;  
               Rx_Buffer4[4]=0x00;            
            }
         }
            
         if(Rx_Buffer3[4]==0x0b){//Tecla Abajo Oprimida            
            if(Rx_Buffer3[4]==0x0b){
               if(Ajuste>0)
                  Ajuste--;
               Rx_Buffer3[4]=0x00;
               Rx_Buffer4[4]=0x00;            
            }
         }
      }else if(Opcion2==6){
         escribeDisplay2("Config.t2c.txt=\"Ajuste -\"");
         sprintf(displayStr,"t3c.txt=\"%u\"",Ajusten);
         escribeDisplay2(displayStr); 
         if(Rx_Buffer3[4]==0x0a){//Tecla Arriba Oprimida            
            if(Rx_Buffer3[4]==0x0a){
               if(Ajusten<100)
                  Ajusten++;
               Rx_Buffer3[4]=0x00;  
               Rx_Buffer4[4]=0x00;            
            }         
         }
            
         if(Rx_Buffer3[4]==0x0b){//Tecla Abajo Oprimida            
            if(Rx_Buffer3[4]==0x0b){
               if(Ajusten>0)
                  Ajusten--;
               Rx_Buffer3[4]=0x00;
               Rx_Buffer4[4]=0x00;            
            }
         }
      }else if(Opcion2==7){
         escribeDisplay2("Config.t2c.txt=\"Media\"");    
         sprintf(displayStr,"t3c.txt=\"%u\"",MediaMovil);
         escribeDisplay2(displayStr);
         if(Rx_Buffer3[4]==0x0a){//Tecla Arriba Oprimida            
            if(Rx_Buffer3[4]==0x0a){
               if(MediaMovil<80)
                  MediaMovil++;
               Rx_Buffer3[4]=0x00;  
               Rx_Buffer4[4]=0x00;            
            }
         }
            
         if(Rx_Buffer3[4]==0x0b){//Tecla Abajo Oprimida
            if(Rx_Buffer3[4]==0x0b){
               if(MediaMovil>10)
                  MediaMovil--;
               Rx_Buffer3[4]=0x00;
               Rx_Buffer4[4]=0x00;            
            }            
         }
      }else if(Opcion2==8){
         escribeDisplay2("Config.t2c.txt=\"Media Hum\"");    
         sprintf(displayStr,"t3c.txt=\"%u\"",MediaMovil2);
         escribeDisplay2(displayStr);
         if(Rx_Buffer3[4]==0x0a){//Tecla Arriba Oprimida            
            if(Rx_Buffer3[4]==0x0a){
               if(MediaMovil2<80)
                  MediaMovil2++;
               Rx_Buffer3[4]=0x00;  
               Rx_Buffer4[4]=0x00;            
            }
         }
            
         if(Rx_Buffer3[4]==0x0b){//Tecla Abajo Oprimida            
            if(Rx_Buffer3[4]==0x0b){
               if(MediaMovil2>10)
                  MediaMovil2--;
               Rx_Buffer3[4]=0x00;
               Rx_Buffer4[4]=0x00;            
            }            
         }
      }else if(Opcion2==9){
         escribeDisplay2("Config.t2c.txt=\"Ajuste2 +\"");  
         sprintf(displayStr,"t3c.txt=\"%u\"",Ajuste2);
         escribeDisplay2(displayStr); 
         if(Rx_Buffer3[4]==0x0a){//Tecla Arriba Oprimida            
            if(Rx_Buffer3[4]==0x0a){
               if(Ajuste2<100)
                  Ajuste2++;
               Rx_Buffer3[4]=0x00;  
               Rx_Buffer4[4]=0x00;            
            }
         }
            
         if(Rx_Buffer3[4]==0x0b){//Tecla Abajo Oprimida            
            if(Rx_Buffer3[4]==0x0b){
               if(Ajuste2>0)
                  Ajuste2--;
               Rx_Buffer3[4]=0x00;
               Rx_Buffer4[4]=0x00;            
            }
         }
      }else if(Opcion2==10){
         escribeDisplay2("Config.t2c.txt=\"Ajuste2 -\"");
         sprintf(displayStr,"t3c.txt=\"%u\"",Ajusten2);
         escribeDisplay2(displayStr); 
         if(Rx_Buffer3[4]==0x0a){//Tecla Arriba Oprimida            
            if(Rx_Buffer3[4]==0x0a){
               if(Ajusten2<100)
                  Ajusten2++;
               Rx_Buffer3[4]=0x00;  
               Rx_Buffer4[4]=0x00;            
            }         
         }
            
         if(Rx_Buffer3[4]==0x0b){//Tecla Abajo Oprimida            
            if(Rx_Buffer3[4]==0x0b){
               if(Ajusten2>0)
                  Ajusten2--;
               Rx_Buffer3[4]=0x00;
               Rx_Buffer4[4]=0x00;            
            }
         }
      }else if(Opcion2==11){
         escribeDisplay2("Config.t2c.txt=\"Muestreo\"");
         sprintf(displayStr,"t3c.txt=\"%03u\"",tmuestreoUSB);
         escribeDisplay2(displayStr); 
         if(Rx_Buffer3[4]==0x0a){//Tecla Arriba Oprimida            
            if(Rx_Buffer3[4]==0x0a){               
                  tmuestreoUSB++;
               Rx_Buffer3[4]=0x00;  
               Rx_Buffer4[4]=0x00;            
            }         
         }
            
         if(Rx_Buffer3[4]==0x0b){//Tecla Abajo Oprimida         
            if(Rx_Buffer3[4]==0x0b){
                  tmuestreoUSB--;
               Rx_Buffer3[4]=0x00;
               Rx_Buffer4[4]=0x00;            
            }
         }
        if(tmuestreoUSB<1)tmuestreoUSB=255;
        if(tmuestreoUSB>254)tmuestreoUSB=1;
      }else if(Opcion2==12){
         escribeDisplay2("Config.t2c.txt=\"T1 Humedad\"");
         sprintf(displayStr,"t3c.txt=\"%02u\"",T1Humedad);
         escribeDisplay2(displayStr); 
         if(T1Humedad>60 || T1Humedad<1)
            T1Humedad=1;
         if(Rx_Buffer3[4]==0x0a){//Tecla Arriba Oprimida
            if(Rx_Buffer3[4]==0x0a){               
                  T1Humedad++;
               Rx_Buffer3[4]=0x00;  
               Rx_Buffer4[4]=0x00;            
            }         
         }
            
         if(Rx_Buffer3[4]==0x0b){//Tecla Abajo Oprimida            
            if(Rx_Buffer3[4]==0x0b){
                  T1Humedad--;
               Rx_Buffer3[4]=0x00;
               Rx_Buffer4[4]=0x00;            
            }
         }
      }else if(Opcion2==13){
         escribeDisplay2("Config.t2c.txt=\"T2 Humedad\"");
         sprintf(displayStr,"t3c.txt=\"%02u\"",T2Humedad);
         escribeDisplay2(displayStr); 
         if(T2Humedad>60 || T2Humedad<4)
            T2Humedad=4;
         if(Rx_Buffer3[4]==0x0a){//Tecla Arriba Oprimida            
            if(Rx_Buffer3[4]==0x0a){               
                  T2Humedad++;
               Rx_Buffer3[4]=0x00;  
               Rx_Buffer4[4]=0x00;            
            }         
         }
            
         if(Rx_Buffer3[4]==0x0b){//Tecla Abajo Oprimida            
            if(Rx_Buffer3[4]==0x0b){
                  T2Humedad--;
               Rx_Buffer3[4]=0x00;
               Rx_Buffer4[4]=0x00;            
            }
         }
      } else if(Opcion2==14){
         escribeDisplay2("Config.t2c.txt=\"T3 Humedad\"");
         sprintf(displayStr,"t3c.txt=\"%02u\"",T3Humedad);
         escribeDisplay2(displayStr); 
         if(T3Humedad>60 || T3Humedad<1)
            T3Humedad=1;
         if(Rx_Buffer3[4]==0x0a){//Tecla Arriba Oprimida            
            if(Rx_Buffer3[4]==0x0a){               
                  T3Humedad++;
               Rx_Buffer3[4]=0x00;  
               Rx_Buffer4[4]=0x00;            
            }         
         }
            
         if(Rx_Buffer3[4]==0x0b){//Tecla Abajo Oprimida            
            if(Rx_Buffer3[4]==0x0b){
                  T3Humedad--;
               Rx_Buffer3[4]=0x00;
               Rx_Buffer4[4]=0x00;            
            }
         }
      } else if(Opcion2==15){
         escribeDisplay2("Config.t2c.txt=\"tDefrost\"");
         sprintf(displayStr,"t3c.txt=\"%03u\"",tDefrost);
         escribeDisplay2(displayStr); 
         if(tDefrost>120)
            tDefrost=1;
         if(Rx_Buffer3[4]==0x0a){//Tecla Arriba Oprimida            
            if(Rx_Buffer3[4]==0x0a){               
                  tDefrost++;
               Rx_Buffer3[4]=0x00;  
               Rx_Buffer4[4]=0x00;            
            }         
         }
            
         if(Rx_Buffer3[4]==0x0b){//Tecla Abajo Oprimida            
            if(Rx_Buffer3[4]==0x0b){
                  tDefrost--;
               Rx_Buffer3[4]=0x00;
               Rx_Buffer4[4]=0x00;            
            }
         }
      }  else if(Opcion2==16){
         escribeDisplay2("Config.t2c.txt=\"tCongela\"");
         sprintf(displayStr,"t3c.txt=\"%03u\"",tCongela);
         escribeDisplay2(displayStr); 
         if(tCongela>120)
            tCongela=1;
         if(tCongela<1)
            tCongela=120;
         if(Rx_Buffer3[4]==0x0a){//Tecla Arriba Oprimida            
            if(Rx_Buffer3[4]==0x0a){               
                  tCongela++;
               Rx_Buffer3[4]=0x00;  
               Rx_Buffer4[4]=0x00;            
            }         
         }
            
         if(Rx_Buffer3[4]==0x0b){//Tecla Abajo Oprimida            
            if(Rx_Buffer3[4]==0x0b){
                  tCongela--;
               Rx_Buffer3[4]=0x00;
               Rx_Buffer4[4]=0x00;            
            }
         }
      }  else if(Opcion2==17){
         escribeDisplay2("Config.t2c.txt=\"tDisplay\"");
         sprintf(displayStr,"t3c.txt=\"%03u\"",tDisplay);
         escribeDisplay2(displayStr); 
         if(tDisplay>60)
            tDisplay=5;
         if(tDisplay<5)
            tDisplay=60;
         if(Rx_Buffer3[4]==0x0a){//Tecla Arriba Oprimida            
            if(Rx_Buffer3[4]==0x0a){               
                  tDisplay++;
               Rx_Buffer3[4]=0x00;  
               Rx_Buffer4[4]=0x00;            
            }         
         }
            
         if(Rx_Buffer3[4]==0x0b){//Tecla Abajo Oprimida            
            if(Rx_Buffer3[4]==0x0b){
                  tDisplay--;
               Rx_Buffer3[4]=0x00;
               Rx_Buffer4[4]=0x00;            
            }
         }
      }           
      
      if(Rx_Buffer3[4]==0x0c){//Tecla Izquierda Oprimida        
         if(Rx_Buffer3[4]==0x0c){
            Opcion2++;
            Rx_Buffer3[4]=0x00;  
            Rx_Buffer4[4]=0x00;            
         }
      }
         
      if(Rx_Buffer3[4]==0x0d){//Tecla Derecha Oprimida         
         if(Rx_Buffer3[4]==0x0d){
            Opcion2--;
            Rx_Buffer3[4]=0x00;
            Rx_Buffer4[4]=0x00;            
         }
      }
      
      if(Rx_Buffer3[4]==0x11){//Tecla Regresar
         CyDelay(20);
         if(Rx_Buffer3[4]==0x11){
            if(!GuardaEEPROM){
                
               EEPROM_WriteByte(convert8(tDefrost,0),5);CyDelay(10);
               EEPROM_WriteByte(convert8(tDefrost,1),6);CyDelay(10);
               EEPROM_WriteByte(convert8(tCongela,0),7);CyDelay(10);
               EEPROM_WriteByte(convert8(tCongela,1),8);CyDelay(10);
               EEPROM_WriteByte(tDisplay,9);
               CyDelay(10);   
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
            escribeDisplay2("page Ajustes");
         }
      }
    }//Menu de Configuración de Parametros de Control
//_--------------------------------------------------------------------------------------------------------------------------------------------------------           
//_-------------------------------------------------------Display 2----------------------------------------------------------------------------------------                
    }
}
