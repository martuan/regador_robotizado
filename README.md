# regador_robotizado
Regador robotizado para incorporar al laboratorio inteligente de la FIE

/****************MODIFICAR TODO LO QUE SIGUE PARA ADAPTARLO A ESTE PROYECTO***************/

# Regador robotizado
Repositorio técnico del proyecto, con toda la información referida al hardware documentación de las tecnologías utilizadas. El mismo contiene las actualizaciones del firmware y el conexionado.

## Conexionado
EL sketch de Fritzing se encuentra en la carpeta del mismo nombre, dentro de /docs. Ahí también hay una carpeta *parts* cuyo contenido son componentes que se deben agregar manualmente al proyecto. Éstos se copian dentro de la carpeta que Fritzing nos asigna para guardar componentes, generalmente es *Documentos/Fritzing/parts*.

Para ver el [pinout de la ESP32](/docs/ESP32.md)

PINES ARDUINO

SHIELD/ARDUINO
| Componente | Pin Shield | Pin Arduino |
| -------------- | --------- | --------- |
| IN 1 Relé | SpindleDir | 13 |
| IN 2 | SpindleEna | 12 |
| PCB final de carrera Salida 1 | X- y GND | 9 y GND |
| PCB final de carrera Salida 2 | Z- y GND | 10 y GND |
| Botón  | Y- y GND | 10 y GND |
| Sensor H | Coolant Enable | A3 |

CONEXIONADO PCB

PCB OPTO
| Componente | Pin PCB |
| -------------- | --------- |
| Final de carrera X | Entrada 1 senal |
| Z- Shield | Salida 1 senal |
| Final de carrera Y | Entrada 2 senal|
| X- Shield | Salida 2 senal |
| Sensor | Salida 5V 2  |
| Fuente | Entrada 12 |

CABLEADO CARRO

DB9
| PIN | Componente |
| -------------- | --------- |
| 1 | PaP Negro |
| 2 | PaP Verde |
| 3 | PaP Rojo|
| 4 | PaP Azul |
| 5 | Final de carrera  |
| 6 | Sensor - |
| 7 | Sensor + |
| 8 | Senal sensor |
| 9 | Final de carrera |

PINES LIBRES
| PIN Arduino| Pin Shield |
| -------------- | --------- |
| 2 | Step pulse Y-Axis |
| 6 | Direction Y-Axis |
| 3 | Spindle direction |
| 13 | PaP Azul |
| A0 | Reser/Abort  |
| A1 | Feed Hold |

![](/img/handtemp-sketch.png)


## Versión de firmware estable

Fecha de lanzamiento: 18/11/2020

Última versión estable [handtemp_1.7](firmware/handtemp_1.7.ino).

### Características
* Posee método de watchDog para resetear el microcontrolador si este queda bloqueado.
* Descarta temperaturas fuera de un rango determinado.
* Establece conexión wifi para enviar datos utilizando protocolo mqtt.
* Posee dos modos de funcionamiento: Modo local y Modo Red (seleccionables mediante switch externo).
* Obtiene la fecha y hora (timeStamp) desde un servidor de reloj global.
* En caso de perder la conexión wifi/mqtt automáticamente se pone en modo local. Al reestablecerse la conexión, vuelve al modo red.
* Permite cambiar ciertos parámetros por puerto serie o bluetooth (utilizando serial port)
* Almacena una cantidad de lecturas en memoria interna EEPROM.
* Mide con 1 o 2 o 3 sensores IR y promedia las lecturas.
* Permite consultar las últimas lecturas.
* Permite escannear dispositivos i2C, grabar coeficiente de emisividad y modificar dirección del dispositivo.
* Graba la temperatura de Offset en memoria EEPROM.
* Señaliza visualmente mediante tira de leds Neopixel.
* Señaliza acústicamente mediante buzzer variando la frecuencia del beep de acuerdo a la distancia del objeto a medir
* Envía mensaje de keepAlive cada cierto tiempo para indicar que el cliente está vivo. 
* Código más ordenado y estructurado.
* Se aplica recta de calibración de temperatura.
* Se incorpora OTA.
* Maneja un solo broker.

## Versión de firmware en desarrollo

Fecha de inicio: 30/05/2021

Versión en desarrollo [handtemp_2.0](firmware/handtemp_1.8.ino).(revisar)

### Características
- [x] Access point (AP mode) para el ingreso de SSID y pass, por medio de un web service. Los datos son persistentes en la memoria y solo se activa el AP, cuando la SSID no esta disponible. (**LISTO PARA TEST**)

## Instrucciones de instalación
Para compilar y ejecutar el firmware en la ESP32 debemos seguir los siguientes pasos:

* Instalar un IDE para ver el código, compilarlo y correrlo en la ESP32. Podemos usar el [PlatformIO en Visual Studio Code](docs/PlatformIO-IDE.md) o el [Arduino IDE](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/).

* Instalar las librerías requeridas por el proyecto. Éstas son:

	* [Adafruit Bus IO](https://github.com/adafruit/Adafruit_BusIO)
	* [Adafruit SH1106](https://github.com/wonho-maker/Adafruit_SH1106)
	* [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)
	* [New Ping](https://bitbucket.org/teckel12/arduino-new-ping/downloads/NewPing_v1.9.1.zip)
	* [Arduino OTA](https://github.com/jandrassy/ArduinoOTA)
	* [PubSubClient](https://github.com/knolleary/pubsubclient/)
	* [Adafruit MLX90614](https://github.com/adafruit/Adafruit-MLX90614-Library)
	* [Arduino JSON](https://github.com/bblanchon/ArduinoJson)

	Las librerías deben colocarse en la carpeta **libraries** del proyecto de **VSCode** o instalarse desde el **Library Manager** de **Arduino IDE**. **Si se usa VSCode**, las librerías ya están incluidas dentro del proyecto subido al repositorio por lo que **no se requiere su instalación**.

* Descargar y agregar el código fuente desde el repositorio, según el IDE utilizado, desde [esta ubicación](../firmware)

* Compilar el código con el botón correspondiente al IDE utilizado.

## Documentación de soporte
* [Repositorio del proyecto (carcasa, esquematicos,ensayos,administrativo,etc).](https://drive.google.com/drive/folders/1y8OxduKYDoWpeUjtB7u9908cSnYStPmV?usp=sharing)
* [MCU ESP32.](/docs/ESP32.md)
* [Sensor de temperatura MLX90614.](/docs/SensorTemperatura.md)
* [Pantalla OLED SH1106.](/docs/OLED.md)
* [Sensor de proximidad HC-SR04.](/docs/SensorProximidad.md)
* [Buzzer.](/docs/Buzzer.md)
* [MQTT y WiFi.](/docs/MQTT-WiFi.md)

## Colaboraciones
Para trabajar en el proyecto respetar las siguientes [pautas](https://gist.github.com/maxiyommi/b5d2ef4c66f17870a92bc27f4afe71c2) definidas para un trabajo más ordenado. 

# Feature - BrokerPorBluetooth
Esta nueva característica le permitiría a un técnico configurar el broker al que tiene que conectarse el microcontrolador. Esto lo hace por medio de Bluetooth o a través del puerto serie.

## Uso

Para enviarle el nuevo broker, un técnico debe vincular un smartphone al microcontrolador a través de bluetooth. Luego, con la aplicación Serial Bluetooth Terminal se le envía un comando y el nombre del nuevo broker. Ejemplo: Kbroker.hivemq.com (siendo 'K' el comando y 'broker.hivemq.com' el broker). Recordar que solo debe enviarse LF (line feed) como Newline; esto se configura desde la aplicación.

## Tareas pendientes
- [x] integrarlo a la versión handtemp_1.9.
- [x] ordenar referencia.
- [x] push a remoto.
- [x] pensar como implementar la feature, integrar la rama (diff) y la mejor metodología para el trabajo colaborativo.
- [x] armar instructivo para que los usuario lo utilicen.
- [x] hacer test del feature.
