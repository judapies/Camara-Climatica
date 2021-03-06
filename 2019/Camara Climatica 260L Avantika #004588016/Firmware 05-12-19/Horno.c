#include <18F4685.h>
#device adc=10
#device HIGH_INTS=TRUE //Activamos niveles de prioridad
#fuses HS,WDT8192,NOPROTECT,NOLVP,NODEBUG,NOSTVREN,NOPUT,NOCPD,NOWRT,NOIESO,NOFCMEN,NOPBADEN,NOWRTC,NOWRTB,NOEBTR,NOEBTRB,NOCPB,NOXINST,NOLPT1OSC,NOMCLR
#use delay(clock=20000000)
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7, bits=8, parity=N)
#include <DS1302.c>
#include <math.h>

// Definición de teclado - NO
#define   UP            !input(PIN_B0)
#define   DOWN          !input(PIN_B1)
#define   RIGHT         !input(PIN_B2)
#define   LEFT          !input(PIN_B3)

#define   DOOR          input(PIN_B4)
#define   TERM          input(PIN_B5)
#define   LEVEL         input(PIN_B6)


// Definición de Salidas
#define   Humidificador_on  output_bit(PIN_E2,1)
#define   Humidificador_off output_bit(PIN_E2,0)
#define   Dehumidificador_on  output_bit(PIN_E1,1)
#define   Dehumidificador_off output_bit(PIN_E1,0)
#define   Buzzer_on         output_bit(PIN_D7,1)
#define   Buzzer_off        output_bit(PIN_D7,0)
#define   SSR3_on           output_bit(PIN_C0,1)
#define   SSR3_off          output_bit(PIN_C0,0)
#define   SSR2_on           output_bit(PIN_C1,1)
#define   SSR2_off          output_bit(PIN_C1,0)
#define   SSR_on            output_bit(PIN_C2,1)
#define   SSR_off           output_bit(PIN_C2,0)
#define   Rele1_on          output_bit(PIN_C3,1)
#define   Rele1_off         output_bit(PIN_C3,0)
#define   Rele2_on          output_bit(PIN_D0,1)
#define   Rele2_off         output_bit(PIN_D0,0)
#define   Rele3_on          output_bit(PIN_D1,1)
#define   Rele3_off         output_bit(PIN_D1,0)
#define   Rele4_on          output_bit(PIN_C5,1)
#define   Rele4_off         output_bit(PIN_C5,0)
#define   Rele5_on          output_bit(PIN_C4,1)//Ventiladores Peltier
#define   Rele5_off         output_bit(PIN_C4,0)//Ventiladores Peltier
#define   Rele6_on          output_bit(PIN_D3,1)//Bomba Peristaltica
#define   Rele6_off         output_bit(PIN_D3,0)//Bomba Peristaltica
#define   Rele7_on          output_bit(PIN_D2,1)
#define   Rele7_off         output_bit(PIN_D2,0)

#define OFF 0
#define ON  1
#define RX_BUFFER_SIZE  10
char Rx_Buffer[RX_BUFFER_SIZE+1];
char Rx_Buffer2[RX_BUFFER_SIZE+1];
char RX_Wr_Index=0;


/* Tabla de Modulos, Primera Fila Ańo Regular y el siguiente ańo Bisiesto
E   F   M   A   M   J   J   A   S   O   N   D
0   3   3   6   1   4   6   2   5   0   3   5
0   3   4   0   2   5   0   3   6   1   4   6
*/
int8 Regular[12]={0,3,3,6,1,4,6,2,5,0,3,5};
int8 Bisiesto[12]={0,3,4,0,2,5,0,3,6,1,4,6};

float promedio[90]={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
float promedioH[120];
float promedioTa[120];
float X[2]={0.0,0.0};
short Desconectado=0,entro=OFF,GuardaEEPROM=OFF,Horas=OFF,Minutos=OFF,PruebaEstados=OFF,EstadoR1=OFF,EstadoR2=OFF,EstadoR3=OFF,EstadoR4=OFF,EstadoR5=OFF;
short Infinito=0,Finalizo=0,flag_clear=0,flagHumedad=OFF,flagTemperatura=OFF,EstadoR6=OFF,EstadoR7=OFF;
short flagControlT=OFF,flagControlH=OFF,rampa=OFF,flagRampa=OFF,flag_calcula=OFF,Humidificar=OFF;
int8 h=0,l=0,Ajuste=0,Ajusten=0,MediaMovil=10,tiemporecta=0,flag_ganancia=0,Lectura2=0,Enfria=0,Controlando=0,Ajusto=10;
int8 Derivativo=40,Integral=0,z=0,Dato2=0,Inicio=0,TipoClave=0,Modulo=0,Opcion2=0,f=0,Segmentos=0,Segmento=0;
float mEstp=0,hEstp=0,mEstpAnt=0,hEstpAnt=0,mEst=0,hEst=0;
short Lectura=0,flag_ajuste=0,flag_ajuste2=0,EstadoBuzzer=OFF;
short flag_clear2=0,flag_clear3=0,flag_tiempocalentamiento=0,flag_setpoint=0;
int8 y=0,tiemporeset=0,n=0;
int8 Menu=0, i=0,Pausa=0,TiempoControl=0;
int8 Opcion=1,tiempo_esterilizacion=0,decimal=0,entero=0,Dato_Exitoso=0,MenuAntt=240;
int16 tiempos=0,tiempos3=0,tiempoctrl=0,tinicio=0,tiempocalentamiento=0,tiemposetpoint=0,Tp=0,Tc=0;
float Potencia=0.0,Gananciamax=0.0,Tam=0.0,RH2=0.0;
signed int  Password[4]={0,0,0,0};        // Contrasena Almacenada de 4 digitos
float V0=0.0,Tciclo=10.0,Tpulso=0.0,error=0.0,Prom=0.0,Temperature=0.0,tmp=0.0,Ganancia=2.0,GananciaR=2.0,desvio=0.5,Coeficiente=44.5,Pt=0.0,Humedad=0.0;
float Aumento=0.0,Temperatura=0.0,Saturacion=10.0,Setpoint=0,Temporal=0.0,SetpointH=0.0;
float a=0.0011,b=2.3302,c=-244.0723;
float PendienteMinutos=0,TempeInicial=20,Tiempo=0,TiempoActual=0,PendienteActual=0,HorasActual=0,MinutosActual=0,TiempoRampa=0;
float PotenciaEntregada=0.0,Pendiente=0.0,DifPendiente=0.0;
float TemperaturaIdeal=0,penCamara=2.5,SetpointAnt=0.0,SetpointHAnt=0.0;

int8 Year=18,Mes=9,Dia=13,Hora=0,Minuto=0,Segundo=0,dow=0;
int8 YearTx=0,MesTx=0,DiaTx=0,HoraTx=0,MinutoTx=0,dowTx=0,CicloLibre=0;

typedef struct{
   int8 TemperaturaDec;
   int8 TemperaturaInt;
   int8 Humedad;
   float Horas;
   float Minutos;
   float Temperatura;
}Personalizado;

Personalizado rampas[20];

#include "Funciones.h"

#int_rda HIGH
void serial_rx_isr(){
   Dato2=fgetc();
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
      if(Rx_Buffer2[0]==0x65 && Rx_Buffer2[1]==0xff && Rx_Buffer2[2]==0xff && Rx_Buffer2[8]==0x00 && Rx_Buffer2[9]==0xff ){
         Dato_Exitoso=5;
      }else{
         Dato_Exitoso=10;
      }
   }
   //output_toggle(PIN_D7);
   //if(Menu==20)
     //Buzzer_on;
}

#int_TIMER1
void temp1s(void){
   set_timer1(40536);
   tiempos++;
   tiempos3++;
   tiempoctrl++;
   
   if(tiempos3>=200)
      Lectura2=1;
   
   if(Controlando==1){
      if(tiempoctrl<Tp){ // Si el tiempo es menor o igual al tiempo de control se enciende la resistencia de lo contrario se apaga.
         SSR_on;
         SSR2_on;
         SSR3_on;
      }else{
         SSR_off;
         SSR2_off;
         SSR3_off;
      }
      
      if(tiempoctrl>=Tc) // Si el tiempo de control es mayor al tiempo de ciclo se reinicia tiempo de control      
         tiempoctrl=0;// tiempoctrl incrementa una unidad cada 5ms, por eso se dividen los tiempos por 0.005.
   }
    
   if(tiempos>=200){  // 12000 para que incremente cada minuto, 200para que incremente cada segundo.
      tiempos=0;Lectura=1;tiemporeset++;TiempoControl++;tiemporecta++;
   
      if(flag_setpoint==1)
         tiemposetpoint++;
         
      if(tiempo_esterilizacion==1)
         tinicio++;
      
      if(flag_tiempocalentamiento==1)
         tiempocalentamiento++;
      
   }        
}

void main()
{
   enable_interrupts(int_rda);
   enable_interrupts(global);
   //delay_ms(1000);   
   ConfiguraInterrupciones();
   LeeEEPROM();
   LimitaValores();
   rtc_init();
   delay_ms(1);
   Buzzer_on;
   delay_ms(500);
   Buzzer_off;
   printf("page Bienvenida");
   SendDataDisplay();
   printf("bkcmd=0");
   SendDataDisplay();
   delay_ms(3500); 
   printf("page Clave");
   SendDataDisplay();
   for(i=0;i<80;i++)
      Temperatura=Leer_Sensor(MediaMovil);
      
   if(Setpoint<Temperatura){
      Enfria=1;
      Ganancia=GananciaR*(Setpoint/44.5);
      //Ganancia=4;
   }else{
      Enfria=0;
      Ganancia=GananciaR*(Setpoint/44.5);
      Gananciamax=(0.21)*Setpoint;
   }   
   
   if(SetpointH<Humedad){
      Humidificar=1;            
   }else{
      Humidificar=0;
   }   
   
   if((mEstp==0)&&(hEstp==0))
      Infinito=1;
   else
      Infinito=0;
   
   for(i=0;i<20;i++){
      if(rampas[i].Horas==0 && rampas[i].Minutos==0){
         Segmentos=i;  
         break;
      }else{
         Segmentos=i+1;  
      }
   }
   
   for(i=0;i<80;i++)
      Humedad=leeHumedad(MediaMovil);
   while(true)
   {
      rtc_get_date(Dia,Mes,Year,dow);
      rtc_get_time(Hora,Minuto,Segundo);
      LeeDisplay();
      restart_wdt();

      if(Lectura2==1){
         Temperatura=Leer_Sensor(MediaMovil);
         Humedad=leeHumedad(MediaMovil);
         Lectura2=0;
      }
   
//_--------------------------------------------------------------------------------------------------------------------------------------------------------
   if(Menu==240){ //Menu de Bienvenida.
      printf("bkcmd=0");
      SendDataDisplay();
      delay_ms(1500);
      printf("page Clave");
      SendDataDisplay();
      ApagaSalidas();
   }
//_--------------------------------------------------------------------------------------------------------------------------------------------------------

//_--------------------------------------------------------------------------------------------------------------------------------------------------------
    if(Menu==0){ //Menu de Contraseńa de acceso.
      //ApagaSalidas();
      entro=OFF;
      
      if(RX_Buffer[4]==0x11){//11
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
      }else if(TipoClave==1 || TipoClave==4){
         printf("titulo.txt=\"Clave Tecnico\"");
         SendDataDisplay();
         printf("doevents");
         SendDataDisplay();
      }else if(TipoClave==0){
         printf("titulo.txt=\"Ingrese Clave\"");
         SendDataDisplay();
      }
      
      if(RX_Buffer[3]==0x0f){//0f, recibe caracteres ingresados desde el Display
         if(TipoClave==0){
            if(RX_Buffer[4]==0x33&&RX_Buffer[5]==0x38&&RX_Buffer[6]==0x39&&RX_Buffer[7]==0x32){ // Si Ingresa clave para reset general del sistema.
               write_eeprom(10,0);delay_ms(20);write_eeprom(11,0);delay_ms(20);// Reestablece a contraseńa de Fabrica y reinicia Programa.
               write_eeprom(12,0);delay_ms(20);write_eeprom(13,0);delay_ms(20);
               reset_cpu();
            }
            if(RX_Buffer[4]>=0x30 && RX_Buffer[5]>=0x30 && RX_Buffer[6]>=0x30 && RX_Buffer[7]>=0x30
            && RX_Buffer[4]<=0x39 && RX_Buffer[5]<=0x39 && RX_Buffer[6]<=0x39 && RX_Buffer[7]<=0x39){
            if((RX_Buffer[4]==Password[0]+0x30)&&(RX_Buffer[5]==Password[1]+0x30)&&(RX_Buffer[6]==Password[2]+0x30)&&(RX_Buffer[7]==Password[3]+0x30)){
                  printf("page ClaveCorrecta");
                  SendDataDisplay();
                  //RX_Buffer[3]=0x00;RX_Buffer2[3]=0x00;  
               }else{
                  printf("page ClaveBad");
                  SendDataDisplay();
                  //RX_Buffer[3]=0x00;RX_Buffer2[3]=0x00;  
               } 
            }
         }else if(TipoClave==1){
            if(RX_Buffer[4]==0x34&&RX_Buffer[5]==0x34&&RX_Buffer[6]==0x34&&RX_Buffer[7]==0x34){ // Si Ingresa clave de Servicio Tecnico
               printf("page Config");
               SendDataDisplay();
            }else{
               printf("page Principal");
               SendDataDisplay();
            }
         }else if(TipoClave==2){
            if(RX_Buffer[4]>=0x30 && RX_Buffer[5]>=0x30 && RX_Buffer[6]>=0x30 && RX_Buffer[7]>=0x30
            && RX_Buffer[4]<=0x39 && RX_Buffer[5]<=0x39 && RX_Buffer[6]<=0x39 && RX_Buffer[7]<=0x39){
            if((RX_Buffer[4]==Password[0]+0x30)&&(RX_Buffer[5]==Password[1]+0x30)&&(RX_Buffer[6]==Password[2]+0x30)&&(RX_Buffer[7]==Password[3]+0x30)){
               printf("page Clave");
               SendDataDisplay();
               printf("titulo.txt=\"Clave Nueva\"");
               SendDataDisplay();
               TipoClave=3;
               RX_Buffer[3]=0x00;
               RX_Buffer2[3]=0x00;
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
               write_eeprom(10,RX_Buffer[4]-0x30);delay_ms(20);write_eeprom(11,RX_Buffer[5]-0x30);delay_ms(20);
               write_eeprom(12,RX_Buffer[6]-0x30);delay_ms(20);write_eeprom(13,RX_Buffer[7]-0x30);delay_ms(20);
               GuardaEEPROM=ON;
            }
            Password[0]=RX_Buffer[4]-0x30;Password[1]=RX_Buffer[5]-0x30;Password[2]=RX_Buffer[6]-0x30;Password[3]=RX_Buffer[7]-0x30;
            //RX_Buffer[3]=0x00;
            //RX_Buffer2[3]=0x00;
         }else if(TipoClave==4){
            if(RX_Buffer[4]==0x34&&RX_Buffer[5]==0x34&&RX_Buffer[6]==0x34&&RX_Buffer[7]==0x34){ // Si Ingresa clave de Servicio Tecnico
               printf("page Test");
               SendDataDisplay();
            }else{
               printf("page Principal");
               SendDataDisplay();
            }
            //RX_Buffer[3]=0x00;
            //RX_Buffer2[3]=0x00;
         }
      }
    }
//_--------------------------------------------------------------------------------------------------------------------------------------------------------       

//_--------------------------------------------------------------------------------------------------------------------------------------------------------
    if(Menu==2){ //Menu de Tiempo de Duraciňn
         printf("fecha.txt=\"Fecha:%02u/%02u/20%02u    Hora: %02u:%02u:%02u\"",Dia,Mes,Year,Hora,Minuto,Segundo);
         SendDataDisplay();
         
         if(!rampa){
            if(RX_Buffer[4]==0x0a){//Selecciono Minutos
               Minutos=OFF;
               Horas=ON;
            }
            
            if(RX_Buffer[4]==0x0b){//Selecciono Segundos
               Minutos=ON;
               Horas=OFF;
            }           
            
            if(RX_Buffer[4]==0x11 || LEFT){//Selecciono Regresar
               delay_ms(200);
               if(RX_Buffer[4]==0x11 || LEFT){
                  Minutos=OFF;
                  Horas=OFF;
                  printf("page Principal");
                  SendDataDisplay();  
                  if(!GuardaEEPROM){
                     write_eeprom(0,mEstp);
                     delay_ms(10);
                     write_eeprom(1,hEstp);
                     delay_ms(10);
                     //mEst=mEstp;
                     //hEst=hEstp;
                     if((mEstp==0)&&(hEstp==0))
                        Infinito=1;
                     else
                        Infinito=0;
                     GuardaEEPROM=ON;
                  }
               }
            }
            
            if(RIGHT){
               delay_ms(200);
               if(RIGHT){
                  if(!Minutos){
                     Minutos=ON;
                     Horas=OFF;
                     printf("tsecest.pco=65535");
                     SendDataDisplay();  
                     printf("tminest.pco=0");
                     SendDataDisplay();  
                  }else if(!Horas){
                     Minutos=OFF;
                     Horas=ON;
                     printf("tsecest.pco=0");
                     SendDataDisplay();  
                     printf("tminest.pco=65535");
                     SendDataDisplay();  
                  }
               }
            }
            
            if(UP){
               delay_ms(200);
               if(UP){
                  if(Minutos){
                     mEstp+=1.0;
                  }
                  if(Horas){
                     hEstp+=1.0;
                  }    
               }
            }
            
            if(DOWN){
               delay_ms(200);
               if(DOWN){
                  if(Minutos && mEstp>0){
                     mEstp-=1.0;
                  }
                  if(Horas){
                     hEstp-=1.0;
                  }    
               }
            }
            
            if(RX_Buffer[4]==0x0c || RX_Buffer[4]==0x0d){//Tecla arriba Oprimida
               if(Minutos){
                  mEstp+=1.0;
               }
               if(Horas){
                  hEstp+=1.0;
               }  
               RX_Buffer[4]=0x00;  
               RX_Buffer2[4]=0x00;
            }
            
            if(RX_Buffer[4]==0x0f || RX_Buffer[4]==0x0e){//Tecla abajo oprimida
               if(Minutos && mEstp>0){
                  mEstp-=1.0;
               }
               if(Horas){
                  hEstp-=1.0;
               }
               RX_Buffer[4]=0x00;  
               RX_Buffer2[4]=0x00;
            }
            
            if(hEstp>99)hEstp=1;
            if(mEstp>59)mEstp=0;
               
            printf("tminsec.txt=\"%02.0f\"",hEstp);
            SendDataDisplay();
            printf("tsecsec.txt=\"%02.0f\"",mEstp);
            SendDataDisplay();
         }else{
            if(RX_Buffer[4]==0x0a){//Selecciono Minutos
               Minutos=OFF;
               Horas=ON;
            }
            
            if(RX_Buffer[4]==0x0b){//Selecciono Segundos
               Minutos=ON;
               Horas=OFF;
            }           
            
            if(RX_Buffer[4]==0x11 || LEFT){//Selecciono Regresar
               delay_ms(200);
               if(RX_Buffer[4]==0x11 || LEFT){
                  Minutos=OFF;
                  Horas=OFF;
                  printf("page Rampas");
                  SendDataDisplay();  
                  if(!GuardaEEPROM){
                     write_eeprom(103+((CicloLibre-1)*5),(int8)rampas[CicloLibre-1].Horas);
                     delay_ms(10);
                     write_eeprom(104+((CicloLibre-1)*5),(int8)rampas[CicloLibre-1].Minutos);
                     delay_ms(10);
                     GuardaEEPROM=ON;
                  }
               }
            }
            
            if(RIGHT){
               delay_ms(200);
               if(RIGHT){
                  if(!Minutos){
                     Minutos=ON;
                     Horas=OFF;
                     printf("tsecest.pco=65535");
                     SendDataDisplay();  
                     printf("tminest.pco=0");
                     SendDataDisplay();  
                  }else if(!Horas){
                     Minutos=OFF;
                     Horas=ON;
                     printf("tsecest.pco=0");
                     SendDataDisplay();  
                     printf("tminest.pco=65535");
                     SendDataDisplay();  
                  }
               }
            }
            
            if(UP){
               delay_ms(200);
               if(UP){
                  if(Minutos){
                     rampas[CicloLibre-1].Minutos+=1.0;
                  }
                  if(Horas){
                     rampas[CicloLibre-1].Horas+=1.0;
                  }    
               }
            }
            
            if(DOWN){
               delay_ms(200);
               if(DOWN){
                  if(Minutos && rampas[CicloLibre-1].Minutos>0){
                     rampas[CicloLibre-1].Minutos-=1.0;
                  }
                  if(Horas && rampas[CicloLibre-1].Horas>0){
                     rampas[CicloLibre-1].Horas-=1.0;
                  }    
               }
            }
            
            if(RX_Buffer[4]==0x0c || RX_Buffer[4]==0x0d){//Tecla arriba Oprimida
               if(Minutos){
                  rampas[CicloLibre-1].Minutos+=1.0;
               }
               if(Horas){
                  rampas[CicloLibre-1].Horas+=1.0;
               }  
               RX_Buffer[4]=0x00;  
               RX_Buffer2[4]=0x00;
            }
            
            if(RX_Buffer[4]==0x0f || RX_Buffer[4]==0x0e){//Tecla abajo oprimida
               if(Minutos && rampas[CicloLibre-1].Minutos>0){
                  rampas[CicloLibre-1].Minutos-=1.0;
               }
               if(Horas  && rampas[CicloLibre-1].Horas>0){
                  rampas[CicloLibre-1].Horas-=1.0;
               }
               RX_Buffer[4]=0x00;  
               RX_Buffer2[4]=0x00;
            }
            
            if(rampas[CicloLibre-1].Horas>99)rampas[CicloLibre-1].Horas=1;
            if(rampas[CicloLibre-1].Minutos>59)rampas[CicloLibre-1].Minutos=0;
               
            printf("tminsec.txt=\"%02.0f\"",rampas[CicloLibre-1].Horas);
            SendDataDisplay();
            printf("tsecsec.txt=\"%02.0f\"",rampas[CicloLibre-1].Minutos);
            SendDataDisplay();
         }
    }
//_--------------------------------------------------------------------------------------------------------------------------------------------------------       

//_--------------------------------------------------------------------------------------------------------------------------------------------------------
    if(Menu==3){ //Menu de Temperatura
      printf("fecha.txt=\"Fecha:%02u/%02u/20%02u    Hora: %02u:%02u:%02u\"",Dia,Mes,Year,Hora,Minuto,Segundo);
      SendDataDisplay();
      
      if(!rampa){
         if(Setpoint<0.0)
            Setpoint=60.0;
         if(Setpoint>60.0)
            Setpoint=0.0;
         
         if(RX_Buffer[4]==0x11 || LEFT){//Selecciono Regresar
            Horas=OFF;
            Minutos=OFF;
            printf("page Principal");
            SendDataDisplay();  
            if(!GuardaEEPROM){
               write_eeprom(80,(int8)Setpoint);
               write_eeprom(81,extraeDecimal(Setpoint));
               delay_ms(10);
               GuardaEEPROM=ON;
            }
            if(Setpoint<Temperatura){
               Enfria=1;
               Ganancia=GananciaR*(Setpoint/44.5);
               //Ganancia=4;
            }else{
               Enfria=0;
               Ganancia=GananciaR*(Setpoint/44.5);
               Gananciamax=(0.21)*Setpoint;
            }
         }
         
         if(UP){
            delay_ms(200);
            if(UP){
               Setpoint+=0.1;
            }
         }
         
         if(DOWN){
            delay_ms(200);
            if(DOWN){
               Setpoint-=0.1;
            }
         }
         
         if(RX_Buffer[4]==0x0d){//Tecla Arriba Oprimida
            Setpoint+=0.1;
            RX_Buffer[4]=0x00;  
            RX_Buffer2[4]=0x00;            
         }
         
         if(RX_Buffer[4]==0x0c){//Tecla Abajo Oprimida
            Setpoint-=0.1;
            RX_Buffer[4]=0x00;
            RX_Buffer2[4]=0x00;                        
         }
         printf("tsettem.txt=\"%2.1f\"",Setpoint);
         SendDataDisplay();
      }else{
         if(rampas[CicloLibre-1].Temperatura<0.0)
            rampas[CicloLibre-1].Temperatura=60.0;
         if(rampas[CicloLibre-1].Temperatura>60.0)
            rampas[CicloLibre-1].Temperatura=0.0;
         
         if(RX_Buffer[4]==0x11 || LEFT){//Selecciono Regresar
            Horas=OFF;
            Minutos=OFF;
            printf("page Rampas");
            SendDataDisplay();  
            if(!GuardaEEPROM){
               write_eeprom(100+((CicloLibre-1)*5),(int8)rampas[CicloLibre-1].Temperatura);
               write_eeprom(101+((CicloLibre-1)*5),extraeDecimal(rampas[CicloLibre-1].Temperatura));
               delay_ms(10);
               GuardaEEPROM=ON;
            }
         }
         
         if(UP){
            delay_ms(200);
            if(UP){
               rampas[CicloLibre-1].Temperatura+=0.1;
            }
         }
         
         if(DOWN){
            delay_ms(200);
            if(DOWN){
               rampas[CicloLibre-1].Temperatura-=0.1;
            }
         }
         
         if(RX_Buffer[4]==0x0d){//Tecla Arriba Oprimida
            rampas[CicloLibre-1].Temperatura+=0.1;
            RX_Buffer[4]=0x00;  
            RX_Buffer2[4]=0x00;            
         }
         
         if(RX_Buffer[4]==0x0c){//Tecla Abajo Oprimida
            rampas[CicloLibre-1].Temperatura-=0.1;
            RX_Buffer[4]=0x00;
            RX_Buffer2[4]=0x00;                        
         }
         printf("tsettem.txt=\"%2.1f\"",rampas[CicloLibre-1].Temperatura);
         SendDataDisplay();
      }
    }
//_--------------------------------------------------------------------------------------------------------------------------------------------------------       

//_--------------------------------------------------------------------------------------------------------------------------------------------------------
    if(Menu==4){ //Menu de Humedad
      printf("fecha.txt=\"Fecha:%02u/%02u/20%02u    Hora: %02u:%02u:%02u\"",Dia,Mes,Year,Hora,Minuto,Segundo);
      SendDataDisplay();
      
      if(!rampa){
         if(SetpointH<10.0)
            SetpointH=90.0;
         if(SetpointH>90.0)
            SetpointH=10.0;
         
         if(RX_Buffer[4]==0x11 || LEFT){//Selecciono Regresar
            Horas=OFF;
            Minutos=OFF;
            printf("page Principal");
            SendDataDisplay();  
            if(!GuardaEEPROM){
               write_eeprom(82,(int8)SetpointH);
               write_eeprom(83,extraeDecimal(SetpointH));
               delay_ms(10);
               GuardaEEPROM=ON;
            }
         }
         
         if(UP){
            delay_ms(200);
            if(UP){
               SetpointH+=1.0;
            }
         }
         
         if(DOWN){
            delay_ms(200);
            if(DOWN){
               SetpointH-=1.0;
            }
         }
         
         if(RX_Buffer[4]==0x0d){//Tecla Arriba Oprimida
            SetpointH+=1.0;
            RX_Buffer[4]=0x00;  
            RX_Buffer2[4]=0x00;            
         }
         
         if(RX_Buffer[4]==0x0c){//Tecla Abajo Oprimida
            SetpointH-=1.0;
            RX_Buffer[4]=0x00;
            RX_Buffer2[4]=0x00;                        
         }
         printf("tsettem.txt=\"%2.0f\"",SetpointH);
         SendDataDisplay();
      }else{
         if(rampas[CicloLibre-1].Humedad<10.0)
            rampas[CicloLibre-1].Humedad=90.0;
         if(rampas[CicloLibre-1].Humedad>90.0)
            rampas[CicloLibre-1].Humedad=10.0;
         
         if(RX_Buffer[4]==0x11 || LEFT){//Selecciono Regresar
            Horas=OFF;
            Minutos=OFF;
            printf("page Rampas");
            SendDataDisplay();  
            if(!GuardaEEPROM){
               write_eeprom(102+((CicloLibre-1)*5),rampas[CicloLibre-1].Humedad);
               delay_ms(10);
               GuardaEEPROM=ON;
            }
         }
         
         if(UP){
            delay_ms(200);
            if(UP){
               rampas[CicloLibre-1].Humedad+=1.0;
            }
         }
         
         if(DOWN){
            delay_ms(200);
            if(DOWN){
               rampas[CicloLibre-1].Humedad-=1.0;
            }
         }
         
         if(RX_Buffer[4]==0x0d){//Tecla Arriba Oprimida
            rampas[CicloLibre-1].Humedad+=1.0;
            RX_Buffer[4]=0x00;  
            RX_Buffer2[4]=0x00;            
         }
         
         if(RX_Buffer[4]==0x0c){//Tecla Abajo Oprimida
            rampas[CicloLibre-1].Humedad-=1.0;
            RX_Buffer[4]=0x00;
            RX_Buffer2[4]=0x00;                        
         }
         printf("tsettem.txt=\"%02u\"",rampas[CicloLibre-1].Humedad);
         SendDataDisplay();
      }
    }
//_--------------------------------------------------------------------------------------------------------------------------------------------------------       

//_--------------------------------------------------------------------------------------------------------------------------------------------------------      
    if(Menu==6){ // Menu de Prueba de Componentes
         PruebaEstados=ON;
         SSR_off;
         SSR2_off;
         SSR3_off;
         Controlando=0;
         if(RX_Buffer[4]==0x01){//Oprimio EV Suministro
            EstadoR1=!EstadoR1;
            RX_Buffer[4]=0x00;
            RX_Buffer2[4]=0x00;
         }
         
         if(EstadoR1){
            printf("b0.bco=2016");
            SendDataDisplay(); 
            Rele1_on;
         }else{
            printf("b0.bco=63488");
            SendDataDisplay(); 
            Rele1_off;
         }
         
         if(RX_Buffer[4]==0x02){//Oprimio EV Suministro
            EstadoR2=!EstadoR2;
            RX_Buffer[4]=0x00;
            RX_Buffer2[4]=0x00;
         }
         
         if(EstadoR2){
            printf("b1.bco=2016");
            SendDataDisplay(); 
            Rele2_on;
         }else{
            printf("b1.bco=63488");
            SendDataDisplay(); 
            Rele2_off;
         }
         
         if(RX_Buffer[4]==0x03){//Oprimio EV Suministro
            EstadoR3=!EstadoR3;
            RX_Buffer[4]=0x00;
            RX_Buffer2[4]=0x00;
         }
         
         if(EstadoR3){
            printf("b2.bco=2016");
            SendDataDisplay(); 
            Rele3_on;
         }else{
            printf("b2.bco=63488");
            SendDataDisplay(); 
            Rele3_off;
         }
         
         if(RX_Buffer[4]==0x04){//Oprimio EV Suministro
            EstadoR4=!EstadoR4;
            RX_Buffer[4]=0x00;
            RX_Buffer2[4]=0x00;
         }
         
         if(EstadoR4){
            printf("b3.bco=2016");
            SendDataDisplay(); 
            Rele4_on;
         }else{
            printf("b3.bco=63488");
            SendDataDisplay(); 
            Rele4_off;
         }
         
         if(RX_Buffer[4]==0x05){//Oprimio EV Suministro
            EstadoR5=!EstadoR5;
            RX_Buffer[4]=0x00;
            RX_Buffer2[4]=0x00;
         }
         
         if(EstadoR5){
            printf("b4.bco=2016");
            SendDataDisplay(); 
            Rele5_on;
         }else{
            printf("b4.bco=63488");
            SendDataDisplay(); 
            Rele5_off;
         }
         
         if(RX_Buffer[4]==0x06){//Oprimio EV Suministro
            EstadoR6=!EstadoR6;
            RX_Buffer[4]=0x00;
            RX_Buffer2[4]=0x00;
         }
         
         if(EstadoR6){
            printf("b5.bco=2016");
            SendDataDisplay(); 
            Rele6_on;
         }else{
            printf("b5.bco=63488");
            SendDataDisplay(); 
            Rele6_off;
         }
         
         if(RX_Buffer[4]==0x07){//Oprimio EV Suministro
            EstadoR7=!EstadoR7;
            RX_Buffer[4]=0x00;
            RX_Buffer2[4]=0x00;
         }
         
         if(EstadoR7){
            printf("b6.bco=2016");
            SendDataDisplay(); 
            Rele7_on;
         }else{
            printf("b6.bco=63488");
            SendDataDisplay(); 
            Rele7_off;
         }
         
         if(DOOR){
            printf("b8.bco=2016");
            SendDataDisplay(); 
         }else{
            printf("b8.bco=63488");
            SendDataDisplay(); 
         }
         
         if(TERM){
            printf("b9.bco=2016");
            SendDataDisplay(); 
         }else{
            printf("b9.bco=63488");
            SendDataDisplay(); 
         }
         
         if(LEVEL){
            printf("b10.bco=2016");
            SendDataDisplay(); 
         }else{
            printf("b10.bco=63488");
            SendDataDisplay(); 
         }
         /*
         if(PRES){
            printf("b11.bco=2016");
            SendDataDisplay(); 
         }else{
            printf("b11.bco=63488");
            SendDataDisplay(); 
         }
         */
         if(RX_Buffer[4]==0x13)//Oprimio Buzzer
         {
            EstadoBuzzer=!EstadoBuzzer;
            RX_Buffer[4]=0x00;
            RX_Buffer2[4]=0x00;
         }
         
         if(EstadoBuzzer){
            printf("b12.bco=2016");
            SendDataDisplay(); 
            Buzzer_on;
         }
         else{
            printf("b12.bco=63488");
            SendDataDisplay(); 
            Buzzer_off;
         }
                  
         printf("b13.txt=\"ADC0:%3.0f\"",sensores(0));         
         SendDataDisplay();
         printf("b14.txt=\"ADC1:%3.0f\"",sensores(1));         
         SendDataDisplay();
         printf("b15.txt=\"A2:%3.0f %2.1f\"",sensores(2),Humedad);
         SendDataDisplay();
         printf("b18.txt=\"A3:%3.0f %2.1f\"",sensores(3),RH2);
         SendDataDisplay();
         printf("b19.txt=\"T2:%3.1f\"",Tam);
         SendDataDisplay();
         printf("b20.txt=\"T1:%3.1f\"",Temperatura);
         SendDataDisplay();
         
         if(RX_Buffer[4]==0x21)//Oprimio salir
         {
            printf("page Principal");
            SendDataDisplay();
            PruebaEstados=OFF;
            ApagaSalidas();
         }
     }
//_--------------------------------------------------------------------------------------------------------------------------------------------------------    

//_--------------------------------------------------------------------------------------------------------------------------------------------------------      
      if(Menu==7){ //Menu de Configuración de Fecha y Hora
         if(RX_Buffer[4]==0x0a){//Selecciono Hora
            Opcion=4;
         }
         
         if(RX_Buffer[4]==0x0b){//Selecciono Minuto
            Opcion=5;
         }
         
         if(RX_Buffer[4]==0x0c){//Selecciono Dia
            Opcion=1;
         }
         
         if(RX_Buffer[4]==0x0d){//Selecciono Hora
            Opcion=2;
         }
         
         if(RX_Buffer[4]==0x0e){//Selecciono Year
            Opcion=3;
         }
         
         if(RIGHT){
            delay_ms(200);
            if(RIGHT){
               Opcion++;
            }
         }
         if(Opcion>5)
            Opcion=1;
         if(Opcion<1)
            Opcion=5;
            
         if(RX_Buffer[4]==0x11 || LEFT){//Selecciono Regresar
            printf("page Principal");
            SendDataDisplay();  
            
            
            if(esBisiesto(YearTx))
               Modulo=Bisiesto[MesTx];
            else
               Modulo=Regular[MesTx];
         
            dowTx=((YearTx-1)%7+((YearTx-1)/4-3*((YearTx-1)/100+1)/4)%7+Modulo+DiaTx%7)%7;
            rtc_set_datetime(DiaTx,MesTx,YearTx,dowTx,HoraTx,MinutoTx);
         }
         
         if((RX_Buffer[4]==0x2a)|| UP){//Tecla Arriba Oprimida
            delay_ms(200);
            if((RX_Buffer[4]==0x2a)|| UP){   
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
               RX_Buffer[4]=0x00;
               RX_Buffer2[4]=0x00;
            }
         }
         
         if(RX_Buffer[4]==0x2b || DOWN){//Tecla Abajo Oprimida
            delay_ms(200);
            if(RX_Buffer[4]==0x2b || DOWN){   
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
               RX_Buffer[4]=0x00;
               RX_Buffer2[4]=0x00;
            }
         }
      
      printf("thora.txt=\"%02u\"",HoraTx);
      SendDataDisplay();
      printf("tminutos.txt=\"%02u\"",MinutoTx);
      SendDataDisplay();
      printf("tdia.txt=\"%02u\"",DiaTx);
      SendDataDisplay();
      printf("tmes.txt=\"%02u\"",MesTx);
      SendDataDisplay();
      printf("tyear.txt=\"%02u\"",YearTx);
      SendDataDisplay();
      LimitaDia();  
    }
//_--------------------------------------------------------------------------------------------------------------------------------------------------------       

//_--------------------------------------------------------------------------------------------------------------------------------------------------------
   if(Menu==8){ //Menu de Rampas
      printf("fecha.txt=\"Fecha:%02u/%02u/20%02u Hora: %02u:%02u:%02u\"",Dia,Mes,Year,Hora,Minuto,Segundo);
      SendDataDisplay();
      
      for(i=0;i<20;i++){
         if(rampas[i].Horas==0 && rampas[i].Minutos==0){
            Segmentos=i;  
            break;
         }else{
            Segmentos=i+1;  
         }
      }
      
      printf("t3.txt=\"Pasos Programados %02u\"",Segmentos);
      SendDataDisplay();
      GuardaEEPROM=OFF;
      
      if(RX_Buffer[4]>0x00 && RX_Buffer[4]<21){//Personalizados
         printf("ciclo.txt=\"Paso %02u\"",RX_Buffer[4]);
         SendDataDisplay();          
         printf("temp.txt=\"Temperatura: %2.1f°C\"",rampas[RX_Buffer[4]-1].Temperatura);
         SendDataDisplay();
         printf("hum.txt=\"Humedad: %02u %%\"",rampas[RX_Buffer[4]-1].Humedad);
         SendDataDisplay();
         printf("tiempo.txt=\"Tiempo: %02.0f:%02.0f \"",rampas[RX_Buffer[4]-1].Horas,rampas[RX_Buffer[4]-1].Minutos);
         SendDataDisplay();
         CicloLibre=RX_Buffer[4];
      }else if(CicloLibre==0x00){
         printf("ciclo.txt=\"Paso --\"");
         SendDataDisplay();          
         printf("temp.txt=\"Temperatura: --\"");
         SendDataDisplay();
         printf("hum.txt=\"Humedad: --\"");
         SendDataDisplay();
         printf("tiempo.txt=\"Tiempo: --:--\"");
         SendDataDisplay();
      }else if(CicloLibre>0 && CicloLibre<21){
         printf("ciclo.txt=\"Paso %02u\"",CicloLibre);
         SendDataDisplay();          
         printf("temp.txt=\"Temperatura: %2.1f°C\"",rampas[CicloLibre-1].Temperatura);
         SendDataDisplay();
         printf("hum.txt=\"Humedad: %02u %%\"",rampas[CicloLibre-1].Humedad);
         SendDataDisplay();
         printf("tiempo.txt=\"Tiempo: %02.0f:%02.0f \"",rampas[CicloLibre-1].Horas,rampas[CicloLibre-1].Minutos);
         SendDataDisplay();
      }
      
      if(RX_Buffer[4]==0x20 && CicloLibre>0){//20, Temperatura
         printf("page Temperatura");
         SendDataDisplay();     
         rampa=ON;
      }
      
      if(RX_Buffer[4]==0x30 && CicloLibre>0){//30, Humedad
         printf("page Humedad");
         SendDataDisplay();     
         rampa=ON;
      }
      
      if(RX_Buffer[4]==0x40 && CicloLibre>0){//40, Tiempo
         printf("page Tiempo");
         SendDataDisplay();     
         rampa=ON;
      }
      
      if(RX_Buffer[4]==0x70){//70, Regresar
         printf("page Principal");
         SendDataDisplay();     
         rampa=OFF;
      }
   }
//_--------------------------------------------------------------------------------------------------------------------------------------------------------       

//_--------------------------------------------------------------------------------------------------------------------------------------------------------      
    if(Menu==15){//Menu de Clave Correcta
      if(!entro){
         delay_ms(2000);
         entro=ON;
      }
      printf("page Principal");
      SendDataDisplay();
   }
//_--------------------------------------------------------------------------------------------------------------------------------------------------------      

//_--------------------------------------------------------------------------------------------------------------------------------------------------------      
    if(Menu==16){//Menu de Clave InCorrecta
      if(!entro){
         delay_ms(2000);
         entro=ON;
      }
      printf("page Clave");
      SendDataDisplay();
   }
//_--------------------------------------------------------------------------------------------------------------------------------------------------------      

//_--------------------------------------------------------------------------------------------------------------------------------------------------------
      if(Menu==20){ //Menu de Funcionamiento
      GuardaEEPROM=OFF;
      printf("fecha.txt=\"Fecha:%02u/%02u/20%02u    Hora: %02u:%02u:%02u\"",Dia,Mes,Year,Hora,Minuto,Segundo);
      SendDataDisplay();

      if(RX_Buffer[4]==0x01){//Temperatura
         printf("page Temperatura");
         SendDataDisplay();
      }
      
      if(RX_Buffer[4]==0x02){//Humedad
         printf("page Humedad");
         SendDataDisplay();         
      }
      
      if(RX_Buffer[4]==0x03){//Tiempo
         printf("page Tiempo");
         SendDataDisplay();
      }
      
      if(RX_Buffer[4]==0x04){//Test de componentes
         printf("page Clave");
         SendDataDisplay();
         TipoClave=4;
      }
      
      if(RX_Buffer[4]==0x05){//Cambio Clave
         printf("page Clave");
         SendDataDisplay();
         TipoClave=2;
      }
      
      if(RX_Buffer[4]==0x06){//Fecha y Hora
         printf("page Fecha");
         SendDataDisplay();
         DiaTx=Dia;
         MesTx=Mes;
         YearTx=Year;
         HoraTx=Hora;
         MinutoTx=Minuto;
      }
      
      if(RX_Buffer[4]==0x07){//Configuracion Parametros
         printf("page Clave");
         SendDataDisplay();
         TipoClave=1;
      }
      
      if(RX_Buffer[4]==0x08){//Inicia Control Temperatura
         flagControlT=!flagControlT;
         RX_Buffer[4]=0x00;
         RX_Buffer2[4]=0x00;
      }
      
      if(RX_Buffer[4]==0x09){//Inicia Control Humedad
         flagControlH=!flagControlH;
         RX_Buffer[4]=0x00;
         RX_Buffer2[4]=0x00;
      }
      
      if(RX_Buffer[4]==0x0a){//Configuracion Parametros
         printf("page Rampas");
         SendDataDisplay();
      }
      
      if(RX_Buffer[4]==0x0b){//Inicia Control Rampa
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
            if(SetpointH<Humedad)
               Humidificar=ON;            
            else
               Humidificar=OFF;   
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
         RX_Buffer[4]=0x00;
         RX_Buffer2[4]=0x00;
      }

      if(!TERM && !DOOR && !Finalizo){
         if(flagRampa){
            iniciaConteo(); 
            printf("b5.pic=85");
            SendDataDisplay();
            printf("b4.pic=85");
            SendDataDisplay();
            printf("b6.pic=85");
            SendDataDisplay();
            controlaHumedad();
            controlaRampa();
            printf("t8.txt=\"Paso %02u\"",Segmento+1);
            //printf("t8.txt=\"%2.0f\"",Tiempo);
            SendDataDisplay();
         }else{
            printf("t8.txt=\"Info\"");
            SendDataDisplay();
            printf("b6.pic=84");
            SendDataDisplay();
            iniciaConteo();
            
            if(flagControlH){ 
               printf("b5.pic=85");
               SendDataDisplay();
               controlaHumedad();
            }else{
               printf("statusH.txt=\"Detenido\"");
               SendDataDisplay();
               printf("b5.pic=84");
               SendDataDisplay();
               Dehumidificador_off;
               Humidificador_off;    
            }
            
            if(flagControlT){
               printf("b4.pic=85");
               SendDataDisplay();
               controlaTemperatura();
            }else{
               printf("statusT.txt=\"Detenido\"");
               SendDataDisplay();
               printf("b4.pic=84");
               SendDataDisplay();
               SSR_off;
               SSR2_off;
               SSR3_off;
               Controlando=0;
               Rele5_off;
               Tpulso=0.0;
            }
         }
      }
      
      if(Finalizo==0){
         if(TERM){
            Rele5_off;
            Controlando=0;
            flag_tiempocalentamiento=0;
            if(flag_clear3==0){
               tiempo_esterilizacion=0;
               SSR_off;
               SSR2_off;
               SSR3_off;
               Tpulso=0.0;
               flag_clear3=1;
               flag_ganancia=0;
            }
            printf("status.txt=\"Estado:Alarma 01\"");
            SendDataDisplay();
            printf("statusH.txt=\"-----\"");
            SendDataDisplay();
            printf("statusT.txt=\"-----\"");
            SendDataDisplay();
            Ganancia=GananciaR*(Setpoint/44.5);
            Buzzer_on;
            delay_ms(1000);
            Buzzer_off;
            delay_ms(1000);
            Dehumidificador_off;
            Humidificador_off;
         }else{
            if(DOOR){
               Rele5_off;
               Controlando=0;
               flag_tiempocalentamiento=0;
               Ganancia=GananciaR*(Setpoint/44.5);
               tiempo_esterilizacion=0;
               SSR_off;
               SSR2_off;
               SSR3_off;
               Tpulso=0.0;
               flag_clear2=1;
               flag_ganancia=0;
               printf("status.txt=\"Estado:Puerta Abierta\"");
               SendDataDisplay();
               printf("statusH.txt=\"-----\"");
               SendDataDisplay();
               printf("statusT.txt=\"-----\"");
               SendDataDisplay();
               Dehumidificador_off;
               Humidificador_off;
            }else{
               if(LEVEL){
                  printf("status.txt=\"Estado:Llenando\"");
                  SendDataDisplay();
                  Humidificador_off;
                  Rele6_on;
               }else{
                  printf("status.txt=\"Estado:---\"");
                  SendDataDisplay();
                  Rele6_off;
               }
            }
         }
      }else{
         printf("t8.txt=\"Paso --\"");
         SendDataDisplay();
         printf("status.txt=\"Reinicie el equipo\"");
         SendDataDisplay();
         SSR_off;         
         SSR2_off;
         SSR3_off;
         Controlando=0;
         Tpulso=0.0;
         tiempo_esterilizacion=0;
         flagRampa=OFF;
         flagControlT=OFF;
         flagControlH=OFF;
         ApagaSalidas();
         printf("b4.pic=84");
         SendDataDisplay();
         printf("b5.pic=84");
         SendDataDisplay();
         printf("b6.pic=84");
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
 //----------------Muestra Temperatura--------------------------
         if(Pt>146.4){
            printf("temperatura.txt=\"T:----\"");
            SendDataDisplay();
            Desconectado=1;
         }else{
            if(Desconectado==1){
               Desconectado=0;
               delay_ms(2000);
               for(i=0;i<80;i++)
                  Temperatura=Leer_Sensor(80);
            }
            printf("temperatura.txt=\"T:%2.1f°C\"",Temperatura);
            SendDataDisplay();
         }
         //printf("statusH.txt=\"%2.1f°C %2.1f%% %2.1f\"",Tam,RH2,sensores(3));
         //SendDataDisplay();
         printf("stemperatura.txt=\"SET:%2.1f °C\"",Setpoint);
         SendDataDisplay();
         printf("shumedad.txt=\"SET:%2.1f %%\"",SetpointH);
         SendDataDisplay();
         printf("humedad.txt=\"HR:%2.1f %%\"",Humedad);
         SendDataDisplay();
         printf("stiempo.txt=\"SET:%02.0f:%02.0f (HH:MM)\"",hEstp,mEstp);
         SendDataDisplay();
         printf("tiempo.txt=\"%02.0f:%02.0f\"",hEst,mEst);
         SendDataDisplay();
         printf("potT.txt=\"Pot. T:%2.0f %%\"",Potencia);
         //printf("potT.txt=\"Pot:%2.0f%% %2.1f %2.4f\"",Potencia,TemperaturaIdeal,Pendiente);
         SendDataDisplay(); 
      } 
      
      // Si cumplio el tiempo de esterilización, Muestre ciclo finalizado y detenga control de temperatura.
      if((mEstp>0)||(hEstp>0)){
         CuentaTiempo();  
         if(flagRampa){
            if((mEst>=mEstp)&&(hEst>=hEstp)&& Finalizo==0){
               flag_calcula=0;
               if(Segmento<Segmentos){
                  Segmento++;
                  hEst=0;
                  mEst=0;
               }else{
                  Finalizo=1;
               }
               for(i=0;i<2;i++){
                  Buzzer_on;
                  delay_ms(1000);
                  Buzzer_off;
                  delay_ms(1000);
               }
            }
         }else{
            if((mEst>=mEstp)&&(hEst>=hEstp)&& tiempo_esterilizacion==1){
               if(flag_clear==0){
                  tiempo_esterilizacion=0;
                  Pausa=2;
                  flag_clear=1;
                  SSR_off;
                  SSR2_off;
                  SSR3_off;
                  Controlando=0;
                  Tpulso=0.0;
                  flag_ganancia=0;
                  Finalizo=1;
               }
               printf("status.txt=\"Finalizado\"");
               SendDataDisplay();
               ApagaSalidas();
               for(i=0;i<2;i++){
                  Buzzer_on;
                  delay_ms(1000);
                  Buzzer_off;
                  delay_ms(1000);
               }
            }
         }
      }else{
         Finalizo=1;
      }
    }
//_--------------------------------------------------------------------------------------------------------------------------------------------------------       

//_--------------------------------------------------------------------------------------------------------------------------------------------------------
      if(Menu==100){ //Menu de Configuración de Parametros de Control
     
      if(Opcion2>7)
         Opcion2=1;
      if(Opcion2<1)
         Opcion2=7;
      
      if(Opcion2==1){
         printf("Config.t2c.txt=\"Proporcional\"");
         SendDataDisplay();      
         printf("t3c.txt=\"%2.0f\"",Ganancia);
         SendDataDisplay();
         if(RX_Buffer[4]==0x0a || UP){//Tecla Arriba Oprimida
            delay_ms(20);
            if(RX_Buffer[4]==0x0a || UP){
               Ganancia+=1.0;
               RX_Buffer[4]=0x00;  
               RX_Buffer2[4]=0x00;            
            }
         }
            
         if(RX_Buffer[4]==0x0b || DOWN){//Tecla Abajo Oprimida
            delay_ms(20);
            if(RX_Buffer[4]==0x0b || DOWN){
               Ganancia-=1.0;
               RX_Buffer[4]=0x00;
               RX_Buffer2[4]=0x00;            
            }
         }
         
         if(Ganancia>20)
            Ganancia=1;
         if(Ganancia<1)
            Ganancia=20;
      }else if(Opcion2==2){
         printf("Config.t2c.txt=\"Derivativo\"");
         SendDataDisplay();      
         printf("t3c.txt=\"%u\"",Derivativo);
         SendDataDisplay();
         if(RX_Buffer[4]==0x0a || UP){//Tecla Arriba Oprimida
            delay_ms(20);
            if(RX_Buffer[4]==0x0a || UP){
               Derivativo++;
               RX_Buffer[4]=0x00;  
               RX_Buffer2[4]=0x00;                           
            }
         }
            
         if(RX_Buffer[4]==0x0b || DOWN){//Tecla Abajo Oprimida
            delay_ms(20);
            if(RX_Buffer[4]==0x0b || DOWN){
               Derivativo--;
               RX_Buffer[4]=0x00;
               RX_Buffer2[4]=0x00;                           
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
         if(RX_Buffer[4]==0x0a || UP){//Tecla Arriba Oprimida
            delay_ms(20);
            if(RX_Buffer[4]==0x0a || UP){
               Integral++;
               RX_Buffer[4]=0x00;  
               RX_Buffer2[4]=0x00;            
            }
         }
            
         if(RX_Buffer[4]==0x0b || DOWN){//Tecla Abajo Oprimida
            delay_ms(20);
            if(RX_Buffer[4]==0x0b || DOWN){
               Integral--;
               RX_Buffer[4]=0x00;
               RX_Buffer2[4]=0x00;                           
            }
         }
         
         if(Integral>4)
            Integral=1;
         if(Integral<1)
            Integral=4;
      }else if(Opcion2==4){
         printf("Config.t2c.txt=\"Tciclo\"");
         SendDataDisplay();      
         printf("t3c.txt=\"%2.0f\"",Tciclo);
         SendDataDisplay();
         if(RX_Buffer[4]==0x0a || UP){//Tecla Arriba Oprimida
            delay_ms(20);
            if(RX_Buffer[4]==0x0a || UP){
               Tciclo+=1.0;
               RX_Buffer[4]=0x00;  
               RX_Buffer2[4]=0x00;            
            }
         }
            
         if(RX_Buffer[4]==0x0b || DOWN){//Tecla Abajo Oprimida
            delay_ms(20);
            if(RX_Buffer[4]==0x0b || DOWN){
               Tciclo-=1.0;
               RX_Buffer[4]=0x00;
               RX_Buffer2[4]=0x00;            
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
         if(RX_Buffer[4]==0x0a || UP){//Tecla Arriba Oprimida
            delay_ms(20);
            if(RX_Buffer[4]==0x0a || UP){
               if(Ajuste<40)
                  Ajuste++;
               RX_Buffer[4]=0x00;  
               RX_Buffer2[4]=0x00;            
            }
         }
            
         if(RX_Buffer[4]==0x0b || DOWN){//Tecla Abajo Oprimida
            delay_ms(20);
            if(RX_Buffer[4]==0x0b || DOWN){
               if(Ajuste>0)
                  Ajuste--;
               RX_Buffer[4]=0x00;
               RX_Buffer2[4]=0x00;            
            }
         }
      }else if(Opcion2==6){
         printf("Config.t2c.txt=\"Ajuste -\"");
         SendDataDisplay();      
         printf("t3c.txt=\"%u\"",Ajusten);
         SendDataDisplay();
         if(RX_Buffer[4]==0x0a || UP){//Tecla Arriba Oprimida
            delay_ms(20);
            if(RX_Buffer[4]==0x0a || UP){
               if(Ajusten<40)
                  Ajusten++;
               RX_Buffer[4]=0x00;  
               RX_Buffer2[4]=0x00;            
            }         
         }
            
         if(RX_Buffer[4]==0x0b || DOWN){//Tecla Abajo Oprimida
            delay_ms(20);
            if(RX_Buffer[4]==0x0b || DOWN){
               if(Ajusten>0)
                  Ajusten--;
               RX_Buffer[4]=0x00;
               RX_Buffer2[4]=0x00;            
            }
         }
      }else if(Opcion2==7){
         printf("Config.t2c.txt=\"Media\"");
         SendDataDisplay();      
         printf("t3c.txt=\"%u\"",MediaMovil);
         SendDataDisplay();
         if(RX_Buffer[4]==0x0a || UP){//Tecla Arriba Oprimida
            delay_ms(20);
            if(RX_Buffer[4]==0x0a || UP){
               if(MediaMovil<80)
                  MediaMovil++;
               RX_Buffer[4]=0x00;  
               RX_Buffer2[4]=0x00;            
            }
         }
            
         if(RX_Buffer[4]==0x0b || DOWN){//Tecla Abajo Oprimida
            delay_ms(20);
            if(RX_Buffer[4]==0x0b || DOWN){
               if(MediaMovil>10)
                  MediaMovil--;
               RX_Buffer[4]=0x00;
               RX_Buffer2[4]=0x00;            
            }            
         }
      }
      
      if(RX_Buffer[4]==0x0c || LEFT){//Tecla Izquierda Oprimida
         delay_ms(20);
         if(RX_Buffer[4]==0x0c || LEFT){
            Opcion2++;
            RX_Buffer[4]=0x00;  
            RX_Buffer2[4]=0x00;            
         }
      }
         
      if(RX_Buffer[4]==0x0d || RIGHT){//Tecla Derecha Oprimida
         delay_ms(20);
         if(RX_Buffer[4]==0x0d || RIGHT){
            Opcion2--;
            RX_Buffer[4]=0x00;
            RX_Buffer2[4]=0x00;            
         }
      }
      
      if(RX_Buffer[4]==0x11 || (RIGHT && LEFT)){//Tecla Regresar
         delay_ms(20);
         if(RX_Buffer[4]==0x11 || (RIGHT && LEFT)){
            if(!GuardaEEPROM){
               write_eeprom(32,(int8)Integral);
               delay_ms(10);
               write_eeprom(31,(int8)Derivativo);
               delay_ms(10);
               write_eeprom(30,(int8)Ganancia);
               delay_ms(10);
               write_eeprom(34,Ajuste);
               delay_ms(10);
               write_eeprom(35,Ajusten);
               delay_ms(10);
               write_eeprom(36,(int8)MediaMovil);
               delay_ms(10);
               write_eeprom(33,(int8)Tciclo);
               delay_ms(10);
               GuardaEEPROM=ON;
               Buzzer_on;
               delay_ms(1000);
               Buzzer_off;
            }
            printf("page Principal");
            SendDataDisplay();
         }
      }
      printf("fecha.txt=\"Fecha:%02u/%02u/20%02u    Hora: %02u:%02u\"",Dia,Mes,Year,Hora,Minuto);
      SendDataDisplay(); 
    }
//_--------------------------------------------------------------------------------------------------------------------------------------------------------       
      
   }
}
