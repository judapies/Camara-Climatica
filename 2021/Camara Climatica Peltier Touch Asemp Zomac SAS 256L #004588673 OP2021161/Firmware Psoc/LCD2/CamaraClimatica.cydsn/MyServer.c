/* ========================================*/

//
//   	HTML-Шпаргалка:    http://ruseller.com/htmlshpora.php?id=52   	Основы HTML   http://html-exp.narod.ru/base.htm    	http://phpclub.ru/detail/article/http_request

#include <stdio.h>
#include <stdlib.h>
#include <funciones.h>
#include <MyServer.h>
#define led_on()    { BUZZER_Write(1);}		
#define led_off()   { BUZZER_Write(0);}	

// variables
char lang_ru;  		// Language page
int counter;		// page update counter
char str[32];
//__________________________________________________________________________

short LED_state (void ){	// LED status monitoring
    return !BUZZER_Read();
}	

char header[] =    
"<!DOCTYPE html>"
"<html>"
"<head>"
  "<meta charset='UTF-8'>"
  "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
  "<meta http-equiv='X-UA-Compatible' content='ie=edge'>"
  "<meta http-equiv='refresh' content='15' >"
  "<title>JP Bioingenieria SAS</title>"

  "<!-- Bootstrap CSS CDN -->"
  "<link rel='stylesheet' href='https://stackpath.bootstrapcdn.com/bootstrap/4.1.0/css/bootstrap.min.css' integrity='sha384-9gVQ4dYFwwWSjIDZnLEWnxCjeSWFphJiwGPXr1jddIhOegiu1FwO5qRGvFXOdJZ4' crossorigin='anonymous'>"

  "<!-- Font Awesome JS -->"
  "<script defer src='https://use.fontawesome.com/releases/v5.0.13/js/solid.js' integrity='sha384-tzzSw1/Vo+0N5UhStP3bvwWPq+uvzCMfrN1fEFe+xBmv1C/AtVX5K0uZtmcHitFZ' crossorigin='anonymous'></script>"
  "<script defer src='https://use.fontawesome.com/releases/v5.0.13/js/fontawesome.js' integrity='sha384-6OIrr52G08NpOFSZdxxz1xdNSndlD4vdcf/q2myIUVO0VsqaGHJsB0RaBE01VTOY' crossorigin='anonymous'></script>"

  "<!--BOOTSTRAP 4 -->"
  "<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css' integrity='sha384-BVYiiSIFeK1dGmJRAkycuHAHRg32OmUcww7on3RYdg4Va+PmSTsz/K68vbdEjh4u' crossorigin='anonymous'>"
    
  "<style>"
  "body {"
	"background: #0F2027;  /* fallback for old browsers */"
	"background: -webkit-linear-gradient(to right, #2C5364, #203A43, #0F2027);  /* Chrome 10-25, Safari 5.1-6 */"
	"background: linear-gradient(to right, #2C5364, #203A43, #0F2027); /* W3C, IE 10+/ Edge, Firefox 16+, Chrome 26+, Opera 12+, Safari 7+ */"
    "color:white;"
  "}"
  "</style>"
"</head>"
    "<h2>Camara Climatica JP Inglobal V1.0.0.0</h2>\r\n";
//__________________________________________________________________________
// Заголовок странички
char web_edit_header[] =   "";// "HTTP/1.0 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n\r\n<html><body>" 
//"<body text='#0000CC' bgcolor='#CCFFFF'>";	  // set color:    http://www.w3schools.com/tags/ref_colorpicker.asp
//__________________________________________________________________________

// Страничка с ошибкой 404
char webif_404_reply[] =
"HTTP/1.0 404 Not Found\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n" 
"<html><body>"   "<pre>Page not found\r\n\r\n"
"<a href='/'>Home page</a></pre>\r\n"
"</body></html>" ; 	// CLOSE PAGE
//__________________________________________________________________________


void set_param(char *params, char *p)	// reading and setting the obtained parameters
{
char *name;   char *value;	

//*____________________________________________________________________________________________________________	
	while(params)	// if detected a question mark   ( ? )
            {
// looking for where it ends.   symbol "&" - the ampersand is a separator between parameters.
                if((p = strchr(params, '&')))
                    *(p++) = 0;	//& replaced by zero ,  p - pointer to the next parameter
// Parse the parameter name and value on
	            name = params;    
                
                if((value = strchr(name, '=')))
                    *(value++) = 0;	//  name - 0 - value  
// params = led (on/off)?
                if( (strcmp (name,  ("led")) == 0 ) && value )	// if name = "led"  and value Found - execute command
                {
                    if(strcmp(value, ("on")) == 0)         		led_off()
                    else if(strcmp(value, ("off")) == 0)       	led_on()
                }
	 params = p;	// next params
			}
}//------------------------------------------------------------------------------------

void display_param(char *buffer)	// Shows received parameters
{
	memset(buffer, 0,  2000);	// all data stored in  buffer  processed. clean the buffer to reply
	strcpy(buffer,	header);	// open page 
    strcat(buffer,	"<pre>");	
/*
    if((!LED_state()))
        strcat(buffer,	"Buzzer encendido. <a href='/?led=on'>Apaguelo</a>." 				);
  	else if (LED_state())
        strcat(buffer,	"Buzzer apagado.  <a href='/?led=off'>Enciendalo</a>."				);*/

	sprintf(str,"<p>Temperatura: <b>%2.1f°C </b>",Temperatura);
	strcat(buffer,	 str);							 
    sprintf(str,"<p>Temperatura2: <b>%2.1f°C </b>",Temperatura2);
	strcat(buffer,	 str);				
    sprintf(str,"<p>Temperatura Ambiente: <b>%02ld°C </b>",Temperature2/100);
	strcat(buffer,	 str);				
    sprintf(str,"<p>Humedad: <b>%2.1f%% </b>",Humedad);
	strcat(buffer,	 str);
    sprintf(str,"<p>Temperatura H: <b>%2.1f°C </b>",Tam);
	strcat(buffer,	 str);
	//strcat(buffer,	"<a href='/edit'>Change</a>");	// The created "Change" button on the page with a link / edit
	//counter = counter+1;									// for the test page refresh increment counter
	strcat(buffer,	"</pre>");
	strcat(buffer,	"</body></html>"); 				// close page
}//----------------------------------

void web_form(char *buffer, int counter)	// form to change the counter
{
	memset(buffer, 0,  2000);			// all data stored in  buffer  processed. clean the buffer to reply
	sprintf(str,"%d",counter);
	strcat( buffer,	 web_edit_header );	// header without  'refresh'
	strcat( buffer,	("<pre> <form action='/' method='GET'>\r\n"));
//	strcat( buffer,	"<form action='/' method='GET'>\r\n");
	strcat( buffer,	"Enter value (0..255):\r\n");
	strcat( buffer,	"<input type='text' name='pwm' size='4' value='");
	strcat( buffer,	 str);
	strcat( buffer,	"'>  ");
	strcat( buffer,	"<input type='submit' value='OK'>\r\n");
	strcat( buffer,	"</form>  </pre> </body> </html>");	// Закрыть страничку
}//__________________________________________________________________________________________________________

/****************************  Called when data is received from the browser  **********************************************************/
/***																																***/
/***************************************************************************************************************************************/

void webif_data(char socket, char *Inbuff)
{
char *url; 		//  URL 
char *p; 		// label or a pointer to the next parameter  (указатель разметки)
char *params;	// a pointer to the name of the parameter

//   Parse query - check that the method - GET and get our URL. (Page address with parameters)
if((p = strchr(Inbuff + 4, ' ')) == 0)
    strcat(Inbuff, " ");	// Request IE update page - there is no space at the end. add!!!
    
if( (memcmp(Inbuff, ("GET "), 4) == 0) &&  ((p = strchr(Inbuff + 4, ' ')) != 0) )	// now begin
  {
	url = Inbuff + 4 ;  					// a pointer to the beginning of the received URL. end - the first space
	*p = 0;   								// celebrate the end of the URL address and the parameters

	//  Parse the URL and the path settings (find in turn question marks)
	if((params = strchr(url, '?')))
        *(params++) = 0;			// write 0 instead of the first one found? immediately followed by - pointer to params

		// Check the path. we have a home page "/" and the page counter compounds   "/edit"  
	
	  if(strcmp(url, ("/")) == 0)			//   home page "/"
		{
       		set_param ( params,  p );		// Parse and process parameters
   			display_param( Inbuff );		// Displays the params on the main page
		}
 	   else if(strcmp(url, ("/edit")) == 0){
            web_form(Inbuff,  counter); 	// Browser requests a page / edit? Submit a form for change the counter
       }else{    // Wrong URL - write 404 page  in  buffer
	        strcat(Inbuff, webif_404_reply);	
       }	

   }	
else    // Wrong URL - write 404 page  in  buffer
   {		 strcat(Inbuff, webif_404_reply);	}	
	ETH0_TcpSend(socket, (uint8*)Inbuff,  strlen((char *)Inbuff));	
}
/* [] END OF FILE */


// iprintf("404: %s\r",Inbuff);



