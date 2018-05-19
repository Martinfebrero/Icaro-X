#ifndef MANO_INCLUDED
#define MANO_INCLUDED

#include "PCA9685.h"

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

class Mano {
public:
  int8_t * buffer;
  uint8_t bufferLen;
  uint8_t angulos;
  uint8_t angulosLen;
  PCA9685 * servoController;
  PCA9685_ServoEvaluator* servoEvaluator;

  bool arrayIsEqual(int8_t& array1,int8_t& array2,uint8_t arrayLen );
  Mano(uint8_t * angulos,uint8_t angulosLen,
     int8_t * buffer,uint8_t bufferLen,
    PCA9685*,PCA9685_ServoEvaluator*);
  void execute();
};

#endif
