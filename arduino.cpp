#include <Arduino.h>
/*
  El programa se encarga de enviar por el puerto serie
  un número que va ascendiendo 
  
  Programado por: 
  Fecha:
  Carrera:
  Versión:
  
*/

void setup() {
  Serial.begin(9600);

}

int i=0;

void loop() {
  Serial.println(i++);
  delay(2000); // poll every 100ms
}