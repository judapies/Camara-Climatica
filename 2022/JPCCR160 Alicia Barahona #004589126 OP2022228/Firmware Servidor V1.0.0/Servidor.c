//Falta crear menu de ajuste de variables y de norma CFR21

#include <18F4685.h>
#device adc=10
#device HIGH_INTS=TRUE //Activamos niveles de prioridad
#fuses HS,WDT4096,NOPROTECT,NOLVP,NODEBUG,NOSTVREN,PUT,NOCPD,NOWRT,NOIESO,NOFCMEN,NOPBADEN,NOWRTC,NOWRTB,NOEBTR,NOEBTRB,NOCPB,NOXINST,NOLPT1OSC,NOMCLR,NOBROWNOUT,BORV43
#use delay(clock=20000000)
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7, bits=8, parity=N,BRGH1OK,ERRORS)
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/** Configuraci�n para el uso del stack tcip **/
#define STACK_USE_ICMP        1  //M�dulo de respuesta ICMP (ping)
#define STACK_USE_ARP         1  //Para solucionar direccionamiento MAC de las IP
#define STACK_USE_TCP         1  //Para enviar paquetes TCP 
#define STACK_USE_HTTP        1  //Uso de las funciones http del stack.
#define STACK_USE_CCS_PICENS  1  //CCS PICENS (Embedded Ethernet) 18F4620 + ENC28J60
#define STACK_USE_MCPENC      1  //Uso del enc28j60 por el stack (stacktsk.c)
#define STACK_USE_MAC         1  //Uso de la tarjeta de red

#define HTTP_SOCKET 80         //N� de puerto asociado al socket.
int8 AD0;
/********** Definici�n del patillaje de conexi�n al enc28j60 ******************/
/* Existen varias posibilidades:

Opci�n 1. No definir nada, en cuyo caso se implementar� una comunicaci�n SPI por 
software y se tomar�n la definici�n de patillas establecida en enc28j60.c

SO  PIN_D7 ---- ENC >>>> PIC
SI  PIN_D6 ---- PIC >>>> ENC
CLK PIN_D5
CS  PIN_D4
RST PIN_D3
INT PIN_D2
WOL PIN_D1

Opci�n 2. Definir todas las patillas de la comunicaci�n SPI, en cuyo caso se 
implementar� una comunicaci�n SPI por software con la definici�n de patillas 
elegida. Por ejemplo...

#define PIN_ENC_MAC_SO  PIN_C4   //Entrada serie de datos
#define PIN_ENC_MAC_SI  PIN_C5   //Salida serie de datos
#define PIN_ENC_MAC_CLK PIN_B4   //Se�al de reloj
#define PIN_ENC_MAC_CS  PIN_B5   //Chip select
#define PIN_ENC_MAC_RST PIN_B6   //Reset
#define PIN_ENC_MAC_INT PIN_B7   //Interrupci�n

Opci�n 3. El que aqu� se ha utilizado, que consiste en habilitar el uso de SPI 
por hardware del PIC y definir las patillas ajenas al hardware del m�dulo SPI 
(CS, INT y RST). En este caso es imprescindible definir tambi�n la patilla SO 
para que el stack (dentro de enc28j60.c) no habilite SPI por software. Da igual 
que patilla SO se defina, la que se deber� cablear ser� la SO real del PIC    */

#define ENC_MAC_USE_SPI 1           //Uso del SPI por hardware

#define PIN_ENC_MAC_SO  PIN_C5      //Entrada serie de datos
#define PIN_ENC_MAC_SI  PIN_C4    //Salida serie de datos (no necesario definir)
#define PIN_ENC_MAC_CLK PIN_C3    //Se�al de reloj  (no necesario definir)
#define PIN_ENC_MAC_CS  PIN_C1      //Chip select
#define PIN_ENC_MAC_RST PIN_C0      //Reset
#define PIN_ENC_MAC_INT PIN_C2      //Interrupci�n

/******************************************************************************/

#include "tcpip/stacktsk.c"      //Carga el stack TCP/IP de Microchip 
#if defined(__PCH__)
   #define GetSystemClock()       getenv("CLOCK")
   #define GetInstructionClock()  (GetSystemClock()/4)
   #define GetPeripheralClock()   (unsigned int32)GetInstructionClock()
#else
   #define GetSystemClock()       getenv("CLOCK")
   #define GetInstructionClock()  (GetSystemClock()/2)
   #define GetPeripheralClock()   (GetSystemClock()/2)
#endif

#define MIN(a,b)  ((a > b) ? b : a)

#define RX_BUFFER_SIZE  27
char Rx_Buffer[RX_BUFFER_SIZE+1];
char Rx_Buffer2[RX_BUFFER_SIZE+1];
int8 RX_Wr_Index=0;
int8 Dato2=0,Inicio=0,Dato_Exitoso=0,i=0,z=0,cont=0,n=0;
int8 IP[4]={0,0,0,0};
int8 Gateway[4]={0,0,0,0};
int8 Subnet[4]={0,0,0,0};
int8 MAC[6]={0,0x04,0xA3,0x06,0x07,0};

#include "pages.h"

#int_rda
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
      if(Rx_Buffer2[0]==0x65 && Rx_Buffer2[1]==0xff && Rx_Buffer2[9]==0x00 && Rx_Buffer2[10]==0xff ){
         Dato_Exitoso=5;
      }else{
         Dato_Exitoso=10;
      }
   }
   //output_toggle(PIN_C0);
}

void LeeDisplay(void){    
    if(Dato_Exitoso==5){
      for(i=0;i<27;i++){
         Rx_Buffer[i]=Rx_Buffer2[i];
      }
         
      if(Rx_Buffer[3]==0x01){// Pregunta por la pagina en la que esta el display,01 es Contrase�a de Acceso
      }
   }else{
      for(z=0;z<RX_BUFFER_SIZE;z++){
            //Rx_Buffer[z]=0;
            //Rx_Buffer2[z]=0;
         }
   }
}

void MACAddrInit(void) {
   MY_MAC_BYTE1=0;
   MY_MAC_BYTE2=0x04;
   MY_MAC_BYTE3=0xA3;
   MY_MAC_BYTE4=0x06;
   MY_MAC_BYTE5=0x07;
   MY_MAC_BYTE6=0x08;
}

void IPAddrInit(void) {
   //Elecci�n de la direcci�n IP. 
   MY_IP_BYTE1=192;
   MY_IP_BYTE2=168;
   MY_IP_BYTE3=0;
   MY_IP_BYTE4=118;

   //Elecci�n de la direcci�n de puerta de enlace. 
   MY_GATE_BYTE1=192;
   MY_GATE_BYTE2=168;
   MY_GATE_BYTE3=0;
   MY_GATE_BYTE4=1;

   //Elecci�n de la m�scara de red.Si no se indica nada se tomar� 255.255.255.0
   MY_MASK_BYTE1=255;
   MY_MASK_BYTE2=255;
   MY_MASK_BYTE3=255;
   MY_MASK_BYTE4=0;
}

/*
void MACAddrInit(void) {
   MY_MAC_BYTE1=MAC[0];
   MY_MAC_BYTE2=MAC[1];
   MY_MAC_BYTE3=MAC[2];
   MY_MAC_BYTE4=MAC[3];
   MY_MAC_BYTE5=MAC[4];
   MY_MAC_BYTE6=MAC[5];
}

void IPAddrInit(void) {
   //Elecci�n de la direcci�n IP. 
   MY_IP_BYTE1=IP[0];
   MY_IP_BYTE2=IP[1];
   MY_IP_BYTE3=IP[2];
   MY_IP_BYTE4=IP[3];

   //Elecci�n de la direcci�n de puerta de enlace. 
   MY_GATE_BYTE1=IP[0];
   MY_GATE_BYTE2=IP[1];
   MY_GATE_BYTE3=IP[2];
   MY_GATE_BYTE4=Gateway[0];

   //Elecci�n de la m�scara de red.Si no se indica nada se tomar� 255.255.255.0
   MY_MASK_BYTE1=Subnet[0];
   MY_MASK_BYTE2=Subnet[1];
   MY_MASK_BYTE3=Subnet[2];
   MY_MASK_BYTE4=Subnet[3];
}
*/

/***************************  FUNCI�N GET PAGE   ******************************/
/* Esta funci�n devuelve la posici�n de memoria donde se encuentra la p�gina web 
a mostrar. En este caso se trata de una web con 2 p�ginas. Una principal index(/) 
y una secundaria(/lecturas)                                                   */

int32 http_get_page(char *file_str) {
   int32 file_loc=0;
   static char index[]="/";
   static char lecturas[]="/lecturas";

   /* Busca la posici�n de memoria donde se encuentra la p�gina solicitada */
   if (stricmp(file_str,index)==0)                 //Si es la principal...
      file_loc=label_address(HTML_INDEX_PAGE);     //...toma su posici�n en la memoria
   //else if (stricmp(file_str,lecturas)==0)            //O si es la secundaria...
     // file_loc=label_address(HTML_LECTURAS_PAGE);    //...toma su posici�n en la memoria

   
   /* Devuelve la posici�n en la memoria donde se encuentra la p�gina a mostrar */
   return(file_loc);                           
}

/**************************  FUNCI�N FORMAT CHAR  *****************************/
/* Con  la funci�n http_format_char  interconectamos las variables virtuales de 
la p�gina web con las variables del programa del PIC. Se encarga de enviar los 
cambios producidos en la aplicaci�n del PIC y reflejarlos en la aplicaci�n web. 
Muestra,por tanto, las lectura obtenidas por el PIC y las representa en la 
aplicaci�n de la p�gina web      

%0 es la variable virtual para representar el valor de la lectura del canal 
anal�gico
%1 es la variable virtual para representar el valor de la lectura del bit 0 del 
puerto E.
*/
int8 http_format_char(char* file, char id, char *str, int8 max_ret) {
   char new_str[60];
   int8 len=0;
   
   int8 RE0;

   *str=0;

   switch(id) {
      case '0':
         sprintf(new_str,"%02u.%01u�C",Rx_Buffer[3],Rx_Buffer[4]);
         len=strlen(new_str);
         strncpy(str, new_str, max_ret);
      break;
      case '1':
         sprintf(new_str,"%02u.%01u�C",Rx_Buffer[5],Rx_Buffer[6]);
         len=strlen(new_str);
         strncpy(str, new_str, max_ret);
      break;  
      case '2':
         sprintf(new_str,"%02u.%01u%%",Rx_Buffer[7],Rx_Buffer[8]);
         len=strlen(new_str);
         strncpy(str, new_str, max_ret);
      break;  /*
      case '3':
         sprintf(new_str,"%u.%u.%u.%u-%u.%u.%u.%u- %02u",Rx_Buffer[11],Rx_Buffer[12],Rx_Buffer[13],Rx_Buffer[14],Rx_Buffer[15],Rx_Buffer[16],Rx_Buffer[17],Rx_Buffer[18],n);
         len=strlen(new_str);
         strncpy(str, new_str, max_ret);
      break;  
      case '4':
         sprintf(new_str,"%02X.%02X.%02X.%02X.%02X.%02X",Rx_Buffer[20],Rx_Buffer[21],Rx_Buffer[22],Rx_Buffer[23],Rx_Buffer[24],Rx_Buffer[25]);
         len=strlen(new_str);
         strncpy(str, new_str, max_ret);
      break;  
      */
      default:
      len=0;
   }

   if (len)strncpy(str, new_str, max_ret);
   else  *str=0;
   
   return(len);
}

/***************************  FUNCI�N EXEC CGI   ******************************/
/* Con la funci�n http_exec_cgi interconectamos las variables virtuales de la 
p�gina web con las variables del programa del PIC. Se encarga de recibir 
los cambios producidos en la aplicaci�n web y reflejarlos en el hardware del PIC. 
Ejecuta, por tanto, la acci�n elegida seg�n el valor de la variable virtual recibida 
de la p�gina web

key es la variable virtual que viene de la pagina web
val es el valor de una variable virtual de la p�gina web
file es la direcci�n de la p�gina web devuelta por http_get_page ()

*/

void http_exec_cgi(int32 file, char *key, char *val) {
   
}

/************************** FUNCI�N PRINCIPAL *********************************/
void main(void) {
   setup_wdt(WDT_ON);
   enable_interrupts(INT_RDA);
   enable_interrupts(GLOBAL);
   delay_ms(100);
   /* Inicializaci�n del Stack */
   MACAddrInit(); //Se asigna la direcci�n MAC elegida 
   IPAddrInit();  //Se asigna IP, mascara de red y puerta de enlace elegidos
   StackInit();   //Inicializa el stack   
   delay_ms(10);

   while(TRUE) {
      restart_wdt();
      StackTask();
      LeeDisplay();

      MY_IP_BYTE1=Rx_Buffer[11];
      MY_IP_BYTE2=Rx_Buffer[12];
      MY_IP_BYTE3=Rx_Buffer[13];
      MY_IP_BYTE4=Rx_Buffer[14];

      //Elecci�n de la direcci�n de puerta de enlace. 
      MY_GATE_BYTE1=Rx_Buffer[11];
      MY_GATE_BYTE2=Rx_Buffer[12];
      MY_GATE_BYTE3=Rx_Buffer[13];
      MY_GATE_BYTE4=Rx_Buffer[19];
   
      //Elecci�n de la m�scara de red.Si no se indica nada se tomar� 255.255.255.0
      MY_MASK_BYTE1=Rx_Buffer[15];
      MY_MASK_BYTE2=Rx_Buffer[16];
      MY_MASK_BYTE3=Rx_Buffer[17];
      MY_MASK_BYTE4=Rx_Buffer[18];
      
      //MY_MAC_BYTE1=Rx_Buffer[20];
      //MY_MAC_BYTE2=Rx_Buffer[21];
      //MY_MAC_BYTE3=Rx_Buffer[22];
      //MY_MAC_BYTE4=Rx_Buffer[23];
      //MY_MAC_BYTE5=Rx_Buffer[24];
      //MY_MAC_BYTE6=Rx_Buffer[25];
    }
}

