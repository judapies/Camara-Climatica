//Falta crear menu de ajuste de variables y de norma CFR21

#include <18F4685.h>
#device adc=10
#device HIGH_INTS=TRUE //Activamos niveles de prioridad
#fuses HS,WDT4096,NOPROTECT,NOLVP,NODEBUG,NOSTVREN,PUT,NOCPD,NOWRT,NOIESO,NOFCMEN,NOPBADEN,NOWRTC,NOWRTB,NOEBTR,NOEBTRB,NOCPB,NOXINST,NOLPT1OSC,NOMCLR,NOBROWNOUT,BORV43
#use delay(clock=20000000)
#use rs232(baud=115200, xmit=PIN_C6, rcv=PIN_C7, bits=8, parity=N,BRGH1OK,ERRORS)
#use i2c(Master,slow,sda=PIN_E2,scl=PIN_E1)
#priority rda
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <DS1302.c>
#include <24512.c>

#define desvio                          0.5
#define desvioH                         2.0
#define aa                              -2.4560
#define bb                              19.801
#define cc                              -73.195
#define dd                              122.55;  

#define Tmax                    70.0
#define Tmin                    0.0
#define Hmax                    90.0
#define Hmin                    10.0

#define tmuestreo                       300.0

#define tiempoLlenado                   300   

#define   DOOR          !input(PIN_E0)
#define   TERM          !input(PIN_A4)

#define   SSR1_on          output_bit(PIN_C1,1)
#define   SSR1_off         output_bit(PIN_C1,0)

#define   SSR2_on          output_bit(PIN_C2,1)
#define   SSR2_off         output_bit(PIN_C2,0)

#define   AC2_on           output_bit(PIN_D1,0)
#define   AC2_off          output_bit(PIN_D1,1)

#define   AC3_on           output_bit(PIN_D2,0)
#define   AC3_off          output_bit(PIN_D2,1)

#define   AC4_on           output_bit(PIN_D3,1)
#define   AC4_off          output_bit(PIN_D3,0)

#define   DC1_on           output_bit(PIN_B2,1)//Deshumidificador
#define   DC1_off          output_bit(PIN_B2,0)

#define   DC2_on           output_bit(PIN_B3,1)//Humidificador
#define   DC2_off          output_bit(PIN_B3,0)

#define   DC3_on           output_bit(PIN_B4,1)//Ventilador Humidificador
#define   DC3_off          output_bit(PIN_B4,0)

#define   DC4_on           output_bit(PIN_B5,1)//Ventilador Interno Peltier
#define   DC4_off          output_bit(PIN_B5,0)

#define   DC5_on           output_bit(PIN_B6,1)
#define   DC5_off          output_bit(PIN_B6,0)

#define   DC6_on           output_bit(PIN_B7,1)//Ventilador Externo Peltier
#define   DC6_off          output_bit(PIN_B7,0)

#define   Buzzer_on        output_bit(PIN_C0,1)
#define   Buzzer_off       output_bit(PIN_C0,0)

#define   Display_on       output_bit(PIN_D4,1)
#define   Display_off      output_bit(PIN_D4,0)

#define   AC1_on           output_bit(PIN_D0,0)
#define   AC1_off          output_bit(PIN_D0,1)


#define OFF 0
#define ON  1
#define anchoGrafica  40.0
#define Control_PWM  1

//Comunicacion//------------------------
byte SlaveB0Tx[0X20];
byte SlaveC0Tx[0X20];
byte SlaveB0Rx[0X20];
byte dato=0;            //Contendr� la informaci�n a enviar o recibir
byte direccion=0;       //Contendr� la direcci�n del esclavo en el bus I2C
byte posicion=0;        //Contendr� la posici�n en el buffer de memoria del esclavo donde se almacenar� o de donde se recuperar� el dato
//Comunicacion//------------------------

int8 Regular[12]={0,3,3,6,1,4,6,2,5,0,3,5};
int8 Bisiesto[12]={0,3,4,0,2,5,0,3,6,1,4,6};

short desconectoTemperatura=OFF,desconectoHumedad=OFF,graficaa=OFF,flagMotor=OFF,Humidificar=0,dehumidificador=0,humidificador=0,flagHumedad=0,flagAlarma=0,flagBuzzer=0;
short flagSobrepasoHumedad=0,estadoVentilador=0,flagTem=0,flagHum=0,alarmaT=0,alarmaH=0,entroRampa=OFF,Controlando=OFF,controlaH=0,calculaMediana=OFF,estadoLuz=OFF;
int8 T1Humedad=0,T2Humedad=0,T3Humedad=0,tDisplay=0,mediaLuces=0,conteoAlarma=0,ajustaIP=0;
int16 tdehumidificador,thumidificador,data=0,data2=0,datacount=0,pwm2=0,tCongela=0,tllenado=0,tCalentamiento=0,thumidificacion=0,tbuzzer=0,ConteoBuzzer=0;

int8 Vel1=0,SetpointH=0,controlL=0,alarma=0,nivelBajo=0,nivelAlto=0,bomba=0,yy=0;  
int8 Opcion2=0,Derivativo=0,Integral=0,MediaMovil=0,MediaMovil2=0,Tciclo=0,Ajuste=0,Ajusten=0,Ajuste2=0,Ajusten2=0,Modulo=0,Ajusto=0;
int16 derivativo_ant=0,T1=0,T3=0,tSobrepaso=0,i=0,n=0,tDisplayHumedad=0,tDefrost2=0,pwm=0,tpwm1=0,tpwm2=0,TcH=0,Tph=0;
float Setpoint=0,SetpointAnt=0,minH=0,maxH=0,alarmaTemperatura=0,alarmaHumedad=0,d6=0,Factor=0.0,tmp3=0,PotenciaH=0;
int32 tGrafica=0;
short dibujo=0,pulsoAumentar=0,pulsoDisminuir=0,flagBorrado=0,alarmaSobreT=0;
short inicioControlT=0,inicioControlH=0,inicioControlL=0;
float Ganancia=0,errorH=0,TpulsoH=0.0,TcicloH=0.0,GananciaH=0,GananciaRH=0;
int32  Temperature2=0;

#define RX_BUFFER_SIZE  10
char Rx_Buffer[RX_BUFFER_SIZE+1];
char Rx_Buffer2[RX_BUFFER_SIZE+1];
int8 RX_Wr_Index=0;

int16 pwm1=0,pwm3=0;
int8 Dato2=0,Dato_Exitoso=0,Menu=0,MenuAntt=0,z=0,TipoClave=0,Segmentos=0,Opcion=0,h=0,l=0;
int8 Dia=0,Mes=0,Year=0,Hora=0,Minuto=0,Segundo=0,dow=0;
short Inicio=0,entro=0,GuardaEEPROM=0,rampa=0,Minutos=0,Horas=0,flagTemperatura=0,x1=0,x10=0,x100=0,Enfria=0,envioUSB=0,flagRed=0,flagControlL=0,flagFinalizo=0;
int8 Password[4];
int8 tiempo_esterilizacion=0,y=0,hEst=0,mEst=0,hEstp=0,mEstp=0,hEstpAnt=0,mEstpAnt=0,Segmento=0,conteo=0,SetpointHAnt;
int16 tiemporeset=0,TiempoControl=0,tiemporecta=0,tiemposetpoint=0,tinicio=0,tiempocalentamiento=0,tiempoup=0,tiempodn=0,CicloLibre=0;
short mensaje=0,Lectura2=0,flag_setpoint=0,flag_tiempocalentamiento=0,Lectura=0,flagup=0,flagdn=0,flagRampa=0,flag_calcula=0,flag_ajuste=0,flag_ajuste2=0;
short Finalizo=0,conteoPregunta=0,imprime=0,Infinito=0,flagControlT=0,flagControlH=0,flag_clear2=0,flag_ganancia=0,flag_clear=0;
float X[2]={0.0,0.0};
float medianaHumedad[2]={0.0,0.0};
float Temperatura=0,Temperatura2=0,Aumento=0,PendienteActual=0,DifPendiente=0,TempeInicial=0,Pendiente=0,TemperaturaIdeal=0,PotenciaEntregada=0,Tpulso=0,penInicial=0,penMufla=0.5;
float error=0,Potencia=0,TpulsoMax=0,Tpulsomin=0,humedadDisplay=0;
int16 TiempoActual=0,MinutosActual=0,HorasActual=0,Tiempo=0,tPregunta=0,tControlH=0,tLuz=0,tLuzProgramada=0;
int8 GananciaR=0,Pausa=0,contH=0;
float Prom=0,Prom2=0,V0=0,V1=0,Temperature=0,TemperaturaPt1002=0.0,Pt=0.0,Pt2=0.0,TiempoRampa=0,Tam=0,RH2=0,Histeresis=0.2,medianaH=0.0;
int8 YearTx=0,MesTx=0,DiaTx=0,HoraTx=0,MinutoTx=0,dowTx=0,f=0;

int16 filt=0,Programa=0;
int8 entero=0;
int8 ll=0,hh=0;
int16 tmp=0;
float Temporal=0,Humedad=0;
int8 k=0,kk=0,tEnvioUSB=0,tmuestreoUSB=0,tmuestreoUSB_ant=0,controlH=0,controlT=0,controlR=0,VelMotor=0;

int16 promedioT1[80];
int16 promedioT2[80];
int16 promediolm35[20];
float promedioTa[80];
float promedioH[80];

int16 tiempos=0,tiempos3=0,tiempoctrl=0,tTermostato=0;
short Control=0,Test=0;
short ip1=0,ip2=0,ip3=0,ip4=0,s1=0,s2=0,s3=0,s4=0,g1=0,g2=0,g3=0,g4=0,m1=0,m2=0,m3=0,m4=0,m5=0,m6=0,entroGrafica=OFF,actualizaNumeracion=OFF,config=OFF;
int8 horaGrafica[15],minutoGrafica[15];
float a=0.0011,b=2.3302,c=-244.0723;

int8 IP[4]={192,168,0,118};
int8 Gateway[4]={192,168,0,1};
int8 Subnet[4]={255,255,255,0};
int8 MAC[6]={0,0x20,0x30,0x40,0x50,0x60};

typedef struct{
   int8 TemperaturaDec;
   int8 TemperaturaInt;
   int8 Horas;
   int8 Minutos;
   float Temperatura;
   int8 Humedad;
   
}Personalizado;
Personalizado rampas[24];

typedef struct{
   int8 Temperatura;
   int8 Hora;
   int8 Minuto;
   int8 Humedad;
}Grafica;
Grafica graph[330];
/*
typedef struct{
   float control1,control2,control3,m1,m2,m3,patron1,patron2,patron3,b1,b2,b3;
}Calibracion;
Calibracion calib[3];
*/
#include "Funciones.h"
#include "Menu.h"

#int_rda HIGH
void serial_rx_isr(){

   Dato2=fgetc();
   if(Dato2==0x65){//Inicio Comunicacion
      Inicio=1;
      RX_Wr_Index =0;
   }
   //Inicio=1;
   if(Inicio==1){
      Rx_Buffer2[RX_Wr_Index] = Dato2;
      RX_Wr_Index++;
   }
   if(RX_Wr_Index >= RX_BUFFER_SIZE){
      RX_Wr_Index =0;
      Inicio=0;
   }

   if(RX_Wr_Index==0){
      if(Rx_Buffer2[0]==0x65 && Rx_Buffer2[1]==0xff && Rx_Buffer2[2]==0xff && Rx_Buffer2[8]==0x00 && Rx_Buffer2[9]==0xff ){
         Dato_Exitoso=5;
      }else{
         Dato_Exitoso=10;
      }
   }
   //output_toggle(PIN_C0);
}
#int_TIMER1 
void temp1s(void){
   set_timer1(40536); // Se repite cada 5ms 
   tiempos++;
   tiempos3++;
   tiempoctrl++; 
   
   if(!TERM){
      if(tTermostato<3000)
         tTermostato++;
   }else{
      tTermostato=0;
   }
   
   if(controlaH==1){
      if(tiempoctrl<TpH){ // Si el tiempo es menor o igual al tiempo de control se enciende la resistencia de lo contrario se apaga.
         CR2_Write(1);//Humidificador_on;
         DC3_on;//Ventilador Humidificador
      }else{
         CR2_Write(0);//Humidificador_off;
         DC3_off;//Ventilador Humidificador
      }
            
      if(tiempoctrl>=TcH) // Si el tiempo de control es mayor al tiempo de ciclo se reinicia tiempo de control
         tiempoctrl=0;// tiempoctrl incrementa una unidad cada 5ms, por eso se dividen los tiempos por 0.005.
   }
   
   if(flagAlarma)
      tbuzzer++;
   
    if(tiempos3>=20){
        Lectura2=1;
        if(flagup)
            tiempoup++;
        if(flagdn)
            tiempodn++;   
        tiempos3=0;
    }
    
    if(tiempos>=200){  // Cuenta cada 5ms   
        tiempos=0;Lectura=1;tiemporeset++;TiempoControl++;tiemporecta++;mensaje=!mensaje;envioUSB=1;
        
        if(estadoLuz)
            tLuz++;
        else
            tLuz=0;
        
        tControlH++;
        
        if(Menu==20 && !flagTemperatura && !Infinito && flagControlT){
            tCalentamiento++;
         }else{
            tCalentamiento=0;
         }
         
         if(Menu==20 && !flagHumedad && !Infinito && flagControlH){
            thumidificacion++;
         }else{
            thumidificacion=0;
         }
         
         if(alarmaT || alarmaH){
            if(conteoAlarma<2){
               Buzzer_on;
            }else if(conteoAlarma<=5 && conteoAlarma>=1){
               Buzzer_off;
            }else if(conteoAlarma>5){
               conteoAlarma=0;
            }
            conteoAlarma++;
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

#ifdef Control_PWM
#int_TIMER2
void temp2s(void){
   set_timer2(206); // Se repite cada 5ms 
   tpwm1++;
   tpwm2++;
   
   if(tpwm1>=60) // Si el tiempo de control es mayor al tiempo de ciclo se reinicia tiempo de control
      tpwm1=0;
      
   if(tpwm1<pwm1) // Si el tiempo es menor o igual al tiempo de control se enciende la resistencia de lo contrario se apaga.
      output_bit(PIN_C2,1);
   else         
      output_bit(PIN_C2,0);      
      
   if(tpwm2>=60) // Si el tiempo de control es mayor al tiempo de ciclo se reinicia tiempo de control
      tpwm2=0;
      
   if(tpwm2<pwm2) // Si el tiempo es menor o igual al tiempo de control se enciende la resistencia de lo contrario se apaga.
      output_bit(PIN_C1,1);
   else         
      output_bit(PIN_C1,0);         
}
#endif

void main()
{ 
    Buzzer_off;AC1_off;AC2_off;AC3_off;AC4_off;DC1_off;DC2_off;DC3_off;DC4_off;DC5_Off;DC6_Off;
    delay_ms(1000);
    Display_off;
    enable_interrupts(int_rda);
    enable_interrupts(global);    
    setup_timer_1(T1_INTERNAL|T1_DIV_BY_1);
    enable_interrupts(INT_TIMER1);
    #ifdef Control_PWM
    setup_timer_2(T2_DIV_BY_1, 206, 1);
    enable_interrupts(INT_TIMER2);
    #endif
    setup_adc_ports(AN0_TO_AN4);
    setup_adc(ADC_CLOCK_DIV_32 ); 
    delay_ms(1);
    setup_wdt(WDT_ON);
    delay_ms(1);
    Display_on;
    rtc_init();
    delay_ms(1);
    for (posicion=0;posicion<0x10;posicion++){
       SlaveB0Tx[posicion] = 0x00;
       SlaveC0Tx[posicion] = 0x00;
    }
    
    delay_ms(100);
    penInicial=penMufla;
    Opcion=1;Control=0;
    
    BUZZER_Write(1);
    LeeEEPROM();
    LimitaValores();
    BUZZER_Write(0);
    enable_interrupts(int_rda);
    enable_interrupts(global);
    
    for(i=0;i<80;i++){
        Temperatura=leePt100(MediaMovil);
        Temperatura2=leePt1002(MediaMovil);
        Humedad=leeHumedad(MediaMovil2);
        Temperature2=(int16)leeLM35(10);
    }
    medianaH=Humedad;
    medianaHumedad[0]=Humedad;
    medianaHumedad[1]=Humedad;
    humedadDisplay=Humedad;
    minH=Humedad;
    maxH=Humedad;
    delay_ms(1);
    for(i=0;i<24;i++){
         if(rampas[i].Horas==0 && rampas[i].Minutos==0){
            Segmentos=i;  
            break;
         }else{
            Segmentos=i+1;  
         }
    } 
    
    PWM_WriteCompare1(0);
    PWM_WriteCompare2(0);
    if(controlT==0 && controlH==0 && controlR==0 && controlL==0){
        Menu=120;
        flagControlT=OFF;
        flagControlH=OFF;      
        flagControlL=OFF;
        flagRampa=OFF;
        
        printf("page Bienvenida");
        SendDataDisplay();
        printf("bkcmd=0");
        SendDataDisplay();
        
        delay_ms(10);
    }else{
        if(controlR==1)
            flagRampa=ON;
        else
            flagRampa=OFF;
        
        if(controlT==1){
            flagControlT=ON;
            inicioControlT=ON;
        }else{
            flagControlT=OFF;
            inicioControlT=OFF;
        }
        
        if(controlH==1){
            flagControlH=ON;
            inicioControlH=ON;
        }else{
            flagControlH=OFF;
            inicioControlH=OFF;
        }
        
        if(controlL==1){
            flagControlL=ON;
            inicioControlL=ON;
        }else{
            flagControlL=OFF;
            inicioControlL=OFF;
        }
        
        T1=(int8)(((float)T1Humedad)*((float)SetpointH/80.0));
        T3=(int8)(((float)T3Humedad)*((float)SetpointH/80.0));
        GananciaH=GananciaRH*(SetpointH/80);
        if(Setpoint<Temperatura){
            Enfria=1;
            Ganancia=GananciaR*(Setpoint/50);
            //Ganancia=4;
        }else{
            Enfria=0;
            Ganancia=GananciaR*(Setpoint/200);
        }
        
        if((mEstp==0)&&(hEstp==0))
            Infinito=1;
        else
            Infinito=0;
        
        printf("page Principal");
        SendDataDisplay();
        printf("bkcmd=0");
        SendDataDisplay();
        delay_ms(10);
        Menu=20;
    }
   
    rtc_get_date(Dia,Mes,Year,dow);
    rtc_get_time(Hora,Minuto,Segundo);
    graph[0].Minuto=Minuto;
    graph[0].Hora=Hora;
    tmuestreoUSB_ant=tmuestreoUSB;
    for(i=0;i<80;i++){
        Humedad=leeHumedad(MediaMovil2);
    }
    medianaH=Humedad;
    medianaHumedad[0]=Humedad;
    medianaHumedad[1]=Humedad;
    humedadDisplay=Humedad;
    
   while(true)
   {
      if(tLuz>(tLuzProgramada*60)){
         estadoLuz=OFF;
      }
      
      Programa=1;
      Display_on;
      rtc_get_date(Dia,Mes,Year,dow);
      rtc_get_time(Hora,Minuto,Segundo);
      Display_on;
      restart_wdt();
      Lectura_Esclavos();
      delay_ms(1);
      Envio_Esclavos();
            
      if(Lectura2){
          Lectura2=0;
          Temperatura=leePt100(MediaMovil);
          Temperatura2=leePt1002(MediaMovil);
          Temperature2=(int16)leeLM35(10);
          Humedad=leeHumedad(MediaMovil2);
          
          if(!Test){            
               AC1_on;//Prende Ventilador que refrigera lo electronico
               if(estadoLuz){
                  AC2_on;
               }else{
                  AC2_off;
               }
          }
          
      }
      
      //dataEthernet();
      dataUSB();
      LeeDisplay();
      leeMenus(); 

   //_--------------------------------------------------------------------------------------------------------------------------------------------------------
        if(Menu == 120){ //Menu de Bienvenida.
            printf("bkcmd=0");
            SendDataDisplay();
            delay_ms(10);
            printf("page Clave");
            SendDataDisplay();
        }
//_--------------------------------------------------------------------------------------------------------------------------------------------------------
     
//_-------------------------------------------------------Display 1----------------------------------------------------------------------------------------        
//_--------------------------------------------------------------------------------------------------------------------------------------------------------
    if(Menu==0){ //Menu de Contrase�a de acceso.
      //ApagaSalidas();
      entro=0;
      if(Rx_Buffer[4]==0x11){//11
         if(TipoClave!=0){
            printf("page Principal");
            SendDataDisplay();
         }
      }
      
      if(TipoClave==3){
         printf("titulo.txt=\"Clave Nueva\"");
         SendDataDisplay();
         printf("doevents");
         SendDataDisplay();
      }else if(TipoClave==2){
         printf("titulo.txt=\"Clave Actual\"");         
         SendDataDisplay();
         printf("doevents");
         SendDataDisplay();
      }else if(TipoClave==1 || TipoClave==4 || TipoClave==5){
         printf("titulo.txt=\"Clave Tecnico\"");         
         SendDataDisplay();
         printf("doevents");         
         SendDataDisplay();
      }else if(TipoClave==0){
        printf("titulo.txt=\"Ingrese Clave\"");
        SendDataDisplay();
      }
      
      if(Rx_Buffer[3]==0x0f){//0f, recibe caracteres ingresados desde el Display
         if(TipoClave==0){
            if(Rx_Buffer[4]>=0x30 && Rx_Buffer[5]>=0x30 && Rx_Buffer[6]>=0x30 && Rx_Buffer[7]>=0x30
            && Rx_Buffer[4]<=0x39 && Rx_Buffer[5]<=0x39 && Rx_Buffer[6]<=0x39 && Rx_Buffer[7]<=0x39){
            if(Rx_Buffer[4]==0x33&&Rx_Buffer[5]==0x38&&Rx_Buffer[6]==0x39&&Rx_Buffer[7]==0x32){ // Si Ingresa clave para reset general del sistema.
               EEPROM_WriteByte(0,10);delay_ms(20);EEPROM_WriteByte(0,11);delay_ms(20);// Reestablece a contrase�a de Fabrica y reinicia Programa.
               EEPROM_WriteByte(0,12);delay_ms(20);EEPROM_WriteByte(0,13);delay_ms(20);
               reset_cpu();
            }
            }
            
            if(Rx_Buffer[4]>=0x30 && Rx_Buffer[5]>=0x30 && Rx_Buffer[6]>=0x30 && Rx_Buffer[7]>=0x30
            && Rx_Buffer[4]<=0x39 && Rx_Buffer[5]<=0x39 && Rx_Buffer[6]<=0x39 && Rx_Buffer[7]<=0x39){
            if((Rx_Buffer[4]==Password[0]+0x30)&&(Rx_Buffer[5]==Password[1]+0x30)&&(Rx_Buffer[6]==Password[2]+0x30)&&(Rx_Buffer[7]==Password[3]+0x30)){
                  printf("page ClaveCorrecta");
                  SendDataDisplay();
                  dibujo=OFF;
                  //RX_Buffer[3]=0x00;RX_Buffer2[3]=0x00;  
               }else{
                  printf("page ClaveBad");
                  SendDataDisplay();
                  //RX_Buffer[3]=0x00;RX_Buffer2[3]=0x00;  
               } 
            }
         }else if(TipoClave==1){
            if(Rx_Buffer[4]>=0x30 && Rx_Buffer[5]>=0x30 && Rx_Buffer[6]>=0x30 && Rx_Buffer[7]>=0x30
            && Rx_Buffer[4]<=0x39 && Rx_Buffer[5]<=0x39 && Rx_Buffer[6]<=0x39 && Rx_Buffer[7]<=0x39){
            if(Rx_Buffer[4]==0x34&&Rx_Buffer[5]==0x34&&Rx_Buffer[6]==0x34&&Rx_Buffer[7]==0x34){ // Si Ingresa clave de Servicio Tecnico
               printf("page Config");
               SendDataDisplay();
            }else{
               printf("page Principal");
               SendDataDisplay();
            }
            }
         }else if(TipoClave==2){
            if(Rx_Buffer[4]>=0x30 && Rx_Buffer[5]>=0x30 && Rx_Buffer[6]>=0x30 && Rx_Buffer[7]>=0x30
            && Rx_Buffer[4]<=0x39 && Rx_Buffer[5]<=0x39 && Rx_Buffer[6]<=0x39 && Rx_Buffer[7]<=0x39){
            if((Rx_Buffer[4]==Password[0]+0x30)&&(Rx_Buffer[5]==Password[1]+0x30)&&(Rx_Buffer[6]==Password[2]+0x30)&&(Rx_Buffer[7]==Password[3]+0x30)){
               printf("page Clave");
               SendDataDisplay();
               printf("titulo.txt=\"Clave Nueva\"");
               SendDataDisplay();
               TipoClave=3;
               Rx_Buffer[3]=0x00;
               Rx_Buffer2[3]=0x00;
            }else{
               printf("page Principal");
               SendDataDisplay();
               //RX_Buffer[3]=0x00;
               //RX_Buffer2[3]=0x00;
            } 
            }
         }else if(TipoClave==3){
            printf("titulo.txt=\"Clave Nueva\"");
            SendDataDisplay();
            printf("page Principal");
            SendDataDisplay();
            if(!GuardaEEPROM){
               //write_eeprom(10,Rx_Buffer[4]-0x30);delay_ms(20);write_eeprom(11,Rx_Buffer[5]-0x30);delay_ms(20);
               EEPROM_WriteByte(Rx_Buffer[4]-0x30,10);delay_ms(20);
               EEPROM_WriteByte(Rx_Buffer[5]-0x30,11);delay_ms(20);
               EEPROM_WriteByte(Rx_Buffer[6]-0x30,12);delay_ms(20);
               EEPROM_WriteByte(Rx_Buffer[7]-0x30,13);delay_ms(20);
               GuardaEEPROM=ON;
            }
            Password[0]=Rx_Buffer[4]-0x30;Password[1]=Rx_Buffer[5]-0x30;Password[2]=Rx_Buffer[6]-0x30;Password[3]=Rx_Buffer[7]-0x30;
            //RX_Buffer[3]=0x00;
            //RX_Buffer2[3]=0x00;
         }else if(TipoClave==4){
            if(Rx_Buffer[4]==0x34&&Rx_Buffer[5]==0x34&&Rx_Buffer[6]==0x34&&Rx_Buffer[7]==0x34){ // Si Ingresa clave de Servicio Tecnico
               printf("page Test");
               SendDataDisplay();
            }else{
               printf("page Principal");
               SendDataDisplay();
            }
            //RX_Buffer[3]=0x00;
            //RX_Buffer2[3]=0x00;
         }else if(TipoClave==5){
            if(Rx_Buffer[4]>=0x30 && Rx_Buffer[5]>=0x30 && Rx_Buffer[6]>=0x30 && Rx_Buffer[7]>=0x30
            && Rx_Buffer[4]<=0x39 && Rx_Buffer[5]<=0x39 && Rx_Buffer[6]<=0x39 && Rx_Buffer[7]<=0x39){
            if(Rx_Buffer[4]==0x35&&Rx_Buffer[5]==0x34&&Rx_Buffer[6]==0x35&&Rx_Buffer[7]==0x34){ // Si Ingresa clave de Servicio Tecnico
               printf("page Calib");
               SendDataDisplay();
            }else{
               printf("page Principal");
               SendDataDisplay();
            }
            }
         }
      }
    }//Menu de Contrase�a de acceso.
//_--------------------------------------------------------------------------------------------------------------------------------------------------------       

//_--------------------------------------------------------------------------------------------------------------------------------------------------------      
    if(Menu==1){//Menu Principal
        //ETH0_StringIP(Str);       
        printf("fecha.txt=\"Fecha:%02u/%02u/20%02u Hora: %02u:%02u:%02u\"",Dia,Mes,Year,Hora,Minuto,Segundo);
        SendDataDisplay();
        
        if(Rx_Buffer[4]==0x01){
            printf("page Red");
            SendDataDisplay();
            flagRed=0;
            ip1=0;ip2=0;ip3=0;ip4=0;g1=0;g2=0;g3=0;g4=0;s1=0;s2=0;s3=0;s4=0;m1=0;m2=0;m3=0;m4=0;m5=0;m6=0;
            Rx_Buffer[4]=0x00;
            Rx_Buffer2[4]=0x00;   
            delay_ms(200);
        } 
    
        if(Rx_Buffer[4]==0x05){
            printf("page Fecha");
            SendDataDisplay();
            Rx_Buffer[4]=0x00;
            Rx_Buffer2[4]=0x00;   
            DiaTx=Dia;
            MesTx=Mes;
            YearTx=Year;
            HoraTx=Hora;
            MinutoTx=Minuto;
        } 
        
        if(Rx_Buffer[4]==0x06){
            printf("page Clave");
            SendDataDisplay();
            Rx_Buffer[4]=0x00;
            Rx_Buffer2[4]=0x00; 
            TipoClave=2;
        } 
        
        if(Rx_Buffer[4]==0x04){
            printf("page Test");
            SendDataDisplay();
            Rx_Buffer[4]=0x00;
            Rx_Buffer2[4]=0x00;   
            TipoClave=4;
        } 
        
        if(Rx_Buffer[4]==0xa0){
            printf("page Clave");
            SendDataDisplay();
            Rx_Buffer[4]=0x00;
            Rx_Buffer2[4]=0x00;   
            TipoClave=1;
            Opcion2=1;
            config=ON;
        } 
        
        if(Rx_Buffer[4]==0x07){
            printf("page Grafica");
            SendDataDisplay();
            Rx_Buffer[4]=0x00;
            Rx_Buffer2[4]=0x00;   
            entroGrafica=ON;            
        } 
        /*
        if(Rx_Buffer[4]==0x08){
            printf("page Calib");
            SendDataDisplay();
            Rx_Buffer[4]=0x00;
            Rx_Buffer2[4]=0x00;   
            Opcion=1;
        } 
        
        if(Rx_Buffer[4]==0x09){
            printf("page CalibHumedad");
            SendDataDisplay();
            Rx_Buffer[4]=0x00;
            Rx_Buffer2[4]=0x00;   
            Opcion=1;
        } 
        */
        
        if(Rx_Buffer[4]==0x09){
            printf("page Alarmas");
            SendDataDisplay();
            Rx_Buffer[4]=0x00;
            Rx_Buffer2[4]=0x00;   
            Opcion=1;
        } 
        
        if(Rx_Buffer[4]==0x11){
            printf("page Principal");
            SendDataDisplay();
            Rx_Buffer[4]=0x00;
            Rx_Buffer2[4]=0x00;   
            Opcion=1;
        } 
    }//Menu Principal
//_--------------------------------------------------------------------------------------------------------------------------------------------------------          

//_--------------------------------------------------------------------------------------------------------------------------------------------------------      
    if(Menu==15){//Menu de Clave Correcta
      if(!entro){
         delay_ms(800);
         entro=ON;
      }
      printf("page Principal");
      SendDataDisplay();
   }//Menu de Clave Correcta
//_--------------------------------------------------------------------------------------------------------------------------------------------------------      

//_--------------------------------------------------------------------------------------------------------------------------------------------------------      
    if(Menu==16){//Menu de Clave InCorrecta
      if(!entro){
         delay_ms(800);         
         entro=ON;
      }
      printf("page Clave");
      SendDataDisplay();
   }//Menu de Clave InCorrecta
//_--------------------------------------------------------------------------------------------------------------------------------------------------------      

//_--------------------------------------------------------------------------------------------------------------------------------------------------------
    if(Menu==20){ //Menu de Funcionamiento
      GuardaEEPROM=OFF;
      dibujo=OFF;
      conteoPregunta=OFF;
      tPregunta=0;
      if(flagRampa){
        //sprintf(displayStr,"titulo.txt=\"E1:%2.0f PWM1:%04u\"",errorL1,tmp1);
        if(!Finalizo){
            printf("titulo.txt=\"Programa %02Lu, Paso %02u\"",Programa,Segmento+1);                 
        }else{
            printf("titulo.txt=\"Programa %02Lu Finalizado\"",Programa);                 
        }
      }else{
        printf("titulo.txt=\"Camara Climatica JP Inglobal\"");
      }
      SendDataDisplay();
      printf("fecha.txt=\"Fecha:%02u/%02u/20%02u    Hora: %02u:%02u:%02u\"",Dia,Mes,Year,Hora,Minuto,Segundo);
      SendDataDisplay();
      x1=ON;x10=OFF;x100=OFF;
      
      if(flagTemperatura && DOOR){
         if(Temperatura>(Setpoint+alarmaTemperatura) || Temperatura<(Setpoint-alarmaTemperatura)){
            alarmaT=ON;
         }else{
            alarmaT=OFF;
            tCalentamiento=0;
         }
      }else{
         if(tCalentamiento>5400){
            alarmaT=ON;
         }else{
            alarmaT=OFF;
         }
      }
      
      if(flagHumedad && DOOR){         
         if(humedadDisplay>(SetpointH+alarmaHumedad) || humedadDisplay<(SetpointH-alarmaHumedad)){
            alarmaH=ON;
         }else{
            alarmaH=OFF;
            tHumidificacion=0;
         }
      }else{
         if(Humidificar){
            if(tHumidificacion>3600){
               alarmaH=ON;
            }else{
               alarmaH=OFF;
            }
         }else{
            if(tHumidificacion>18000){
               alarmaH=ON;
            }else{
               alarmaH=OFF;
            }
         }
      }
      
      if(!alarmaH && !alarmaT)
         Buzzer_off;
      
      if(Rx_Buffer[4]==0x01){//Temperatura
         printf("page Temperatura");
         SendDataDisplay();
      }
      
      if(Rx_Buffer[4]==0x02){//Humedad
         printf("page Humedad");
         SendDataDisplay();
      }
      
      if(Rx_Buffer[4]==0x03){//Tiempo
         printf("page Tiempo");
         SendDataDisplay();
      }
    
      if(Rx_Buffer[4]==0x04){//Menu
         printf("page Ajustes");
         SendDataDisplay();
      }
    
      if(Rx_Buffer[4]==0x05){//Programas
         printf("page Rampas");
         SendDataDisplay();
      }
    
      if(Rx_Buffer[4]==0x06){//Luz interna
         if(!estadoLuz){
            estadoLuz=ON;
            printf("bluz.pic=119");
            SendDataDisplay();
         }else{
            estadoLuz=OFF;
            printf("bluz.pic=120");
            SendDataDisplay();
         }
         Rx_Buffer[4]=0x00;
         Rx_Buffer2[4]=0x00;
      }
      
      if(!estadoLuz){            
         printf("bluz.pic=120");
         SendDataDisplay();
         printf("bluz.pic2=120");
         SendDataDisplay();
      }else{            
         printf("bluz.pic=119");
         SendDataDisplay();
         printf("bluz.pic2=119");
         SendDataDisplay();
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
      
      if(alarma==10){
         flagControlH=0;
      }
      
      if(Finalizo==0){
            if(DOOR){
               printf("status.txt=\"Puerta cerrada\"");
               SendDataDisplay();
                if(Rx_Buffer[4]==0x08){//Inicia Control Temperatura
                    flagControlT=!flagControlT;
                    if(flagControlT)
                        inicioControlT=ON;
                    else
                        inicioControlT=OFF;
                    Rx_Buffer[4]=0x00;
                    Rx_Buffer2[4]=0x00;
                    if(Setpoint<Temperatura){
                        Enfria=1;
                        Ganancia=GananciaR*(Setpoint/200);
                    //Ganancia=4;
                    }else{
                        Enfria=0;
                        Ganancia=GananciaR*(Setpoint/200);
                    }
                    if((mEstp==0)&&(hEstp==0))
                        Infinito=1;
                    else
                        Infinito=0;
                    tCalentamiento=0;
                    EEPROM_WriteByte((int8)flagControlT,41);delay_ms(20);
                }
              
                if(Rx_Buffer[4]==0x09){//Inicia Control Humedad
                    flagControlH=!flagControlH;
                    T1=(int8)(((float)T1Humedad)*((float)SetpointH/80.0));
                    T3=(int8)(((float)T3Humedad)*((float)SetpointH/80.0));
                    GananciaH=GananciaRH*(SetpointH/80);
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
                    tHumidificacion=0;
                    EEPROM_WriteByte((int8)flagControlH,42);delay_ms(20);
                }
                
                if(flagRampa){
                    DC6_on;//Ventiladores Peltier
                    inicioControlH=ON;                    
                    inicioControlT=ON;
                    iniciaConteo(); 
                    printf("b4.pic=52");
                    SendDataDisplay();
                    printf("b5.pic=52");
                    SendDataDisplay();
                    printf("b6.pic=52");
                    SendDataDisplay();
                    printf("thumedad.txt=\"Humedad\"");
                    SendDataDisplay();                    
                    controlaHumedad2();
                    
                    if(Temperatura>Tmax){
                        printf("statusT.txt=\"!Termostato!\"");      
                        SendDataDisplay();
                        Control=0;
                        PWM_WriteCompare1(0);
                        PWM_WriteCompare2(0);
                        Ganancia=GananciaR*(Setpoint/200);
                        BUZZER_Write(1);
                        delay_ms(300);
                        BUZZER_Write(0);
                        delay_ms(300);
                    }else{
                        controlaRampa();
                    }
                }else{
                    //escribeDisplay1("t8.txt=\"Info\"");
                    iniciaConteo();
                    
                    if(flagControlH){ 
                        VelMotor=Vel1;
                        printf("thumedad.txt=\"Humedad\"");
                        SendDataDisplay();
                        controlaHumedad2();
                        printf("b5.pic=52");
                        SendDataDisplay();
                        DC6_on;//Ventiladores Peltier
                    }else{
                        flagHumedad=0;
                        printf("statusH.txt=\"Detenido\"");
                        SendDataDisplay();
                        GananciaH=GananciaRH*(SetpointH/80);
                        if(alarma==10){
                           printf("thumedad.txt=\"!Sin Agua!\"");
                           SendDataDisplay();
                           ActivaAlarma(3);
                        }else{
                           printf("thumedad.txt=\"Humedad\"");
                           SendDataDisplay();
                        }
                        printf("b5.pic=51");
                        SendDataDisplay();
                        tllenado=0;
                        DC1_off;//Deshumidificador
                        DC2_off;//Humidificador
                        DC3_off;//Ventilador Humidificador
                        controlaH=0;
                        TpulsoH=0.0;
                        tHumidificacion=0;
                        humidificador=OFF;
                        thumidificador=0;
                        dehumidificador=OFF;
                        tdehumidificador=0;
                    }
            
                    if(flagControlT){
                        printf("b4.pic=52");
                        SendDataDisplay();
                        controlaTemperatura();
                        DC6_on;//Ventiladores Peltier
                    }else{
                        if(!flagControlH){
                            DC6_off;//Ventiladores Peltier
                            flagMotor=OFF;
                        }
                        flagTemperatura=0;    
                        printf("statusT.txt=\"Detenido\"");
                        SendDataDisplay();
                        printf("b4.pic=51");
                        SendDataDisplay();
                        SSR1_off;
                        Control=0;
                        PWM_WriteCompare1(0);
                        PWM_WriteCompare2(0);
                        Tpulso=0.0;
                    }
                }
            }else if(!DOOR){    
               controlaH=0;
               TpulsoH=0.0;
               DC3_off;//Ventilador Humidificador
               DC1_off;//Deshumidificador
               DC2_off;//Humidificador
               DC6_off;//Ventiladores Peltier               
               flagMotor=OFF;
               //pwm1=1023;pwm2=1023;pwm3=1023;
               flagTemperatura=0;
               flagHumedad=0;
               tCalentamiento=0;
               tHumidificacion=0;
               humidificador=OFF;
               thumidificador=0;
               dehumidificador=OFF;
               tdehumidificador=0;
               printf("b4.pic=51");
               SendDataDisplay();
               printf("b5.pic=51");
               SendDataDisplay();
               printf("b6.pic=51");
               SendDataDisplay();
               Control=0;
               PWM_WriteCompare1(0);
               PWM_WriteCompare2(0);
               Control=0;
               Tpulso=0.0;               
               flag_tiempocalentamiento=0;
               tiempocalentamiento=0;
               GananciaH=GananciaRH*(SetpointH/80);
               Ganancia=GananciaR*(Setpoint/200);
               tiempo_esterilizacion=0;
               Tpulso=0.0;
               flag_clear2=1;
               flag_ganancia=0;
               printf("statusH.txt=\"-----\"");
               SendDataDisplay();
               printf("statusT.txt=\"-----\"");
               SendDataDisplay();
               printf("status.txt=\"Puerta abierta\"");
               SendDataDisplay();
            }
      }else{
         controlaH=0;
         TpulsoH=0.0;
         DC3_off;//Ventilador Humidificador
         DC1_off;//Deshumidificador
         DC2_off;//Humidificador
         DC6_off;//Ventiladores Peltier
         flagMotor=OFF;         
         PWM_WriteCompare1(0);
         PWM_WriteCompare2(0);
         Control=0;
         Tpulso=0.0;         
         tiempo_esterilizacion=0;
         flagRampa=OFF;
         flagControlT=OFF;
         flagControlH=OFF;
         ApagaSalidas();
         printf("b4.pic=51");
         SendDataDisplay();
         printf("b5.pic=51");
         SendDataDisplay();
         printf("b6.pic=51");
         SendDataDisplay();
         printf("statusT.txt=\"Finalizado\"");
         SendDataDisplay();
         printf("statusH.txt=\"Finalizado\"");
         SendDataDisplay();
      }
      
      if(Lectura==1)// Utilizado para visualizar la lectura de la temperatura
      {                   
         Lectura=0;
         Potencia=(100.0/Tciclo)*Tpulso; 
         PotenciaH=(100.0/TcicloH)*TpulsoH; 
 //----------------Muestra Temperatura--------------------------
         if(sensores(1)>0){
             printf("temperatura.txt=\"T1:%1.1f�C\"", Temperatura); 
            desconectoTemperatura=OFF;
         }else{
             printf("temperatura.txt=\"T1:-----\""); 
             desconectoTemperatura=ON;
         }
         SendDataDisplay();
        
         if(sensores(2)>0){
             printf("temperatura2.txt=\"T2:%1.1f�C\"", Temperatura2); 
         }else{
             printf("temperatura2.txt=\"T2:-----\""); 
         }
         SendDataDisplay();
        
         //printf("statusH.txt=\"%2.1f�C %2.1f%% %2.1f\"",Tam,RH2,sensores(3));
         //SendDataDisplay();
         printf("stemperatura.txt=\"SET:%1.1f�C\"",Setpoint);
         SendDataDisplay();
         printf("shumedad.txt=\"SET:%03u%% \"",SetpointH);
         SendDataDisplay();
         if(Humedad>0.0 && Humedad<=110){
            printf("humedad.txt=\"HR:%03.1f%%\"",medianaH);
            //printf("humedad.txt=\"HR:%03.1f%%\"",Humedad);
            desconectoHumedad=OFF;
         }else if(Humedad>110 || Humedad<=0.0){
            if(Humedad>=100 && Humedad<110)
                printf("humedad.txt=\"HR:100%%\"");    
            else    
                printf("humedad.txt=\"HR:----\"");
            desconectoHumedad=ON;
         }
         SendDataDisplay();
         
         printf("stiempo.txt=\"SET:%02u:%02u (HH:MM)\"",hEstp,mEstp);
         SendDataDisplay();
         printf("tiempo.txt=\"%02u:%02u \"",hEst,mEst);
         SendDataDisplay();
         printf("potT.txt=\"P.T:%2.0f %% P.H:%2.0f %%\"",Potencia,PotenciaH);
         SendDataDisplay();
      } 
      
      // Si cumplio el tiempo de esterilizaci�n, Muestre ciclo finalizado y detenga control de temperatura.
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
                  delay_ms(600);
                  //CyWdtClear();
                  BUZZER_Write(0);
                  delay_ms(600);
                  //CyWdtClear();
               }
            }
         }else{
            if((mEst>=mEstp)&&(hEst>=hEstp)&& tiempo_esterilizacion==1){
               if(flag_clear==0){
                  tiempo_esterilizacion=0;
                  Pausa=2;
                  flag_clear=1;
                  SSR1_off;
                  Control=0;
                  Tpulso=0.0;
                  flag_ganancia=0;
                  Finalizo=1;
               }
               printf("status.txt=\"Finalizado\"");
               SendDataDisplay();
               ApagaSalidas();
               for(i=0;i<2;i++){
                  BUZZER_Write(1);
                  delay_ms(600);
                  //CyWdtClear();
                  BUZZER_Write(0);
                  delay_ms(600);
                  //CyWdtClear();
               }
            }
         }
      }
    }//Menu de Funcionamiento
//_--------------------------------------------------------------------------------------------------------------------------------------------------------       

//_--------------------------------------------------------------------------------------------------------------------------------------------------------
     if(Menu==100){ //Menu de Configuraci�n de Parametros de Control
      config=OFF;
      if(Opcion2>19)
         Opcion2=1;
      if(Opcion2<1)
         Opcion2=19;
      
      if(Opcion2==1){
         printf("Config.t2c.txt=\"Proporcional\"");      
         SendDataDisplay();
         printf("t3c.txt=\"%u\"",(int8)GananciaR);
         SendDataDisplay();
         
         if(Rx_Buffer[4]==0x0a){//Tecla Arriba Oprimida
            if(Rx_Buffer[4]==0x0a){
               GananciaR+=1.0;
               Rx_Buffer[4]=0x00;  
               Rx_Buffer2[4]=0x00;            
            }
         }
            
         if(Rx_Buffer[4]==0x0b){//Tecla Abajo Oprimida
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
         printf("Config.t2c.txt=\"Derivativo\"");
         SendDataDisplay();
         printf("t3c.txt=\"%u\"",Derivativo);
         SendDataDisplay();
        
         if(Rx_Buffer[4]==0x0a){//Tecla Arriba Oprimida            
            if(Rx_Buffer[4]==0x0a){
               Derivativo++;
               Rx_Buffer[4]=0x00;  
               Rx_Buffer2[4]=0x00;                           
            }
         }
            
         if(Rx_Buffer[4]==0x0b){//Tecla Abajo Oprimida
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
         printf("Config.t2c.txt=\"Integral\"");
         SendDataDisplay();
         printf("t3c.txt=\"%u\"",Integral);
         SendDataDisplay();
         if(Rx_Buffer[4]==0x0a){//Tecla Arriba Oprimida
            if(Rx_Buffer[4]==0x0a){
               Integral++;
               Rx_Buffer[4]=0x00;  
               Rx_Buffer2[4]=0x00;            
            }
         }
            
         if(Rx_Buffer[4]==0x0b){//Tecla Abajo Oprimida            
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
         printf("Config.t2c.txt=\"Tciclo\"");
         SendDataDisplay();
         printf("t3c.txt=\"%u\"",Tciclo);
         SendDataDisplay();
         if(Rx_Buffer[4]==0x0a){//Tecla Arriba Oprimida            
            if(Rx_Buffer[4]==0x0a){
               Tciclo+=1.0;
               Rx_Buffer[4]=0x00;  
               Rx_Buffer2[4]=0x00;            
            }
         }
            
         if(Rx_Buffer[4]==0x0b){//Tecla Abajo Oprimida
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
         printf("Config.t2c.txt=\"Ajuste +\"");  
         SendDataDisplay();
         printf("t3c.txt=\"%u\"",Ajuste);
         SendDataDisplay();
         if(Rx_Buffer[4]==0x0a){//Tecla Arriba Oprimida            
            if(Rx_Buffer[4]==0x0a){
               if(Ajuste<100)
                  Ajuste++;
               Rx_Buffer[4]=0x00;  
               Rx_Buffer2[4]=0x00;            
            }
         }
            
         if(Rx_Buffer[4]==0x0b){//Tecla Abajo Oprimida            
            if(Rx_Buffer[4]==0x0b){
               if(Ajuste>0)
                  Ajuste--;
               Rx_Buffer[4]=0x00;
               Rx_Buffer2[4]=0x00;            
            }
         }
      }else if(Opcion2==6){
         printf("Config.t2c.txt=\"Ajuste -\"");
         SendDataDisplay();
         printf("t3c.txt=\"%u\"",Ajusten);
         SendDataDisplay();
         if(Rx_Buffer[4]==0x0a){//Tecla Arriba Oprimida            
            if(Rx_Buffer[4]==0x0a){
               if(Ajusten<100)
                  Ajusten++;
               Rx_Buffer[4]=0x00;  
               Rx_Buffer2[4]=0x00;            
            }         
         }
            
         if(Rx_Buffer[4]==0x0b){//Tecla Abajo Oprimida            
            if(Rx_Buffer[4]==0x0b){
               if(Ajusten>0)
                  Ajusten--;
               Rx_Buffer[4]=0x00;
               Rx_Buffer2[4]=0x00;            
            }
         }
      }else if(Opcion2==7){
         printf("Config.t2c.txt=\"Media\"");    
         SendDataDisplay();
         printf("t3c.txt=\"%u\"",MediaMovil);
         SendDataDisplay();
         if(Rx_Buffer[4]==0x0a){//Tecla Arriba Oprimida            
            if(Rx_Buffer[4]==0x0a){
               if(MediaMovil<80)
                  MediaMovil++;
               Rx_Buffer[4]=0x00;  
               Rx_Buffer2[4]=0x00;            
            }
         }
            
         if(Rx_Buffer[4]==0x0b){//Tecla Abajo Oprimida
            if(Rx_Buffer[4]==0x0b){
               if(MediaMovil>10)
                  MediaMovil--;
               Rx_Buffer[4]=0x00;
               Rx_Buffer2[4]=0x00;            
            }            
         }
      }else if(Opcion2==8){
         printf("Config.t2c.txt=\"Media Hum\"");    
         SendDataDisplay();
         printf("t3c.txt=\"%u\"",MediaMovil2);
         SendDataDisplay();
         if(Rx_Buffer[4]==0x0a){//Tecla Arriba Oprimida            
            if(Rx_Buffer[4]==0x0a){
               if(MediaMovil2<80)
                  MediaMovil2++;
               Rx_Buffer[4]=0x00;  
               Rx_Buffer2[4]=0x00;            
            }
         }
            
         if(Rx_Buffer[4]==0x0b){//Tecla Abajo Oprimida            
            if(Rx_Buffer[4]==0x0b){
               if(MediaMovil2>10)
                  MediaMovil2--;
               Rx_Buffer[4]=0x00;
               Rx_Buffer2[4]=0x00;            
            }            
         }
      }else if(Opcion2==9){
         printf("Config.t2c.txt=\"Ajuste2 +\"");  
         SendDataDisplay();
         printf("t3c.txt=\"%u\"",Ajuste2);
         SendDataDisplay();
         if(Rx_Buffer[4]==0x0a){//Tecla Arriba Oprimida            
            if(Rx_Buffer[4]==0x0a){
               if(Ajuste2<100)
                  Ajuste2++;
               Rx_Buffer[4]=0x00;  
               Rx_Buffer2[4]=0x00;            
            }
         }
            
         if(Rx_Buffer[4]==0x0b){//Tecla Abajo Oprimida            
            if(Rx_Buffer[4]==0x0b){
               if(Ajuste2>0)
                  Ajuste2--;
               Rx_Buffer[4]=0x00;
               Rx_Buffer2[4]=0x00;            
            }
         }
      }else if(Opcion2==10){
         printf("Config.t2c.txt=\"Ajuste2 -\"");
         SendDataDisplay();
         printf("t3c.txt=\"%u\"",Ajusten2);
         SendDataDisplay();
         if(Rx_Buffer[4]==0x0a){//Tecla Arriba Oprimida            
            if(Rx_Buffer[4]==0x0a){
               if(Ajusten2<100)
                  Ajusten2++;
               Rx_Buffer[4]=0x00;  
               Rx_Buffer2[4]=0x00;            
            }         
         }
            
         if(Rx_Buffer[4]==0x0b){//Tecla Abajo Oprimida            
            if(Rx_Buffer[4]==0x0b){
               if(Ajusten2>0)
                  Ajusten2--;
               Rx_Buffer[4]=0x00;
               Rx_Buffer2[4]=0x00;            
            }
         }
      }else if(Opcion2==11){
         printf("Config.t2c.txt=\"Muestreo\"");
         SendDataDisplay();
         printf("t3c.txt=\"%03u\"",tmuestreoUSB);
         SendDataDisplay();
         if(Rx_Buffer[4]==0x0a){//Tecla Arriba Oprimida            
            if(Rx_Buffer[4]==0x0a){               
                  tmuestreoUSB++;
               Rx_Buffer[4]=0x00;  
               Rx_Buffer2[4]=0x00;            
            }         
         }
            
         if(Rx_Buffer[4]==0x0b){//Tecla Abajo Oprimida         
            if(Rx_Buffer[4]==0x0b){
                  tmuestreoUSB--;
               Rx_Buffer[4]=0x00;
               Rx_Buffer2[4]=0x00;            
            }
         }
        if(tmuestreoUSB<1)tmuestreoUSB=255;
        if(tmuestreoUSB>254)tmuestreoUSB=1;
      }else if(Opcion2==12){
         printf("Config.t2c.txt=\"T1 Humedad\"");
         SendDataDisplay();
         printf("t3c.txt=\"%02u\"",T1Humedad);
         SendDataDisplay();
         if(T1Humedad>60 || T1Humedad<1)
            T1Humedad=1;
         if(Rx_Buffer[4]==0x0a){//Tecla Arriba Oprimida
            if(Rx_Buffer[4]==0x0a){               
                  T1Humedad++;
               Rx_Buffer[4]=0x00;  
               Rx_Buffer2[4]=0x00;            
            }         
         }
            
         if(Rx_Buffer[4]==0x0b){//Tecla Abajo Oprimida            
            if(Rx_Buffer[4]==0x0b){
                  T1Humedad--;
               Rx_Buffer[4]=0x00;
               Rx_Buffer2[4]=0x00;            
            }
         }
      }else if(Opcion2==13){
         printf("Config.t2c.txt=\"T2 Humedad\"");
         SendDataDisplay();
         printf("t3c.txt=\"%02u\"",T2Humedad);
         SendDataDisplay();
         if(T2Humedad>60 || T2Humedad<4)
            T2Humedad=4;
         if(Rx_Buffer[4]==0x0a){//Tecla Arriba Oprimida            
            if(Rx_Buffer[4]==0x0a){               
                  T2Humedad++;
               Rx_Buffer[4]=0x00;  
               Rx_Buffer2[4]=0x00;            
            }         
         }
            
         if(Rx_Buffer[4]==0x0b){//Tecla Abajo Oprimida            
            if(Rx_Buffer[4]==0x0b){
                  T2Humedad--;
               Rx_Buffer[4]=0x00;
               Rx_Buffer2[4]=0x00;            
            }
         }
      } else if(Opcion2==14){
         printf("Config.t2c.txt=\"T3 Humedad\"");
         SendDataDisplay();
         printf("t3c.txt=\"%02u\"",T3Humedad);
         SendDataDisplay();
         if(T3Humedad>60 || T3Humedad<1)
            T3Humedad=1;
         if(Rx_Buffer[4]==0x0a){//Tecla Arriba Oprimida            
            if(Rx_Buffer[4]==0x0a){               
                  T3Humedad++;
               Rx_Buffer[4]=0x00;  
               Rx_Buffer2[4]=0x00;            
            }         
         }
            
         if(Rx_Buffer[4]==0x0b){//Tecla Abajo Oprimida            
            if(Rx_Buffer[4]==0x0b){
                  T3Humedad--;
               Rx_Buffer[4]=0x00;
               Rx_Buffer2[4]=0x00;            
            }
         }
      }  else if(Opcion2==15){
         printf("Config.t2c.txt=\"tCongela\"");
         SendDataDisplay();
         printf("t3c.txt=\"%03Lu\"",tCongela);
         SendDataDisplay();
         if(tCongela>200)
            tCongela=1;
         if(tCongela<1)
            tCongela=200;
         if(Rx_Buffer[4]==0x0a){//Tecla Arriba Oprimida            
            if(Rx_Buffer[4]==0x0a){               
                  tCongela++;
               Rx_Buffer[4]=0x00;  
               Rx_Buffer2[4]=0x00;            
            }         
         }
            
         if(Rx_Buffer[4]==0x0b){//Tecla Abajo Oprimida            
            if(Rx_Buffer[4]==0x0b){
                  tCongela--;
               Rx_Buffer[4]=0x00;
               Rx_Buffer2[4]=0x00;            
            }
         }
      }  else if(Opcion2==16){
         printf("Config.t2c.txt=\"tDisplay\"");
         SendDataDisplay();
         printf("t3c.txt=\"%03u\"",tDisplay);
         SendDataDisplay();
         if(tDisplay>60)
            tDisplay=1;
         if(tDisplay<1)
            tDisplay=60;
         if(Rx_Buffer[4]==0x0a){//Tecla Arriba Oprimida            
            if(Rx_Buffer[4]==0x0a){               
                  tDisplay++;
               Rx_Buffer[4]=0x00;  
               Rx_Buffer2[4]=0x00;            
            }         
         }
            
         if(Rx_Buffer[4]==0x0b){//Tecla Abajo Oprimida            
            if(Rx_Buffer[4]==0x0b){
                  tDisplay--;
               Rx_Buffer[4]=0x00;
               Rx_Buffer2[4]=0x00;            
            }
         }
      }else if(Opcion2==17){
         printf("Config.t2c.txt=\"P. Humedad\"");      
         SendDataDisplay();
         printf("t3c.txt=\"%u\"",(int8)GananciaRH);
         SendDataDisplay();
         
         if(Rx_Buffer[4]==0x0a){//Tecla Arriba Oprimida
            if(Rx_Buffer[4]==0x0a){
               GananciaRH+=1.0;
               Rx_Buffer[4]=0x00;  
               Rx_Buffer2[4]=0x00;            
            }
         }
            
         if(Rx_Buffer[4]==0x0b){//Tecla Abajo Oprimida
            if(Rx_Buffer[4]==0x0b){
               GananciaRH-=1.0;
               Rx_Buffer[4]=0x00;
               Rx_Buffer2[4]=0x00;            
            }
         }
         
         if(GananciaRH>20)
            GananciaRH=1;
         if(GananciaRH<1)
            GananciaRH=20;
      }else if(Opcion2==18){
         printf("Config.t2c.txt=\"tDefrost2\"");
         SendDataDisplay();
         printf("t3c.txt=\"%03Lu\"",tDefrost2);
         SendDataDisplay();
         if(tDefrost2>200)
            tDefrost2=1;
         if(tDefrost2<1)
            tDefrost2=200;
         if(Rx_Buffer[4]==0x0a){//Tecla Arriba Oprimida            
            if(Rx_Buffer[4]==0x0a){               
                  tDefrost2++;
               Rx_Buffer[4]=0x00;  
               Rx_Buffer2[4]=0x00;            
            }         
         }
            
         if(Rx_Buffer[4]==0x0b){//Tecla Abajo Oprimida            
            if(Rx_Buffer[4]==0x0b){
                  tDefrost2--;
               Rx_Buffer[4]=0x00;
               Rx_Buffer2[4]=0x00;            
            }
         }
      }else if(Opcion2==19){
         printf("Config.t2c.txt=\"tLuz\"");
         SendDataDisplay();
         printf("t3c.txt=\"%03Lu\"",tLuzProgramada);
         SendDataDisplay();
         if(tLuzProgramada>200)
            tLuzProgramada=1;
         if(tLuzProgramada<1)
            tLuzProgramada=200;
         if(Rx_Buffer[4]==0x0a){//Tecla Arriba Oprimida            
            if(Rx_Buffer[4]==0x0a){               
                  tLuzProgramada++;
               Rx_Buffer[4]=0x00;  
               Rx_Buffer2[4]=0x00;            
            }         
         }
            
         if(Rx_Buffer[4]==0x0b){//Tecla Abajo Oprimida            
            if(Rx_Buffer[4]==0x0b){
                  tLuzProgramada--;
               Rx_Buffer[4]=0x00;
               Rx_Buffer2[4]=0x00;            
            }
         }
      } 
      
      if(Rx_Buffer[4]==0x0c){//Tecla Izquierda Oprimida        
         if(Rx_Buffer[4]==0x0c){
            Opcion2++;
            Rx_Buffer[4]=0x00;  
            Rx_Buffer2[4]=0x00;            
         }
      }
         
      if(Rx_Buffer[4]==0x0d){//Tecla Derecha Oprimida         
         if(Rx_Buffer[4]==0x0d){
            Opcion2--;
            Rx_Buffer[4]=0x00;
            Rx_Buffer2[4]=0x00;            
         }
      }
      
      if(Rx_Buffer[4]==0x11){//Tecla Regresar
         delay_ms(20);
         if(Rx_Buffer[4]==0x11){
            if(!GuardaEEPROM){
               EEPROM_WriteByte(mediaLuces,5);delay_ms(10);
               delay_ms(10);   
               EEPROM_WriteByte((int8)Factor,6);delay_ms(10);
               delay_ms(10);   
               EEPROM_WriteByte((int8)tDefrost2,7);
               delay_ms(10);   
               EEPROM_WriteByte((int8)tCongela,8);
               delay_ms(10);   
               EEPROM_WriteByte(tDisplay,9);
               delay_ms(10);   
               EEPROM_WriteByte((int8)tLuzProgramada,14);
               delay_ms(10);   
               EEPROM_WriteByte(Integral,32);
               delay_ms(10);
               EEPROM_WriteByte(Derivativo,31);
               delay_ms(10);
               EEPROM_WriteByte((int8)GananciaR,30);
               delay_ms(10);
               EEPROM_WriteByte((int8)GananciaRH,27);
               delay_ms(10);
               EEPROM_WriteByte(Ajuste,34);
               delay_ms(10);
               EEPROM_WriteByte(Ajusten,35);
               delay_ms(10);
               EEPROM_WriteByte(MediaMovil,36);
               delay_ms(10);
               EEPROM_WriteByte(Tciclo,33);
               delay_ms(10);
               EEPROM_WriteByte(Ajuste2,37);
               delay_ms(10);
               EEPROM_WriteByte(Ajusten2,38);
               delay_ms(10);
               EEPROM_WriteByte(MediaMovil2,39);
               delay_ms(10);
               Ganancia=GananciaR*(Setpoint/200);
               EEPROM_WriteByte(tmuestreoUSB,40);
               delay_ms(10);
               EEPROM_WriteByte(T1Humedad,44);
               delay_ms(10);
               EEPROM_WriteByte(T2Humedad,45);
               delay_ms(10);
               EEPROM_WriteByte(T3Humedad,46);
               delay_ms(10);
               GuardaEEPROM=ON;
            }
            printf("page Ajustes");
            SendDataDisplay();
         }
      }
    }//Menu de Configuraci�n de Parametros de Control
//_--------------------------------------------------------------------------------------------------------------------------------------------------------           

//_-------------------------------------------------------Display 2----------------------------------------------------------------------------------------                

   }
}