//CONTROL - REGADOR AUTOMÁTICO (activado por pulsador)- 
//Revisar los valores ajustables para el correcto funcionamiento
//-----------------------------------------------------------------------------------------------------------

//DECLARACIÓN DE ETIQUETAS, VARIABLES Y FUNCIONES

#define PIN_PAP_ENA 8             //Pin para habilitar la etapa de potencia de los motores PaP 
#define PIN_PAP_X_DIR 5           //Pin para manejar el sentido de giro del motor del eje x
#define PIN_PAP_X_STEP 2          //Pin para enviar pulsos al motor del eje x
#define PIN_PAP_Z_DIR 7           //Pin para manejar el sentido de giro del motor del eje z
#define PIN_PAP_Z_STEP 4          //Pin para enviar pulsos al motor del eje z
#define PIN_ENDSTOP_X 9           //Pin del fin de carrera del eje x - Conexión final de carrera del eje X (X-)
#define PIN_ENDSTOP_Z 11          //Pin del fin de carrera del eje z - Conexión final de carrera del eje Z (Z-)
#define PIN_BOTON 10              //Pin del botón - Entrada boton (Y-)
#define PIN_BOMBA 12              //Pin para accionamiento del relay que enciende la bomba - Salida bomba de agua (SpindleEna)
#define PIN_SENSOR A3             //Pin del sensor de humedad - Entrada sensor de humedad (CoolEn)

#define PAP_X_DIR_ADELANTE 1
#define PAP_Z_DIR_ADELANTE 1

#define DELAY_HOMING 600                //Delay entre paso y paso para ir a HOME
#define DELAY_DEBOUNCE_ENDSTOP 50       //Delay para evitar efecto rebote en final de carrera
#define DELAY_DEBOUNCE_BOTON 100        //Delay para evitar efecto rebote en BOTON
#define DELAY_MOVIMIENTO_X 600          //Delay entre pasos para movimiento del eje x
#define DELAY_MOVIMIENTO_Z 600          //Delay entre pasos para movimiento del eje z

#define HUMEDAD_SECO 504                //Valores de referencia para la medición del porcentaje de humedad           
#define HUMEDAD_MOJADO 247
#define CANTIDAD_MEDICIONES 8
#define PORCENTAJE_RIEGO 20             //Porcentaje de humedad para el cual se activa la bomba
#define DELAY_BOMBA_MAX 3000            //Tiempo maximo de activación de la bomba en ms

#define BOTON_ENCENDIDO 0               //Estado del botón cuando está presionado
#define BOMBA_ENCENDIDO 1               //Estado de la bomba cuando está activada
#define ENDSTOP_ENCENDIDO 0             //Estado del endstop cuando está activado

int cant_macetas = 5;                       //Defino la cantidad de macetas
int distancia_entre_macetas = 300;          //Distancia entre macetas en mm
int distancia_sensor = 150;                 //Recorrido el sensor en mm
int posicion_x = 0;                         //Guardo la posición del carro en el eje x
int posicion_z = 0;                         //Guardo la posición del carro en el eje z
float pasos_x_mm_eje_x = 20;                //Cantidad de pasos por mm en el eje x
float pasos_x_mm_eje_z = 50;                //Cantidad de pasos por mm en el eje Z

//ATENCIÓN: Verificar siempre que la cantidad de macetas multiplicado por la distancia entre macetas
//no supere en largo del eje x

//-----------------------------------------------------------------------------------------------------------

void setup()                                //Función de seteo, el programa pasa una única vez por acá
{
  pinMode(PIN_BOMBA, OUTPUT);               //Declaro el pin de bomba como salida
  pinMode(PIN_PAP_X_STEP,OUTPUT);
  pinMode(PIN_PAP_X_DIR,OUTPUT);
  pinMode(PIN_PAP_Z_STEP,OUTPUT);
  pinMode(PIN_PAP_Z_DIR,OUTPUT);
  pinMode(PIN_PAP_ENA,OUTPUT);

  digitalWrite(PIN_BOMBA, !BOMBA_ENCENDIDO);        //Inicio con la bomba apagada
  digitalWrite(PIN_PAP_Z_STEP, LOW);
  digitalWrite(PIN_PAP_Z_DIR, LOW);
  digitalWrite(PIN_PAP_X_STEP, LOW);
  digitalWrite(PIN_PAP_X_DIR, LOW);
  digitalWrite(PIN_PAP_ENA, LOW);                   //Habilito los motores (lógica inversa)

  pinMode(PIN_BOTON, INPUT_PULLUP);                 //Declaro el pin del boton como entrada en estado alto cuando el botón no está presionado
  pinMode(PIN_ENDSTOP_Z, INPUT_PULLUP);             //Idem boton
  pinMode(PIN_ENDSTOP_X, INPUT_PULLUP);             //Idem boton

  Serial.begin(9600);                               //Incializo el puerto serie para funciones de debuging
}

void loop()                                         //Función principal, void significa que no devuelve valores
{
  go_home();                                        //Voy a la posición de home en los dos ejes
  if(digitalRead(PIN_BOTON) == BOTON_ENCENDIDO)     //Leo estado del pin y pregunto si el boton esta apretado. (funciona con lógica inversa)
  { 
    delay(DELAY_DEBOUNCE_BOTON);                  
    digitalWrite(PIN_PAP_ENA,LOW);                  //Habilito el driver
    for(int i=1; i <= cant_macetas; i++)            //Esta estructura realizará las mediciones en las posiciones definidas
    {
      mover_eje_x();
      posicion_x = i * distancia_entre_macetas;     //Guardo la nueva posición sobre el eje x
      mover_eje_z();
      medicion_y_riego();
      mover_eje_z();
    }
    go_home();
  }
}

void go_home()
{
  int detecteX = 0, detecteZ = 0;                    //Defino flags para indicar que los endstop están activos

  digitalWrite(PIN_PAP_ENA, LOW);
  digitalWrite(PIN_PAP_X_DIR, !PAP_X_DIR_ADELANTE);  //! cambia estado, si era 0 pasa a 1, si era 1 pasa a 0
  digitalWrite(PIN_PAP_Z_DIR, !PAP_Z_DIR_ADELANTE);

  do 
  {                                                
    if(digitalRead(PIN_ENDSTOP_Z) == ENDSTOP_ENCENDIDO)            //Si está presionado el endstop del eje z
    {
      delay(DELAY_DEBOUNCE_ENDSTOP);
      if(digitalRead(PIN_ENDSTOP_Z) == ENDSTOP_ENCENDIDO)          //Repregunto por el efecto rebote en el pulsador
       detecteZ = 1;                                               //Si efectivamente esta presionado llegué a home en este eje
    } 
    else                                                           //Si aún no llegué
    {
      digitalWrite(PIN_PAP_Z_STEP, HIGH);                          //Avanzo un paso hacia home en el eje z
      delayMicroseconds(DELAY_HOMING);
      digitalWrite(PIN_PAP_Z_STEP, LOW);
      delayMicroseconds(DELAY_HOMING);
    }
  } 
  while(detecteZ == 0);                                            //Repite las instrucciones del do hasta llegar al endtop
  
  do                                                               //Misma estructura pero para el eje x
  {                                               
    if(digitalRead(PIN_ENDSTOP_X) == ENDSTOP_ENCENDIDO) 
    {
        delay(DELAY_DEBOUNCE_ENDSTOP);
        if (digitalRead(PIN_ENDSTOP_X) == ENDSTOP_ENCENDIDO) 
        detecteX = 1;
    } 
    else 
    {
      digitalWrite(PIN_PAP_X_STEP, HIGH);
      delayMicroseconds(DELAY_HOMING);
      digitalWrite(PIN_PAP_X_STEP, LOW);
      delayMicroseconds(DELAY_HOMING);
    }
  } 
  while(detecteX == 0);
    
  digitalWrite(PIN_PAP_ENA, HIGH);              //Desabilito los motores

  posicion_x = 0;                               //La posición en el eje x ahora es cero
  posicion_z = 0;                               //La posición en el eje z ahora es cero
}

void mover_eje_x()
{
  long pasos_x = 0;
  digitalWrite(PIN_PAP_X_DIR, PAP_X_DIR_ADELANTE);             //Seteo el moviemiento del eje x hacia adelante
  pasos_x = distancia_entre_macetas * pasos_x_mm_eje_x;        //Calculo los pasos que debe girar el motor para que el carro recorra la distancia entre macetas
  for(int j=0 ; j < pasos_x ; j++)                             //Avanzo la cantidad de pasos calculada                              
  {
    digitalWrite(PIN_PAP_X_STEP, HIGH);
    delayMicroseconds(DELAY_MOVIMIENTO_X);
    digitalWrite(PIN_PAP_X_STEP, LOW);
    delayMicroseconds(DELAY_MOVIMIENTO_X);
  }
}

void mover_eje_z()
{
  long pasos_z = 0, detecteZ = 0;
  if(posicion_z == 0)
  {
    digitalWrite(PIN_PAP_Z_DIR, PAP_Z_DIR_ADELANTE);         //Seteo el movimiento del eje z hacia adelante (abajo)
    pasos_z = distancia_sensor * pasos_x_mm_eje_z;           //Calculo los pasos que debe girar el motor para que el carro recorra la distancia entre que debe bajar el sensor //Avanzo la cantidad de pasos calculada                              
    for(int w=0; w < pasos_z; w++)
    {
      digitalWrite(PIN_PAP_Z_STEP, HIGH);
      delayMicroseconds(DELAY_MOVIMIENTO_Z);
      digitalWrite(PIN_PAP_Z_STEP, LOW);
      delayMicroseconds(DELAY_MOVIMIENTO_Z);
    }
    posicion_z = distancia_sensor;
  }
  
  else
  {
    digitalWrite(PIN_PAP_Z_DIR, !PAP_Z_DIR_ADELANTE);                      //Seteo el movimiento del eje z hacia atras (arriba)
    do
    {                                                
      if(digitalRead(PIN_ENDSTOP_Z) == ENDSTOP_ENCENDIDO)                  //Si está presionado el endstop del eje z
      {
        delay(DELAY_DEBOUNCE_ENDSTOP);
        if(digitalRead(PIN_ENDSTOP_Z) == ENDSTOP_ENCENDIDO)                //Repregunto por el efecto rebote en el pulsador
        detecteZ = 1;                                                      //Si efectivamente esta presionado llegué a home en este eje
      } 
      else                                                                 //Si aún no llegué
      {
        digitalWrite(PIN_PAP_Z_STEP, HIGH);                                //Avanzo un paso hacia home en el eje z
        delayMicroseconds(DELAY_HOMING);
        digitalWrite(PIN_PAP_Z_STEP, LOW);
        delayMicroseconds(DELAY_HOMING);
      }
    } 
    while(detecteZ == 0);                                                  //Repite las instrucciones del do hasta llegar al endtop
    posicion_z = 0;    
  }
}

void medicion_y_riego()
{
  int hum_porcentaje = 0, valor_adc, valor_humedad, tiempo_apertura;

  // podriamos no calcular porcentaje y trabajar solo con mapeo de tiempo de apertura?
  for(int z = 0; z < CANTIDAD_MEDICIONES; z++)                                  //Realizamos varias mediciones y efectuamos un promedio del porcentaje de humedad
  { 
    valor_adc = analogRead(PIN_SENSOR);                                         //Hacemos la lectura analógica y la guardamos en una variable
    valor_humedad = map(valor_adc, HUMEDAD_SECO, HUMEDAD_MOJADO, 0, 100);       //Mapeamos el valor medido para obtener un porcentaje
    if (valor_humedad > 100) valor_humedad = 100;
    if (valor_humedad < 0) valor_humedad = 0;
    hum_porcentaje += valor_humedad;                                            //NO es porcentaje todavia
  }
  hum_porcentaje = hum_porcentaje/CANTIDAD_MEDICIONES;                          //Calculamos porcentaje

  if(hum_porcentaje < PORCENTAJE_RIEGO)                                         //Si el porcentaje de humedad es menor al límite preestablecido
  {
    digitalWrite(PIN_BOMBA,BOMBA_ENCENDIDO);                                    //Activa la bomba durante un tiempo
    tiempo_apertura = map(hum_porcentaje, 100, 0, 0, DELAY_BOMBA_MAX);          //Mapeamos el tiempo de apertura en funcion de valores max (vaso de agua) y min (aire) de porcentaje de humedad
    delay(tiempo_apertura);
    digitalWrite(PIN_BOMBA,!BOMBA_ENCENDIDO);
  }
}
