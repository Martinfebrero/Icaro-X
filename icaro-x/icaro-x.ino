
#include <compat/deprecated.h>
#include <FlexiTimer2.h>

// All definitions
#define NUMCHANNELS 1
#define SAMPFREQ 256                      // ADC sampling rate 256
#define TIMER2VAL (1024/(SAMPFREQ))       // Set 256Hz sampling frequency                    

// Global constants and variables
volatile unsigned char CurrentCh;                       //Current channel being sampled.
volatile unsigned int ADC_Value = 0;                	  //ADC current value
volatile unsigned int ADC_Value_calibres[NUMCHANNELS];  // Valor cero de calibrado.

//~~~~~~~~~~
// Functions
//~~~~~~~~~~

void init_hardware_and_variables(){
  for (int i=0;i<NUMCHANNELS;i++){
    ADC_Value_calibres[i] = 0;         // Inicio el calibre de todos los canales en 0, arbitrario.
  }
}

void setup() {

  delay(5000);    //Delay en el inicio, por si el shield necesita "acomodarse".

 init_hardware_and_variables(); // Inicio del hardware.
 calibrar_canales();

 
 noInterrupts();
 FlexiTimer2::set(TIMER2VAL, Timer2_Overflow_ISR);
 FlexiTimer2::start();
 Serial.begin(57600);
 interrupts();          // Enable all interrupts after initialization has been completed
 }


void Timer2_Overflow_ISR()
{
  //Read the 6 ADC inputs and store current values in Packet
  for(CurrentCh=0;CurrentCh<NUMCHANNELS;CurrentCh++){
    ADC_Value = analogRead(CurrentCh);
    Serial.println(ADC_Value);  //AÑADIDO
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
  for(CurrentCh=0;CurrentCh<NUMCHANNELS;CurrentCh++){
    ADC_Value = analogRead(CurrentCh);
    Serial.println(ADC_Value_calibres[CurrentCh]);
    ADC_Value_calibres[CurrentCh] = ( ADC_Value_calibres[CurrentCh] + ADC_Value ) / 2; 
  }
}
