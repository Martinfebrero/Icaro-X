#include <Arduino.h>
//#include <Servo.h>
#include <Wire.h>
#include "PCA9685.h"
//#include "mano.h"

volatile int8_t buffer[4];
int8_t bufferAux[4]={0,0,0,0};

//Values para el mapper

int8_t bufferValue[4]={1,1,1,1};
int8_t bufferValue1[4]={2,1,1,1};
int8_t bufferValue2[4]={1,2,1,1};
int8_t bufferValue3[4]={1,1,2,1};

int8_t anglesValue[5]={0,0,0,0,0};
int8_t anglesValue1[5]={0,50,0,50,0};
int8_t anglesValue2[5]={50,0,50,0,50};
int8_t anglesValue3[5]={-60,0,60,0,-60};


int8_t* mapper [4] [2]={
  {bufferValue,anglesValue},
  {bufferValue1,anglesValue1},
  {bufferValue2,anglesValue2},
  {bufferValue3,anglesValue3}
};

void backupBuffer(){
  for(int i=0;i<4;i++){
    bufferAux[i]=buffer[i];
  }
}

bool arrayIsEqual(int8_t* array1, int8_t* array2, uint8_t arrayLen){
  for(uint8_t i=0;i<arrayLen;i++){
    if ((array1)[i]!=(array2)[i]) return false;
  }
  return true;
};

//Declaracion para el main
volatile bool datoFlag;

//    Servo Part  //

PCA9685 pwmController(Wire, PCA9685_PhaseBalancer_Weaved); // Library using Wire and weaved phase balancing scheme

// Linearly interpolates between standard 2.5%/12.5% phase length (102/512) for -90°/+90°
PCA9685_ServoEvaluator pwmServo1;

//      TIMER2 PART

volatile uint8_t timerCounter;
volatile bool timerFlag;
volatile bool timerMitadFlag;

void timerSet(){
  cli();
  //Inicio variables globales
  timerCounter=0;
  timerFlag=0;
  timerMitadFlag=0;

  TCCR2A = 0; //Limpio registro
  TCCR2B = 0; //Limpio registro
  TIMSK2 = 0; //Limpio registro
  TCNT2 = 0;  //Counter Value a 0.

  //Set CompareMatchRegister calculado para la freq requerida. (100 Hz)
  //OCR2A = 121;    //El valor 121 está calculado con el excel provisto.            // 155.25 = ((16*10^6) / (1024*100)) - 1 (must be <256)
  //OCR2A = 121;  // 2Hz
  //OCR2A = 244;  // 1Hz
  OCR2A = 81;   // 3HZ
  // turn on CTC mode
  TCCR2A |= (1 << WGM21);

  // Set CS2[2:0] bit for 1024 prescaler
  TCCR2B |= (1 << CS20) | (1 << CS21) | (1 << CS22);
sei();    //allow interrupts
};

void timerStart(){
  //cli();
  TIMSK2 |= (1 << OCIE2A);
  //sei();
};

void timerStop(){
  //cli();
  timerCounter=0;
  TIMSK2 &= ~(1<<OCIE2A);
  TCNT2 = 0;  //Limpio el valor del counter.
  //sei();
};

ISR(TIMER2_COMPA_vect){
  cli();
  if(timerCounter<64){
    timerCounter++;
    if(timerCounter==10){timerMitadFlag=true;}
  }  //El valor 64 está calculado.
  else if (timerCounter>=64){
    timerStop();
    timerFlag=true;
    digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
    timerCounter=0;
  }
  sei();
};




//        Pin Change Interrupts PART

#define DASH 2
#define DOT 1

volatile bool pinChangeState;
volatile uint8_t bufferIndex;

void PCI_init(byte pin){
  *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
    PCIFR  |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
    PCICR  |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group

    pinChangeState=false;
    for(int i=0;i<4;i++){buffer[i]=0;}  //Asigno valores al buffer para no leer basura.
    bufferIndex=0;                      //Índice del Buffer en 0;
};

void PCI_stop(byte pin){
  PCIFR  |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
  PCICR  |= !bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
};

volatile uint8_t contadorDeActivaciones=0;

ISR (PCINT2_vect) // handle pin change interrupt for D0 to D7 here
 {
   //cli();
   pinChangeState=digitalRead(2);  //Determino si el PinChange es FALLING o RISING, Sólo RISING inician un conteo.

   if(pinChangeState){timerStart();contadorDeActivaciones++;}
   else {
     timerStop();   //Freno timer para que no siga variando.
     if(timerMitadFlag){buffer[bufferIndex]=DOT;bufferIndex++;}
     if(timerFlag){buffer[bufferIndex-1]=DASH;}
     if(bufferIndex>=4){datoFlag=true;}

     timerMitadFlag=false;
     timerFlag=false;
   }
   //sei();
 };

 //         Buffer Interpreter PART

 void ejecutar(){
   backupBuffer();
   if (arrayIsEqual(bufferAux, bufferValue, 4)){
     for(int i=0;i<5;i++){
       pwmController.setChannelPWM(i , pwmServo1.pwmForAngle(anglesValue[i]));
     }
   }

   if (arrayIsEqual(bufferAux, bufferValue1, 4)){
     for(int i=0;i<5;i++){
       pwmController.setChannelPWM(i , pwmServo1.pwmForAngle(anglesValue1[i]));
     }
   }

   if (arrayIsEqual(bufferAux, bufferValue2, 4)){
     for(int i=0;i<5;i++){
       pwmController.setChannelPWM(i ,pwmServo1.pwmForAngle(anglesValue2[i]));
     }
   }

   if (arrayIsEqual(bufferAux, bufferValue3, 4)){
     for(int i=0;i<5;i++){
       pwmController.setChannelPWM(i , pwmServo1.pwmForAngle(anglesValue3[i]));
     }
   }
   delay(250);
 }

 void interpretBuffer(){
   backupBuffer();
   for (uint8_t mapperFila=0;mapperFila<4;mapperFila++){
     if(arrayIsEqual(bufferAux, mapper[mapperFila][0], 4)){
       for(uint8_t angleCount=0;angleCount<5;angleCount++){
         pwmController.setChannelPWM(angleCount, pwmServo1.pwmForAngle(*mapper[mapperFila][1]));
       }
     }
   }

 };

//          MAIN PART

int main(int argc, char const *argv[]) {
  init();   //Init de arduino
  pinMode(LED_BUILTIN,OUTPUT);  //Led de Arduino como indicador.

  Serial.begin(9600); //Inicio serial com.


  //    Pruebas de manejo de arrays
  /*
  int8_t bufferAux1[4];
  bufferAux1[0]=0;
  bufferAux1[1]=0;
  bufferAux1[2]=1;
  bufferAux1[3]=1;

  bufferAux[0]=0;
  bufferAux[1]=0;
  bufferAux[2]=1;
  bufferAux[3]=0;

  if (arrayIsEqual(bufferAux1, bufferAux, 4)){
    Serial.println("iguales");
  }
  else{Serial.println("no iguales");}

  buffer[0]=1;
  buffer[1]=1;
  buffer[2]=1;
  buffer[3]=1;

  backupBuffer();

  if (arrayIsEqual(buffer, bufferAux, 4)){
    Serial.println("iguales");
  }
  else{Serial.println("no iguales");}



  interpretBuffer();

  delay (1000);

  */

  datoFlag=false; //No tengo ningún dato cuando empieza.

  timerStop();
  timerSet();
  delay(1000);
  Serial.println("listo");
  Serial.flush();
  //timerStart();

  PCI_init(2);  //Inicio pin change en pin 2.

// Inicio de Servos
  Wire.begin();                      // Wire must be started first
  Wire.setClock(400000);             // Supported baud rates are 100kHz, 400kHz, and 1000kHz

  pwmController.resetDevices();       // Software resets all PCA9685 devices on Wire line

  pwmController.init(B000000);        // Address pins A5-A0 set to B000000
  pwmController.setPWMFrequency(50);  // 50Hz provides 20ms standard servo phase length

// fin de servos setup
  while (1) {
    if (datoFlag){
      //cli();
      digitalWrite(LED_BUILTIN, 0);
      Serial.println("buffer:");
      for (int i=0;i<4;i++){Serial.print(buffer[i]);} //Imprimo el buffer.
      Serial.println();

      Serial.print("activaciones: ");
      Serial.print(contadorDeActivaciones);
      Serial.println();

      ejecutar();

      //if(buffer[2]==1){pwmController.setAllChannelsPWM(pwmServo1.pwmForAngle(0));}
      //if(buffer[2]==2){pwmController.setAllChannelsPWM(pwmServo1.pwmForAngle(65));}
      //delay(200);

      datoFlag=0;
      buffer[0]=0;buffer[1]=0;buffer[2]=0;buffer[3]=0;
      bufferAux[0]=0;bufferAux[1]=0;bufferAux[2]=0;bufferAux[3]=0;

      //Serial.print("buffer: ");
      //for (int i=0;i<4;i++){Serial.print(buffer[i]);} //Imprimo el buffer limpio
      //Serial.println("\n****************");


      bufferIndex=0;
      Serial.flush();
      //sei();
    }
  }
  //return 0;
}
