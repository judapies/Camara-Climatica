void BorraBuffer(void){
RX_Buffer[0]=0;
RX_Buffer[1]=0;
RX_Buffer[2]=0;
RX_Buffer[3]=0;
RX_Buffer[4]=0;
RX_Buffer[5]=0;
RX_Buffer[6]=0;
RX_Buffer[7]=0;
RX_Buffer[8]=0;
RX_Buffer[9]=0;

RX_Buffer2[0]=0;
RX_Buffer2[1]=0;
RX_Buffer2[2]=0;
RX_Buffer2[3]=0;
RX_Buffer2[4]=0;
RX_Buffer2[5]=0;
RX_Buffer2[6]=0;
RX_Buffer2[7]=0;
RX_Buffer2[8]=0;
RX_Buffer2[9]=0;
}

void SendDataDisplay(void){
delay_us(10);
putc(0xFF);
delay_us(10);
putc(0xFF);
delay_us(10);
putc(0xFF);
delay_us(10);
}

short esBisiesto(int8 year) {
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

// Funcion para conversor analogo-digital
float sensores(int x){
float y;set_adc_channel(x);delay_ms(1);y=read_adc();return (y);
}
float Leer_Sensor(int media)
{
   Prom=0.0;
   V0 = sensores(0);   //Lectura de Divisor de Voltaje de PT100 con resistencia de 6.18k (+-2%)
   V0 = V0/105.08; // Se elimina la ganancia
   V0 += 15.05; // Se suma voltaje de divisor fijo
   Pt = 6790/((1023/V0)-1);
   
   if(l>(media-1))
   {l=0;}
   promedio[l]=Pt;l++;
   //promedio[l]=V0;l++;
         
   for(h=0;h<media;h++)
   {
      Prom+=promedio[h];
   } 
   Prom=Prom/media;
   Temperature=((Prom*Prom)*a)+(Prom*b)+c;  // Ecuación determinada por linealización en Matlab. Revisar este polinomio.
   return Temperature+Ajuste-Ajusten;   
}

float leeHumedad(int8 media){
   float VH=0.0,RH=0.0,Ta=0.0,VT=0.0,a=-2.4560,b=19.801,c=-73.195,d=122.55,PromH=0.0,PromTa=0.0;
   PromH=0.0;
   PromTa=0.0;
   VH=(sensores(2)/1023.0)*5.0;
   VT=(sensores(3)/1023.0)*5.0;
   VH=VH*1000;
   //VT=VT*1000;
   Ta=(pow(VT,3)*a)+(pow(VT,2)*b)+(VT*c)+d;
   Tam=Ta;
   if(f>(media-1))
      f=0;
   promedioTa[f]=Ta;f++;
         
   for(h=0;h<media;h++)
      PromTa+=promedioTa[h]; 
   PromTa=PromTa/media;
   
   RH =((-0.0000000019206*pow(VH,3))+( 0.000014370*pow(VH,2))+( 0.0034210*VH)-12.4)/(1+(PromTa-23)*( 0.0024));
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

void Controla(){
//-------------------------------Control de Calentamiento------------------------------------------------------------//         
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
   
   Rele5_on;
   
   if(flagRampa){
      if(Segmento==0){
         if(flag_calcula==0){
            Tiempo=(rampas[0].Horas*3600)+(rampas[0].Minutos*60);         
            Setpoint=rampas[0].Temperatura;
            Pendiente=((Setpoint-TempeInicial)/Tiempo);
            hEstp=rampas[0].Horas;
            mEstp=rampas[0].Minutos;
            flag_calcula=1;
            delay_ms(10);
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
      }else{
         if(flag_calcula==0){
            Tiempo=(rampas[Segmento].Horas*3600)+(rampas[Segmento].Minutos*60);         
            Setpoint=rampas[Segmento].Temperatura;
            Pendiente=((rampas[Segmento].Temperatura-rampas[Segmento-1].Temperatura)/Tiempo);
            hEstp=rampas[Segmento].Horas;
            mEstp=rampas[Segmento].Minutos;
            flag_calcula=1;
            delay_ms(10);
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
      }
   }
  
   if(Pendiente!=0 && flagRampa){// Si Esta realizando una Rampa debe realizar el control de acuerdo a la pendiente de Temperatura. Aprox 10°C * min Maximo Valor de Pendiente.
      //PendienteActual=((Setpoint-Temperatura)/TiempoActual);
      //PendienteMinutos=PendienteActual*60;
      //PotenciaEntregada=((PendienteMinutos+DifPendiente)*100)/penMufla;
      TiempoRampa=((hEst*3600)+(mEst*60)+tinicio);
      if(Segmento==0){
         TemperaturaIdeal=(Pendiente*TiempoRampa)+TempeInicial;
      }else{
         TemperaturaIdeal=(Pendiente*TiempoRampa)+rampas[Segmento-1].Temperatura;
      }
      PotenciaEntregada=((TemperaturaIdeal-Temperatura)*100)/penCamara;
// Pendiente determinar si el comportamiento de la mufla es lineal 10°C/min o si tiene un comportamiento diferente.         
      
      if(Pendiente<0){
         if(PotenciaEntregada<0 && PotenciaEntregada>-100.0){
            PotenciaEntregada=abs(PotenciaEntregada);     
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
         Rele1_off;
         Rele2_off;
         Rele3_off;
         Rele4_off;
      }else{
         Rele1_on;
         Rele2_on;
         Rele3_on;
         Rele4_on;
      }
      
      Tpulso=(PotenciaEntregada*Tciclo)/100;  
   }else{
      if(Enfria==1){
         Rele1_off;
         Rele2_off;
         Rele3_off;
         Rele4_off;
         error=Setpoint-desvio-Temperatura;      // Cálculo del error ******
         
         if(error<=-0.4 && Aumento>-0.2){
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
         Rele1_on;
         Rele2_on;
         Rele3_on;
         Rele4_on;
         error=desvio+Setpoint-Temperatura;      // Cálculo del error ******
      
         if(error>0.4 && error<30 && Aumento<0.3){
            flag_ajuste=0;
            flag_ajuste2=0;
            Ajusto=10;
            if(Potencia<90.0){
               if(TiempoControl>=80){
                  TiempoControl=0;
                  if(Aumento<-0.5){
                     Ganancia+=1;
                  }else{
                     if(Aumento<-0.1){
                        Ganancia+=0.6;
                     }else{
                        Ganancia+=0.3;
                     }
                  }
               }
            }
         }
         
         if(error<0.0){
            Ganancia=GananciaR*(Setpoint/44.5);
            flag_ajuste=0;
            flag_ajuste2=0;
         }
         
         if(Ganancia<0.1){
            Ganancia=0.1;
         }
         if(Ganancia>Gananciamax){
            Ganancia=Gananciamax;
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
      }  
   }
   Tp=(Tpulso/0.005);
   Tc=(Tciclo/0.005);
   Controlando=1;     
//--------------------------------------------------------------------------------------------------------------------//   
  
}

int8 extraeDecimal(float data){
   float entero=0;
   float tmp=0.0;
   
   entero=(int8)data;
   tmp=(data-entero)*10;
   return (int8)tmp;
}

float inttofloat(int8 decimal){
   float tmp;
   tmp=decimal;
   return tmp/10;
}

void ConfiguraInterrupciones(){
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_1);
   enable_interrupts(INT_TIMER1);
   setup_adc_ports(AN0_TO_AN4);
   setup_adc(ADC_CLOCK_DIV_64);
   port_b_pullups(true);
   
   setup_wdt(WDT_ON);
   enable_interrupts(global);
}  

void LeeEEPROM(){
   mEstp=read_eeprom(0);
   delay_ms(10);   
   hEstp=read_eeprom(1);
   delay_ms(10);   
   
   entero=read_eeprom(80);
   delay_ms(10);   
   Temporal=read_eeprom(81);
   Temporal=Temporal/10.0;
   Setpoint=entero+Temporal;
   delay_ms(10);   
   
   entero=read_eeprom(82);
   delay_ms(10);   
   Temporal=read_eeprom(83);
   Temporal=Temporal/10.0;
   SetpointH=entero+Temporal;
   delay_ms(10);   
   
   Password[0]=read_eeprom(10);
   delay_ms(10);   
   Password[1]=read_eeprom(11);
   delay_ms(10);   
   Password[2]=read_eeprom(12);
   delay_ms(10);   
   Password[3]=read_eeprom(13);
   delay_ms(10);   
   
   Ganancia=read_eeprom(30);
   delay_ms(10);   
   Derivativo=read_eeprom(31);
   delay_ms(10);   
   Integral=read_eeprom(32);
   delay_ms(10);   
   Tciclo=read_eeprom(33);
   delay_ms(10);   
   Ajuste=read_eeprom(34);
   delay_ms(10);   
   Ajusten=read_eeprom(35);
   delay_ms(10);   
   MediaMovil=read_eeprom(36);
   delay_ms(10);   
   Coeficiente=read_eeprom(37);
   delay_ms(10);   
   Saturacion=read_eeprom(38);
   delay_ms(10);   
   GananciaR=Ganancia;
   
   for(i=0;i<20;i++){//Lee valores almacenados de ciclos personalizados
      rampas[i].TemperaturaInt=read_eeprom(100+(i*5)); 
      rampas[i].TemperaturaDec=read_eeprom(101+(i*5)); 
      rampas[i].Humedad=read_eeprom(102+(i*5)); 
      rampas[i].Horas=read_eeprom(103+(i*5)); 
      rampas[i].Minutos=read_eeprom(104+(i*5)); 
      
      rampas[i].Temperatura=rampas[i].TemperaturaInt+inttofloat(rampas[i].TemperaturaDec);
      if(rampas[i].Temperatura>60.0)rampas[i].Temperatura=30.0;
      if(rampas[i].Humedad>90.0)rampas[i].Humedad=60.0;
      if(rampas[i].Horas>99.0)rampas[i].Horas=10.0;
      if(rampas[i].Minutos>59.0)rampas[i].Minutos=10.0;
   }
}

void LimitaValores(){
   if(mEstp>59){
      mEstp=10;write_eeprom(0,10);delay_ms(20);
   }
   if(hEstp>99){
      hEstp=10;write_eeprom(1,10);delay_ms(20);
   }
   if(Password[0]>9 || Password[0]<0){
      Password[0]=0;write_eeprom(10,0);delay_ms(20);
   }
   if(Password[1]>9 || Password[1]<0){
      Password[1]=0;write_eeprom(11,0);delay_ms(20);
   }
   if(Password[2]>9 || Password[2]<0){
      Password[2]=0;write_eeprom(12,0);delay_ms(20);
   }
   if(Password[3]>9 || Password[3]<0){
      Password[3]=0;write_eeprom(13,0);delay_ms(20);
   }
   if(Setpoint>60.0){
      Setpoint=30.0;write_eeprom(80,30);delay_ms(20);write_eeprom(81,0);delay_ms(20);
   }
   if(SetpointH>90.0 || SetpointH<10.0){
      SetpointH=50.0;write_eeprom(82,50);delay_ms(20);write_eeprom(83,0);delay_ms(20);
   }
   if(Ganancia>40){
      Ganancia=2;write_eeprom(30,2);delay_ms(20);
   }
   if(Derivativo>80){
      Derivativo=80;write_eeprom(31,80);delay_ms(20);
   }
   if(Integral>4){
      Integral=1;write_eeprom(32,1);delay_ms(20);
   }
   if(Tciclo>40){
      Tciclo=5;write_eeprom(33,5);delay_ms(20);
   }
   if(Ajuste>100){
      Ajuste=0;write_eeprom(34,0);delay_ms(20);
   }
   if(Ajusten>100){
      Ajusten=0;write_eeprom(35,0);delay_ms(20);
   }
   if(MediaMovil>80){
      MediaMovil=80;write_eeprom(36,80);delay_ms(20);
   }
}

void DelayPuls(){
   delay_ms(2);
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

void LeeDisplay(void){
   if(Dato_Exitoso==5){
         RX_Buffer[0]=RX_Buffer2[0];
         RX_Buffer[1]=RX_Buffer2[1];
         RX_Buffer[2]=RX_Buffer2[2];
         RX_Buffer[3]=RX_Buffer2[3];
         RX_Buffer[4]=RX_Buffer2[4];
         RX_Buffer[5]=RX_Buffer2[5];
         RX_Buffer[6]=RX_Buffer2[6];
         RX_Buffer[7]=RX_Buffer2[7];
         RX_Buffer[8]=RX_Buffer2[8];
         RX_Buffer[9]=RX_Buffer2[9];
         
      if(RX_Buffer[3]==0x01){// Pregunta por la pagina en la que esta el display,01 es Contraseña de Acceso
         Menu=0;
         if(MenuAntt!=Menu)
            MenuAntt=Menu;
      }else if(RX_Buffer[3]==0x02){//02 es Menu Principal con PantallaPriincipal=0, y con PantallaPrincipal=1 es Ciclo Libre
         Menu=1;
         if(MenuAntt!=Menu)
            MenuAntt=Menu;
      }else if(RX_Buffer[3]==0x04){//04 es Tiempo 
         Menu=2;
         if(MenuAntt!=Menu)
            MenuAntt=Menu;
      }else if(RX_Buffer[3]==0x05){//05 es Temperatura
         Menu=3;
         if(MenuAntt!=Menu)
            MenuAntt=Menu;
      }else if(RX_Buffer[3]==0x06){//06 es Humedad
         Menu=4;
         if(MenuAntt!=Menu)
            MenuAntt=Menu;
      }else if(RX_Buffer[3]==0x07){//07 es Test de Componentes
         Menu=6;
         if(MenuAntt!=Menu)
            MenuAntt=Menu;
      }else if(RX_Buffer[3]==0x08){//08 es Fecha y hora
         Menu=7;
         if(MenuAntt!=Menu)
            MenuAntt=Menu;
      }else if(RX_Buffer[3]==0x09){//09 es Ciclos Personalizados
         Menu=8;
         if(MenuAntt!=Menu)
            MenuAntt=Menu;
      }else if(RX_Buffer[3]==0x0f){//0f es Recibe caracteres de contraseña desde display
         
      }else if(RX_Buffer[3]==0x1a){//1a es Menu de Funcionamiento
         Menu=20;
         if(MenuAntt!=Menu)
            MenuAntt=Menu;
      }else if(RX_Buffer[3]==0x1b){//1b es Menu de clave correcta
         Menu=15;
         if(MenuAntt!=Menu)
            MenuAntt=Menu;
      }else if(RX_Buffer[3]==0x1c){//1c es Menu de clave incorrecta
         Menu=16;
         if(MenuAntt!=Menu)
            MenuAntt=Menu;
      }else if(RX_Buffer[3]==0x2c){//2c es Menu de Configuracion de Parametros
         Menu=100;
         if(MenuAntt!=Menu)
            MenuAntt=Menu;
      }else if(RX_Buffer[3]==0xcc){//cc es Menu de Bienvenida
         Menu=240;
         if(MenuAntt!=240)
            reset_cpu();
      }
   }else{
      for(z=0;z<RX_BUFFER_SIZE;z++){
            //Rx_Buffer[z]=0;
            //Rx_Buffer2[z]=0;
         }
   }
}

void ApagaSalidas(){
   Buzzer_off;
   SSR_off;
   SSR2_off;
   SSR3_off;
   Rele1_off;
   Rele2_off;
   Rele3_off;
   Rele4_off;
   Rele5_off;
   Rele6_off;
   Rele7_off;  
   Dehumidificador_off;
   Humidificador_off;
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

void controlaHumedad(){
   if(Humidificar){
      if(Humedad<SetpointH-2.0){
         printf("statusH.txt=\"Humidificando\"");
         SendDataDisplay();
         Dehumidificador_off;
         if(!LEVEL)
            Humidificador_on;
         else
            Humidificador_off;
         flagHumedad=OFF;
      }else if(Humedad<SetpointH-0.1 && Humedad>SetpointH-1.8){
         printf("statusH.txt=\"Controlando\"");
         SendDataDisplay();
         Dehumidificador_off;
         if(!LEVEL)
            Humidificador_on;
         else
            Humidificador_off;
         if(!Infinito)   
            flagHumedad=ON;
         else
            flagHumedad=OFF;
      }else if(Humedad>SetpointH && Humedad<SetpointH+2.0){
         Humidificador_off;
         Dehumidificador_off;
         if(!Infinito)   
            flagHumedad=ON;
         else
            flagHumedad=OFF;
      }else if(Humedad>SetpointH+2.0){
         Humidificar=OFF; 
      }
   }else{
      if(Humedad>SetpointH+2.0){
         printf("statusH.txt=\"Deshumidificando\"");
         SendDataDisplay();
         Dehumidificador_on;
         Humidificador_off;
         flagHumedad=OFF;
      }else if(Humedad>SetpointH && Humedad<SetpointH+1.8){
         printf("statusH.txt=\"Controlando\"");
         SendDataDisplay();
         Dehumidificador_on;
         Humidificador_off;
         if(!Infinito)   
            flagHumedad=ON;
         else
            flagHumedad=OFF;
      }else if(Humedad<SetpointH-0.1 && Humedad>SetpointH-2.0){
         Humidificador_off;
         Dehumidificador_off;
         if(!Infinito)   
            flagHumedad=ON;
         else
            flagHumedad=OFF;
      }else if(Humedad<SetpointH-2.0){
         Humidificar=ON; 
      }
   }
}

void controlaTemperatura(){
   if(Enfria==1){
      if(Temperatura<=Setpoint+0.5 && Temperatura>=Setpoint-0.5){
         printf("statusT.txt=\"Controlando\"");
         SendDataDisplay();
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
         Ganancia=GananciaR*(Setpoint/44.5);
         Gananciamax=(0.21)*Setpoint;
         SSR_off;
         SSR2_off;
         SSR3_off;
         Controlando=0;
      }else if(Temperatura>Setpoint+0.6){
         flag_tiempocalentamiento=0;
         printf("statusT.txt=\"Enfriando\"");
         SendDataDisplay();
         //tiempo_esterilizacion=0;
         flagTemperatura=OFF;
         Controla();
      }
   }else{  
      if(Temperatura>=Setpoint-0.5 && Temperatura<=Setpoint+0.5){
         printf("statusT.txt=\"Controlando\"");
         SendDataDisplay();
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
         Ganancia=GananciaR*(Setpoint/44.5);
         SSR_off;
         SSR2_off;
         SSR3_off;
         Controlando=0;
      }else if(Temperatura<Setpoint-0.6){
         flag_tiempocalentamiento=1;
         printf("statusT.txt=\"Calentando\"");
         SendDataDisplay();
         //tiempo_esterilizacion=0;
         flagTemperatura=OFF;
         Controla();
      }
   }
}

void controlaRampa(){
   if(Enfria==1){
      if(Temperatura<=Setpoint+0.5 && Temperatura>=Setpoint-0.5){
         if(Pendiente!=0){
            printf("statusT.txt=\"Controlando\"");
            SendDataDisplay();
            flagTemperatura=ON;            
         }
         
         if(Pendiente==0){
            printf("statusT.txt=\"Meseta\"");
            SendDataDisplay();
            flagTemperatura=ON;
         }
         Controla();
         flag_tiempocalentamiento=0;
      }else if(Temperatura<Setpoint-0.5){
         Enfria=0;
         Ganancia=GananciaR*(Setpoint/44.5);
         Gananciamax=(0.21)*Setpoint;
         SSR_off;
         SSR2_off;
         SSR3_off;
         Controlando=0;
      }else if(Temperatura>Setpoint+0.6){
         if(Pendiente!=0){
            printf("statusT.txt=\"Rampa -\"");
            SendDataDisplay();
            flagTemperatura=ON;            
         }
         if(Pendiente==0){
            printf("statusT.txt=\"Enfriando\"");
            SendDataDisplay();
            flagTemperatura=OFF;            
         }
         Controla();
      }
   }else{  
      if(Temperatura>=Setpoint-0.5 && Temperatura<=Setpoint+0.5){
         if(Pendiente!=0){
            printf("statusT.txt=\"Controlando\"");
            SendDataDisplay();
            flagTemperatura=ON;            
         }
         
         if(Pendiente==0){
            printf("statusT.txt=\"Meseta\"");
            SendDataDisplay();
            flagTemperatura=ON;
         }
         Controla();
         flag_tiempocalentamiento=0;
      }else if(Temperatura>Setpoint+0.5){
         Enfria=1;
         Ganancia=GananciaR*(Setpoint/44.5);
         SSR_off;
         SSR2_off;
         SSR3_off;
         Controlando=0;
      }else if(Temperatura<Setpoint-0.6){
         if(Pendiente!=0){
            printf("statusT.txt=\"Rampa +\"");
            SendDataDisplay();
            flagTemperatura=ON;            
         }
         if(Pendiente==0){
            printf("statusT.txt=\"Calentando\"");
            SendDataDisplay();
            flagTemperatura=OFF;            
         }
         Controla();
      }
   }
}
