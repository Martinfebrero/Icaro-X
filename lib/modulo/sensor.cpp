#include <sensor.h>

Sensor::Sensor(uint8_t pin) : Modulo(pin) {
  this->lastM = 0;
  this->calibre = 0;
  this->limite_max = 0;
  this->limite_min = 0;
  this->rango = 0;
};

uint8_t Sensor::getBufferCount(){
  return this->bufferCount;
}

void Sensor::calibrar(void){

}

int Sensor::medir(void){
  this->lastM = analogRead(this->get_pin());
  this->buffer[bufferCount]=lastM;
  if(bufferCount<100){bufferCount++;}
  else {bufferCount=0;}
  return this->lastM;
};

void Sensor::medirToBuffer(void){
  buffer[bufferCount]=analogRead(get_pin());
  Serial.println(buffer[bufferCount]);
  Serial.flush();
  /*
  if(bufferCount<100){bufferCount++;}
  if (bufferCount==100){
    cli();
    uint32_t sumatoria=0;
    for(int i=0;i<100;i++){
      sumatoria=sumatoria+buffer[i];
    }
    sumatoria=sumatoria/100;
    Serial.println("sumatoria:");
    Serial.println(sumatoria);
    sei();
  }
  else if(bufferCount>100) {bufferCount=0;}
  */
};

void Sensor::setCalibreRango(uint16_t calibreVal, uint16_t rango) {
  this->calibre = calibreVal;
  this->rango = rango;
  this->limite_max = this->calibre + this->rango;
  this->limite_min = this->calibre - this->rango;
  if (this->calibre < 0) {
    this->calibre = 0;
    this->limite_min = 0;
    this->limite_max = this->calibre + this->rango;
  }
  if (this->limite_min <= 0) {
    this->limite_min = 0;
  }
};

void Sensor::info() {
  if (!Serial) {
    Serial.begin(9600);
  }
  Serial.print("\n* Sensor *\t*\n");
  Serial.print("Pin:\t");
  Serial.println(this->get_pin());
  Serial.print("Locked:\t");
  Serial.println(this->islocked());
  Serial.print("LastM:\t");
  Serial.println(this->lastM);
  Serial.print("Calibre:\t");
  Serial.println(this->calibre);
  Serial.print("Rango:\t\t");
  Serial.println(this->rango);
  Serial.print("Lim Max:\t");
  Serial.println(this->limite_max);
  Serial.print("Lim Min:\t");
  Serial.println(this->limite_min);
  Serial.print("bufferCount:\t");
  Serial.println(this->bufferCount);
  Serial.print("* *\t* *\t*\n");
  Serial.flush();
};
