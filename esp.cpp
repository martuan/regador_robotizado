/*
  El programa se encarga de enviar por MQTT los datos 
  recibidos por el puerto serie
  
  Programado por: 
  Fecha:
  Carrera:
  Versión:
  
*/
#include <Arduino.h>

#include <WiFi.h>
#include <PubSubClient.h>
//#include "DHTesp.h"
#include <stdlib.h>

#define LED_BUILTIN 2 //para ESP32
//#define LED_BUILTIN 13 //para Arduino UNO


char inChar = ' ';
String maceta = "";

//const char* ssid = "infiniem";//nombre_de_mi_red
//const char* password =  "12345678";//pass_de_mi_red
const char* ssid = "infiniem";
const char* password =  "12345678";
const char* mqttServer = "broker.hivemq.com";//test.mosquitto.org
const int mqttPort = 1883;
const char* mqttUser = "";
const char* mqttPassword = "";
  
WiFiClient espClient;
PubSubClient client(espClient);


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);

//**********INICIALIZACIÓN WIFI**********
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Conectando a red WiFi...");
  }
  
  Serial.println("Conectado a la red WiFi");
//****************************************

//**********INICIALIZACIÓN MQTT**********
  client.setServer(mqttServer, mqttPort);
  
  while (!client.connected()) {
    Serial.println("Conectando a Broker MQTT...");
  
    if (client.connect("IOT-ESP32", mqttUser, mqttPassword )) {
  
      Serial.println("conectado");
  
    } else {
  
      Serial.print("conexion fallida ");
      Serial.print(client.state());
      delay(2000);
  
    }
  }
//*****************************************

}

// the loop function runs over and over again forever
void loop() {


  //Chequea si existe algún byte en el puerto serie
  while (Serial.available() > 0) { //primera lectura, el numero de la maceta

    inChar = Serial.read();//lee el byte del puerto serie
    Serial.println(inChar);
    if (inChar != '\n'){

      maceta += inChar;//arma el string
      
    }else{
      
      Serial.println(maceta);
      //client.publish("unsam/test2021/hum", str.c_str());//envía por mqtt un aviso
      //client.publish("unsam/test2021/hum", "prueba2");//envía por mqtt un aviso

      //Intento de topico variable (error en publish)
      String maceta;
      String path = "unsam/test2021/hum/" ;
      String top;
      top = path+maceta;
      Serial.println(top);

      String doc="prueba";

      void publish();


      /*
      switch(str[0]){
        case '1':
          //Serial.println("Maximo ^ ^ ^ ^");
          digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
          client.publish("unsam/test2021/hum/1", "prueba");//envía por mqtt un aviso
        
        break;
        case '2':
          //Serial.println("Minimo . . . .");
          digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
          client.publish("unsam/test2021/hum/2", "prueba");//envía por mqtt un aviso
        
        break;
        case '3':
          //Serial.println("Minimo . . . .");
          digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
          client.publish("unsam/test2021/hum/3", "prueba");//envía por mqtt un aviso
        
        break;
        case '4':
          //Serial.println("Minimo . . . .");
          digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
          client.publish("unsam/test2021/hum/4", "prueba");//envía por mqtt un aviso
        
        break;
        case '5':
          //Serial.println("Minimo . . . .");
          digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
          client.publish("unsam/test2021/hum/5", "prueba");//envía por mqtt un aviso
        
        break;
        
        default:
          Serial.print("valor leido: ");
          Serial.println(str);//imprime por puerto serie el string armado
 
        break;
        
      }
      
      */
      
        maceta = "";//limpia el string
        break;
      
    }

  }

    maceta = "";//limpia el string

  client.loop();

  delay(1000);
  
}

char* toCharArray(String str) {
  return &str[0];
}

void publish(String doc, String top) {
  char Buf[50];
  client.publish(toCharArray(top), toCharArray(Buf));
}