//PROYECTO MATERIA INTEGRADORA
//COMPARACION DE DOS TECNICAS DE CONTROL
//HUERTO DOMESTICOS


//LIBRERIAS
#include <avr/wdt.h>
//DHT11
#include <DHT.h>
#include <DHT_U.h>
#include "DHT.h"

//EHTERNET SHIELD
#include <SPI.h>
#include <Ethernet.h>

#include <NTPClient.h> 
#include <EthernetUdp.h>

#include <time.h>

//THINGSPEAK
//#include "secrets.h"
#include "ThingSpeak.h" 
//
#define DHTTYPE DHT11


//PINES
int fc28D1=48;
int fc28D2=50;
int fc28D3=52;
char fc28A1=A0;
char fc28A2=A1;
char fc28A3=A2;

//int dht11_1=32;
int dht11_1=34;
//int dht11_3=36;

int pump1=5;
int pump2=6;
int pump3=7;

//ESTRUCTURAS CON LAS VARIABLES GLOBALES 

//valores leidos de los sensores, parametros
struct Parametros
{
  float humedadsuelo1=0;
  bool humground1=0;
  float humedadsuelo2=0;
  bool humground2=0;
  float humedadsuelo3=0;
  bool humground3=0;
  float humedadaire1=0;
  float temperaturaire1=0;
  float senscalor1=0;

};
// valores de referencia dados
struct Referencias
{
  int refHumedadsuelo=0;
  int refHumedadaire=0;
  int refTemperaturaire=0;
  int refSensacioncalor=0;
  int riegoManual=0;
  int ventManual=0;
};

// variables del controlador
struct Controles
{
  int error=0;
  unsigned long currentTime=0;
  unsigned long previousTime=0;
  double elapsedTime=0;
  double lastError=0;
  double cumError=0;
  double rateError=0;  
};

// DECLARACION DE ESTRUCTURAS

Parametros PARAMETRO;
Parametros PROMEDIO;
Parametros SUMA;
Referencias REFERENCIA;
Controles RIEGO;
Controles VENTIL;
//
//VARIABLES DE CONTROLADOR

int DatosHum[100];
int DatosSdC[100];


//........................................................

//VARIABLES THINGSPEAK

//Canal 1 PID ProyectoIntegradorAB2021
unsigned long canalID1 = 1407484;
const char * myWriteAPIKey1 = "YTAFNNI0Z3QZT9KN";
const char * myCounterReadAPIKey1 = "1SP7WVJWYR7181FG";

//Canal 2 DIFUSO ProyectoIntegradorAB2021
unsigned long canalID2 = 1424030;
const char * myWriteAPIKey2 = "R89XH91FDMFO0DE4";
const char * myCounterReadAPIKey2 = "R7YPPUSJF7BLZRYR";

//........................................................

EthernetClient clientTS;
EthernetClient clientTZ;

int countTS=1;
int countLoopTS=0;
int TSskill=100;

float bomba;
float venti;
float RPM;
String HoraLocal;
int minuto;
int hora;
int segundo;
String HoraLocalactualizacion;
String ActualizacionHora;
String Thing1up;
String Thing2up;
String Bombup;
String Ventup;
float minpump;
      
// INICIO DE DHT
DHT dht1(dht11_1,DHTTYPE);
//DHT dht2(dht11_2,DHTTYPE);
//DHT dht3(dht11_3,DHTTYPE);

//ETHERNET SHIELD
byte mac[]={0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED}; //MAC
IPAddress ip(192,168,100,121); //IP
IPAddress myDns(192, 168, 100, 1);
EthernetServer servidor(80);

EthernetUDP UdpNtpTIME;
NTPClient timeClient(UdpNtpTIME,"ec.pool.ntp.org",-18000,6000);

// *********  VOID SETUP ******************************************************
// ****************************************************************************
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("INICIO 1");
  wdt_disable();
  pinMode(fc28D1,INPUT); //FC28 DIGITAL 1
  pinMode(fc28D2,INPUT);// FC 28 DIGITAL 2
  pinMode(fc28D3,INPUT);// FC 28 DIGITAL 3
  pinMode(dht11_1,INPUT);// DHT11 DIGITAL 1
  //pinMode(dht11_2,INPUT);// DHT11 DIGITAL 2
  //pinMode(dht11_3,INPUT);// DHT11 DIGITAL 3
  pinMode(pump1,OUTPUT);//PWM BOMBA 1
  pinMode(pump2,OUTPUT);//PWM BOMBA 2
  pinMode(pump3,OUTPUT);//PWM BOMBA 3

  Serial.println("INICIO 2");
// INICIO DE LA LIBRERIA DEL SENSOR
  dht1.begin();
  //dht2.begin();
  //dht3.begin();

  Serial.println("INICIO 3");

  //INICIO DE COMPONENTES DE RED
  Ethernet.begin(mac,ip,myDns);
  servidor.begin();
  timeClient.begin();
  Serial.println("INICIO 4");
  
  byte macBuffer[6];  // create a buffer to hold the MAC address
  Ethernet.MACAddress(macBuffer); // fill the buffer
  Serial.println("INICIO 5");
  Serial.print("The MAC address is: ");
  for (byte octet = 0; octet < 6; octet++) {
    Serial.print(macBuffer[octet], HEX);
    if (octet < 5) {
      Serial.print('-');
    }
  }
  delay(500);
  ThingSpeak.begin(clientTS);  // Initialize ThingSpeak
  //ThingSpeak.begin(clientTZ);  // Initialize ThingSpeak
/*
  while(!timeClient.update()) {
      Serial.println("Hora update");
      timeClient.forceUpdate();
      Serial.println("Hora force update");
    }*/
  if(timeClient.update()) {
      Serial.println("Hora update");
      HoraLocalactualizacion=timeClient.getFormattedTime();
      ActualizacionHora="SI actualizo";
    }
    else{
      HoraLocalactualizacion=HoraLocal;
      Serial.println("NO Hora update");
      ActualizacionHora="NO Actualizo";
    } 
     HoraLocal=timeClient.getFormattedTime();
     LECTORSENSORES();
     PROMEDIOFUN();
     THINGSPEAK();

    REFERENCIA.refHumedadsuelo=60;
    
    HoraLocal=timeClient.getFormattedTime();
    Serial.println(hora=timeClient.getHours());
    Serial.println(minuto=timeClient.getMinutes());
    Serial.println(segundo=timeClient.getSeconds());
    Serial.println("Hora Actualizada");  
    Serial.println(" Fin Setup");
}

// *********  VOID LOOP ******************************************************
// ****************************************************************************
void loop() {

 
  Serial.println("Loop1");
  EthernetClient cliente= servidor.available();

  Serial.println(HoraLocal);//timeClient.getFormattedTime());
  Serial.println("Loop15");
  
  LECTORSENSORES();
  
  //IMPRESIONSERIAL(PARAMETRO);
  
  SERVIDOR(cliente,PARAMETRO,REFERENCIA);
  
  
  REFERENCIA=FORMULARIO(cliente);
  RPM=CONTROLADORPID();
  minpump=45.00;
  bomba=RPM+minpump;
  if(bomba>=minpump&&minuto%12==0){
  
    analogWrite(pump1,(bomba*255)/100);
  
  }
  else{
    
    analogWrite(pump1,(0*255)/100);
   }
    
    //analogWrite(pump1,(REFERENCIA.riegoManual*255)/100);
  //analogWrite(pump2,(REFERENCIA.ventManual*255)/100);
  
  //CONTROLADOR DE ENVIO Y RECEPCION DE DATOS CON THNGSPEAK
  Serial.println("Loop2");
  if(minuto%15==0&&segundo<=1){         //(countTS>TSskill){//
    
      Serial.println("Thingspeak");

      PROMEDIOFUN();
      THINGSPEAK();
      
  }
  
  if(hora%2==0&&minuto==17&&segundo<3){ 
    
    if(timeClient.update()) {
      Serial.println("Hora update");
      HoraLocalactualizacion=timeClient.getFormattedTime();
      ActualizacionHora="SI actualizo";
    }
    else{
      HoraLocalactualizacion=HoraLocal;
      Serial.println("NO Hora update");
      ActualizacionHora="NO Actualizo";
    }
    
    HoraLocal=timeClient.getFormattedTime();
    //TiempoActual=timeClient
    delay(200);
    //UdpNtpTIME.close();
  }
  //.................................
  /*
  if(hora%3==0&&minuto==5&&segundo<15){//(countIDENT>=4 &&countTS<200){hora%1==0&&
    bomba=(70*255)/100;
    Bombup=HoraLocal;
    analogWrite(pump1,bomba);//(60*255)/100);
    Serial.println("pump1 on ");Serial.println(bomba);
    
    delay(400);
    
  }
  else{
    bomba=0;
    analogWrite(pump1,bomba);//(0*255)/100);
    
    Serial.print("pump1 off ");Serial.println(bomba);
  }
  Serial.println("Loop3");
//........................................................
  if(hora%2==0&&minuto>0&&minuto<=10){//(countIDENTvent<8){
    venti=(65*255)/100;
    analogWrite(pump2,venti);
    Serial.print("vent on ");Serial.println(venti); 
  }
  else if(hora%2==0&&minuto>10&&minuto<=20){//(countIDENTvent<8){
    venti=(90*255)/100;
    analogWrite(pump2,venti);
    Serial.print("vent on ");Serial.println(venti); 
  }
  else if(hora%2==0&&minuto>20&&minuto<=30){//(countIDENTvent<8){
    venti=(99*255)/100;
    analogWrite(pump2,venti);
    Serial.print("vent on ");Serial.println(venti); 
  }
  else if(hora%2==0&&minuto>30&&minuto<=40){//(countIDENTvent<8){
    venti=(99*255)/100;
    analogWrite(pump2,venti);
    Serial.print("vent on ");Serial.println(venti); 
  }
  else if(hora%2==0&&minuto>40&&minuto<=50){//(countIDENTvent<8){
    venti=(90*255)/100;
    analogWrite(pump2,venti);
    Serial.print("vent on ");Serial.println(venti); 
  }
  else if(hora%2==0&&minuto>50&&minuto<=59){//(countIDENTvent<8){
    venti=(65*255)/100;
    analogWrite(pump2,venti);
    Serial.print("vent on ");Serial.println(venti); 
  }
  else{
    venti=0;
    analogWrite(pump2,venti);//(0*255)/100);
    Serial.print("vent off ");Serial.println(venti);
  }*/
  //.................................

  //if(countTS%3==0){
    HoraLocal=timeClient.getFormattedTime();
    Serial.println(hora=timeClient.getHours());
    Serial.println(minuto=timeClient.getMinutes());
    Serial.println(segundo=timeClient.getSeconds());
    Serial.println("Hora Actualizada");  
//}
  //SERIALPROMEDIO();
  countTS++;
  Serial.println(countTS);

  Ethernet.maintain();
  delay (200);
  Serial.println("Loop4");
}
// ****************************************************************************
// ****************************************************************************
void LECTORSENSORES(){

  
  PARAMETRO.humedadsuelo1=(1000-analogRead(fc28A1));
  SUMA.humedadsuelo1+=PARAMETRO.humedadsuelo1;
  PARAMETRO.humedadsuelo2=(1000-analogRead(fc28A2));
  SUMA.humedadsuelo2+=PARAMETRO.humedadsuelo2;
  PARAMETRO.humedadsuelo3=(1000-analogRead(fc28A3));
  SUMA.humedadsuelo3+=PARAMETRO.humedadsuelo3;
 /* 
  PARAMETRO.humground1=digitalRead(fc28D1);
  PARAMETRO.humground2=digitalRead(fc28D2);
  PARAMETRO.humground2=digitalRead(fc28D3);
 */ 
  PARAMETRO.temperaturaire1=dht1.readTemperature();
  SUMA.temperaturaire1+=PARAMETRO.temperaturaire1;
  PARAMETRO.humedadaire1=dht1.readHumidity();
  SUMA.humedadaire1+=PARAMETRO.humedadaire1;
  PARAMETRO.senscalor1=dht1.computeHeatIndex(PARAMETRO.temperaturaire1, PARAMETRO.humedadaire1, false);
  SUMA.senscalor1+=PARAMETRO.senscalor1;
/*
  PARAMETRO.temperaturaire2=dht2.readTemperature();
  SUMA.temperaturaire2+=PARAMETRO.temperaturaire2;
  PARAMETRO.humedadaire2=dht2.readHumidity();
  SUMA.humedadaire2+=PARAMETRO.humedadaire2;
  PARAMETRO.senscalor2=dht2.computeHeatIndex(PARAMETRO.temperaturaire2, PARAMETRO.humedadaire2, false);
  SUMA.senscalor2+=PARAMETRO.senscalor2;

  PARAMETRO.temperaturaire3=dht3.readTemperature();
  SUMA.temperaturaire3+=PARAMETRO.temperaturaire3;
  PARAMETRO.humedadaire3=dht3.readHumidity();
  SUMA.humedadaire3+=PARAMETRO.humedadaire3;
  PARAMETRO.senscalor3=dht3.computeHeatIndex(PARAMETRO.temperaturaire3, PARAMETRO.humedadaire3, false);
  SUMA.senscalor3+=PARAMETRO.senscalor3;
*/  
}
void PROMEDIOFUN(){
    
    PROMEDIO.humedadsuelo1=SUMA.humedadsuelo1/countTS;
    PROMEDIO.humedadsuelo2=SUMA.humedadsuelo2/countTS;
    PROMEDIO.humedadsuelo3=SUMA.humedadsuelo3/countTS;

    PROMEDIO.temperaturaire1=SUMA.temperaturaire1/countTS;
    PROMEDIO.humedadaire1=SUMA.humedadaire1/countTS;
    PROMEDIO.senscalor1=SUMA.senscalor1/countTS;
/*
    PROMEDIO.temperaturaire2=SUMA.temperaturaire2/countTS;
    PROMEDIO.humedadaire2=SUMA.humedadaire2/countTS;
    PROMEDIO.senscalor2=SUMA.senscalor2/countTS;
    
    PROMEDIO.temperaturaire3=SUMA.temperaturaire3/countTS;
    PROMEDIO.humedadaire3=SUMA.humedadaire3/countTS;
    PROMEDIO.senscalor3=SUMA.senscalor3/countTS;
*/    
    SUMA.humedadsuelo1=0;
    SUMA.humedadsuelo2=0;
    SUMA.humedadsuelo3=0;

    SUMA.temperaturaire1=0;
    SUMA.humedadaire1=0;
    SUMA.senscalor1=0;
/*
    SUMA.temperaturaire2=0;
    SUMA.humedadaire2=0;
    SUMA.senscalor2=0;

    SUMA.temperaturaire3=0;
    SUMA.humedadaire3=0;
    SUMA.senscalor3=0;
*/  countLoopTS=countTS;
    countTS=1;
    
    Serial.print(countTS);
    Serial.println("PROMEDIADO");
  
  //return;
}
// ****************************************************************************
// ****************************************************************************
void IMPRESIONSERIAL(Parametros PARAMETRO){
  
  Serial.print("humedad suelo 1: ");
  Serial.println(PARAMETRO.humedadsuelo1);
  Serial.print("humedad suelo 2: ");
  Serial.println(PARAMETRO.humedadsuelo2);
  Serial.print("temperatura del aire:  ");
  Serial.println(PARAMETRO.temperaturaire1);
  Serial.print("humedad del aire:  ");
  Serial.println(PARAMETRO.humedadaire1);
  Serial.println("xxxxxxxxxxxxxxxxxxxxxxxxxx");
  
}

// ****************************************************************************
// ****************************************************************************
void SERVIDOR (EthernetClient cliente,Parametros PARAMETRO, Referencias REFERENCIA){
  Serial.println("Servidor Inicia");
  
  cliente.println("HTTP/1.1 200 OK");
  cliente.println("Content-Type: text/html;charset=utf-8");
  cliente.println();        
          //Página Web en HTML
  cliente.println("<html>");
  cliente.println("<head>");
  cliente.println("<title>PID CONTROL HUERTO</title>");
  cliente.println("</head>");
  cliente.println("<body width=100% height=100%>");
  cliente.println("<center>");
  cliente.println("<h1>Alejandro Daniel Bermúdez Rodríguez</h1>");
  //cliente.println("<h1>SUPERMEGAHIPERMAGNO HUERTO</h1>");
  cliente.println("<h2>Proyecto Materia Integradora - ESPOL - FIEC - 2021</h2>");
  cliente.println("<h2>Monitoreo y Control de Huerto Automatizado </h2>");
  cliente.println("<h2>Control PID</h2>");
  cliente.println("<h5>Hora local: ");
  cliente.println(HoraLocal);
  cliente.println("</center>");
  cliente.print("<br>");
  
  cliente.print("<h4>MONITOREO</h4>");
  cliente.print("<h5>Humedad del Suelo 1:*____");
  cliente.print(PARAMETRO.humedadsuelo1);
  cliente.print("<h5>Humedad del Suelo 2:____");
  cliente.print(PARAMETRO.humedadsuelo2);
  cliente.print("<h5>Humedad del Suelo 3:_____");
  cliente.print(PARAMETRO.humedadsuelo3);
  cliente.print("<h5>Temperatura Aire 1:_____");
  cliente.print(PARAMETRO.temperaturaire1);
  /*
  cliente.print("<h5>Temperatura Aire 2:_____");
  cliente.print(PARAMETRO.temperaturaire2);
  cliente.print("<h5>Temperatura Aire 3:_____");
  cliente.print(PARAMETRO.temperaturaire3);
  */
  cliente.print("<h5>Humedad Aire 1:________");
  cliente.print(PARAMETRO.humedadaire1);
  
  cliente.print("<h5>Sensacion Calor 1:________");
  cliente.print(PARAMETRO.senscalor1);
  /*
  cliente.print("<h5>Humedad Aire 2:________");
  cliente.print(PARAMETRO.humedadaire2);
  cliente.print("<h5>Humedad Aire 3:________");
  cliente.print(PARAMETRO.humedadaire3);
  */
  cliente.print("<br>");
  cliente.print("<br><br>");
  cliente.print("<h4>DATOS DE ACTUADORES</h4>");
  cliente.print("<h4>RIEGO");
  cliente.print("<h4>Hore último riego: .");
  cliente.print(Bombup);
  cliente.print("Potencia de Riego: .");
  cliente.print(bomba);
  cliente.print("<h4>VENTILACIÓN: .");
  cliente.print(venti);
  cliente.print("<h4>Última actialización de hora: .");
  cliente.print(HoraLocalactualizacion);
  cliente.print(". . ");
  cliente.print(ActualizacionHora);
  cliente.print("<h4>Estado de Datos: .");
  cliente.print(Thing1up);
  cliente.print("<h4>Estado de Loop: .");
  cliente.print(countLoopTS);
  cliente.print("<h4>Error: .");
  cliente.print(RIEGO.error);
  cliente.print("<h4>CONTROLADOR .");
  cliente.print(RPM);
  cliente.print("<br>");
  cliente.print("<br><br>");
  cliente.print("<br>");
  cliente.print("<br><br>");
  
  cliente.print("<h4>DATOS REFERENCIA</h4>");
  cliente.print("<form action='/' method='get'>");
  cliente.print("<h5>");
  cliente.println("<input type=number name=dato1 style=width:80px;height:40px>"); //"<?php echo >">"); //ondragenter=location.href=dato1>");
  cliente.print("Control Manual de Bomba de Riego:__");
  cliente.print(REFERENCIA.riegoManual);
  cliente.print("<br>");
  cliente.println("<input type=number name=dato2 style=width:80px;height:40px>"); //ondragenter=location.href=dato2>");
  cliente.print("Control Manual de Ventilador______Referencia Actual:__");
  cliente.print(REFERENCIA.ventManual);
  cliente.print("<br>");
  cliente.println("<input type=number name=dato3 style=width:80px;height:40px>"); //"<?php echo >">"); //ondragenter=location.href=dato1>");
  cliente.print("Humedad del Suelo___Referencia Actual:__");
  cliente.print(REFERENCIA.refHumedadsuelo);
  cliente.print("<br>");
  cliente.println("<input type=submit name=dato3 style=width:80px;height:40px>");
  cliente.print("<META http-equiv=refresh content=15>");
  cliente.println("</form>");
  cliente.print("<br><br>");  
 Serial.println("Servidor Fin");
}

// ****************************************************************************
// ****************************************************************************
Referencias FORMULARIO(EthernetClient cliente)
{
  Serial.println("Formulario Inicia");
  //Referencias REFERENCIA;
  //String HTTP_req.  w=[1e=.[ew[]d;X/S/`8rj ,-t  3-r[D[
  String readString;
  //EthernetClient cliente= servidor.available();
  boolean lineablanca=true;
  //Serial.println("clietne tragador");
  while (cliente.connected()) {
            //Serial.println("clietne while");
            if (cliente.available()) {   // client data available to read
                //Serial.println("clietne while if");
                char c = cliente.read(); // read 1 byte (character) from client
                
                if (readString.length() < 100) {
                  readString += c; 
                  delay(10);
                }
                else{
                  readString="";
                  cliente.stop();
                  Serial.println("cleinte cerrado1");
                  }
                
                if (c == '\n') {
                  
                  Serial.println("si linea vacia");
                  
                  if (readString.indexOf("dato1") >0){

                    int posiciona = readString.indexOf("=");
                    int n1 = ((readString[posiciona+1] -48) * 10) + readString[posiciona+2] -48;
                    REFERENCIA.riegoManual=n1;
                    Serial.print("valor de n1 ");
                    Serial.println(n1);
                  }
                  
                  if (readString.indexOf("dato2") >0){

                    int posicionb = readString.indexOf("dato2=")+5;
                    int n2 = ((readString[posicionb+1] -48) * 10) + readString[posicionb+2] -48;
                    REFERENCIA.ventManual=n2;
                    Serial.print("valor de n2 ");
                    Serial.println(n2);
                  }
                  
                  if (readString.indexOf("dato3") >0){

                    int posicionb = readString.indexOf("dato3=")+5;
                    int n3 = ((readString[posicionb+1] -48) * 10) + readString[posicionb+2] -48;
                    REFERENCIA.refHumedadsuelo=n3;
                    Serial.print("valor de n2 ");
                    Serial.println(n3);
                  }
                  //cliente.stop();
                 }
                
            }
            //delay(1000);
  }
  return REFERENCIA;
}

// ****************************************************************************
// ****************************************************************************
float CONTROLADORPID()
{
  float actuador;
  
  float proporcional;
  float integrador;
  float diferenciador;
  float Kp=0.075;
  float Ki=0.05;
  float Kd=0.03;
  
  RIEGO.currentTime = millis();                               // obtener el tiempo actual
  RIEGO.elapsedTime = (double)(RIEGO.currentTime - RIEGO.previousTime);     // calcular el tiempo transcurrido
        
  RIEGO.error=((REFERENCIA.refHumedadsuelo*10)-PARAMETRO.humedadsuelo1)/10;
  if(RIEGO.error<0){RIEGO.cumError=0;}
  Serial.print("referencia=");
  Serial.print(REFERENCIA.refHumedadsuelo);
  Serial.print("humedad=");
  Serial.println(PARAMETRO.humedadsuelo1); 
  
  Serial.print("pid Error=");
  Serial.println(RIEGO.error);
  Serial.print("pied ElapsedTime=");
  Serial.println(RIEGO.elapsedTime);                             
  RIEGO.cumError += RIEGO.error ;//* RIEGO.elapsedTime;                      // calcular la integral del error
  RIEGO.rateError = (RIEGO.error - RIEGO.lastError);// / RIEGO.elapsedTime;         // calcular la derivada del error
 
  proporcional=Kp*RIEGO.error;
  
  Serial.print("Proporcional=");
  Serial.println(proporcional);
  
  integrador=Ki*RIEGO.cumError;
  
  Serial.print("Integrador=");
  Serial.println(integrador);
  
  diferenciador=Kd*RIEGO.rateError;
  
  Serial.print("Derivativo=");
  Serial.println(diferenciador);
  
  actuador=proporcional+integrador+diferenciador;
  
  Serial.print("Controlador=");
  Serial.println(actuador);
  
  RIEGO.lastError = RIEGO.error;                                      // almacenar error anterior
  RIEGO.previousTime = RIEGO.currentTime;
  
  return actuador;
}
// ****************************************************************************
// ****************************************************************************
void THINGSPEAK(){
  
  Serial.println("ThingSepak Inicia");
  float temperaturapromedio;
  float humedadairepromedio;
  int contador=0;
  int x=0;
  
  do{
    delay(200);
    ThingSpeak.setField(1, PROMEDIO.humedadsuelo1);
    delay(200);
    ThingSpeak.setField(2, PROMEDIO.humedadsuelo2);
    delay(200);
    ThingSpeak.setField(3, PROMEDIO.humedadsuelo3);
    delay(200);
    ThingSpeak.setField(4, PROMEDIO.temperaturaire1);
    delay(200);
    ThingSpeak.setField(5,PROMEDIO.humedadaire1);
    delay(200);
    ThingSpeak.setField(6, PROMEDIO.senscalor1);
    delay(200);
    ThingSpeak.setField(7, RIEGO.error);
    delay(200);
    ThingSpeak.setField(8, venti);
    delay(200);
    
    x = ThingSpeak.writeFields(canalID1, myWriteAPIKey1);
    if(x == 200){
      Serial.println("Channel update successful.");
      Thing1up="Actualizado Thingspeak Parámetros ._"+HoraLocal;
      contador==0;
      delay(3000);
    }
    else{
      Serial.println("Problem updating channel. HTTP error code " + String(x));
      Thing1up=HoraLocal+String(x); 
      contador++;
      delay(15000);
     }
     if(contador==3){
      wdt_enable(WDTO_15MS);
      while(1){Serial.print("x");};
       x=200;
       contador=0;
     }
     delay(500);
  }while(x != 200);
  
  delay(1000);
  Serial.println("Thingspeak Fin");
}
/*
void THINGSPEAK2(){
  int contador;
  int x;
Serial.println("ThingSepak 2 Inicia");
  delay(400);
  ThingSpeak.setField(5,bomba);
  delay(400);
  ThingSpeak.setField(6,venti);
  delay(400);
    do{
      x = ThingSpeak.writeFields(canalID1, myWriteAPIKey1);
     if(x == 200){
      Serial.println("Channel update successful.");
      Thing1up="Actualizado Thingspeak Parámetros";
      contador==0;
     }
     else{
      Serial.println("Problem updating channel. HTTP error code " + String(x));
      Thing1up=HoraLocal+String(x); 
      contador++;
     }
     
  }while(x != 200&&contador<4);  
  delay(1000);
  Serial.println("Thingspeak 2 Fin");
}
*/
/*
  int x = ThingSpeak.writeFields(canalID1, myWriteAPIKey1);
  if(x == 200){
    Serial.println("Channel update successful2.");
    Thing2up="Actualizado Bomba Thingspeak Parámetros";
  }
  else{
    Serial.println("Problem updating channel. HTTP error code 2 " + String(x));
    Thing2up=HoraLocal;
  }

}

void THINGSPEAK2(){
  Serial.println("ThingSepak 2 Inicia");
  delay(400);
  ThingSpeak.setField(1,RIEGO.error );
  delay(400);
  ThingSpeak.setField(2,VENTIL.error);
  delay(400);
  ThingSpeak.setField(3,REFERENCIA.refHumedadsuelo);
  delay(400);
  ThingSpeak.setField(4,REFERENCIA.refSensacioncalor);
  delay(400);
  ThingSpeak.setField(5,bomba );
  delay(400);
  ThingSpeak.setField(6,venti);
  delay(400);

  int x = ThingSpeak.writeFields(canalID3, myWriteAPIKey3);
  if(x == 200){
    Serial.println("Channel update successful2.");
    Thing2up="Actualizado Thingspeak Parámetros";
  }
  else{
    Serial.println("Problem updating channel. HTTP error code 2 " + String(x));
    Thing2up=HoraLocal;
  }
  delay(1000);
  Serial.println("Thingspeak 2 Fin");
}
*/
void SERIALPROMEDIO(){
  
  Serial.println("PROMEDIACION");
  Serial.println(PROMEDIO.humedadsuelo1);
  Serial.println(PROMEDIO.humedadsuelo2);
  Serial.println(PROMEDIO.humedadsuelo3);
  Serial.println(PROMEDIO.temperaturaire1);
//  Serial.println(PROMEDIO.temperaturaire2);
//Serial.println(PROMEDIO.temperaturaire3);
  Serial.println(PROMEDIO.humedadaire1);
 // Serial.println(PROMEDIO.humedadaire2);
 // Serial.println(PROMEDIO.humedadaire3);
  Serial.println(countTS);
  Serial.println("PROMEDIACION FIN");
}
