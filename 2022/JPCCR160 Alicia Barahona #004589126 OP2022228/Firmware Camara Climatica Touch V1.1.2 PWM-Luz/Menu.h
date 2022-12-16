void leeMenus(){
    
//_--------------------------------------------------------------------------------------------------------------------------------------------------------
    if(Menu==2){ //Menu de Tiempo de Duraci�n
        dibujo=OFF;
        imprime=OFF;
        printf("fecha.txt=\"Fecha:%02u/%02u/20%02u    Hora: %02u:%02u:%02u\"",Dia,Mes,Year,Hora,Minuto,Segundo);
        SendDataDisplay();
        
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
               delay_ms(200);
               if(Rx_Buffer[4]==0x11){
                  Minutos=OFF;
                  Horas=OFF;
                  delay_ms(10);
                  printf("page Principal");
                  SendDataDisplay();
                  if(!GuardaEEPROM){
                     EEPROM_WriteByte(mEstp,0);
                     delay_ms(10);
                     EEPROM_WriteByte(hEstp,1);
                     delay_ms(10);
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
                  delay_ms(200);
               }
               if(Horas){
                  hEstp++;
                  delay_ms(200);
               }  
            }
            
            if(pulsoDisminuir){//Tecla abajo oprimida
               if(Minutos && mEstp>0){
                  mEstp--;
                  delay_ms(200);
               }
               if(Horas  && hEstp>0){
                  hEstp--;
                  delay_ms(200);
               }
            }
            
            if(hEstp>99)hEstp=1;
            if(mEstp>59)mEstp=0;
               
            printf("tminsec.txt=\"%02u\"",hEstp);
            SendDataDisplay();
            printf("tsecsec.txt=\"%02u\"",mEstp);
            SendDataDisplay();
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
               delay_ms(200);
               if(Rx_Buffer[4]==0x11){
                  Minutos=OFF;
                  Horas=OFF;
                  delay_ms(10);
                  printf("page Rampas");
                  SendDataDisplay();
                  if(!GuardaEEPROM){
                     write_ext_eeprom(3+((Programa-1)*264)+((CicloLibre-1)*11),rampas[CicloLibre-1].Horas);
                     delay_ms(5);
                     write_ext_eeprom(4+((Programa-1)*264)+((CicloLibre-1)*11),rampas[CicloLibre-1].Minutos);
                     delay_ms(5);
                     GuardaEEPROM=ON;
                  }
                  pulsoAumentar=OFF;
                  pulsoDisminuir=OFF;
               }
            }
                      
            if(pulsoAumentar){//Tecla arriba Oprimida
               if(Minutos){
                  rampas[CicloLibre-1].Minutos+=1.0;
                  delay_ms(200);
               }
               if(Horas){
                  rampas[CicloLibre-1].Horas+=1.0;
                  delay_ms(200);
               }  
            }
            
            if(pulsoDisminuir){//Tecla abajo oprimida
               if(Minutos && rampas[CicloLibre-1].Minutos>0){
                  rampas[CicloLibre-1].Minutos-=1.0;
                  delay_ms(200);
               }
               if(Horas  && rampas[CicloLibre-1].Horas>0){
                  rampas[CicloLibre-1].Horas-=1.0;
                  delay_ms(200);
               }
            }
            
            if(rampas[CicloLibre-1].Horas>99)rampas[CicloLibre-1].Horas=1;
            if(rampas[CicloLibre-1].Minutos>59)rampas[CicloLibre-1].Minutos=0;
               
            printf("tminsec.txt=\"%02u\"",(int8)rampas[CicloLibre-1].Horas);
            SendDataDisplay();
            printf("tsecsec.txt=\"%02u\"",(int8)rampas[CicloLibre-1].Minutos);
            SendDataDisplay();
         }
    }//Menu de Tiempo de Duraci�n
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
            printf("c0.val=1");
            SendDataDisplay();
            printf("c1.val=0");
            SendDataDisplay();
            printf("c2.val=0");
            SendDataDisplay();
            Rx_Buffer[4]=0x00;  
            Rx_Buffer2[4]=0x00;     
        }else if(Rx_Buffer[4]==0x21){//Selecciono x10
            x1=OFF;
            x10=ON;
            x100=OFF;
            printf("c0.val=0");
            SendDataDisplay();
            printf("c1.val=1");
            SendDataDisplay();
            printf("c2.val=0");
            SendDataDisplay();
            Rx_Buffer[4]=0x00;  
            Rx_Buffer2[4]=0x00;     
        }else if(Rx_Buffer[4]==0x22){//Selecciono x100
            x1=OFF;
            x10=OFF;
            x100=ON;
            printf("c0.val=0");
            SendDataDisplay();
            printf("c1.val=0");
            SendDataDisplay();
            printf("c2.val=1");
            SendDataDisplay();
            Rx_Buffer[4]=0x00;  
            Rx_Buffer2[4]=0x00;     
        }else{//if(Rx_Buffer[4]!=0x00){
           pulsoAumentar=OFF;
           pulsoDisminuir=OFF; 
        }
        
      printf("fecha.txt=\"Fecha:%02u/%02u/20%02u    Hora: %02u:%02u:%02u\"",Dia,Mes,Year,Hora,Minuto,Segundo);
      SendDataDisplay();
      
      if(!rampa){
         printf("tsettem.txt=\"%2.1f\"",Setpoint);
         SendDataDisplay();
         
         if(Rx_Buffer[4]==0x11){//Selecciono Regresar
            Horas=OFF;
            Minutos=OFF;
            delay_ms(10);
            printf("page Principal");  
            SendDataDisplay();
            if(!GuardaEEPROM){
               if(Setpoint<0.0){
                  EEPROM_WriteByte(0,2);
                  EEPROM_WriteByte(0,3);
               }else{
                  EEPROM_WriteByte((int8)Setpoint,2);
                  EEPROM_WriteByte(extraeDecimal(Setpoint),3);
               }
               delay_ms(10);
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
         
        if(Setpoint<Tmin)
            Setpoint=Tmax;
         if(Setpoint>Tmax)
            Setpoint=Tmin;
      }else{
         printf("tsettem.txt=\"%2.1f\"",rampas[CicloLibre-1].Temperatura);
         SendDataDisplay();
                    
         if(Rx_Buffer[4]==0x11){//Selecciono Regresar
            conteo=0;
            Horas=OFF;
            Minutos=OFF;
            delay_ms(10);
            printf("page Rampas");
            SendDataDisplay();
            if(!GuardaEEPROM){
               if(rampas[CicloLibre-1].Temperatura<0.0){
                  write_ext_eeprom(((Programa-1)*264)+((CicloLibre-1)*11),0);
                  delay_ms(5);
                  write_ext_eeprom(1+((Programa-1)*264)+((CicloLibre-1)*11),0);
                  delay_ms(5);
               }else{
                  rampas[CicloLibre-1].TemperaturaInt=(int8)rampas[CicloLibre-1].Temperatura;
                  rampas[CicloLibre-1].TemperaturaDec=extraeDecimal(rampas[CicloLibre-1].Temperatura);
                  write_ext_eeprom(((Programa-1)*264)+((CicloLibre-1)*11),rampas[CicloLibre-1].TemperaturaInt);
                  delay_ms(5);
                  write_ext_eeprom(1+((Programa-1)*264)+((CicloLibre-1)*11),rampas[CicloLibre-1].TemperaturaDec);
                  delay_ms(5);
               }
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
        
        if(rampas[CicloLibre-1].Temperatura<Tmin)
            rampas[CicloLibre-1].Temperatura=Tmax;
         if(rampas[CicloLibre-1].Temperatura>Tmax)
            rampas[CicloLibre-1].Temperatura=Tmin;
      }
         
    }//Menu de Temperatura
//_--------------------------------------------------------------------------------------------------------------------------------------------------------       

//_--------------------------------------------------------------------------------------------------------------------------------------------------------
    if(Menu==4){ //Menu de Humedad
      printf("fecha.txt=\"Fecha:%02u/%02u/20%02u    Hora: %02u:%02u:%02u\"",Dia,Mes,Year,Hora,Minuto,Segundo);
      SendDataDisplay();
      
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
            printf("c0.val=1");
            SendDataDisplay();
            printf("c1.val=0");
            SendDataDisplay();
            printf("c2.val=0");
            SendDataDisplay();
            Rx_Buffer[4]=0x00;  
            Rx_Buffer2[4]=0x00;     
        }else if(Rx_Buffer[4]==0x21){//Selecciono x10
            x1=OFF;
            x10=ON;
            x100=OFF;
            printf("c0.val=0");
            SendDataDisplay();
            printf("c1.val=1");
            SendDataDisplay();
            printf("c2.val=0");
            SendDataDisplay();
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
            printf("page Principal");
            SendDataDisplay();
            if(!GuardaEEPROM){
               EEPROM_WriteByte(SetpointH,4);
               delay_ms(10);
               GuardaEEPROM=ON;
            }
            alarmaH=OFF;
            tHumidificacion=0;
            T1=(int8)(((float)T1Humedad)*((float)SetpointH/80.0));
            T3=(int8)(((float)T3Humedad)*((float)SetpointH/80.0));
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
        
        if(SetpointH<Hmin)
            SetpointH=Hmax;
         if(SetpointH>Hmax)
            SetpointH=Hmin;
        
         printf("tsettem.txt=\"%02u\"",SetpointH);
         SendDataDisplay();
      }else{
         
         if(Rx_Buffer[4]==0x11){//Selecciono Regresar
            Horas=OFF;
            Minutos=OFF;
            printf("page Rampas");
            SendDataDisplay();
            if(!GuardaEEPROM){
               write_ext_eeprom(2+((Programa-1)*264)+((CicloLibre-1)*11),rampas[CicloLibre-1].Humedad);
               delay_ms(5);
               delay_ms(10);
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
        
        if(rampas[CicloLibre-1].Humedad<Hmin)
            rampas[CicloLibre-1].Humedad=Hmax;
         if(rampas[CicloLibre-1].Humedad>Hmax)
            rampas[CicloLibre-1].Humedad=Hmin;
            
         printf("tsettem.txt=\"%02u\"",rampas[CicloLibre-1].Humedad);
         SendDataDisplay();
      }
    }//Menu de Humedad
//_--------------------------------------------------------------------------------------------------------------------------------------------------------       

//_--------------------------------------------------------------------------------------------------------------------------------------------------------
    if(Menu==6){//Menu de Test
        Test=ON;
        Control=0;
        inicioControlL=0;
        
        if(Rx_Buffer[4]==0x08){
            SSR_1_Write(!SSR_1_Read());
            Rx_Buffer[4]=0x00;
            Rx_Buffer2[4]=0x00;   
        }
        
        if(Rx_Buffer[4]==0x06){
            //SSR_2_Write(!SSR_2_Read());
            Rx_Buffer[4]=0x00;
            Rx_Buffer2[4]=0x00;   
        }
        
        if(Rx_Buffer[4]==0x13){
            BUZZER_Write(!BUZZER_Read());
            Rx_Buffer[4]=0x00;
            Rx_Buffer2[4]=0x00;   
        }
        
        if(Rx_Buffer[4]==0x01){
            CR6_Write(!CR6_Read());
            Rx_Buffer[4]=0x00;
            Rx_Buffer2[4]=0x00;   
        }
        
        if(Rx_Buffer[4]==0x02){
            CR2_Write(!CR2_Read());
            Rx_Buffer[4]=0x00;
            Rx_Buffer2[4]=0x00;   
        }
        
        if(Rx_Buffer[4]==0x03){
            CR1_Write(!CR1_Read());
            Rx_Buffer[4]=0x00;
            Rx_Buffer2[4]=0x00;   
        }
        
        if(Rx_Buffer[4]==0x12){
            //BOMBA_Write(!BOMBA_Read());
            Rx_Buffer[4]=0x00;
            Rx_Buffer2[4]=0x00;   
        }
        
        if(Rx_Buffer[4]==0x07){
            estadoVentilador=!estadoVentilador;
            Rx_Buffer[4]=0x00;
            Rx_Buffer2[4]=0x00;   
        }
        
        if(Rx_Buffer[4]==0x04){
            AC1_Write(AC1_Read());
            Rx_Buffer[4]=0x00;
            Rx_Buffer2[4]=0x00;   
        }
        
        if(Rx_Buffer[4]==0x05){
            //VAL2_Write(VAL2_Read());
            Rx_Buffer[4]=0x00;
            Rx_Buffer2[4]=0x00;   
        }
        
        /*
        if(Rx_Buffer[4]==0x1f){
            Setpoint1=(float)Rx_Buffer[5];
            Setpoint1=(Setpoint1/100.0)*1023.0;
            pwm1=1023-(int16)Setpoint1;
            Rx_Buffer[4]=0x00;
            Rx_Buffer2[4]=0x00;   
        }        
        
        if(Rx_Buffer[4]==0x2f){
            Setpoint2=(float)Rx_Buffer[5];
            Setpoint2=(Setpoint2/100.0)*1023.0;
            pwm2=1023-(int16)Setpoint2;
            Rx_Buffer[4]=0x00;
            Rx_Buffer2[4]=0x00;   
        }        
        
        if(Rx_Buffer[4]==0x3f){
            Setpoint3=(float)Rx_Buffer[5];
            Setpoint3=(Setpoint3/100.0)*1023.0;
            pwm3=1023-(int16)Setpoint3;
            Rx_Buffer[4]=0x00;
            Rx_Buffer2[4]=0x00;   
        } 
        
        
        if(SSR_1_Read()==1){
            printf("b7.bco=1024");
            SendDataDisplay();
        }else{
            printf("b7.bco=63488");
            SendDataDisplay();
        }
        
        if(SSR_2_Read()==1){
            printf("b22.bco=1024");
            SendDataDisplay();
        }else{
            printf("b22.bco=63488");
            SendDataDisplay();
        }
        */
        if(BUZZER_Read()==1){
            printf("b12.bco=1024");
            SendDataDisplay();
        }else{
            printf("b12.bco=63488");
            SendDataDisplay();
        }
        
        if(CR6_Read()==1){
            printf("b0.bco=1024");
            SendDataDisplay();
        }else{
            printf("b0.bco=63488");
            SendDataDisplay();
        }
        
        if(CR2_Read()==1){
            printf("b1.bco=1024");
            SendDataDisplay();
        }else{            
            printf("b1.bco=63488");
            SendDataDisplay();
        }
        
        if(CR1_Read()==1){            
            printf("b2.bco=1024");
            SendDataDisplay();
        }else{            
            printf("b2.bco=63488");
            SendDataDisplay();
        }
        
        if(bomba==5){            
            printf("b11.bco=1024");
            SendDataDisplay();
        }else{            
            printf("b11.bco=63488");
            SendDataDisplay();
        }
        
        if(AC1_Read()==0){            
            printf("b3.bco=1024");
            SendDataDisplay();
        }else{            
            printf("b3.bco=63488");
            SendDataDisplay();
        }
        /*
        if(VAL2_Read()==0){            
            printf("b4.bco=1024");
            SendDataDisplay();
        }else{            
            printf("b4.bco=63488");
            SendDataDisplay();
        }
        */
        if(estadoVentilador){
            printf("b6.bco=1024");
            SendDataDisplay();
            DC3_on;
        }else{            
            printf("b6.bco=63488");
            SendDataDisplay();
            DC3_off;
        }
        
        if(DOOR){
            printf("b8.bco=1024");
            SendDataDisplay();
        }else{
            printf("b8.bco=63488");
            SendDataDisplay();
        }
       
        if(TERM){
            printf("b9.bco=1024");
            SendDataDisplay();
        }else{
            printf("b9.bco=63488");    
            SendDataDisplay();
        }

        if(nivelBajo==5){
           printf("b17.bco=1024");
           SendDataDisplay();
        }else{
           printf("b17.bco=63488");    
           SendDataDisplay();
        }
        
        if(nivelAlto==5){
           printf("b21.bco=1024");
           SendDataDisplay();
        }else{
           printf("b21.bco=63488");    
           SendDataDisplay();
        }
      
      printf("b17.txt=\"Nivel Bajo\"");
      SendDataDisplay(); 
      printf("b21.txt=\"Nivel Alto\"");
      SendDataDisplay(); 
      printf("b16.txt=\"Al:%02u\"",alarma);
      SendDataDisplay();
      printf("b13.txt=\"ADC1:%03.0f %1.1f\"",sensores(1),Pt);
      SendDataDisplay();
      printf("b14.txt=\"ADC2:%03.0f %1.1f\"",sensores(2),V1);
      SendDataDisplay();
      printf("b15.txt=\"T1:%1.2f \"",Temperatura);
      SendDataDisplay();
      printf("b5.txt=\"T35:%ld\"",Temperature2);
      SendDataDisplay();
      printf("b18.txt=\"T2:%1.2f\"",Temperatura2);
      SendDataDisplay();
      printf("b20.txt=\"Thum:%1.2f\"",Tam);
      SendDataDisplay();
      printf("b19.txt=\"Hum:%1.2f%%\"",Humedad);
      SendDataDisplay();
      
      if(Rx_Buffer[4]==0x21){
            printf("page Ajustes");
            SendDataDisplay();
            Test=OFF;
            pwm1=1023;
            pwm2=1023;
            pwm3=1023;
            Rx_Buffer[4]=0x00;
            Rx_Buffer2[4]=0x00;   
        }
      
   }//Menu de Test
//_--------------------------------------------------------------------------------------------------------------------------------------------------------          
   
//_--------------------------------------------------------------------------------------------------------------------------------------------------------      
    if(Menu==7){ //Menu de Configuraci�n de Fecha y Hora
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
            printf("page Ajustes");
            SendDataDisplay();
            
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
               Rx_Buffer[4]=0x00;
               Rx_Buffer2[4]=0x00;
         }
      LimitaDia(); 
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
    }//Menu de Configuraci�n de Fecha y Hora
//_--------------------------------------------------------------------------------------------------------------------------------------------------------       

//_--------------------------------------------------------------------------------------------------------------------------------------------------------      
    if(Menu==9){//Menu de Grafica
        if(Rx_Buffer[4]==0x11){
            printf("page Principal");
            SendDataDisplay();
            Rx_Buffer[4]=0x00;
            Rx_Buffer2[4]=0x00; 
            entroGrafica=OFF;
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
                printf("add 1,0,%u",graph[data2].Temperatura*2); 
                SendDataDisplay();
                printf("add 1,1,%u",graph[data2].Humedad*2); 
                SendDataDisplay();
            }   
            entroGrafica=OFF;
        }
        
        if(flagControlT && !flagControlH && !flagRampa){
            printf("t0.txt=\"Control de Temperatura Activado\"");
            SendDataDisplay();
        }else if(!flagControlT && flagControlH && !flagRampa){
            printf("t0.txt=\"Control de Humedad Activado\"");
            SendDataDisplay();
        }else if(flagControlT && flagControlH && !flagRampa){
            printf("t0.txt=\"Control de Temperatura y Humedad Activado\"");
            SendDataDisplay();
        }else if(flagRampa){
            printf("t0.txt=\"Control por Rampas Activado\"");
            SendDataDisplay();
        }else{
            printf("t0.txt=\"Controles Desactivados\"");
            SendDataDisplay();
        }
        
        printf("temp.txt=\"%1.1f�C\"",Temperatura);
        SendDataDisplay();
        printf("humed.txt=\"%1.1f%%\"",Humedad);
        SendDataDisplay();
        
        
        if(graficaa){
            graficaa=OFF;
            printf("add 1,0,%1.0f",Temperatura*2); 
            SendDataDisplay();
            //sprintf(displayStr,"add 1,1,%1.0f",Humedad*2); 
            printf("add 1,1,%1.0f",humedadDisplay*2); 
            SendDataDisplay();
        }
         
    }//Menu de Grafica
//_--------------------------------------------------------------------------------------------------------------------------------------------------------          

//_--------------------------------------------------------------------------------------------------------------------------------------------------------      
    if(Menu==10){//Menu de RED
        if(!flagRed){
            printf("ip1.txt=\"%u\"",IP[0]); 
            SendDataDisplay();
            printf("ip2.txt=\"%u\"",IP[1]); 
            SendDataDisplay();
            printf("ip3.txt=\"%u\"",IP[2]); 
            SendDataDisplay();
            printf("ip4.txt=\"%u\"",IP[3]); 
            SendDataDisplay();
        
            printf("g1.txt=\"%u\"",Gateway[0]); 
            SendDataDisplay();
            printf("g2.txt=\"%u\"",Gateway[1]); 
            SendDataDisplay();
            printf("g3.txt=\"%u\"",Gateway[2]); 
            SendDataDisplay();
            printf("g4.txt=\"%u\"",Gateway[3]); 
            SendDataDisplay();
        
            printf("s1.txt=\"%u\"",Subnet[0]); 
            SendDataDisplay();
            printf("s2.txt=\"%u\"",Subnet[1]); 
            SendDataDisplay();
            printf("s3.txt=\"%u\"",Subnet[2]); 
            SendDataDisplay();
            printf("s4.txt=\"%u\"",Subnet[3]); 
            SendDataDisplay();
            
            printf("m1.txt=\"%u\"",MAC[0]); 
            SendDataDisplay();
            printf("m2.txt=\"%u\"",MAC[1]); 
            SendDataDisplay();
            printf("m3.txt=\"%u\"",MAC[2]); 
            SendDataDisplay();
            printf("m4.txt=\"%u\"",MAC[3]); 
            SendDataDisplay();
            printf("m5.txt=\"%u\"",MAC[4]); 
            SendDataDisplay();
            printf("m6.txt=\"%u\"",MAC[5]); 
            SendDataDisplay();
            
        }
        
        if(Rx_Buffer[4]==0x00){
            if(ip1){
                IP[0]=Rx_Buffer[5];
            }else if(ip2){
                IP[1]=Rx_Buffer[5];
            }else if(ip3){
                IP[2]=Rx_Buffer[5];
            }else if(ip4){
                IP[3]=Rx_Buffer[5];
            }else if(g1){
                Gateway[0]=Rx_Buffer[5];
            }else if(g2){
                Gateway[1]=Rx_Buffer[5];
            }else if(g3){
                Gateway[2]=Rx_Buffer[5];
            }else if(g4){
                Gateway[3]=Rx_Buffer[5];
            }else if(s1){
                Subnet[0]=Rx_Buffer[5];
            }else if(s2){
                Subnet[1]=Rx_Buffer[5];
            }else if(s3){
                Subnet[2]=Rx_Buffer[5];
            }else if(s4){
                Subnet[3]=Rx_Buffer[5];
            }else if(m1){
                MAC[0]=Rx_Buffer[5];
            }else if(m2){
                MAC[1]=Rx_Buffer[5];
            }else if(m3){
                MAC[2]=Rx_Buffer[5];
            }else if(m4){
                MAC[3]=Rx_Buffer[5];
            }else if(m5){
                MAC[4]=Rx_Buffer[5];
            }else if(m6){
                MAC[5]=Rx_Buffer[5];
            }
            flagRed=0;
        }else if(Rx_Buffer[4]==0x01){//Oprimio IP1
            ip1=1;ip2=0;ip3=0;ip4=0;g1=0;g2=0;g3=0;g4=0;s1=0;s2=0;s3=0;s4=0;m1=0;m2=0;m3=0;m4=0;m5=0;m6=0;
            Rx_Buffer[4]=0x00;Rx_Buffer2[4]=0x00;flagRed=1;   
        }else if(Rx_Buffer[4]==0x02){//Oprimio IP2
            ip1=0;ip2=1;ip3=0;ip4=0;g1=0;g2=0;g3=0;g4=0;s1=0;s2=0;s3=0;s4=0;m1=0;m2=0;m3=0;m4=0;m5=0;m6=0;
            Rx_Buffer[4]=0x00;Rx_Buffer2[4]=0x00;flagRed=1;   
        }else if(Rx_Buffer[4]==0x03){//Oprimio IP3
            ip1=0;ip2=0;ip3=1;ip4=0;g1=0;g2=0;g3=0;g4=0;s1=0;s2=0;s3=0;s4=0;m1=0;m2=0;m3=0;m4=0;m5=0;m6=0;
            Rx_Buffer[4]=0x00;Rx_Buffer2[4]=0x00;flagRed=1;   
        }else if(Rx_Buffer[4]==0x04){//Oprimio IP4
            ip1=0;ip2=0;ip3=0;ip4=1;g1=0;g2=0;g3=0;g4=0;s1=0;s2=0;s3=0;s4=0;m1=0;m2=0;m3=0;m4=0;m5=0;m6=0;
            Rx_Buffer[4]=0x00;Rx_Buffer2[4]=0x00;flagRed=1;   
        }else if(Rx_Buffer[4]==0x05){//Oprimio Gateway1
            ip1=0;ip2=0;ip3=0;ip4=0;g1=1;g2=0;g3=0;g4=0;s1=0;s2=0;s3=0;s4=0;m1=0;m2=0;m3=0;m4=0;m5=0;m6=0;
            Rx_Buffer[4]=0x00;Rx_Buffer2[4]=0x00;flagRed=1;   
        }else if(Rx_Buffer[4]==0x06){//Oprimio Gateway2
            ip1=0;ip2=0;ip3=0;ip4=0;g1=0;g2=1;g3=0;g4=0;s1=0;s2=0;s3=0;s4=0;m1=0;m2=0;m3=0;m4=0;m5=0;m6=0;
            Rx_Buffer[4]=0x00;Rx_Buffer2[4]=0x00;flagRed=1;   
        }else if(Rx_Buffer[4]==0x07){//Oprimio Gateway3
            ip1=0;ip2=0;ip3=0;ip4=0;g1=0;g2=0;g3=1;g4=0;s1=0;s2=0;s3=0;s4=0;m1=0;m2=0;m3=0;m4=0;m5=0;m6=0;
            Rx_Buffer[4]=0x00;Rx_Buffer2[4]=0x00;flagRed=1;   
        }else if(Rx_Buffer[4]==0x08){//Oprimio Gateway4
            ip1=0;ip2=0;ip3=0;ip4=0;g1=0;g2=0;g3=0;g4=1;s1=0;s2=0;s3=0;s4=0;m1=0;m2=0;m3=0;m4=0;m5=0;m6=0;
            Rx_Buffer[4]=0x00;Rx_Buffer2[4]=0x00;flagRed=1;   
        }else if(Rx_Buffer[4]==0x09){//Oprimio Subnet1
            ip1=0;ip2=0;ip3=0;ip4=0;g1=0;g2=0;g3=0;g4=0;s1=1;s2=0;s3=0;s4=0;m1=0;m2=0;m3=0;m4=0;m5=0;m6=0;
            Rx_Buffer[4]=0x00;Rx_Buffer2[4]=0x00;flagRed=1;   
        }else if(Rx_Buffer[4]==0x0a){//Oprimio Subnet2
            ip1=0;ip2=0;ip3=0;ip4=0;g1=0;g2=0;g3=0;g4=0;s1=0;s2=1;s3=0;s4=0;m1=0;m2=0;m3=0;m4=0;m5=0;m6=0;
            Rx_Buffer[4]=0x00;Rx_Buffer2[4]=0x00;flagRed=1;   
        }else if(Rx_Buffer[4]==0x0b){//Oprimio Subnet3
            ip1=0;ip2=0;ip3=0;ip4=0;g1=0;g2=0;g3=0;g4=0;s1=0;s2=0;s3=1;s4=0;m1=0;m2=0;m3=0;m4=0;m5=0;m6=0;
            Rx_Buffer[4]=0x00;Rx_Buffer2[4]=0x00;flagRed=1;   
        }else if(Rx_Buffer[4]==0x0c){//Oprimio Subnet4
            ip1=0;ip2=0;ip3=0;ip4=0;g1=0;g2=0;g3=0;g4=0;s1=0;s2=0;s3=0;s4=1;m1=0;m2=0;m3=0;m4=0;m5=0;m6=0;
            Rx_Buffer[4]=0x00;Rx_Buffer2[4]=0x00;flagRed=1;   
        }
        /*
        else if(Rx_Buffer[4]==0x0d){//Oprimio Subnet1
            ip1=0;ip2=0;ip3=0;ip4=0;g1=0;g2=0;g3=0;g4=0;s1=0;s2=0;s3=0;s4=0;m1=1;m2=0;m3=0;m4=0;m5=0;m6=0;
            Rx_Buffer[4]=0x00;Rx_Buffer2[4]=0x00;flagRed=1;   
        }else if(Rx_Buffer[4]==0x0e){
            ip1=0;ip2=0;ip3=0;ip4=0;g1=0;g2=0;g3=0;g4=0;s1=0;s2=0;s3=0;s4=0;m1=0;m2=1;m3=0;m4=0;m5=0;m6=0;
            Rx_Buffer[4]=0x00;Rx_Buffer2[4]=0x00;flagRed=1;   
        }else if(Rx_Buffer[4]==0x0f){
            ip1=0;ip2=0;ip3=0;ip4=0;g1=0;g2=0;g3=0;g4=0;s1=0;s2=0;s3=0;s4=0;m1=0;m2=0;m3=1;m4=0;m5=0;m6=0;
            Rx_Buffer[4]=0x00;Rx_Buffer2[4]=0x00;flagRed=1;   
        }else if(Rx_Buffer[4]==0x10){
            ip1=0;ip2=0;ip3=0;ip4=0;g1=0;g2=0;g3=0;g4=0;s1=0;s2=0;s3=0;s4=0;m1=0;m2=0;m3=0;m4=1;m5=0;m6=0;
            Rx_Buffer[4]=0x00;Rx_Buffer2[4]=0x00;flagRed=1;   
        }else if(Rx_Buffer[4]==0x12){
            ip1=0;ip2=0;ip3=0;ip4=0;g1=0;g2=0;g3=0;g4=0;s1=0;s2=0;s3=0;s4=0;m1=0;m2=0;m3=0;m4=0;m5=1;m6=0;
            Rx_Buffer[4]=0x00;Rx_Buffer2[4]=0x00;flagRed=1;   
        }else if(Rx_Buffer[4]==0x13){
            ip1=0;ip2=0;ip3=0;ip4=0;g1=0;g2=0;g3=0;g4=0;s1=0;s2=0;s3=0;s4=0;m1=0;m2=0;m3=0;m4=0;m5=0;m6=1;
            Rx_Buffer[4]=0x00;Rx_Buffer2[4]=0x00;flagRed=1;   
        }
        */
        
        if(Rx_Buffer[4]==0x11){
            printf("page Ajustes");
            SendDataDisplay();
            /*
            sprintf(Str,"%d.%d.%d.%d",IP[0],IP[1],IP[2],IP[3]); 
            ETH0_SetIP(ETH0_ParseIP(Str));
            sprintf(displayStr,"%d.%d.%d.%d",Gateway[0],Gateway[1],Gateway[2],Gateway[3]); 
            sprintf(Str2,"%d.%d.%d.%d",Subnet[0],Subnet[1],Subnet[2],Subnet[3]); 
            ETH0_Init(MAC,ETH0_ParseIP(Str),ETH0_ParseIP(Str2),ETH0_ParseIP(displayStr));
            */
            EEPROM_WriteByte(IP[0],47);
            delay_ms(10);
            EEPROM_WriteByte(IP[1],48);
            delay_ms(10); 
            EEPROM_WriteByte(IP[2],49);
            delay_ms(10); 
            EEPROM_WriteByte(IP[3],50);
            delay_ms(10); 
            EEPROM_WriteByte(Gateway[0],51);
            delay_ms(10); 
            EEPROM_WriteByte(Gateway[1],52);
            delay_ms(10); 
            EEPROM_WriteByte(Gateway[2],53);
            delay_ms(10); 
            EEPROM_WriteByte(Gateway[3],54);
            delay_ms(10); 
            EEPROM_WriteByte(Subnet[0],55);
            delay_ms(10); 
            EEPROM_WriteByte(Subnet[1],56);
            delay_ms(10); 
            EEPROM_WriteByte(Subnet[2],57);
            delay_ms(10);
            EEPROM_WriteByte(Subnet[3],58);
            delay_ms(10); 
            /*
            EEPROM_WriteByte(MAC[0],59);
            delay_ms(10); 
            EEPROM_WriteByte(MAC[1],60);
            delay_ms(10); 
            EEPROM_WriteByte(MAC[2],61);
            delay_ms(10); 
            EEPROM_WriteByte(MAC[3],62);
            delay_ms(10); 
            EEPROM_WriteByte(MAC[4],63);
            delay_ms(10); 
            */
            //EEPROM_WriteByte(MAC[5],64);
            //CyDelay(10); 
            ajustaIP=10;
            Envio_Esclavos();
            delay_ms(200);
            ajustaIP=5;
            Envio_Esclavos();
            delay_ms(100);
            Rx_Buffer[4]=0x00;
            Rx_Buffer2[4]=0x00;   
            flagRed=0;
        } 
    }//Menu de RED
//_--------------------------------------------------------------------------------------------------------------------------------------------------------          

/*
//_--------------------------------------------------------------------------------------------------------------------------------------------------------      
    if(Menu==11){ //Menu de Calibracion de Temperatura
         if(Rx_Buffer[4]==0x01){// Sensor 1
            Opcion=1;            
         }else if(Rx_Buffer[4]==0x02){//Patron 1
            Opcion=2;
         }else if(Rx_Buffer[4]==0x03){//Sensor 2
            Opcion=3;
         }else if(Rx_Buffer[4]==0x04){//Patron 2
            Opcion=4;
         }else if(Rx_Buffer[4]==0x05){//Sensor 3
            Opcion=5;
         }else if(Rx_Buffer[4]==0x06){//Patron 3
            Opcion=6;
         }else if(Rx_Buffer[4]==0x07){//Sensor 1, Temperatura 2
            Opcion=7;
         }else if(Rx_Buffer[4]==0x08){//Patron 1, Temperatura 2
            Opcion=8;
         }else if(Rx_Buffer[4]==0x09){//Sensor 2, Temperatura 2
            Opcion=9;
         }else if(Rx_Buffer[4]==0x0a){//Patron 2, Temperatura 2
            Opcion=10;
         }else if(Rx_Buffer[4]==0x0b){//Sensor 3, Temperatura 2
            Opcion=11;
         }else if(Rx_Buffer[4]==0x0c){//Patron 3, Temperatura 2
            Opcion=12;
         }
         
         if(Opcion>12)
            Opcion=1;
         if(Opcion<1)
            Opcion=12;
         
         for(i=0;i<12;i++){
            if(i==Opcion-1){
                 printf("t%Lu.bco=27501",i);
                 SendDataDisplay();
                 printf("t%Lu.pco=65535",i);
                 SendDataDisplay();
             }else{
                 printf("t%Lu.bco=65535",i);
                 SendDataDisplay();
                 printf("t%Lu.pco=0",i);
                 SendDataDisplay();
             }
         }
        
         if(Rx_Buffer[4]==0x11){//Selecciono Regresar
            printf("page Ajustes");
            SendDataDisplay();
            EEPROM_WriteByte(calib[0].patron1,64);delay_ms(20);
            EEPROM_WriteByte(extraeDecimal(calib[0].patron1),65);delay_ms(20);
            EEPROM_WriteByte(calib[0].patron2,66);delay_ms(20);
            EEPROM_WriteByte(extraeDecimal(calib[0].patron2),67);delay_ms(20);
            EEPROM_WriteByte(calib[0].patron3,68);delay_ms(20);
            EEPROM_WriteByte(extraeDecimal(calib[0].patron3),69);delay_ms(20);
            EEPROM_WriteByte(calib[0].control1,70);delay_ms(20);
            EEPROM_WriteByte(extraeDecimal(calib[0].control1),71);delay_ms(20);
            EEPROM_WriteByte(calib[0].control2,72);delay_ms(20);
            EEPROM_WriteByte(extraeDecimal(calib[0].control2),73);delay_ms(20);
            EEPROM_WriteByte(calib[0].control3,74);delay_ms(20);
            EEPROM_WriteByte(extraeDecimal(calib[0].control3),75);delay_ms(20);
            
            EEPROM_WriteByte(calib[1].patron1,76);delay_ms(20);
            EEPROM_WriteByte(extraeDecimal(calib[1].patron1),77);delay_ms(20);
            EEPROM_WriteByte(calib[1].patron2,78);delay_ms(20);
            EEPROM_WriteByte(extraeDecimal(calib[1].patron2),79);delay_ms(20);
            EEPROM_WriteByte(calib[1].patron3,80);delay_ms(20);
            EEPROM_WriteByte(extraeDecimal(calib[1].patron3),81);delay_ms(20);
            EEPROM_WriteByte(calib[1].control1,82);delay_ms(20);
            EEPROM_WriteByte(extraeDecimal(calib[1].control1),83);delay_ms(20);
            EEPROM_WriteByte(calib[1].control2,84);delay_ms(20);
            EEPROM_WriteByte(extraeDecimal(calib[1].control2),85);delay_ms(20);
            EEPROM_WriteByte(calib[1].control3,86);delay_ms(20);
            EEPROM_WriteByte(extraeDecimal(calib[1].control3),87);delay_ms(20);
         }
        
        for(i=0;i<2;i++){
            calib[i].m1=(calib[i].control2-calib[i].control1)/(calib[i].patron2-calib[i].patron1);
            calib[i].b1=calib[i].control2-(calib[i].m1*calib[i].patron2);
            calib[i].m2=(calib[i].control3-calib[i].control2)/(calib[i].patron3-calib[i].patron2);
            calib[i].b2=calib[i].control3-(calib[i].m2*calib[i].patron3);
            printf("t%Lu.txt=\"m1:%2.2f b1:%2.2f m2:%2.2f b2:%2.2f\"",i+20,calib[i].m1,calib[i].b1,calib[i].m2,calib[i].b2);
            SendDataDisplay();
        }
        
         if(Rx_Buffer[4]==0x2a || Rx_Buffer[4]==0x2c){//Tecla Arriba Oprimida
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
            if(Rx_Buffer[4]==0x2c){
                Rx_Buffer[4]=0x00;
                Rx_Buffer2[4]=0x00;  
                delay_ms(100);
            }
         }
         
         if(Rx_Buffer[4]==0x2b || Rx_Buffer[4]==0x2d){//Tecla Abajo Oprimida
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
            if(Rx_Buffer[4]==0x2d){
                Rx_Buffer[4]=0x00;
                Rx_Buffer2[4]=0x00;
                delay_ms(100);
            }
         }
        
        printf("t0.txt=\"%2.1f�C\"",calib[0].control1);
        SendDataDisplay();
        printf("t1.txt=\"%2.1f�C\"",calib[0].patron1);
        SendDataDisplay();
        printf("t2.txt=\"%2.1f�C\"",calib[0].control2);
        SendDataDisplay();
        printf("t3.txt=\"%2.1f�C\"",calib[0].patron2);
        SendDataDisplay();
        printf("t4.txt=\"%2.1f�C\"",calib[0].control3);
        SendDataDisplay();
        printf("t5.txt=\"%2.1f�C\"",calib[0].patron3);
        SendDataDisplay();
        printf("t6.txt=\"%2.1f�C\"",calib[1].control1);
        SendDataDisplay();
        printf("t7.txt=\"%2.1f�C\"",calib[1].patron1);
        SendDataDisplay();
        printf("t8.txt=\"%2.1f�C\"",calib[1].control2);
        SendDataDisplay();
        printf("t9.txt=\"%2.1f�C\"",calib[1].patron2);
        SendDataDisplay();
        printf("t10.txt=\"%2.1f�C\"",calib[1].control3);
        SendDataDisplay();
        printf("t11.txt=\"%2.1f�C\"",calib[1].patron3);
        SendDataDisplay();
      
    }//Menu de Calibracion de Temperatura
//_--------------------------------------------------------------------------------------------------------------------------------------------------------       
*/

//_--------------------------------------------------------------------------------------------------------------------------------------------------------
    if(Menu==18){ //Menu de Rampas
        flagBorrado=OFF;
        x1=ON;x10=OFF;x100=OFF;
        
        if(!entroRampa){
            n=0;
            for(i=0;i<24;i++){
                  rampas[i].TemperaturaInt=read_ext_eeprom((n*264)+(i*11));
                  rampas[i].TemperaturaDec=read_ext_eeprom(1+(n*264)+(i*11));
                  rampas[i].Humedad=read_ext_eeprom(2+(n*264)+(i*11));
                  rampas[i].Horas=read_ext_eeprom(3+(n*264)+(i*11));
                  rampas[i].Minutos=read_ext_eeprom(4+(n*264)+(i*11));
                     
                  rampas[i].Temperatura=(inttofloat(rampas[i].TemperaturaDec)+(float)rampas[i].TemperaturaInt);
                  if(rampas[i].Temperatura>45)rampas[i].Temperatura=10;
                  if(rampas[i].Horas>99)rampas[i].Horas=0;
                  if(rampas[i].Minutos>59)rampas[i].Minutos=0;
                  if(rampas[i].Humedad>100.0)rampas[i].Humedad=50.0;
            }
            entroRampa=ON;
        }
        
        for(i=0;i<24;i++){
            if(rampas[i].Horas==0 && rampas[i].Minutos==0){
                Segmentos=i;  
                break;
            }else{
                Segmentos=i+1;  
            }
        }
        if(!flagRampa){
            printf("b61.txt=\"INICIO\"");
            SendDataDisplay();
            printf("b61.bco=1024");
            SendDataDisplay();
        }else{
            printf("b61.txt=\"DETENER\"");
            SendDataDisplay();
            printf("b61.bco=63488");   
            SendDataDisplay();
        }
        printf("titrampas.txt=\"Programa %02Lu, Pasos Programados %02u, Paso seleccionado: %02Lu\"",Programa,Segmentos,CicloLibre);
        SendDataDisplay();
        GuardaEEPROM=OFF;
      
        if(Rx_Buffer[4]>0x00 && Rx_Buffer[4]<0x19){//Personalizados
            if(CicloLibre>1){
                printf("temp.txt=\"Temperatura:TA:%2.1fC  T:%2.1fC\"",rampas[CicloLibre-2].Temperatura,rampas[Rx_Buffer[4]-1].Temperatura);
                SendDataDisplay();
            }else{
                printf("temp.txt=\"Temperatura:TA:---  T:%2.1fC\"",rampas[Rx_Buffer[4]-1].Temperatura);
                SendDataDisplay();
            }
            printf("tiempo.txt=\"Tiempo:%02u:%02u (HH:MM)\"",rampas[Rx_Buffer[4]-1].Horas,rampas[Rx_Buffer[4]-1].Minutos);
            SendDataDisplay();
            printf("hum.txt=\"Humedad:%03u %% \"",rampas[Rx_Buffer[4]-1].Humedad);
            SendDataDisplay();
            CicloLibre=Rx_Buffer[4];
        }else if(CicloLibre==0x00){
            printf("temp.txt=\"Temperatura: --\"");
            SendDataDisplay();
            printf("tiempo.txt=\"Tiempo:--:--\"");
            SendDataDisplay();
            printf("hum.txt=\"Humedad: --\"");
            SendDataDisplay();
        }else if(CicloLibre>0 && CicloLibre<0x19){
            if(CicloLibre>1){
                printf("temp.txt=\"Temperatura:TA:%2.1fC  T:%2.1fC\"",rampas[CicloLibre-2].Temperatura,rampas[CicloLibre-1].Temperatura);
                SendDataDisplay();
            }else{
                printf("temp.txt=\"Temperatura:TA:---  T:%2.1fC\"",rampas[CicloLibre-1].Temperatura);
                SendDataDisplay();
            }
            printf("tiempo.txt=\"Tiempo:%02u:%02u (HH:MM)\"",rampas[CicloLibre-1].Horas,rampas[CicloLibre-1].Minutos);
            SendDataDisplay();
            printf("hum.txt=\"Humedad:%03u %% \"",rampas[CicloLibre-1].Humedad);
            SendDataDisplay();
        }
          
        if(Rx_Buffer[4]==0x20 && CicloLibre>0){//20, Temperatura
            printf("page Temperatura");
            SendDataDisplay();
            rampa=ON;
        }
        
          if(Rx_Buffer[4]==0x30 && CicloLibre>0){//30, Humedad
            printf("page Humedad");
            SendDataDisplay();
            rampa=ON;
        }
          
          if(Rx_Buffer[4]==0x40 && CicloLibre>0){//40, Tiempo
            printf("page Tiempo");
            SendDataDisplay();
            rampa=ON;
        }
        
        if(Rx_Buffer[4]==0x70){//70, Regresar
            printf("page Principal");
            SendDataDisplay();
            rampa=OFF;
            entroRampa=OFF;
        }
    
        if(Rx_Buffer[4]==0x80){//80, Iniciar
            if(!flagRampa){
                if(Segmentos>0){
                  if(!inicioControlH && !inicioControlT && !inicioControlL){
                    rampa=OFF;
                    flagRampa=ON;
                    // Configura parametros para inicio de rampa  
                    SetpointAnt=Setpoint;
                    SetpointHAnt=SetpointH;
                    hEstpAnt=hEstp;
                    mEstpAnt=mEstp;
                    TempeInicial=Temperatura;
                    Tiempo=((float)rampas[Segmento].Horas*3600.0)+((float)rampas[Segmento].Minutos*60.0);         
                    Setpoint=rampas[Segmento].Temperatura;
                    SetpointH=rampas[Segmento].Humedad;
                    Pendiente=((Setpoint-TempeInicial)/Tiempo);
                    hEstp=rampas[Segmento].Horas;
                    mEstp=rampas[Segmento].Minutos;
                    flagControlH=ON;
                    flagControlT=ON;
                    flagControlL=ON;
                    flagFinalizo=OFF;
                    printf("page Iniciado");
                    SendDataDisplay();
                    Rx_Buffer[4]=0x00;  
                    Rx_Buffer2[4]=0x00; 
                  }
                }else{
                    printf("titrampas.txt=\"!Programa Vacio, programe al menos 1 segmento!\"");
                    SendDataDisplay();
                    BUZZER_Write(1);
                    delay_ms(1000);
                    BUZZER_Write(0);
                    Rx_Buffer[4]=0x00;  
                    Rx_Buffer2[4]=0x00;    
                }
            }else{
                flagRampa=OFF;
                Setpoint=SetpointAnt;
                SetpointH=SetpointHAnt;
                hEstp=hEstpAnt;
                mEstp=mEstpAnt;
                mEst=0;
                hEst=0;
                flagControlH=OFF;
                flagControlT=OFF;
                flagControlL=OFF;
                Rx_Buffer[4]=0x00;  
                Rx_Buffer2[4]=0x00; 
                printf("page Iniciado");
                SendDataDisplay();
                entroRampa=OFF;
            }
            entroRampa=OFF;
        }
        
        if(Rx_Buffer[4]==0x90){//90, Borrar  
            rampa=OFF;
            for(i=0;i<24;i++){
                BUZZER_Write(1);
                delay_ms(50);
                BUZZER_Write(0);
                delay_ms(50);
                rampas[i].Horas=0;
                rampas[i].Minutos=0;
                write_ext_eeprom(((Programa-1)*264)+((i)*11),0);
                delay_ms(5);
                write_ext_eeprom(1+((Programa-1)*264)+((i)*11),0);
                delay_ms(5);
                write_ext_eeprom(2+((Programa-1)*264)+((i)*11),0);
                delay_ms(5);
                write_ext_eeprom(3+((Programa-1)*264)+((i)*11),0);
                delay_ms(5);
                write_ext_eeprom(4+((Programa-1)*264)+((i)*11),0);
                delay_ms(5);
                write_ext_eeprom(5+((Programa-1)*264)+((i)*11),0);
                delay_ms(5);
                write_ext_eeprom(6+((Programa-1)*264)+((i)*11),0);
                delay_ms(5);
                rampas[i].Temperatura=0;
                rampas[i].Humedad=0;                
            }
            printf("page Eliminado");
            SendDataDisplay();
            Rx_Buffer[4]=0x00;  
            Rx_Buffer2[4]=0x00;
        }
   }//Menu de Rampas
//_--------------------------------------------------------------------------------------------------------------------------------------------------------       

//_--------------------------------------------------------------------------------------------------------------------------------------------------------      
    if(Menu==21){//Menu de programa eliminado
      delay_ms(800);         
      entro=ON;      
      printf("page Rampas");
      SendDataDisplay();
   }//Menu de programa eliminado
//_--------------------------------------------------------------------------------------------------------------------------------------------------------      

//_--------------------------------------------------------------------------------------------------------------------------------------------------------      
    if(Menu==22){//Menu de programa iniciado
      if(flagRampa){
        printf("trampa.txt=\"Programa %02Lu Iniciado\"",Programa);
        SendDataDisplay();
      }else{
        printf("trampa.txt=\"Programa Detenido\"");
        SendDataDisplay();
      }
      delay_ms(800);         
      printf("page Principal");
      SendDataDisplay();
   }//Menu de programa iniciado
//_--------------------------------------------------------------------------------------------------------------------------------------------------------          

 //_--------------------------------------------------------------------------------------------------------------------------------------------------------
    if(Menu==23){ //Menu de Alarmas
      
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
        }else if(Rx_Buffer[4]==0x01){//Selecciono Temperatura
            flagTem=ON;
            flagHum=OFF;
            printf("ttemp.pco=65535");
            SendDataDisplay();
            printf("thum.pco=0");
            SendDataDisplay();
            Rx_Buffer[4]=0x00;  
            Rx_Buffer2[4]=0x00;     
        }else if(Rx_Buffer[4]==0x02){//Selecciono Humedad
            flagTem=OFF;
            flagHum=ON;
            printf("thum.pco=65535");
            SendDataDisplay();
            printf("ttemp.pco=0");
            SendDataDisplay();
            SendDataDisplay();
            Rx_Buffer[4]=0x00;  
            Rx_Buffer2[4]=0x00;     
        }else{// if(Rx_Buffer[4]!=0x00){
           pulsoAumentar=OFF;
           pulsoDisminuir=OFF; 
        }
        
         if(Rx_Buffer[4]==0x11){//Selecciono Regresar
            Horas=OFF;
            Minutos=OFF;
            printf("page Ajustes");
            SendDataDisplay();
            if(!GuardaEEPROM){
               write_eeprom(23,(int8)alarmaTemperatura);delay_ms(20);write_eeprom(24,extraeDecimal(alarmaTemperatura));delay_ms(20);
               write_eeprom(25,(int8)alarmaHumedad);delay_ms(20);write_eeprom(26,extraeDecimal(alarmaHumedad));delay_ms(20);
               GuardaEEPROM=ON;
            }
            pulsoAumentar=OFF;
            pulsoDisminuir=OFF;
         }
         
        if(pulsoAumentar){            
                if(flagTem)
                    alarmaTemperatura+=0.1;    
                if(flagHum)
                    alarmaHumedad+=1;    
        }
        
        if(pulsoDisminuir){
            if(flagTem)
                    alarmaTemperatura-=0.1;    
                if(flagHum)
                    alarmaHumedad-=1; 
        }
        
        if(alarmaTemperatura<0.5)
           alarmaTemperatura=0.5;
        
        if(alarmaHumedad<5.0)
           alarmaHumedad=5.0;
        
        
         printf("ttemp.txt=\"+/-%1.1f\"",alarmaTemperatura);
         SendDataDisplay();
         printf("thum.txt=\"+/-%1.0f\"",alarmaHumedad);
         SendDataDisplay();
      
    }//Menu de Alarmas
//_--------------------------------------------------------------------------------------------------------------------------------------------------------       

 
}
