# regador_robotizado
Regador robotizado para incorporar al laboratorio inteligente de la FIE

/****************MODIFICAR TODO LO QUE SIGUE PARA ADAPTARLO A ESTE PROYECTO***************/

# Medidor de temperatura corporal - Handtemp
Repositorio técnico del proyecto, con toda la información referida al hardware documentación de las tecnologías utilizadas. El mismo contiene las actualizaciones del firmware y el conexionado.

## Conexionado
EL sketch de Fritzing se encuentra en la carpeta del mismo nombre, dentro de /docs. Ahí también hay una carpeta *parts* cuyo contenido son componentes que se deben agregar manualmente al proyecto. Éstos se copian dentro de la carpeta que Fritzing nos asigna para guardar componentes, generalmente es *Documentos/Fritzing/parts*.

| Pin Componente | Pin ESP32 |
| -------------- | --------- |
| MLX-90614 Vin | 3V3 |
| MLX-90614 GND | GND |
| MLX-90614 SCL | GPIO 22 |
| MLX-90614 SDA | GPIO 21 |
| Buzzer SIG | GPIO 32 |
| Buzzer VCC | Vin |
| Buzzer GND | GND |
| OLED GND | GND |
| OLED VCC | 3V3 |
| OLED CLK | GPIO 14 |
| OLED MOSI | GPIO 13 |
| OLED RES | GPIO 26 |
| OLED DC | GPIO 27 |
| OLED CS | GPIO 15 |
| HC-SR04 Vcc | Vin |
| HC-SR04 TRIG | GPIO 17 |
| HC-SR04 ECHO | GPIO 16 |
| HC-SR04 GND | GND |

| Adicionales | Pin ESP32 |
| -------------- | --------- |
| Led Distancia OK | 25 |
| Led Fiebre Neopixel| 33 |
| Switch de MODO| 34 |
| Switch de Config Wifi| 35 |

Para ver el [pinout de la ESP32](/docs/ESP32.md)

![](/img/handtemp-sketch.png)
