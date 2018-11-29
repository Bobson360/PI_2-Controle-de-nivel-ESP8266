#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>         
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>   
#include <FirebaseArduino.h>
#include "Temperature.h"
#include "Ultrassonic.h"
#include "Volume.h"

#define Temp A0
#define Trig D2
#define Echo D1
#define rele_1  D3
#define rele_2  D4
#define wifi_pin D6
#define fireBase_pin D5
//
#define radius 7.5
#define height 21


#define FIREBASE_HOST "nodemcu-pi.firebaseio.com"
#define FIREBASE_AUTH "EkzjUR8gQdGfWnqJ23B649t9DeLPHD3oH8NhCpYa"

#define _SSID "Robson_Eng_Comp"
#define _PASSWORD "senha123"

#define udp_link "a.st1.ntp.br"
#define fuso -2




float level_max = 2.5;
float level_min = 1.0;

int cycle;
int count_cycle = 0;

float newVolume;
float newTemp;
bool  bomba;
bool  lock; 


int distance;

/********** functions declaration **********/
void configPin();
void pump(int i);

Temperature temperature( Temp );
Ultrassonic ultrassonic( Trig, Echo );
Volume volume( radius, height );

WiFiUDP ntpUDP;
NTPClient timeClient( ntpUDP, udp_link, fuso * 3600, 60000 );//Cria um objeto "NTP" com as configurações.


void setup() {

   temperature.initialize();
   ultrassonic.initialize();
      
  configPin();
  
  WiFiManager wifiManager;
  wifiManager.autoConnect( _SSID, _PASSWORD );
  
  Serial.begin(9600);
    
  Firebase.begin( FIREBASE_HOST, FIREBASE_AUTH );
  
  timeClient.begin();
  timeClient.update();
  Serial.println( timeClient.getFormattedTime() );
  delay(100);
}

void loop() {

  if( WiFi.status() == WL_CONNECTED ){
      digitalWrite( wifi_pin, HIGH );
  }else{
      digitalWrite( wifi_pin, LOW );
      digitalWrite( fireBase_pin, LOW );
  }

  if( Firebase.getBool("connection" ) ){
      digitalWrite( fireBase_pin, HIGH );
  }else{
      digitalWrite( fireBase_pin, LOW );
  }
  
    timeClient.update();
 
    newVolume = 0;
    
//    for( int i = 0; i < count_cycle; i++ ){   
        newVolume = volume.vol(ultrassonic.distance());
        newTemp = temperature.celcius();
        distance = ultrassonic.distance();
//        delay(50);
//    }

//      Serial.println( "" );
//      Serial.println( "======================" );
//      Serial.print( "Distance: " );
//      Serial.print( ultrassonic.distance() );
//      Serial.println( " cm" );
//      Serial.print( "Volume: " );
//      Serial.print( volume.vol(ultrassonic.distance()) );
//      Serial.println( " litros" );
//      Serial.println( "======================" );
//      Serial.println( "" );
    
//    newVolume /= count_cycle;
//    newTemp   /= count_cycle;
//    distance  /= count_cycle;
    pump(cycle);

    String volume      = Firebase.pushInt   ( "Volume",  newVolume );
    String temperatura = Firebase.pushInt   ( "Temperatura", int( newTemp ));
    String hora        = Firebase.pushString( "Hora", timeClient.getFormattedTime() );
           bomba       = Firebase.getBool   ( "Bomba" );
           lock        = Firebase.getBool   ( "lock" );

    
}


void pump(int i){

//  if( !lock ){
//          Serial.println( "" );
//          Serial.println( "======================" );
//          Serial.println("LOCK OFF");
//          Serial.println( "======================" );
//          Serial.println( "" );
          digitalWrite( rele_1, LOW );
    if(  distance  >= 10 ){
          digitalWrite( rele_2, HIGH );
          Firebase.setBool( "Bomba", true );
          Serial.println( "" );
          Serial.println( "======================" );
          Serial.println( "Bomba ligada" );
          Serial.println( "Firebase bomba: " );
//          Serial.println( Firebase.getBool   ( "Bomba" ));
          Serial.println( "======================" );
          Serial.println( "" );
          
    }else if(  distance  <= 5 ){
          digitalWrite( rele_2, LOW );
          Firebase.setBool( "Bomba", false );
          Serial.println( "Bomba desligada" );
          Serial.print( "Firebase bomba: " );
//          Serial.println( Firebase.getBool   ( "Bomba" ) );
//    }else if( distance > 5 ){
//          if( bomba ){
//              digitalWrite( rele_2, HIGH );
//          }else{
//              digitalWrite( rele_2, LOW );
//          }
          
    }
      
    

//  }else{
//        Serial.println( "" );
//        Serial.println( "======================" );
//        Serial.println("LOCK ON");
//        Serial.println( "======================" );
//        Serial.println( "" );
//        digitalWrite( rele_1, HIGH );
//        digitalWrite( rele_2, LOW );
//  }
}

void configPin(){
  
    pinMode(rele_1,         OUTPUT);
    pinMode(rele_2,         OUTPUT);
    pinMode(wifi_pin,       OUTPUT);
    pinMode(fireBase_pin,   OUTPUT);

    digitalWrite(rele_1,       LOW);
    digitalWrite(rele_2,       LOW);
    digitalWrite(wifi_pin,     LOW);
    digitalWrite(fireBase_pin, LOW);
}
