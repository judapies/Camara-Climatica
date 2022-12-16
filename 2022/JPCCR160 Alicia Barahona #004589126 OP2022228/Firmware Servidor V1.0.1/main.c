#include <main.h>
#use standard_io(b)
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7, bits=8, parity=N,BRGH1OK,ERRORS)

#define RX_BUFFER_SIZE  27
char Rx_Buffer[RX_BUFFER_SIZE+1];
char Rx_Buffer2[RX_BUFFER_SIZE+1];
int8 RX_Wr_Index=0;
int8 Dato2=0,Inicio=0,Dato_Exitoso=0,i=0,z=0;

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

/* TODO: Web server enabled. Server files are added to the /pages
directory. You can modify the contents of the dynamic display fields
at the end of the index.htm file. By default, headings are in the left
column and the dynamic display elements are in the right. The text on
the input buttons can be set by changing the 'value' strings in the
form section. Dynamic display elements can be added or removed from
index.htm and index.xml */
/* TODO: Server files must be built into an MPFS image prior to 
compiling and runing the web server. Running makempfsimg.bat in the
/mpfs directory will create an MPFS image of the files in /pages.
A new MPFS image has to be compiled and re-loaded if a file in
/pages needs to be changed. */

unsigned int8 http_format_char(char* file, char id, char *str, unsigned int8 max_ret)
{
   /* TODO: This is a callback function to the HTTP stack.
   'id' is the char for the dynamic element to be updated.
   For each dynamic element you create, you need to parse
   and save a result to 'str'. *str is where the callback
   function must save the formatting result, and it should
   not store more than max_ret characters to this pointer
   (buffer-overrun protection).
   Example:
   switch(id) {
      case 0x00:
         set_adc_channel(0);
         delay_us(100);
         i=read_adc();
         sprintf(new_str,"0x%X", i);
   .....
   strncpy(str, new_str, max_ret);
   */
   char new_str[20];
   int8 len=0;
   
   if (id == 0){
         sprintf(new_str,"%02u.%01u",Rx_Buffer[3],Rx_Buffer[4]);
         len=strlen(new_str);
         strncpy(str, new_str, max_ret);
   }
   if (id == 1){
         sprintf(new_str,"%02u.%01u",Rx_Buffer[5],Rx_Buffer[6]);
         len=strlen(new_str);
         strncpy(str, new_str, max_ret);
   }
   if (id == 2){
         sprintf(new_str,"%02u.%01u%%",Rx_Buffer[7],Rx_Buffer[8]);
         len=strlen(new_str);
         strncpy(str, new_str, max_ret);
   }
   return(str);
}

void http_exec_cgi(char* file, char *key, char *val)
{
   /* TODO: This is a callback function to the HTTP stack.
   This function is called with each key/value pair read in
   the GET/POST request before any web data is sent to the
   web browser. 'key' matches the name of the field and
   'val' is the value it was changed to.
   Example:
   int8 v = atoi(val);
   if (stricmp(key,led1_key)==0)
      LedSet(0, v);
   */
 /*
   if (strcmp(key, "button00") == 0)
   {
         //TODO: Handle button00
         output_toggle(PIN_C7);
         delay_ms(100);
   }
*/
}

void IPAddressInit(void)
{
   //MAC address of this unit
   MY_MAC_BYTE1=MY_DEFAULT_MAC_BYTE1;
   MY_MAC_BYTE2=MY_DEFAULT_MAC_BYTE2;
   MY_MAC_BYTE3=MY_DEFAULT_MAC_BYTE3;
   MY_MAC_BYTE4=MY_DEFAULT_MAC_BYTE4;
   MY_MAC_BYTE5=MY_DEFAULT_MAC_BYTE5;
   MY_MAC_BYTE6=MY_DEFAULT_MAC_BYTE6;

   //IP address of this unit
   MY_IP_BYTE1=192;
   MY_IP_BYTE2=168;
   MY_IP_BYTE3=0;
   MY_IP_BYTE4=118;

   //network gateway
   MY_GATE_BYTE1=192;
   MY_GATE_BYTE2=168;
   MY_GATE_BYTE3=0;
   MY_GATE_BYTE4=1;

   //subnet mask
   MY_MASK_BYTE1=MY_DEFAULT_MASK_BYTE1;
   MY_MASK_BYTE2=MY_DEFAULT_MASK_BYTE2;
   MY_MASK_BYTE3=MY_DEFAULT_MASK_BYTE3;
   MY_MASK_BYTE4=MY_DEFAULT_MASK_BYTE4;
}

void main()
{
   setup_wdt(WDT_ON);
   enable_interrupts(INT_RDA);
   enable_interrupts(GLOBAL);
   delay_ms(100);
   IPAddressInit();
   TickInit();
   StackInit();


   while(TRUE)
   {
      restart_wdt();
      StackTask();
      StackApplications();
      
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
   }

}