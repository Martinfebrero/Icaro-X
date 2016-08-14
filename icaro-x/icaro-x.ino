#include <Servo.h>

#include <compat/deprecated.h>
#include <FlexiTimer2.h>

// All definitions
#define NUMCHANNELS 1
#define SAMPFREQ 1000//256                      // ADC sampling rate 256
#define TIMER2VAL (1024/(SAMPFREQ))       // Set 256Hz sampling frequency
#define NSAMPLES  100                     // numero de samples a tomar antes de imprimir
unsigned int SERVO = 8;

#define APAGADO 0   //Definicion para sensor apagado
#define ENCENDIDO 1 // Definicion para sensor encendido

#define UMBRAL 250

/*********/
Servo MiServo;
/*********/

// Global constants and variables
volatile unsigned char currentCh;                       //Current channel being sampled.
volatile unsigned int adc_value = 0;                	  //ADC current value
volatile unsigned int adc_value_anterior = 0;
volatile unsigned int adc_value_calibres[NUMCHANNELS];  // Valor cero de calibrado.

volatile unsigned int sumador_datos = 0;

volatile unsigned int cont = 0;

volatile unsigned int servo_flag = APAGADO;

//~~~~~~~~~~
// Functions
//~~~~~~~~~~

void init_hardware_and_variables(){
  for (int i=0;i<NUMCHANNELS;i++){
    adc_value_calibres[i] = 300;         // Inicio el calibre de todos los canales en 0, arbitrario.
  }

  //pinMode(SERVO,OUTPUT);
}

void setup() {

  delay(500);    //Delay en el inicio, por si el shield necesita "acomodarse".

 init_hardware_and_variables(); // Inicio del hardware.

 /*********/
 MiServo.attach(8);
 /*********/

 
 calibrar_canales();

 
 noInterrupts();

 Serial.begin(9600);//57600

 // Muestro el calibre para ver la línea.
 for (int i = 0; i<500; i++){
  Serial.println(adc_value_calibres[0]);
 }



 FlexiTimer2::set(TIMER2VAL, Timer2_Overflow_ISR);
 FlexiTimer2::start();
 interrupts();          // Enable all interrupts after initialization has been completed
 }


void Timer2_Overflow_ISR()
{
  //Read the 6 ADC inputs and store current values in Packet
  for(currentCh=0;currentCh<NUMCHANNELS;currentCh++){

    adc_value = analogRead(currentCh);      // Leo el valor de un canal.

    if (adc_value < adc_value_calibres[currentCh]){   //elimino valores debajo del calibre
      adc_value = (2 * adc_value_calibres[currentCh]) - adc_value ;
    }

    if (adc_value <= (adc_value_calibres[currentCh] + UMBRAL)) {adc_value = APAGADO;}

    sumador_datos = sumador_datos + adc_value;

    if (cont >= NSAMPLES){
      unsigned int aux = sumador_datos / NSAMPLES;
      Serial.println((aux));   // Imprimo la integral


      MiServo.write( map(aux,0,1023,0,179));


      analogWrite(SERVO,map(aux, 0, 1023, 0, 180));
      //analogWrite(SERVO,aux);   // Para escritura analogica
      /*
      if (aux>0) { digitalWrite(SERVO,HIGH);}
      else if (aux<=0) { digitalWrite(SERVO,LOW);}
      */
      
      cont = 0;
      sumador_datos = 0;
    }

    cont++;

  }
}

void loop() {
 __asm__ __volatile__ ("sleep");
}

void calibrar_canales(){
  noInterrupts();
  FlexiTimer2::set(TIMER2VAL, Timer2_Overflow_Calibre_ISR);
  FlexiTimer2::start();
  interrupts();  // Dejo espacio al tiempo de calibre, después sigo con el resto
  delay(5000); // 5 segundos para calibrar, se puede calcular un valor de tiempo determinado para optimizar.
  noInterrupts();
  FlexiTimer2::stop();
  interrupts();
}

void Timer2_Overflow_Calibre_ISR(){
  for(currentCh=0;currentCh<NUMCHANNELS;currentCh++){
    adc_value = analogRead(currentCh);
    Serial.println(adc_value_calibres[currentCh]);
    adc_value_calibres[currentCh] = ( adc_value_calibres[currentCh] + adc_value ) / 2; 
  }
}
