#include <controlador.h>

Controlador::Controlador(Sensor *sensores, uint8_t nSensores, Actuador *actuadores,
            uint8_t nActuadores) {
  this->sensores = sensores;
  this->actuadores = actuadores;
  this->nSensores = nSensores;
  this->nActuadores = nActuadores;
  lock = false;
};

void Controlador::activar() {
  this->changeLock(true);
  //this->sensores->lock();
  //this->actuadores->lock();
};

//void Controlador::sensar(){}

/*
void Controlador::set_timer(unsigned long ms){
  this->timer->stop();
  //this->timer->set(ms, sensar);
  this->timer->start();
};

void Controlador::start_timer(){
  this->timer->stop();
  this->timer->start();
}

void Controlador::stop_timer(){
  this->timer->stop();
}
*/

void Controlador::info() {
  if (!Serial){Serial.begin(9600);}
  Serial.println("\n\n* Controlador *\t**");
  Serial.print("sensores:\t");
  Serial.println(this->nSensores);
  Serial.print("actuadores:\t");
  Serial.println(this->nActuadores);
  Serial.print("Locked:\t");
  Serial.println(this->lock);
  Serial.print("* *\t* *\t*\n");
  Serial.flush();
};
