void ActualizaRecta(){
   X[y]=Temperatura;
   y++;
   if(y>=2)
   {
      y=0;
      Aumento=X[1]-X[0];
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
   Temperature=(0.1272*V0)-25.002;
   
   if(l>(media-1))
   {l=0;}
   promedio[l]=Temperature;l++;
   //promedio[l]=V0;l++;
         
   for(h=0;h<media;h++)
   {
      Prom+=promedio[h];
   } 
   Prom=Prom/media;   
   return Prom;
   //return Temperature;
}

void Controla(){
//-------------------------------Control de Calentamiento------------------------------------------------------------//         
   if(tiemporecta>=40)
   {
      ActualizaRecta();
      tiemporecta=0;
   } 
  
   if(Enfria==1)
   {
      Rele1_off;
      Rele2_off;
      error=Setpoint-desvio-Temperatura;      // Cálculo del error ******
      
      if(error<=-0.7 && Aumento>-0.2)
      {
         if(Potencia<90.0)
         {
            if(TiempoControl>=80)
            {
               TiempoControl=0;
               if(Aumento>0.2)
               {
                  Ganancia+=1;
               }
               else
               {
                  if(Aumento>0.0)
                  {
                     Ganancia+=0.6;
                  }   
                  else
                  {
                     Ganancia+=0.3;
                  }
               }
            }
         }
      }
   
      if(error>0.0)     // Anti Wind_DOWN
      {
         error=0.0;
      }
      error=error*-1.0;
      
      if(Ganancia<0.1)
      {
         Ganancia=0.1;
      }
   
      tmp=error*Ganancia;  // Control Proporcional.
            
      if(tmp>Setpoint)   // Anti Wind-UP      
      {
         tmp=Setpoint;
      }
                  
      if(tmp<0.0)     // Anti Wind_DOWN    
      {
         tmp=0.0;
      }
      
      // Tpulso(t)= Tciclo*(y(t)-ymin)/(ymax - ymin); calculo de ciclo util para control de resistencia.ymax=140.ymin=0;
      // Tpulso es float, hay que pasar ese ciclo a una salida de un puerto usando el TMR1.
      Tpulso=(tmp/Setpoint)*Tciclo;  
      
      Tp=(Tpulso/0.005);
      Tc=(Tciclo/0.005);
      Controlando=1;       
   }
   else
   {
      Rele1_on;
      Rele2_on;
      error=desvio+Setpoint-Temperatura;      // Cálculo del error ******
   
      if(error>0.7 && error<30 && Aumento<0.3)
      {
         flag_ajuste=0;
         flag_ajuste2=0;
         Ajusto=10;
         if(Potencia<90.0)
         {
            if(TiempoControl>=80)
            {
               TiempoControl=0;
               if(Aumento<-0.5)
               {
                  Ganancia+=1;
               }
               else
               {
                  if(Aumento<-0.1)
                  {
                     Ganancia+=0.6;
                  }   
                  else
                  {
                     Ganancia+=0.3;
                  }
               }
            }
         }
      }
      
      if(error<0.0)
      {
         Ganancia=GananciaR*(Setpoint/44.5);
         flag_ajuste=0;
         flag_ajuste2=0;
      }
      
      if(Ganancia<0.1)
      {
         Ganancia=0.1;
      }
      if(Ganancia>Gananciamax)
      {
         Ganancia=Gananciamax;
      }
   
      if(error<0.0)     // Anti Wind_DOWN    
      {
         error=0.0;
      }
      //tmp=(Setpoint-Prom)*Ganancia;  // Control Proporcional.
      tmp=error*Ganancia;  // Control Proporcional.
            
      if(tmp>Setpoint)   // Anti Wind-UP      
      {
         tmp=Setpoint;
      }
                  
      if(tmp<0.0)     // Anti Wind_DOWN    
      {
         tmp=0.0;
      }
   
      Tpulso=(tmp/Setpoint)*Tciclo;
      
      /*if(error<1.6)
      {
         if(error<=1.3){   
            if(flag_ajuste==0){
               TpulsoMax=Tpulso;
               flag_ajuste=1;  
               Ajusto=5;
            }
         }
         
         if(error<=0.9){   
            if(flag_ajuste2==0){
               Tpulsomin=Tpulso;
               flag_ajuste2=1;           
               Ajusto=5;
            }
         }
         
         if(flag_ajuste2==1){
            if(Tpulso<Tpulsomin){
               Tpulso=Tpulsomin;
            }
         }
         if(flag_ajuste==1){   
            if(Tpulso>TpulsoMax){
               Tpulso=TpulsoMax;
            } 
         }
      }*/
      Tp=(Tpulso/0.005);
      Tc=(Tciclo/0.005);
      Controlando=1;     
   }
   
//--------------------------------------------------------------------------------------------------------------------//   
  
}

void displayMenu(char palabra[30], int x, int y, int sombreado, int tamano)
{
   strcpy(Menus.Letra,palabra);
   if(sombreado==2)
      {glcd_text57(x+1, y+1, Menus.Letra, tamano, ON);}
   if(sombreado==1)
      {glcd_rect(0, y, 127, y+(tamano*8), YES, ON);glcd_text57(x+1, y+1, Menus.Letra, tamano, OFF);}
   if(sombreado==0)
      {glcd_rect(0, y, 127, y+(tamano*8), YES, OFF);glcd_text57(x+1, y+1, Menus.Letra, tamano, ON);}
   glcd_update();
}

void displayTemps(char palabra[30], float digito, int x, int y, int sombreado, int tamano)
{
   char voltage[9];
   
   if(sombreado==1)
   {
      sprintf(voltage, "%03.1f", digito);
      glcd_rect(0, y-2, 127, y+(tamano*8), YES, ON);
      strcpy(Menus.Letra,palabra);
      glcd_text57(x+90, y+1, voltage, tamano, OFF);
      glcd_text57(x+1, y+1, Menus.Letra, tamano, OFF);
      strcpy(Menus.Letra,"C");
      glcd_text57(x+120, y+1, Menus.Letra, tamano, OFF);
      glcd_circle(x+115,y-1,1,NO,OFF);
   }
   if(sombreado==0)
   {
      sprintf(voltage, "%03.1f", digito);
      glcd_rect(0, y-2, 127, y+(tamano*8), YES, OFF);
      strcpy(Menus.Letra,palabra);
      glcd_text57(x+90, y+1, voltage, tamano, ON);
      glcd_text57(x+1, y+1, Menus.Letra, tamano, ON);
      strcpy(Menus.Letra,"C");
      glcd_text57(x+120, y+1, Menus.Letra, tamano, ON);
      glcd_circle(x+115,y-1,1,NO,ON);
   }
   glcd_update();
}

void displayTiempo(int digito, int x, int y, int sombreado, int tamano)
{
   char voltage[9];
   
   if(sombreado==2)
   {
      sprintf(voltage, "%03u", digito);
      glcd_rect(x, y, x+(tamano*18), y+(tamano*8), YES, ON);
      glcd_text57(x+1, y+1, voltage, tamano, OFF);
   }
   else
   {
      if(sombreado==3)
      {
         sprintf(voltage, "%03u", digito);
         glcd_rect(x, y, x+(tamano*18), y+(tamano*8), YES, OFF);
         glcd_text57(x+1, y+1, voltage, tamano, ON);
      }
      sprintf(voltage, "%u", digito);
      if(sombreado==1)
         {glcd_rect(x, y, x+(tamano*5), y+(tamano*8), YES, ON);glcd_text57(x+1, y+1, voltage, tamano, OFF);}
      if(sombreado==0)
         {glcd_rect(x, y, x+(tamano*5), y+(tamano*8), YES, OFF);glcd_text57(x+1, y+1, voltage, tamano, ON);}
   }
   glcd_update();
}

void displayTemperatura(int16 digito, int x, int y, int sombreado, int tamano)
{
   char voltage[9];
   
   if(sombreado==2)
   {
      sprintf(voltage, "%03Lu", digito);
      glcd_rect(x, y, x+(tamano*18), y+(tamano*8), YES, ON);
      glcd_text57(x+1, y+1, voltage, tamano, OFF);
   }
   else
   {
      sprintf(voltage, "%03Lu", digito);
   
      if(sombreado==4)
      {
         glcd_rect(x, y, x+(tamano*16), y+(tamano*8), YES, OFF);
         glcd_text57(x+1, y+1, voltage, tamano, ON);
      }
      if(sombreado==1)
         {glcd_rect(x, y, x+(tamano*5), y+(tamano*8), YES, ON);glcd_text57(x+1, y+1, voltage, tamano, OFF);}
      if(sombreado==0)
         {glcd_rect(x, y, x+(tamano*5), y+(tamano*8), YES, OFF);glcd_text57(x+1, y+1, voltage, tamano, ON);}
   }
   glcd_update();
}

void glcd_imagen(int8 x)
{
   char i,j;
   signed char k; 
   
   if(x==5){
   for( i = 0 ; i < 64 ; i ++ )
   {  
      for( j = 0 ; j < 16 ; j ++)
      {    
         for(k=7;k>-1;k--)
         {      
            if( bit_test(Horno[i][j] ,7-k )) 
               glcd_pixel( j*8+k,i, ON );
         }  
      }
   }
   }
   
}
   
void Limpia(int x1,int x2,int y){
   glcd_rect(x1, y, x2, y+7, YES, OFF);
}

void displayT(float adc,int x,int y,int w) {
   char voltage[9];    
   sprintf(voltage, "%03.1f",adc); // Converts adc to text
   glcd_rect(x, y, x+(w*27), y+(w*8), YES, OFF);
   glcd_text57(x, y, voltage, w, ON);
}

void displayContrasena(int digito, int x, int y, int sombreado)
{
   char voltage[9];
   sprintf(voltage, "%i", digito);
   
   if(sombreado==1)
      {glcd_rect(x, y, x+18, y+24, YES, ON);glcd_text57(x+2, y+1, voltage, 2, OFF);}
   else   
      {glcd_rect(x, y, x+18, y+24, NO, OFF);glcd_text57(x+2, y+1, voltage, 2, ON);}

}


void MensajeAlmacenado(){
   glcd_fillScreen(OFF);
   strcpy(Menus.Letra,"Valor");
   displayMenu(Menus.Letra,30,0,0,2);
   strcpy(Menus.Letra," Almacenado");
   displayMenu(Menus.Letra,0,30,0,2);
   glcd_update();
   delay_ms(2000);
   glcd_fillScreen(OFF);
}

void ConfiguraInterrupciones(){
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_1);
   enable_interrupts(INT_TIMER1);
   setup_adc_ports(AN0_TO_AN1);
   setup_adc(ADC_CLOCK_DIV_64);
   enable_interrupts(global);
}

void MensajeBienvenida(){
   glcd_init(ON);             //Inicializa la glcd
   glcd_fillScreen(OFF);      //Limpia la pantalla
   strcpy(Menus.Letra,"JP Inglobal");
   displayMenu(Menus.Letra,0,0,0,2);     
   strcpy(Menus.Letra,"    Incubadora");
   displayMenu(Menus.Letra,0,20,0,1);       
   strcpy(Menus.Letra,"     Digital  ");
   displayMenu(Menus.Letra,0,30,0,1);
   strcpy(Menus.Letra,"    53 Lts");
   displayMenu(Menus.Letra,0,45,0,2);
   delay_ms(1000);
   glcd_fillScreen(OFF);      //Limpia la pantalla
   
   for(i=0;i<80;i++)
   {
      Temperatura=Leer_Sensor(MediaMovil);
   }
}

void LeeEEPROM(){
   Testerilizacionp[0]=read_eeprom(0);
   delay_ms(10);   
   Testerilizacionp[1]=read_eeprom(1);
   delay_ms(10);   
   Testerilizacionp[2]=read_eeprom(2);
   delay_ms(10);
   Testerilizacionp[3]=read_eeprom(3);
   delay_ms(10);   
   
   entero=read_eeprom(80);
   delay_ms(10);   
   Temporal=read_eeprom(81);
   Temporal=Temporal/10.0;
   Temperatura1=entero+Temporal;
   delay_ms(10);   
   
   entero=read_eeprom(83);
   delay_ms(10);   
   Temporal=read_eeprom(84);
   Temporal=Temporal/10.0;
   Temperatura2=entero+Temporal;
   delay_ms(10);   
   
   entero=read_eeprom(86);
   delay_ms(10);   
   Temporal=read_eeprom(87);
   Temporal=Temporal/10.0;
   Temperatura3=entero+Temporal;
   delay_ms(10);   
   
   Password[0]=read_eeprom(10);
   delay_ms(10);   
   Password[1]=read_eeprom(11);
   delay_ms(10);   
   Password[2]=read_eeprom(12);
   delay_ms(10);   
   Password[3]=read_eeprom(13);
   delay_ms(10);   
   
   Ganancia=read_eeprom(90);
   delay_ms(10);   
   Tciclo=read_eeprom(30);
   delay_ms(10);   
   Ajuste=read_eeprom(40);
   delay_ms(10);   
   Ajusten=read_eeprom(50);
   delay_ms(10);   
   MediaMovil=read_eeprom(60);
   delay_ms(10);   
   Coeficiente=read_eeprom(70);
   delay_ms(10);   
   Saturacion=read_eeprom(100);
   delay_ms(10);   
   GananciaR=Ganancia;
}

void LimitaValores(){
   if(Testerilizacionp[0]>9 || Testerilizacionp[0]<0){
      Testerilizacionp[0]=0;write_eeprom(0,0);delay_ms(20);
   }
   if(Testerilizacionp[1]>9 || Testerilizacionp[1]<0){
      Testerilizacionp[1]=0;write_eeprom(1,0);delay_ms(20);
   }
   if(Testerilizacionp[2]>9 || Testerilizacionp[2]<0){
      Testerilizacionp[2]=0;write_eeprom(2,0);delay_ms(20);
   }
   if(Testerilizacionp[3]>9 || Testerilizacionp[3]<0){
      Testerilizacionp[3]=0;write_eeprom(3,0);delay_ms(20);
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
   if(Temperatura1>40.0){
      Temperatura1=30.0;write_eeprom(80,30);delay_ms(20);write_eeprom(81,0);delay_ms(20);
   }
   if(Temperatura2>40.0){
      Temperatura2=30.0;write_eeprom(83,30);delay_ms(20);write_eeprom(84,0);delay_ms(20);
   }
   if(Temperatura3>40.0){
      Temperatura3=30.0;write_eeprom(86,30);delay_ms(20);write_eeprom(87,0);delay_ms(20);
   }
   if(Ganancia>40){
      Ganancia=2;write_eeprom(90,2);delay_ms(20);
   }
   if(Tciclo>40){
      Tciclo=5;write_eeprom(30,5);delay_ms(20);
   }
   if(Ajuste>100){
      Ajuste=0;write_eeprom(40,0);delay_ms(20);
   }
   if(Ajusten>100){
      Ajusten=0;write_eeprom(50,0);delay_ms(20);
   }
   if(MediaMovil>80){
      MediaMovil=80;write_eeprom(60,80);delay_ms(20);
   }
}

void DelayPuls(){
   delay_ms(2);
}

short PidePassword(int8 MenuAnt, int8 MenuActual, int8 MenuPost, int8 clave0,int8 clave1, int8 clave2,int8 clave3,short cambia){
      if(n==0)
      {
         displayContrasena(Contrasena[0],20,30,1);displayContrasena(Contrasena[1],40,30,0);
         displayContrasena(Contrasena[2],60,30,0);displayContrasena(Contrasena[0],80,30,0);
         if(!cambia){
            strcpy(Menus.Letra,"Clave");
            displayMenu(Menus.Letra,20,0,1,3);
         }else{
            strcpy(Menus.Letra,"Clave Nueva");
            displayMenu(Menus.Letra,0,0,1,2);  
         }
         n=1;
      }
      
      if(UP){
         if(Flanco == 0){
            Flanco = 1;DelayPuls();
            for(i=1;i<=4;i++){
               if(unidad==i){
                  Contrasena[i-1]++;
                  if(Contrasena[i-1]>9){
                     Contrasena[i-1]=0;
                  }
                  displayContrasena(Contrasena[i-1],i*20,30,1);
               }else{
                  displayContrasena(Contrasena[i-1],i*20,30,0);
               }
            }
         }
      }else{
         Flanco = 0;
      }
            
      if(DOWN){
         if(Flanco2 == 0){
            Flanco2 = 1;DelayPuls();
            for(i=1;i<=4;i++){
               if(unidad==i){
                  Contrasena[i-1]--;
                  if(Contrasena[i-1]<0){
                     Contrasena[i-1]=9;
                  }
                  displayContrasena(Contrasena[i-1],i*20,30,1);
               }else{
                  displayContrasena(Contrasena[i-1],i*20,30,0);
               }
            }      
         }     
      }else{
         Flanco2 = 0;
      }
            
      if(RIGHT){
         if(Flanco1 == 0){
            Flanco1 = 1;DelayPuls();unidad++;
            for(i=1;i<=4;i++){
               if(unidad==i){
                  displayContrasena(Contrasena[i-1],i*20,30,1);
               }else{
                  displayContrasena(Contrasena[i-1],i*20,30,0);
               }
            }
         }
      }else{
         Flanco1 = 0;
      }
      
      if(LEFT){
         delay_ms(100);
         if(LEFT){
            Menu=MenuAnt;glcd_fillScreen(OFF);n=0;Opcion=1;unidad=1;
            Contrasena[0]=0;Contrasena[1]=0;Contrasena[2]=0;Contrasena[3]=0;
            glcd_fillScreen(OFF);glcd_update();
            return 1;
         }
      }
    
      if(unidad>4){
         glcd_fillScreen(OFF);unidad=1;
         
         if(!cambia){
            if(Contrasena[0]==3&&Contrasena[1]==8&&Contrasena[2]==9&&Contrasena[3]==2 && MenuActual==0){ // Si Ingresa clave para reset general del sistema.
               write_eeprom(10,0);delay_ms(20);write_eeprom(11,0);delay_ms(20);// Reestablece a contraseña de Fabrica y reinicia Programa.
               write_eeprom(12,0);delay_ms(20);write_eeprom(13,0);delay_ms(20);
               reset_cpu();
            }
         
            if((Contrasena[0]==clave0)&&(Contrasena[1]==clave1)&&(Contrasena[2]==clave2)&&(Contrasena[3]==clave3)){
               glcd_fillScreen(OFF);
               strcpy(Menus.Letra,"Clave");
               displayMenu(Menus.Letra,30,0,0,2);
               strcpy(Menus.Letra,"Correcta");
               displayMenu(Menus.Letra,15,30,0,2);
               Menu=MenuPost;glcd_update();delay_ms(1000);glcd_fillScreen(OFF);
               Contrasena[0]=0;Contrasena[1]=0;Contrasena[2]=0;Contrasena[3]=0;
               n=0;
               return 1;
            }else{
               glcd_fillScreen(OFF);
               strcpy(Menus.Letra,"Clave");
               displayMenu(Menus.Letra,30,0,0,2);
               strcpy(Menus.Letra,"Incorrecta");
               displayMenu(Menus.Letra,0,30,0,2);
               unidad=1;glcd_update();delay_ms(1000);Contrasena[0]=0;Contrasena[1]=0;Contrasena[2]=0;Contrasena[3]=0;glcd_fillScreen(OFF);
               glcd_rect(0, 0, 127, 25, YES, ON);strcpy(Menus.Letra,"Clave");glcd_text57(25, 1, Menus.Letra, 3, OFF);displayContrasena(Contrasena[0],20,30,1);
               displayContrasena(Contrasena[1],40,30,0);displayContrasena(Contrasena[2],60,30,0);displayContrasena(Contrasena[0],80,30,0);
               Menu=MenuAnt;glcd_fillScreen(OFF);n=0;
               return 0;
            }
         }else{
            Opcion=1;unidad=1;
            write_eeprom(10,Contrasena[0]);
            delay_ms(50);   
            write_eeprom(11,Contrasena[1]);
            delay_ms(50);   
            write_eeprom(12,Contrasena[2]);
            delay_ms(50);   
            write_eeprom(13,Contrasena[3]);
            delay_ms(1000);glcd_fillScreen(OFF);Menu=MenuAnt;
            n=0;
            Contrasena[0]=0;Contrasena[1]=0;Contrasena[2]=0;Contrasena[3]=0;
            MensajeAlmacenado();
            return 0;
         }
      }
}

void CuentaTiempo(){
      if(tinicio>=60)
      {
         tinicio=0;
         Testerilizacion[0]++;
         if(Testerilizacion[0]>9)
         {
            Testerilizacion[0]=0;
            Testerilizacion[1]++;
         }
         if(Testerilizacion[1]>5)
         {
            Testerilizacion[1]=0;
            Testerilizacion[2]++;
         }
         if(Testerilizacion[2]>9)
         {
            Testerilizacion[2]=0;
            Testerilizacion[3]++;
         }
         if(Testerilizacion[3]>9)
         {
            Testerilizacion[3]=0;
         }
      }
}
