short AC1_Read(){
   return input_state(PIN_D0);
}

short CR1_Read(){
   return input_state(PIN_B2);
}

short CR2_Read(){
   return input_state(PIN_B3);
}

short CR3_Read(){
   return input_state(PIN_B4);
}

short CR4_Read(){
   return input_state(PIN_B5);
}

short CR5_Read(){
   return input_state(PIN_B6);
}

short CR6_Read(){
   return input_state(PIN_B7);
}
short BUZZER_Read(){
   return input_state(PIN_C0);
}

short SSR_1_Read(){
   return input_state(PIN_C1);
}

void SSR_1_Write(short estado){
   if(estado){
      SSR1_on;
   }else{
      SSR1_off;
   }
}

void AC1_Write(short estado){
   if(estado){
      AC1_on;
   }else{
      AC1_off;
   }
}

void CR1_Write(short estado){
   if(estado){
      DC1_on;
   }else{
      DC1_off;
   }
}

void CR2_Write(short estado){
   if(estado){
      DC2_on;
   }else{
      DC2_off;
   }
}

void CR3_Write(short estado){
   if(estado){
      DC3_on;
   }else{
      DC3_off;
   }
}

void CR4_Write(short estado){
   if(estado){
      DC4_on;
   }else{
      DC4_off;
   }
}

void CR5_Write(short estado){
   if(estado){
      DC5_on;
   }else{
      DC5_off;
   }
}

void CR6_Write(short estado){
   if(estado){
      DC6_on;
   }else{
      DC6_off;
   }
}

void PWM_WriteCompare1(int16 data){
   if(data>100)
      data=100;
   pwm1=data;
}

void PWM_WriteCompare2(int16 data){
   if(data>100)
      data=100;
   pwm2=data;
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

void BUZZER_Write(short estado){
   output_bit(PIN_C0,estado);
}

// Funcion para conversor analogo-digital
float sensores(int x){
float y;set_adc_channel(x);delay_ms(1);y=read_adc();return (y);
}

void EEPROM_WriteByte(int8 data,int8 pos){
   write_eeprom(pos,data);
}

int8 EEPROM_ReadByte(int8 data){
   return read_eeprom(data);
}

void escribeDisplay1(char Buffer[30]){
    printf(Buffer);
    delay_us(10);
    putc(0xFF);
    delay_us(10);
    putc(0xFF);
    delay_us(10);
    putc(0xFF);
    delay_us(10);
}

int8 extraeDecimal(float data){
   float entero=0;
   float tmp=0.0;
   
   entero=(int8)data;
   tmp=(data-entero)*10.0;
   return (int8)tmp;
}

float extraeDecimal2(float data){
   float entero=0;
   float tmp=0.0;
   
   entero=(int8)data;
   tmp=(data-entero);
   return tmp;
}

float inttofloat(int8 decimal){
   float tmp;
   tmp=decimal;
   return tmp/10;
}

int8 convert8(int16 data,int8 nible){
    int8 dato=0;
    if(nible==0){
        dato=(int8)data;   
    }else if(nible==1){
        dato=data>>8;
    }
    return dato;
}

int16 convert16(int8 nibleh,int8 niblel){
    int16 temp,var;
    temp=(int16)nibleh;
    var=temp<<8 | (int16)niblel;
    return var;
}

void LeeEEPROM(){
   mEstp=EEPROM_ReadByte(0);delay_ms(5);  
   hEstp=EEPROM_ReadByte(1);delay_ms(5);  
   
   entero=EEPROM_ReadByte(2);delay_ms(5);  
   Temporal=EEPROM_ReadByte(3);delay_ms(5);  
   Temporal=Temporal/10.0;
   Setpoint=entero+Temporal;  
   
   SetpointH=EEPROM_ReadByte(4);
   delay_ms(5);   

   mediaLuces=EEPROM_ReadByte(5);
   delay_ms(5);   
   Factor=EEPROM_ReadByte(6);
   delay_ms(5);   
   tDefrost2=read_eeprom(7);
   delay_ms(5);   
   tCongela=read_eeprom(8);
   delay_ms(5);   

   tDisplay=EEPROM_ReadByte(9);delay_ms(5);  
   Password[0]=EEPROM_ReadByte(10);delay_ms(5);  
   Password[1]=EEPROM_ReadByte(11);delay_ms(5);  
   Password[2]=EEPROM_ReadByte(12);delay_ms(5);  
   Password[3]=EEPROM_ReadByte(13);delay_ms(5);  
/*
   SetpointL1=convert16(EEPROM_ReadByte(15),EEPROM_ReadByte(14));
   delay_ms(5);   
   SetpointL2=convert16(EEPROM_ReadByte(17),EEPROM_ReadByte(16));
   delay_ms(5);   
   SetpointL3=convert16(EEPROM_ReadByte(19),EEPROM_ReadByte(18));
   delay_ms(5);   
*/
   controlL=EEPROM_ReadByte(20);delay_ms(5);
   Vel1=EEPROM_ReadByte(21);delay_ms(5);
   Histeresis=EEPROM_ReadByte(22);delay_ms(5);
   Histeresis=Histeresis/10.0;
   Ganancia=EEPROM_ReadByte(30);delay_ms(5);  
   Derivativo=EEPROM_ReadByte(31);delay_ms(5);  
   Integral=EEPROM_ReadByte(32);delay_ms(5);  
   Tciclo=EEPROM_ReadByte(33);delay_ms(5);  
   Ajuste=EEPROM_ReadByte(34);delay_ms(5);  
   Ajusten=EEPROM_ReadByte(35);delay_ms(5);  
   MediaMovil=EEPROM_ReadByte(36);delay_ms(5);  
   Ajuste2=EEPROM_ReadByte(37);delay_ms(5);
   Ajusten2=EEPROM_ReadByte(38);delay_ms(5);
   MediaMovil2=EEPROM_ReadByte(39);delay_ms(5);  
   tmuestreoUSB=EEPROM_ReadByte(40);delay_ms(5);
   controlT=EEPROM_ReadByte(41);delay_ms(5);
   controlH=EEPROM_ReadByte(42);delay_ms(5);
   controlR=EEPROM_ReadByte(43);delay_ms(5);
   T1Humedad=EEPROM_ReadByte(44);delay_ms(5);
   T2Humedad=EEPROM_ReadByte(45);delay_ms(5);
   T3Humedad=EEPROM_ReadByte(46);delay_ms(5);
   d6=read_eeprom(24);
   delay_ms(10);
   d6=d6/10;
   delay_ms(10);
   alarmaTemperatura=read_eeprom(23)+d6;
   delay_ms(10);
   d6=read_eeprom(26);
   delay_ms(10);
   d6=d6/10;
   delay_ms(10);
   alarmaHumedad=read_eeprom(25)+d6;   
   delay_ms(10);
   IP[0]=EEPROM_ReadByte(47);delay_ms(5);
   IP[1]=EEPROM_ReadByte(48);delay_ms(5);
   IP[2]=EEPROM_ReadByte(49);delay_ms(5);
   IP[3]=EEPROM_ReadByte(50);delay_ms(5);
   Gateway[0]=EEPROM_ReadByte(51);delay_ms(5);
   Gateway[1]=EEPROM_ReadByte(52);delay_ms(5);
   Gateway[2]=EEPROM_ReadByte(53);delay_ms(5);
   Gateway[3]=EEPROM_ReadByte(54);delay_ms(5);
   Subnet[0]=EEPROM_ReadByte(55);delay_ms(5);
   Subnet[1]=EEPROM_ReadByte(56);delay_ms(5);
   Subnet[2]=EEPROM_ReadByte(57);delay_ms(5);
   Subnet[3]=EEPROM_ReadByte(58);delay_ms(5);
   //MAC[0]=EEPROM_ReadByte(59);delay_ms(5);
   //MAC[1]=EEPROM_ReadByte(60);delay_ms(5);
   //MAC[2]=EEPROM_ReadByte(61);delay_ms(5);
   //MAC[3]=EEPROM_ReadByte(62);delay_ms(5);
   //MAC[4]=EEPROM_ReadByte(63);delay_ms(5);
   //MAC[5]=EEPROM_ReadByte(64);delay_ms(5);
   
   /*
   for(i=0;i<3;i++){
        calib[i].patron1=EEPROM_ReadByte(64+(i*12))+inttofloat(EEPROM_ReadByte(65+(i*12)));delay_ms(5);
        calib[i].patron2=EEPROM_ReadByte(66+(i*12))+inttofloat(EEPROM_ReadByte(67+(i*12)));delay_ms(5);
        calib[i].patron3=EEPROM_ReadByte(68+(i*12))+inttofloat(EEPROM_ReadByte(69+(i*12)));delay_ms(5);
        calib[i].control1=EEPROM_ReadByte(70+(i*12))+inttofloat(EEPROM_ReadByte(71+(i*12)));delay_ms(5);
        calib[i].control2=EEPROM_ReadByte(72+(i*12))+inttofloat(EEPROM_ReadByte(73+(i*12)));delay_ms(5);
        calib[i].control3=EEPROM_ReadByte(74+(i*12))+inttofloat(EEPROM_ReadByte(75+(i*12)));delay_ms(5);
    }   
    */
}

void LimitaValores(){
   if(mEstp>59){
      mEstp=10;EEPROM_WriteByte(10,0);delay_ms(20);
   }
   if(hEstp>99){
      hEstp=10;EEPROM_WriteByte(10,1);delay_ms(20);
   }
   if(Setpoint>Tmax || Setpoint<Tmin){
      Setpoint=30.0;EEPROM_WriteByte(100,2);delay_ms(20);EEPROM_WriteByte(0,3);delay_ms(20);
   }
   if(SetpointH>Hmax || SetpointH<Hmin){
      SetpointH=60.0;EEPROM_WriteByte(60,4);delay_ms(20);
   }
   if(mediaLuces>80 || mediaLuces<10){
      mediaLuces=20;EEPROM_WriteByte(20,5);delay_ms(20);
   }
   if(Factor>200 || Factor<1){
      Factor=74;EEPROM_WriteByte(74,6);delay_ms(20);
   }
   if(tDefrost2>200 || tDefrost2<1){
     tDefrost2=200;EEPROM_WriteByte(200,7);delay_ms(20);
   }
   if(tCongela>200 || tCongela<1){
     tCongela=60;EEPROM_WriteByte(60,8);delay_ms(20);
   }
   if(tDisplay>60 || tDisplay<1){
      tDisplay=5;EEPROM_WriteByte(5,9);delay_ms(20);
   }
   if(Password[0]>9 || Password[0]<0){
      Password[0]=0;EEPROM_WriteByte(0,10);delay_ms(20);
   }
   if(Password[1]>9 || Password[1]<0){
      Password[1]=0;EEPROM_WriteByte(0,11);delay_ms(20);
   }
   if(Password[2]>9 || Password[2]<0){
      Password[2]=0;EEPROM_WriteByte(0,12);delay_ms(20);
   }
   if(Password[3]>9 || Password[3]<0){
      Password[3]=0;EEPROM_WriteByte(0,13);delay_ms(20);
   }
   
   if(controlL>1){
      controlL=0;EEPROM_WriteByte(0,20);delay_ms(20);
   }
   if(Histeresis>5.0 || Histeresis==0.0){
      Histeresis=0.2;EEPROM_WriteByte(2,22);delay_ms(20);
   }
   if(Ganancia>20 || Ganancia<1){
      Ganancia=2;EEPROM_WriteByte(2,30);delay_ms(20);
   }
   if(Derivativo>80 || Derivativo<5){
      Derivativo=80;EEPROM_WriteByte(80,31);delay_ms(20);
   }
   if(Integral>4 || Integral<1){
      Integral=1;EEPROM_WriteByte(1,32);delay_ms(20);
   }
   if(Tciclo>20 || Tciclo<5){
      Tciclo=5;EEPROM_WriteByte(5,33);delay_ms(20);
   }
   if(Ajuste>100){
      Ajuste=0;EEPROM_WriteByte(0,34);delay_ms(20);
   }
   if(Ajusten>100){
      Ajusten=0;EEPROM_WriteByte(0,35);delay_ms(20);
   }
   if(MediaMovil>80 || MediaMovil<10){
      MediaMovil=80;EEPROM_WriteByte(80,36);delay_ms(20);
   }
   if(Ajuste2>100){
      Ajuste2=0;EEPROM_WriteByte(0,37);delay_ms(20);
   }
   if(Ajusten2>100){
      Ajusten2=0;EEPROM_WriteByte(0,38);delay_ms(20);
   }
   if(MediaMovil2>80 || MediaMovil2<10){
      MediaMovil2=40;EEPROM_WriteByte(40,39);delay_ms(20);
   }
   if(tmuestreoUSB<1){
      tmuestreoUSB=20;EEPROM_WriteByte(20,40);delay_ms(20);
   }
   if(controlT>1){
      controlT=0;EEPROM_WriteByte(0,41);delay_ms(20);
   }
   if(controlH>1){
      controlH=0;EEPROM_WriteByte(0,42);delay_ms(20);
   }
   if(controlR>1){
      controlR=0;EEPROM_WriteByte(0,43);delay_ms(20);
   }
   if(T1Humedad>60 || T1Humedad<1){
      T1Humedad=10;EEPROM_WriteByte(10,44);delay_ms(20);
   }
   if(T2Humedad>60 || T2Humedad<4){
      T2Humedad=30;EEPROM_WriteByte(30,45);delay_ms(20);
   }
   if(T3Humedad>60 || T3Humedad<1){
      T3Humedad=6;EEPROM_WriteByte(6,46);delay_ms(20);
   }
   if(alarmaTemperatura>20.0 || alarmaTemperatura<0.5){
      alarmaTemperatura=1.0;write_eeprom(50,1);delay_ms(20);write_eeprom(51,0);delay_ms(20);
   }
   if(alarmaHumedad>40.0 || alarmaHumedad<5.0){
      alarmaHumedad=5.0;write_eeprom(52,5);delay_ms(20);write_eeprom(53,0);delay_ms(20);
   }
   GananciaR=(int8)Ganancia;
   derivativo_ant=Derivativo;
}

void ActualizaRecta(){
   X[y]=Temperatura;
   y++;
   if(y>=2){
      y=0;
      Aumento=X[1]-X[0];
      HorasActual=hEst;
      MinutosActual=mEst;
      TiempoActual=Tiempo-(((float)HorasActual*3600.0)+((float)MinutosActual*60.0)+tinicio);
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
    
    if(flagRampa){
      if(Segmento==0){
         if(flag_calcula==0){
            Tiempo=((float)rampas[0].Horas*3600.0)+((float)rampas[0].Minutos*60.0);         
            Setpoint=rampas[0].Temperatura;
            SetpointH=rampas[Segmento].Humedad;
            Pendiente=(((float)Setpoint-TempeInicial)/Tiempo);
            hEstp=rampas[0].Horas;
            mEstp=rampas[0].Minutos;
            flag_calcula=1;
            if(Setpoint<Temperatura){
               Enfria=1;
               Ganancia=GananciaR*(Setpoint/200);
               //Ganancia=4;
            }else{
               Enfria=0;
               Ganancia=GananciaR*(Setpoint/200);
            }
         }
      }else{
         if(flag_calcula==0){
            Tiempo=((float)rampas[Segmento].Horas*3600.0)+((float)rampas[Segmento].Minutos*60.0);         
            Setpoint=rampas[Segmento].Temperatura;
            SetpointH=rampas[Segmento].Humedad;
            Pendiente=(((float)rampas[Segmento].Temperatura-(float)rampas[Segmento-1].Temperatura)/Tiempo);
            hEstp=rampas[Segmento].Horas;
            mEstp=rampas[Segmento].Minutos;
            flag_calcula=1;
            if(Setpoint<Temperatura){
               Enfria=1;
               Ganancia=GananciaR*(Setpoint/200);
               //Ganancia=4;
            }else{
               Enfria=0;
               Ganancia=GananciaR*(Setpoint/200);
            }
         }
      }
    }
   
  
   if(Pendiente!=0 && flagRampa){// Si Esta realizando una Rampa debe realizar el control de acuerdo a la pendiente de Temperatura. Aprox 10�C * min Maximo Valor de Pendiente.
      
      TiempoRampa=(((float)hEst*3600.0)+((float)mEst*60.0)+tinicio);
      if(Segmento==0){
         TemperaturaIdeal=(Pendiente*TiempoRampa)+TempeInicial;
      }else{
         TemperaturaIdeal=(Pendiente*TiempoRampa)+rampas[Segmento-1].Temperatura;
      }
      PotenciaEntregada=((TemperaturaIdeal-Temperatura)*100)/penMufla;
// Pendiente determinar si el comportamiento de la mufla es lineal 10�C/min o si tiene un comportamiento diferente.         
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
         pwm=(int16)(((1/(float)Tciclo)*Tpulso)*60);
         PWM_WriteCompare1(0);
         PWM_WriteCompare2(pwm);
      }else{
         pwm=(int16)(((1/(float)Tciclo)*Tpulso)*60);
         PWM_WriteCompare1(pwm);
         PWM_WriteCompare2(0);
      }
   }else{
               
      if(Enfria==1){//Control Enfriamiento
         //CR1_Write(0);
         //CR2_Write(0);
         //CR3_Write(0);
         //CR4_Write(0);
         error=Setpoint-(desvio-0.2)-Temperatura;      // C�lculo del error ******
         
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
         pwm=(int16)(((1/(float)Tciclo)*Tpulso)*60);
         PWM_WriteCompare1(0);
         PWM_WriteCompare2(pwm);
         //Tp=(Tpulso/0.005);
         //Tc=(Tciclo/0.005);
         Controlando=1;       
      }else{//Control Calentamiento
         //CR1_Write(1);
         //CR2_Write(1);
         //CR3_Write(1);
         //CR4_Write(1);
         error=desvio+Setpoint-Temperatura;      // C�lculo del error ******
      
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
         }else if(error>=desvio+0.2 && error<10 && Aumento<=0.3){
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
         if(error<desvio+0.2 && error>0){
            if(error<=desvio && error>0){   
                if(flag_ajuste==0){
                    TpulsoMax=Tpulso;
                    flag_ajuste=1;  
                    Ajusto=5;
                }
            }
         
            if(error<=desvio-0.3 && error>0){   
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
         pwm=(int16)(((1/(float)Tciclo)*Tpulso)*60);
         PWM_WriteCompare2(0);
         PWM_WriteCompare1(pwm);
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
      if(!flagControlT && !flagControlH && !flagControlL){
         tiempo_esterilizacion=0;
       }else if(flagControlT && !flagControlH && !flagControlL){
          if(flagTemperatura)
             tiempo_esterilizacion=1;
          else
             tiempo_esterilizacion=0;
       }else if(!flagControlT && flagControlH && !flagControlL){
         if(flagHumedad)
            tiempo_esterilizacion=1;
         else
            tiempo_esterilizacion=0;
      }else if(!flagControlT && !flagControlH && flagControlL){
            tiempo_esterilizacion=1;
      }else if(flagControlT && !flagControlH && flagControlL){
            if(flagTemperatura)
             tiempo_esterilizacion=1;
          else
             tiempo_esterilizacion=0;
      }else if(!flagControlT && flagControlH && flagControlL){
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
         
      if(Rx_Buffer[3]==0x01){// Pregunta por la pagina en la que esta el display,01 es Contrase�a de Acceso
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
      }else if(Rx_Buffer[3]==0x09){//09 es Programas
         Menu=8;
         if(MenuAntt!=Menu)
            MenuAntt=Menu;
      }else if(Rx_Buffer[3]==0x0a){//0a es Grafica
         Menu=9;
         if(MenuAntt!=Menu)
            MenuAntt=Menu;
      }else if(Rx_Buffer[3]==0x0b){//0b es Red
         Menu=10;
         if(MenuAntt!=Menu)
            MenuAntt=Menu;
      }else if(Rx_Buffer[3]==0x0c){//0c es Calibracion Temperatura
         Menu=11;
         if(MenuAntt!=Menu)
            MenuAntt=Menu;
      }else if(Rx_Buffer[3]==0x0d){//0c es Calibracion humedad
         Menu=12;
         if(MenuAntt!=Menu)
            MenuAntt=Menu;
      }else if(Rx_Buffer[3]==0x0e){//0e es Iluminacion
         Menu=13;
         if(MenuAntt!=Menu)
            MenuAntt=Menu;
      }else if(Rx_Buffer[3]==0x0f){//0f es Recibe caracteres de contrase�a desde display
         
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
      }else if(Rx_Buffer[3]==0x1d){//1d es Menu de Velocidad
         Menu=17;
         if(MenuAntt!=Menu)
            MenuAntt=Menu;
      }else if(Rx_Buffer[3]==0x1e){//1e es Menu de Rampas
         Menu=18;
         if(MenuAntt!=Menu)
            MenuAntt=Menu;
      }else if(Rx_Buffer[3]==0x1f){//1f es Menu de Luces
         Menu=19;
         if(MenuAntt!=Menu)
            MenuAntt=Menu;
      }else if(Rx_Buffer[3]==0x20){//20 es Programa Eliminado
         Menu=21;
         if(MenuAntt!=Menu)
            MenuAntt=Menu;
      }else if(Rx_Buffer[3]==0x21){//21 es Programa XX Iniciado
         Menu=22;
         if(MenuAntt!=Menu)
            MenuAntt=Menu;
      }else if(Rx_Buffer[3]==0x22){//22 es Alarmas
         Menu=23;
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
         Ganancia=GananciaR*(Setpoint/200);
         //Gananciamax=(0.21)*Setpoint;
         SSR_1_Write(0);
         Control=0;
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
         Ganancia=GananciaR*(Setpoint/200);
         SSR_1_Write(0);
         Control=0;
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

float leePt100(int8 media){
   int16 ad1;
   float ajp=0.0,ajn=0.0,Tref;
   Pt=0.0;
   Prom=0.0;
   ad1= (int16)sensores(1);   //Lectura de Divisor de Voltaje de PT100 con resistencia de 6.18k (+-2%)
   
   if(l>(media-1))
   {l=0;}
   //promedio[l]=Temperature;l++;
   promedioT1[l]=ad1;l++;
         
   for(h=0;h<media;h++)
   {
      Prom+=(float)promedioT1[h];
   } 
   V0=Prom/media;
  
   V0 = V0/105.56; // Se elimina la ganancia
   //V0 += 14.82;
   V0 += 14.79;//5.039V 72.5mV
   Pt=6800/((1023/V0)-1);
               
   //Temperature=(Prom-100.0)/(0.385);  // Ecuaci�n determinada por linealizaci�n en Matlab. Revisar este polinomio.  
   Temperature=((Pt*Pt)*a)+(Pt*b)+c;  // Ecuaci�n determinada por linealizaci�n en Matlab. Revisar este polinomio.
      
   ajp=(float)Ajuste;
   ajp=ajp/10;
   ajn=(float)Ajusten;
   ajn=ajn/10;
   Temperature+=(ajp-ajn);
     /*  
   if(Temperature<=calib[0].control2){
       Tref=(Temperature-calib[0].b1)/calib[0].m1;
   }else{
       Tref=(Temperature-calib[0].b2)/calib[0].m2;
   }
   return Tref;
   */
   return Temperature;
}

float leePt1002(int8 media2){
    int16 ad2;
    float tmp3,ajp2,ajn2;
    float Tref2;
    Pt2=0.0;
    Prom2=0.0;
    tmp3=0;
    ad2 = (int16)sensores(2);   //Lectura de Divisor de Voltaje de PT100 con resistencia de 6.18k (+-2%)
        
    if(ll>(media2-1))
        ll=0;

    promedioT2[ll]=ad2;ll++;
             
    for(hh=0;hh<media2;hh++){
        Prom2+=(float)promedioT2[hh];
    }
    V1=Prom2/media2;
    
    V1 = V1/105.56; // Se elimina la ganancia
    //V0 += 14.82;
    V1 += 14.79;//5.039V 72.5mV
    Pt2=6800/((1023/V1)-1);
    
    TemperaturaPt1002=((Pt2*Pt2)*a)+(Pt2*b)+c;  // Ecuaci�n determinada por linealizaci�n en Matlab. Revisar este polinomio.
      
    ajp2=(float)Ajuste2;
    ajp2=ajp2/10.0;
    ajn2=(float)Ajusten2;
    ajn2=ajn2/10.0;
    TemperaturaPt1002+=(ajp2-ajn2);   
    /*
    if(TemperaturaPt1002<=calib[1].control2){
        Tref2=(TemperaturaPt1002-calib[1].b1)/calib[1].m1;
    }else{
        Tref2=(TemperaturaPt1002-calib[1].b2)/calib[1].m2;
    }
    return Tref2;
    */
    return TemperaturaPt1002;
}

float leeHumedad(int8 media){
   float VH=0.0,RH=0.0,Ta=0.0,VT=0.0;
   float PromH,PromTa;
    float Href;
   PromH=0.0;
   PromTa=0.0;
   VH=sensores(4);   //Lectura de Divisor de Voltaje de PT100 con resistencia de 6.18k (+-2%)
   VH=5.0*(VH/1023.0);
   VT=sensores(3);
   VT=5.0*(VT/1023.0);
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
    /*
    if(PromH<=calib[2].control2){
        Href=(PromH-calib[2].b1)/calib[2].m1;
    }else{
        Href=(PromH-calib[2].b2)/calib[2].m2;
    }
   return Href;
   */
   return PromH;
}

int32 LowPassFilter(int32 input){
    int32 k;
    input <<= 8;
    filt = filt + (((input-filt) >> 8) * 40);
    k = (filt>>8) + ((filt & 0x00000080) >> 7);
    return k;
}

float leeLM35(int8 media){ //Falta hacerle promedio
    int16 ad;
    float Promlm35;
    Promlm35=0.0;
    float ADC=0,V=0,T=0;
    ad=(int16)sensores(0);
    if(k>media)
       k=0;
     
    promediolm35[k]=ad;k++;    
    
    for(kk=0;kk<=(media-1);kk++){
       Promlm35+=(float)promediolm35[kk];
    }      
   
   ADC=Promlm35/media;
   V=5*ADC/1023;
   T=V/0.01;
    return T;
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

void ApagaSalidas(){
    Buzzer_off;
    DC1_off;
    DC2_off;
    DC3_off;
    DC4_off;
    DC5_off;
    DC6_off;
    AC2_off;
    AC3_off;
    SSR1_off;
}

void controlaTemperatura(){
    if(tTermostato>200 || Temperatura>Tmax || alarmaSobreT){
      printf("statusT.txt=\"!Termostato!\"");      
      SendDataDisplay();
      PWM_WriteCompare1(0);
      PWM_WriteCompare2(0);
      Control=0;
      Ganancia=GananciaR*(Setpoint/200);
      BUZZER_Write(1);
      delay_ms(300);
      BUZZER_Write(0);
      delay_ms(300);
    }else{
       if(Enfria==1){
            if(Temperatura<=Setpoint+0.5 && Temperatura>=Setpoint-0.5){
                printf("statusT.txt=\"Controlando -\"");
                SendDataDisplay();
                if(!Infinito)
                    flagTemperatura=ON;
                //tiempo_esterilizacion=1;
                else
                    flagTemperatura=OFF;
                //tiempo_esterilizacion=0;
                Controla();
                flag_tiempocalentamiento=0;
                tCalentamiento=0;
            }else if(Temperatura<Setpoint-0.5){
                Enfria=0;
                Ganancia=GananciaR*(Setpoint/400);
                //Gananciamax=(0.21)*Setpoint;
                PWM_WriteCompare1(0);
                PWM_WriteCompare2(0);
                Control=0;
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
                printf("statusT.txt=\"Controlando +\"");
                SendDataDisplay();
                if(!Infinito)
                    //tiempo_esterilizacion=1;
                    flagTemperatura=ON;
                else
                    //tiempo_esterilizacion=0;   
                    flagTemperatura=OFF;
                Controla();
                flag_tiempocalentamiento=0;
                tCalentamiento=0;
            }else if(Temperatura>Setpoint+0.5){
                if(Temperatura>Setpoint+0.7){
                    Enfria=1;
                    Ganancia=GananciaR*(Setpoint/50);
                }
                PWM_WriteCompare1(0);
                PWM_WriteCompare2(0);
                Control=0;
            }else if(Temperatura<Setpoint-0.5){
                flag_tiempocalentamiento=1;
                printf("statusT.txt=\"Calentando\"");
                SendDataDisplay();
                //tiempo_esterilizacion=0;
                flagTemperatura=OFF;
                Controla();
            }
        }
    
       if(alarmaT){            
         if(Temperatura<(Setpoint-alarmaTemperatura)){
            printf("statusT.txt=\"!Temp. Baja!\"");
            SendDataDisplay();
         }
                  
         if(Temperatura>(Setpoint+alarmaTemperatura)){
            printf("statusT.txt=\"!Temp. Alta!\"");
            SendDataDisplay();
         }
       }else{
        if(Enfria==1){
            if(Temperatura<=Setpoint+0.5 && Temperatura>=Setpoint-0.5){
                printf("statusT.txt=\"Controlando -\"");
                SendDataDisplay();
            }else if(Temperatura<Setpoint-1.5){
                printf("statusT.txt=\"Enfriando\"");
                SendDataDisplay();
            }else if(Temperatura>Setpoint+0.6){
                printf("statusT.txt=\"Enfriando\"");
                SendDataDisplay();
            }
        }else{  
            if(Temperatura>=Setpoint-0.5 && Temperatura<=Setpoint+0.5){
                printf("statusT.txt=\"Controlando +\"");
                SendDataDisplay();
            }else if(Temperatura>Setpoint+0.5){
                printf("statusT.txt=\"Controlando +\"");
                SendDataDisplay();
            }else if(Temperatura<Setpoint-1.0){
                printf("statusT.txt=\"Calentando\"");
                SendDataDisplay();
            }
        }
       }
    }
}

void actualizaEjeX(){
    for(i=0;i<14;i++){//for dedicado a actualizar el label del eje x
        horaGrafica[i]=graph[0].Hora+((int16)((float)tmuestreoUSB*anchoGrafica*(float)i)/3600);
        minutoGrafica[i]=graph[0].Minuto+(int8)(60.0*extraeDecimal2(((float)tmuestreoUSB*anchoGrafica*(float)i)/3600.0));
        if(horaGrafica[i]>=24)
            horaGrafica[i]=horaGrafica[i]-24;
        if(minutoGrafica[i]>=60){
            minutoGrafica[i]=minutoGrafica[i]-60;
            horaGrafica[i]++;
        }
        printf("t%Lu.txt=\"%02u:%02u\"",i+11,horaGrafica[i],minutoGrafica[i]); 
        SendDataDisplay();
     }   
}

void borraVector(){
    for(data2=0;data2<(320);data2++){
        graph[data2].Temperatura=0.0;           
        graph[data2].Hora=0.0;
        graph[data2].Humedad=0.0;
        graph[data2].Minuto=0.0;
    }
    data=0;
    datacount=0;
    graph[0].Minuto=Minuto;
    graph[0].Hora=Hora;
}

void dataUSB(){
    //USB----------------------------------------------------------------
                
        if(envioUSB){
            envioUSB=0;
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
            tDisplayHumedad++;
            tEnvioUSB++;
            if(tEnvioUSB>=tmuestreoUSB){
                tEnvioUSB=0;
                graficaa=ON;
                graph[data].Temperatura=Temperatura;
                graph[data].Humedad=humedadDisplay;             
                graph[data].Hora=Hora;
                graph[data].Minuto=Minuto;
                if(data<320)//Valor de cuadros en el eje x de la grafica por el ancho de cada cuadro
                    data++;
                else{
                    if(datacount>=anchoGrafica){
                        actualizaNumeracion=ON;
                        datacount=0;
                    }
                    datacount++;
                    for(data2=0;data2<(320);data2++){
                        graph[data2].Temperatura=graph[data2+1].Temperatura;                        
                        graph[data2].Hora=graph[data2+1].Hora;
                        graph[data2].Minuto=graph[data2+1].Minuto;
                        graph[data2].Humedad=graph[data2+1].Humedad;
                    }
                }
            }
        }
//USB----------------------------------------------------------------        
}    

void controlaHumedad2(){
   if(Humidificar){
         dehumidificador=OFF;
         tdehumidificador=0;
         DC3_on;
         if(Humedad<SetpointH-5.0){
            flagSobrepasoHumedad=OFF;
            tSobrepaso=0;
            humidificador=ON;
            DC4_off;//Dehumidificador_off;
               tllenado=0;
               if(Humedad<SetpointH-10.0){
                  CR5_Write(1);//Humidificador_on;
                  thumidificador=T2Humedad+1;
               }else{
                  if(thumidificador<=T1){
                     CR5_Write(1);//Humidificador_on;
                  }else if(thumidificador>T1&& thumidificador<T2Humedad){
                     CR5_Write(0);//Humidificador_off;    
                  }else if(thumidificador>T2Humedad){
                     thumidificador=0;   
                  }
               }
          
         if(Humedad<SetpointH-10.0)
               flagHumedad=OFF;
         }else if(Humedad<SetpointH+2.0 && Humedad>SetpointH-5.0){
            flagSobrepasoHumedad=OFF;
            tSobrepaso=0;
            humidificador=ON;
            thumidificacion=0;
            DC4_off;//Dehumidificador_off;
               tllenado=0;
               //CR7_Write(0);//Humidificador_on;
               if(thumidificador<=T3){
                  CR5_Write(1);//Humidificador_on;
               }else if(thumidificador>T3&& thumidificador<T2Humedad){
                  CR5_Write(0);//Humidificador_off;    
               }else if(thumidificador>T2Humedad){
                  thumidificador=0;   
               }
            if(!Infinito)   
               flagHumedad=ON;
            else
               flagHumedad=OFF;
         }else if(Humedad>SetpointH+2.0 && Humedad<=SetpointH+3.0){
            thumidificacion=0;
            flagSobrepasoHumedad=OFF;
            tSobrepaso=0;
            CR5_Write(0);//Humidificador_off;
            CR4_Write(0);//Dehumidificador_off;
            humidificador=OFF;
            thumidificador=T2Humedad+1;
            if(!Infinito)   
               flagHumedad=ON;
            else
               flagHumedad=OFF;
         }else if(Humedad>SetpointH+3.0){
            flagSobrepasoHumedad=ON;
            CR5_Write(0);//Humidificador_off;
            if(tSobrepaso>300)
               Humidificar=OFF; 
         }
      }else{     
         DC3_off;
         humidificador=OFF;
         thumidificador=0;
         if(Humedad>SetpointH+2.0){
            dehumidificador=ON;
            if(tdehumidificador<(tCongela)){
               CR4_Write(1);//Dehumidificador_on;
            }else if(tdehumidificador>=(tCongela) && tdehumidificador<((tDefrost2)+(tCongela))){
               CR4_Write(0);//Dehumidificador_off;
            }else if(tdehumidificador>=((tDefrost2)+(tCongela))){
               tdehumidificador=0;
            }
            CR5_Write(0);//Humidificador_off;
            if(Humedad>SetpointH+10.0)
               flagHumedad=OFF;
         }else if(Humedad>SetpointH && Humedad<SetpointH+2.8){
            thumidificacion=0;
            if(tdehumidificador<(tCongela)){
               CR4_Write(1);//Dehumidificador_on;
            }else if(tdehumidificador>=(tCongela) && tdehumidificador<((tCongela)+(tDefrost2))){
               CR4_Write(0);//Dehumidificador_off;
            }else if(tdehumidificador>=((tCongela)+(tDefrost2))){
               tdehumidificador=0;
            }
            CR5_Write(0);//Humidificador_off;
            if(!Infinito)   
               flagHumedad=ON;
            else
               flagHumedad=OFF;
         }else if(Humedad<SetpointH-0.1 && Humedad>SetpointH-2.0){
            dehumidificador=OFF;
            tdehumidificador=0;
            CR5_Write(0);//Humidificador_off;
            CR4_Write(0);//Dehumidificador_off;
            if(!Infinito)   
               flagHumedad=ON;
            else
               flagHumedad=OFF;
         }else if(Humedad<SetpointH-3.0){
            Humidificar=ON; 
         }
      }
   
   
   if(alarmaH){
      if(Humedad<(SetpointH-alarmaHumedad)){
         printf("statusH.txt=\"!Hum. Baja!\"");
         SendDataDisplay();
      }
                     
      if(Humedad>(SetpointH+alarmaHumedad)){
         printf("statusH.txt=\"!Hum. Alta!\"");
         SendDataDisplay();
      }
   }else{
      if(Humidificar){
         if(Humedad<SetpointH-5.0){
            printf("statusH.txt=\"Humidificando\"");
            SendDataDisplay();
         }else if(Humedad<SetpointH+2.0 && Humedad>SetpointH-5.0){
            printf("statusH.txt=\"Controlando\"");
            SendDataDisplay();
         }else if(Humedad>SetpointH+2.0 && Humedad<=SetpointH+3.0){
            printf("statusH.txt=\"Controlando\"");
            SendDataDisplay();
         }      
      }else{        
         if(Humedad>SetpointH+2.0){
            printf("statusH.txt=\"Deshumidificando\"");
            SendDataDisplay();
         }else if(Humedad>SetpointH && Humedad<SetpointH+2.8){
            printf("statusH.txt=\"Controlando\"");
            SendDataDisplay();
         }else if(Humedad<SetpointH-0.1 && Humedad>SetpointH-2.0){
            printf("statusH.txt=\"Controlando\"");
            SendDataDisplay();
         }
      }
   }
}

void Envio_I2C(direccion, posicion, dato){
   i2c_start();            // Comienzo comunicaci�n
   i2c_write(direccion);   // Direcci�n del esclavo en el bus I2C
   i2c_write(posicion);    // Posici�n donde se guardara el dato transmitido
   i2c_write(dato);        // Dato a transmitir
   i2c_stop();             // Fin comunicaci�n
 }

void Lectura_I2C (byte direccion, byte posicion, byte &dato) {
   i2c_start();            // Comienzo de la comunicaci�n
   i2c_write(direccion);   // Direcci�n del esclavo en el bus I2C
   i2c_write(posicion);    // Posici�n de donde se leer� el dato en el esclavo
   i2c_start();            // Reinicio
   i2c_write(direccion+1); // Direcci�n del esclavo en modo lectura
   dato=i2c_read(0);       // Lectura del dato
   i2c_stop();             // Fin comunicaci�n
}

void Carga_Vector(void){
   SlaveB0Tx[0]=(int8)Temperatura;
   SlaveB0Tx[1]=extraeDecimal(Temperatura);
   SlaveB0Tx[2]=(int8)Temperatura2;
   SlaveB0Tx[3]=extraeDecimal(Temperatura2);
   SlaveB0Tx[4]=(int8)Humedad;
   SlaveB0Tx[5]=extraeDecimal(Humedad);
   SlaveB0Tx[6]=(int8)Setpoint;
   SlaveB0Tx[7]=255;
   SlaveB0Tx[8]=255;
   SlaveB0Tx[9]=128;
   SlaveB0Tx[10]=IP[0];
   SlaveB0Tx[11]=IP[1];
   SlaveB0Tx[12]=IP[2];
   SlaveB0Tx[13]=IP[3];
   SlaveB0Tx[14]=Subnet[0];
   SlaveB0Tx[15]=Subnet[1];
   SlaveB0Tx[16]=Subnet[2];
   SlaveB0Tx[17]=Subnet[3];
   SlaveB0Tx[18]=MAC[0];
   SlaveB0Tx[19]=MAC[1];
   SlaveB0Tx[20]=MAC[2];
   SlaveB0Tx[21]=MAC[3];
   SlaveB0Tx[22]=MAC[4];
   SlaveB0Tx[23]=MAC[5];
   SlaveB0Tx[24]=ajustaIP;
}

void Lee_Vector(void){
   alarma=SlaveB0Rx[0];
   nivelBajo=SlaveB0Rx[1];
   nivelAlto=SlaveB0Rx[2];
   bomba=SlaveB0Rx[3];
}

void Envio_Esclavos(void){
   Carga_Vector();
   for(i=0;i<25;i++){      
         direccion=0xB0;
         Envio_I2C(direccion,i,SlaveB0Tx[i]);
   } 
}
void Lectura_Esclavos(void){
   //if(txbuf[0]==10 && txbuf[49]==128){
      for(i=0;i<5;i++){
         direccion=0xB0;                        //Direcci�n en el bus I2c
         posicion=i;                         //Posici�n de memoria a leer
         Lectura_I2C(direccion, posicion, dato);    //Lectura por I2C
         SlaveB0Rx[i]=(int8)dato;
      }
      Lee_Vector();   
   //}
}


void controlaHumedad3(){
   errorH=SetpointH-Humedad;      // C�lculo del error ******
   if(errorH<5.0 && errorH>-5.0)
   {}
   else{
      if(abs(errorH)>20){
         tmp3+=ceil(errorH/10); 
      }else{
         if(errorH>0){
            if(errorH>10){
              tmp3+=3;
            }else{
               if(errorH>5)                     
                  tmp3+=2;
               else
                  tmp3+=1;
            }
         }
                        
         if(errorH<0){
            if(errorH<-30){
               tmp3-=3;
            }else{
               if(errorH<-15)                     
                  tmp3-=2;
               else
                  tmp3-=1;
            }
         }
      }
   }
  
   if(tmp3>SetpointH)   // Anti Wind-UP      
      tmp3=SetpointH;
                     
   if(tmp3<0.0)     // Anti Wind_DOWN    
      tmp3=0.0;
      
   TpulsoH=(tmp3/SetpointH)*TcicloH; 
   
   TpH=(TpulsoH/0.005);
   TcH=(TcicloH/0.005);
   controlaH=1;
      
      if(alarmaH){
      if(Humedad<(SetpointH-alarmaHumedad)){
         printf("statusH.txt=\"!Hum. Baja!\"");
         SendDataDisplay();
      }
                     
      if(Humedad>(SetpointH+alarmaHumedad)){
         printf("statusH.txt=\"!Hum. Alta!\"");
         SendDataDisplay();
      }
   }else{
      if(Humidificar){
         if(Humedad<SetpointH-5.0){
            printf("statusH.txt=\"Humidificando\"");
            SendDataDisplay();
         }else if(Humedad<SetpointH+2.0 && Humedad>SetpointH-5.0){
            printf("statusH.txt=\"Controlando\"");
            SendDataDisplay();
         }else if(Humedad>SetpointH+2.0 && Humedad<=SetpointH+3.0){
            printf("statusH.txt=\"Controlando\"");
            SendDataDisplay();
         }      
      }else{        
         if(Humedad>SetpointH+2.0){
            printf("statusH.txt=\"Deshumidificando\"");
            SendDataDisplay();
         }else if(Humedad>SetpointH && Humedad<SetpointH+2.8){
            printf("statusH.txt=\"Controlando\"");
            SendDataDisplay();
         }else if(Humedad<SetpointH-0.1 && Humedad>SetpointH-2.0){
            printf("statusH.txt=\"Controlando\"");
            SendDataDisplay();
         }
      }
   }   
}
