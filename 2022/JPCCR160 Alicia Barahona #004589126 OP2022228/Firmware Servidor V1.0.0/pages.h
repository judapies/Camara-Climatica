/*********************  PAGINA WEB A MOSTRAR **********************************/
/* Página principal INDEX (/) */
const char  HTML_INDEX_PAGE[]="
   <html>
<head>
<meta http-equiv=\"refresh\"content=\"20;url=\">
<title>JPInglobal Datos Ethernet</title>
</head>

<body style=\"background:#ffffff\"> 

<div style=\"background:#4383d3\" align=\"center\">

   <img src=\"https://jpinglobal.com/images/ando_sistema/Logo-JP_2019-05.png\" alt=\"Logo JPInglobal\" align=\"center\" width=\"567\" height=\"189\">
</div>

<hr size=\"8px\" color=\"#4383d3\">

<div style=\"background:#E1F6FF\" align=\"center\">

   <h2 align=\"center\"> Visualizaci�n de datos</h2>
   <TABLE BORDER=0>
      <TR>
    <TD> <img src=\"https://w7.pngwing.com/pngs/2/237/png-transparent-computer-icons-thermometer-temperature-business-celsius-themometer-measurement-business-temperature-measurement.png\" alt=\"Temperatura\" align=\"center\" width=\"92\" height=\"51\"> </TD>
         <TH>Temperatura:</TH>
         <TD id=\"dyn0\">%0</TD>         
      </TR>
      <TR>
    <TD> <img src=\"https://w7.pngwing.com/pngs/2/237/png-transparent-computer-icons-thermometer-temperature-business-celsius-themometer-measurement-business-temperature-measurement.png\" alt=\"Temperatura\" align=\"center\" width=\"92\" height=\"51\"> </TD>
         <TH>Temperatura2:</TH>
         <TD id=\"dyn1\">%1</TD>         
      </TR>
      <TR> 
    <TD> <img src=\"https://w7.pngwing.com/pngs/762/351/png-transparent-humidity-computer-icons-climate-moisture-humid-angle-cloud-meteorology.png\" alt=\"Humedad\" align=\"center\" width=\"92\" height=\"51\"></TD>
         <TH>Humedad:</TH>
         <TD id=\"dyn2\">%2</TD>
      </TR>      
      <TR>
    <TD> <img src=\"https://w7.pngwing.com/pngs/493/958/png-transparent-symbol-logo-computer-icons-time-symbol-miscellaneous-angle-logo.png\" alt=\"Tiempo\" align=\"center\" width=\"92\" height=\"51\"> </TD>
    <TH> Fecha: </TH>
    <TD id=\"HoraActual\"> </TD>
      </TR>
   </TABLE>


   <BR/>
</div>


</body>

<script type=\"text/javascript\">
/////////////////////////////////////////////////////////////////////////
////                                                                 ////
////  AJAX routines for rx/tx of data without having to reload page. ////
////                                                                 ////
////  Written by CCS, Inc.                    http://www.ccsinfo.com ////
////                                                                 ////
////        (C) Copyright 1996,2013 Custom Computer Services         ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.                                                     ////
/////////////////////////////////////////////////////////////////////////
var ajaxGet;
var t;
var waiting = 0;
var tValid = 0;
var nextCgi = \"\";
var lastCgi = \"\";
var ios6workaround = 0;

function setStartTime()
{
   tValid = 1;
   t = setTimeout(\'ajax()\', 500);
}

function ajax()
{
   if (waiting)
   {
      waiting--;
      if (waiting == 0)
      {
         if (lastCgi != \"\")
         {
            if (nextCgi == \"\")
               nextCgi = lastCgi;
            else
               nextCgi = lastCgi + \"&\" + nextCgi;
         }
      }
   }

   if (!waiting)
   {
      ajaxGet=GetXmlHttpObject();

      if (ajaxGet==null)
      {
         alert (\"Your browser does not support AJAX!\");
         return;
      }

      waiting = 5;  //wait 2.5 seconds for a response

      ajaxGet.open(\"POST\", \"index.xml\", true);

      ajaxGet.onreadystatechange = stateChanged;

      if (nextCgi != \"\")
      {
         ajaxGet.send(nextCgi);
         lastCgi = nextCgi;
         nextCgi = \"\";
      }
      else
      {
         ajaxGet.send(\"IGNOREDTAG=\"+ios6workaround);

         if (++ios6workaround > 255)
            ios6workaround = 0;
      }
   }

   setStartTime();
}

function sendCgi(cmd, value)
{
   if (nextCgi != \"\")
      nextCgi += \"&\";

   nextCgi += cmd + \"=\" + value;
}

function GetElementNodeValue(xmlDoc, id)
{
   return xmlDoc.getElementsByTagName(id)[0].childNodes[0].nodeValue;
}

function GetElementAttributeValue(xmlDoc, element, attribute)
{
   var n;
   var i;

   n = xmlDoc.getElementsByTagName(element)[0].attributes.length;

   for (i=0; i<n; i++)
   {
      if (xmlDoc.getElementsByTagName(element)[0].attributes[i].name == attribute)
      {
         return xmlDoc.getElementsByTagName(element)[0].attributes[i].value;
      }
   }

   return "";
}

function UpdateExistingTable(xmlDoc)
{
   var i = 0;

   for(;;)
   {
      var id = \"dyn\"+i;
      i++;
      if (document.getElementById(id))
      {         
         document.getElementById(id).innerHTML = GetElementNodeValue(xmlDoc, id);
      }
      else{
         break;
      }
   }
}

function stateChanged()
{
   if (ajaxGet.readyState==4)
   {
      UpdateExistingTable(ajaxGet.responseXML.documentElement);

      waiting = 0;
  }
}

function GetXmlHttpObject()
{
   if (window.XMLHttpRequest)
   {
      // code for IE7+, Firefox, Chrome, Opera, Safari
      return new XMLHttpRequest();
   }
   if (window.ActiveXObject)
   {
      // code for IE6, IE5
      return new ActiveXObject(\"Microsoft.XMLHTTP\");
   }
   return null;
}

showTime();

function showTime(){
   myDate = new Date();
   //document.write(myDate);
   //myDate.toUTCString();
   //document.getElementById(\"HoraActual\").innerHTML = myDate.toUTCString();
   hours = myDate.getHours();
   minutes = myDate.getMinutes();
   seconds = myDate.getSeconds();
   if (hours < 10) hours = 0 + hours;
   if (minutes < 10) minutes = \"0\" + minutes;
   if (seconds < 10) seconds = \"0\" + seconds;
   //$(\"#HoraActual\").text(hours+ \":\" +minutes+ \":\" +seconds);
   document.getElementById(\"HoraActual\").innerHTML=myDate.toUTCString();
   setTimeout(\"showTime()\", 1000);   
   setTimeout(\"UpdateExistingTable()\", 1000);  

}

</script>

</html>
";
