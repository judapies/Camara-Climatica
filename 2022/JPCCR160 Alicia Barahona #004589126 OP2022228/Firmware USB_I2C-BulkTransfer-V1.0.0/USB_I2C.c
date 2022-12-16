// Cuatro pulsadores de entrada (Derecha, Izquierda, arriba y abajo).
// Reloj con XT de 4 MHz.
// Programaci�n para Autoclave con Calderin
// Tiene Men�:Ciclo: Liquidos, Instrumental suave, Textil Instrumental y Caucho.
// Tiene 6 niveles programables para desfogue suave.
// Ing. Juan David Pi�eros.
// JP Inglobal. 2015

#include <18F2550.h>
#device adc=10
#fuses HSPLL,WDT4096,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV1,VREGEN,NOMCLR
#use delay(clock=48000000)
//#use RS232(BAUD=19200, BITS=8, PARITY=N, XMIT=PIN_C6, RCV=PIN_C7, stream=printer)
#use i2c(slave, slow, sda=PIN_B0, scl=PIN_B1, address=0xB0)
#use rs232(baud=9700, xmit=PIN_C6, rcv=PIN_C7, bits=8, parity=N,stop=1, ERRORS)

#define USB_HID_DEVICE FALSE // deshabilitamos el uso de las directivas HID
#define USB_EP1_TX_ENABLE USB_ENABLE_BULK // turn on EP1(EndPoint1) for IN bulk/interrupt transfers
#define USB_EP1_RX_ENABLE USB_ENABLE_BULK // turn on EP1(EndPoint1) for OUT bulk/interrupt transfers
#define USB_EP1_TX_SIZE 32 // size to allocate for the tx endpoint 1 buffer
#define USB_EP1_RX_SIZE 32 // size to allocate for the rx endpoint 1 buffer
#include <pic18_usb.h> // Microchip PIC18Fxx5x Hardware layer for CCS's PIC USB driver
#include "header.h" // Configuraci�n del USB y los descriptores para este dispositivo
#include <usb.c> // handles usb setup tokens and get descriptor reports
const int8 Lenbuf = 32; 
int8 recbuf[Lenbuf];
int8 rxubuf[Lenbuf];

#define   LEVEL_L          !input(PIN_A0)
#define   LEVEL_H          !input(PIN_A1)

#define   SSR1_on          output_bit(PIN_C0,1)
#define   SSR1_off         output_bit(PIN_C0,0)

#define   SSR2_on          output_bit(PIN_C1,1)
#define   SSR2_off         output_bit(PIN_C1,0)

#define   Bomba_on          output_bit(PIN_C2,1)
#define   Bomba_off         output_bit(PIN_C2,0)

// Variables para Pt100
short conteoLlenado=0,envia=0,ajustaIP=0;
float d3=0.0;
float Temperatura=0.0,TempUSB=0.0,Temporal=0.0,Temperatura2=0.0,Humedad=0;
int16 tlevelL=0,tlevelH=0,tLlenado=0,tiempos=0,n=0;

byte fstate;                     //Guardara el estado del bus I2C
byte posicion, buffer[0x20], txbuf[0x20],rxbuf[0x20];     //Buffer de memoria
int8 IP[4]={0,0,0,0};
int8 Gateway[4]={0,0,0,0};
int8 Subnet[4]={0,0,0,0};
int8 MAC[6]={0,0x04,0xA3,0x06,0x07,0};

#INT_SSP
void ssp_interupt (){
   int incoming;                //Variable donde se recibe el byte que manda el maestro
   
   fstate = i2c_isr_state();    //Lectura del estado del bus I2c la interrupci�n
   /* Solicitud de lectura del esclavo por el master */
   if(fstate == 0x80) {         
       //Manda al maestro la informaci�n contenida en la posici�n de memoria que le ha solicitado
      i2c_write (txbuf[posicion]);
   }else{                              //Sino es que hay dato en el bus I2C...
      incoming = i2c_read();           //... lo lee
      if (fState == 1) {          //Informaci�n recibida corresponde a la posicion
         posicion = incoming;          //Se guarda posici�n
      }else if (fState == 2) {          //Informaci�n recibida corresponde al dato
         rxbuf[posicion] = incoming;
      }
   }
}

void Lee_Vector(void){
   if(rxbuf[7]==255 && rxbuf[8]==255 && rxbuf[9]==128){
      Temporal=(float)rxbuf[1];
      Temperatura=rxbuf[0];
      Temperatura=Temperatura+(Temporal/10);
      Temporal=(float)rxbuf[3];
      Temperatura2=rxbuf[2];
      Temperatura2=Temperatura2+(Temporal/10);
      Temporal=(float)rxbuf[5];
      Humedad=rxbuf[4];
      Humedad=Humedad+(Temporal/10);
      IP[0]=rxbuf[10];
      IP[1]=rxbuf[11];
      IP[2]=rxbuf[12];
      IP[3]=rxbuf[13];
      Subnet[0]=rxbuf[14];
      Subnet[1]=rxbuf[15];
      Subnet[2]=rxbuf[16];
      Subnet[3]=rxbuf[17];
      MAC[0]=rxbuf[18];
      MAC[1]=rxbuf[19];
      MAC[2]=rxbuf[20];
      MAC[3]=rxbuf[21];
      MAC[4]=rxbuf[22];
      MAC[5]=rxbuf[23];
   }
}

void controlaTemperatura(){
   if(Temperatura2>=(Temperatura-1) && Temperatura2<(Temperatura+1)){
      SSR1_off;SSR2_off;//apaga
   }else{
      SSR1_on;SSR2_off;//Enfria
   }
}

#int_TIMER1 
void temp1s(void){
   set_timer1(5536); // Se repite cada 5ms 
   if(LEVEL_L){
      if(tlevelL<3000)
         tlevelL++;
   }else{
      tlevelL=0;
   }
      
   if(LEVEL_H){
      if(tlevelH<3000)
         tlevelH++;
   }else{
      tlevelH=0;   
   }
   tiempos++;
   if(tiempos>=200){
      tiempos=0;
      envia=1;
      if(conteoLlenado){
         tLlenado++;
      }else{
         tLlenado=0;
      }
   }
}
void main(){
   fState = 0;
   for (posicion=0;posicion<0x10;posicion++){
      buffer[posicion] = 0x00;
      txbuf[posicion] = 0x00;
      rxbuf[posicion] = 0x00;
   }
   
   Lee_Vector();
   enable_interrupts(INT_SSP);
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_1);
   enable_interrupts(INT_TIMER1);
   usb_init();
   usb_task();
   setup_wdt(WDT_ON);
   enable_interrupts(global);
   Bomba_off;
   tLevelL=3000;

while(true){
   Lee_Vector();
   restart_wdt();
   
   if(tLevelL>400){
      txbuf[1]=5;//Nivel Bajo con agua
   }else{
      txbuf[1]=10;//Nivel Bajo sin agua
   }
   
   if(tLevelH>400){
      txbuf[2]=5;//Nivel Alto con agua
   }else{
      txbuf[2]=10;//Nivel Alto sin agua
   }
   
   if(input_state(PIN_C2)){
      txbuf[3]=5;//Bomba Activada
   }else{
      txbuf[3]=10;//Bomba desactivada
   }
   
   if(tLevelL>400){
      if(Temperatura2>2.0 && Temperatura2<50.0)
         controlaTemperatura();
   }else{
      SSR1_off;SSR2_off;//Apaga
   }
   
   if(tLlenado<300){   
      if(tLevelL==0){
         Bomba_on;
         conteoLlenado=1;
      }else{
         if(tlevelH>=400){
            Bomba_off;
            conteoLlenado=0;
         }
      }
   }
   
   if(tLlenado>=300){
      txbuf[0]=10;//Alarma de llenado
      Bomba_off;
   }else if(tlevelH>=600){
      txbuf[0]=20;//indicacion de que esta lleno el humidificador
   }else if(tlevelL==0){
      txbuf[0]=30;//indicacion de que esta sin agua el humidificador
   }else{
      txbuf[0]=0;//indicacion de que esta sin agua el humidificador
   }
   
   if(envia){
      envia=0;
         putc(0x65);//[0]
         delay_us(10);
         putc(0xff);//[1]
         delay_us(10);
         putc(0xff);//[2]
         delay_us(10);
         putc(rxbuf[0]);//[3]
         delay_us(10);
         putc(rxbuf[1]);//[4]
         delay_us(10);
         putc(rxbuf[2]);//[5]
         delay_us(10);
         putc(rxbuf[3]);//[6]
         delay_us(10);
         putc(rxbuf[4]);//[7]
         delay_us(10);
         putc(rxbuf[5]);//[8]
         delay_us(10);
         putc(0x00);//[9]
         delay_us(10);
         putc(0xff);//[10]
         delay_us(10);
         putc(IP[0]);//[11]
         delay_us(10);
         putc(IP[1]);//[12]
         delay_us(10);
         putc(IP[2]);//[13]
         delay_us(10);
         putc(IP[3]);//[14]
         delay_us(10);
         putc(Subnet[0]);//[15]
         delay_us(10);
         putc(Subnet[1]);//[16]
         delay_us(10);
         putc(Subnet[2]);//[17]
         delay_us(10);
         putc(Subnet[3]);//[18]
         delay_us(10);
         putc(0x01);//[19]
         delay_us(10);
         putc(MAC[0]);//[20]
         delay_us(10);
         putc(MAC[1]);//[21]
         delay_us(10);
         putc(MAC[2]);//[22]
         delay_us(10);
         putc(MAC[3]);//[23]
         delay_us(10);
         putc(MAC[4]);//[24]
         delay_us(10);
         putc(MAC[5]);//[25]
         delay_us(10);
      
      //printf("T%2.1f,T2%2.1f,H%2.1f\r\n",Temperatura,Temperatura2,Humedad);
   }
//_Menu_De_USB--------------------------------------------------------------------------------------------------------------------------------------------    
if(usb_enumerated()){
   
   if(usb_kbhit(1)){
      usb_get_packet(1, rxubuf, Lenbuf);
   }
   
   TempUSB=Temperatura;
   recbuf[0]=rxbuf[0];
   recbuf[1]=120;
   d3=TempUSB-(int8)TempUSB;
   d3=d3*10.0;
   recbuf[2]=rxbuf[1];
   recbuf[3]=rxbuf[2];
   recbuf[4]=rxbuf[3];
   recbuf[5]=rxbuf[4];    
   recbuf[6]=rxbuf[5];
   recbuf[7]=IP[0];
   recbuf[8]=IP[1];
   recbuf[9]=IP[2];
   recbuf[10]=IP[3];     
   recbuf[11]=n;
   recbuf[13]=rxbuf[24];
   recbuf[14]=rxbuf[25];
                
    usb_put_packet(1, recbuf, Lenbuf, USB_DTS_TOGGLE); //y enviamos el mismo paquete de tama�o 32bytes del EP1 al PC
}
//_--------------------------------------------------------------------------------------------------------------------------------------------------------    
   }
}