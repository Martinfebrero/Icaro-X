#include "mano.h"

bool Mano::arrayIsEqual(int8_t& array1, int8_t& array2, uint8_t arrayLen){
  for(uint8_t i=0;i<arrayLen;i++){
    if ((&array1)[i]!=(&array2)[i]) return false;
  }
  return true;
};

Mano::Mano(uint8_t * angulos,uint8_t angulosLen,
  int8_t * buffer,uint8_t bufferLen,
  PCA9685*,PCA9685_ServoEvaluator*){
  this->buffer=buffer;
  this->bufferLen=bufferLen;
  this->servoController=servoController;
  this->servoEvaluator=servoEvaluator;
};

void Mano::execute(){
  for(uint8_t f=0;f<4;f++){
    if (arrayIsEqual(*this->buffer, *mapper[f][0], 4)){
      for(uint8_t i =0;i<this->angulosLen;i++){
        servoController->setChannelPWM(i, servoEvaluator->pwmForAngle(*mapper[f][i]));
      }
    }
  }

  //Tomo los datos y los convierto en sus valores correspondientes en ángulos para los distintos servos.
  //procesarBuffer(); Sería por ejemplo la función para hacer la conversión.
};





/*
class Mano {
public:
  volatile uint8_t * buffer;
  uint8_t bufferLen;
  PCA9685 * servoController;
  PCA9685_ServoEvaluator* servoEvaluator;

  Mano(volatile uint8_t * buffer,uint8_t bufferLen,PCA9685*,PCA9685_ServoEvaluator*);
  void execute();

};
*/
