
#include <compat/deprecated.h>
#include <FlexiTimer2.h>

// All definitions
#define NUMCHANNELS 1
#define SAMPFREQ 1000//256                      // ADC sampling rate 256
#define TIMER2VAL (1024/(SAMPFREQ))       // Set 256Hz sampling frequency
#define NSAMPLES  100                     // numero de samples a tomar antes de imprimir              

// Global constants and variables
volatile unsigned char currentCh;                       //Current channel being sampled.
volatile unsigned int adc_value = 0;                	  //ADC current value
volatile unsigned int adc_value_anterior = 0;
volatile unsigned int adc_value_calibres[NUMCHANNELS];  // Valor cero de calibrado.

volatile unsigned int cont = 0;

//~~~~~~~~~~
// Functions
//~~~~~~~~~~

void init_hardware_and_variables(){
  for (int i=0;i<NUMCHANNELS;i++){
    adc_value_calibres[i] = 300;         // Inicio el calibre de todos los canales en 0, arbitrario.
  }
}

void setup() {

  delay(500);    //Delay en el inicio, por si el shield necesita "acomodarse".

 init_hardware_and_variables(); // Inicio del hardware.
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
    adc_value = analogRead(currentCh);
    if (adc_value < adc_value_calibres[currentCh]){
      adc_value = adc_value_anterior;
    }

    //adc_value = (adc_value+adc_value_anterior)/2;

    
     if (cont >= NSAMPLES) {
      
      Serial.println((adc_value+adc_value_anterior)/2);cont = 0;
      }  //AÑADIDO
      
      adc_value = (adc_value+adc_value_anterior)/2;
      
     cont ++;
    adc_value_anterior = adc_value;
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
